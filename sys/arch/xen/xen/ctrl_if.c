/*	$NetBSD: ctrl_if.c,v 1.2 2005/03/09 22:39:21 bouyer Exp $	*/

/******************************************************************************
 * ctrl_if.c
 * 
 * Management functions for special interface to the domain controller.
 * 
 * Copyright (c) 2004, K A Fraser
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: ctrl_if.c,v 1.2 2005/03/09 22:39:21 bouyer Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/malloc.h>

#include <machine/xen.h>
#include <machine/hypervisor.h>
#include <machine/ctrl_if.h>
#include <machine/evtchn.h>

void printk(char *, ...);
#if 0
#define DPRINTK(_f, _a...) printk("(file=%s, line=%d) " _f, \
                           __FILE__ , __LINE__ , ## _a )
#else
#define DPRINTK(_f, _a...) ((void)0)
#endif

/*
 * Only used by initial domain which must create its own control-interface
 * event channel. This value is picked up by the user-space domain controller
 * via an ioctl.
 */
int initdom_ctrlif_domcontroller_port = -1;

/* static */ int ctrl_if_evtchn = -1;
static int ctrl_if_irq;
static struct simplelock ctrl_if_lock;

static CONTROL_RING_IDX ctrl_if_tx_resp_cons;
static CONTROL_RING_IDX ctrl_if_rx_req_cons;

/* Incoming message requests. */
    /* Primary message type -> message handler. */
static ctrl_msg_handler_t ctrl_if_rxmsg_handler[256];
    /* Primary message type -> callback in process context? */
static unsigned long ctrl_if_rxmsg_blocking_context[256/sizeof(unsigned long)];
#if 0
    /* Is it late enough during bootstrap to use schedule_task()? */
static int safe_to_schedule_task;
#endif
    /* Queue up messages to be handled in process context. */
static ctrl_msg_t ctrl_if_rxmsg_deferred[CONTROL_RING_SIZE];
static CONTROL_RING_IDX ctrl_if_rxmsg_deferred_prod;
static CONTROL_RING_IDX ctrl_if_rxmsg_deferred_cons;

/* Incoming message responses: message identifier -> message handler/id. */
static struct {
    ctrl_msg_handler_t fn;
    unsigned long      id;
} ctrl_if_txmsg_id_mapping[CONTROL_RING_SIZE];

/* For received messages that must be deferred to process context. */
static void __ctrl_if_rxmsg_deferred(void *unused);

#ifdef notyet
/* Deferred callbacks for people waiting for space in the transmit ring. */
static int DECLARE_TASK_QUEUE(ctrl_if_tx_tq);
#endif

static void *ctrl_if_softintr = NULL;

static int ctrl_if_tx_wait;
static void __ctrl_if_tx_tasklet(unsigned long data);

static void __ctrl_if_rx_tasklet(unsigned long data);

#define get_ctrl_if() ((control_if_t *)((char *)HYPERVISOR_shared_info + 2048))
#define TX_FULL(_c)   \
    (((_c)->tx_req_prod - ctrl_if_tx_resp_cons) == CONTROL_RING_SIZE)

static void ctrl_if_notify_controller(void)
{
    hypervisor_notify_via_evtchn(ctrl_if_evtchn);
}

static void ctrl_if_rxmsg_default_handler(ctrl_msg_t *msg, unsigned long id)
{
    msg->length = 0;
    ctrl_if_send_response(msg);
}

static void __ctrl_if_tx_tasklet(unsigned long data)
{
    control_if_t *ctrl_if = get_ctrl_if();
    ctrl_msg_t   *msg;
    int           was_full = TX_FULL(ctrl_if);
    CONTROL_RING_IDX rp;

    rp = ctrl_if->tx_resp_prod;
    __insn_barrier(); /* Ensure we see all requests up to 'rp'. */

    while ( ctrl_if_tx_resp_cons != rp )
    {
        msg = &ctrl_if->tx_ring[MASK_CONTROL_IDX(ctrl_if_tx_resp_cons)];

        DPRINTK("Rx-Rsp %u/%u :: %d/%d\n", 
                ctrl_if_tx_resp_cons,
                ctrl_if->tx_resp_prod,
                msg->type, msg->subtype);

        /* Execute the callback handler, if one was specified. */
        if ( msg->id != 0xFF )
        {
            (*ctrl_if_txmsg_id_mapping[msg->id].fn)(
                msg, ctrl_if_txmsg_id_mapping[msg->id].id);
            __insn_barrier(); /* Execute, /then/ free. */
            ctrl_if_txmsg_id_mapping[msg->id].fn = NULL;
        }

        /*
         * Step over the message in the ring /after/ finishing reading it. As 
         * soon as the index is updated then the message may get blown away.
         */
        __insn_barrier();
        ctrl_if_tx_resp_cons++;
    }

    if ( was_full && !TX_FULL(ctrl_if) )
    {
        wakeup(&ctrl_if_tx_wait);
#ifdef notyet
        run_task_queue(&ctrl_if_tx_tq);
#endif
    }
}

static void __ctrl_if_rxmsg_deferred(void *unused)
{
	ctrl_msg_t *msg;
	CONTROL_RING_IDX dp;

	dp = ctrl_if_rxmsg_deferred_prod;
	__insn_barrier(); /* Ensure we see all deferred requests up to 'dp'. */

	while ( ctrl_if_rxmsg_deferred_cons != dp )
	{
		msg = &ctrl_if_rxmsg_deferred[
		    MASK_CONTROL_IDX(ctrl_if_rxmsg_deferred_cons)];
		(*ctrl_if_rxmsg_handler[msg->type])(msg, 0);
		ctrl_if_rxmsg_deferred_cons++;
	}
}

static void __ctrl_if_rx_tasklet(unsigned long data)
{
    control_if_t *ctrl_if = get_ctrl_if();
    ctrl_msg_t    msg, *pmsg;
    CONTROL_RING_IDX rp, dp;

    dp = ctrl_if_rxmsg_deferred_prod;
    rp = ctrl_if->rx_req_prod;
    __insn_barrier(); /* Ensure we see all requests up to 'rp'. */

    while ( ctrl_if_rx_req_cons != rp )
    {
        pmsg = &ctrl_if->rx_ring[MASK_CONTROL_IDX(ctrl_if_rx_req_cons)];
        memcpy(&msg, pmsg, offsetof(ctrl_msg_t, msg));

        DPRINTK("Rx-Req %u/%u :: %d/%d\n", 
                ctrl_if_rx_req_cons-1,
                ctrl_if->rx_req_prod,
                msg.type, msg.subtype);

        if ( msg.length != 0 )
            memcpy(msg.msg, pmsg->msg, msg.length);

        if ( x86_atomic_test_bit(
                      (unsigned long *)&ctrl_if_rxmsg_blocking_context,
		      msg.type) )
            memcpy(&ctrl_if_rxmsg_deferred[MASK_CONTROL_IDX(dp++)],
                   &msg, offsetof(ctrl_msg_t, msg) + msg.length);
        else
            (*ctrl_if_rxmsg_handler[msg.type])(&msg, 0);

	ctrl_if_rx_req_cons++;
    }

    if ( dp != ctrl_if_rxmsg_deferred_prod )
    {
        __insn_barrier();
        ctrl_if_rxmsg_deferred_prod = dp;
	if (ctrl_if_softintr)
		softintr_schedule(ctrl_if_softintr);
    }
}

static int ctrl_if_interrupt(void *arg)
{
	control_if_t *ctrl_if = get_ctrl_if();

	if ( ctrl_if_tx_resp_cons != ctrl_if->tx_resp_prod )
		__ctrl_if_tx_tasklet(0);

	if ( ctrl_if_rx_req_cons != ctrl_if->rx_req_prod )
		__ctrl_if_rx_tasklet(0);

	return 0;
}

int
ctrl_if_send_message_noblock(
    ctrl_msg_t *msg, 
    ctrl_msg_handler_t hnd,
    unsigned long id)
{
    control_if_t *ctrl_if = get_ctrl_if();
    unsigned long flags;
    int           i;
    int s;

    save_and_cli(flags);
    simple_lock(&ctrl_if_lock);

    if ( TX_FULL(ctrl_if) )
    {
        simple_unlock(&ctrl_if_lock);
	restore_flags(flags);
	s = splhigh();
	if ( ctrl_if_tx_resp_cons != ctrl_if->tx_resp_prod )
		__ctrl_if_tx_tasklet(0);
	splx(s);
        return EAGAIN;
    }

    msg->id = 0xFF;
    if ( hnd != NULL )
    {
        for ( i = 0; ctrl_if_txmsg_id_mapping[i].fn != NULL; i++ )
            continue;
        ctrl_if_txmsg_id_mapping[i].fn = hnd;
        ctrl_if_txmsg_id_mapping[i].id = id;
        msg->id = i;
    }

    DPRINTK("Tx-Req %u/%u :: %d/%d\n", 
            ctrl_if->tx_req_prod, 
            ctrl_if_tx_resp_cons,
            msg->type, msg->subtype);

    memcpy(&ctrl_if->tx_ring[MASK_CONTROL_IDX(ctrl_if->tx_req_prod)], 
           msg, sizeof(*msg));
    __insn_barrier(); /* Write the message before letting the controller peek at it. */
    ctrl_if->tx_req_prod++;

    simple_unlock(&ctrl_if_lock);
    restore_flags(flags);

    ctrl_if_notify_controller();

    return 0;
}

int
ctrl_if_send_message_block(
    ctrl_msg_t *msg, 
    ctrl_msg_handler_t hnd, 
    unsigned long id,
    long wait_state)
{
	int rc;

	while ((rc = ctrl_if_send_message_noblock(msg, hnd, id)) == EAGAIN) {
		/* XXXcl possible race -> add a lock and ltsleep */
#if 1
		HYPERVISOR_yield();
#else
		rc = tsleep((caddr_t) &ctrl_if_tx_wait, PUSER | PCATCH,
		    "ctrl_if", 0);
		if (rc)
			break;
#endif
	}

	return rc;
}

/* Allow a reponse-callback handler to find context of a blocked requester.  */
struct rsp_wait {
    ctrl_msg_t         *msg;  /* Buffer for the response message.            */
    struct task_struct *task; /* The task that is blocked on the response.   */
    int                 done; /* Indicate to 'task' that response is rcv'ed. */
};

static void __ctrl_if_get_response(ctrl_msg_t *msg, unsigned long id)
{
    struct rsp_wait    *wait = (struct rsp_wait *)id;

    memcpy(wait->msg, msg, sizeof(*msg));
    __insn_barrier();
    wait->done = 1;

    wakeup(wait);
}

int
ctrl_if_send_message_and_get_response(
    ctrl_msg_t *msg, 
    ctrl_msg_t *rmsg,
    long wait_state)
{
    struct rsp_wait wait;
    int rc;

    wait.msg  = rmsg;
    wait.done = 0;

    if ( (rc = ctrl_if_send_message_block(msg, __ctrl_if_get_response,
                                          (unsigned long)&wait,
                                          wait_state)) != 0 )
        return rc;

    for ( ; ; )
    {
	    if ( wait.done )
		    break;
	    tsleep((caddr_t)&wait, PUSER | PCATCH, "ctrl_if", 0);
    }

    return 0;
}

#ifdef notyet
int
ctrl_if_enqueue_space_callback(
    struct tq_struct *task)
{
    control_if_t *ctrl_if = get_ctrl_if();

    /* Fast path. */
    if ( !TX_FULL(ctrl_if) )
        return 0;

    (void)queue_task(task, &ctrl_if_tx_tq);

    /*
     * We may race execution of the task queue, so return re-checked status. If
     * the task is not executed despite the ring being non-full then we will
     * certainly return 'not full'.
     */
    __insn_barrier();
    return TX_FULL(ctrl_if);
}
#endif

void
ctrl_if_send_response(
    ctrl_msg_t *msg)
{
    control_if_t *ctrl_if = get_ctrl_if();
    unsigned long flags;
    ctrl_msg_t   *dmsg;

    /*
     * NB. The response may the original request message, modified in-place.
     * In this situation we may have src==dst, so no copying is required.
     */
    save_and_cli(flags);
    simple_lock(&ctrl_if_lock);

    DPRINTK("Tx-Rsp %u :: %d/%d\n", 
            ctrl_if->rx_resp_prod, 
            msg->type, msg->subtype);

    dmsg = &ctrl_if->rx_ring[MASK_CONTROL_IDX(ctrl_if->rx_resp_prod)];
    if ( dmsg != msg )
        memcpy(dmsg, msg, sizeof(*msg));

    __insn_barrier(); /* Write the message before letting the controller peek at it. */
    ctrl_if->rx_resp_prod++;

    simple_unlock(&ctrl_if_lock);
    restore_flags(flags);

    ctrl_if_notify_controller();
}

int
ctrl_if_register_receiver(
    uint8_t type, 
    ctrl_msg_handler_t hnd, 
    unsigned int flags)
{
    unsigned long _flags;
    int inuse;

    save_and_cli(_flags);
    simple_lock(&ctrl_if_lock);

    inuse = (ctrl_if_rxmsg_handler[type] != ctrl_if_rxmsg_default_handler);

    if ( inuse )
    {
        printf("Receiver %p already established for control "
               "messages of type %d.\n", ctrl_if_rxmsg_handler[type], type);
    }
    else
    {
        ctrl_if_rxmsg_handler[type] = hnd;
        x86_atomic_clear_bit((unsigned long *)&ctrl_if_rxmsg_blocking_context, type);
        if ( flags == CALLBACK_IN_BLOCKING_CONTEXT )
        {
            x86_atomic_set_bit((unsigned long *)&ctrl_if_rxmsg_blocking_context, type);
#if 0
            if ( !safe_to_schedule_task )
                BUG();
#endif
        }
    }

    simple_unlock(&ctrl_if_lock);
    restore_flags(_flags);

    return !inuse;
}

void 
ctrl_if_unregister_receiver(
    uint8_t type,
    ctrl_msg_handler_t hnd)
{
    unsigned long flags;

    save_and_cli(flags);
    simple_lock(&ctrl_if_lock);

    if ( ctrl_if_rxmsg_handler[type] != hnd )
        printf("Receiver %p is not registered for control "
               "messages of type %d.\n", hnd, type);
    else
        ctrl_if_rxmsg_handler[type] = ctrl_if_rxmsg_default_handler;

    simple_unlock(&ctrl_if_lock);
    restore_flags(flags);

    /* Ensure that @hnd will not be executed after this function returns. */
    if (ctrl_if_softintr)
	    softintr_schedule(ctrl_if_softintr);
}

static void
ctrl_if_softintr_handler(void *arg)
{

	if ( ctrl_if_rxmsg_deferred_cons != ctrl_if_rxmsg_deferred_prod )
		__ctrl_if_rxmsg_deferred(NULL);
}

#ifdef notyet
void ctrl_if_suspend(void)
{
    free_irq(ctrl_if_irq, NULL);
    unbind_evtchn_from_irq(ctrl_if_evtchn);
}
#endif

void ctrl_if_resume(void)
{
    control_if_t *ctrl_if = get_ctrl_if();

    if ( xen_start_info.flags & SIF_INITDOMAIN )
    {
        /*
         * The initial domain must create its own domain-controller link.
         * The controller is probably not running at this point, but will
         * pick up its end of the event channel from 
         */
        evtchn_op_t op;
        op.cmd = EVTCHNOP_bind_interdomain;
        op.u.bind_interdomain.dom1 = DOMID_SELF;
        op.u.bind_interdomain.dom2 = DOMID_SELF;
	op.u.bind_interdomain.port1 = 0;
	op.u.bind_interdomain.port2 = 0;
        if ( HYPERVISOR_event_channel_op(&op) != 0 )
		panic("EVTCHNOP_bind_interdomain");
        xen_start_info.domain_controller_evtchn = op.u.bind_interdomain.port1;
        initdom_ctrlif_domcontroller_port   = op.u.bind_interdomain.port2;
    }

    /* Sync up with shared indexes. */
    ctrl_if_tx_resp_cons = ctrl_if->tx_resp_prod;
    ctrl_if_rx_req_cons  = ctrl_if->rx_resp_prod;

    ctrl_if_evtchn = xen_start_info.domain_controller_evtchn;
    ctrl_if_irq    = bind_evtchn_to_irq(ctrl_if_evtchn);

    event_set_handler(ctrl_if_irq, &ctrl_if_interrupt, NULL, IPL_HIGH + 2);
    hypervisor_enable_irq(ctrl_if_irq);
}

void ctrl_if_early_init(void)
{

	simple_lock_init(&ctrl_if_lock);

	ctrl_if_evtchn = xen_start_info.domain_controller_evtchn;
}

void ctrl_if_init(void)
{
	int i;

	for ( i = 0; i < 256; i++ )
		ctrl_if_rxmsg_handler[i] = ctrl_if_rxmsg_default_handler;

	if (ctrl_if_evtchn == -1)
		ctrl_if_early_init();

	ctrl_if_softintr = softintr_establish(IPL_SOFTNET,
	    ctrl_if_softintr_handler, NULL);

	ctrl_if_resume();
}


#if 0
/* This is called after it is safe to call schedule_task(). */
static int __init ctrl_if_late_setup(void)
{
    safe_to_schedule_task = 1;
    return 0;
}
__initcall(ctrl_if_late_setup);
#endif


/*
 * !! The following are DANGEROUS FUNCTIONS !!
 * Use with care [for example, see xencons_force_flush()].
 */

int ctrl_if_transmitter_empty(void)
{
    return (get_ctrl_if()->tx_req_prod == ctrl_if_tx_resp_cons);
}

void ctrl_if_discard_responses(void)
{
    ctrl_if_tx_resp_cons = get_ctrl_if()->tx_resp_prod;
}


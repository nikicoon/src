/*	$NetBSD: mach_host.c,v 1.14 2002/12/09 21:29:24 manu Exp $ */

/*-
 * Copyright (c) 2002 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Emmanuel Dreyfus
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: mach_host.c,v 1.14 2002/12/09 21:29:24 manu Exp $");

#include <sys/types.h>
#include <sys/malloc.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/proc.h>

#include <uvm/uvm_extern.h>
#include <uvm/uvm_param.h>

#include <compat/mach/mach_types.h>
#include <compat/mach/mach_host.h>
#include <compat/mach/mach_errno.h>

int 
mach_host_info(p, msgh, maxlen, dst)
	struct proc *p;
	mach_msg_header_t *msgh;
	size_t maxlen;
	mach_msg_header_t *dst;
{
	mach_host_info_request_t req;
	mach_host_info_reply_t rep;
	mach_host_info_reply_simple_t *reps;
	int error;
	int msglen;

	if ((error = copyin(msgh, &req, sizeof(req))) != 0)
		return error;

	bzero(&rep, sizeof(rep));

	rep.rep_msgh.msgh_bits = 
	    MACH_MSGH_REPLY_LOCAL_BITS(MACH_MSG_TYPE_MOVE_SEND_ONCE);
	rep.rep_msgh.msgh_size = sizeof(rep) - sizeof(rep.rep_trailer);
	rep.rep_msgh.msgh_local_port = req.req_msgh.msgh_local_port;
	rep.rep_msgh.msgh_id = req.req_msgh.msgh_id + 100;
	rep.rep_trailer.msgh_trailer_size = 8;

	msglen = sizeof(rep);

	switch(req.req_flavor) {
	case MACH_HOST_BASIC_INFO: {
		struct mach_host_basic_info *info
		    = (struct mach_host_basic_info *)&rep.rep_data[0];

		DPRINTF(("mach_host_info(BASIC_INFO);\n"));
		rep.rep_msgh.msgh_size = sizeof(*reps) 
		    - sizeof(rep.rep_trailer) + sizeof(*info);
		rep.rep_count = sizeof(*info) / sizeof(mach_integer_t);
		mach_host_basic_info(info);
		/* 
		 * XXX this is the trailer, the way it 
		 * is filled should be improved 
		 */
		rep.rep_data[rep.rep_count + 1] = 8;
		break;
	}

	case MACH_HOST_PRIORITY_INFO: {
		struct mach_host_priority_info *info
		    = (struct mach_host_priority_info *)&rep.rep_data[0];

		DPRINTF(("mach_host_info(PRIORITY_INFO);\n"));
		rep.rep_msgh.msgh_size = sizeof(*reps) 
		    - sizeof(rep.rep_trailer) + sizeof(*info);
		rep.rep_count = sizeof(*info) / sizeof(mach_integer_t);
		mach_host_priority_info(info);
		/* 
		 * XXX this is the trailer, the way it 
		 * is filled should be improved 
		 */
		rep.rep_data[rep.rep_count + 1] = 8;
		break;
	}

	case MACH_HOST_SEMAPHORE_TRAPS:
	case MACH_HOST_MACH_MSG_TRAP:
		DPRINTF(("mach_host_info(%s);\n",
		    req.req_flavor == MACH_HOST_SEMAPHORE_TRAPS ?
		    "SEMAPHORE_TRAPS" : "MACH_MSG_TRAP"));
		reps = (mach_host_info_reply_simple_t *)&rep;
		reps->rep_msgh.msgh_size = 
		    sizeof(*reps) - sizeof(reps->rep_trailer);
		reps->rep_trailer.msgh_trailer_size = 8;
		msglen = sizeof(*reps);
		break;

	case MACH_HOST_SCHED_INFO:
	case MACH_HOST_RESOURCE_SIZES:
		DPRINTF(("Unimplemented host_info flavor %d\n", 
		    req.req_flavor));
	default:
		uprintf("Unknown host_info flavor %d\n", req.req_flavor);
		rep.rep_retval = native_to_mach_errno[EINVAL];
		break;
	}

	return MACH_MSG_RETURN(p, &rep, msgh, msglen, maxlen, dst);
}


int 
mach_host_page_size(p, msgh, maxlen, dst)
	struct proc *p;
	mach_msg_header_t *msgh;
	size_t maxlen;
	mach_msg_header_t *dst;
{
	mach_host_page_size_request_t req;
	mach_host_page_size_reply_t rep;
	int error;

	if ((error = copyin(msgh, &req, sizeof(req))) != 0)
		return error;

	DPRINTF(("mach_host_page_size();\n"));

	bzero(&rep, sizeof(rep));

	rep.rep_msgh.msgh_bits =
	    MACH_MSGH_REPLY_LOCAL_BITS(MACH_MSG_TYPE_MOVE_SEND_ONCE);
	rep.rep_msgh.msgh_size = sizeof(rep) - sizeof(rep.rep_trailer);
	rep.rep_msgh.msgh_local_port = req.req_msgh.msgh_local_port;
	rep.rep_msgh.msgh_id = req.req_msgh.msgh_id + 100;
	rep.rep_page_size = PAGE_SIZE;
	rep.rep_trailer.msgh_trailer_size = 8;
	
	return MACH_MSG_RETURN(p, &rep, msgh, sizeof(rep), maxlen, dst);
}

int
mach_host_get_clock_service(p, msgh, maxlen, dst)
	struct proc *p;
	mach_msg_header_t *msgh;
	size_t maxlen;
	mach_msg_header_t *dst;
{
	mach_host_get_clock_service_request_t req;
	mach_host_get_clock_service_reply_t rep;
	int error;

	if ((error = copyin(msgh, &req, sizeof(req))) != 0)
		return error;

	DPRINTF(("mach_host_get_clock_service();\n"));

	bzero(&rep, sizeof(rep));

	rep.rep_msgh.msgh_bits = 
	    MACH_MSGH_REPLY_LOCAL_BITS(MACH_MSG_TYPE_MOVE_SEND_ONCE) |
	    MACH_MSGH_BITS_COMPLEX;
	rep.rep_msgh.msgh_size = sizeof(rep) - sizeof(rep.rep_trailer);
	rep.rep_msgh.msgh_local_port = req.req_msgh.msgh_local_port;
	rep.rep_msgh.msgh_id = req.req_msgh.msgh_id + 100;
	rep.rep_body.msgh_descriptor_count = 1; /* XXX why? */
	rep.rep_clock_serv.name = 0x60b; /* XXX */
	rep.rep_clock_serv.disposition = 0x11; /* XXX */
	rep.rep_trailer.msgh_trailer_size = 8;

	return MACH_MSG_RETURN(p, &rep, msgh, sizeof(rep), maxlen, dst);
}

void
mach_host_priority_info(info)
	struct mach_host_priority_info *info;
{
	/* XXX One day, try to fill this correctly */
	info->kernel_priority = 0x50;
	info->system_priority = 0x50;
	info->server_priority = 0x40;
	info->user_priority = 0x1f;
	info->depress_priority = 0x00;
	info->idle_priority = 0x00;
	info->minimum_priority = 0x00;
	info->maximum_priority = 0x4f;

	return;
}

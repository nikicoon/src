/*	$KAME: handler.c,v 1.43 2001/02/06 16:28:16 thorpej Exp $	*/

/*
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "var.h"
#include "misc.h"
#include "vmbuf.h"
#include "plog.h"
#include "sockmisc.h"
#include "debug.h"

#include "schedule.h"
#include "algorithm.h"
#include "crypto_openssl.h"
#include "policy.h"
#include "proposal.h"
#include "isakmp_var.h"
#include "isakmp.h"
#include "isakmp_inf.h"
#include "oakley.h"
#include "remoteconf.h"
#include "handler.h"

#ifdef HAVE_GSSAPI
#include "gssapi.h"
#endif

static LIST_HEAD(_ph1tree_, ph1handle) ph1tree;
static LIST_HEAD(_ph2tree_, ph2handle) ph2tree;
static LIST_HEAD(_ctdtree_, contacted) ctdtree;

/*
 * functions about management of the isakmp status table
 */
/* %%% management phase 1 handler */
/*
 * search for isakmpsa handler with isakmp index.
 */

extern caddr_t val2str(const char *, size_t);

struct ph1handle *
getph1byindex(index)
	isakmp_index *index;
{
	struct ph1handle *p;

	LIST_FOREACH(p, &ph1tree, chain) {
		if (p->status == PHASE1ST_EXPIRED)
			continue;
		if (memcmp(&p->index, index, sizeof(*index)) == 0)
			return p;
	}

	return NULL;
}

/*
 * search for isakmp handler by i_ck in index.
 */
struct ph1handle *
getph1byindex0(index)
	isakmp_index *index;
{
	struct ph1handle *p;

	LIST_FOREACH(p, &ph1tree, chain) {
		if (p->status == PHASE1ST_EXPIRED)
			continue;
		if (memcmp(&p->index, index, sizeof(cookie_t)) == 0)
			return p;
	}

	return NULL;
}

/*
 * search for isakmpsa handler by remote address.
 * don't use port number to search because this function search
 * with phase 2's destinaion.
 */
struct ph1handle *
getph1byaddr(local, remote)
	struct sockaddr *local, *remote;
{
	struct ph1handle *p;

	LIST_FOREACH(p, &ph1tree, chain) {
		if (p->status == PHASE1ST_EXPIRED)
			continue;
		if (cmpsaddrwop(local, p->local) == 0
		 && cmpsaddrwop(remote, p->remote) == 0)
			return p;
	}

	return NULL;
}

/*
 * dump isakmp-sa
 */
vchar_t *
dumpph1()
{
	struct ph1handle *iph1;
	struct ph1dump *pd;
	int cnt = 0;
	vchar_t *buf;

	/* get length of buffer */
	LIST_FOREACH(iph1, &ph1tree, chain)
		cnt++;

	buf = vmalloc(cnt * sizeof(struct ph1dump));
	if (buf == NULL) {
		plog(LLV_ERROR, LOCATION, NULL,
			"failed to get buffer\n");
		return NULL;
	}
	pd = (struct ph1dump *)buf->v;

	LIST_FOREACH(iph1, &ph1tree, chain) {
		memcpy(&pd->index, &iph1->index, sizeof(iph1->index));
		pd->status = iph1->status;
		pd->side = iph1->side;
		memcpy(&pd->remote, iph1->remote, iph1->remote->sa_len);
		memcpy(&pd->local, iph1->local, iph1->local->sa_len);
		pd->version = iph1->version;
		pd->etype = iph1->etype;
		pd->created = iph1->created;
		pd->ph2cnt = iph1->ph2cnt;
		pd++;
	}

	return buf;
}

/*
 * create new isakmp Phase 1 status record to handle isakmp in Phase1
 */
struct ph1handle *
newph1()
{
	struct ph1handle *iph1;

	/* create new iph1 */
	iph1 = CALLOC(sizeof(*iph1), struct ph1handle *);
	if (iph1 == NULL)
		return NULL;

	iph1->status = PHASE1ST_SPAWN;

	return iph1;
}

/*
 * delete new isakmp Phase 1 status record to handle isakmp in Phase1
 */
void
delph1(iph1)
	struct ph1handle *iph1;
{
	if (iph1->remote) {
		free(iph1->remote);
		iph1->remote = NULL;
	}
	if (iph1->local) {
		free(iph1->local);
		iph1->local = NULL;
	}

	VPTRINIT(iph1->authstr);

	if (iph1->sce)
		SCHED_KILL(iph1->sce);
	if (iph1->scr)
		SCHED_KILL(iph1->scr);
	VPTRINIT(iph1->sendbuf);

	flush_recvedpkt(iph1->rlist);
	iph1->rlist = NULL;

	VPTRINIT(iph1->dhpriv);
	VPTRINIT(iph1->dhpub);
	VPTRINIT(iph1->dhpub_p);
	VPTRINIT(iph1->dhgxy);
	VPTRINIT(iph1->nonce);
	VPTRINIT(iph1->nonce_p);
	VPTRINIT(iph1->skeyid);
	VPTRINIT(iph1->skeyid_d);
	VPTRINIT(iph1->skeyid_a);
	VPTRINIT(iph1->skeyid_e);
	VPTRINIT(iph1->key);
	VPTRINIT(iph1->hash);
	VPTRINIT(iph1->sig);
	VPTRINIT(iph1->sig_p);
	oakley_delcert(iph1->cert);
	iph1->cert = NULL;
	oakley_delcert(iph1->cert_p);
	iph1->cert_p = NULL;
	oakley_delcert(iph1->crl_p);
	iph1->crl_p = NULL;
	oakley_delcert(iph1->cr_p);
	iph1->cr_p = NULL;
	VPTRINIT(iph1->id);
	VPTRINIT(iph1->id_p);

	if (iph1->ivm) {
		oakley_delivm(iph1->ivm);
		iph1->ivm = NULL;
	}

	VPTRINIT(iph1->sa);
	VPTRINIT(iph1->sa_ret);

#ifdef HAVE_GSSAPI
	VPTRINIT(iph1->gi_i);
	VPTRINIT(iph1->gi_r);

	gssapi_free_state(iph1);
#endif

	free(iph1);
}

/*
 * create new isakmp Phase 1 status record to handle isakmp in Phase1
 */
int
insph1(iph1)
	struct ph1handle *iph1;
{
	/* validity check */
	if (iph1->remote == NULL) {
		plog(LLV_ERROR, LOCATION, NULL,
			"invalid isakmp SA handler. no remote address.\n");
		return -1;
	}
	LIST_INSERT_HEAD(&ph1tree, iph1, chain);

	return 0;
}

void
remph1(iph1)
	struct ph1handle *iph1;
{
	LIST_REMOVE(iph1, chain);
}

/*
 * flush isakmp-sa
 */
void
flushph1()
{
	struct ph1handle *p, *next;

	for (p = LIST_FIRST(&ph1tree); p; p = next) {
		next = LIST_NEXT(p, chain);

		/* send delete information */
		if (p->status == PHASE1ST_ESTABLISHED) 
			isakmp_info_send_d1(p);

		remph1(p);
		delph1(p);
	}
}

void
initph1tree()
{
	LIST_INIT(&ph1tree);
}

/* %%% management phase 2 handler */
/*
 * search ph2handle with policyindex.
 */
#if 0
struct ph2handle *
getph2byspidx(spidx)
	struct policyindex *spidx;
{
	struct ph2handle *p;

	LIST_FOREACH(p, &ph2tree, chain) {
		/*
		 * there are ph2handle independent on policy
		 * such like informational exchange.
		 */
		if (p->spidx == NULL)
			continue;
		if (cmpspidx(spidx, p->spidx) == 0)
			return p;
	}

	return NULL;
}
#endif

/*
 * search ph2handle with policy id.
 */
struct ph2handle *
getph2byspid(spid)
      u_int32_t spid;
{
	struct ph2handle *p;

	LIST_FOREACH(p, &ph2tree, chain) {
		/*
		 * there are ph2handle independent on policy
		 * such like informational exchange.
		 */
		if (p->spid == spid)
			return p;
	}

	return NULL;
}

/*
 * search ph2handle with sequence number.
 */
struct ph2handle *
getph2byseq(seq)
	u_int32_t seq;
{
	struct ph2handle *p;

	LIST_FOREACH(p, &ph2tree, chain) {
		if (p->seq == seq)
			return p;
	}

	return NULL;
}

/*
 * search ph2handle with message id.
 */
struct ph2handle *
getph2bymsgid(iph1, msgid)
	struct ph1handle *iph1;
	u_int32_t msgid;
{
	struct ph2handle *p;

	LIST_FOREACH(p, &ph2tree, chain) {
		if (p->msgid == msgid)
			return p;
	}

	return NULL;
}

/*
 * call by pk_recvexpire().
 */
struct ph2handle *
getph2bysaidx(src, dst, proto_id, spi)
	struct sockaddr *src, *dst;
	u_int proto_id;
	u_int32_t spi;
{
	struct ph2handle *iph2;
	struct saproto *pr;

	LIST_FOREACH(iph2, &ph2tree, chain) {
		if (iph2->proposal == NULL && iph2->approval == NULL)
			continue;
		if (iph2->approval != NULL) {
			for (pr = iph2->approval->head; pr != NULL; pr = pr->next) {
				if (proto_id != pr->proto_id)
					break;
				if (spi == pr->spi || spi == pr->spi_p)
					return iph2;
			}
		} else if (iph2->proposal != NULL) {
			for (pr = iph2->proposal->head; pr != NULL; pr = pr->next) {
				if (proto_id != pr->proto_id)
					break;
				if (spi == pr->spi)
					return iph2;
			}
		}
	}

	return NULL;
}

/*
 * create new isakmp Phase 2 status record to handle isakmp in Phase2
 */
struct ph2handle *
newph2()
{
	struct ph2handle *iph2 = NULL;

	/* create new iph2 */
	iph2 = CALLOC(sizeof(*iph2), struct ph2handle *);
	if (iph2 == NULL)
		return NULL;

	iph2->status = PHASE1ST_SPAWN;

	return iph2;
}

/*
 * initialize ph2handle
 * NOTE: don't initialize src/dst.
 *       SPI in the proposal is cleared.
 */
void
initph2(iph2)
	struct ph2handle *iph2;
{
	if (iph2->sce)
		SCHED_KILL(iph2->sce);
	if (iph2->scr)
		SCHED_KILL(iph2->scr);

	VPTRINIT(iph2->sendbuf);

	flush_recvedpkt(iph2->rlist);
	iph2->rlist = NULL;

	/* clear spi, keep variables in the proposal */
	if (iph2->proposal) {
		struct saproto *pr;
		for (pr = iph2->proposal->head; pr != NULL; pr = pr->next)
			pr->spi = 0;
	}

	/* clear approval */
	if (iph2->approval) {
		flushsaprop(iph2->approval);
		iph2->approval = NULL;
	}

	if (iph2->pfsgrp) {
		oakley_dhgrp_free(iph2->pfsgrp);
		iph2->pfsgrp = NULL;
	}

	VPTRINIT(iph2->dhpriv);
	VPTRINIT(iph2->dhpub);
	VPTRINIT(iph2->dhpub_p);
	VPTRINIT(iph2->dhgxy);
	VPTRINIT(iph2->id);
	VPTRINIT(iph2->id_p);
	VPTRINIT(iph2->nonce);
	VPTRINIT(iph2->nonce_p);
	VPTRINIT(iph2->hash);
	VPTRINIT(iph2->sa);
	VPTRINIT(iph2->sa_ret);

	if (iph2->ivm) {
		oakley_delivm(iph2->ivm);
		iph2->ivm = NULL;
	}
}

/*
 * delete new isakmp Phase 2 status record to handle isakmp in Phase2
 */
void
delph2(iph2)
	struct ph2handle *iph2;
{
	initph2(iph2);

	if (iph2->src) {
		free(iph2->src);
		iph2->src = NULL;
	}
	if (iph2->dst) {
		free(iph2->dst);
		iph2->dst = NULL;
	}
	if (iph2->src_id) {
	      free(iph2->src_id);
	      iph2->src_id = NULL;
	}
	if (iph2->dst_id) {
	      free(iph2->dst_id);
	      iph2->dst_id = NULL;
	}

	if (iph2->proposal) {
		flushsaprop(iph2->proposal);
		iph2->proposal = NULL;
	}

	free(iph2);
}

/*
 * create new isakmp Phase 2 status record to handle isakmp in Phase2
 */
int
insph2(iph2)
	struct ph2handle *iph2;
{
	LIST_INSERT_HEAD(&ph2tree, iph2, chain);

	return 0;
}

void
remph2(iph2)
	struct ph2handle *iph2;
{
	LIST_REMOVE(iph2, chain);
}

void
initph2tree()
{
	LIST_INIT(&ph2tree);
}

void
flushph2()
{
	struct ph2handle *p, *next;

	for (p = LIST_FIRST(&ph2tree); p; p = next) {
		next = LIST_NEXT(p, chain);

		/* send delete information */
		if (p->status == PHASE2ST_ESTABLISHED) 
			isakmp_info_send_d2(p);

		unbindph12(p);
		remph2(p);
		delph2(p);
	}
}

/* %%% */
void
bindph12(iph1, iph2)
	struct ph1handle *iph1;
	struct ph2handle *iph2;
{
	iph2->ph1 = iph1;
	LIST_INSERT_HEAD(&iph1->ph2tree, iph2, ph1bind);
}

void
unbindph12(iph2)
	struct ph2handle *iph2;
{
	if (iph2->ph1 != NULL) {
		iph2->ph1 = NULL;
		LIST_REMOVE(iph2, ph1bind);
	}
}

/* %%% management contacted list */
/*
 * search contacted list.
 */
struct contacted *
getcontacted(remote)
	struct sockaddr *remote;
{
	struct contacted *p;

	LIST_FOREACH(p, &ctdtree, chain) {
		if (cmpsaddr(remote, p->remote) == 0)
			return p;
	}

	return NULL;
}

/*
 * create new isakmp Phase 2 status record to handle isakmp in Phase2
 */
int
inscontacted(remote)
	struct sockaddr *remote;
{
	struct contacted *new;

	/* create new iph2 */
	new = CALLOC(sizeof(*new), struct contacted *);
	if (new == NULL)
		return -1;

	new->remote = dupsaddr(remote);

	LIST_INSERT_HEAD(&ctdtree, new, chain);

	return 0;
}

void
initctdtree()
{
	LIST_INIT(&ctdtree);
}

/*
 * checking a packet whether is received or not.
 * OUT:
 *	 0:	the packet is first received.
 *	 1:	the packet was reveiced before, or error happened.
 */
int
check_recvedpkt(msg, list)
	vchar_t *msg;
	struct recvedpkt *list;
{
	vchar_t *buf;
	struct recvedpkt *n;

	buf = eay_md5_one(msg);
	if (!buf) {
		plog(LLV_ERROR, LOCATION, NULL,
			"failed to allocate buffer.\n");
		return 1;
	}

	for (n = list; n; n = n->next) {
		if (memcmp(buf->v, n->hash->v, n->hash->l) == 0)
			break;
	}

	vfree(buf);

	if (n)
		return 1;

	return 0;
}

/*
 * adding a hash of received packet into the received list.
 */
int
add_recvedpkt(msg, list)
	vchar_t *msg;
	struct recvedpkt **list;
{
	struct recvedpkt *new;

	new = CALLOC(sizeof(*new), struct recvedpkt *);
	if (!new) {
		plog(LLV_ERROR, LOCATION, NULL,
			"failed to allocate buffer.\n");
		return -1;
	}
	new->hash = eay_md5_one(msg);
	if (!new->hash) {
		plog(LLV_ERROR, LOCATION, NULL,
			"failed to allocate buffer.\n");
		free(new);
		return -1;
	}

	new->next = *list;
	*list = new;

	return 0;
}

void
flush_recvedpkt(list)
	struct recvedpkt *list;
{
	struct recvedpkt *n, *next;

	for (n = list; n; n = next) {
		next = n->next;
		vfree(n->hash);
		free(n);
	}
}

/*	$NetBSD: framebuf.c,v 1.6 2007/05/11 21:27:13 pooka Exp $	*/

/*
 * Copyright (c) 2007  Antti Kantee.  All Rights Reserved.
 *
 * Development of this software was supported by the
 * Finnish Cultural Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#if !defined(lint)
__RCSID("$NetBSD: framebuf.c,v 1.6 2007/05/11 21:27:13 pooka Exp $");
#endif /* !lint */

#include <sys/types.h>
#include <sys/queue.h>

#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <puffs.h>
#include <stdlib.h>
#include <unistd.h>

#include "puffs_priv.h"

struct puffs_framebuf {
	struct puffs_cc *pcc;	/* pcc to continue with */
	/* OR */
	puffs_framebuf_cb fcb;	/* non-blocking callback */
	void *fcb_arg;		/* argument for previous */

	uint8_t *buf;		/* buffer base */
	size_t len;		/* total length */

	size_t offset;		/* cursor, telloff() */
	size_t maxoff;		/* maximum offset for data, tellsize() */

	volatile int rv;	/* errno value for pcc framebufs */

	int	istat;

	TAILQ_ENTRY(puffs_framebuf) pfb_entries;
};
#define ISTAT_NODESTROY	0x01	/* indestructible by framebuf_destroy() */
#define ISTAT_INTERNAL	0x02	/* never leaves library			*/
#define ISTAT_NOREPLY	0x04	/* nuke after sending 			*/

#define PUFBUF_INCRALLOC 65536	/* 64k ought to be enough for anyone */
#define PUFBUF_REMAIN(p) (p->len - p->offset)

static struct puffs_fctrl_io *
getfiobyfd(struct puffs_usermount *pu, int fd)
{
	struct puffs_framectrl *pfctrl = &pu->pu_framectrl;
	struct puffs_fctrl_io *fio;

	LIST_FOREACH(fio, &pfctrl->fb_ios, fio_entries)
		if (fio->io_fd == fd)
			return fio;
	return NULL;
}

struct puffs_framebuf *
puffs_framebuf_make()
{
	struct puffs_framebuf *pufbuf;

	pufbuf = malloc(sizeof(struct puffs_framebuf));
	if (pufbuf == NULL)
		return NULL;
	memset(pufbuf, 0, sizeof(struct puffs_framebuf));

	pufbuf->buf = malloc(PUFBUF_INCRALLOC);
	pufbuf->len = PUFBUF_INCRALLOC;
	if (pufbuf->buf == NULL) {
		free(pufbuf);
		return NULL;
	}

	puffs_framebuf_recycle(pufbuf);
	return pufbuf;
}

void
puffs_framebuf_destroy(struct puffs_framebuf *pufbuf)
{

	assert((pufbuf->istat & ISTAT_NODESTROY) == 0);

	free(pufbuf->buf);
	free(pufbuf);
}

void
puffs_framebuf_recycle(struct puffs_framebuf *pufbuf)
{

	assert((pufbuf->istat & ISTAT_NODESTROY) == 0);

	pufbuf->offset = 0;
	pufbuf->maxoff = 0;
	pufbuf->istat = 0;
}

static int
reservespace(struct puffs_framebuf *pufbuf, size_t off, size_t wantsize)
{
	size_t incr;
	void *nd;

	if (off <= pufbuf->len && pufbuf->len - off >= wantsize)
		return 0;

	for (incr = PUFBUF_INCRALLOC;
	    pufbuf->len + incr < off + wantsize;
	    incr += PUFBUF_INCRALLOC)
		continue;

	nd = realloc(pufbuf->buf, pufbuf->offset + incr);
	if (nd == NULL)
		return -1;

	pufbuf->buf = nd;
	pufbuf->len += incr;

	return 0;
}

int
puffs_framebuf_reserve_space(struct puffs_framebuf *pufbuf, size_t wantsize)
{

	return reservespace(pufbuf, pufbuf->offset, wantsize);
}

int
puffs_framebuf_putdata(struct puffs_framebuf *pufbuf,
	const void *data, size_t dlen)
{

	if (PUFBUF_REMAIN(pufbuf) < dlen)
		if (puffs_framebuf_reserve_space(pufbuf, dlen) == -1)
			return -1;

	memcpy(pufbuf->buf + pufbuf->offset, data, dlen);
	pufbuf->offset += dlen;

	if (pufbuf->offset > pufbuf->maxoff)
		pufbuf->maxoff = pufbuf->offset;

	return 0;
}

int
puffs_framebuf_putdata_atoff(struct puffs_framebuf *pufbuf, size_t offset,
	const void *data, size_t dlen)
{

	if (reservespace(pufbuf, offset, dlen) == -1)
		return -1;

	memcpy(pufbuf->buf + offset, data, dlen);

	if (offset + dlen > pufbuf->maxoff)
		pufbuf->maxoff = offset + dlen;

	return 0;
}

int
puffs_framebuf_getdata(struct puffs_framebuf *pufbuf, void *data, size_t dlen)
{

	if (pufbuf->maxoff < pufbuf->offset + dlen) {
		errno = ENOBUFS;
		return -1;
	}

	memcpy(data, pufbuf->buf + pufbuf->offset, dlen);
	pufbuf->offset += dlen;

	return 0;
}

int
puffs_framebuf_getdata_atoff(struct puffs_framebuf *pufbuf, size_t offset,
	void *data, size_t dlen)
{

	if (pufbuf->maxoff < offset + dlen) {
		errno = ENOBUFS;
		return -1;
	}

	memcpy(data, pufbuf->buf + offset, dlen);
	return 0;
}

size_t
puffs_framebuf_telloff(struct puffs_framebuf *pufbuf)
{

	return pufbuf->offset;
}

size_t
puffs_framebuf_tellsize(struct puffs_framebuf *pufbuf)
{

	return pufbuf->maxoff;
}

size_t
puffs_framebuf_remaining(struct puffs_framebuf *pufbuf)
{

	return puffs_framebuf_tellsize(pufbuf) - puffs_framebuf_telloff(pufbuf);
}

int
puffs_framebuf_seekset(struct puffs_framebuf *pufbuf, size_t newoff)
{

	if (reservespace(pufbuf, newoff, 0) == -1)
		return -1;

	pufbuf->offset = newoff;
	return 0;
}

int
puffs_framebuf_getwindow(struct puffs_framebuf *pufbuf, size_t winoff,
	void **data, size_t *dlen)
{
	size_t winlen;

#ifdef WINTESTING
	winlen = MIN(*dlen, 32);
#else
	winlen = *dlen;
#endif

	if (reservespace(pufbuf, winoff, winlen) == -1)
		return -1;

	*data = pufbuf->buf + winoff;
	if (pufbuf->maxoff < winoff + winlen)
		pufbuf->maxoff = winoff + winlen;

	return 0;
}

int
puffs_framebuf_enqueue_cc(struct puffs_cc *pcc, int fd,
	struct puffs_framebuf *pufbuf)
{
	struct puffs_usermount *pu = puffs_cc_getusermount(pcc);
	struct puffs_fctrl_io *fio;

	fio = getfiobyfd(pu, fd);
	if (fio == NULL) {
		errno = EINVAL;
		return -1;
	}

	pufbuf->pcc = pcc;
	pufbuf->fcb = NULL;
	pufbuf->fcb_arg = NULL;

	pufbuf->offset = 0;
	pufbuf->istat |= ISTAT_NODESTROY;

	TAILQ_INSERT_TAIL(&fio->snd_qing, pufbuf, pfb_entries);

	puffs_cc_yield(pcc);
	if (pufbuf->rv) {
		errno = pufbuf->rv;
		return -1;
	}

	return 0;
}

int
puffs_framebuf_enqueue_cb(struct puffs_usermount *pu, int fd,
	struct puffs_framebuf *pufbuf, puffs_framebuf_cb fcb, void *arg)
{
	struct puffs_fctrl_io *fio;

	fio = getfiobyfd(pu, fd);
	if (fio == NULL) {
		errno = EINVAL;
		return -1;
	}

	pufbuf->pcc = NULL;
	pufbuf->fcb = fcb;
	pufbuf->fcb_arg = arg;

	pufbuf->offset = 0;
	pufbuf->istat |= ISTAT_NODESTROY;

	TAILQ_INSERT_TAIL(&fio->snd_qing, pufbuf, pfb_entries);

	return 0;
}

int
puffs_framebuf_enqueue_justsend(struct puffs_usermount *pu, int fd,
	struct puffs_framebuf *pufbuf, int reply)
{
	struct puffs_fctrl_io *fio;

	fio = getfiobyfd(pu, fd);
	if (fio == NULL) {
		errno = EINVAL;
		return -1;
	}

	pufbuf->pcc = NULL;
	pufbuf->fcb = NULL;
	pufbuf->fcb_arg = NULL;

	pufbuf->offset = 0;
	pufbuf->istat |= ISTAT_NODESTROY;
	if (!reply)
		pufbuf->istat |= ISTAT_NOREPLY;

	TAILQ_INSERT_TAIL(&fio->snd_qing, pufbuf, pfb_entries);

	return 0;
}

static struct puffs_framebuf *
findbuf(struct puffs_usermount *pu, struct puffs_framectrl *fctrl,
	struct puffs_fctrl_io *fio, struct puffs_framebuf *findme)
{
	struct puffs_framebuf *cand;

	TAILQ_FOREACH(cand, &fio->res_qing, pfb_entries)
		if (fctrl->cmpfb(pu, findme, cand))
			break;

	if (cand == NULL)
		return NULL;

	TAILQ_REMOVE(&fio->res_qing, cand, pfb_entries);
	return cand;
}

static void
moveinfo(struct puffs_framebuf *from, struct puffs_framebuf *to)
{

	assert(from->istat & ISTAT_INTERNAL);

	/* migrate buffer */
	free(to->buf);
	to->buf = from->buf;
	from->buf = NULL;

	/* migrate buffer info */
	to->len = from->len;
	to->offset = from->offset;
	to->maxoff = from->maxoff;
}

int
puffs_framebuf_input(struct puffs_usermount *pu, struct puffs_framectrl *fctrl,
	struct puffs_fctrl_io *fio, struct puffs_putreq *ppr)
{
	struct puffs_framebuf *pufbuf, *appbuf;
	int rv, complete;

	for (;;) {
		if ((pufbuf = fio->cur_in) == NULL) {
			pufbuf = puffs_framebuf_make();
			if (pufbuf == NULL)
				return -1;
			pufbuf->istat |= ISTAT_INTERNAL;
			fio->cur_in = pufbuf;
		}

		complete = 0;
		rv = fctrl->rfb(pu, pufbuf, fio->io_fd, &complete);

		/* error */
		if (rv) {
			errno = rv;
			return -1;
		}

		/* partial read, come back to fight another day */
		if (complete == 0)
			break;

		/* else: full read, process */

		appbuf = findbuf(pu, fctrl, fio, pufbuf);
		if (appbuf == NULL) {
			errno = ENOMSG;
			return -1;
		}
			
		appbuf->istat &= ~ISTAT_NODESTROY;
		moveinfo(pufbuf, appbuf);
		if (appbuf->pcc) {
			puffs_docc(appbuf->pcc, ppr);
		} else if (appbuf->fcb) {
			appbuf->fcb(pu, appbuf, appbuf->fcb_arg);
		} else {
			puffs_framebuf_destroy(appbuf);
		}
		puffs_framebuf_destroy(pufbuf);

		/* hopeless romantics, here we go again */
		fio->cur_in = NULL;
	}

	return rv;
}

int
puffs_framebuf_output(struct puffs_usermount *pu, struct puffs_framectrl *fctrl,
	struct puffs_fctrl_io *fio)
{
	struct puffs_framebuf *pufbuf, *pufbuf_next;
	int rv, complete;

	for (pufbuf = TAILQ_FIRST(&fio->snd_qing);
	    pufbuf;
	    pufbuf = pufbuf_next) {
		complete = 0;
		pufbuf_next = TAILQ_NEXT(pufbuf, pfb_entries);
		rv = fctrl->wfb(pu, pufbuf, fio->io_fd, &complete);

		if (rv) {
			TAILQ_REMOVE(&fio->snd_qing, pufbuf, pfb_entries);
			pufbuf->rv = rv;
			errno = rv;
			return -1;
		}

		/* partial write */
		if (complete == 0)
			return 0;

		/* else, complete write */
		TAILQ_REMOVE(&fio->snd_qing, pufbuf, pfb_entries);

		if ((pufbuf->istat & ISTAT_NOREPLY) == 0) {
			TAILQ_INSERT_TAIL(&fio->res_qing, pufbuf,
			    pfb_entries);
		} else {
			pufbuf->istat &= ~ISTAT_NODESTROY;
			puffs_framebuf_destroy(pufbuf);
		}
	}

	return 0;
}

int
puffs_framebuf_addfd(struct puffs_usermount *pu, int fd)
{
	struct puffs_framectrl *pfctrl = &pu->pu_framectrl;
	struct puffs_fctrl_io *fio;
	struct kevent kev[2];
	struct kevent *newevs;
	int rv, nfds;

	nfds = pfctrl->nfds+1;
	newevs = realloc(pfctrl->evs, (2*nfds+1) * sizeof(struct kevent));
	if (newevs == NULL)
		return -1;
	pfctrl->evs = newevs;

	newevs = realloc(pfctrl->ch_evs, nfds * sizeof(struct kevent));
	if (newevs == NULL)
		return -1;
	pfctrl->ch_evs = newevs;

	fio = malloc(sizeof(struct puffs_fctrl_io));
	if (fio == NULL)
		return -1;

	if (pu->pu_state & PU_INLOOP) {
		EV_SET(&kev[0], fd, EVFILT_READ, EV_ADD, 0, 0, (intptr_t)fio);
		EV_SET(&kev[1], fd, EVFILT_WRITE, EV_ADD|EV_DISABLE,
		    0, 0, (intptr_t)fio);
		rv = kevent(pu->pu_kq, kev, 2, NULL, 0, NULL);
		if (rv == -1) {
			free(fio);
			return -1;
		}
	}

	fio->io_fd = fd;
	fio->cur_in = NULL;
	fio->wrstat = 0;
	TAILQ_INIT(&fio->snd_qing);
	TAILQ_INIT(&fio->res_qing);

	LIST_INSERT_HEAD(&pfctrl->fb_ios, fio, fio_entries);
	pfctrl->nfds = nfds;

	return 0;
}

static int
removefio(struct puffs_usermount *pu, struct puffs_fctrl_io *fio)
{
	struct puffs_framectrl *pfctrl = &pu->pu_framectrl;
	struct puffs_framebuf *pufbuf;
	struct kevent kev[2];

	/* found, now remove */
	if (pu->pu_state & PU_INLOOP) {
		EV_SET(&kev[0], fio->io_fd, EVFILT_READ, EV_DELETE, 0, 0, 0);
		EV_SET(&kev[1], fio->io_fd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
		(void) kevent(pu->pu_kq, kev, 2, NULL, 0, NULL);
	}

	LIST_REMOVE(fio, fio_entries);

	/* free buffers */
	while ((pufbuf = TAILQ_FIRST(&fio->snd_qing)) != NULL) {
		TAILQ_REMOVE(&fio->snd_qing, pufbuf, pfb_entries);
		puffs_framebuf_destroy(pufbuf);
	}
	while ((pufbuf = TAILQ_FIRST(&fio->res_qing)) != NULL) {
		TAILQ_REMOVE(&fio->res_qing, pufbuf, pfb_entries);
		puffs_framebuf_destroy(pufbuf);
	}
	if (fio->cur_in)
		puffs_framebuf_destroy(fio->cur_in);
	free(fio);

	/* don't bother with realloc */
	pfctrl->nfds--;

	return 0;

}

int
puffs_framebuf_removefd(struct puffs_usermount *pu, int fd)
{
	struct puffs_fctrl_io *fio;

	fio = getfiobyfd(pu, fd);
	if (fio == NULL) {
		errno = ENXIO;
		return -1;
	}

	return removefio(pu, fio);
}

int
puffs_framebuf_init(struct puffs_usermount *pu,
	puffs_framebuf_readframe_fn rfb, puffs_framebuf_writeframe_fn wfb,
	puffs_framebuf_respcmp_fn cmpfb,
	puffs_framebuf_loop_fn lfb)
{
	struct puffs_framectrl *pfctrl;

	pfctrl = &pu->pu_framectrl;
	pfctrl->rfb = rfb;
	pfctrl->wfb = wfb;
	pfctrl->cmpfb = cmpfb;
	pfctrl->lfb = lfb;
	pfctrl->evs = NULL;
	pfctrl->ch_evs = NULL;
	pfctrl->nfds = 0;

	return 0;
}

void
puffs_framebuf_exit(struct puffs_usermount *pu)
{
	struct puffs_framectrl *pfctrl = &pu->pu_framectrl;
	struct puffs_fctrl_io *fio;

	while ((fio = LIST_FIRST(&pfctrl->fb_ios)) != NULL)
		removefio(pu, fio);
	free(pfctrl->evs);

	/* closing pu->pu_kq takes care of puffsfd */
}

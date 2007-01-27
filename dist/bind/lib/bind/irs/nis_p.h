/*	$NetBSD: nis_p.h,v 1.1.1.4 2007/01/27 21:08:59 christos Exp $	*/

/*
 * Copyright (c) 2004 by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1996,1999 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Id: nis_p.h,v 1.2.18.1 2005/04/27 05:01:04 sra Exp
 */

/*! \file
 * \brief
 * nis_p.h - private include file for the NIS functions.
 */

/*%
 * Object state.
 */
struct nis_p {
	char *			domain;
	struct __res_state *	res;
	void                    (*free_res) __P((void *));
};


/*
 * Methods.
 */

extern struct irs_gr *	irs_nis_gr __P((struct irs_acc *));
extern struct irs_pw *	irs_nis_pw __P((struct irs_acc *));
extern struct irs_sv *	irs_nis_sv __P((struct irs_acc *));
extern struct irs_pr *	irs_nis_pr __P((struct irs_acc *));
extern struct irs_ho *	irs_nis_ho __P((struct irs_acc *));
extern struct irs_nw *	irs_nis_nw __P((struct irs_acc *));
extern struct irs_ng *	irs_nis_ng __P((struct irs_acc *));

/*	$NetBSD: conf.c,v 1.9 1999/01/30 12:30:57 simonb Exp $	*/

/*
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Ralph Campbell.
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
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)conf.c	8.1 (Berkeley) 6/10/93
 */

#include <sys/types.h>
#include <stand.h>
#include <ufs.h>
#include <machine/dec_prom.h>
#include <rz.h>
#ifndef BOOT
#include <tz.h>
#endif

const	struct callback *callv = &callvec;
int	errno;

extern void	nullsys();
extern int	nodev(), noioctl();

#ifdef SMALL
#define rzclose /*(()(struct open_file*))*/0
#define rzioctl /*(()(struct open_file*, u_long, void*))*/0
#else
#define	rzioctl		noioctl
#define	tzioctl		noioctl
#endif

struct devsw devsw[] = {
	{ "rz",	rzstrategy,	rzopen,	rzclose,	rzioctl }, /*0*/
#ifndef BOOT
	{ "tz",	tzstrategy,	tzopen,	tzclose,	tzioctl }, /*1*/
#endif
};

int	ndevs = (sizeof(devsw)/sizeof(devsw[0]));

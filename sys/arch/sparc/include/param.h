/*	$NetBSD: param.h,v 1.11 1995/02/01 12:37:46 pk Exp $ */

/*
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This software was developed by the Computer Systems Engineering group
 * at Lawrence Berkeley Laboratory under DARPA contract BG 91-66 and
 * contributed to Berkeley.
 *
 * All advertising materials mentioning features or use of this software
 * must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Lawrence Berkeley Laboratory.
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
 *	@(#)param.h	8.1 (Berkeley) 6/11/93
 */

/*
 * Machine dependent constants for Sun-4c (SPARCstation)
 */
#define	MACHINE		"sparc"
#define	MACHINE_ARCH	"sparc"
#define	MID_MACHINE	MID_SPARC

#ifdef KERNEL				/* XXX */
#include <machine/cpu.h>		/* XXX */
#endif					/* XXX */

/*
 * Round p (pointer or byte index) up to a correctly-aligned value for
 * the machine's strictest data type.  The result is u_int and must be
 * cast to any desired pointer type.
 */
#define	ALIGNBYTES	7
#define	ALIGN(p)	(((u_int)(p) + ALIGNBYTES) & ~ALIGNBYTES)

#define SUN4_PGSHIFT	13	/* for a sun4 machine */
#define SUN4CM_PGSHIFT	12	/* for a sun4c or sun4m machine */

/*
 * Three possible cases:
 * 	sun4 only		8192 bytes/page
 *	sun4c/sun4m only	4096 bytes/page
 *	sun4/sun4c/sun4m	either of the above
 * 
 * In the later case NBPG, PGOFSET, and PGSHIFT are encoded in variables
 * initialized early in locore.s.  Since they are variables, rather than
 * simple constants, the kernel will not perform slighly worse.
 */
#if defined(SUN4) && !defined(SUN4C) && !defined(SUN4M)
#define	NBPG		8192		/* bytes/page */
#define	PGOFSET		(NBPG-1)	/* byte offset into page */
#define	PGSHIFT		SUN4_PGSHIFT	/* log2(NBPG) */
#endif
#if !defined(SUN4) && (defined(SUN4C) || defined(SUN4M))
#define	NBPG		4096		/* bytes/page */
#define	PGOFSET		(NBPG-1)	/* byte offset into page */
#define	PGSHIFT		SUN4CM_PGSHIFT	/* log2(NBPG) */
#endif
#if defined(SUN4) && (defined(SUN4C) || defined(SUN4M))
#if defined(KERNEL) && !defined(LOCORE)
extern int nbpg, pgofset, pgshift;
#endif
#define	NBPG		nbpg		/* bytes/page */
#define	PGOFSET		pgofset		/* byte offset into page */
#define	PGSHIFT		pgshift		/* log2(NBPG) */
#endif

#define	KERNBASE	0xf8000000	/* start of kernel virtual space */
#define	KERNTEXTOFF	0xf8004000	/* start of kernel text */

#define	DEV_BSIZE	512
#define	DEV_BSHIFT	9		/* log2(DEV_BSIZE) */
#define	BLKDEV_IOSIZE	2048
#define	MAXPHYS		(64 * 1024)

#define	CLSIZE		1
#define	CLSIZELOG2	0

/* NOTE: SSIZE must be multiple of CLSIZE */
#define	SSIZE		1		/* initial stack size in pages */
#define	USPACE		8192

/*
 * Constants related to network buffer management.
 * MCLBYTES must be no larger than CLBYTES (the software page size), and,
 * on machines that exchange pages of input or output buffers with mbuf
 * clusters (MAPPED_MBUFS), MCLBYTES must also be an integral multiple
 * of the hardware page size.
 */
#define	MSIZE		128		/* size of an mbuf */
#define	MCLBYTES	2048		/* enough for whole Ethernet packet */
#define	MCLSHIFT	11		/* log2(MCLBYTES) */
#define	MCLOFSET	(MCLBYTES - 1)

#ifndef NMBCLUSTERS
#ifdef GATEWAY
#define	NMBCLUSTERS	512		/* map size, max cluster allocation */
#else
#define	NMBCLUSTERS	256		/* map size, max cluster allocation */
#endif
#endif

/*
 * Size of kernel malloc arena in CLBYTES-sized logical pages.
 */
#ifndef	NKMEMCLUSTERS
#define	NKMEMCLUSTERS	(6 * 1024 * 1024 / CLBYTES)
#endif

/* pages ("clicks") (4096 bytes) to disk blocks */
#define	ctod(x)	((x) << (PGSHIFT - DEV_BSHIFT))
#define	dtoc(x)	((x) >> (PGSHIFT - DEV_BSHIFT))
#define	dtob(x)	((x) << DEV_BSHIFT)

/* pages to bytes */
#define	ctob(x)	((x) << PGSHIFT)

/* bytes to pages */
#define	btoc(x)	(((unsigned)(x) + PGOFSET) >> PGSHIFT)

#define	btodb(bytes)		/* calculates (bytes / DEV_BSIZE) */ \
	((unsigned)(bytes) >> DEV_BSHIFT)
#define	dbtob(db)		/* calculates (db * DEV_BSIZE) */ \
	((unsigned)(db) << DEV_BSHIFT)

/*
 * Map a ``block device block'' to a file system block.
 * This should be device dependent, and should use the bsize
 * field from the disk label.
 * For now though just use DEV_BSIZE.
 */
#define	bdbtofsb(bn)	((bn) / (BLKDEV_IOSIZE / DEV_BSIZE))

#ifdef KERNEL
#ifndef LOCORE
#define	DELAY(n)	delay(n)
#endif
#else
#define	DELAY(n)	{ register volatile int N = (n); while (--N > 0); }
#endif

#ifdef KERNEL
extern int cputyp;
extern int cpumod;
#endif
/*
 * Values for the cputyp variable.
 */
#define CPU_SUN4	0
#define CPU_SUN4C	1
#define CPU_SUN4M	2
/*
 * Values for cpumod (cpu model) variable.  XXX currently valid only for sun4
 */
#define SUN4_100	0x22
#define SUN4_200	0x21
#define SUN4_300	0x23
#define SUN4_400	0x24

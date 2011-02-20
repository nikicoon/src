/*	$NetBSD: proc.h,v 1.27 2011/02/20 07:45:47 matt Exp $	*/

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
 * 3. Neither the name of the University nor the names of its contributors
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
 *	@(#)proc.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _MIPS_PROC_H_
#define _MIPS_PROC_H_

#include <sys/param.h>
#include <mips/vmparam.h>

struct lwp;

/*
 * Machine-dependent part of the lwp structure for MIPS
 */
struct trapframe;

struct mdlwp {
	struct trapframe *md_utf;	/* trapframe from userspace */
	vaddr_t	md_ss_addr;		/* single step address for ptrace */
	int	md_ss_instr;		/* single step instruction for ptrace */
	volatile int md_astpending;	/* AST pending on return to userland */
#if USPACE > PAGE_SIZE
	int	md_upte[USPACE/4096];	/* ptes for mapping u page */
#else
	int	md_dpte[USPACE/4096];	/* dummy ptes to keep the same */
#endif
};

struct mdproc {
					/* syscall entry for this process */
	void	(*md_syscall)(struct lwp *, u_int, u_int, vaddr_t);
	int	md_abi;			/* which ABI is this process using? */
};

/* md_flags */

#ifdef _KERNEL
struct lwp;

/* kernel single-step emulation */
int	mips_singlestep(struct lwp *);

#define	LWP0_CPU_INFO	&cpu_info_store	/* staticly set in lwp0 */
#endif /* _KERNEL */

#endif /* _MIPS_PROC_H_ */

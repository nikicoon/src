/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz.
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
 *	from: @(#)SYS.h	5.5 (Berkeley) 5/7/91
 *	$NetBSD: SYS.h,v 1.4 2002/05/26 12:24:59 wiz Exp $
 */

/*
 * XXXfvdl change to use syscall/sysret.
 */

#include <machine/asm.h>
#include <sys/syscall.h>

#ifdef __STDC__
#define SYSTRAP(x)	movl $(SYS_ ## x),%eax; int $0x80
#else
#define SYSTRAP(x)	movl $(SYS_/**/x),%eax; int $0x80
#endif

#define CERROR		_C_LABEL(__cerror)
#define CURBRK		_C_LABEL(__curbrk)

#define _SYSCALL_NOERROR(x,y)						\
	ENTRY(x);							\
	SYSTRAP(y)

#ifdef PIC
#define _SYSCALL(x,y)							\
	.text; _ALIGN_TEXT;						\
	2: mov PIC_GOT(CERROR), %rcx;					\
	jmp *%rcx;							\
	_SYSCALL_NOERROR(x,y);						\
	jc 2b
#else
#define _SYSCALL(x,y)							\
	.text; _ALIGN_TEXT;						\
	2: jmp CERROR;							\
	_SYSCALL_NOERROR(x,y);						\
	jc 2b
#endif

#define SYSCALL_NOERROR(x)						\
	_SYSCALL_NOERROR(x,x)

#define SYSCALL(x)							\
	_SYSCALL(x,x)

#define PSEUDO_NOERROR(x,y)						\
	_SYSCALL_NOERROR(x,y);						\
	ret

#define PSEUDO(x,y)							\
	_SYSCALL(x,y);							\
	ret

#define RSYSCALL_NOERROR(x)						\
	PSEUDO_NOERROR(x,x)

#define RSYSCALL(x)							\
	PSEUDO(x,x)

#define	WSYSCALL(weak,strong)						\
	WEAK_ALIAS(weak,strong);					\
	PSEUDO(strong,weak)

	.globl	CERROR

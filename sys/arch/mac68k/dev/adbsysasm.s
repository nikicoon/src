/*	$NetBSD: adbsysasm.s,v 1.7 1998/10/23 01:16:23 ender Exp $	*/

/*-
 * Copyright (C) 1994	Bradley A. Grantham
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Bradley A. Grantham.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* 
 * ADB subsystem routines in assembly
 */

/* This routine is called when a keyboard has sent us some data. */
/* (provided it has been set up with SetADBInfo) */
	.global	_adb_kbd_asmcomplete
	.global	_kbd_adbcomplete
_adb_kbd_asmcomplete:
	moveml	#0xc0c0, sp@-	| save scratch regs
	movl	d0, sp@-	/* ADB command byte */
	movl	a2, sp@-	/* data area pointer */
	/*	a1 is the pointer to this routine itself. */
	movl	a0, sp@-	/* device data buffer */
	jbsr	_kbd_adbcomplete
	addl	#12, sp		/* pop params */
	moveml	sp@+, #0x0303	| restore scratch regs
	rts

/* This routine is called when a mouse has sent us some data. */
/* (provided it has been set up with SetADBInfo) */
	.global	_adb_ms_asmcomplete
	.global	_ms_adbcomplete
_adb_ms_asmcomplete:
	moveml	#0xc0c0, sp@-	| save scratch regs
	movl	d0, sp@-	/* ADB command byte */
	movl	a2, sp@-	/* data area pointer */
	/*	a1 is the pointer to this routine itself. */
	movl	a0, sp@-	/* device data buffer */
	jbsr	_ms_adbcomplete
	addl	#12, sp		/* pop params */
	moveml	sp@+, #0x0303	| restore scratch regs
	rts


_adb_jadbprochello:
	.asciz	"adb: hello from adbproc\n"
	.even


	.global	_adb_jadbproc
_adb_jadbproc:
#if defined(ADB_DEBUG) && 0
	moveml	#0xc0c0, sp@-	| save scratch regs
	movl	_adb_jadbprochello, sp@-
	jbsr	_printf
	addl	#4, sp		/* pop params */
	moveml	sp@+, #0x0303	| restore scratch regs
#endif
		/* Don't do anything; adbattach fixes dev info for us. */
	rts

	/* ADBOp's completion routine used by extdms_init() in adbsys.c. */
	.global	_extdms_complete
_extdms_complete:
	movl	#-1,a2@		| set done flag
	rts

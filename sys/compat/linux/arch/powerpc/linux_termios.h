/*	$NetBSD: linux_termios.h,v 1.3 2001/01/20 18:59:55 manu Exp $ */

/*-
 * Copyright (c) 1998, 2001 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Eric Haszlakiewicz and Emmanuel Dreyfus.
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
 *	This product includes software developed by the NetBSD
 *	Foundation, Inc. and its contributors.
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

#ifndef _POWERPC_LINUX_TERMIOS_H
#define _POWERPC_LINUX_TERMIOS_H

#define LINUX_NCC 10

/*
 * Enable optional part of struct termios in sys/compat/linux/common/termios.h
 */
#define LINUX_LARGE_STRUCT_TERMIOS

/* 
 * From Linux's include/asm-ppc/termios.h 
 */
#define LINUX_TCGETS		_LINUX_IOR('t', 19, struct linux_termios)
#define LINUX_TCSETS		_LINUX_IOW('t', 20, struct linux_termios)
#define LINUX_TCSETSW	_LINUX_IOW('t', 21, struct linux_termios)
#define LINUX_TCSETSF	_LINUX_IOW('t', 22, struct linux_termios)
#define LINUX_TCGETA		_LINUX_IOR('t', 23, struct linux_termio)
#define LINUX_TCSETA		_LINUX_IOW('t', 24, struct linux_termio)
#define LINUX_TCSETAW	_LINUX_IOW('t', 25, struct linux_termio)
#define LINUX_TCSETAF	_LINUX_IOW('t', 28, struct linux_termio)
#define LINUX_TCSBRK		_LINUX_IO('t', 29)
#define LINUX_TCXONC		_LINUX_IO('t', 30)
#define LINUX_TCFLSH		_LINUX_IO('t', 31)
#define LINUX_TIOCEXCL	0x540c 
#define LINUX_TIOCNXCL	0x540d 
#define LINUX_TIOCSCTTY	0x540e 
#define LINUX_TIOCGPGRP	_LINUX_IOR('t', 119, int)
#define LINUX_TIOCSPGRP	_LINUX_IOW('t', 118, int)
#define LINUX_TIOCOUTQ	_LINUX_IOR('t', 115, int)
#define LINUX_TIOCSTI		0x5412 
#define LINUX_TIOCGWINSZ 	_LINUX_IOR('t', 104, struct linux_winsize)
#define LINUX_TIOCSWINSZ 	_LINUX_IOW('t', 103, struct linux_winsize)
#define LINUX_TIOCMGET		0x5415 
#define LINUX_TIOCMBIS		0x5416 
#define LINUX_TIOCMBIC		0x5417 
#define LINUX_TIOCMSET		0x5418 
#define LINUX_TIOCGSOFTCAR	0x5419 
#define LINUX_TIOCSSOFTCAR	0x541a 
#define LINUX_FIONREAD		_LINUX_IOR('f', 127, int)
#define LINUX_TIOCINQ		LINUX_FIONREAD
#define LINUX_TIOCLINUX		0x541c 
#define LINUX_TIOCCONS		0x541d 
#define LINUX_TIOCGSERIAL	0x541e 
#define LINUX_TIOCSSERIAL	0x541f 
#define LINUX_TIOCPKT		0x5420 
#define LINUX_FIONBIO		_LINUX_IOW('f', 126, int)
#define LINUX_TIOCNOTTY		0x5422 
#define LINUX_TIOCSETD		0x5423
#define LINUX_TIOCGETD		0x5424
#define LINUX_TCSBRKP		0x5425
#define LINUX_TIOCTTYGSTRUCT	0x5426

#define LINUX_FIONCLEX		_LINUX_IO('f', 2)
#define LINUX_FIOCLEX		_LINUX_IO('f', 1)
#define LINUX_FIOASYNC		_LINUX_IOW('f', 125, int)
#define LINUX_TIOCSERCONFIG	0x5453
#define LINUX_TIOCSERGWILD		0x5454
#define LINUX_TIOCSERSWILD		0x5455
#define LINUX_TIOCGLCKTRMIOS	0x5456
#define LINUX_TIOCSLCKTRMIOS	0x5457
#define LINUX_TIOCSERGSTRUCT	0x5458
#define LINUX_TIOCSERGETLSR	0x5459
#define LINUX_TIOCSERGETMULTI 0x545a
#define LINUX_TIOCSERSETMULTI 0x545b

/* 
 * linux_termios c_cc chars: 
 * From Linux include/asm-ppc/termios.h AND include/asm-ppc/termbits.h 
 */
#define LINUX_VINTR 	0
#define LINUX_VQUIT 	1
#define LINUX_VERASE 2
#define LINUX_VKILL 	3
#define LINUX_VEOF 	4
#define LINUX_VMIN 	5
#define LINUX_VEOL 	6
#define LINUX_VTIME 	7
#define LINUX_VEOL2 	8
#define LINUX_VSWTC 	9

/* 
 * The following is from include/asm-ppc/termbits.h only 
 */
#define LINUX_VWERASE   10
#define LINUX_VREPRINT  11
#define LINUX_VSUSP     12
#define LINUX_VSTART    13
#define LINUX_VSTOP     14
#define LINUX_VLNEXT    15    
#define LINUX_VDISCARD  16

/* 
 * Old style linux_termio 
 * XXX Not found anywhere in Linux 
 */
#define	LINUX_OLD_VINTR	LINUX_VINTR
#define	LINUX_OLD_VQUIT	LINUX_VQUIT
#define	LINUX_OLD_VERASE	LINUX_VERASE
#define	LINUX_OLD_VKILL	LINUX_VKILL
#define	LINUX_OLD_VEOF		LINUX_VEOF
#define	LINUX_OLD_VMIN		LINUX_VMIN
#define	LINUX_OLD_VEOL		LINUX_VEOL
#define	LINUX_OLD_VTIME	LINUX_VTIME
#define	LINUX_OLD_VEOL2	LINUX_VEOL2
#define	LINUX_OLD_VSWTC	LINUX_VSWTC

/* 
 * All the remaining stuff is from  Linux's include/asm-ppc/termbits.h 
 * Linux c_iflag masks 
 */
#define LINUX_IGNBRK	0x0000001
#define LINUX_BRKINT	0x0000002
#define LINUX_IGNPAR	0x0000004
#define LINUX_PARMRK	0x0000010
#define LINUX_INPCK	0x0000020
#define LINUX_ISTRIP	0x0000040
#define LINUX_INLCR	0x0000100
#define LINUX_IGNCR	0x0000200
#define LINUX_ICRNL	0x0000400
#define LINUX_IXON	0x0001000
#define LINUX_IXOFF	0x0002000
#define LINUX_IXANY	0x0004000
#define LINUX_IUCLC	0x0010000
#define LINUX_IMAXBEL 0x0002000

/* 
 * Linux c_oflag masks 
 */
#define LINUX_OPOST	0x0000001
#define LINUX_ONLCR	0x0000002
#define LINUX_OLCUC	0x0000004
#define LINUX_OCRNL	0x0000010
#define LINUX_ONOCR	0x0000020
#define LINUX_ONLRET	0x0000040

#define LINUX_OFILL  0x00000100
#define LINUX_OFDEL  0x00000200
#define LINUX_NLDLY  0x00001400
#define LINUX_NL0  	0x00000000
#define LINUX_NL1  	0x00000400
#define LINUX_NL2  	0x00001000
#define LINUX_NL3  	0x00001400
#define LINUX_TABDLY 0x00006000
#define LINUX_TAB0 	0x00000000
#define LINUX_TAB1 	0x00002000
#define LINUX_TAB2 	0x00004000
#define LINUX_TAB3 	0x00006000
#define LINUX_CRDLY  0x00030000
#define LINUX_CR0  	0x00000000
#define LINUX_CR1  	0x00010000
#define LINUX_CR2  	0x00020000
#define LINUX_CR3  	0x00030000
#define LINUX_FFDLY  0x00040000
#define LINUX_FF0  	0x00000000
#define LINUX_FF1  	0x00040000
#define LINUX_BSDLY  0x00100000
#define LINUX_BS0  	0x00000000
#define LINUX_BS1  	0x00100000
#define LINUX_VTDLY  0x00200000
#define LINUX_VT0  	0x00000000
#define LINUX_VT1  	0x00200000
/* Hmm.. Linux/i386 considers this part of TABDLY.. */
#define LINUX_XTABS  0x01000000 

/* 
 * Linux c_cflag bit masks 
 * XXX not found in Linux, but it is needed to build
 */
#define LINUX_NSPEEDS   16 

#define LINUX_CBAUD	0x0000377
#define LINUX_B0		0x0000000     /* hang up */
#define LINUX_B50  	0x0000001
#define LINUX_B75   	0x0000002
#define LINUX_B110  	0x0000003
#define LINUX_B134  	0x0000004
#define LINUX_B150  	0x0000005
#define LINUX_B200  	0x0000006
#define LINUX_B300  	0x0000007
#define LINUX_B600  	0x0000010
#define LINUX_B1200 	0x0000011
#define LINUX_B1800 	0x0000012
#define LINUX_B2400 	0x0000013
#define LINUX_B4800 	0x0000014
#define LINUX_B9600 	0x0000015
#define LINUX_B19200	0x0000016
#define LINUX_B38400	0x0000017
#define LINUX_EXTA 	B19200
#define LINUX_EXTB 	B38400
#define LINUX_CBAUDEX	0x0000000
#define LINUX_B57600 	0x00020
#define LINUX_B115200	0x00021
#define LINUX_B230400	0x00022
#define LINUX_B460800	0x00023
#define LINUX_B500000	0x00024
#define LINUX_B576000	0x00025
#define LINUX_B921600	0x00026
#define LINUX_B1000000	0x00027
#define LINUX_B1152000	0x00030
#define LINUX_B1500000	0x00031
#define LINUX_B2000000	0x00032
#define LINUX_B2500000	0x00033
#define LINUX_B3000000	0x00034
#define LINUX_B3500000	0x00035
#define LINUX_B4000000	0x00036

#define LINUX_CSIZE	0x00001400
#define LINUX_CS5		0x00000000
#define LINUX_CS6		0x00000400
#define LINUX_CS7 	0x00001000
#define LINUX_CS8		0x00001400 
   
#define LINUX_CSTOPB 0x00002000 
#define LINUX_CREAD  0x00004000 
#define LINUX_PARENB 0x00010000 
#define LINUX_PARODD 0x00020000 
#define LINUX_HUPCL  0x00040000 
   
#define LINUX_CLOCAL 	0x00100000
#define LINUX_CRTSCTS	0x020000000000    /* flow control */

/* 
 * Linux c_lflag masks 
 */
#define LINUX_ISIG   0x00000080
#define LINUX_ICANON 0x00000100
#define LINUX_XCASE  0x00004000
#define LINUX_ECHO   0x00000008
#define LINUX_ECHOE  0x00000002
#define LINUX_ECHOK  0x00000004
#define LINUX_ECHONL 0x00000010
#define LINUX_NOFLSH 0x80000000
#define LINUX_TOSTOP 0x00400000
#define LINUX_ECHOCTL 0x00000040
#define LINUX_ECHOPRT 0x00000020
#define LINUX_ECHOKE 0x00000001
#define LINUX_FLUSHO 0x00800000
#define LINUX_PENDIN 0x20000000
#define LINUX_IEXTEN 0x00000400

#endif /* !_POWERPC_LINUX_TERMIOS_H */

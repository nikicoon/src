/*-
 * Copyright (C) 1993	Allen K. Briggs, Chris P. Caputo,
 *			Michael L. Finch, Bradley A. Grantham, and
 *			Lawrence A. Kesteloot
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
 *	This product includes software developed by the Alice Group.
 * 4. The names of the Alice Group or any of its members may not be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE ALICE GROUP ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE ALICE GROUP BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ident "$Id: via.h,v 1.1.1.1 1993/09/29 06:09:14 briggs Exp $"
/*

	Prototype VIA control definitions

	06/04/92,22:33:57 BG Let's see what I can do.

*/


	/* VIA1 data register A */
#define DA1I_vSCCWrReq	0x80
#define DA1O_vPage2	0x40
#define DA1I_CPU_ID1	0x40
#define DA1O_vHeadSel	0x20
#define DA1O_vOverlay	0x10
#define DA1O_vSync	0x08

	/* VIA1 data register B */
#define DB1I_Par_Err	0x80
#define DB1O_vSndEnb	0x80
#define DB1O_Par_Enb	0x40
#define DB1O_vFDesk2	0x20
#define DB1O_vFDesk1	0x10
#define DB1I_vFDBInt	0x08
#define DB1O_rTCEnb	0x04
#define DB1O_rTCCLK	0x02
#define DB1O_rTCData	0x01
#define DB1I_rTCData	0x01

	/* VIA2 data register A */
#define DA2O_v2Ram1	0x80
#define DA2O_v2Ram0	0x40
#define DA2I_v2IRQE	0x20
#define DA2I_v2IRQD	0x10
#define DA2I_v2IRQC	0x08
#define DA2I_v2IRQB	0x04
#define DA2I_v2IRQA	0x02
#define DA2I_v2IRQ9	0x01

	/* VIA2 data register B */
#define DB2O_v2VBL	0x80
#define DB2O_Par_Test	0x80
#define DB2I_v2SNDEXT	0x40
#define DB2I_v2TM0A	0x20
#define DB2I_v2TM1A	0x10
#define DB2I_vFC3	0x08
#define DB2O_vFC3	0x08
#define DB2O_v2PowerOff	0x04
#define DB2O_v2BusLk	0x02
#define DB2O_vCDis	0x01
#define DB2O_CEnable	0x01

	/* VIA1 interrupt bits */
#define V1IF_IRQ	0x80
#define V1IF_T1		0x40
#define V1IF_T2		0x20
#define V1IF_ADBCLK	0x10
#define V1IF_ADBDATA	0x08
#define V1IF_ADBRDY	0x04
#define V1IF_VBLNK	0x02
#define V1IF_ONESEC	0x01

	/* VIA2 interrupt bits */
#define V2IF_IRQ	0x80
#define V2IF_T1		0x40
#define V2IF_T2		0x20
#define V2IF_ASC	0x10
#define V2IF_SCSIIRQ	0x08
#define V2IF_EXPIRQ	0x04
#define V2IF_SLOTINT	0x02
#define V2IF_SCSIDRQ	0x01

#define VIA1_INTS	(V1IF_T1 | V1IF_ADBRDY)
#define VIA2_INTS	(V2IF_T1 | V2IF_ASC | V2IF_SCSIIRQ | V2IF_SLOTINT | \
			 V2IF_SCSIDRQ)

#define ACR_T1LATCH	0x40

#define VIA1_addr	0x50000000
#define VIA2_addr	0x50002000


#define VIA1		0
#define VIA2		1
#define VIABUFA		0
#define VIABUFB		1


#define vBufA		7680
#define vBufB		0
#define vDirA		1536
#define vDirB		1024
#define vT1C		2048
#define vT1CH		2560
#define vT1L		3072
#define vT1LH		3584
#define vT2C		4096
#define vT2CH		4608
#define vSR		5120
#define vACR		5632
#define vPCR		6144
#define vIFR		6656 /* +3 */
#define vIER		7168 /* +0x13 */


#define via_reg(v, r) (*((volatile unsigned char *)VIA1_addr+(v)*0x2000+(r)))


void VIA_initialize();

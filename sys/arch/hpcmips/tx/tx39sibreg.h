/*	$NetBSD: tx39sibreg.h,v 1.2 2000/03/03 19:54:37 uch Exp $ */

/*
 * Copyright (c) 2000, by UCHIYAMA Yasushi
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the developer may NOT be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
/*
 * Toshiba TX3912 SIB module
 */

#define	TX39_SIBSIZE_REG	0x060 /* W */
#define	TX39_SIBSNDRXSTART_REG	0x064 /* W */
#define	TX39_SIBSNDTXSTART_REG	0x068 /* W */
#define	TX39_SIBTELRXSTART_REG	0x06c /* W */
#define	TX39_SIBTELTXSTART_REG	0x070 /* W */
#define	TX39_SIBCTRL_REG	0x074 /* R/W */
#define	TX39_SIBSNDHOLD_REG	0x078 /* R/W */
#define	TX39_SIBTELHOLD_REG	0x07c /* R/W */
#define	TX39_SIBSF0CTRL_REG	0x080 /* R/W */
#define	TX39_SIBSF1CTRL_REG	0x084 /* R/W */
#define	TX39_SIBSF0STAT_REG	0x088 /* R */
#define	TX39_SIBSF1STAT_REG	0x08c /* R */
#define	TX39_SIBDMACTRL_REG	0x090 /* R/W */

/*
 *	SIB DMA
 */
#define TX39_SIBDMA_SIZE	16384

/*
 *	SIB Size Register
 */
#define TX39_SIBSIZE_SND_SHIFT	18
#define TX39_SIBSIZE_TEL_SHIFT	2
#define TX39_SIBSIZE_MASK	0xfff

#define TX39_SIBSIZE_SNDSIZE_SET(cr, val)				\
	((cr) | (((((val) >> 2) - 1) << TX39_SIBSIZE_SND_SHIFT) &	\
	(TX39_SIBSIZE_MASK << TX39_SIBSIZE_SND_SHIFT)))
#define TX39_SIBSIZE_TELSIZE_SET(cr, val)				\
	((cr) | (((((val) >> 2) - 1) << TX39_SIBSIZE_TEL_SHIFT) &	\
	(TX39_SIBSIZE_MASK << TX39_SIBSIZE_TEL_SHIFT)))

/*
 *	SIB Sound RX Start Register
 *	 [1:0] reserved
 */
/*
 *	SIB Sound TX Start Register
 *	 [1:0] reserved
 */
/*
 *	SIB Telecom RX Start Register
 *	 [1:0] reserved
 */
/*
 *	SIB Telecom TX Start Register
 *	 [1:0] reserved
 */
/*
 *	SIB Control Register
 */
#define	TX39_SIBCTRL_SIBIRQ	0x80000000
#define	TX39_SIBCTRL_ENCNTTEST	0x40000000 /* Don't set */
#define	TX39_SIBCTRL_ENDMATEST	0x20000000 /* Don't set */
#define	TX39_SIBCTRL_SNDMONO	0x10000000
#define	TX39_SIBCTRL_RMONOSNDIN	0x08000000

#define TX39_SIBCTRL_SCLKDIV_SHIFT	24
#define TX39_SIBCTRL_SCLKDIV_MASK	0x7
#define TX39_SIBCTRL_SCLKDIV(cr) \
	(((cr) >> TX39_SIBCTRL_SCLKDIV_SHIFT) & \
	TX39_SIBCTRL_SCLKDIV_MASK)
#define TX39_SIBCTRL_SCLKDIV_SET(cr, val) \
	((cr) | (((val) << TX39_SIBCTRL_SCLKDIV_SHIFT) & \
	(TX39_SIBCTRL_SCLKDIV_MASK << TX39_SIBCTRL_SCLKDIV_SHIFT)))

#define	TX39_SIBCTRL_TEL16	0x00800000

#define TX39_SIBCTRL_TELFSDIV_SHIFT	16
#define TX39_SIBCTRL_TELFSDIV_MASK	0x7f
#define TX39_SIBCTRL_TELFSDIV(cr) \
	(((cr) >> TX39_SIBCTRL_TELFSDIV_SHIFT) & \
	TX39_SIBCTRL_TELFSDIV_MASK)
#define TX39_SIBCTRL_TELFSDIV_SET(cr, val) \
	((cr) | (((val) << TX39_SIBCTRL_TELFSDIV_SHIFT) & \
	(TX39_SIBCTRL_TELFSDIV_MASK << TX39_SIBCTRL_TELFSDIV_SHIFT)))

#define	TX39_SIBCTRL_SND16	0x00008000

#define TX39_SIBCTRL_SNDFSDIV_SHIFT	8
#define TX39_SIBCTRL_SNDFSDIV_MASK	0x7f
#define TX39_SIBCTRL_SNDFSDIV(cr) \
	(((cr) >> TX39_SIBCTRL_SNDFSDIV_SHIFT) & \
	TX39_SIBCTRL_SNDFSDIV_MASK)
#define TX39_SIBCTRL_SNDFSDIV_SET(cr, val) \
	((cr) | (((val) << TX39_SIBCTRL_SNDFSDIV_SHIFT) & \
	(TX39_SIBCTRL_SNDFSDIV_MASK << TX39_SIBCTRL_SNDFSDIV_SHIFT)))

#define	TX39_SIBCTRL_SELTELSF1	0x00000080
#define	TX39_SIBCTRL_SELSNDSF1	0x00000040
#define	TX39_SIBCTRL_ENTEL	0x00000020
#define	TX39_SIBCTRL_ENSND	0x00000010
#define	TX39_SIBCTRL_SIBLOOP	0x00000008
#define	TX39_SIBCTRL_ENSF1	0x00000004
#define	TX39_SIBCTRL_ENSF0	0x00000002
#define	TX39_SIBCTRL_ENSIB	0x00000001

/*
 *	SIB Sound RX/TX Holding Register
 */
/*
 *	SIB Telecom RX/TX Holding Register
 */

/*
 *	SIB Subframe 0 Control Register
 *	SIB Subframe 0 Status Register
 */
/* Control/Status bit, field definition (See also UCB1200) */
#define TX39_SIBSF0_REGADDR_SHIFT	27
#define TX39_SIBSF0_REGADDR_MASK	0xf
#define TX39_SIBSF0_REGADDR(cr) \
	(((cr) >> TX39_SIBSF0_REGADDR_SHIFT) & \
	TX39_SIBSF0_REGADDR_MASK)
#define TX39_SIBSF0_REGADDR_SET(cr, val) \
	((cr) | (((val) << TX39_SIBSF0_REGADDR_SHIFT) & \
	(TX39_SIBSF0_REGADDR_MASK << TX39_SIBSF0_REGADDR_SHIFT)))

#define	TX39_SIBSF0_WRITE	0x04000000
#define	TX39_SIBSF0_SNDVALID	0x00020000
#define	TX39_SIBSF0_TELVALID	0x00010000

#define TX39_SIBSF0_REGDATA_SHIFT	0
#define TX39_SIBSF0_REGDATA_MASK	0xffff
#define TX39_SIBSF0_REGDATA(cr) \
	(((cr) >> TX39_SIBSF0_REGDATA_SHIFT) & \
	TX39_SIBSF0_REGDATA_MASK)
#define TX39_SIBSF0_REGDATA_SET(cr, val) \
	((cr) | (((val) << TX39_SIBSF0_REGDATA_SHIFT) & \
	(TX39_SIBSF0_REGDATA_MASK << TX39_SIBSF0_REGDATA_SHIFT)))
#define TX39_SIBSF0_REGDATA_CLR(cr) \
	((cr) &= ~(TX39_SIBSF0_REGDATA_MASK << TX39_SIBSF0_REGDATA_SHIFT))

/*
 *	SIB Subframe 1 Control Register
 */
#define	TX39_SIBSF1CTRL_MUTE	0x04000000
#define	TX39_SIBSF1CTRL_MUXL	0x02000000
#define	TX39_SIBSF1CTRL_MUXR	0x01000000

#define TX39_SIBSF1CTRL_ADCGAINL_SHIFT	20
#define TX39_SIBSF1CTRL_ADCGAINL_MASK	0xf
#define TX39_SIBSF1CTRL_ADCGAINL_SET(cr, val) \
	((cr) | (((val) << TX39_SIBSF1CTRL_ADCGAINL_SHIFT) & \
	(TX39_SIBSF1CTRL_ADCGAINL_MASK << TX39_SIBSF1CTRL_ADCGAINL_SHIFT)))

#define TX39_SIBSF1CTRL_ADCGAINR_SHIFT	16
#define TX39_SIBSF1CTRL_ADCGAINR_MASK	0xf
#define TX39_SIBSF1CTRL_ADCGAINR_SET(cr, val) \
	((cr) | (((val) << TX39_SIBSF1CTRL_ADCGAINR_SHIFT) & \
	(TX39_SIBSF1CTRL_ADCGAINR_MASK << TX39_SIBSF1CTRL_ADCGAINR_SHIFT)))

#define TX39_SIBSF1CTRL_DACATTNL_SHIFT	8
#define TX39_SIBSF1CTRL_DACATTNL_MASK	0xf
#define TX39_SIBSF1CTRL_DACATTNL_SET(cr, val) \
	((cr) | (((val) << TX39_SIBSF1CTRL_DACATTNL_SHIFT) & \
	(TX39_SIBSF1CTRL_DACATTNL_MASK << TX39_SIBSF1CTRL_DACATTNL_SHIFT)))

#define TX39_SIBSF1CTRL_DACATTNR_SHIFT	4
#define TX39_SIBSF1CTRL_DACATTNR_MASK	0xf
#define TX39_SIBSF1CTRL_DACATTNR_SET(cr, val) \
	((cr) | (((val) << TX39_SIBSF1CTRL_DACATTNR_SHIFT) & \
	(TX39_SIBSF1CTRL_DACATTNR_MASK << TX39_SIBSF1CTRL_DACATTNR_SHIFT)))

#define TX39_SIBSF1CTRL_DIGITALOUT_SHIFT	0
#define TX39_SIBSF1CTRL_DIGITALOUT_MASK	0xf
#define TX39_SIBSF1CTRL_DIGITALOUT_SET(cr, val) \
	((cr) | (((val) << TX39_SIBSF1CTRL_DIGITALOUT_SHIFT) & \
	(TX39_SIBSF1CTRL_DIGITALOUT_MASK << TX39_SIBSF1CTRL_DIGITALOUT_SHIFT)))

/*
 *	SIB Subframe 1 Status Register
 */
#define	TX39_SIBSF1STAT_ADCVALID    0x04000000
#define	TX39_SIBSF1STAT_ADCCLIPL    0x02000000
#define	TX39_SIBSF1STAT_ADCCLIPR    0x01000000

#define TX39_SIBSF1STAT_ERROR_SHIFT	20
#define TX39_SIBSF1STAT_ERROR_MASK	0xf
#define TX39_SIBSF1STAT_ERROR_SET(cr, val) \
	((cr) | (((val) << TX39_SIBSF1STAT_ERROR_SHIFT) & \
	(TX39_SIBSF1STAT_ERROR_MASK << TX39_SIBSF1STAT_ERROR_SHIFT)))

#define TX39_SIBSF1STAT_REVISION_SHIFT	16
#define TX39_SIBSF1STAT_REVISION_MASK	0xf
#define TX39_SIBSF1STAT_REVISION_SET(cr, val) \
	((cr) | (((val) << TX39_SIBSF1STAT_REVISION_SHIFT) & \
	(TX39_SIBSF1STAT_REVISION_MASK << TX39_SIBSF1STAT_REVISION_SHIFT)))

#define TX39_SIBSF1STAT_DIGITALIN_SHIFT	0
#define TX39_SIBSF1STAT_DIGITALIN_MASK	0xf
#define TX39_SIBSF1STAT_DIGITALIN_SET(cr, val) \
	((cr) | (((val) << TX39_SIBSF1STAT_DIGITALIN_SHIFT) & \
	(TX39_SIBSF1STAT_DIGITALIN_MASK << TX39_SIBSF1STAT_DIGITALIN_SHIFT)))

/*
 *	SIB DMA Control Register
 */
#define	TX39_SIBDMACTRL_SNDBUFF1TIME	0x80000000
#define	TX39_SIBDMACTRL_SNDDMALOOP	0x40000000

#define TX39_SIBDMACTRL_SNDDMAPTR_SHIFT	18
#define TX39_SIBDMACTRL_SNDDMAPTR_MASK	0xfff
#define TX39_SIBDMACTRL_SNDDMAPTR(cr) \
	(((cr) >> TX39_SIBDMACTRL_SNDDMAPTR_SHIFT) & \
	TX39_SIBDMACTRL_SNDDMAPTR_MASK)

#define	TX39_SIBDMACTRL_ENDMARXSND	0x00020000
#define	TX39_SIBDMACTRL_ENDMATXSND	0x00010000
#define	TX39_SIBDMACTRL_TELBUFF1TIME	0x00008000
#define	TX39_SIBDMACTRL_TELDMALOOP	0x00004000

#define TX39_SIBDMACTRL_TELDMAPTR_SHIFT	2
#define TX39_SIBDMACTRL_TELDMAPTR_MASK	0xfff
#define TX39_SIBDMACTRL_TELDMAPTR(cr) \
	(((cr) >> TX39_SIBDMACTRL_TELDMAPTR_SHIFT) & \
	TX39_SIBDMACTRL_TELDMAPTR_MASK)

#define	TX39_SIBDMACTRL_ENDMARXTEL	0x00000002
#define	TX39_SIBDMACTRL_ENDMATXTEL	0x00000001


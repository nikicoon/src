/*	$NetBSD: if_ixreg.h,v 1.3 2001/01/22 22:28:47 bjh21 Exp $	*/

/*
 * Copyright (c) 1993, 1994, 1995
 *	Rodney W. Grimes, Milwaukie, Oregon  97222.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer as
 *    the first lines of this file unmodified.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Rodney W. Grimes.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY RODNEY W. GRIMES ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL RODNEY W. GRIMES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Definitions for EtherExpress 16
 */

#define IX_IOSIZE	16	/* card has 16 registers in IO space */

#define IX_DATAPORT  		0x00	/* shared memory data port */
#define IX_WRITEPTR  		0x02	/* shared memory write pointer */
#define IX_READPTR  		0x04	/* shared memory read pointer */

#define IX_ATTN			0x06	/* channel attention control */
#define IX_IRQ			0x07	/* IRQ configuration */
#define IX_IRQ_ENABLE		0x08	/* enable board interrupts */

#define IX_SHADOWPTR  		0x08	/* shadow memory pointer */

#define IX_MEMDEC		0x0a	/* memory decode */
#define IX_MCTRL		0x0b	/* memory control */
#define IX_MCTRL_FMCS16		0x10	/* MEMCS16- for F000 */

#define IX_MPCTRL		0x0c	/* memory page control */
#define IX_CONFIG		0x0d	/* config register */
#define IX_BART_LOOPBACK	0x02	/* loopback, 0=none, 1=loopback */
#define IX_BART_IOCHRDY_LATE	0x10	/* iochrdy late control bit */
#define IX_BART_IO_TEST_EN	0x20	/* enable iochrdy timing test */
#define IX_BART_IO_RESULT	0x40	/* result of the iochrdy test */
#define IX_BART_MCS16_TEST	0x80	/* enable memcs16 select test */

#define IX_ECTRL		0x0e	/* eeprom control */
#define IX_ECTRL_EESK		0x01	/* EEPROM clock bit */
#define IX_ECTRL_EECS		0x02	/* EEPROM chip select */
#define IX_ECTRL_EEDI		0x04	/* EEPROM data in bit */
#define IX_ECTRL_EEDO		0x08	/* EEPROM data out bit */
#define IX_RESET_ASIC		0x40	/* reset ASIC (bart) pin */
#define IX_RESET_586		0x80	/* reset 82586 pin */
#define IX_ECTRL_MASK		0xb2	/* and'ed with ECTRL to enable read  */

#define IX_MECTRL		0x0f	/* memory control, 0xe000 seg 'W' */
#define IX_ID_PORT		0x0f	/* auto-id port 'R' */

#define IX_ID			0xbaba	/* known id of EE16 */

#define IX_EEPROM_READ	0x06	/* EEPROM read opcode */
#define IX_EEPROM_OPSIZE1	0x03	/* size of EEPROM opcodes */
#define IX_EEPROM_ADDR_SIZE	0x06	/* size of EEPROM address */

/* Locations in the EEPROM */
#define IX_EEPROM_CONFIG1	0x00	/* Configuration register 1 */
#define IX_EEPROM_MEDIA_EXT	0x1000	/* Using external transceiver */

#define IX_EEPROM_IRQ		0xE000	/* Encoded IRQ */
#define IX_EEPROM_IRQ_SHIFT	13	/* To shift IRQ to lower bits */
#define IX_EEPROM_LOCK_ADDR	0x01	/* contains the lock bit */
#define IX_EEPROM_LOCKED	0x01	/* means that it is locked */

#define IX_EEPROM_ENET_LOW	0x02	/* Ethernet address, low word */
#define IX_EEPROM_ENET_MID	0x03	/* Ethernet address, middle word */
#define IX_EEPROM_ENET_HIGH	0x04	/* Ethernet address, high word */

#define IX_EEPROM_MEDIA		0x05	/* Selects between TP/BNC */
#define IX_EEPROM_MEDIA_TP	0x01	/* if ON, using TP, else BNC */


/*	$NetBSD: atapi_tape.h,v 1.3 2005/01/31 23:06:41 reinoud Exp $	*/


/*-
 * Copyright (c) 1998,1999,2000,2001 S�ren Schmidt
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification, immediately at the beginning of the file.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
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
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD: src/sys/dev/ata/atapi-tape.h,v 1.15 2001/03/14 12:05:44 sos Exp $
 */

/* defines for the device specific byte in the mode select/sense header */
#define SMH_DSP_BUFF_MODE       0x70
#define SMH_DSP_BUFF_MODE_OFF   0x00
#define SMH_DSP_BUFF_MODE_ON    0x10
#define SMH_DSP_BUFF_MODE_MLTI  0x20
#define SMH_DSP_WRITE_PROT      0x80

/* ATAPI tape drive Capabilities and Mechanical Status Page */
struct atapi_cappage {
    /* mode page data header */
    struct scsipi_mode_header header;
    /* capabilities page */
    uint8_t	page_code;
#define ATAPI_TAPE_CAP_PAGE	0x2a
    uint8_t	page_length;			/* page Length == 0x12 */
    uint8_t	reserved2;
    uint8_t	reserved3;
    uint8_t	cap1;
#define ATAPI_TAPE_CAP_PAGE_RO	0x01;		/* read Only Mode */
#define ATAPI_TAPE_CAP_PAGE_REV	0x20;		/* supports reverse direction */
    uint8_t	cap2;
#define ATAPI_TAPE_CAP_PAGE_EF	0x08;		/* supports ERASE formatting */
#define ATAPI_TAPE_CAP_PAGE_QFA	0x20		/* supports QFA formats */
    uint8_t	cap3;
#define ATAPI_TAPE_CAP_PAGE_LCK	0x01		/* supports locking media */
#define ATAPI_TAPE_CAP_PAGE_LCKED 0x02		/* the media is locked */
#define ATAPI_TAPE_CAP_PAGE_PRV	0x04		/* defaults to prevent state */
#define ATAPI_TAPE_CAP_PAGE_EJ	0x08		/* supports eject */
#define ATAPI_TAPE_CAP_PAGE_DIS	0x10		/* can break request > ctl */
#define ATAPI_TAPE_CAP_PAGE_ECC	0x40		/* supports error correction */
#define ATAPI_TAPE_CAP_PAGE_CMP	0x80		/* supports data compression */
    uint8_t	cap4;
#define ATAPI_TAPE_CAP_PAGE_BLK512 0x02		/* supports 512b block size */
#define ATAPI_TAPE_CAP_PAGE_BLK1K 0x04		/* supports 1024b block size */
#define ATAPI_TAPE_CAP_PAGE_BLK32K 0x80		/* supports 32kb block size */
    uint16_t	max_speed;			/* supported speed in KBps */
    uint16_t	max_defects;			/* max stored defect entries */
    uint16_t	ctl;				/* continuous transfer limit */
    uint16_t	speed;				/* current Speed, in KBps */
    uint16_t	buffer_size;			/* buffer Size, in 512 bytes */
    uint8_t	reserved18;
    uint8_t	reserved19;
};

/* ATAPI OnStream ADR data transfer mode page (ADR unique) */
struct ast_transferpage {
    /* mode page data header */
    uint8_t	data_length;			/* total length of data */
    uint8_t	medium_type;			/* medium type (if any) */
    uint8_t	dsp;				/* device specific parameter */
    uint8_t	blk_desc_len;			/* block Descriptor Length */

    /* data transfer page */
    uint8_t	page_code	:6;
#define ATAPI_TAPE_TRANSFER_PAGE     0x30

    uint8_t	reserved0_6	:1;
    uint8_t	ps		:1;		/* parameters saveable */
    uint8_t	page_length;			/* page Length == 0x02 */
    uint8_t	reserved2;
    uint8_t	read32k		:1;		/* 32k blk size (data only) */
    uint8_t	read32k5	:1;		/* 32.5k blk size (data&AUX) */
    uint8_t	reserved3_23	:2;
    uint8_t	write32k	:1;		/* 32k blk size (data only) */
    uint8_t	write32k5	:1;		/* 32.5k blk size (data&AUX) */
    uint8_t	reserved3_6	:1;
    uint8_t	streaming	:1;		/* streaming mode enable */
};

/* ATAPI OnStream ADR vendor identification mode page (ADR unique) */
struct ast_identifypage {
    /* mode page data header */
    struct scsipi_mode_header header;
    /* data transfer page */
    uint8_t	page_code;
#define ATAPI_TAPE_IDENTIFY_PAGE     0x36
    uint8_t	page_length;			/* page Length == 0x06 */
    uint8_t	ident[4];			/* host id string */
    uint8_t	reserved6;
    uint8_t	reserved7;
};

/*	$NetBSD: sdvar.h,v 1.25 2005/01/31 23:06:41 reinoud Exp $	*/

/*-
 * Copyright (c) 1998, 2004 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Charles M. Hannum.
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
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
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

/*
 * Originally written by Julian Elischer (julian@dialix.oz.au)
 * for TRW Financial Systems for use under the MACH(2.5) operating system.
 *
 * TRW Financial Systems, in accordance with their agreement with Carnegie
 * Mellon University, makes this software available to CMU to distribute
 * or use in any manner that they see fit as long as this message is kept with
 * the software. For this reason TFS also grants any other persons or
 * organisations permission to use or modify this software.
 *
 * TFS supplies this software to be publicly redistributed
 * on the understanding that TFS is not responsible for the correct
 * functioning of this software in any circumstances.
 *
 * Ported to run under 386BSD by Julian Elischer (julian@dialix.oz.au) Sept 1992
 */

#ifndef _DEV_SCSIPI_SDVAR_H_
#define _DEV_SCSIPI_SDVAR_H_

#include "opt_scsi.h"
#include "rnd.h"
#if NRND > 0
#include <sys/rnd.h>
#endif

#ifndef	SDRETRIES
#define	SDRETRIES	4
#endif

#ifndef	SD_IO_TIMEOUT
#define	SD_IO_TIMEOUT	(60 * 1000)
#endif

struct sd_softc {
	struct device sc_dev;
	struct disk sc_dk;

	int flags;
#define	SDF_WLABEL	0x04		/* label is writable */
#define	SDF_LABELLING	0x08		/* writing label */
#define	SDF_ANCIENT	0x10		/* disk is ancient; for minphys */
#define	SDF_DIRTY	0x20		/* disk is dirty; needs cache flush */
#define	SDF_FLUSHING	0x40		/* flushing, for sddone() */

	struct scsipi_periph *sc_periph;/* contains our targ, lun, etc. */

	struct disk_parms {
		uint32_t	heads;		/* number of heads */
		uint32_t	cyls;		/* number of cylinders */
		uint32_t	sectors;	/* number of sectors/track */
		uint32_t	blksize;	/* number of bytes/sector */
		uint32_t	rot_rate;	/* rotational rate, in RPM */
		uint64_t disksize;	/* total number sectors */
		uint64_t disksize512;	/* total number sectors */
	} params;

	struct bufq_state buf_queue;
	struct callout sc_callout;
	uint8_t type;
	char name[16]; /* product name, for default disklabel */

	void *sc_sdhook;		/* our shutdown hook */

#if NRND > 0
	rndsource_element_t rnd_source;
#endif
};

#define	SDGP_RESULT_OK		0	/* parameters obtained */
#define	SDGP_RESULT_OFFLINE	1	/* no media, or otherwise losing */
#define	SDGP_RESULT_UNFORMATTED	2	/* unformatted media (max params) */

#endif /* _DEV_SCSIPI_SDVAR_H_ */

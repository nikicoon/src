/*	$NetBSD: mouse.c,v 1.2 1999/11/11 18:29:40 ad Exp $ */

/*-
 * Copyright (c) 1998 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Juergen Hannken-Illjes.
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

#include <sys/ioctl.h>
#include <sys/time.h>
#include <dev/wscons/wsconsio.h>
#include <err.h>
#include "wsconsctl.h"

static int mstype;
static int resolution;
static int samplerate;

struct field mouse_field_tab[] = {
    { "resolution",		&resolution,	FMT_UINT,	FLG_WRONLY },
    { "samplerate",		&samplerate,	FMT_UINT,	FLG_WRONLY },
    { "type",			&mstype,	FMT_MSTYPE,	FLG_RDONLY },
};

int mouse_field_tab_len = sizeof(mouse_field_tab)/
			   sizeof(mouse_field_tab[0]);

void
mouse_get_values(fd)
	int fd;
{
	if (field_by_value(&mstype)->flags & FLG_GET)
		if (ioctl(fd, WSMOUSEIO_GTYPE, &mstype) < 0)
			err(1, "WSMOUSEIO_GTYPE");
}

void
mouse_put_values(fd)
	int fd;
{

	if (field_by_value(&resolution)->flags & FLG_SET)
		if (ioctl(fd, WSMOUSEIO_SRES, &resolution) < 0)
			err(1, "WSMOUSEIO_SRES");
	if (field_by_value(&samplerate)->flags & FLG_SET)
		if (ioctl(fd, WSMOUSEIO_SRATE, &samplerate) < 0)
			err(1, "WSMOUSEIO_SRES");
}

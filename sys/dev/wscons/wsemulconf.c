/* $NetBSD: wsemulconf.c,v 1.1 1998/03/22 14:24:03 drochner Exp $ */

/*
 * Copyright (c) 1996, 1997 Christopher G. Demetriou.  All rights reserved.
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
 *      This product includes software developed by Christopher G. Demetriou
 *	for the NetBSD Project.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
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
 */

static const char _copyright[] __attribute__ ((unused)) =
    "Copyright (c) 1996, 1997 Christopher G. Demetriou.  All rights reserved.";
static const char _rcsid[] __attribute__ ((unused)) =
    "$NetBSD: wsemulconf.c,v 1.1 1998/03/22 14:24:03 drochner Exp $";

#include <sys/param.h>
#include <sys/systm.h>

#include <dev/wscons/wsdisplayvar.h>
#include <dev/wscons/wsemulvar.h>
#include <dev/wscons/wscons_callbacks.h>

static const struct wsemul_ops *wsemul_conf[] = {
#ifdef WSEMUL_SUN
	&wsemul_sun_ops,
#endif
#ifndef WSEMUL_NO_DUMB
	&wsemul_dumb_ops,
#endif
	NULL
};

const struct wsemul_ops *
wsemul_pick(name)
	const char *name;
{
	const struct wsemul_ops *ops;

	if (name == NULL) {
		/* default */
#ifdef WSEMUL_DEFAULT
		name = WSEMUL_DEFAULT;
#else
		return (wsemul_conf[0]);
#endif
	}

	for (ops = wsemul_conf[0]; ops != NULL; ops++)
		if (strcmp(name, ops->name) == 0)
			break;

	return (ops);
}

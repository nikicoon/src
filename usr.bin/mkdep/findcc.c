/* $NetBSD: findcc.c,v 1.1 2002/06/14 23:14:18 simonb Exp $ */

/*-
 * Copyright (c) 1999 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Matthias Scheler.
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

#include <sys/cdefs.h>
#if defined(__COPYRIGHT) && !defined(lint)
__COPYRIGHT("@(#) Copyright (c) 1999 The NetBSD Foundation, Inc.\n\
	All rights reserved.\n");
#endif /* not lint */

#if defined(__RCSID) && !defined(lint)
__RCSID("$NetBSD: findcc.c,v 1.1 2002/06/14 23:14:18 simonb Exp $");
#endif /* not lint */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "findcc.h"

char *
findcc(progname)
	const char	*progname;
{
	char   *path, *dir, *next;
	char   buffer[MAXPATHLEN];

	if ((next = strchr(progname, ' ')) != NULL) {
		*next = '\0';
	}

	if (strchr(progname, '/') != NULL)
		return access(progname, X_OK) ? NULL : strdup(progname);

	if (((path = getenv("PATH")) == NULL) ||
	    ((path = strdup(path)) == NULL))
		return NULL;

	dir = path;
	while (dir != NULL) {
		if ((next = strchr(dir, ':')) != NULL)
			*next++ = '\0';

		if (snprintf(buffer, sizeof(buffer),
		    "%s/%s", dir, progname) < sizeof(buffer)) {
			if (!access(buffer, X_OK)) {
				free(path);
				return strdup(buffer);
			}
		}
		dir = next;
	}

	free(path);
	return NULL;
}


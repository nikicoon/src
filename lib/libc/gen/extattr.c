/*	$NetBSD: extattr.c,v 1.1 2005/01/02 16:43:26 thorpej Exp $	*/

/*-
 * Copyright (c) 2001 Robert N. M. Watson
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
 */

/*
 * TrustedBSD: Utility functions for extended attributes.
 */

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
__RCSID("$NetBSD: extattr.c,v 1.1 2005/01/02 16:43:26 thorpej Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>
#include <sys/extattr.h>

#include <errno.h>
#include <string.h>

int
extattr_namespace_to_string(int attrnamespace, char **string)
{

	switch(attrnamespace) {
	case EXTATTR_NAMESPACE_USER:
		if (string != NULL) {
			if ((*string =
			     strdup(EXTATTR_NAMESPACE_USER_STRING)) == NULL)
				return (-1);
		}
		return (0);

	case EXTATTR_NAMESPACE_SYSTEM:
		if (string != NULL)
			if ((*string =
			     strdup(EXTATTR_NAMESPACE_SYSTEM_STRING)) == NULL)
				return (-1);
		return (0);

	default:
		errno = EINVAL;
		return (-1);
	}
}

int
extattr_string_to_namespace(const char *string, int *attrnamespace)
{

	if (strcmp(string, EXTATTR_NAMESPACE_USER_STRING) == 0) {
		if (attrnamespace != NULL)
			*attrnamespace = EXTATTR_NAMESPACE_USER;
		return (0);
	} else if (strcmp(string, EXTATTR_NAMESPACE_SYSTEM_STRING) == 0) {
		if (attrnamespace != NULL)
			*attrnamespace = EXTATTR_NAMESPACE_SYSTEM;
		return (0);
	} else {
		errno = EINVAL;
		return (-1);
	}
}

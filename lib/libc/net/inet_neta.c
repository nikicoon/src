/*	$NetBSD: inet_neta.c,v 1.11 2000/01/22 22:19:15 mycroft Exp $	*/

/*
 * Copyright (c) 1996 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
#if 0
static const char rcsid[] = "Id: inet_neta.c,v 8.2 1996/08/08 06:54:44 vixie Exp ";
#else
__RCSID("$NetBSD: inet_neta.c,v 1.11 2000/01/22 22:19:15 mycroft Exp $");
#endif
#endif

#include "namespace.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#ifdef __weak_alias
__weak_alias(inet_neta,_inet_neta)
#endif

#ifdef SPRINTF_CHAR
# define SPRINTF(x) strlen(sprintf/**/x)
#else
# define SPRINTF(x) ((size_t)sprintf x)
#endif

/*
 * char *
 * inet_neta(src, dst, size)
 *	format a u_long network number into presentation format.
 * return:
 *	pointer to dst, or NULL if an error occurred (check errno).
 * note:
 *	format of ``src'' is as for inet_network().
 * author:
 *	Paul Vixie (ISC), July 1996
 */
char *
inet_neta(src, dst, size)
	u_long src;
	char *dst;
	size_t size;
{
	char *odst = dst;
	char *tp;

	_DIAGASSERT(dst != NULL);

	while (src & 0xffffffff) {
		u_char b = (u_char)((src & 0xff000000) >> 24);

		src <<= 8;
		if (b) {
			if (size < sizeof "255.")
				goto emsgsize;
			tp = dst;
			dst += SPRINTF((dst, "%u", b));
			if (src != 0L) {
				*dst++ = '.';
				*dst = '\0';
			}
			size -= (size_t)(dst - tp);
		}
	}
	if (dst == odst) {
		if (size < sizeof "0.0.0.0")
			goto emsgsize;
		strcpy(dst, "0.0.0.0");
	}
	return (odst);

 emsgsize:
	errno = EMSGSIZE;
	return (NULL);
}

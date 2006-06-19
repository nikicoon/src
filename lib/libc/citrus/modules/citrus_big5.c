/*	$NetBSD: citrus_big5.c,v 1.10 2006/06/19 17:28:24 tnozaki Exp $	*/

/*-
 * Copyright (c)2002, 2006 Citrus Project,
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

/*-
 * Copyright (c) 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Paul Borman at Krystal Technologies.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
__RCSID("$NetBSD: citrus_big5.c,v 1.10 2006/06/19 17:28:24 tnozaki Exp $");
#endif /* LIBC_SCCS and not lint */

#include <sys/queue.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <locale.h>
#include <wchar.h>
#include <limits.h>

#include "citrus_namespace.h"
#include "citrus_types.h"
#include "citrus_bcs.h"
#include "citrus_module.h"
#include "citrus_ctype.h"
#include "citrus_stdenc.h"
#include "citrus_big5.h"

/* ----------------------------------------------------------------------
 * private stuffs used by templates
 */

typedef struct {
	char ch[2];
	int chlen;
} _BIG5State;

typedef struct _BIG5Exclude {
	TAILQ_ENTRY(_BIG5Exclude) entry;
	wint_t start, end;
} _BIG5Exclude;

typedef TAILQ_HEAD(_BIG5ExcludeList, _BIG5Exclude) _BIG5ExcludeList;

typedef struct {
	int cell[0x100];
	_BIG5ExcludeList excludes;
} _BIG5EncodingInfo;

typedef struct {
	_BIG5EncodingInfo	ei;
	struct {
		/* for future multi-locale facility */
		_BIG5State	s_mblen;
		_BIG5State	s_mbrlen;
		_BIG5State	s_mbrtowc;
		_BIG5State	s_mbtowc;
		_BIG5State	s_mbsrtowcs;
		_BIG5State	s_wcrtomb;
		_BIG5State	s_wcsrtombs;
		_BIG5State	s_wctomb;
	} states;
} _BIG5CTypeInfo;

#define _CEI_TO_EI(_cei_)		(&(_cei_)->ei)
#define _CEI_TO_STATE(_cei_, _func_)	(_cei_)->states.s_##_func_

#define _FUNCNAME(m)			_citrus_BIG5_##m
#define _ENCODING_INFO			_BIG5EncodingInfo
#define _CTYPE_INFO			_BIG5CTypeInfo
#define _ENCODING_STATE			_BIG5State
#define _ENCODING_MB_CUR_MAX(_ei_)	2
#define _ENCODING_IS_STATE_DEPENDENT	0
#define _STATE_NEEDS_EXPLICIT_INIT(_ps_)	0


static __inline void
/*ARGSUSED*/
_citrus_BIG5_init_state(_BIG5EncodingInfo * __restrict ei,
			_BIG5State * __restrict s)
{
	memset(s, 0, sizeof(*s));
}

static __inline void
/*ARGSUSED*/
_citrus_BIG5_pack_state(_BIG5EncodingInfo * __restrict ei,
			void * __restrict pspriv,
			const _BIG5State * __restrict s)
{
	memcpy(pspriv, (const void *)s, sizeof(*s));
}

static __inline void
/*ARGSUSED*/
_citrus_BIG5_unpack_state(_BIG5EncodingInfo * __restrict ei,
			  _BIG5State * __restrict s,
			  const void * __restrict pspriv)
{
	memcpy((void *)s, pspriv, sizeof(*s));
}

static __inline int
_citrus_BIG5_check(_BIG5EncodingInfo *ei, u_int c)
{
	_DIAGASSERT(ei != NULL);

	return (ei->cell[c & 0xFF] & 0x1) ? 2 : 1;
}

static __inline int
_citrus_BIG5_check2(_BIG5EncodingInfo *ei, u_int c)
{
	_DIAGASSERT(ei != NULL);

	return (ei->cell[c & 0xFF] & 0x2) ? 1 : 0;
}

static __inline int
_citrus_BIG5_check_excludes(_BIG5EncodingInfo *ei, wint_t c)
{
	_BIG5Exclude *exclude;

	_DIAGASSERT(ei != NULL);

	TAILQ_FOREACH(exclude, &ei->excludes, entry) {
		if (c >= exclude->start && c <= exclude->end)
			return EILSEQ;
	}
	return 0;
}

#define _STRTOL_LEN(_func_, _type_, _limit_)		\
static int						\
_prop_##_func_##_len(const char *s, size_t *n,		\
	char **endptr, int base, _type_ *result)	\
{							\
	const char *ptr;				\
	char buf[_limit_ + 1], *tail;			\
	size_t siz;					\
	int save_errno, err;				\
							\
	_DIAGASSERT(s != NULL);				\
	_DIAGASSERT(n != NULL);				\
	/* endptr may be null */			\
	_DIAGASSERT(result != NULL);			\
							\
	ptr = _bcs_skip_ws_len(s, n);			\
	if (*n < 1) {					\
		*result = 0;				\
		if (endptr != NULL)			\
			*endptr = __UNCONST(s);		\
		return 0;				\
	}						\
	siz = _limit_ < *n ? _limit_ : *n;		\
	memcpy(buf, ptr, siz);				\
	buf[siz] = '\0';				\
							\
	save_errno = errno;				\
	errno = 0;					\
	*result = _func_(buf, &tail, base);		\
	err = errno;					\
	errno = save_errno;				\
	if (err == 0) {					\
		siz = (size_t)(tail - buf);		\
		if (endptr != NULL)			\
			*endptr = __UNCONST(ptr + siz);	\
		*n -= siz;				\
	}						\
	return (err);					\
}
_STRTOL_LEN(strtoul, unsigned long, 13)

#define _PROP_READ_UINT(_func_, _type_, _max_)			\
static int							\
_prop_read_##_func_(const char **s, size_t *n, _type_ *result)	\
{								\
	const char *s0;						\
	char *t;						\
	size_t n0;						\
	unsigned long l;					\
								\
	_DIAGASSERT(s != NULL);					\
	_DIAGASSERT(n != NULL);					\
	_DIAGASSERT(result != NULL);				\
								\
	s0 = *s;						\
	n0 = *n;						\
	if (_prop_strtoul_len(s0, &n0, &t, 0, &l) != 0 ||	\
	    s0 == t)						\
		return EINVAL;					\
	if (l > _max_)						\
		return ERANGE;					\
	*result = (_type_)l;					\
	*s = (const char *)t;					\
	*n = n0;						\
								\
	return 0;						\
}
_PROP_READ_UINT( u8,  uint8_t,  UINT8_MAX)
_PROP_READ_UINT(u16, uint16_t, UINT16_MAX)

#define _PROP_HANDLE_TYPE_T(_func_, _type_)			\
typedef int (*_prop_handle_##_func_##_t)			\
	(void *, const char *, _type_, _type_);
_PROP_HANDLE_TYPE_T( u8,  uint8_t)
_PROP_HANDLE_TYPE_T(u16, uint16_t)

typedef struct _prop_key_t _prop_key_t;

typedef union {
#define _PROP_HANDLE_TYPE_OPS(_func_) \
	_prop_handle_##_func_##_t _func_

	_PROP_HANDLE_TYPE_OPS( u8);
	_PROP_HANDLE_TYPE_OPS(u16);
} _prop_handler_t;

typedef enum {
	_PROP_U8, _PROP_U16,
} _prop_type_t;

struct _prop_key_t {
	const char *name;
	_prop_type_t type;
	_prop_handler_t handler;
};

static int
_prop_parse_variable(void *ctx, const char **s, size_t *n,
	const _prop_key_t *keys)
{
	const char *s0, *s1;
	size_t n0, n1, nlen;
	const _prop_key_t *key;
	const _prop_handler_t *ptr;
	int ch0, ret;

	s0 = *s;
	n0 = *n;

#define PARSE(_func_, _type_)					\
do {								\
	_type_ x, y;						\
	if (ptr->_func_ == NULL)				\
		return EINVAL;					\
	for (ch0 = 0; ch0 != ';';) {				\
		ret = _prop_read_##_func_(&s0, &n0, &x);	\
		if (ret != 0)					\
			return ret;				\
		s0 = _bcs_skip_ws_len(s0, &n0);			\
		if (n0 < 1 || (ch0 = (int)*s0) == '\0')		\
			return EINVAL;				\
		if(ch0 == '-') {				\
			++s0, --n0;				\
			ret = _prop_read_##_func_(&s0, &n0, &y);\
			if (ret != 0)				\
				return ret;			\
			if (x >= y)				\
				return EINVAL;			\
		} else						\
			y = x;					\
		s0 = _bcs_skip_ws_len(s0, &n0);			\
		if (n0 < 1)					\
			return EINVAL;				\
		ch0 = (int)*s0;					\
		if (ch0 != ',' && ch0 != ';')			\
			return EINVAL;				\
		++s0, --n0;					\
		_DIAGASSERT(ctx != NULL);			\
		ret = (*ptr->_func_)(ctx, key->name, x, y);	\
		if (ret != 0)					\
			return ret;				\
	}							\
} while (/*CONSTCOND*/0)

	for (;;) {
		s0 = _bcs_skip_ws_len(s0, &n0);
		if (n0 < 1 || *s0 == '\0')
			break;
		for (key = keys; key->name != NULL; ++key) {
			s1 = s0, n1 = n0;
			nlen = strlen(key->name);
			if (n1 <= nlen || strncmp(s1, key->name, nlen) != 0)
				continue;
			s1 += nlen, n1 -= nlen;
			s1 = _bcs_skip_ws_len(s1, &n1);
			if (n1 < 1)
				continue;
			ptr = (const _prop_handler_t *)&key->handler;
			if (*s1 == '=') {
				s0 = ++s1, n0 = --n1;
				switch (key->type) {
				case _PROP_U8:
					PARSE( u8,  uint8_t);
					break;
				case _PROP_U16:
					PARSE(u16, uint16_t);
						break;
				default:
					goto invalid;
				}
				break;
			}
		}
		if (key->name == NULL)
			goto invalid;
	}
	*s = s0;
	*n = n0;

	return 0;

invalid:
	return EINVAL;
}

static int
_citrus_BIG5_fill_rowcol(void * __restrict ctx, const char * __restrict s,
	uint8_t start, uint8_t end)
{
	_BIG5EncodingInfo *ei;
	int i;
	uint8_t n;

	_DIAGASSERT(ctx != NULL);

	ei = (_BIG5EncodingInfo *)ctx;
	i = strcmp("row", s) ? 1 : 0;
	i = 1 << i;
	for (n = start; n <= end; ++n)
		ei->cell[n & 0xFF] |= i;
	return 0;
}

static int
/*ARGSUSED*/
_citrus_BIG5_fill_excludes(void * __restrict ctx, const char * __restrict s,
	uint16_t start, uint16_t end)
{
	_BIG5EncodingInfo *ei;
	_BIG5Exclude *exclude;

	_DIAGASSERT(ctx != NULL);

	ei = (_BIG5EncodingInfo *)ctx;
	exclude = TAILQ_LAST(&ei->excludes, _BIG5ExcludeList);
	if (exclude != NULL && (wint_t)start <= exclude->end)
		return EINVAL;
	exclude = (void *)malloc(sizeof(*exclude));
	if (exclude == NULL)
		return ENOMEM;
	exclude->start = (wint_t)start;
	exclude->end = (wint_t)end;
	TAILQ_INSERT_TAIL(&ei->excludes, exclude, entry);

	return 0;
}

static const _prop_key_t rootkeys[] = {
	{ "row",      _PROP_U8,  { u8:  &_citrus_BIG5_fill_rowcol   } },
	{ "col",      _PROP_U8,  { u8:  &_citrus_BIG5_fill_rowcol   } },
	{ "excludes", _PROP_U16, { u16: &_citrus_BIG5_fill_excludes } },
	{ NULL },
};

static void
/*ARGSUSED*/
_citrus_BIG5_encoding_module_uninit(_BIG5EncodingInfo *ei)
{
	_BIG5Exclude *exclude;

	_DIAGASSERT(ei != NULL);

	while ((exclude = TAILQ_FIRST(&ei->excludes)) != NULL) {
		TAILQ_REMOVE(&ei->excludes, exclude, entry);
		free(exclude);
	}
}

static int
/*ARGSUSED*/
_citrus_BIG5_encoding_module_init(_BIG5EncodingInfo * __restrict ei,
				  const void * __restrict var, size_t lenvar)
{
	int err;
	const char *s;

	_DIAGASSERT(ei != NULL);

	memset((void *)ei, 0, sizeof(*ei));
	TAILQ_INIT(&ei->excludes);

	if (lenvar > 0 && var != NULL) {
		s = _bcs_skip_ws_len((const char *)var, &lenvar);
		if (lenvar > 0 && *s != '\0') {
			err = _prop_parse_variable(ei, &s, &lenvar, rootkeys);
			if (err == 0)
				return 0;

			_citrus_BIG5_encoding_module_uninit(ei);
			memset((void *)ei, 0, sizeof(*ei));
			TAILQ_INIT(&ei->excludes);
		}
	}

	/* fallback Big5-1984, for backward compatibility. */
	_citrus_BIG5_fill_rowcol(ei, "row", 0xA1, 0xFE);
	_citrus_BIG5_fill_rowcol(ei, "col", 0x40, 0x7E);
	_citrus_BIG5_fill_rowcol(ei, "col", 0xA1, 0xFE);

	return 0;
}

static int
/*ARGSUSED*/
_citrus_BIG5_mbrtowc_priv(_BIG5EncodingInfo * __restrict ei,
			  wchar_t * __restrict pwc,
			  const char ** __restrict s, size_t n,
			  _BIG5State * __restrict psenc,
			  size_t * __restrict nresult)
{
	wchar_t wchar;
	int c;
	int chlenbak;
	const char *s0;

	_DIAGASSERT(nresult != 0);
	_DIAGASSERT(ei != NULL);
	_DIAGASSERT(psenc != NULL);
	_DIAGASSERT(s != NULL && *s != NULL);

	s0 = *s;

	if (s0 == NULL) {
		_citrus_BIG5_init_state(ei, psenc);
		*nresult = 0;
		return (0);
	}

	chlenbak = psenc->chlen;

	/* make sure we have the first byte in the buffer */
	switch (psenc->chlen) {
	case 0:
		if (n < 1)
			goto restart;
		psenc->ch[0] = *s0++;
		psenc->chlen = 1;
		n--;
		break;
	case 1:
		break;
	default:
		/* illegal state */
		goto ilseq;
	}

	c = _citrus_BIG5_check(ei, psenc->ch[0] & 0xff);
	if (c == 0)
		goto ilseq;
	while (psenc->chlen < c) {
		if (n < 1) {
			goto restart;
		}
		psenc->ch[psenc->chlen] = *s0++;
		psenc->chlen++;
		n--;
	}

	switch (c) {
	case 1:
		wchar = psenc->ch[0] & 0xff;
		break;
	case 2:
		if (!_citrus_BIG5_check2(ei, psenc->ch[1] & 0xff))
			goto ilseq;
		wchar = ((psenc->ch[0] & 0xff) << 8) | (psenc->ch[1] & 0xff);
		break;
	default:
		/* illegal state */
		goto ilseq;
	}

	if (_citrus_BIG5_check_excludes(ei, (wint_t)wchar) != 0)
		goto ilseq;

	*s = s0;
	psenc->chlen = 0;
	if (pwc)
		*pwc = wchar;
	if (!wchar)
		*nresult = 0;
	else
		*nresult = c - chlenbak;

	return (0);

ilseq:
	psenc->chlen = 0;
	*nresult = (size_t)-1;
	return (EILSEQ);

restart:
	*s = s0;
	*nresult = (size_t)-2;
	return (0);
}

static int
/*ARGSUSED*/
_citrus_BIG5_wcrtomb_priv(_BIG5EncodingInfo * __restrict ei,
			  char * __restrict s,
			  size_t n, wchar_t wc, _BIG5State * __restrict psenc,
			  size_t * __restrict nresult)
{
	int l, ret;

	_DIAGASSERT(ei != NULL);
	_DIAGASSERT(nresult != 0);
	_DIAGASSERT(s != NULL);

	/* check invalid sequence */
	if (wc & ~0xffff ||
	    _citrus_BIG5_check_excludes(ei, (wint_t)wc) != 0) {
		ret = EILSEQ;
		goto err;
	}

	if (wc & 0x8000) {
		if (_citrus_BIG5_check(ei, (wc >> 8) & 0xff) != 2 ||
		    !_citrus_BIG5_check2(ei, wc & 0xff)) {
			ret = EILSEQ;
			goto err;
		}
		l = 2;
	} else {
		if (wc & ~0xff || !_citrus_BIG5_check(ei, wc & 0xff)) {
			ret = EILSEQ;
			goto err;
		}
		l = 1;
	}

	if (n < l) {
		/* bound check failure */
		ret = E2BIG;
		goto err;
	}

	if (l == 2) {
		s[0] = (wc >> 8) & 0xff;
		s[1] = wc & 0xff;
	} else
		s[0] = wc & 0xff;

	*nresult = l;

	return 0;

err:
	*nresult = (size_t)-1;
	return ret;
}

static __inline int
/*ARGSUSED*/
_citrus_BIG5_stdenc_wctocs(_BIG5EncodingInfo * __restrict ei,
			   _csid_t * __restrict csid,
			   _index_t * __restrict idx, wchar_t wc)
{

	_DIAGASSERT(csid != NULL && idx != NULL);

	*csid = (wc < 0x100) ? 0 : 1;
	*idx = (_index_t)wc;

	return 0;
}

static __inline int
/*ARGSUSED*/
_citrus_BIG5_stdenc_cstowc(_BIG5EncodingInfo * __restrict ei,
			   wchar_t * __restrict wc,
			   _csid_t csid, _index_t idx)
{
	_DIAGASSERT(wc != NULL);

	switch (csid) {
	case 0:
	case 1:
		*wc = (wchar_t)idx;
		break;
	default:
		return EILSEQ;
	}

	return 0;
}

static __inline int
/*ARGSUSED*/
_citrus_BIG5_stdenc_get_state_desc_generic(_BIG5EncodingInfo * __restrict ei,
					   _BIG5State * __restrict psenc,
					   int * __restrict rstate)
{

	if (psenc->chlen == 0)
		*rstate = _STDENC_SDGEN_INITIAL;
	else
		*rstate = _STDENC_SDGEN_INCOMPLETE_CHAR;

	return 0;
}

/* ----------------------------------------------------------------------
 * public interface for ctype
 */

_CITRUS_CTYPE_DECLS(BIG5);
_CITRUS_CTYPE_DEF_OPS(BIG5);

#include "citrus_ctype_template.h"


/* ----------------------------------------------------------------------
 * public interface for stdenc
 */

_CITRUS_STDENC_DECLS(BIG5);
_CITRUS_STDENC_DEF_OPS(BIG5);

#include "citrus_stdenc_template.h"

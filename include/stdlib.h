/*-
 * Copyright (c) 1990 The Regents of the University of California.
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
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
 *
 *	from: @(#)stdlib.h	5.13 (Berkeley) 6/4/91
 *	$Id: stdlib.h,v 1.7 1993/10/13 17:56:09 jtc Exp $
 */

#ifndef _STDLIB_H_
#define _STDLIB_H_
#include <machine/ansi.h>

#ifdef	_SIZE_T_
typedef	_SIZE_T_	size_t;
#undef	_SIZE_T_
#endif

#ifdef	_WCHAR_T_
typedef	_WCHAR_T_	wchar_t;
#undef	_WCHAR_T_
#endif

typedef struct {
	int quot;		/* quotient */
	int rem;		/* remainder */
} div_t;
typedef struct {
	long quot;		/* quotient */
	long rem;		/* remainder */
} ldiv_t;

#define	EXIT_FAILURE	1
#define	EXIT_SUCCESS	0

#define	RAND_MAX	0x7fffffff

#define	MB_CUR_MAX	1	/* XXX */

#include <sys/cdefs.h>

__BEGIN_DECLS
void	 abort __P((void));
int	 abs __P((int));
int	 atexit __P((void (*)(void)));
double	 atof __P((const char *));
int	 atoi __P((const char *));
long	 atol __P((const char *));
void	*bsearch __P((const void *, const void *, size_t,
	    size_t, int (*)(const void *, const void *)));
void	*calloc __P((size_t, size_t));
div_t	 div __P((int, int));
void	 exit __P((int));
void	 free __P((void *));
char	*getenv __P((const char *));
long	 labs __P((long));
ldiv_t	 ldiv __P((long, long));
void	*malloc __P((size_t));
void	 qsort __P((void *, size_t, size_t,
	    int (*)(const void *, const void *)));
int	 rand __P((void));
void	*realloc __P((void *, size_t));
void	 srand __P((unsigned));
double	 strtod __P((const char *, char **));
long	 strtol __P((const char *, char **, int));
unsigned long
	 strtoul __P((const char *, char **, int));
int	 system __P((const char *));

/* these are currently just stubs */
int	 mblen __P((const char *, size_t));
size_t	 mbstowcs __P((wchar_t *, const char *, size_t));
int	 wctomb __P((char *, wchar_t));
int	 mbtowc __P((wchar_t *, const char *, size_t));
size_t	 wcstombs __P((char *, const wchar_t *, size_t));

#if !defined(_ANSI_SOURCE) && !defined(_POSIX_SOURCE)
#include <sys/types.h>

#if defined(alloca) && (alloca == __builtin_alloca) && (__GNUC__ < 2)
void  *alloca __P((int));     /* built-in for gcc */ 
#else 
void  *alloca __P((size_t)); 
#endif /* __GNUC__ */ 
void	 cfree __P((void *));

int	 getopt __P((int, char * const *, const char *));
extern	 char *optarg;			/* getopt(3) external variables */
extern	 int opterr;
extern	 int optind;
extern	 int optopt;
int	 getsubopt __P((char **, char * const *, char **));
extern	 char *suboptarg;		/* getsubopt(3) external variable */

int	 heapsort __P((void *, size_t, size_t,
	    int (*)(const void *, const void *)));
int	 radixsort __P((const u_char **, int, const u_char *, u_char));

char	*initstate __P((unsigned, char *, int));
long	 random __P((void));
char	*setstate __P((char *));
void	 srandom __P((unsigned));

int	 putenv __P((const char *));
int	 setenv __P((const char *, const char *, int));
void	 unsetenv __P((const char *));

double	 drand48 __P((void));
double	 erand48 __P((unsigned short[3]));
long	 jrand48 __P((unsigned short[3]));
void	 lcong48 __P((unsigned short[7]));
long	 lrand48 __P((void));
long	 mrand48 __P((void));
long	 nrand48 __P((unsigned short[3]));
unsigned short *seed48 __P((unsigned short[3]));
void	 srand48 __P((long));
#endif /* !_ANSI_SOURCE && !_POSIX_SOURCE */

__END_DECLS

#endif /* _STDLIB_H_ */

/*-
 * Copyright (c) 2009 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Alistair Crooks (agc@NetBSD.org)
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
#ifndef FASTCTYPE_H_
#define FASTCTYPE_H_

int fast_isalnum(unsigned char);
int fast_isalpha(unsigned char);
int fast_isascii(unsigned char);
int fast_iscntrl(unsigned char);
int fast_isdigit(unsigned char);
int fast_islower(unsigned char);
int fast_isupper(unsigned char);
int fast_isprint(unsigned char);
int fast_isspace(unsigned char);
int fast_isxdigit(unsigned char);
int fast_tolower(unsigned char);
int fast_toupper(unsigned char);

#define isalnum(x)	fast_isalnum(x)
#define isalpha(x)	fast_isalpha(x)
#define isascii(x)	fast_isascii(x)
#define iscntrl(x)	fast_iscntrl(x)
#define isdigit(x)	fast_isdigit(x)
#define islower(x)	fast_islower(x)
#define isupper(x)	fast_isupper(x)
#define isprint(x)	fast_isprint(x)
#define isspace(x)	fast_isspace(x)
#define tolower(x)	fast_tolower(x)
#define toupper(x)	fast_toupper(x)
#define isxdigit(x)	fast_isxdigit(x)

#endif

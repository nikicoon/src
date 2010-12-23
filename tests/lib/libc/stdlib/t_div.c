/*	$NetBSD: t_div.c,v 1.1 2010/12/23 15:27:44 pgoyette Exp $ */

/*-
 * Copyright (c) 2001 The NetBSD Foundation, Inc.
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

#include <atf-c.h>

#include <stdio.h>
#include <stdlib.h>

#define	NUM	1999236
#define	DENOM	1000000
#define	QUOT	1
#define	REM	999236

ATF_TC(div);

ATF_TC_HEAD(div, tc)
{

	atf_tc_set_md_var(tc, "descr", "Test div(3) for correctness");
}

ATF_TC_BODY(div, tc)
{
	div_t d;

	d = div(NUM, DENOM);

	ATF_CHECK(d.quot == QUOT);
	ATF_CHECK(d.rem  == REM);
}

ATF_TC(ldiv);

ATF_TC_HEAD(ldiv, tc)
{

	atf_tc_set_md_var(tc, "descr", "Test ldiv(3) for correctness");
}

ATF_TC_BODY(ldiv, tc)
{
	ldiv_t ld;

	ld = ldiv(NUM, DENOM);

	ATF_CHECK(ld.quot == QUOT);
	ATF_CHECK(ld.rem  == REM);
}

ATF_TC(lldiv);

ATF_TC_HEAD(lldiv, tc)
{

	atf_tc_set_md_var(tc, "descr", "Test lllldiv(3) for correctness");
}

ATF_TC_BODY(lldiv, tc)
{
	lldiv_t lld;

	lld = lldiv(NUM, DENOM);

	ATF_CHECK(lld.quot == QUOT);
	ATF_CHECK(lld.rem  == REM);
}

ATF_TP_ADD_TCS(tp)
{

	ATF_TP_ADD_TC(tp, div);
	ATF_TP_ADD_TC(tp, ldiv);
	ATF_TP_ADD_TC(tp, lldiv);

	return atf_no_error();
}

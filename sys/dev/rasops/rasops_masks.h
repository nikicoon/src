/* 	$NetBSD: rasops_masks.h,v 1.3 1999/10/23 23:14:14 ad Exp $	*/

/*-
 * Copyright (c) 1999 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Andy Doran.
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

#ifndef _RASOPS_MASKS_H_
#define _RASOPS_MASKS_H_ 1

#include <sys/types.h>
#include <machine/endian.h>

/*
 * Convenience macros. To get around the problem of dealing with properly
 * ordered bits on little-endian machines, we just convert everything to
 * big-endian and back again when we're done.  
 *
 * MBL: move bits left
 * MBR: move bits right
 * MBE: make big-endian
 */
#if BYTE_ORDER == BIG_ENDIAN

#define MBL(x,y)	((y) > 31 ? 0 : (x) >> (y))
#define MBR(x,y)    	((y) > 31 ? 0 : (x) << (y))
#define MBE(x)		(x)

#else

#define MBL(x,y)    	((y) > 31 ? 0 : MBE(MBE(x) << (y)))
#define MBR(x,y)    	((y) > 31 ? 0 : MBE(MBE(x) >> (y)))
#define MBE(x)		( (((x) & 0x000000FFU) << 24) \
                        | (((x) & 0x0000FF00U) <<  8) \
                        | (((x) & 0x00FF0000U) >>  8) \
                        | (((x) & 0xFF000000U) >> 24) )
#endif

/*
 * Using GETBITS() and PUTBITS() inside a loop mightn't be such a good idea.
 * There's probably some CSE and strength-reduction that the compiler won't
 * even think about - really should have a few assumptions/separate cases.
 */

/* Get a number of bits ( <= 32 ) from *sp and store in dw */
#define GETBITS(sp, x, w, dw) do { \
    dw = MBL(*(sp), (x)); \
    if (((x) + (w)) > 32) \
	dw |= (MBR((sp)[1], 32 - (x))); \
} while(0);

/* Put a number of bits ( <= 32 ) from sw to *dp */ 
#define PUTBITS(sw, x, w, dp) do { \
    int n = (x) + (w) - 32; \
    \
    if (n <= 0) { \
	n = rasops_pmask[x & 31][w & 31]; \
	*(dp) = (*(dp) & ~n) | (MBR(sw, x) & n); \
    } else { \
	*(dp) = (*(dp) & rasops_rmask[x]) | (MBR((sw), x)); \
	(dp)[1] = ((dp)[1] & rasops_rmask[n]) | \
		(MBL(sw, 32-(x)) & rasops_lmask[n]); \
    } \
} while(0);

/* rasops_masks.c */
extern int32_t	rasops_lmask[32+1];
extern int32_t	rasops_rmask[32+1];
extern int32_t	rasops_pmask[32][32];

#endif /* _RASOPS_MASKS_H_ */

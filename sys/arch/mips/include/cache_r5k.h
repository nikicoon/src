/*	$NetBSD: cache_r5k.h,v 1.1 2003/03/08 04:43:26 rafal Exp $	*/

/*
 * Copyright 2001 Wasabi Systems, Inc.
 * All rights reserved.
 *
 * Written by Jason R. Thorpe for Wasabi Systems, Inc.
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
 *	This product includes software developed for the NetBSD Project by
 *	Wasabi Systems, Inc.
 * 4. The name of Wasabi Systems, Inc. may not be used to endorse
 *    or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY WASABI SYSTEMS, INC. ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL WASABI SYSTEMS, INC
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#if defined(_KERNEL) && !defined(_LOCORE)

void	r5k_icache_sync_all_32(void);
void	r5k_icache_sync_range_32(vaddr_t, vsize_t);
void	r5k_icache_sync_range_index_32(vaddr_t, vsize_t);

void	r5k_pdcache_wbinv_all_16(void);
void	r5k_pdcache_wbinv_all_32(void);
void	r4600v1_pdcache_wbinv_range_32(vaddr_t, vsize_t);
void	r4600v2_pdcache_wbinv_range_32(vaddr_t, vsize_t);
void	vr4131v1_pdcache_wbinv_range_16(vaddr_t, vsize_t);
void	r5k_pdcache_wbinv_range_16(vaddr_t, vsize_t);
void	r5k_pdcache_wbinv_range_32(vaddr_t, vsize_t);
void	r5k_pdcache_wbinv_range_index_16(vaddr_t, vsize_t);
void	r5k_pdcache_wbinv_range_index_32(vaddr_t, vsize_t);

void	r4600v1_pdcache_inv_range_32(vaddr_t, vsize_t);
void	r4600v2_pdcache_inv_range_32(vaddr_t, vsize_t);
void	r5k_pdcache_inv_range_16(vaddr_t, vsize_t);
void	r5k_pdcache_inv_range_32(vaddr_t, vsize_t);
void	r4600v1_pdcache_wb_range_32(vaddr_t, vsize_t);
void	r4600v2_pdcache_wb_range_32(vaddr_t, vsize_t);
void	r5k_pdcache_wb_range_16(vaddr_t, vsize_t);
void	r5k_pdcache_wb_range_32(vaddr_t, vsize_t);

void	r5k_enable_sdcache(void);

void	r5k_sdcache_wbinv_all(void);
void	r5k_sdcache_wbinv_range(vaddr_t, vsize_t);
void	r5k_sdcache_wbinv_rangeall(vaddr_t, vsize_t);
void	r5k_sdcache_inv_range(vaddr_t, vsize_t);
void	r5k_sdcache_wb_range(vaddr_t, vsize_t);

#endif /* _KERNEL && !_LOCORE */

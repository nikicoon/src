/*	$NetBSD: ipsec_private.h,v 1.2 2008/04/23 07:29:47 thorpej Exp $	*/

/*-
 * Copyright (c) 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe.
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

#ifndef _NETIPSEC_IPSEC_PRIVATE_H_
#define _NETIPSEC_IPSEC_PRIVATE_H_

#ifdef _KERNEL
#include <net/net_stats.h>

extern	percpu_t *ipsecstat_percpu;
extern	percpu_t *ahstat_percpu;
extern	percpu_t *espstat_percpu;
extern	percpu_t *ipcompstat_percpu;
extern	percpu_t *ipipstat_percpu;
extern	percpu_t *pfkeystat_percpu;

#define	IPSEC_STAT_GETREF()	_NET_STAT_GETREF(ipsecstat_percpu)
#define	IPSEC_STAT_PUTREF()	_NET_STAT_PUTREF(ipsecstat_percpu)
#define	IPSEC_STATINC(x)	_NET_STATINC(ipsecstat_percpu, x)
#define	IPSEC_STATADD(x, v)	_NET_STATADD(ipsecstat_percpu, x, v)

#define	IPSEC6_STAT_GETREF()	IPSEC_STAT_GETREF()
#define	IPSEC6_STAT_PUTREF()	IPSEC_STAT_PUTREF()
#define	IPSEC6_STATINC(x)	IPSEC_STATINC(x)

#define	AH_STATINC(x)		_NET_STATINC(ahstat_percpu, x)
#define	AH_STATADD(x, v)	_NET_STATADD(ahstat_percpu, x, v)

#define	ESP_STATINC(x)		_NET_STATINC(espstat_percpu, x)
#define	ESP_STATADD(x, v)	_NET_STATADD(espstat_percpu, x, v)

#define	IPCOMP_STATINC(x)	_NET_STATINC(ipcompstat_percpu, x)
#define	IPCOMP_STATADD(x, v)	_NET_STATADD(ipcompstat_percpu, x, v)

#define	IPIP_STATINC(x)		_NET_STATINC(ipipstat_percpu, x)
#define	IPIP_STATADD(x, v)	_NET_STATADD(ipipstat_percpu, x, v)

#define	PFKEY_STAT_GETREF()	_NET_STAT_GETREF(pfkeystat_percpu)
#define	PFKEY_STAT_PUTREF()	_NET_STAT_PUTREF(pfkeystat_percpu)
#define	PFKEY_STATINC(x)	_NET_STATINC(pfkeystat_percpu, x)
#define	PFKEY_STATADD(x, v)	_NET_STATADD(pfkeystat_percpu, x, v)
#endif /* _KERNEL */

#endif /* !_NETIPSEC_IPSEC_PRIVATE_H_ */

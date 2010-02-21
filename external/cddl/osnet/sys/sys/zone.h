/*-
 * Copyright (c) 2007 Pawel Jakub Dawidek <pjd@FreeBSD.org>
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/sys/cddl/compat/opensolaris/sys/zone.h,v 1.3.2.1 2009/08/03 08:13:06 kensmith Exp $
 */

#ifndef _OPENSOLARIS_SYS_ZONE_H_
#define	_OPENSOLARIS_SYS_ZONE_H_

#ifdef _KERNEL

#include <sys/jail.h>

/*
 * Macros to help with zone visibility restrictions.
 */

/*
 * Is thread in the global zone?
 */
#define	INGLOBALZONE(thread)	(!jailed((thread)->td_ucred))

/*
 * Attach the given dataset to the given jail.
 */
extern int zone_dataset_attach(struct ucred *, const char *, int);

/*
 * Detach the given dataset to the given jail.
 */
extern int zone_dataset_detach(struct ucred *, const char *, int);

/*
 * Returns true if the named pool/dataset is visible in the current zone.
 */
extern int zone_dataset_visible(const char *, int *);

#else	/* !_KERNEL */

#define	GLOBAL_ZONEID	0

#endif	/* _KERNEL */

#endif	/* !_OPENSOLARIS_SYS_ZONE_H_ */

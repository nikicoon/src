/*	$NetBSD: amu.h,v 1.1.1.7 2004/11/27 01:01:05 christos Exp $	*/

/*
 * Copyright (c) 1997-2004 Erez Zadok
 * Copyright (c) 1990 Jan-Simon Pendry
 * Copyright (c) 1990 Imperial College of Science, Technology & Medicine
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Jan-Simon Pendry at Imperial College, London.
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
 *    must display the following acknowledgment:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
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
 *
 * Id: amu.h,v 1.9 2004/01/06 03:56:20 ezk Exp
 *
 */

#ifndef _AMU_H
#define _AMU_H

/*
 * Decide what maximum level of NFS server to try and mount with.
 */
#ifdef HAVE_FS_NFS3
# define NFS_VERS_MAX NFS_VERSION3
#else /* not HAVE_FS_NFS3 */
# define NFS_VERS_MAX NFS_VERSION
#endif /* not HAVE_FS_NFS3 */

/* some systems like ncr2 do not define this in <rpcsvc/mount.h> */
#ifndef MNTPATHLEN
# define MNTPATHLEN 1024
#endif /* not MNTPATHLEN */
#ifndef MNTNAMLEN
# define MNTNAMLEN 255
#endif /* not MNTNAMLEN */

/*
 * external definitions for building libamu.a
 */
extern voidp amqproc_null_1(voidp argp, CLIENT *rqstp);
extern amq_mount_tree_p *amqproc_mnttree_1(amq_string *argp, CLIENT *rqstp);
extern voidp amqproc_umnt_1(amq_string *argp, CLIENT *rqstp);
extern amq_mount_stats *amqproc_stats_1(voidp argp, CLIENT *rqstp);
extern amq_mount_tree_list *amqproc_export_1(voidp argp, CLIENT *rqstp);
extern int *amqproc_setopt_1(amq_setopt *argp, CLIENT *rqstp);
extern amq_mount_info_list *amqproc_getmntfs_1(voidp argp, CLIENT *rqstp);
extern int *amqproc_mount_1(voidp argp, CLIENT *rqstp);
extern amq_string *amqproc_getvers_1(voidp argp, CLIENT *rqstp);

extern long get_server_pid(void);

#endif /* not _AMU_H */

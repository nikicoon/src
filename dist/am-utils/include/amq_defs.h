/*	$NetBSD: amq_defs.h,v 1.2 2000/06/16 02:10:13 dogcow Exp $ */
/*
 * Copyright (c) 1997-2000 Erez Zadok
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
 *      %W% (Berkeley) %G%
 *
 * Id: amq_defs.h,v 1.4 2000/01/12 16:45:05 ezk Exp 
 *
 */

#ifndef _AMQ_DEFS_H
#define _AMQ_DEFS_H

/*
 * MACROS
 */
#ifndef AMQ_SIZE
# define AMQ_SIZE 16384
#endif /* not AMQ_SIZE */
#define AMQ_STRLEN 10240
#define AMQ_PROGRAM ((u_long)300019)
#define AMQ_VERSION ((u_long)1)
#define AMQPROC_NULL ((u_long)0)
#define AMQPROC_MNTTREE ((u_long)1)
#define AMQPROC_UMNT ((u_long)2)
#define AMQPROC_STATS ((u_long)3)
#define AMQPROC_EXPORT ((u_long)4)
#define AMQPROC_SETOPT ((u_long)5)
#define AMQPROC_GETMNTFS ((u_long)6)
#define AMQPROC_MOUNT ((u_long)7)
#define AMQPROC_GETVERS ((u_long)8)
#define AMQPROC_GETPID ((u_long)9)

/*
 * TYPEDEFS
 */
typedef long *time_type;
typedef struct amq_mount_info amq_mount_info;
typedef struct amq_mount_stats amq_mount_stats;
typedef struct amq_mount_tree amq_mount_tree;
typedef struct amq_setopt amq_setopt;
typedef amq_mount_tree *amq_mount_tree_p;

/*
 * STRUCTURES:
 */
struct amq_mount_tree {
  amq_string mt_mountinfo;
  amq_string mt_directory;
  amq_string mt_mountpoint;
  amq_string mt_type;
  time_type mt_mounttime;
  u_short mt_mountuid;
  int mt_getattr;
  int mt_lookup;
  int mt_readdir;
  int mt_readlink;
  int mt_statfs;
  struct amq_mount_tree *mt_next;
  struct amq_mount_tree *mt_child;
};

struct amq_mount_info {
  amq_string mi_type;
  amq_string mi_mountpt;
  amq_string mi_mountinfo;
  amq_string mi_fserver;
  int mi_error;
  int mi_refc;
  int mi_up;
};

typedef struct {
  u_int amq_mount_info_list_len;
  amq_mount_info *amq_mount_info_list_val;
} amq_mount_info_list;

typedef struct {
  u_int amq_mount_tree_list_len;
  amq_mount_tree_p *amq_mount_tree_list_val;
} amq_mount_tree_list;

struct amq_mount_stats {
  int as_drops;
  int as_stale;
  int as_mok;
  int as_merr;
  int as_uerr;
};

enum amq_opt {
  AMOPT_DEBUG = 0,
  AMOPT_LOGFILE = 1,
  AMOPT_XLOG = 2,
  AMOPT_FLUSHMAPC = 3
};
typedef enum amq_opt amq_opt;	/* enum typedefs should be after enum */

struct amq_setopt {
  amq_opt as_opt;
  amq_string as_str;
};

/*
 * EXTERNALS:
 *
 * external definitions for amqproc_*_1() have been moved off to private
 * headers in lib/amu.h, amd/amd.h, etc.  They have to be private since the
 * same named functions appear in different places with different prototypes
 * an functionality.
 */
extern bool_t xdr_amq_mount_info(XDR *xdrs, amq_mount_info *objp);
extern bool_t xdr_amq_mount_info_list(XDR *xdrs, amq_mount_info_list *objp);
extern bool_t xdr_amq_mount_stats(XDR *xdrs, amq_mount_stats *objp);
extern bool_t xdr_amq_mount_tree(XDR *xdrs, amq_mount_tree *objp);
extern bool_t xdr_amq_mount_tree_list(XDR *xdrs, amq_mount_tree_list *objp);
extern bool_t xdr_amq_mount_tree_p(XDR *xdrs, amq_mount_tree_p *objp);
extern bool_t xdr_amq_opt(XDR *xdrs, amq_opt *objp);
extern bool_t xdr_amq_setopt(XDR *xdrs, amq_setopt *objp);
extern bool_t xdr_pri_free(XDRPROC_T_TYPE xdr_args, caddr_t args_ptr);
extern bool_t xdr_time_type(XDR *xdrs, time_type *objp);

#endif /* not _AMQ_DEFS_H */

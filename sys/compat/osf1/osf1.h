/* $NetBSD: osf1.h,v 1.6 1999/04/26 06:10:36 cgd Exp $ */

/*
 * Copyright (c) 1999 Christopher G. Demetriou.  All rights reserved.
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
 *      This product includes software developed by Christopher G. Demetriou
 *	for the NetBSD Project.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _COMPAT_OSF1_OSF1_H_
#define _COMPAT_OSF1_OSF1_H_

/*
 * Collected OSF/1 definitions and structures, sorted by OSF/1 header.
 * Error numbers (errno.h) aren't here, since they're likely to change
 * (additions) more often.
 *
 * This file is up to date as of Digital UNIX V4.0.
 */

#include <sys/types.h>
#include <compat/osf1/osf1_errno.h>

/* type definitions used by structures */

typedef int16_t		osf1_short;
typedef int32_t		osf1_int;
typedef int64_t		osf1_long;
typedef u_int32_t	osf1_u_int;

typedef int32_t		osf1_dev_t;
typedef u_int32_t	osf1_ino_t;
typedef u_int32_t	osf1_mode_t;
typedef u_int16_t	osf1_nlink_t;
typedef u_int32_t	osf1_uid_t;
typedef u_int32_t	osf1_gid_t;
typedef u_int64_t	osf1_off_t;
typedef int32_t		osf1_time_t;
typedef u_int32_t	osf1_uint_t;
typedef u_int64_t	osf1_sigset_t;
typedef u_int64_t	osf1_size_t;
typedef u_int64_t	osf1_fsid_t;
typedef u_int64_t	osf1_rlim_t;
typedef void		*osf1_data_ptr;	/* XXX hard to fix size */
typedef void		*osf1_fcn_ptr;	/* XXX hard to fix size, bogus */


/* auxv.h */

union osf1_auxv_data {
	osf1_long	a_val;
	osf1_data_ptr	a_ptr;
	osf1_fcn_ptr	a_fcn;
};

struct osf1_auxv {
	osf1_int	a_type;
	union osf1_auxv_data a_un;
};

/* auxv entry types */
#define OSF1_AT_NULL			0
#define OSF1_AT_IGNORE			1
#define OSF1_AT_EXECFD			2
#define OSF1_AT_PHDR			3
#define OSF1_AT_PHENT			4
#define OSF1_AT_PHNUM			5
#define OSF1_AT_PAGESZ			6
#define OSF1_AT_BASE			7
#define OSF1_AT_FLAGS			8
#define OSF1_AT_ENTRY			9
#define OSF1_AT_EXEC_FILENAME		1001
#define OSF1_AT_EXEC_LOADER_FILENAME	1002
#define OSF1_AT_EXEC_LOADER_FLAGS	1003


/* exec.h/ldr_exec.h */

#define	OSF1_LDR_EXEC_DEFAULT_LOADER	"/sbin/loader"

/* exec_with_loader()/OSF1_AT_EXEC_LOADER_FLAGS flags regions */
#define	OSF1_LDR_EXEC_SYSTEM_MASK	0xffff0000
#define	OSF1_LDR_EXEC_USER_MASK		0xffff0000

/* OSF1_AT_EXEC_LOADER_FLAGS flags */
#define	OSF1_LDR_EXEC_NOPRIVS_F		0x10000000
#define	OSF1_LDR_EXEC_PTRACE_F		0x20000000
#define	OSF1_LDR_EXEC_SETGID_F		0x40000000
#define	OSF1_LDR_EXEC_SETUID_F		0x80000000


/* fcntl.h */

/* fcntl ops */
#define OSF1_F_DUPFD		0
#define OSF1_F_GETFD		1	/* uses flags, see below */
#define OSF1_F_SETFD		2	/* uses flags, see below */
#define OSF1_F_GETFL		3	/* uses flags, see below */
#define OSF1_F_SETFL		4	/* uses flags, see below */
#define OSF1_F_GETOWN		5
#define OSF1_F_SETOWN		6
#define OSF1_F_GETLK		7	/* uses osf1_flock, see below */
#define OSF1_F_SETLK		8	/* uses osf1_flock, see below */
#define OSF1_F_SETLKW		9	/* uses osf1_flock, see below */
#define OSF1_F_RGETLK		10	/* [lock mgr op] */
#define OSF1_F_RSETLK		11	/* [lock mgr op] */
#define OSF1_F_CNVT		12	/* [lock mgr op] */
#define OSF1_F_RSETLKW		13	/* [lock mgr op] */
#define OSF1_F_PURGEFS		14	/* [lock mgr op] */
#define OSF1_F_PURGENFS		15	/* [DECsafe op] */

/* fcntl GETFD/SETFD flags */
#define OSF1_FD_CLOEXEC		1

/* fcntl GETFL/SETFL flags */
#define	OSF1_FNONBLOCK		0x00000004
#define	OSF1_FAPPEND		0x00000008
#define	OSF1_FASYNC		0x00000040
#define	OSF1_FSYNC		0x00004000
#define	OSF1_FNDELAY		0x00008000
#define	OSF1_FDSYNC		0x00080000
#define	OSF1_FRSYNC		0x00100000

/* struct osf1_flock, for GETLK/SETLK/SETLKW */
/* XXX */

/* open flags */
#define OSF1_O_RDONLY		0x00000000
#define OSF1_O_WRONLY		0x00000001
#define OSF1_O_RDWR		0x00000002
#define OSF1_O_ACCMODE		0x00000003	/* mask of RD and WR bits */
#define OSF1_O_NONBLOCK		0x00000004
#define OSF1_O_APPEND		0x00000008
/* no				0x00000010 */
#define OSF1_O_DEFER		0x00000020
/* no				0x00000040 */
/* no				0x00000080 */
/* no				0x00000100 */
#define OSF1_O_CREAT		0x00000200
#define OSF1_O_TRUNC		0x00000400
#define OSF1_O_EXCL		0x00000800
#define OSF1_O_NOCTTY		0x00001000
#define OSF1_O_SYNC		0x00004000
#define OSF1_O_NDELAY		0x00008000
#define OSF1_O_DRD		0x00008000	/* == O_NDELAY, DON'T USE */
/* no				0x00010000 */
/* no				0x00020000 */
/* no				0x00040000 */
#define OSF1_O_DSYNC		0x00080000
#define OSF1_O_RSYNC		0x00100000
/* no				0x00200000+ */


/* ioctl.h */

#define OSF1_IOCPARM_MASK	0x1fff
#define OSF1_IOCPARM_LEN(x)	(((x) >> 16) & OSF1_IOCPARM_MASK)
#define OSF1_IOCGROUP(x)	(((x) >> 8) & 0xff)
#define OSF1_IOCCMD(x)          ((x) & 0xff)

#define OSF1_IOCPARM_MAX	8192
#define OSF1_IOC_VOID		0x20000000
#define OSF1_IOC_OUT		0x40000000
#define OSF1_IOC_IN		0x80000000
#define OSF1_IOC_INOUT		(OSF1_IOC_IN|OSF1_IOC_OUT)
#define OSF1_IOC_DIRMASK	0xe0000000


/* mman.h */

/* protection mask */
#define OSF1_PROT_NONE		0		/* pseudo-flag */
#define	OSF1_PROT_READ		0x0001
#define	OSF1_PROT_WRITE		0x0002
#define	OSF1_PROT_EXEC		0x0004

/* mmap flags */
#define OSF1_MAP_SHARED		0x0001
#define OSF1_MAP_PRIVATE	0x0002

#define OSF1_MAP_FILE		0		/* pseudo-flag */
#define OSF1_MAP_ANON		0x0010
#define OSF1_MAP_TYPE		0x00f0

#define OSF1_MAP_FIXED		0x0100
#define OSF1_MAP_VARIABLE	0		/* pseudo-flag */

#define OSF1_MAP_HASSEMAPHORE	0x0200
#define OSF1_MAP_INHERIT	0x0400
#define OSF1_MAP_UNALIGNED	0x0800


/* mount.h */

/* file system type numbers */
#define OSF1_MOUNT_NONE		0
#define OSF1_MOUNT_UFS		1
#define OSF1_MOUNT_NFS		2
#define OSF1_MOUNT_MFS		3
#define OSF1_MOUNT_PC		4
#define OSF1_MOUNT_S5FS		5
#define OSF1_MOUNT_CDFS		6
#define OSF1_MOUNT_DFS		7
#define OSF1_MOUNT_EFS		8
#define OSF1_MOUNT_PROCFS	9
#define OSF1_MOUNT_MSFS		10
#define OSF1_MOUNT_FFM		11
#define OSF1_MOUNT_FDFS		12
#define OSF1_MOUNT_ADDON	13
#define OSF1_MOUNT_NFS3		14

#define OSF1_MNAMELEN		90

/* MFS mount argument structure */
struct osf1_mfs_args {
	osf1_data_ptr	name;
	osf1_data_ptr	base;
	osf1_u_int	size;
};

/* NFS mount argument structure */
struct osf1_nfs_args {
	osf1_data_ptr	addr;
	osf1_data_ptr	fh;
	osf1_int	flags;
	osf1_int	wsize;
	osf1_int	rsize;
	osf1_int	timeo;
	osf1_int	retrans;
	osf1_data_ptr	hostname;
	osf1_int	acregmin;
	osf1_int	acregmax;
	osf1_int	acdirmin;
	osf1_int	acdirmax;
	osf1_data_ptr	netname;
	osf1_data_ptr	pathconf;
};

/* NFS mount argument structure flags */
#define OSF1_NFSMNT_SOFT	0x00000001
#define OSF1_NFSMNT_WSIZE	0x00000002
#define OSF1_NFSMNT_RSIZE	0x00000004
#define OSF1_NFSMNT_TIMEO	0x00000008
#define OSF1_NFSMNT_RETRANS	0x00000010
#define OSF1_NFSMNT_HOSTNAME	0x00000020
#define OSF1_NFSMNT_INT		0x00000040
#define OSF1_NFSMNT_NOCONN	0x00000080
#define OSF1_NFSMNT_NOAC	0x00000100
#define OSF1_NFSMNT_ACREGMIN	0x00000200
#define OSF1_NFSMNT_ACREGMAX	0x00000400
#define OSF1_NFSMNT_ACDIRMIN	0x00000800
#define OSF1_NFSMNT_ACDIRMAX	0x00001000
#define OSF1_NFSMNT_NOCTO	0x00002000
#define OSF1_NFSMNT_POSIX	0x00004000
#define OSF1_NFSMNT_AUTO	0x00008000
#define OSF1_NFSMNT_SEC		0x00010000
#define OSF1_NFSMNT_TCP		0x00020000
#define OSF1_NFSMNT_PROPLIST	0x00040000
/* no				0x00080000+ */

/* union of all mount argument structures */
union osf1_mount_info {
	struct osf1_mfs_args mfs_args;
	struct osf1_nfs_args nfs_args;
	char		pad[80];
};

/* statfs structure */
struct osf1_statfs {
	osf1_short	f_type;
	osf1_short	f_flags;
	osf1_int	f_fsize;
	osf1_int	f_bsize;
	osf1_int	f_blocks;
	osf1_int	f_bfree;
	osf1_int	f_bavail;
	osf1_int	f_files;
	osf1_int	f_ffree;
	osf1_fsid_t	f_fsid;
	osf1_int	f_spare[9];
	char		f_mntonname[OSF1_MNAMELEN];
	char		f_mntfromname[OSF1_MNAMELEN];
	union osf1_mount_info mount_info;
};


/* reboot.h */

/* reboot flags */
#define OSF1_RB_AUTOBOOT	0		/* pseudo-flag */

#define OSF1_RB_ASKNAME		0x0001
#define OSF1_RB_SINGLE		0x0002
#define OSF1_RB_NOSYNC		0x0004
#define OSF1_RB_KDB		0x0004		/* == RB_NOSYNC; boot only? */
#define OSF1_RB_HALT		0x0008
#define OSF1_RB_INITNAME	0x0010
#define OSF1_RB_DFLTROOT	0x0020
#define OSF1_RB_ALTBOOT		0x0040
#define OSF1_RB_UNIPROC		0x0080
#define OSF1_RB_PARAM		0x0100
#define OSF1_RB_DUMP		0x0200


/* resource.h */

#define OSF1_RLIMIT_CPU		0
#define OSF1_RLIMIT_FSIZE	1
#define OSF1_RLIMIT_DATA	2
#define OSF1_RLIMIT_STACK	3
#define OSF1_RLIMIT_CORE	4
#define OSF1_RLIMIT_RSS		5
#define OSF1_RLIMIT_NOFILE	6
#define OSF1_RLIMIT_AS		7

#define OSF1_RLIM_INFINITY	0x7fffffffffffffffL 

struct osf1_rlimit {
	rlim_t	rlim_cur;
	rlim_t	rlim_max;
};


/* signal.h */

struct osf1_sigaction {
	osf1_fcn_ptr	sa_handler;
	osf1_sigset_t	sa_mask;
	osf1_int	sa_flags;
	osf1_int	sa_signo;
};

/* actually from sysmisc.h */
struct osf1_sigaltstack {
	osf1_data_ptr	ss_sp;
	osf1_int	ss_flags;
	osf1_size_t	ss_size;
};

/* sigaction flags */
#define OSF1_SA_ONSTACK		0x00000001
#define OSF1_SA_RESTART		0x00000002
#define OSF1_SA_NOCLDSTOP	0x00000004
#define OSF1_SA_NODEFER		0x00000008
#define OSF1_SA_RESETHAND	0x00000010
#define OSF1_SA_NOCLDWAIT	0x00000020
#define OSF1_SA_SIGINFO		0x00000040

/* sigaltstack flags */
#define OSF1_SS_ONSTACK		0x00000001
#define OSF1_SS_DISABLE		0x00000002
#define OSF1_SS_NOMASK		0x00000004
#define OSF1_SS_UCONTEXT	0x00000008


/* socket.h */

/* max message iov len */
#define	OSF1_MSG_MAXIOVLEN	16

/* send/recv-family message flags */
#define OSF1_MSG_OOB		0x0001
#define OSF1_MSG_PEEK		0x0002
#define OSF1_MSG_DONTROUTE	0x0004
#define OSF1_MSG_EOR		0x0008
#define OSF1_MSG_TRUNC		0x0010
#define OSF1_MSG_CTRUNC		0x0020
#define OSF1_MSG_WAITALL	0x0040


/* stat.h */

struct osf1_stat {
	osf1_dev_t	st_dev;
	osf1_ino_t	st_ino;
	osf1_mode_t	st_mode;
	osf1_nlink_t	st_nlink;
	osf1_uid_t	st_uid;
	osf1_gid_t	st_gid;
	osf1_dev_t	st_rdev;
	osf1_off_t	st_size;
	osf1_time_t	st_atime_sec;
	osf1_int	st_spare1;
	osf1_time_t	st_mtime_sec;
	osf1_int	st_spare2;
	osf1_time_t	st_ctime_sec;
	osf1_int	st_spare3;
	osf1_uint_t	st_blksize;
	osf1_int	st_blocks;
	osf1_uint_t	st_flags;
	osf1_uint_t	st_gen;
};


/* types.h */

#define osf1_major(x)	((((dev_t)(x)) >> 20) & 0x00000fff)
#define osf1_minor(x)	((((dev_t)(x)) >>  0) & 0x000fffff)
#define osf1_makedev(x,y) ((((dev_t)(x)) << 20) | ((dev_t)(x)))


/* uio.h */

/*
 * The X/Open version of this uses size_t iov_len, but we can't count on
 * the not-in-int bits being zero.  (The non-X/Open version uses int.)
 */
struct osf1_iovec {
	osf1_data_ptr	iov_base;
	osf1_int	iov_len;
};

#endif /* _COMPAT_OSF1_OSF1_H_ */

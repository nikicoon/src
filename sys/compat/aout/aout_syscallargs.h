/*	$NetBSD: aout_syscallargs.h,v 1.10 2000/08/07 18:10:24 bjh21 Exp $	*/

/*
 * System call argument lists.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.10 2000/08/07 17:59:35 bjh21 Exp 
 */

#ifndef _AOUT_SYS__SYSCALLARGS_H_
#define _AOUT_SYS__SYSCALLARGS_H_

#ifdef	syscallarg
#undef	syscallarg
#endif

#define	syscallarg(x)							\
	union {								\
		register_t pad;						\
		struct { x datum; } le;					\
		struct {						\
			int8_t pad[ (sizeof (register_t) < sizeof (x))	\
				? 0					\
				: sizeof (register_t) - sizeof (x)];	\
			x datum;					\
		} be;							\
	}

struct aout_sys_open_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
	syscallarg(mode_t) mode;
};

struct aout_sys_creat_args {
	syscallarg(const char *) path;
	syscallarg(mode_t) mode;
};

struct aout_sys_link_args {
	syscallarg(const char *) path;
	syscallarg(const char *) link;
};

struct aout_sys_unlink_args {
	syscallarg(const char *) path;
};

struct aout_sys_chdir_args {
	syscallarg(const char *) path;
};

struct aout_sys_mknod_args {
	syscallarg(const char *) path;
	syscallarg(mode_t) mode;
	syscallarg(dev_t) dev;
};

struct aout_sys_chmod_args {
	syscallarg(const char *) path;
	syscallarg(mode_t) mode;
};

struct aout_sys_chown_args {
	syscallarg(const char *) path;
	syscallarg(uid_t) uid;
	syscallarg(gid_t) gid;
};

struct aout_sys_mount_args {
	syscallarg(const char *) type;
	syscallarg(const char *) path;
	syscallarg(int) flags;
	syscallarg(void *) data;
};

struct aout_sys_unmount_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
};

struct aout_sys_access_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
};

struct aout_sys_chflags_args {
	syscallarg(const char *) path;
	syscallarg(u_long) flags;
};

struct aout_compat_43_sys_stat_args {
	syscallarg(const char *) path;
	syscallarg(struct stat43 *) ub;
};

struct aout_compat_43_sys_lstat_args {
	syscallarg(const char *) path;
	syscallarg(struct stat43 *) ub;
};

struct aout_sys_ktrace_args {
	syscallarg(const char *) fname;
	syscallarg(int) ops;
	syscallarg(int) facs;
	syscallarg(int) pid;
};

struct aout_sys_acct_args {
	syscallarg(const char *) path;
};

struct aout_sys_revoke_args {
	syscallarg(const char *) path;
};

struct aout_sys_symlink_args {
	syscallarg(const char *) path;
	syscallarg(const char *) link;
};

struct aout_sys_readlink_args {
	syscallarg(const char *) path;
	syscallarg(char *) buf;
	syscallarg(size_t) count;
};

struct aout_sys_execve_args {
	syscallarg(const char *) path;
	syscallarg(char *const *) argp;
	syscallarg(char *const *) envp;
};

struct aout_sys_chroot_args {
	syscallarg(const char *) path;
};

struct aout_sys_rename_args {
	syscallarg(const char *) from;
	syscallarg(const char *) to;
};

struct aout_compat_43_sys_truncate_args {
	syscallarg(const char *) path;
	syscallarg(long) length;
};

struct aout_sys_mkfifo_args {
	syscallarg(const char *) path;
	syscallarg(mode_t) mode;
};

struct aout_sys_mkdir_args {
	syscallarg(const char *) path;
	syscallarg(mode_t) mode;
};

struct aout_sys_rmdir_args {
	syscallarg(const char *) path;
};

struct aout_sys_utimes_args {
	syscallarg(const char *) path;
	syscallarg(const struct timeval *) tptr;
};

struct aout_sys_quotactl_args {
	syscallarg(const char *) path;
	syscallarg(int) cmd;
	syscallarg(int) uid;
	syscallarg(caddr_t) arg;
};

struct aout_sys_statfs_args {
	syscallarg(const char *) path;
	syscallarg(struct statfs *) buf;
};

struct aout_sys_getfh_args {
	syscallarg(const char *) fname;
	syscallarg(fhandle_t *) fhp;
};

struct aout_compat_12_sys_stat_args {
	syscallarg(const char *) path;
	syscallarg(struct stat12 *) ub;
};

struct aout_compat_12_sys_lstat_args {
	syscallarg(const char *) path;
	syscallarg(struct stat12 *) ub;
};

struct aout_sys_pathconf_args {
	syscallarg(const char *) path;
	syscallarg(int) name;
};

struct aout_sys_truncate_args {
	syscallarg(const char *) path;
	syscallarg(int) pad;
	syscallarg(off_t) length;
};

struct aout_sys_undelete_args {
	syscallarg(const char *) path;
};

struct aout_sys___posix_rename_args {
	syscallarg(const char *) from;
	syscallarg(const char *) to;
};

struct aout_sys_lchmod_args {
	syscallarg(const char *) path;
	syscallarg(mode_t) mode;
};

struct aout_sys_lchown_args {
	syscallarg(const char *) path;
	syscallarg(uid_t) uid;
	syscallarg(gid_t) gid;
};

struct aout_sys_lutimes_args {
	syscallarg(const char *) path;
	syscallarg(const struct timeval *) tptr;
};

struct aout_sys___posix_chown_args {
	syscallarg(const char *) path;
	syscallarg(uid_t) uid;
	syscallarg(gid_t) gid;
};

/*
 * System call prototypes.
 */

int	sys_exit	__P((struct proc *, void *, register_t *));
int	sys_fork	__P((struct proc *, void *, register_t *));
int	sys_read	__P((struct proc *, void *, register_t *));
int	sys_write	__P((struct proc *, void *, register_t *));
int	aout_sys_open	__P((struct proc *, void *, register_t *));
int	sys_close	__P((struct proc *, void *, register_t *));
int	sys_wait4	__P((struct proc *, void *, register_t *));
int	aout_sys_creat	__P((struct proc *, void *, register_t *));
int	aout_sys_link	__P((struct proc *, void *, register_t *));
int	aout_sys_unlink	__P((struct proc *, void *, register_t *));
int	aout_sys_chdir	__P((struct proc *, void *, register_t *));
int	sys_fchdir	__P((struct proc *, void *, register_t *));
int	aout_sys_mknod	__P((struct proc *, void *, register_t *));
int	aout_sys_chmod	__P((struct proc *, void *, register_t *));
int	aout_sys_chown	__P((struct proc *, void *, register_t *));
int	sys_obreak	__P((struct proc *, void *, register_t *));
int	sys_getfsstat	__P((struct proc *, void *, register_t *));
int	compat_43_sys_lseek	__P((struct proc *, void *, register_t *));
int	sys_getpid	__P((struct proc *, void *, register_t *));
int	aout_sys_mount	__P((struct proc *, void *, register_t *));
int	aout_sys_unmount	__P((struct proc *, void *, register_t *));
int	sys_setuid	__P((struct proc *, void *, register_t *));
int	sys_getuid	__P((struct proc *, void *, register_t *));
int	sys_geteuid	__P((struct proc *, void *, register_t *));
int	sys_ptrace	__P((struct proc *, void *, register_t *));
int	sys_recvmsg	__P((struct proc *, void *, register_t *));
int	sys_sendmsg	__P((struct proc *, void *, register_t *));
int	sys_recvfrom	__P((struct proc *, void *, register_t *));
int	sys_accept	__P((struct proc *, void *, register_t *));
int	sys_getpeername	__P((struct proc *, void *, register_t *));
int	sys_getsockname	__P((struct proc *, void *, register_t *));
int	aout_sys_access	__P((struct proc *, void *, register_t *));
int	aout_sys_chflags	__P((struct proc *, void *, register_t *));
int	sys_fchflags	__P((struct proc *, void *, register_t *));
int	sys_sync	__P((struct proc *, void *, register_t *));
int	sys_kill	__P((struct proc *, void *, register_t *));
int	aout_compat_43_sys_stat	__P((struct proc *, void *, register_t *));
int	sys_getppid	__P((struct proc *, void *, register_t *));
int	aout_compat_43_sys_lstat	__P((struct proc *, void *, register_t *));
int	sys_dup	__P((struct proc *, void *, register_t *));
int	sys_pipe	__P((struct proc *, void *, register_t *));
int	sys_getegid	__P((struct proc *, void *, register_t *));
int	sys_profil	__P((struct proc *, void *, register_t *));
#if defined(KTRACE) || !defined(_KERNEL)
int	aout_sys_ktrace	__P((struct proc *, void *, register_t *));
#else
#endif
int	compat_13_sys_sigaction	__P((struct proc *, void *, register_t *));
int	sys_getgid	__P((struct proc *, void *, register_t *));
int	compat_13_sys_sigprocmask	__P((struct proc *, void *, register_t *));
int	sys___getlogin	__P((struct proc *, void *, register_t *));
int	sys_setlogin	__P((struct proc *, void *, register_t *));
int	aout_sys_acct	__P((struct proc *, void *, register_t *));
int	compat_13_sys_sigpending	__P((struct proc *, void *, register_t *));
int	compat_13_sys_sigaltstack	__P((struct proc *, void *, register_t *));
int	sys_ioctl	__P((struct proc *, void *, register_t *));
int	compat_12_sys_reboot	__P((struct proc *, void *, register_t *));
int	aout_sys_revoke	__P((struct proc *, void *, register_t *));
int	aout_sys_symlink	__P((struct proc *, void *, register_t *));
int	aout_sys_readlink	__P((struct proc *, void *, register_t *));
int	aout_sys_execve	__P((struct proc *, void *, register_t *));
int	sys_umask	__P((struct proc *, void *, register_t *));
int	aout_sys_chroot	__P((struct proc *, void *, register_t *));
int	compat_43_sys_fstat	__P((struct proc *, void *, register_t *));
int	compat_43_sys_getkerninfo	__P((struct proc *, void *, register_t *));
int	compat_43_sys_getpagesize	__P((struct proc *, void *, register_t *));
int	compat_12_sys_msync	__P((struct proc *, void *, register_t *));
int	sys_vfork	__P((struct proc *, void *, register_t *));
int	sys_sbrk	__P((struct proc *, void *, register_t *));
int	sys_sstk	__P((struct proc *, void *, register_t *));
int	compat_43_sys_mmap	__P((struct proc *, void *, register_t *));
int	sys_ovadvise	__P((struct proc *, void *, register_t *));
int	sys_munmap	__P((struct proc *, void *, register_t *));
int	sys_mprotect	__P((struct proc *, void *, register_t *));
int	sys_madvise	__P((struct proc *, void *, register_t *));
int	sys_mincore	__P((struct proc *, void *, register_t *));
int	sys_getgroups	__P((struct proc *, void *, register_t *));
int	sys_setgroups	__P((struct proc *, void *, register_t *));
int	sys_getpgrp	__P((struct proc *, void *, register_t *));
int	sys_setpgid	__P((struct proc *, void *, register_t *));
int	sys_setitimer	__P((struct proc *, void *, register_t *));
int	compat_43_sys_wait	__P((struct proc *, void *, register_t *));
int	compat_12_sys_swapon	__P((struct proc *, void *, register_t *));
int	sys_getitimer	__P((struct proc *, void *, register_t *));
int	compat_43_sys_gethostname	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sethostname	__P((struct proc *, void *, register_t *));
int	compat_43_sys_getdtablesize	__P((struct proc *, void *, register_t *));
int	sys_dup2	__P((struct proc *, void *, register_t *));
int	sys_fcntl	__P((struct proc *, void *, register_t *));
int	sys_select	__P((struct proc *, void *, register_t *));
int	sys_fsync	__P((struct proc *, void *, register_t *));
int	sys_setpriority	__P((struct proc *, void *, register_t *));
int	sys_socket	__P((struct proc *, void *, register_t *));
int	sys_connect	__P((struct proc *, void *, register_t *));
int	compat_43_sys_accept	__P((struct proc *, void *, register_t *));
int	sys_getpriority	__P((struct proc *, void *, register_t *));
int	compat_43_sys_send	__P((struct proc *, void *, register_t *));
int	compat_43_sys_recv	__P((struct proc *, void *, register_t *));
int	compat_13_sys_sigreturn	__P((struct proc *, void *, register_t *));
int	sys_bind	__P((struct proc *, void *, register_t *));
int	sys_setsockopt	__P((struct proc *, void *, register_t *));
int	sys_listen	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sigvec	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sigblock	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sigsetmask	__P((struct proc *, void *, register_t *));
int	compat_13_sys_sigsuspend	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sigstack	__P((struct proc *, void *, register_t *));
int	compat_43_sys_recvmsg	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sendmsg	__P((struct proc *, void *, register_t *));
int	sys_gettimeofday	__P((struct proc *, void *, register_t *));
int	sys_getrusage	__P((struct proc *, void *, register_t *));
int	sys_getsockopt	__P((struct proc *, void *, register_t *));
int	sys_readv	__P((struct proc *, void *, register_t *));
int	sys_writev	__P((struct proc *, void *, register_t *));
int	sys_settimeofday	__P((struct proc *, void *, register_t *));
int	sys_fchown	__P((struct proc *, void *, register_t *));
int	sys_fchmod	__P((struct proc *, void *, register_t *));
int	compat_43_sys_recvfrom	__P((struct proc *, void *, register_t *));
int	sys_setreuid	__P((struct proc *, void *, register_t *));
int	sys_setregid	__P((struct proc *, void *, register_t *));
int	aout_sys_rename	__P((struct proc *, void *, register_t *));
int	aout_compat_43_sys_truncate	__P((struct proc *, void *, register_t *));
int	compat_43_sys_ftruncate	__P((struct proc *, void *, register_t *));
int	sys_flock	__P((struct proc *, void *, register_t *));
int	aout_sys_mkfifo	__P((struct proc *, void *, register_t *));
int	sys_sendto	__P((struct proc *, void *, register_t *));
int	sys_shutdown	__P((struct proc *, void *, register_t *));
int	sys_socketpair	__P((struct proc *, void *, register_t *));
int	aout_sys_mkdir	__P((struct proc *, void *, register_t *));
int	aout_sys_rmdir	__P((struct proc *, void *, register_t *));
int	aout_sys_utimes	__P((struct proc *, void *, register_t *));
int	sys_adjtime	__P((struct proc *, void *, register_t *));
int	compat_43_sys_getpeername	__P((struct proc *, void *, register_t *));
int	compat_43_sys_gethostid	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sethostid	__P((struct proc *, void *, register_t *));
int	compat_43_sys_getrlimit	__P((struct proc *, void *, register_t *));
int	compat_43_sys_setrlimit	__P((struct proc *, void *, register_t *));
int	compat_43_sys_killpg	__P((struct proc *, void *, register_t *));
int	sys_setsid	__P((struct proc *, void *, register_t *));
int	aout_sys_quotactl	__P((struct proc *, void *, register_t *));
int	compat_43_sys_quota	__P((struct proc *, void *, register_t *));
int	compat_43_sys_getsockname	__P((struct proc *, void *, register_t *));
#if defined(NFS) || defined(NFSSERVER) || !defined(_KERNEL)
int	sys_nfssvc	__P((struct proc *, void *, register_t *));
#else
#endif
int	compat_43_sys_getdirentries	__P((struct proc *, void *, register_t *));
int	aout_sys_statfs	__P((struct proc *, void *, register_t *));
int	sys_fstatfs	__P((struct proc *, void *, register_t *));
#if defined(NFS) || defined(NFSSERVER) || !defined(_KERNEL)
int	aout_sys_getfh	__P((struct proc *, void *, register_t *));
#else
#endif
int	compat_09_sys_getdomainname	__P((struct proc *, void *, register_t *));
int	compat_09_sys_setdomainname	__P((struct proc *, void *, register_t *));
int	compat_09_sys_uname	__P((struct proc *, void *, register_t *));
int	sys_sysarch	__P((struct proc *, void *, register_t *));
#if (defined(SYSVSEM) || !defined(_KERNEL)) && !defined(alpha)
int	compat_10_sys_semsys	__P((struct proc *, void *, register_t *));
#else
#endif
#if (defined(SYSVMSG) || !defined(_KERNEL)) && !defined(alpha)
int	compat_10_sys_msgsys	__P((struct proc *, void *, register_t *));
#else
#endif
#if (defined(SYSVSHM) || !defined(_KERNEL)) && !defined(alpha)
int	compat_10_sys_shmsys	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_pread	__P((struct proc *, void *, register_t *));
int	sys_pwrite	__P((struct proc *, void *, register_t *));
int	sys_ntp_gettime	__P((struct proc *, void *, register_t *));
#if defined(NTP) || !defined(_KERNEL)
int	sys_ntp_adjtime	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_setgid	__P((struct proc *, void *, register_t *));
int	sys_setegid	__P((struct proc *, void *, register_t *));
int	sys_seteuid	__P((struct proc *, void *, register_t *));
#if defined(LFS) || !defined(_KERNEL)
int	sys_lfs_bmapv	__P((struct proc *, void *, register_t *));
int	sys_lfs_markv	__P((struct proc *, void *, register_t *));
int	sys_lfs_segclean	__P((struct proc *, void *, register_t *));
int	sys_lfs_segwait	__P((struct proc *, void *, register_t *));
#else
#endif
int	aout_compat_12_sys_stat	__P((struct proc *, void *, register_t *));
int	compat_12_sys_fstat	__P((struct proc *, void *, register_t *));
int	aout_compat_12_sys_lstat	__P((struct proc *, void *, register_t *));
int	aout_sys_pathconf	__P((struct proc *, void *, register_t *));
int	sys_fpathconf	__P((struct proc *, void *, register_t *));
int	sys_getrlimit	__P((struct proc *, void *, register_t *));
int	sys_setrlimit	__P((struct proc *, void *, register_t *));
int	compat_12_sys_getdirentries	__P((struct proc *, void *, register_t *));
int	sys_mmap	__P((struct proc *, void *, register_t *));
int	sys_lseek	__P((struct proc *, void *, register_t *));
int	aout_sys_truncate	__P((struct proc *, void *, register_t *));
int	sys_ftruncate	__P((struct proc *, void *, register_t *));
int	sys___sysctl	__P((struct proc *, void *, register_t *));
int	sys_mlock	__P((struct proc *, void *, register_t *));
int	sys_munlock	__P((struct proc *, void *, register_t *));
int	aout_sys_undelete	__P((struct proc *, void *, register_t *));
int	sys_futimes	__P((struct proc *, void *, register_t *));
int	sys_getpgid	__P((struct proc *, void *, register_t *));
int	sys_reboot	__P((struct proc *, void *, register_t *));
int	sys_poll	__P((struct proc *, void *, register_t *));
#if defined(LKM) || !defined(_KERNEL)
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
#else	/* !LKM */
#endif	/* !LKM */
#if defined(SYSVSEM) || !defined(_KERNEL)
#ifdef COMPAT_14
int	compat_14_sys___semctl	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_semget	__P((struct proc *, void *, register_t *));
int	sys_semop	__P((struct proc *, void *, register_t *));
int	sys_semconfig	__P((struct proc *, void *, register_t *));
#else
#endif
#if defined(SYSVMSG) || !defined(_KERNEL)
#ifdef COMPAT_14
int	compat_14_sys_msgctl	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_msgget	__P((struct proc *, void *, register_t *));
int	sys_msgsnd	__P((struct proc *, void *, register_t *));
int	sys_msgrcv	__P((struct proc *, void *, register_t *));
#else
#endif
#if defined(SYSVSHM) || !defined(_KERNEL)
int	sys_shmat	__P((struct proc *, void *, register_t *));
#ifdef COMPAT_14
int	compat_14_sys_shmctl	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_shmdt	__P((struct proc *, void *, register_t *));
int	sys_shmget	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_clock_gettime	__P((struct proc *, void *, register_t *));
int	sys_clock_settime	__P((struct proc *, void *, register_t *));
int	sys_clock_getres	__P((struct proc *, void *, register_t *));
int	sys_nanosleep	__P((struct proc *, void *, register_t *));
int	sys_fdatasync	__P((struct proc *, void *, register_t *));
int	sys_mlockall	__P((struct proc *, void *, register_t *));
int	sys_munlockall	__P((struct proc *, void *, register_t *));
int	aout_sys___posix_rename	__P((struct proc *, void *, register_t *));
int	sys_swapctl	__P((struct proc *, void *, register_t *));
int	sys_getdents	__P((struct proc *, void *, register_t *));
int	sys_minherit	__P((struct proc *, void *, register_t *));
int	aout_sys_lchmod	__P((struct proc *, void *, register_t *));
int	aout_sys_lchown	__P((struct proc *, void *, register_t *));
int	aout_sys_lutimes	__P((struct proc *, void *, register_t *));
int	sys___msync13	__P((struct proc *, void *, register_t *));
int	sys___stat13	__P((struct proc *, void *, register_t *));
int	sys___fstat13	__P((struct proc *, void *, register_t *));
int	sys___lstat13	__P((struct proc *, void *, register_t *));
int	sys___sigaltstack14	__P((struct proc *, void *, register_t *));
int	sys___vfork14	__P((struct proc *, void *, register_t *));
int	aout_sys___posix_chown	__P((struct proc *, void *, register_t *));
int	sys___posix_fchown	__P((struct proc *, void *, register_t *));
int	sys___posix_lchown	__P((struct proc *, void *, register_t *));
int	sys_getsid	__P((struct proc *, void *, register_t *));
#if defined(KTRACE) || !defined(_KERNEL)
int	sys_fktrace	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_preadv	__P((struct proc *, void *, register_t *));
int	sys_pwritev	__P((struct proc *, void *, register_t *));
int	sys___sigaction14	__P((struct proc *, void *, register_t *));
int	sys___sigpending14	__P((struct proc *, void *, register_t *));
int	sys___sigprocmask14	__P((struct proc *, void *, register_t *));
int	sys___sigsuspend14	__P((struct proc *, void *, register_t *));
int	sys___sigreturn14	__P((struct proc *, void *, register_t *));
int	sys___getcwd	__P((struct proc *, void *, register_t *));
int	sys_fchroot	__P((struct proc *, void *, register_t *));
int	sys_fhopen	__P((struct proc *, void *, register_t *));
int	sys_fhstat	__P((struct proc *, void *, register_t *));
int	sys_fhstatfs	__P((struct proc *, void *, register_t *));
#if defined(SYSVSEM) || !defined(_KERNEL)
int	sys_____semctl13	__P((struct proc *, void *, register_t *));
#else
#endif
#if defined(SYSVMSG) || !defined(_KERNEL)
int	sys___msgctl13	__P((struct proc *, void *, register_t *));
#else
#endif
#if defined(SYSVSHM) || !defined(_KERNEL)
int	sys___shmctl13	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_lchflags	__P((struct proc *, void *, register_t *));
int	sys_issetugid	__P((struct proc *, void *, register_t *));
#endif /* _AOUT_SYS__SYSCALLARGS_H_ */

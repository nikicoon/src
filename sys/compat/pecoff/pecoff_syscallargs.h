/* $NetBSD: pecoff_syscallargs.h,v 1.3 2002/05/03 00:26:06 eeh Exp $ */

/*
 * System call argument lists.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.3 2002/05/03 00:20:57 eeh Exp 
 */

#ifndef _PECOFF_SYS__SYSCALLARGS_H_
#define	_PECOFF_SYS__SYSCALLARGS_H_

#ifdef	syscallarg
#undef	syscallarg
#endif

#define	syscallarg(x)							\
	union {								\
		register_t pad;						\
		struct { x datum; } le;					\
		struct { /* LINTED zero array dimension */		\
			int8_t pad[  /* CONSTCOND */			\
				(sizeof (register_t) < sizeof (x))	\
				? 0					\
				: sizeof (register_t) - sizeof (x)];	\
			x datum;					\
		} be;							\
	}

struct pecoff_sys_open_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
	syscallarg(mode_t) mode;
};

struct pecoff_sys_link_args {
	syscallarg(const char *) path;
	syscallarg(const char *) link;
};

struct pecoff_sys_unlink_args {
	syscallarg(const char *) path;
};

struct pecoff_sys_chdir_args {
	syscallarg(const char *) path;
};

struct pecoff_sys_chmod_args {
	syscallarg(const char *) path;
	syscallarg(mode_t) mode;
};

struct pecoff_sys_chown_args {
	syscallarg(const char *) path;
	syscallarg(uid_t) uid;
	syscallarg(gid_t) gid;
};

struct pecoff_sys_unmount_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
};

struct pecoff_sys_access_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
};

struct pecoff_sys_chflags_args {
	syscallarg(const char *) path;
	syscallarg(u_long) flags;
};

struct pecoff_sys_revoke_args {
	syscallarg(const char *) path;
};

struct pecoff_sys_symlink_args {
	syscallarg(const char *) path;
	syscallarg(const char *) link;
};

struct pecoff_sys_readlink_args {
	syscallarg(const char *) path;
	syscallarg(char *) buf;
	syscallarg(size_t) count;
};

struct pecoff_sys_execve_args {
	syscallarg(const char *) path;
	syscallarg(char *const *) argp;
	syscallarg(char *const *) envp;
};

struct pecoff_sys_chroot_args {
	syscallarg(const char *) path;
};

struct pecoff_sys_rename_args {
	syscallarg(const char *) from;
	syscallarg(const char *) to;
};

struct pecoff_sys_rmdir_args {
	syscallarg(const char *) path;
};

struct pecoff_sys_utimes_args {
	syscallarg(const char *) path;
	syscallarg(const struct timeval *) tptr;
};

struct pecoff_sys_statfs_args {
	syscallarg(const char *) path;
	syscallarg(struct statfs *) buf;
};

struct pecoff_sys_getfh_args {
	syscallarg(const char *) fname;
	syscallarg(fhandle_t *) fhp;
};

struct pecoff_sys_pathconf_args {
	syscallarg(const char *) path;
	syscallarg(int) name;
};

struct pecoff_sys_truncate_args {
	syscallarg(const char *) path;
	syscallarg(int) pad;
	syscallarg(off_t) length;
};

struct pecoff_sys_undelete_args {
	syscallarg(const char *) path;
};

struct pecoff_sys___posix_rename_args {
	syscallarg(const char *) from;
	syscallarg(const char *) to;
};

struct pecoff_sys_lchmod_args {
	syscallarg(const char *) path;
	syscallarg(mode_t) mode;
};

struct pecoff_sys_lchown_args {
	syscallarg(const char *) path;
	syscallarg(uid_t) uid;
	syscallarg(gid_t) gid;
};

struct pecoff_sys_lutimes_args {
	syscallarg(const char *) path;
	syscallarg(const struct timeval *) tptr;
};

struct pecoff_sys___posix_chown_args {
	syscallarg(const char *) path;
	syscallarg(uid_t) uid;
	syscallarg(gid_t) gid;
};

/*
 * System call prototypes.
 */

int	sys_exit(struct proc *, void *, register_t *);
int	sys_fork(struct proc *, void *, register_t *);
int	sys_read(struct proc *, void *, register_t *);
int	sys_write(struct proc *, void *, register_t *);
int	pecoff_sys_open(struct proc *, void *, register_t *);
int	sys_close(struct proc *, void *, register_t *);
int	sys_wait4(struct proc *, void *, register_t *);
int	pecoff_sys_link(struct proc *, void *, register_t *);
int	pecoff_sys_unlink(struct proc *, void *, register_t *);
int	pecoff_sys_chdir(struct proc *, void *, register_t *);
int	sys_fchdir(struct proc *, void *, register_t *);
int	sys_mknod(struct proc *, void *, register_t *);
int	pecoff_sys_chmod(struct proc *, void *, register_t *);
int	pecoff_sys_chown(struct proc *, void *, register_t *);
int	sys_obreak(struct proc *, void *, register_t *);
int	sys_getfsstat(struct proc *, void *, register_t *);
#ifdef COMPAT_43
int	sys_getpid_with_ppid(struct proc *, void *, register_t *);
#else
int	sys_getpid(struct proc *, void *, register_t *);
#endif
int	sys_mount(struct proc *, void *, register_t *);
int	pecoff_sys_unmount(struct proc *, void *, register_t *);
int	sys_setuid(struct proc *, void *, register_t *);
#ifdef COMPAT_43
int	sys_getuid_with_euid(struct proc *, void *, register_t *);
#else
int	sys_getuid(struct proc *, void *, register_t *);
#endif
int	sys_geteuid(struct proc *, void *, register_t *);
int	sys_ptrace(struct proc *, void *, register_t *);
int	sys_recvmsg(struct proc *, void *, register_t *);
int	sys_sendmsg(struct proc *, void *, register_t *);
int	sys_recvfrom(struct proc *, void *, register_t *);
int	sys_accept(struct proc *, void *, register_t *);
int	sys_getpeername(struct proc *, void *, register_t *);
int	sys_getsockname(struct proc *, void *, register_t *);
int	pecoff_sys_access(struct proc *, void *, register_t *);
int	pecoff_sys_chflags(struct proc *, void *, register_t *);
int	sys_fchflags(struct proc *, void *, register_t *);
int	sys_sync(struct proc *, void *, register_t *);
int	sys_kill(struct proc *, void *, register_t *);
int	sys_getppid(struct proc *, void *, register_t *);
int	sys_dup(struct proc *, void *, register_t *);
int	sys_pipe(struct proc *, void *, register_t *);
int	sys_getegid(struct proc *, void *, register_t *);
int	sys_profil(struct proc *, void *, register_t *);
#if defined(KTRACE) || !defined(_KERNEL)
int	sys_ktrace(struct proc *, void *, register_t *);
#else
#endif
#ifdef COMPAT_43
int	sys_getgid_with_egid(struct proc *, void *, register_t *);
#else
int	sys_getgid(struct proc *, void *, register_t *);
#endif
int	sys___getlogin(struct proc *, void *, register_t *);
int	sys_setlogin(struct proc *, void *, register_t *);
int	sys_acct(struct proc *, void *, register_t *);
int	sys_ioctl(struct proc *, void *, register_t *);
int	pecoff_sys_revoke(struct proc *, void *, register_t *);
int	pecoff_sys_symlink(struct proc *, void *, register_t *);
int	pecoff_sys_readlink(struct proc *, void *, register_t *);
int	pecoff_sys_execve(struct proc *, void *, register_t *);
int	sys_umask(struct proc *, void *, register_t *);
int	pecoff_sys_chroot(struct proc *, void *, register_t *);
int	sys_vfork(struct proc *, void *, register_t *);
int	sys_sbrk(struct proc *, void *, register_t *);
int	sys_sstk(struct proc *, void *, register_t *);
int	sys_ovadvise(struct proc *, void *, register_t *);
int	sys_munmap(struct proc *, void *, register_t *);
int	sys_mprotect(struct proc *, void *, register_t *);
int	sys_madvise(struct proc *, void *, register_t *);
int	sys_mincore(struct proc *, void *, register_t *);
int	sys_getgroups(struct proc *, void *, register_t *);
int	sys_setgroups(struct proc *, void *, register_t *);
int	sys_getpgrp(struct proc *, void *, register_t *);
int	sys_setpgid(struct proc *, void *, register_t *);
int	sys_setitimer(struct proc *, void *, register_t *);
int	sys_getitimer(struct proc *, void *, register_t *);
int	sys_dup2(struct proc *, void *, register_t *);
int	sys_fcntl(struct proc *, void *, register_t *);
int	sys_select(struct proc *, void *, register_t *);
int	sys_fsync(struct proc *, void *, register_t *);
int	sys_setpriority(struct proc *, void *, register_t *);
int	sys_socket(struct proc *, void *, register_t *);
int	sys_connect(struct proc *, void *, register_t *);
int	sys_getpriority(struct proc *, void *, register_t *);
int	sys_bind(struct proc *, void *, register_t *);
int	sys_setsockopt(struct proc *, void *, register_t *);
int	sys_listen(struct proc *, void *, register_t *);
int	sys_gettimeofday(struct proc *, void *, register_t *);
int	sys_getrusage(struct proc *, void *, register_t *);
int	sys_getsockopt(struct proc *, void *, register_t *);
int	sys_readv(struct proc *, void *, register_t *);
int	sys_writev(struct proc *, void *, register_t *);
int	sys_settimeofday(struct proc *, void *, register_t *);
int	sys_fchown(struct proc *, void *, register_t *);
int	sys_fchmod(struct proc *, void *, register_t *);
int	sys_setreuid(struct proc *, void *, register_t *);
int	sys_setregid(struct proc *, void *, register_t *);
int	pecoff_sys_rename(struct proc *, void *, register_t *);
int	sys_flock(struct proc *, void *, register_t *);
int	sys_mkfifo(struct proc *, void *, register_t *);
int	sys_sendto(struct proc *, void *, register_t *);
int	sys_shutdown(struct proc *, void *, register_t *);
int	sys_socketpair(struct proc *, void *, register_t *);
int	sys_mkdir(struct proc *, void *, register_t *);
int	pecoff_sys_rmdir(struct proc *, void *, register_t *);
int	pecoff_sys_utimes(struct proc *, void *, register_t *);
int	sys_adjtime(struct proc *, void *, register_t *);
int	sys_setsid(struct proc *, void *, register_t *);
int	sys_quotactl(struct proc *, void *, register_t *);
#if defined(NFS) || defined(NFSSERVER) || !defined(_KERNEL)
int	sys_nfssvc(struct proc *, void *, register_t *);
#else
#endif
int	pecoff_sys_statfs(struct proc *, void *, register_t *);
int	sys_fstatfs(struct proc *, void *, register_t *);
int	pecoff_sys_getfh(struct proc *, void *, register_t *);
int	sys_sysarch(struct proc *, void *, register_t *);
#if (defined(SYSVSEM) || !defined(_KERNEL)) && !defined(_LP64)
#else
#endif
#if (defined(SYSVMSG) || !defined(_KERNEL)) && !defined(_LP64)
#else
#endif
#if (defined(SYSVSHM) || !defined(_KERNEL)) && !defined(_LP64)
#else
#endif
int	sys_pread(struct proc *, void *, register_t *);
int	sys_pwrite(struct proc *, void *, register_t *);
int	sys_ntp_gettime(struct proc *, void *, register_t *);
#if defined(NTP) || !defined(_KERNEL)
int	sys_ntp_adjtime(struct proc *, void *, register_t *);
#else
#endif
int	sys_setgid(struct proc *, void *, register_t *);
int	sys_setegid(struct proc *, void *, register_t *);
int	sys_seteuid(struct proc *, void *, register_t *);
#if defined(LFS) || !defined(_KERNEL)
int	sys_lfs_bmapv(struct proc *, void *, register_t *);
int	sys_lfs_markv(struct proc *, void *, register_t *);
int	sys_lfs_segclean(struct proc *, void *, register_t *);
int	sys_lfs_segwait(struct proc *, void *, register_t *);
#else
#endif
int	pecoff_sys_pathconf(struct proc *, void *, register_t *);
int	sys_fpathconf(struct proc *, void *, register_t *);
int	sys_getrlimit(struct proc *, void *, register_t *);
int	sys_setrlimit(struct proc *, void *, register_t *);
int	sys_mmap(struct proc *, void *, register_t *);
int	sys_lseek(struct proc *, void *, register_t *);
int	pecoff_sys_truncate(struct proc *, void *, register_t *);
int	sys_ftruncate(struct proc *, void *, register_t *);
int	sys___sysctl(struct proc *, void *, register_t *);
int	sys_mlock(struct proc *, void *, register_t *);
int	sys_munlock(struct proc *, void *, register_t *);
int	pecoff_sys_undelete(struct proc *, void *, register_t *);
int	sys_futimes(struct proc *, void *, register_t *);
int	sys_getpgid(struct proc *, void *, register_t *);
int	sys_reboot(struct proc *, void *, register_t *);
int	sys_poll(struct proc *, void *, register_t *);
#if defined(LKM) || !defined(_KERNEL)
int	sys_lkmnosys(struct proc *, void *, register_t *);
int	sys_lkmnosys(struct proc *, void *, register_t *);
int	sys_lkmnosys(struct proc *, void *, register_t *);
int	sys_lkmnosys(struct proc *, void *, register_t *);
int	sys_lkmnosys(struct proc *, void *, register_t *);
int	sys_lkmnosys(struct proc *, void *, register_t *);
int	sys_lkmnosys(struct proc *, void *, register_t *);
int	sys_lkmnosys(struct proc *, void *, register_t *);
int	sys_lkmnosys(struct proc *, void *, register_t *);
int	sys_lkmnosys(struct proc *, void *, register_t *);
#else	/* !LKM */
#endif	/* !LKM */
#if defined(SYSVSEM) || !defined(_KERNEL)
int	sys_semget(struct proc *, void *, register_t *);
int	sys_semop(struct proc *, void *, register_t *);
int	sys_semconfig(struct proc *, void *, register_t *);
#else
#endif
#if defined(SYSVMSG) || !defined(_KERNEL)
int	sys_msgget(struct proc *, void *, register_t *);
int	sys_msgsnd(struct proc *, void *, register_t *);
int	sys_msgrcv(struct proc *, void *, register_t *);
#else
#endif
#if defined(SYSVSHM) || !defined(_KERNEL)
int	sys_shmat(struct proc *, void *, register_t *);
int	sys_shmdt(struct proc *, void *, register_t *);
int	sys_shmget(struct proc *, void *, register_t *);
#else
#endif
int	sys_clock_gettime(struct proc *, void *, register_t *);
int	sys_clock_settime(struct proc *, void *, register_t *);
int	sys_clock_getres(struct proc *, void *, register_t *);
int	sys_nanosleep(struct proc *, void *, register_t *);
int	sys_fdatasync(struct proc *, void *, register_t *);
int	sys_mlockall(struct proc *, void *, register_t *);
int	sys_munlockall(struct proc *, void *, register_t *);
int	pecoff_sys___posix_rename(struct proc *, void *, register_t *);
int	sys_swapctl(struct proc *, void *, register_t *);
int	sys_getdents(struct proc *, void *, register_t *);
int	sys_minherit(struct proc *, void *, register_t *);
int	pecoff_sys_lchmod(struct proc *, void *, register_t *);
int	pecoff_sys_lchown(struct proc *, void *, register_t *);
int	pecoff_sys_lutimes(struct proc *, void *, register_t *);
int	sys___msync13(struct proc *, void *, register_t *);
int	sys___stat13(struct proc *, void *, register_t *);
int	sys___fstat13(struct proc *, void *, register_t *);
int	sys___lstat13(struct proc *, void *, register_t *);
int	sys___sigaltstack14(struct proc *, void *, register_t *);
int	sys___vfork14(struct proc *, void *, register_t *);
int	pecoff_sys___posix_chown(struct proc *, void *, register_t *);
int	sys___posix_fchown(struct proc *, void *, register_t *);
int	sys___posix_lchown(struct proc *, void *, register_t *);
int	sys_getsid(struct proc *, void *, register_t *);
int	sys___clone(struct proc *, void *, register_t *);
#if defined(KTRACE) || !defined(_KERNEL)
int	sys_fktrace(struct proc *, void *, register_t *);
#else
#endif
int	sys_preadv(struct proc *, void *, register_t *);
int	sys_pwritev(struct proc *, void *, register_t *);
int	sys___sigaction14(struct proc *, void *, register_t *);
int	sys___sigpending14(struct proc *, void *, register_t *);
int	sys___sigprocmask14(struct proc *, void *, register_t *);
int	sys___sigsuspend14(struct proc *, void *, register_t *);
int	sys___sigreturn14(struct proc *, void *, register_t *);
int	sys___getcwd(struct proc *, void *, register_t *);
int	sys_fchroot(struct proc *, void *, register_t *);
int	sys_fhopen(struct proc *, void *, register_t *);
int	sys_fhstat(struct proc *, void *, register_t *);
int	sys_fhstatfs(struct proc *, void *, register_t *);
#if defined(SYSVSEM) || !defined(_KERNEL)
int	sys_____semctl13(struct proc *, void *, register_t *);
#else
#endif
#if defined(SYSVMSG) || !defined(_KERNEL)
int	sys___msgctl13(struct proc *, void *, register_t *);
#else
#endif
#if defined(SYSVSHM) || !defined(_KERNEL)
int	sys___shmctl13(struct proc *, void *, register_t *);
#else
#endif
int	sys_lchflags(struct proc *, void *, register_t *);
int	sys_issetugid(struct proc *, void *, register_t *);
int	sys_utrace(struct proc *, void *, register_t *);
#endif /* _PECOFF_SYS__SYSCALLARGS_H_ */

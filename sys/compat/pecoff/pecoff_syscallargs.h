/* $NetBSD: pecoff_syscallargs.h,v 1.13 2005/02/26 23:58:20 perry Exp $ */

/*
 * System call argument lists.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.11 2004/05/08 14:31:59 kent Exp
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
#ifdef COMPAT_20
#else
#endif
#ifdef COMPAT_43
#else
#endif

struct pecoff_sys_unmount_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
};
#ifdef COMPAT_43
#else
#endif

struct pecoff_sys_access_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
};

struct pecoff_sys_chflags_args {
	syscallarg(const char *) path;
	syscallarg(u_long) flags;
};
#if defined(KTRACE) || !defined(_KERNEL)
#else
#endif
#ifdef COMPAT_43
#else
#endif

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
#if defined(NFS) || defined(NFSSERVER) || !defined(_KERNEL)
#else
#endif

struct pecoff_sys_statfs_args {
	syscallarg(const char *) path;
	syscallarg(struct statvfs12 *) buf;
};
#ifdef COMPAT_20
#else
#endif

struct pecoff_sys_getfh_args {
	syscallarg(const char *) fname;
	syscallarg(fhandle_t *) fhp;
};
#if (defined(SYSVSEM) || !defined(_KERNEL)) && !defined(_LP64)
#else
#endif
#if (defined(SYSVMSG) || !defined(_KERNEL)) && !defined(_LP64)
#else
#endif
#if (defined(SYSVSHM) || !defined(_KERNEL)) && !defined(_LP64)
#else
#endif
#if defined(NTP) || !defined(_KERNEL)
#else
#endif
#if defined(LFS) || !defined(_KERNEL)
#else
#endif

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
#if defined(LKM) || !defined(_KERNEL)
#else	/* !LKM */
#endif	/* !LKM */
#if defined(SYSVSEM) || !defined(_KERNEL)
#else
#endif
#if defined(SYSVMSG) || !defined(_KERNEL)
#else
#endif
#if defined(SYSVSHM) || !defined(_KERNEL)
#else
#endif
#if defined(P1003_1B_SEMAPHORE) || !defined(_KERNEL)
#else
#endif

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

struct pecoff_sys___stat13_args {
	syscallarg(const char *) path;
	syscallarg(struct stat *) ub;
};

struct pecoff_sys___lstat13_args {
	syscallarg(const char *) path;
	syscallarg(struct stat *) ub;
};

struct pecoff_sys___posix_chown_args {
	syscallarg(const char *) path;
	syscallarg(uid_t) uid;
	syscallarg(gid_t) gid;
};

struct pecoff_sys___posix_lchown_args {
	syscallarg(const char *) path;
	syscallarg(uid_t) uid;
	syscallarg(gid_t) gid;
};
#if defined(KTRACE) || !defined(_KERNEL)
#else
#endif
#ifdef COMPAT_16
#else
#endif
#ifdef COMPAT_20
#else
#endif
#if defined(SYSVSEM) || !defined(_KERNEL)
#else
#endif
#if defined(SYSVMSG) || !defined(_KERNEL)
#else
#endif
#if defined(SYSVSHM) || !defined(_KERNEL)
#else
#endif

struct pecoff_sys_lchflags_args {
	syscallarg(const char *) path;
	syscallarg(u_long) flags;
};

/*
 * System call prototypes.
 */

int	sys_exit(struct lwp *, void *, register_t *);

int	sys_fork(struct lwp *, void *, register_t *);

int	sys_read(struct lwp *, void *, register_t *);

int	sys_write(struct lwp *, void *, register_t *);

int	pecoff_sys_open(struct lwp *, void *, register_t *);

int	sys_close(struct lwp *, void *, register_t *);

int	sys_wait4(struct lwp *, void *, register_t *);

int	pecoff_sys_link(struct lwp *, void *, register_t *);

int	pecoff_sys_unlink(struct lwp *, void *, register_t *);

int	pecoff_sys_chdir(struct lwp *, void *, register_t *);

int	sys_fchdir(struct lwp *, void *, register_t *);

int	sys_mknod(struct lwp *, void *, register_t *);

int	pecoff_sys_chmod(struct lwp *, void *, register_t *);

int	pecoff_sys_chown(struct lwp *, void *, register_t *);

int	sys_obreak(struct lwp *, void *, register_t *);

#ifdef COMPAT_20
int	compat_20_sys_getfsstat(struct lwp *, void *, register_t *);

#else
#endif
#ifdef COMPAT_43
int	sys_getpid_with_ppid(struct lwp *, void *, register_t *);

#else
int	sys_getpid(struct lwp *, void *, register_t *);

#endif
int	sys_mount(struct lwp *, void *, register_t *);

int	pecoff_sys_unmount(struct lwp *, void *, register_t *);

int	sys_setuid(struct lwp *, void *, register_t *);

#ifdef COMPAT_43
int	sys_getuid_with_euid(struct lwp *, void *, register_t *);

#else
int	sys_getuid(struct lwp *, void *, register_t *);

#endif
int	sys_geteuid(struct lwp *, void *, register_t *);

int	sys_ptrace(struct lwp *, void *, register_t *);

int	sys_recvmsg(struct lwp *, void *, register_t *);

int	sys_sendmsg(struct lwp *, void *, register_t *);

int	sys_recvfrom(struct lwp *, void *, register_t *);

int	sys_accept(struct lwp *, void *, register_t *);

int	sys_getpeername(struct lwp *, void *, register_t *);

int	sys_getsockname(struct lwp *, void *, register_t *);

int	pecoff_sys_access(struct lwp *, void *, register_t *);

int	pecoff_sys_chflags(struct lwp *, void *, register_t *);

int	sys_fchflags(struct lwp *, void *, register_t *);

int	sys_sync(struct lwp *, void *, register_t *);

int	sys_kill(struct lwp *, void *, register_t *);

int	sys_getppid(struct lwp *, void *, register_t *);

int	sys_dup(struct lwp *, void *, register_t *);

int	sys_pipe(struct lwp *, void *, register_t *);

int	sys_getegid(struct lwp *, void *, register_t *);

int	sys_profil(struct lwp *, void *, register_t *);

#if defined(KTRACE) || !defined(_KERNEL)
int	sys_ktrace(struct lwp *, void *, register_t *);

#else
#endif
#ifdef COMPAT_43
int	sys_getgid_with_egid(struct lwp *, void *, register_t *);

#else
int	sys_getgid(struct lwp *, void *, register_t *);

#endif
int	sys___getlogin(struct lwp *, void *, register_t *);

int	sys___setlogin(struct lwp *, void *, register_t *);

int	sys_acct(struct lwp *, void *, register_t *);

int	sys_ioctl(struct lwp *, void *, register_t *);

int	pecoff_sys_revoke(struct lwp *, void *, register_t *);

int	pecoff_sys_symlink(struct lwp *, void *, register_t *);

int	pecoff_sys_readlink(struct lwp *, void *, register_t *);

int	pecoff_sys_execve(struct lwp *, void *, register_t *);

int	sys_umask(struct lwp *, void *, register_t *);

int	pecoff_sys_chroot(struct lwp *, void *, register_t *);

int	sys_vfork(struct lwp *, void *, register_t *);

int	sys_sbrk(struct lwp *, void *, register_t *);

int	sys_sstk(struct lwp *, void *, register_t *);

int	sys_ovadvise(struct lwp *, void *, register_t *);

int	sys_munmap(struct lwp *, void *, register_t *);

int	sys_mprotect(struct lwp *, void *, register_t *);

int	sys_madvise(struct lwp *, void *, register_t *);

int	sys_mincore(struct lwp *, void *, register_t *);

int	sys_getgroups(struct lwp *, void *, register_t *);

int	sys_setgroups(struct lwp *, void *, register_t *);

int	sys_getpgrp(struct lwp *, void *, register_t *);

int	sys_setpgid(struct lwp *, void *, register_t *);

int	sys_setitimer(struct lwp *, void *, register_t *);

int	sys_getitimer(struct lwp *, void *, register_t *);

int	sys_dup2(struct lwp *, void *, register_t *);

int	sys_fcntl(struct lwp *, void *, register_t *);

int	sys_select(struct lwp *, void *, register_t *);

int	sys_fsync(struct lwp *, void *, register_t *);

int	sys_setpriority(struct lwp *, void *, register_t *);

int	sys_socket(struct lwp *, void *, register_t *);

int	sys_connect(struct lwp *, void *, register_t *);

int	sys_getpriority(struct lwp *, void *, register_t *);

int	sys_bind(struct lwp *, void *, register_t *);

int	sys_setsockopt(struct lwp *, void *, register_t *);

int	sys_listen(struct lwp *, void *, register_t *);

int	sys_gettimeofday(struct lwp *, void *, register_t *);

int	sys_getrusage(struct lwp *, void *, register_t *);

int	sys_getsockopt(struct lwp *, void *, register_t *);

int	sys_readv(struct lwp *, void *, register_t *);

int	sys_writev(struct lwp *, void *, register_t *);

int	sys_settimeofday(struct lwp *, void *, register_t *);

int	sys_fchown(struct lwp *, void *, register_t *);

int	sys_fchmod(struct lwp *, void *, register_t *);

int	sys_setreuid(struct lwp *, void *, register_t *);

int	sys_setregid(struct lwp *, void *, register_t *);

int	pecoff_sys_rename(struct lwp *, void *, register_t *);

int	sys_flock(struct lwp *, void *, register_t *);

int	sys_mkfifo(struct lwp *, void *, register_t *);

int	sys_sendto(struct lwp *, void *, register_t *);

int	sys_shutdown(struct lwp *, void *, register_t *);

int	sys_socketpair(struct lwp *, void *, register_t *);

int	sys_mkdir(struct lwp *, void *, register_t *);

int	pecoff_sys_rmdir(struct lwp *, void *, register_t *);

int	pecoff_sys_utimes(struct lwp *, void *, register_t *);

int	sys_adjtime(struct lwp *, void *, register_t *);

int	sys_setsid(struct lwp *, void *, register_t *);

int	sys_quotactl(struct lwp *, void *, register_t *);

#if defined(NFS) || defined(NFSSERVER) || !defined(_KERNEL)
int	sys_nfssvc(struct lwp *, void *, register_t *);

#else
#endif
int	pecoff_sys_statfs(struct lwp *, void *, register_t *);

#ifdef COMPAT_20
int	compat_20_sys_fstatfs(struct lwp *, void *, register_t *);

#else
#endif
int	pecoff_sys_getfh(struct lwp *, void *, register_t *);

int	sys_sysarch(struct lwp *, void *, register_t *);

#if (defined(SYSVSEM) || !defined(_KERNEL)) && !defined(_LP64)
#else
#endif
#if (defined(SYSVMSG) || !defined(_KERNEL)) && !defined(_LP64)
#else
#endif
#if (defined(SYSVSHM) || !defined(_KERNEL)) && !defined(_LP64)
#else
#endif
int	sys_pread(struct lwp *, void *, register_t *);

int	sys_pwrite(struct lwp *, void *, register_t *);

int	sys_ntp_gettime(struct lwp *, void *, register_t *);

#if defined(NTP) || !defined(_KERNEL)
int	sys_ntp_adjtime(struct lwp *, void *, register_t *);

#else
#endif
int	sys_setgid(struct lwp *, void *, register_t *);

int	sys_setegid(struct lwp *, void *, register_t *);

int	sys_seteuid(struct lwp *, void *, register_t *);

#if defined(LFS) || !defined(_KERNEL)
int	sys_lfs_bmapv(struct lwp *, void *, register_t *);

int	sys_lfs_markv(struct lwp *, void *, register_t *);

int	sys_lfs_segclean(struct lwp *, void *, register_t *);

int	sys_lfs_segwait(struct lwp *, void *, register_t *);

#else
#endif
int	pecoff_sys_pathconf(struct lwp *, void *, register_t *);

int	sys_fpathconf(struct lwp *, void *, register_t *);

int	sys_getrlimit(struct lwp *, void *, register_t *);

int	sys_setrlimit(struct lwp *, void *, register_t *);

int	sys_mmap(struct lwp *, void *, register_t *);

int	sys_lseek(struct lwp *, void *, register_t *);

int	pecoff_sys_truncate(struct lwp *, void *, register_t *);

int	sys_ftruncate(struct lwp *, void *, register_t *);

int	sys___sysctl(struct lwp *, void *, register_t *);

int	sys_mlock(struct lwp *, void *, register_t *);

int	sys_munlock(struct lwp *, void *, register_t *);

int	pecoff_sys_undelete(struct lwp *, void *, register_t *);

int	sys_futimes(struct lwp *, void *, register_t *);

int	sys_getpgid(struct lwp *, void *, register_t *);

int	sys_reboot(struct lwp *, void *, register_t *);

int	sys_poll(struct lwp *, void *, register_t *);

#if defined(LKM) || !defined(_KERNEL)
int	sys_lkmnosys(struct lwp *, void *, register_t *);

#else	/* !LKM */
#endif	/* !LKM */
#if defined(SYSVSEM) || !defined(_KERNEL)
int	sys_semget(struct lwp *, void *, register_t *);

int	sys_semop(struct lwp *, void *, register_t *);

int	sys_semconfig(struct lwp *, void *, register_t *);

#else
#endif
#if defined(SYSVMSG) || !defined(_KERNEL)
int	sys_msgget(struct lwp *, void *, register_t *);

int	sys_msgsnd(struct lwp *, void *, register_t *);

int	sys_msgrcv(struct lwp *, void *, register_t *);

#else
#endif
#if defined(SYSVSHM) || !defined(_KERNEL)
int	sys_shmat(struct lwp *, void *, register_t *);

int	sys_shmdt(struct lwp *, void *, register_t *);

int	sys_shmget(struct lwp *, void *, register_t *);

#else
#endif
int	sys_clock_gettime(struct lwp *, void *, register_t *);

int	sys_clock_settime(struct lwp *, void *, register_t *);

int	sys_clock_getres(struct lwp *, void *, register_t *);

int	sys_timer_create(struct lwp *, void *, register_t *);

int	sys_timer_delete(struct lwp *, void *, register_t *);

int	sys_timer_settime(struct lwp *, void *, register_t *);

int	sys_timer_gettime(struct lwp *, void *, register_t *);

int	sys_timer_getoverrun(struct lwp *, void *, register_t *);

int	sys_nanosleep(struct lwp *, void *, register_t *);

int	sys_fdatasync(struct lwp *, void *, register_t *);

int	sys_mlockall(struct lwp *, void *, register_t *);

int	sys_munlockall(struct lwp *, void *, register_t *);

int	sys___sigtimedwait(struct lwp *, void *, register_t *);

#if defined(P1003_1B_SEMAPHORE) || !defined(_KERNEL)
int	sys__ksem_init(struct lwp *, void *, register_t *);

int	sys__ksem_open(struct lwp *, void *, register_t *);

int	sys__ksem_unlink(struct lwp *, void *, register_t *);

int	sys__ksem_close(struct lwp *, void *, register_t *);

int	sys__ksem_post(struct lwp *, void *, register_t *);

int	sys__ksem_wait(struct lwp *, void *, register_t *);

int	sys__ksem_trywait(struct lwp *, void *, register_t *);

int	sys__ksem_getvalue(struct lwp *, void *, register_t *);

int	sys__ksem_destroy(struct lwp *, void *, register_t *);

#else
#endif
int	pecoff_sys___posix_rename(struct lwp *, void *, register_t *);

int	sys_swapctl(struct lwp *, void *, register_t *);

int	sys_getdents(struct lwp *, void *, register_t *);

int	sys_minherit(struct lwp *, void *, register_t *);

int	pecoff_sys_lchmod(struct lwp *, void *, register_t *);

int	pecoff_sys_lchown(struct lwp *, void *, register_t *);

int	pecoff_sys_lutimes(struct lwp *, void *, register_t *);

int	sys___msync13(struct lwp *, void *, register_t *);

int	pecoff_sys___stat13(struct lwp *, void *, register_t *);

int	sys___fstat13(struct lwp *, void *, register_t *);

int	pecoff_sys___lstat13(struct lwp *, void *, register_t *);

int	sys___sigaltstack14(struct lwp *, void *, register_t *);

int	sys___vfork14(struct lwp *, void *, register_t *);

int	pecoff_sys___posix_chown(struct lwp *, void *, register_t *);

int	sys___posix_fchown(struct lwp *, void *, register_t *);

int	pecoff_sys___posix_lchown(struct lwp *, void *, register_t *);

int	sys_getsid(struct lwp *, void *, register_t *);

int	sys___clone(struct lwp *, void *, register_t *);

#if defined(KTRACE) || !defined(_KERNEL)
int	sys_fktrace(struct lwp *, void *, register_t *);

#else
#endif
int	sys_preadv(struct lwp *, void *, register_t *);

int	sys_pwritev(struct lwp *, void *, register_t *);

#ifdef COMPAT_16
int	compat_16_sys___sigaction14(struct lwp *, void *, register_t *);

#else
#endif
int	sys___sigpending14(struct lwp *, void *, register_t *);

int	sys___sigprocmask14(struct lwp *, void *, register_t *);

int	sys___sigsuspend14(struct lwp *, void *, register_t *);

int	compat_16_sys___sigreturn14(struct lwp *, void *, register_t *);

int	sys___getcwd(struct lwp *, void *, register_t *);

int	sys_fchroot(struct lwp *, void *, register_t *);

int	sys_fhopen(struct lwp *, void *, register_t *);

int	sys_fhstat(struct lwp *, void *, register_t *);

#ifdef COMPAT_20
int	compat_20_sys_fhstatfs(struct lwp *, void *, register_t *);

#else
#endif
#if defined(SYSVSEM) || !defined(_KERNEL)
int	sys_____semctl13(struct lwp *, void *, register_t *);

#else
#endif
#if defined(SYSVMSG) || !defined(_KERNEL)
int	sys___msgctl13(struct lwp *, void *, register_t *);

#else
#endif
#if defined(SYSVSHM) || !defined(_KERNEL)
int	sys___shmctl13(struct lwp *, void *, register_t *);

#else
#endif
int	pecoff_sys_lchflags(struct lwp *, void *, register_t *);

int	sys_issetugid(struct lwp *, void *, register_t *);

int	sys_utrace(struct lwp *, void *, register_t *);

int	sys_getcontext(struct lwp *, void *, register_t *);

int	sys_setcontext(struct lwp *, void *, register_t *);

int	sys__lwp_create(struct lwp *, void *, register_t *);

int	sys__lwp_exit(struct lwp *, void *, register_t *);

int	sys__lwp_self(struct lwp *, void *, register_t *);

int	sys__lwp_wait(struct lwp *, void *, register_t *);

int	sys__lwp_suspend(struct lwp *, void *, register_t *);

int	sys__lwp_continue(struct lwp *, void *, register_t *);

int	sys__lwp_wakeup(struct lwp *, void *, register_t *);

int	sys__lwp_getprivate(struct lwp *, void *, register_t *);

int	sys__lwp_setprivate(struct lwp *, void *, register_t *);

int	sys_sa_register(struct lwp *, void *, register_t *);

int	sys_sa_stacks(struct lwp *, void *, register_t *);

int	sys_sa_enable(struct lwp *, void *, register_t *);

int	sys_sa_setconcurrency(struct lwp *, void *, register_t *);

int	sys_sa_yield(struct lwp *, void *, register_t *);

int	sys_sa_preempt(struct lwp *, void *, register_t *);

int	sys___sigaction_sigtramp(struct lwp *, void *, register_t *);

int	sys_pmc_get_info(struct lwp *, void *, register_t *);

int	sys_pmc_control(struct lwp *, void *, register_t *);

int	sys_rasctl(struct lwp *, void *, register_t *);

int	sys_kqueue(struct lwp *, void *, register_t *);

int	sys_kevent(struct lwp *, void *, register_t *);

int	sys_fsync_range(struct lwp *, void *, register_t *);

int	sys_uuidgen(struct lwp *, void *, register_t *);

int	sys_getvfsstat(struct lwp *, void *, register_t *);

int	sys_statvfs1(struct lwp *, void *, register_t *);

int	sys_fstatvfs1(struct lwp *, void *, register_t *);

int	sys_fhstatvfs1(struct lwp *, void *, register_t *);

#endif /* _PECOFF_SYS__SYSCALLARGS_H_ */

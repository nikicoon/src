/* $NetBSD: freebsd_syscallargs.h,v 1.48 2002/05/03 00:25:30 eeh Exp $ */

/*
 * System call argument lists.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.37 2002/05/03 00:20:57 eeh Exp 
 */

#ifndef _FREEBSD_SYS__SYSCALLARGS_H_
#define	_FREEBSD_SYS__SYSCALLARGS_H_

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

struct freebsd_sys_open_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
	syscallarg(int) mode;
};

struct compat_43_freebsd_sys_creat_args {
	syscallarg(const char *) path;
	syscallarg(int) mode;
};

struct freebsd_sys_link_args {
	syscallarg(const char *) path;
	syscallarg(const char *) link;
};

struct freebsd_sys_unlink_args {
	syscallarg(const char *) path;
};

struct freebsd_sys_chdir_args {
	syscallarg(const char *) path;
};

struct freebsd_sys_mknod_args {
	syscallarg(const char *) path;
	syscallarg(int) mode;
	syscallarg(int) dev;
};

struct freebsd_sys_chmod_args {
	syscallarg(const char *) path;
	syscallarg(int) mode;
};

struct freebsd_sys_chown_args {
	syscallarg(const char *) path;
	syscallarg(int) uid;
	syscallarg(int) gid;
};

struct freebsd_sys_mount_args {
	syscallarg(int) type;
	syscallarg(const char *) path;
	syscallarg(int) flags;
	syscallarg(caddr_t) data;
};

struct freebsd_sys_unmount_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
};

struct freebsd_sys_ptrace_args {
	syscallarg(int) req;
	syscallarg(pid_t) pid;
	syscallarg(caddr_t) addr;
	syscallarg(int) data;
};

struct freebsd_sys_access_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
};

struct freebsd_sys_chflags_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
};

struct compat_43_freebsd_sys_stat_args {
	syscallarg(const char *) path;
	syscallarg(struct stat43 *) ub;
};

struct compat_43_freebsd_sys_lstat_args {
	syscallarg(const char *) path;
	syscallarg(struct stat43 *) ub;
};

struct freebsd_sys_ioctl_args {
	syscallarg(int) fd;
	syscallarg(u_long) com;
	syscallarg(caddr_t) data;
};

struct freebsd_sys_revoke_args {
	syscallarg(const char *) path;
};

struct freebsd_sys_symlink_args {
	syscallarg(const char *) path;
	syscallarg(const char *) link;
};

struct freebsd_sys_readlink_args {
	syscallarg(const char *) path;
	syscallarg(char *) buf;
	syscallarg(int) count;
};

struct freebsd_sys_execve_args {
	syscallarg(const char *) path;
	syscallarg(char **) argp;
	syscallarg(char **) envp;
};

struct freebsd_sys_chroot_args {
	syscallarg(const char *) path;
};

struct freebsd_sys_msync_args {
	syscallarg(caddr_t) addr;
	syscallarg(size_t) len;
	syscallarg(int) flags;
};

struct freebsd_sys_sigreturn_args {
	syscallarg(struct freebsd_sigcontext *) scp;
};

struct freebsd_sys_rename_args {
	syscallarg(const char *) from;
	syscallarg(const char *) to;
};

struct compat_43_freebsd_sys_truncate_args {
	syscallarg(const char *) path;
	syscallarg(long) length;
};

struct freebsd_sys_mkfifo_args {
	syscallarg(const char *) path;
	syscallarg(int) mode;
};

struct freebsd_sys_mkdir_args {
	syscallarg(const char *) path;
	syscallarg(int) mode;
};

struct freebsd_sys_rmdir_args {
	syscallarg(const char *) path;
};

struct freebsd_sys_statfs_args {
	syscallarg(const char *) path;
	syscallarg(struct statfs *) buf;
};

struct freebsd_sys_getfh_args {
	syscallarg(const char *) fname;
	syscallarg(fhandle_t *) fhp;
};

struct freebsd_sys_rtprio_args {
	syscallarg(int) function;
	syscallarg(pid_t) pid;
	syscallarg(struct freebsd_rtprio *) rtp;
};

struct freebsd_sys_semsys_args {
	syscallarg(int) which;
	syscallarg(int) a2;
	syscallarg(int) a3;
	syscallarg(int) a4;
	syscallarg(int) a5;
};

struct freebsd_sys_msgsys_args {
	syscallarg(int) which;
	syscallarg(int) a2;
	syscallarg(int) a3;
	syscallarg(int) a4;
	syscallarg(int) a5;
	syscallarg(int) a6;
};

struct freebsd_sys_shmsys_args {
	syscallarg(int) which;
	syscallarg(int) a2;
	syscallarg(int) a3;
	syscallarg(int) a4;
};

struct freebsd_ntp_adjtime_args {
	syscallarg(struct freebsd_timex *) tp;
};

struct freebsd_sys_stat_args {
	syscallarg(const char *) path;
	syscallarg(struct stat12 *) ub;
};

struct freebsd_sys_lstat_args {
	syscallarg(const char *) path;
	syscallarg(struct stat12 *) ub;
};

struct freebsd_sys_pathconf_args {
	syscallarg(const char *) path;
	syscallarg(int) name;
};

struct freebsd_sys_truncate_args {
	syscallarg(const char *) path;
	syscallarg(int) pad;
	syscallarg(off_t) length;
};

struct freebsd_sys_undelete_args {
	syscallarg(char *) path;
};

struct freebsd_sys_lchown_args {
	syscallarg(const char *) path;
	syscallarg(int) uid;
	syscallarg(int) gid;
};

struct freebsd_sys_utrace_args {
	syscallarg(void *) addr;
	syscallarg(size_t) len;
};

struct freebsd_sys_sigaction4_args {
	syscallarg(int) signum;
	syscallarg(const struct freebsd_sigaction4 *) nsa;
	syscallarg(struct freebsd_sigaction4 *) osa;
};

/*
 * System call prototypes.
 */

int	sys_nosys(struct proc *, void *, register_t *);
int	sys_exit(struct proc *, void *, register_t *);
int	sys_fork(struct proc *, void *, register_t *);
int	sys_read(struct proc *, void *, register_t *);
int	sys_write(struct proc *, void *, register_t *);
int	freebsd_sys_open(struct proc *, void *, register_t *);
int	sys_close(struct proc *, void *, register_t *);
int	sys_wait4(struct proc *, void *, register_t *);
int	compat_43_freebsd_sys_creat(struct proc *, void *, register_t *);
int	freebsd_sys_link(struct proc *, void *, register_t *);
int	freebsd_sys_unlink(struct proc *, void *, register_t *);
int	freebsd_sys_chdir(struct proc *, void *, register_t *);
int	sys_fchdir(struct proc *, void *, register_t *);
int	freebsd_sys_mknod(struct proc *, void *, register_t *);
int	freebsd_sys_chmod(struct proc *, void *, register_t *);
int	freebsd_sys_chown(struct proc *, void *, register_t *);
int	sys_obreak(struct proc *, void *, register_t *);
int	sys_getfsstat(struct proc *, void *, register_t *);
int	compat_43_sys_lseek(struct proc *, void *, register_t *);
int	sys_getpid_with_ppid(struct proc *, void *, register_t *);
int	freebsd_sys_mount(struct proc *, void *, register_t *);
int	freebsd_sys_unmount(struct proc *, void *, register_t *);
int	sys_setuid(struct proc *, void *, register_t *);
int	sys_getuid_with_euid(struct proc *, void *, register_t *);
int	sys_geteuid(struct proc *, void *, register_t *);
int	freebsd_sys_ptrace(struct proc *, void *, register_t *);
int	sys_recvmsg(struct proc *, void *, register_t *);
int	sys_sendmsg(struct proc *, void *, register_t *);
int	sys_recvfrom(struct proc *, void *, register_t *);
int	sys_accept(struct proc *, void *, register_t *);
int	sys_getpeername(struct proc *, void *, register_t *);
int	sys_getsockname(struct proc *, void *, register_t *);
int	freebsd_sys_access(struct proc *, void *, register_t *);
int	freebsd_sys_chflags(struct proc *, void *, register_t *);
int	sys_fchflags(struct proc *, void *, register_t *);
int	sys_sync(struct proc *, void *, register_t *);
int	sys_kill(struct proc *, void *, register_t *);
int	compat_43_freebsd_sys_stat(struct proc *, void *, register_t *);
int	sys_getppid(struct proc *, void *, register_t *);
int	compat_43_freebsd_sys_lstat(struct proc *, void *, register_t *);
int	sys_dup(struct proc *, void *, register_t *);
int	sys_pipe(struct proc *, void *, register_t *);
int	sys_getegid(struct proc *, void *, register_t *);
int	sys_profil(struct proc *, void *, register_t *);
#ifdef KTRACE
int	sys_ktrace(struct proc *, void *, register_t *);
#else
#endif
int	compat_13_sys_sigaction(struct proc *, void *, register_t *);
int	sys_getgid_with_egid(struct proc *, void *, register_t *);
int	compat_13_sys_sigprocmask(struct proc *, void *, register_t *);
int	sys___getlogin(struct proc *, void *, register_t *);
int	sys_setlogin(struct proc *, void *, register_t *);
int	sys_acct(struct proc *, void *, register_t *);
int	compat_13_sys_sigpending(struct proc *, void *, register_t *);
int	compat_13_sys_sigaltstack(struct proc *, void *, register_t *);
int	freebsd_sys_ioctl(struct proc *, void *, register_t *);
int	sys_reboot(struct proc *, void *, register_t *);
int	freebsd_sys_revoke(struct proc *, void *, register_t *);
int	freebsd_sys_symlink(struct proc *, void *, register_t *);
int	freebsd_sys_readlink(struct proc *, void *, register_t *);
int	freebsd_sys_execve(struct proc *, void *, register_t *);
int	sys_umask(struct proc *, void *, register_t *);
int	freebsd_sys_chroot(struct proc *, void *, register_t *);
int	compat_43_sys_fstat(struct proc *, void *, register_t *);
int	compat_43_sys_getkerninfo(struct proc *, void *, register_t *);
int	compat_43_sys_getpagesize(struct proc *, void *, register_t *);
int	freebsd_sys_msync(struct proc *, void *, register_t *);
int	sys_vfork(struct proc *, void *, register_t *);
int	sys_sbrk(struct proc *, void *, register_t *);
int	sys_sstk(struct proc *, void *, register_t *);
int	compat_43_sys_mmap(struct proc *, void *, register_t *);
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
int	compat_43_sys_wait(struct proc *, void *, register_t *);
int	compat_12_sys_swapon(struct proc *, void *, register_t *);
int	sys_getitimer(struct proc *, void *, register_t *);
int	compat_43_sys_gethostname(struct proc *, void *, register_t *);
int	compat_43_sys_sethostname(struct proc *, void *, register_t *);
int	compat_43_sys_getdtablesize(struct proc *, void *, register_t *);
int	sys_dup2(struct proc *, void *, register_t *);
int	sys_fcntl(struct proc *, void *, register_t *);
int	sys_select(struct proc *, void *, register_t *);
int	sys_fsync(struct proc *, void *, register_t *);
int	sys_setpriority(struct proc *, void *, register_t *);
int	sys_socket(struct proc *, void *, register_t *);
int	sys_connect(struct proc *, void *, register_t *);
int	compat_43_sys_accept(struct proc *, void *, register_t *);
int	sys_getpriority(struct proc *, void *, register_t *);
int	compat_43_sys_send(struct proc *, void *, register_t *);
int	compat_43_sys_recv(struct proc *, void *, register_t *);
int	freebsd_sys_sigreturn(struct proc *, void *, register_t *);
int	sys_bind(struct proc *, void *, register_t *);
int	sys_setsockopt(struct proc *, void *, register_t *);
int	sys_listen(struct proc *, void *, register_t *);
int	compat_43_sys_sigvec(struct proc *, void *, register_t *);
int	compat_43_sys_sigblock(struct proc *, void *, register_t *);
int	compat_43_sys_sigsetmask(struct proc *, void *, register_t *);
int	compat_13_sys_sigsuspend(struct proc *, void *, register_t *);
int	compat_43_sys_sigstack(struct proc *, void *, register_t *);
int	compat_43_sys_recvmsg(struct proc *, void *, register_t *);
int	compat_43_sys_sendmsg(struct proc *, void *, register_t *);
#ifdef TRACE
int	sys_vtrace(struct proc *, void *, register_t *);
#else
#endif
int	sys_gettimeofday(struct proc *, void *, register_t *);
int	sys_getrusage(struct proc *, void *, register_t *);
int	sys_getsockopt(struct proc *, void *, register_t *);
int	sys_readv(struct proc *, void *, register_t *);
int	sys_writev(struct proc *, void *, register_t *);
int	sys_settimeofday(struct proc *, void *, register_t *);
int	sys_fchown(struct proc *, void *, register_t *);
int	sys_fchmod(struct proc *, void *, register_t *);
int	compat_43_sys_recvfrom(struct proc *, void *, register_t *);
int	sys_setreuid(struct proc *, void *, register_t *);
int	sys_setregid(struct proc *, void *, register_t *);
int	freebsd_sys_rename(struct proc *, void *, register_t *);
int	compat_43_freebsd_sys_truncate(struct proc *, void *, register_t *);
int	compat_43_sys_ftruncate(struct proc *, void *, register_t *);
int	sys_flock(struct proc *, void *, register_t *);
int	freebsd_sys_mkfifo(struct proc *, void *, register_t *);
int	sys_sendto(struct proc *, void *, register_t *);
int	sys_shutdown(struct proc *, void *, register_t *);
int	sys_socketpair(struct proc *, void *, register_t *);
int	freebsd_sys_mkdir(struct proc *, void *, register_t *);
int	freebsd_sys_rmdir(struct proc *, void *, register_t *);
int	sys_utimes(struct proc *, void *, register_t *);
int	sys_adjtime(struct proc *, void *, register_t *);
int	compat_43_sys_getpeername(struct proc *, void *, register_t *);
int	compat_43_sys_gethostid(struct proc *, void *, register_t *);
int	compat_43_sys_sethostid(struct proc *, void *, register_t *);
int	compat_43_sys_getrlimit(struct proc *, void *, register_t *);
int	compat_43_sys_setrlimit(struct proc *, void *, register_t *);
int	compat_43_sys_killpg(struct proc *, void *, register_t *);
int	sys_setsid(struct proc *, void *, register_t *);
int	sys_quotactl(struct proc *, void *, register_t *);
int	compat_43_sys_quota(struct proc *, void *, register_t *);
int	compat_43_sys_getsockname(struct proc *, void *, register_t *);
#if defined(NFS) || defined(NFSSERVER)
int	sys_nfssvc(struct proc *, void *, register_t *);
#else
#endif
int	compat_43_sys_getdirentries(struct proc *, void *, register_t *);
int	freebsd_sys_statfs(struct proc *, void *, register_t *);
int	sys_fstatfs(struct proc *, void *, register_t *);
#ifdef NFS
int	freebsd_sys_getfh(struct proc *, void *, register_t *);
#else
#endif
int	compat_09_sys_getdomainname(struct proc *, void *, register_t *);
int	compat_09_sys_setdomainname(struct proc *, void *, register_t *);
int	compat_09_sys_uname(struct proc *, void *, register_t *);
int	sys_sysarch(struct proc *, void *, register_t *);
int	freebsd_sys_rtprio(struct proc *, void *, register_t *);
#if defined(SYSVSEM) && !defined(_LP64)
int	freebsd_sys_semsys(struct proc *, void *, register_t *);
#else
#endif
#if defined(SYSVMSG) && !defined(_LP64)
int	freebsd_sys_msgsys(struct proc *, void *, register_t *);
#else
#endif
#if defined(SYSVSHM) && !defined(_LP64)
int	freebsd_sys_shmsys(struct proc *, void *, register_t *);
#else
#endif
int	sys_pread(struct proc *, void *, register_t *);
int	sys_pwrite(struct proc *, void *, register_t *);
#ifdef NTP
int	freebsd_ntp_adjtime(struct proc *, void *, register_t *);
#else
#endif
int	sys_setgid(struct proc *, void *, register_t *);
int	sys_setegid(struct proc *, void *, register_t *);
int	sys_seteuid(struct proc *, void *, register_t *);
#ifdef LFS
int	sys_lfs_bmapv(struct proc *, void *, register_t *);
int	sys_lfs_markv(struct proc *, void *, register_t *);
int	sys_lfs_segclean(struct proc *, void *, register_t *);
int	sys_lfs_segwait(struct proc *, void *, register_t *);
#else
#endif
int	freebsd_sys_stat(struct proc *, void *, register_t *);
int	compat_12_sys_fstat(struct proc *, void *, register_t *);
int	freebsd_sys_lstat(struct proc *, void *, register_t *);
int	freebsd_sys_pathconf(struct proc *, void *, register_t *);
int	sys_fpathconf(struct proc *, void *, register_t *);
int	sys_getrlimit(struct proc *, void *, register_t *);
int	sys_setrlimit(struct proc *, void *, register_t *);
int	compat_12_sys_getdirentries(struct proc *, void *, register_t *);
int	sys_mmap(struct proc *, void *, register_t *);
int	sys_nosys(struct proc *, void *, register_t *);
int	sys_lseek(struct proc *, void *, register_t *);
int	freebsd_sys_truncate(struct proc *, void *, register_t *);
int	sys_ftruncate(struct proc *, void *, register_t *);
int	sys___sysctl(struct proc *, void *, register_t *);
int	sys_mlock(struct proc *, void *, register_t *);
int	sys_munlock(struct proc *, void *, register_t *);
#ifdef FREEBSD_BASED_ON_44LITE_R2
int	freebsd_sys_undelete(struct proc *, void *, register_t *);
#else
#endif
int	sys_futimes(struct proc *, void *, register_t *);
int	sys_getpgid(struct proc *, void *, register_t *);
#if 0
int	sys_reboot(struct proc *, void *, register_t *);
#else
#endif
int	sys_poll(struct proc *, void *, register_t *);
#ifdef SYSVSEM
int	compat_14_sys___semctl(struct proc *, void *, register_t *);
int	sys_semget(struct proc *, void *, register_t *);
int	sys_semop(struct proc *, void *, register_t *);
int	sys_semconfig(struct proc *, void *, register_t *);
#else
#endif
#ifdef SYSVMSG
int	compat_14_sys_msgctl(struct proc *, void *, register_t *);
int	sys_msgget(struct proc *, void *, register_t *);
int	sys_msgsnd(struct proc *, void *, register_t *);
int	sys_msgrcv(struct proc *, void *, register_t *);
#else
#endif
#ifdef SYSVSHM
int	sys_shmat(struct proc *, void *, register_t *);
int	compat_14_sys_shmctl(struct proc *, void *, register_t *);
int	sys_shmdt(struct proc *, void *, register_t *);
int	sys_shmget(struct proc *, void *, register_t *);
#else
#endif
int	sys_clock_gettime(struct proc *, void *, register_t *);
int	sys_clock_settime(struct proc *, void *, register_t *);
int	sys_clock_getres(struct proc *, void *, register_t *);
int	sys_nanosleep(struct proc *, void *, register_t *);
int	sys_minherit(struct proc *, void *, register_t *);
int	sys_issetugid(struct proc *, void *, register_t *);
int	freebsd_sys_lchown(struct proc *, void *, register_t *);
int	sys_getdents(struct proc *, void *, register_t *);
int	sys_lchmod(struct proc *, void *, register_t *);
int	sys_lchown(struct proc *, void *, register_t *);
int	sys_lutimes(struct proc *, void *, register_t *);
int	sys___msync13(struct proc *, void *, register_t *);
int	sys___stat13(struct proc *, void *, register_t *);
int	sys___fstat13(struct proc *, void *, register_t *);
int	sys___lstat13(struct proc *, void *, register_t *);
int	sys_fhstatfs(struct proc *, void *, register_t *);
int	sys_fhopen(struct proc *, void *, register_t *);
int	sys_fhstat(struct proc *, void *, register_t *);
int	sys_getsid(struct proc *, void *, register_t *);
int	sys_mlockall(struct proc *, void *, register_t *);
int	sys_munlockall(struct proc *, void *, register_t *);
int	sys___getcwd(struct proc *, void *, register_t *);
int	freebsd_sys_utrace(struct proc *, void *, register_t *);
int	sys___sigprocmask14(struct proc *, void *, register_t *);
int	sys___sigsuspend14(struct proc *, void *, register_t *);
int	freebsd_sys_sigaction4(struct proc *, void *, register_t *);
int	sys___sigpending14(struct proc *, void *, register_t *);
#endif /* _FREEBSD_SYS__SYSCALLARGS_H_ */

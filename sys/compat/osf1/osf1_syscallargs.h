/* $NetBSD: osf1_syscallargs.h,v 1.53 2005/02/26 23:58:20 perry Exp $ */

/*
 * System call argument lists.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.41 2005/02/26 23:10:21 perry Exp
 */

#ifndef _OSF1_SYS__SYSCALLARGS_H_
#define	_OSF1_SYS__SYSCALLARGS_H_

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

struct osf1_sys_wait4_args {
	syscallarg(int) pid;
	syscallarg(int *) status;
	syscallarg(int) options;
	syscallarg(struct osf1_rusage *) rusage;
};

struct osf1_sys_mknod_args {
	syscallarg(const char *) path;
	syscallarg(int) mode;
	syscallarg(int) dev;
};

struct osf1_sys_getfsstat_args {
	syscallarg(struct osf1_statfs *) buf;
	syscallarg(long) bufsize;
	syscallarg(int) flags;
};

struct osf1_sys_lseek_args {
	syscallarg(int) fd;
	syscallarg(off_t) offset;
	syscallarg(int) whence;
};

struct osf1_sys_mount_args {
	syscallarg(int) type;
	syscallarg(const char *) path;
	syscallarg(int) flags;
	syscallarg(caddr_t) data;
};

struct osf1_sys_unmount_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
};

struct osf1_sys_setuid_args {
	syscallarg(uid_t) uid;
};

struct osf1_sys_recvmsg_xopen_args {
	syscallarg(int) s;
	syscallarg(struct osf1_msghdr_xopen *) msg;
	syscallarg(int) flags;
};

struct osf1_sys_sendmsg_xopen_args {
	syscallarg(int) s;
	syscallarg(const struct osf1_msghdr_xopen *) msg;
	syscallarg(int) flags;
};

struct osf1_sys_access_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
};

struct osf1_sys_set_program_attributes_args {
	syscallarg(caddr_t) taddr;
	syscallarg(unsigned long) tsize;
	syscallarg(caddr_t) daddr;
	syscallarg(unsigned long) dsize;
};

struct osf1_sys_open_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
	syscallarg(int) mode;
};

struct osf1_sys_classcntl_args {
	syscallarg(int) opcode;
	syscallarg(int) arg1;
	syscallarg(int) arg2;
	syscallarg(int) arg3;
};

struct osf1_sys_ioctl_args {
	syscallarg(int) fd;
	syscallarg(int) com;
	syscallarg(caddr_t) data;
};

struct osf1_sys_reboot_args {
	syscallarg(int) opt;
};

struct osf1_sys_execve_args {
	syscallarg(const char *) path;
	syscallarg(char *const *) argp;
	syscallarg(char *const *) envp;
};

struct osf1_sys_stat_args {
	syscallarg(const char *) path;
	syscallarg(struct osf1_stat *) ub;
};

struct osf1_sys_lstat_args {
	syscallarg(const char *) path;
	syscallarg(struct osf1_stat *) ub;
};

struct osf1_sys_mmap_args {
	syscallarg(caddr_t) addr;
	syscallarg(size_t) len;
	syscallarg(int) prot;
	syscallarg(int) flags;
	syscallarg(int) fd;
	syscallarg(off_t) pos;
};

struct osf1_sys_mprotect_args {
	syscallarg(void *) addr;
	syscallarg(size_t) len;
	syscallarg(int) prot;
};

struct osf1_sys_madvise_args {
	syscallarg(void *) addr;
	syscallarg(size_t) len;
	syscallarg(int) behav;
};

struct osf1_sys_setitimer_args {
	syscallarg(u_int) which;
	syscallarg(struct osf1_itimerval *) itv;
	syscallarg(struct osf1_itimerval *) oitv;
};

struct osf1_sys_getitimer_args {
	syscallarg(u_int) which;
	syscallarg(struct osf1_itimerval *) itv;
};

struct osf1_sys_fstat_args {
	syscallarg(int) fd;
	syscallarg(void *) sb;
};

struct osf1_sys_fcntl_args {
	syscallarg(int) fd;
	syscallarg(int) cmd;
	syscallarg(void *) arg;
};

struct osf1_sys_select_args {
	syscallarg(u_int) nd;
	syscallarg(fd_set *) in;
	syscallarg(fd_set *) ou;
	syscallarg(fd_set *) ex;
	syscallarg(struct osf1_timeval *) tv;
};

struct osf1_sys_socket_args {
	syscallarg(int) domain;
	syscallarg(int) type;
	syscallarg(int) protocol;
};

struct osf1_sys_gettimeofday_args {
	syscallarg(struct osf1_timeval *) tp;
	syscallarg(struct osf1_timezone *) tzp;
};

struct osf1_sys_getrusage_args {
	syscallarg(int) who;
	syscallarg(struct osf1_rusage *) rusage;
};

struct osf1_sys_readv_args {
	syscallarg(int) fd;
	syscallarg(struct osf1_iovec *) iovp;
	syscallarg(u_int) iovcnt;
};

struct osf1_sys_writev_args {
	syscallarg(int) fd;
	syscallarg(struct osf1_iovec *) iovp;
	syscallarg(u_int) iovcnt;
};

struct osf1_sys_settimeofday_args {
	syscallarg(struct osf1_timeval *) tv;
	syscallarg(struct osf1_timezone *) tzp;
};

struct osf1_sys_truncate_args {
	syscallarg(const char *) path;
	syscallarg(off_t) length;
};

struct osf1_sys_ftruncate_args {
	syscallarg(int) fd;
	syscallarg(off_t) length;
};

struct osf1_sys_setgid_args {
	syscallarg(gid_t) gid;
};

struct osf1_sys_sendto_args {
	syscallarg(int) s;
	syscallarg(caddr_t) buf;
	syscallarg(size_t) len;
	syscallarg(int) flags;
	syscallarg(struct sockaddr *) to;
	syscallarg(int) tolen;
};

struct osf1_sys_socketpair_args {
	syscallarg(int) domain;
	syscallarg(int) type;
	syscallarg(int) protocol;
	syscallarg(int *) rsv;
};

struct osf1_sys_utimes_args {
	syscallarg(const char *) path;
	syscallarg(const struct osf1_timeval *) tptr;
};

struct osf1_sys_getrlimit_args {
	syscallarg(u_int) which;
	syscallarg(struct rlimit *) rlp;
};

struct osf1_sys_setrlimit_args {
	syscallarg(u_int) which;
	syscallarg(struct rlimit *) rlp;
};

struct osf1_sys_sigaction_args {
	syscallarg(int) signum;
	syscallarg(struct osf1_sigaction *) nsa;
	syscallarg(struct osf1_sigaction *) osa;
};

struct osf1_sys_statfs_args {
	syscallarg(const char *) path;
	syscallarg(struct osf1_statfs *) buf;
	syscallarg(int) len;
};

struct osf1_sys_fstatfs_args {
	syscallarg(int) fd;
	syscallarg(struct osf1_statfs *) buf;
	syscallarg(int) len;
};

struct osf1_sys_uname_args {
	syscallarg(struct osf1_uname *) name;
};

struct osf1_sys_shmat_args {
	syscallarg(int) shmid;
	syscallarg(const void *) shmaddr;
	syscallarg(int) shmflg;
};

struct osf1_sys_shmctl_args {
	syscallarg(int) shmid;
	syscallarg(int) cmd;
	syscallarg(struct osf1_shmid_ds *) buf;
};

struct osf1_sys_shmdt_args {
	syscallarg(const void *) shmaddr;
};

struct osf1_sys_shmget_args {
	syscallarg(osf1_key_t) key;
	syscallarg(size_t) size;
	syscallarg(int) flags;
};

struct osf1_sys_stat2_args {
	syscallarg(const char *) path;
	syscallarg(struct osf1_stat2 *) ub;
};

struct osf1_sys_lstat2_args {
	syscallarg(const char *) path;
	syscallarg(struct osf1_stat2 *) ub;
};

struct osf1_sys_fstat2_args {
	syscallarg(int) fd;
	syscallarg(struct osf1_stat2 *) sb;
};

struct osf1_sys_sigaltstack_args {
	syscallarg(struct osf1_sigaltstack *) nss;
	syscallarg(struct osf1_sigaltstack *) oss;
};

struct osf1_sys_sysinfo_args {
	syscallarg(int) cmd;
	syscallarg(char *) buf;
	syscallarg(long) len;
};

struct osf1_sys_pathconf_args {
	syscallarg(const char *) path;
	syscallarg(int) name;
};

struct osf1_sys_fpathconf_args {
	syscallarg(int) fd;
	syscallarg(int) name;
};

struct osf1_sys_usleep_thread_args {
	syscallarg(struct osf1_timeval *) sleep;
	syscallarg(struct osf1_timeval *) slept;
};

struct osf1_sys_getsysinfo_args {
	syscallarg(u_long) op;
	syscallarg(caddr_t) buffer;
	syscallarg(u_long) nbytes;
	syscallarg(caddr_t) arg;
	syscallarg(u_long) flag;
};

struct osf1_sys_setsysinfo_args {
	syscallarg(u_long) op;
	syscallarg(caddr_t) buffer;
	syscallarg(u_long) nbytes;
	syscallarg(caddr_t) arg;
	syscallarg(u_long) flag;
};

/*
 * System call prototypes.
 */

int	sys_nosys(struct lwp *, void *, register_t *);

int	sys_exit(struct lwp *, void *, register_t *);

int	sys_fork(struct lwp *, void *, register_t *);

int	sys_read(struct lwp *, void *, register_t *);

int	sys_write(struct lwp *, void *, register_t *);

int	sys_close(struct lwp *, void *, register_t *);

int	osf1_sys_wait4(struct lwp *, void *, register_t *);

int	sys_link(struct lwp *, void *, register_t *);

int	sys_unlink(struct lwp *, void *, register_t *);

int	sys_chdir(struct lwp *, void *, register_t *);

int	sys_fchdir(struct lwp *, void *, register_t *);

int	osf1_sys_mknod(struct lwp *, void *, register_t *);

int	sys_chmod(struct lwp *, void *, register_t *);

int	sys___posix_chown(struct lwp *, void *, register_t *);

int	sys_obreak(struct lwp *, void *, register_t *);

int	osf1_sys_getfsstat(struct lwp *, void *, register_t *);

int	osf1_sys_lseek(struct lwp *, void *, register_t *);

int	sys_getpid_with_ppid(struct lwp *, void *, register_t *);

int	osf1_sys_mount(struct lwp *, void *, register_t *);

int	osf1_sys_unmount(struct lwp *, void *, register_t *);

int	osf1_sys_setuid(struct lwp *, void *, register_t *);

int	sys_getuid_with_euid(struct lwp *, void *, register_t *);

int	osf1_sys_recvmsg_xopen(struct lwp *, void *, register_t *);

int	osf1_sys_sendmsg_xopen(struct lwp *, void *, register_t *);

int	osf1_sys_access(struct lwp *, void *, register_t *);

int	sys_sync(struct lwp *, void *, register_t *);

int	sys_kill(struct lwp *, void *, register_t *);

int	sys_setpgid(struct lwp *, void *, register_t *);

int	sys_dup(struct lwp *, void *, register_t *);

int	sys_pipe(struct lwp *, void *, register_t *);

int	osf1_sys_set_program_attributes(struct lwp *, void *, register_t *);

int	osf1_sys_open(struct lwp *, void *, register_t *);

int	sys_getgid_with_egid(struct lwp *, void *, register_t *);

int	compat_13_sys_sigprocmask(struct lwp *, void *, register_t *);

int	sys___getlogin(struct lwp *, void *, register_t *);

int	sys___setlogin(struct lwp *, void *, register_t *);

int	sys_acct(struct lwp *, void *, register_t *);

int	osf1_sys_classcntl(struct lwp *, void *, register_t *);

int	osf1_sys_ioctl(struct lwp *, void *, register_t *);

int	osf1_sys_reboot(struct lwp *, void *, register_t *);

int	sys_revoke(struct lwp *, void *, register_t *);

int	sys_symlink(struct lwp *, void *, register_t *);

int	sys_readlink(struct lwp *, void *, register_t *);

int	osf1_sys_execve(struct lwp *, void *, register_t *);

int	sys_umask(struct lwp *, void *, register_t *);

int	sys_chroot(struct lwp *, void *, register_t *);

int	sys_getpgrp(struct lwp *, void *, register_t *);

int	compat_43_sys_getpagesize(struct lwp *, void *, register_t *);

int	sys_vfork(struct lwp *, void *, register_t *);

int	osf1_sys_stat(struct lwp *, void *, register_t *);

int	osf1_sys_lstat(struct lwp *, void *, register_t *);

int	osf1_sys_mmap(struct lwp *, void *, register_t *);

int	sys_munmap(struct lwp *, void *, register_t *);

int	osf1_sys_mprotect(struct lwp *, void *, register_t *);

int	osf1_sys_madvise(struct lwp *, void *, register_t *);

int	sys_getgroups(struct lwp *, void *, register_t *);

int	sys_setgroups(struct lwp *, void *, register_t *);

int	osf1_sys_setitimer(struct lwp *, void *, register_t *);

int	osf1_sys_getitimer(struct lwp *, void *, register_t *);

int	compat_43_sys_gethostname(struct lwp *, void *, register_t *);

int	compat_43_sys_sethostname(struct lwp *, void *, register_t *);

int	compat_43_sys_getdtablesize(struct lwp *, void *, register_t *);

int	sys_dup2(struct lwp *, void *, register_t *);

int	osf1_sys_fstat(struct lwp *, void *, register_t *);

int	osf1_sys_fcntl(struct lwp *, void *, register_t *);

int	osf1_sys_select(struct lwp *, void *, register_t *);

int	sys_poll(struct lwp *, void *, register_t *);

int	sys_fsync(struct lwp *, void *, register_t *);

int	sys_setpriority(struct lwp *, void *, register_t *);

int	osf1_sys_socket(struct lwp *, void *, register_t *);

int	sys_connect(struct lwp *, void *, register_t *);

int	compat_43_sys_accept(struct lwp *, void *, register_t *);

int	sys_getpriority(struct lwp *, void *, register_t *);

int	compat_43_sys_send(struct lwp *, void *, register_t *);

int	compat_43_sys_recv(struct lwp *, void *, register_t *);

int	compat_13_sys_sigreturn(struct lwp *, void *, register_t *);

int	sys_bind(struct lwp *, void *, register_t *);

int	sys_setsockopt(struct lwp *, void *, register_t *);

int	sys_listen(struct lwp *, void *, register_t *);

int	compat_13_sys_sigsuspend(struct lwp *, void *, register_t *);

int	compat_43_sys_sigstack(struct lwp *, void *, register_t *);

int	osf1_sys_gettimeofday(struct lwp *, void *, register_t *);

int	osf1_sys_getrusage(struct lwp *, void *, register_t *);

int	sys_getsockopt(struct lwp *, void *, register_t *);

int	osf1_sys_readv(struct lwp *, void *, register_t *);

int	osf1_sys_writev(struct lwp *, void *, register_t *);

int	osf1_sys_settimeofday(struct lwp *, void *, register_t *);

int	sys___posix_fchown(struct lwp *, void *, register_t *);

int	sys_fchmod(struct lwp *, void *, register_t *);

int	compat_43_sys_recvfrom(struct lwp *, void *, register_t *);

int	sys___posix_rename(struct lwp *, void *, register_t *);

int	osf1_sys_truncate(struct lwp *, void *, register_t *);

int	osf1_sys_ftruncate(struct lwp *, void *, register_t *);

int	sys_flock(struct lwp *, void *, register_t *);

int	osf1_sys_setgid(struct lwp *, void *, register_t *);

int	osf1_sys_sendto(struct lwp *, void *, register_t *);

int	sys_shutdown(struct lwp *, void *, register_t *);

int	osf1_sys_socketpair(struct lwp *, void *, register_t *);

int	sys_mkdir(struct lwp *, void *, register_t *);

int	sys_rmdir(struct lwp *, void *, register_t *);

int	osf1_sys_utimes(struct lwp *, void *, register_t *);

int	compat_43_sys_getpeername(struct lwp *, void *, register_t *);

int	compat_43_sys_gethostid(struct lwp *, void *, register_t *);

int	compat_43_sys_sethostid(struct lwp *, void *, register_t *);

int	osf1_sys_getrlimit(struct lwp *, void *, register_t *);

int	osf1_sys_setrlimit(struct lwp *, void *, register_t *);

int	sys_setsid(struct lwp *, void *, register_t *);

int	compat_43_sys_quota(struct lwp *, void *, register_t *);

int	compat_43_sys_getsockname(struct lwp *, void *, register_t *);

int	osf1_sys_sigaction(struct lwp *, void *, register_t *);

int	compat_43_sys_getdirentries(struct lwp *, void *, register_t *);

int	osf1_sys_statfs(struct lwp *, void *, register_t *);

int	osf1_sys_fstatfs(struct lwp *, void *, register_t *);

int	compat_09_sys_getdomainname(struct lwp *, void *, register_t *);

int	compat_09_sys_setdomainname(struct lwp *, void *, register_t *);

int	osf1_sys_uname(struct lwp *, void *, register_t *);

int	sys___posix_lchown(struct lwp *, void *, register_t *);

int	osf1_sys_shmat(struct lwp *, void *, register_t *);

int	osf1_sys_shmctl(struct lwp *, void *, register_t *);

int	osf1_sys_shmdt(struct lwp *, void *, register_t *);

int	osf1_sys_shmget(struct lwp *, void *, register_t *);

int	osf1_sys_stat2(struct lwp *, void *, register_t *);

int	osf1_sys_lstat2(struct lwp *, void *, register_t *);

int	osf1_sys_fstat2(struct lwp *, void *, register_t *);

int	sys_getsid(struct lwp *, void *, register_t *);

int	osf1_sys_sigaltstack(struct lwp *, void *, register_t *);

int	osf1_sys_sysinfo(struct lwp *, void *, register_t *);

int	osf1_sys_pathconf(struct lwp *, void *, register_t *);

int	osf1_sys_fpathconf(struct lwp *, void *, register_t *);

int	osf1_sys_usleep_thread(struct lwp *, void *, register_t *);

int	osf1_sys_getsysinfo(struct lwp *, void *, register_t *);

int	osf1_sys_setsysinfo(struct lwp *, void *, register_t *);

#endif /* _OSF1_SYS__SYSCALLARGS_H_ */

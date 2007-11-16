/* $NetBSD: linux_syscallargs.h,v 1.62 2007/11/16 12:53:56 njoly Exp $ */

/*
 * System call argument lists.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.58 2007/11/16 12:51:54 njoly Exp
 */

#ifndef _LINUX_SYS_SYSCALLARGS_H_
#define	_LINUX_SYS_SYSCALLARGS_H_

#define	LINUX_SYS_MAXSYSARGS	8

#undef	syscallarg
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

#undef check_syscall_args
#define check_syscall_args(call) \
	typedef char call##_check_args[sizeof (struct call##_args) \
		<= LINUX_SYS_MAXSYSARGS * sizeof (register_t) ? 1 : -1];

struct osf1_sys_wait4_args {
	syscallarg(int) pid;
	syscallarg(int *) status;
	syscallarg(int) options;
	syscallarg(struct osf1_rusage *) rusage;
};
check_syscall_args(osf1_sys_wait4)

struct linux_sys_creat_args {
	syscallarg(const char *) path;
	syscallarg(mode_t) mode;
};
check_syscall_args(linux_sys_creat)

struct linux_sys_unlink_args {
	syscallarg(const char *) path;
};
check_syscall_args(linux_sys_unlink)

struct linux_sys_mknod_args {
	syscallarg(const char *) path;
	syscallarg(int) mode;
	syscallarg(int) dev;
};
check_syscall_args(linux_sys_mknod)

struct linux_sys_brk_args {
	syscallarg(char *) nsize;
};
check_syscall_args(linux_sys_brk)

struct osf1_sys_mount_args {
	syscallarg(int) type;
	syscallarg(const char *) path;
	syscallarg(int) flags;
	syscallarg(void *) data;
};
check_syscall_args(osf1_sys_mount)

struct linux_sys_ptrace_args {
	syscallarg(long) request;
	syscallarg(long) pid;
	syscallarg(long) addr;
	syscallarg(long) data;
};
check_syscall_args(linux_sys_ptrace)

struct linux_sys_kill_args {
	syscallarg(int) pid;
	syscallarg(int) signum;
};
check_syscall_args(linux_sys_kill)

struct osf1_sys_set_program_attributes_args {
	syscallarg(void *) taddr;
	syscallarg(unsigned long) tsize;
	syscallarg(void *) daddr;
	syscallarg(unsigned long) dsize;
};
check_syscall_args(osf1_sys_set_program_attributes)

struct linux_sys_open_args {
	syscallarg(const char *) path;
	syscallarg(int) flags;
	syscallarg(int) mode;
};
check_syscall_args(linux_sys_open)

struct linux_sys_sigpending_args {
	syscallarg(linux_old_sigset_t *) set;
};
check_syscall_args(linux_sys_sigpending)

struct linux_sys_ioctl_args {
	syscallarg(int) fd;
	syscallarg(u_long) com;
	syscallarg(void *) data;
};
check_syscall_args(linux_sys_ioctl)

struct linux_sys_stat_args {
	syscallarg(const char *) path;
	syscallarg(struct linux_stat *) sp;
};
check_syscall_args(linux_sys_stat)

struct linux_sys_lstat_args {
	syscallarg(const char *) path;
	syscallarg(struct linux_stat *) sp;
};
check_syscall_args(linux_sys_lstat)

struct linux_sys_mprotect_args {
	syscallarg(const void *) start;
	syscallarg(unsigned long) len;
	syscallarg(int) prot;
};
check_syscall_args(linux_sys_mprotect)

struct osf1_sys_setitimer_args {
	syscallarg(u_int) which;
	syscallarg(struct osf1_itimerval *) itv;
	syscallarg(struct osf1_itimerval *) oitv;
};
check_syscall_args(osf1_sys_setitimer)

struct linux_sys_fstat_args {
	syscallarg(int) fd;
	syscallarg(struct linux_stat *) sp;
};
check_syscall_args(linux_sys_fstat)

struct linux_sys_fcntl_args {
	syscallarg(int) fd;
	syscallarg(int) cmd;
	syscallarg(void *) arg;
};
check_syscall_args(linux_sys_fcntl)

struct osf1_sys_select_args {
	syscallarg(u_int) nd;
	syscallarg(fd_set *) in;
	syscallarg(fd_set *) ou;
	syscallarg(fd_set *) ex;
	syscallarg(struct osf1_timeval *) tv;
};
check_syscall_args(osf1_sys_select)

struct linux_sys_socket_args {
	syscallarg(int) domain;
	syscallarg(int) type;
	syscallarg(int) protocol;
};
check_syscall_args(linux_sys_socket)

struct linux_sys_connect_args {
	syscallarg(int) s;
	syscallarg(const struct osockaddr *) name;
	syscallarg(unsigned int) namelen;
};
check_syscall_args(linux_sys_connect)

struct linux_sys_accept_args {
	syscallarg(int) s;
	syscallarg(struct osockaddr *) name;
	syscallarg(int *) anamelen;
};
check_syscall_args(linux_sys_accept)

struct linux_sys_getpriority_args {
	syscallarg(int) which;
	syscallarg(int) who;
};
check_syscall_args(linux_sys_getpriority)

struct linux_sys_send_args {
	syscallarg(int) s;
	syscallarg(void *) buf;
	syscallarg(int) len;
	syscallarg(int) flags;
};
check_syscall_args(linux_sys_send)

struct linux_sys_recv_args {
	syscallarg(int) s;
	syscallarg(void *) buf;
	syscallarg(int) len;
	syscallarg(int) flags;
};
check_syscall_args(linux_sys_recv)

struct linux_sys_sigreturn_args {
	syscallarg(struct linux_sigframe *) sfp;
};
check_syscall_args(linux_sys_sigreturn)

struct linux_sys_bind_args {
	syscallarg(int) s;
	syscallarg(const struct osockaddr *) name;
	syscallarg(unsigned int) namelen;
};
check_syscall_args(linux_sys_bind)

struct linux_sys_setsockopt_args {
	syscallarg(int) s;
	syscallarg(int) level;
	syscallarg(int) optname;
	syscallarg(void *) optval;
	syscallarg(int) optlen;
};
check_syscall_args(linux_sys_setsockopt)

struct linux_sys_sigsuspend_args {
	syscallarg(void *) restart;
	syscallarg(int) oldmask;
	syscallarg(int) mask;
};
check_syscall_args(linux_sys_sigsuspend)

struct linux_sys_recvmsg_args {
	syscallarg(int) s;
	syscallarg(struct msghdr *) msg;
	syscallarg(int) flags;
};
check_syscall_args(linux_sys_recvmsg)

struct linux_sys_sendmsg_args {
	syscallarg(int) s;
	syscallarg(const struct msghdr *) msg;
	syscallarg(int) flags;
};
check_syscall_args(linux_sys_sendmsg)

struct osf1_sys_gettimeofday_args {
	syscallarg(struct osf1_timeval *) tv;
	syscallarg(struct osf1_timezone *) tzp;
};
check_syscall_args(osf1_sys_gettimeofday)

struct osf1_sys_getrusage_args {
	syscallarg(int) who;
	syscallarg(struct osf1_rusage *) rusage;
};
check_syscall_args(osf1_sys_getrusage)

struct linux_sys_getsockopt_args {
	syscallarg(int) s;
	syscallarg(int) level;
	syscallarg(int) optname;
	syscallarg(void *) optval;
	syscallarg(int *) optlen;
};
check_syscall_args(linux_sys_getsockopt)

struct osf1_sys_settimeofday_args {
	syscallarg(struct osf1_timeval *) tv;
	syscallarg(struct osf1_timezone *) tzp;
};
check_syscall_args(osf1_sys_settimeofday)

struct linux_sys_recvfrom_args {
	syscallarg(int) s;
	syscallarg(void *) buf;
	syscallarg(int) len;
	syscallarg(int) flags;
	syscallarg(struct osockaddr *) from;
	syscallarg(int *) fromlenaddr;
};
check_syscall_args(linux_sys_recvfrom)

struct linux_sys_sendto_args {
	syscallarg(int) s;
	syscallarg(void *) msg;
	syscallarg(int) len;
	syscallarg(int) flags;
	syscallarg(struct osockaddr *) to;
	syscallarg(int) tolen;
};
check_syscall_args(linux_sys_sendto)

struct linux_sys_socketpair_args {
	syscallarg(int) domain;
	syscallarg(int) type;
	syscallarg(int) protocol;
	syscallarg(int *) rsv;
};
check_syscall_args(linux_sys_socketpair)

struct osf1_sys_utimes_args {
	syscallarg(const char *) path;
	syscallarg(const struct osf1_timeval *) tptr;
};
check_syscall_args(osf1_sys_utimes)

struct linux_sys_getpeername_args {
	syscallarg(int) fdes;
	syscallarg(void *) asa;
	syscallarg(int *) alen;
};
check_syscall_args(linux_sys_getpeername)

struct linux_sys_getrlimit_args {
	syscallarg(int) which;
	syscallarg(struct orlimit *) rlp;
};
check_syscall_args(linux_sys_getrlimit)

struct linux_sys_setrlimit_args {
	syscallarg(int) which;
	syscallarg(const struct orlimit *) rlp;
};
check_syscall_args(linux_sys_setrlimit)

struct linux_sys_getsockname_args {
	syscallarg(int) fdec;
	syscallarg(void *) asa;
	syscallarg(int *) alen;
};
check_syscall_args(linux_sys_getsockname)

struct linux_sys_sigaction_args {
	syscallarg(int) signum;
	syscallarg(const struct linux_old_sigaction *) nsa;
	syscallarg(struct linux_old_sigaction *) osa;
};
check_syscall_args(linux_sys_sigaction)

struct osf1_sys_statfs_args {
	syscallarg(const char *) path;
	syscallarg(struct osf1_statfs *) buf;
	syscallarg(int) len;
};
check_syscall_args(osf1_sys_statfs)

struct osf1_sys_fstatfs_args {
	syscallarg(int) fd;
	syscallarg(struct osf1_statfs *) buf;
	syscallarg(int) len;
};
check_syscall_args(osf1_sys_fstatfs)

struct linux_sys_setdomainname_args {
	syscallarg(char *) domainname;
	syscallarg(int) len;
};
check_syscall_args(linux_sys_setdomainname)

struct linux_sys_ugetrlimit_args {
	syscallarg(int) which;
	syscallarg(struct rlimit *) rlp;
};
check_syscall_args(linux_sys_ugetrlimit)
#ifdef SYSVMSG
#else
#endif
#ifdef SYSVSEM
#else
#endif

struct linux_sys_olduname_args {
	syscallarg(struct linux_old_utsname *) up;
};
check_syscall_args(linux_sys_olduname)
#ifdef SYSVSHM

struct linux_sys_shmget_args {
	syscallarg(key_t) key;
	syscallarg(size_t) size;
	syscallarg(int) shmflg;
};
check_syscall_args(linux_sys_shmget)
#else
#endif

struct linux_sys_msync_args {
	syscallarg(void *) addr;
	syscallarg(int) len;
	syscallarg(int) fl;
};
check_syscall_args(linux_sys_msync)

struct linux_sys_getpgid_args {
	syscallarg(int) pid;
};
check_syscall_args(linux_sys_getpgid)

struct osf1_sys_sysinfo_args {
	syscallarg(int) cmd;
	syscallarg(char) buf;
	syscallarg(long) len;
};
check_syscall_args(osf1_sys_sysinfo)

struct osf1_sys_usleep_thread_args {
	syscallarg(struct osf1_timeval *) sleep;
	syscallarg(struct osf1_timeval *) slept;
};
check_syscall_args(osf1_sys_usleep_thread)

struct osf1_sys_getsysinfo_args {
	syscallarg(u_long) op;
	syscallarg(void *) buffer;
	syscallarg(u_long) nbytes;
	syscallarg(void *) arg;
	syscallarg(u_long) flag;
};
check_syscall_args(osf1_sys_getsysinfo)

struct osf1_sys_setsysinfo_args {
	syscallarg(u_long) op;
	syscallarg(void *) buffer;
	syscallarg(u_long) nbytes;
	syscallarg(void *) arg;
	syscallarg(u_long) flag;
};
check_syscall_args(osf1_sys_setsysinfo)

struct linux_sys_fdatasync_args {
	syscallarg(int) fd;
};
check_syscall_args(linux_sys_fdatasync)

struct linux_sys_swapoff_args {
	syscallarg(const char *) path;
};
check_syscall_args(linux_sys_swapoff)

struct linux_sys_getdents_args {
	syscallarg(int) fd;
	syscallarg(struct linux_dirent *) dent;
	syscallarg(unsigned int) count;
};
check_syscall_args(linux_sys_getdents)

struct linux_sys_reboot_args {
	syscallarg(int) magic1;
	syscallarg(int) magic2;
	syscallarg(int) cmd;
	syscallarg(void *) arg;
};
check_syscall_args(linux_sys_reboot)

struct linux_sys_clone_args {
	syscallarg(int) flags;
	syscallarg(void *) stack;
};
check_syscall_args(linux_sys_clone)
#ifdef EXEC_AOUT

struct linux_sys_uselib_args {
	syscallarg(const char *) path;
};
check_syscall_args(linux_sys_uselib)
#else
#endif

struct linux_sys_sysinfo_args {
	syscallarg(struct linux_sysinfo *) arg;
};
check_syscall_args(linux_sys_sysinfo)

struct linux_sys___sysctl_args {
	syscallarg(struct linux___sysctl *) lsp;
};
check_syscall_args(linux_sys___sysctl)

struct linux_sys_swapon_args {
	syscallarg(const char *) name;
};
check_syscall_args(linux_sys_swapon)

struct linux_sys_times_args {
	syscallarg(struct times *) tms;
};
check_syscall_args(linux_sys_times)

struct linux_sys_personality_args {
	syscallarg(int) per;
};
check_syscall_args(linux_sys_personality)

struct linux_sys_setfsuid_args {
	syscallarg(uid_t) uid;
};
check_syscall_args(linux_sys_setfsuid)

struct linux_sys_statfs_args {
	syscallarg(const char *) path;
	syscallarg(struct linux_statfs *) sp;
};
check_syscall_args(linux_sys_statfs)

struct linux_sys_fstatfs_args {
	syscallarg(int) fd;
	syscallarg(struct linux_statfs *) sp;
};
check_syscall_args(linux_sys_fstatfs)

struct linux_sys_sched_setparam_args {
	syscallarg(pid_t) pid;
	syscallarg(const struct linux_sched_param *) sp;
};
check_syscall_args(linux_sys_sched_setparam)

struct linux_sys_sched_getparam_args {
	syscallarg(pid_t) pid;
	syscallarg(struct linux_sched_param *) sp;
};
check_syscall_args(linux_sys_sched_getparam)

struct linux_sys_sched_setscheduler_args {
	syscallarg(pid_t) pid;
	syscallarg(int) policy;
	syscallarg(const struct linux_sched_param *) sp;
};
check_syscall_args(linux_sys_sched_setscheduler)

struct linux_sys_sched_getscheduler_args {
	syscallarg(pid_t) pid;
};
check_syscall_args(linux_sys_sched_getscheduler)

struct linux_sys_sched_get_priority_max_args {
	syscallarg(int) policy;
};
check_syscall_args(linux_sys_sched_get_priority_max)

struct linux_sys_sched_get_priority_min_args {
	syscallarg(int) policy;
};
check_syscall_args(linux_sys_sched_get_priority_min)

struct linux_sys_uname_args {
	syscallarg(struct linux_utsname *) up;
};
check_syscall_args(linux_sys_uname)

struct linux_sys_mremap_args {
	syscallarg(void *) old_address;
	syscallarg(size_t) old_size;
	syscallarg(size_t) new_size;
	syscallarg(u_long) flags;
};
check_syscall_args(linux_sys_mremap)

struct linux_sys_setresuid_args {
	syscallarg(uid_t) ruid;
	syscallarg(uid_t) euid;
	syscallarg(uid_t) suid;
};
check_syscall_args(linux_sys_setresuid)

struct linux_sys_getresuid_args {
	syscallarg(uid_t *) ruid;
	syscallarg(uid_t *) euid;
	syscallarg(uid_t *) suid;
};
check_syscall_args(linux_sys_getresuid)

struct linux_sys_pread_args {
	syscallarg(int) fd;
	syscallarg(char *) buf;
	syscallarg(size_t) nbyte;
	syscallarg(linux_off_t) offset;
};
check_syscall_args(linux_sys_pread)

struct linux_sys_pwrite_args {
	syscallarg(int) fd;
	syscallarg(char *) buf;
	syscallarg(size_t) nbyte;
	syscallarg(linux_off_t) offset;
};
check_syscall_args(linux_sys_pwrite)

struct linux_sys_rt_sigreturn_args {
	syscallarg(struct linux_rt_sigframe *) sfp;
};
check_syscall_args(linux_sys_rt_sigreturn)

struct linux_sys_rt_sigaction_args {
	syscallarg(int) signum;
	syscallarg(const struct linux_sigaction *) nsa;
	syscallarg(struct linux_sigaction *) osa;
	syscallarg(size_t) sigsetsize;
};
check_syscall_args(linux_sys_rt_sigaction)

struct linux_sys_rt_sigprocmask_args {
	syscallarg(int) how;
	syscallarg(const linux_sigset_t *) set;
	syscallarg(linux_sigset_t *) oset;
	syscallarg(size_t) sigsetsize;
};
check_syscall_args(linux_sys_rt_sigprocmask)

struct linux_sys_rt_sigpending_args {
	syscallarg(linux_sigset_t *) set;
	syscallarg(size_t) sigsetsize;
};
check_syscall_args(linux_sys_rt_sigpending)

struct linux_sys_rt_queueinfo_args {
	syscallarg(int) pid;
	syscallarg(int) signum;
	syscallarg(linux_siginfo_t *) uinfo;
};
check_syscall_args(linux_sys_rt_queueinfo)

struct linux_sys_rt_sigsuspend_args {
	syscallarg(linux_sigset_t *) unewset;
	syscallarg(size_t) sigsetsize;
};
check_syscall_args(linux_sys_rt_sigsuspend)

struct linux_sys_select_args {
	syscallarg(int) nfds;
	syscallarg(fd_set *) readfds;
	syscallarg(fd_set *) writefds;
	syscallarg(fd_set *) exceptfds;
	syscallarg(struct timeval *) timeout;
};
check_syscall_args(linux_sys_select)

struct linux_sys_wait4_args {
	syscallarg(int) pid;
	syscallarg(int *) status;
	syscallarg(int) options;
	syscallarg(struct rusage *) rusage;
};
check_syscall_args(linux_sys_wait4)

struct linux_sys_getdents64_args {
	syscallarg(int) fd;
	syscallarg(struct linux_dirent64 *) dent;
	syscallarg(unsigned int) count;
};
check_syscall_args(linux_sys_getdents64)

struct linux_sys_setxattr_args {
	syscallarg(char *) path;
	syscallarg(char *) name;
	syscallarg(void *) value;
	syscallarg(size_t) size;
	syscallarg(int) flags;
};
check_syscall_args(linux_sys_setxattr)

struct linux_sys_lsetxattr_args {
	syscallarg(char *) path;
	syscallarg(char *) name;
	syscallarg(void *) value;
	syscallarg(size_t) size;
	syscallarg(int) flags;
};
check_syscall_args(linux_sys_lsetxattr)

struct linux_sys_fsetxattr_args {
	syscallarg(int) fd;
	syscallarg(char *) name;
	syscallarg(void *) value;
	syscallarg(size_t) size;
	syscallarg(int) flags;
};
check_syscall_args(linux_sys_fsetxattr)

struct linux_sys_getxattr_args {
	syscallarg(char *) path;
	syscallarg(char *) name;
	syscallarg(void *) value;
	syscallarg(size_t) size;
};
check_syscall_args(linux_sys_getxattr)

struct linux_sys_lgetxattr_args {
	syscallarg(char *) path;
	syscallarg(char *) name;
	syscallarg(void *) value;
	syscallarg(size_t) size;
};
check_syscall_args(linux_sys_lgetxattr)

struct linux_sys_fgetxattr_args {
	syscallarg(int) fd;
	syscallarg(char *) name;
	syscallarg(void *) value;
	syscallarg(size_t) size;
};
check_syscall_args(linux_sys_fgetxattr)

struct linux_sys_listxattr_args {
	syscallarg(char *) path;
	syscallarg(char *) list;
	syscallarg(size_t) size;
};
check_syscall_args(linux_sys_listxattr)

struct linux_sys_llistxattr_args {
	syscallarg(char *) path;
	syscallarg(char *) list;
	syscallarg(size_t) size;
};
check_syscall_args(linux_sys_llistxattr)

struct linux_sys_flistxattr_args {
	syscallarg(int) fd;
	syscallarg(char *) list;
	syscallarg(size_t) size;
};
check_syscall_args(linux_sys_flistxattr)

struct linux_sys_removexattr_args {
	syscallarg(char *) path;
	syscallarg(char *) name;
};
check_syscall_args(linux_sys_removexattr)

struct linux_sys_lremovexattr_args {
	syscallarg(char *) path;
	syscallarg(char *) name;
};
check_syscall_args(linux_sys_lremovexattr)

struct linux_sys_fremovexattr_args {
	syscallarg(int) fd;
	syscallarg(char *) name;
};
check_syscall_args(linux_sys_fremovexattr)

struct linux_sys_exit_group_args {
	syscallarg(int) error_code;
};
check_syscall_args(linux_sys_exit_group)

struct linux_sys_clock_settime_args {
	syscallarg(clockid_t) which;
	syscallarg(struct linux_timespec *) tp;
};
check_syscall_args(linux_sys_clock_settime)

struct linux_sys_clock_gettime_args {
	syscallarg(clockid_t) which;
	syscallarg(struct linux_timespec *) tp;
};
check_syscall_args(linux_sys_clock_gettime)

struct linux_sys_clock_getres_args {
	syscallarg(clockid_t) which;
	syscallarg(struct linux_timespec *) tp;
};
check_syscall_args(linux_sys_clock_getres)

struct linux_sys_clock_nanosleep_args {
	syscallarg(clockid_t) which;
	syscallarg(int) flags;
	syscallarg(struct linux_timespec *) rqtp;
	syscallarg(struct linux_timespec *) rmtp;
};
check_syscall_args(linux_sys_clock_nanosleep)

/*
 * System call prototypes.
 */

int	linux_sys_nosys(struct lwp *, void *, register_t *);

int	sys_exit(struct lwp *, void *, register_t *);

int	sys_fork(struct lwp *, void *, register_t *);

int	sys_read(struct lwp *, void *, register_t *);

int	sys_write(struct lwp *, void *, register_t *);

int	sys_close(struct lwp *, void *, register_t *);

int	osf1_sys_wait4(struct lwp *, void *, register_t *);

int	linux_sys_creat(struct lwp *, void *, register_t *);

int	sys_link(struct lwp *, void *, register_t *);

int	linux_sys_unlink(struct lwp *, void *, register_t *);

int	sys_chdir(struct lwp *, void *, register_t *);

int	sys_fchdir(struct lwp *, void *, register_t *);

int	linux_sys_mknod(struct lwp *, void *, register_t *);

int	sys_chmod(struct lwp *, void *, register_t *);

int	sys___posix_chown(struct lwp *, void *, register_t *);

int	linux_sys_brk(struct lwp *, void *, register_t *);

int	compat_43_sys_lseek(struct lwp *, void *, register_t *);

int	sys_getpid_with_ppid(struct lwp *, void *, register_t *);

int	osf1_sys_mount(struct lwp *, void *, register_t *);

int	sys_setuid(struct lwp *, void *, register_t *);

int	sys_getuid_with_euid(struct lwp *, void *, register_t *);

int	linux_sys_ptrace(struct lwp *, void *, register_t *);

int	sys_access(struct lwp *, void *, register_t *);

int	sys_sync(struct lwp *, void *, register_t *);

int	linux_sys_kill(struct lwp *, void *, register_t *);

int	sys_setpgid(struct lwp *, void *, register_t *);

int	sys_dup(struct lwp *, void *, register_t *);

int	linux_sys_pipe(struct lwp *, void *, register_t *);

int	osf1_sys_set_program_attributes(struct lwp *, void *, register_t *);

int	linux_sys_open(struct lwp *, void *, register_t *);

int	sys_getgid_with_egid(struct lwp *, void *, register_t *);

int	compat_13_sys_sigprocmask(struct lwp *, void *, register_t *);

int	sys_acct(struct lwp *, void *, register_t *);

int	linux_sys_sigpending(struct lwp *, void *, register_t *);

int	linux_sys_ioctl(struct lwp *, void *, register_t *);

int	sys_symlink(struct lwp *, void *, register_t *);

int	sys_readlink(struct lwp *, void *, register_t *);

int	sys_execve(struct lwp *, void *, register_t *);

int	sys_umask(struct lwp *, void *, register_t *);

int	sys_chroot(struct lwp *, void *, register_t *);

int	sys_getpgrp(struct lwp *, void *, register_t *);

int	compat_43_sys_getpagesize(struct lwp *, void *, register_t *);

int	sys___vfork14(struct lwp *, void *, register_t *);

int	linux_sys_stat(struct lwp *, void *, register_t *);

int	linux_sys_lstat(struct lwp *, void *, register_t *);

int	linux_sys_mmap(struct lwp *, void *, register_t *);

int	sys_munmap(struct lwp *, void *, register_t *);

int	linux_sys_mprotect(struct lwp *, void *, register_t *);

int	sys_madvise(struct lwp *, void *, register_t *);

int	sys_getgroups(struct lwp *, void *, register_t *);

int	sys_setgroups(struct lwp *, void *, register_t *);

int	osf1_sys_setitimer(struct lwp *, void *, register_t *);

int	compat_43_sys_gethostname(struct lwp *, void *, register_t *);

int	compat_43_sys_sethostname(struct lwp *, void *, register_t *);

int	sys_dup2(struct lwp *, void *, register_t *);

int	linux_sys_fstat(struct lwp *, void *, register_t *);

int	linux_sys_fcntl(struct lwp *, void *, register_t *);

int	osf1_sys_select(struct lwp *, void *, register_t *);

int	sys_poll(struct lwp *, void *, register_t *);

int	sys_fsync(struct lwp *, void *, register_t *);

int	sys_setpriority(struct lwp *, void *, register_t *);

int	linux_sys_socket(struct lwp *, void *, register_t *);

int	linux_sys_connect(struct lwp *, void *, register_t *);

int	linux_sys_accept(struct lwp *, void *, register_t *);

int	linux_sys_getpriority(struct lwp *, void *, register_t *);

int	linux_sys_send(struct lwp *, void *, register_t *);

int	linux_sys_recv(struct lwp *, void *, register_t *);

int	linux_sys_sigreturn(struct lwp *, void *, register_t *);

int	linux_sys_bind(struct lwp *, void *, register_t *);

int	linux_sys_setsockopt(struct lwp *, void *, register_t *);

int	sys_listen(struct lwp *, void *, register_t *);

int	linux_sys_sigsuspend(struct lwp *, void *, register_t *);

int	compat_43_sys_sigstack(struct lwp *, void *, register_t *);

int	linux_sys_recvmsg(struct lwp *, void *, register_t *);

int	linux_sys_sendmsg(struct lwp *, void *, register_t *);

int	osf1_sys_gettimeofday(struct lwp *, void *, register_t *);

int	osf1_sys_getrusage(struct lwp *, void *, register_t *);

int	linux_sys_getsockopt(struct lwp *, void *, register_t *);

int	sys_readv(struct lwp *, void *, register_t *);

int	sys_writev(struct lwp *, void *, register_t *);

int	osf1_sys_settimeofday(struct lwp *, void *, register_t *);

int	sys___posix_fchown(struct lwp *, void *, register_t *);

int	sys_fchmod(struct lwp *, void *, register_t *);

int	linux_sys_recvfrom(struct lwp *, void *, register_t *);

int	sys_setreuid(struct lwp *, void *, register_t *);

int	sys_setregid(struct lwp *, void *, register_t *);

int	sys___posix_rename(struct lwp *, void *, register_t *);

int	compat_43_sys_truncate(struct lwp *, void *, register_t *);

int	compat_43_sys_ftruncate(struct lwp *, void *, register_t *);

int	sys_flock(struct lwp *, void *, register_t *);

int	sys_setgid(struct lwp *, void *, register_t *);

int	linux_sys_sendto(struct lwp *, void *, register_t *);

int	sys_shutdown(struct lwp *, void *, register_t *);

int	linux_sys_socketpair(struct lwp *, void *, register_t *);

int	sys_mkdir(struct lwp *, void *, register_t *);

int	sys_rmdir(struct lwp *, void *, register_t *);

int	osf1_sys_utimes(struct lwp *, void *, register_t *);

int	linux_sys_getpeername(struct lwp *, void *, register_t *);

int	linux_sys_getrlimit(struct lwp *, void *, register_t *);

int	linux_sys_setrlimit(struct lwp *, void *, register_t *);

int	sys_setsid(struct lwp *, void *, register_t *);

int	linux_sys_getsockname(struct lwp *, void *, register_t *);

int	linux_sys_sigaction(struct lwp *, void *, register_t *);

int	compat_43_sys_getdirentries(struct lwp *, void *, register_t *);

int	osf1_sys_statfs(struct lwp *, void *, register_t *);

int	osf1_sys_fstatfs(struct lwp *, void *, register_t *);

int	compat_09_sys_getdomainname(struct lwp *, void *, register_t *);

int	linux_sys_setdomainname(struct lwp *, void *, register_t *);

int	linux_sys_ugetrlimit(struct lwp *, void *, register_t *);

#ifdef SYSVMSG
int	linux_sys_msgctl(struct lwp *, void *, register_t *);

int	sys_msgget(struct lwp *, void *, register_t *);

int	sys_msgrcv(struct lwp *, void *, register_t *);

int	sys_msgsnd(struct lwp *, void *, register_t *);

#else
#endif
#ifdef SYSVSEM
int	linux_sys_semctl(struct lwp *, void *, register_t *);

int	sys_semget(struct lwp *, void *, register_t *);

int	sys_semop(struct lwp *, void *, register_t *);

#else
#endif
int	linux_sys_olduname(struct lwp *, void *, register_t *);

int	sys___posix_lchown(struct lwp *, void *, register_t *);

#ifdef SYSVSHM
int	linux_sys_shmat(struct lwp *, void *, register_t *);

int	linux_sys_shmctl(struct lwp *, void *, register_t *);

int	sys_shmdt(struct lwp *, void *, register_t *);

int	linux_sys_shmget(struct lwp *, void *, register_t *);

#else
#endif
int	linux_sys_msync(struct lwp *, void *, register_t *);

int	linux_sys_getpgid(struct lwp *, void *, register_t *);

int	sys_getsid(struct lwp *, void *, register_t *);

int	osf1_sys_sysinfo(struct lwp *, void *, register_t *);

int	osf1_sys_usleep_thread(struct lwp *, void *, register_t *);

int	osf1_sys_getsysinfo(struct lwp *, void *, register_t *);

int	osf1_sys_setsysinfo(struct lwp *, void *, register_t *);

int	linux_sys_fdatasync(struct lwp *, void *, register_t *);

int	linux_sys_swapoff(struct lwp *, void *, register_t *);

int	linux_sys_getdents(struct lwp *, void *, register_t *);

int	linux_sys_reboot(struct lwp *, void *, register_t *);

int	linux_sys_clone(struct lwp *, void *, register_t *);

#ifdef EXEC_AOUT
int	linux_sys_uselib(struct lwp *, void *, register_t *);

#else
#endif
int	sys_mlock(struct lwp *, void *, register_t *);

int	sys_munlock(struct lwp *, void *, register_t *);

int	sys_mlockall(struct lwp *, void *, register_t *);

int	sys_munlockall(struct lwp *, void *, register_t *);

int	linux_sys_sysinfo(struct lwp *, void *, register_t *);

int	linux_sys___sysctl(struct lwp *, void *, register_t *);

int	linux_sys_swapon(struct lwp *, void *, register_t *);

int	linux_sys_times(struct lwp *, void *, register_t *);

int	linux_sys_personality(struct lwp *, void *, register_t *);

int	linux_sys_setfsuid(struct lwp *, void *, register_t *);

int	linux_sys_statfs(struct lwp *, void *, register_t *);

int	linux_sys_fstatfs(struct lwp *, void *, register_t *);

int	linux_sys_sched_setparam(struct lwp *, void *, register_t *);

int	linux_sys_sched_getparam(struct lwp *, void *, register_t *);

int	linux_sys_sched_setscheduler(struct lwp *, void *, register_t *);

int	linux_sys_sched_getscheduler(struct lwp *, void *, register_t *);

int	linux_sys_sched_yield(struct lwp *, void *, register_t *);

int	linux_sys_sched_get_priority_max(struct lwp *, void *, register_t *);

int	linux_sys_sched_get_priority_min(struct lwp *, void *, register_t *);

int	linux_sys_uname(struct lwp *, void *, register_t *);

int	sys_nanosleep(struct lwp *, void *, register_t *);

int	linux_sys_mremap(struct lwp *, void *, register_t *);

int	linux_sys_setresuid(struct lwp *, void *, register_t *);

int	linux_sys_getresuid(struct lwp *, void *, register_t *);

int	linux_sys_pread(struct lwp *, void *, register_t *);

int	linux_sys_pwrite(struct lwp *, void *, register_t *);

int	linux_sys_rt_sigreturn(struct lwp *, void *, register_t *);

int	linux_sys_rt_sigaction(struct lwp *, void *, register_t *);

int	linux_sys_rt_sigprocmask(struct lwp *, void *, register_t *);

int	linux_sys_rt_sigpending(struct lwp *, void *, register_t *);

int	linux_sys_rt_queueinfo(struct lwp *, void *, register_t *);

int	linux_sys_rt_sigsuspend(struct lwp *, void *, register_t *);

int	linux_sys_select(struct lwp *, void *, register_t *);

int	sys_gettimeofday(struct lwp *, void *, register_t *);

int	sys_settimeofday(struct lwp *, void *, register_t *);

int	sys_getitimer(struct lwp *, void *, register_t *);

int	sys_setitimer(struct lwp *, void *, register_t *);

int	sys_utimes(struct lwp *, void *, register_t *);

int	sys_getrusage(struct lwp *, void *, register_t *);

int	linux_sys_wait4(struct lwp *, void *, register_t *);

int	sys___getcwd(struct lwp *, void *, register_t *);

int	sys_mincore(struct lwp *, void *, register_t *);

int	linux_sys_getdents64(struct lwp *, void *, register_t *);

int	linux_sys_setxattr(struct lwp *, void *, register_t *);

int	linux_sys_lsetxattr(struct lwp *, void *, register_t *);

int	linux_sys_fsetxattr(struct lwp *, void *, register_t *);

int	linux_sys_getxattr(struct lwp *, void *, register_t *);

int	linux_sys_lgetxattr(struct lwp *, void *, register_t *);

int	linux_sys_fgetxattr(struct lwp *, void *, register_t *);

int	linux_sys_listxattr(struct lwp *, void *, register_t *);

int	linux_sys_llistxattr(struct lwp *, void *, register_t *);

int	linux_sys_flistxattr(struct lwp *, void *, register_t *);

int	linux_sys_removexattr(struct lwp *, void *, register_t *);

int	linux_sys_lremovexattr(struct lwp *, void *, register_t *);

int	linux_sys_fremovexattr(struct lwp *, void *, register_t *);

int	linux_sys_exit_group(struct lwp *, void *, register_t *);

int	linux_sys_clock_settime(struct lwp *, void *, register_t *);

int	linux_sys_clock_gettime(struct lwp *, void *, register_t *);

int	linux_sys_clock_getres(struct lwp *, void *, register_t *);

int	linux_sys_clock_nanosleep(struct lwp *, void *, register_t *);

#endif /* _LINUX_SYS_SYSCALLARGS_H_ */

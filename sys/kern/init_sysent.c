/*
 * System call switch table.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.46 1996/12/22 08:04:06 cgd Exp 
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/syscallargs.h>

#ifdef COMPAT_43
#define compat_43(func) __CONCAT(compat_43_,func)
#else
#define compat_43(func) sys_nosys
#endif

#ifdef COMPAT_09
#define compat_09(func) __CONCAT(compat_09_,func)
#else
#define compat_09(func) sys_nosys
#endif

#ifdef COMPAT_10
#define compat_10(func) __CONCAT(compat_10_,func)
#else
#define compat_10(func) sys_nosys
#endif

#ifdef COMPAT_11
#define compat_11(func) __CONCAT(compat_11_,func)
#else
#define compat_11(func) sys_nosys
#endif

#ifdef COMPAT_12
#define compat_12(func) __CONCAT(compat_12_,func)
#else
#define compat_12(func) sys_nosys
#endif

#define	s(type)	sizeof(type)

struct sysent sysent[] = {
	{ 0, 0,
	    sys_nosys },			/* 0 = syscall (indir) */
	{ 1, s(struct sys_exit_args),
	    sys_exit },				/* 1 = exit */
	{ 0, 0,
	    sys_fork },				/* 2 = fork */
	{ 3, s(struct sys_read_args),
	    sys_read },				/* 3 = read */
	{ 3, s(struct sys_write_args),
	    sys_write },			/* 4 = write */
	{ 3, s(struct sys_open_args),
	    sys_open },				/* 5 = open */
	{ 1, s(struct sys_close_args),
	    sys_close },			/* 6 = close */
	{ 4, s(struct sys_wait4_args),
	    sys_wait4 },			/* 7 = wait4 */
	{ 2, s(struct compat_43_sys_creat_args),
	    compat_43(sys_creat) },		/* 8 = compat_43 ocreat */
	{ 2, s(struct sys_link_args),
	    sys_link },				/* 9 = link */
	{ 1, s(struct sys_unlink_args),
	    sys_unlink },			/* 10 = unlink */
	{ 0, 0,
	    sys_nosys },			/* 11 = obsolete execv */
	{ 1, s(struct sys_chdir_args),
	    sys_chdir },			/* 12 = chdir */
	{ 1, s(struct sys_fchdir_args),
	    sys_fchdir },			/* 13 = fchdir */
	{ 3, s(struct sys_mknod_args),
	    sys_mknod },			/* 14 = mknod */
	{ 2, s(struct sys_chmod_args),
	    sys_chmod },			/* 15 = chmod */
	{ 3, s(struct sys_chown_args),
	    sys_chown },			/* 16 = chown */
	{ 1, s(struct sys_obreak_args),
	    sys_obreak },			/* 17 = break */
	{ 3, s(struct sys_getfsstat_args),
	    sys_getfsstat },			/* 18 = getfsstat */
	{ 3, s(struct compat_43_sys_lseek_args),
	    compat_43(sys_lseek) },		/* 19 = compat_43 olseek */
	{ 0, 0,
	    sys_getpid },			/* 20 = getpid */
	{ 4, s(struct sys_mount_args),
	    sys_mount },			/* 21 = mount */
	{ 2, s(struct sys_unmount_args),
	    sys_unmount },			/* 22 = unmount */
	{ 1, s(struct sys_setuid_args),
	    sys_setuid },			/* 23 = setuid */
	{ 0, 0,
	    sys_getuid },			/* 24 = getuid */
	{ 0, 0,
	    sys_geteuid },			/* 25 = geteuid */
	{ 4, s(struct sys_ptrace_args),
	    sys_ptrace },			/* 26 = ptrace */
	{ 3, s(struct sys_recvmsg_args),
	    sys_recvmsg },			/* 27 = recvmsg */
	{ 3, s(struct sys_sendmsg_args),
	    sys_sendmsg },			/* 28 = sendmsg */
	{ 6, s(struct sys_recvfrom_args),
	    sys_recvfrom },			/* 29 = recvfrom */
	{ 3, s(struct sys_accept_args),
	    sys_accept },			/* 30 = accept */
	{ 3, s(struct sys_getpeername_args),
	    sys_getpeername },			/* 31 = getpeername */
	{ 3, s(struct sys_getsockname_args),
	    sys_getsockname },			/* 32 = getsockname */
	{ 2, s(struct sys_access_args),
	    sys_access },			/* 33 = access */
	{ 2, s(struct sys_chflags_args),
	    sys_chflags },			/* 34 = chflags */
	{ 2, s(struct sys_fchflags_args),
	    sys_fchflags },			/* 35 = fchflags */
	{ 0, 0,
	    sys_sync },				/* 36 = sync */
	{ 2, s(struct sys_kill_args),
	    sys_kill },				/* 37 = kill */
	{ 2, s(struct compat_43_sys_stat_args),
	    compat_43(sys_stat) },		/* 38 = compat_43 ostat */
	{ 0, 0,
	    sys_getppid },			/* 39 = getppid */
	{ 2, s(struct compat_43_sys_lstat_args),
	    compat_43(sys_lstat) },		/* 40 = compat_43 olstat */
	{ 1, s(struct sys_dup_args),
	    sys_dup },				/* 41 = dup */
	{ 0, 0,
	    sys_pipe },				/* 42 = pipe */
	{ 0, 0,
	    sys_getegid },			/* 43 = getegid */
	{ 4, s(struct sys_profil_args),
	    sys_profil },			/* 44 = profil */
#ifdef KTRACE
	{ 4, s(struct sys_ktrace_args),
	    sys_ktrace },			/* 45 = ktrace */
#else
	{ 0, 0,
	    sys_nosys },			/* 45 = unimplemented ktrace */
#endif
	{ 3, s(struct sys_sigaction_args),
	    sys_sigaction },			/* 46 = sigaction */
	{ 0, 0,
	    sys_getgid },			/* 47 = getgid */
	{ 2, s(struct sys_sigprocmask_args),
	    sys_sigprocmask },			/* 48 = sigprocmask */
	{ 2, s(struct sys___getlogin_args),
	    sys___getlogin },			/* 49 = __getlogin */
	{ 1, s(struct sys_setlogin_args),
	    sys_setlogin },			/* 50 = setlogin */
	{ 1, s(struct sys_acct_args),
	    sys_acct },				/* 51 = acct */
	{ 0, 0,
	    sys_sigpending },			/* 52 = sigpending */
	{ 2, s(struct sys_sigaltstack_args),
	    sys_sigaltstack },			/* 53 = sigaltstack */
	{ 3, s(struct sys_ioctl_args),
	    sys_ioctl },			/* 54 = ioctl */
	{ 1, s(struct compat_12_sys_reboot_args),
	    compat_12(sys_reboot) },		/* 55 = compat_12 oreboot */
	{ 1, s(struct sys_revoke_args),
	    sys_revoke },			/* 56 = revoke */
	{ 2, s(struct sys_symlink_args),
	    sys_symlink },			/* 57 = symlink */
	{ 3, s(struct sys_readlink_args),
	    sys_readlink },			/* 58 = readlink */
	{ 3, s(struct sys_execve_args),
	    sys_execve },			/* 59 = execve */
	{ 1, s(struct sys_umask_args),
	    sys_umask },			/* 60 = umask */
	{ 1, s(struct sys_chroot_args),
	    sys_chroot },			/* 61 = chroot */
	{ 2, s(struct compat_43_sys_fstat_args),
	    compat_43(sys_fstat) },		/* 62 = compat_43 ofstat */
	{ 4, s(struct compat_43_sys_getkerninfo_args),
	    compat_43(sys_getkerninfo) },	/* 63 = compat_43 ogetkerninfo */
	{ 0, 0,
	    compat_43(sys_getpagesize) },	/* 64 = compat_43 ogetpagesize */
	{ 2, s(struct sys_msync_args),
	    sys_msync },			/* 65 = msync */
	{ 0, 0,
	    sys_vfork },			/* 66 = vfork */
	{ 0, 0,
	    sys_nosys },			/* 67 = obsolete vread */
	{ 0, 0,
	    sys_nosys },			/* 68 = obsolete vwrite */
	{ 1, s(struct sys_sbrk_args),
	    sys_sbrk },				/* 69 = sbrk */
	{ 1, s(struct sys_sstk_args),
	    sys_sstk },				/* 70 = sstk */
	{ 6, s(struct compat_43_sys_mmap_args),
	    compat_43(sys_mmap) },		/* 71 = compat_43 ommap */
	{ 1, s(struct sys_ovadvise_args),
	    sys_ovadvise },			/* 72 = vadvise */
	{ 2, s(struct sys_munmap_args),
	    sys_munmap },			/* 73 = munmap */
	{ 3, s(struct sys_mprotect_args),
	    sys_mprotect },			/* 74 = mprotect */
	{ 3, s(struct sys_madvise_args),
	    sys_madvise },			/* 75 = madvise */
	{ 0, 0,
	    sys_nosys },			/* 76 = obsolete vhangup */
	{ 0, 0,
	    sys_nosys },			/* 77 = obsolete vlimit */
	{ 3, s(struct sys_mincore_args),
	    sys_mincore },			/* 78 = mincore */
	{ 2, s(struct sys_getgroups_args),
	    sys_getgroups },			/* 79 = getgroups */
	{ 2, s(struct sys_setgroups_args),
	    sys_setgroups },			/* 80 = setgroups */
	{ 0, 0,
	    sys_getpgrp },			/* 81 = getpgrp */
	{ 2, s(struct sys_setpgid_args),
	    sys_setpgid },			/* 82 = setpgid */
	{ 3, s(struct sys_setitimer_args),
	    sys_setitimer },			/* 83 = setitimer */
	{ 0, 0,
	    compat_43(sys_wait) },		/* 84 = compat_43 owait */
	{ 1, s(struct sys_swapon_args),
	    sys_swapon },			/* 85 = swapon */
	{ 2, s(struct sys_getitimer_args),
	    sys_getitimer },			/* 86 = getitimer */
	{ 2, s(struct compat_43_sys_gethostname_args),
	    compat_43(sys_gethostname) },	/* 87 = compat_43 ogethostname */
	{ 2, s(struct compat_43_sys_sethostname_args),
	    compat_43(sys_sethostname) },	/* 88 = compat_43 osethostname */
	{ 0, 0,
	    compat_43(sys_getdtablesize) },	/* 89 = compat_43 ogetdtablesize */
	{ 2, s(struct sys_dup2_args),
	    sys_dup2 },				/* 90 = dup2 */
	{ 0, 0,
	    sys_nosys },			/* 91 = unimplemented getdopt */
	{ 3, s(struct sys_fcntl_args),
	    sys_fcntl },			/* 92 = fcntl */
	{ 5, s(struct sys_select_args),
	    sys_select },			/* 93 = select */
	{ 0, 0,
	    sys_nosys },			/* 94 = unimplemented setdopt */
	{ 1, s(struct sys_fsync_args),
	    sys_fsync },			/* 95 = fsync */
	{ 3, s(struct sys_setpriority_args),
	    sys_setpriority },			/* 96 = setpriority */
	{ 3, s(struct sys_socket_args),
	    sys_socket },			/* 97 = socket */
	{ 3, s(struct sys_connect_args),
	    sys_connect },			/* 98 = connect */
	{ 3, s(struct compat_43_sys_accept_args),
	    compat_43(sys_accept) },		/* 99 = compat_43 oaccept */
	{ 2, s(struct sys_getpriority_args),
	    sys_getpriority },			/* 100 = getpriority */
	{ 4, s(struct compat_43_sys_send_args),
	    compat_43(sys_send) },		/* 101 = compat_43 osend */
	{ 4, s(struct compat_43_sys_recv_args),
	    compat_43(sys_recv) },		/* 102 = compat_43 orecv */
	{ 1, s(struct sys_sigreturn_args),
	    sys_sigreturn },			/* 103 = sigreturn */
	{ 3, s(struct sys_bind_args),
	    sys_bind },				/* 104 = bind */
	{ 5, s(struct sys_setsockopt_args),
	    sys_setsockopt },			/* 105 = setsockopt */
	{ 2, s(struct sys_listen_args),
	    sys_listen },			/* 106 = listen */
	{ 0, 0,
	    sys_nosys },			/* 107 = obsolete vtimes */
	{ 3, s(struct compat_43_sys_sigvec_args),
	    compat_43(sys_sigvec) },		/* 108 = compat_43 osigvec */
	{ 1, s(struct compat_43_sys_sigblock_args),
	    compat_43(sys_sigblock) },		/* 109 = compat_43 osigblock */
	{ 1, s(struct compat_43_sys_sigsetmask_args),
	    compat_43(sys_sigsetmask) },	/* 110 = compat_43 osigsetmask */
	{ 1, s(struct sys_sigsuspend_args),
	    sys_sigsuspend },			/* 111 = sigsuspend */
	{ 2, s(struct compat_43_sys_sigstack_args),
	    compat_43(sys_sigstack) },		/* 112 = compat_43 osigstack */
	{ 3, s(struct compat_43_sys_recvmsg_args),
	    compat_43(sys_recvmsg) },		/* 113 = compat_43 orecvmsg */
	{ 3, s(struct compat_43_sys_sendmsg_args),
	    compat_43(sys_sendmsg) },		/* 114 = compat_43 osendmsg */
#ifdef TRACE
	{ 2, s(struct sys_vtrace_args),
	    sys_vtrace },			/* 115 = vtrace */
#else
	{ 0, 0,
	    sys_nosys },			/* 115 = obsolete vtrace */
#endif
	{ 2, s(struct sys_gettimeofday_args),
	    sys_gettimeofday },			/* 116 = gettimeofday */
	{ 2, s(struct sys_getrusage_args),
	    sys_getrusage },			/* 117 = getrusage */
	{ 5, s(struct sys_getsockopt_args),
	    sys_getsockopt },			/* 118 = getsockopt */
	{ 0, 0,
	    sys_nosys },			/* 119 = obsolete resuba */
	{ 3, s(struct sys_readv_args),
	    sys_readv },			/* 120 = readv */
	{ 3, s(struct sys_writev_args),
	    sys_writev },			/* 121 = writev */
	{ 2, s(struct sys_settimeofday_args),
	    sys_settimeofday },			/* 122 = settimeofday */
	{ 3, s(struct sys_fchown_args),
	    sys_fchown },			/* 123 = fchown */
	{ 2, s(struct sys_fchmod_args),
	    sys_fchmod },			/* 124 = fchmod */
	{ 6, s(struct compat_43_sys_recvfrom_args),
	    compat_43(sys_recvfrom) },		/* 125 = compat_43 orecvfrom */
	{ 2, s(struct sys_setreuid_args),
	    sys_setreuid },			/* 126 = setreuid */
	{ 2, s(struct sys_setregid_args),
	    sys_setregid },			/* 127 = setregid */
	{ 2, s(struct sys_rename_args),
	    sys_rename },			/* 128 = rename */
	{ 2, s(struct compat_43_sys_truncate_args),
	    compat_43(sys_truncate) },		/* 129 = compat_43 otruncate */
	{ 2, s(struct compat_43_sys_ftruncate_args),
	    compat_43(sys_ftruncate) },		/* 130 = compat_43 oftruncate */
	{ 2, s(struct sys_flock_args),
	    sys_flock },			/* 131 = flock */
	{ 2, s(struct sys_mkfifo_args),
	    sys_mkfifo },			/* 132 = mkfifo */
	{ 6, s(struct sys_sendto_args),
	    sys_sendto },			/* 133 = sendto */
	{ 2, s(struct sys_shutdown_args),
	    sys_shutdown },			/* 134 = shutdown */
	{ 4, s(struct sys_socketpair_args),
	    sys_socketpair },			/* 135 = socketpair */
	{ 2, s(struct sys_mkdir_args),
	    sys_mkdir },			/* 136 = mkdir */
	{ 1, s(struct sys_rmdir_args),
	    sys_rmdir },			/* 137 = rmdir */
	{ 2, s(struct sys_utimes_args),
	    sys_utimes },			/* 138 = utimes */
	{ 0, 0,
	    sys_nosys },			/* 139 = obsolete 4.2 sigreturn */
	{ 2, s(struct sys_adjtime_args),
	    sys_adjtime },			/* 140 = adjtime */
	{ 3, s(struct compat_43_sys_getpeername_args),
	    compat_43(sys_getpeername) },	/* 141 = compat_43 ogetpeername */
	{ 0, 0,
	    compat_43(sys_gethostid) },		/* 142 = compat_43 ogethostid */
	{ 1, s(struct compat_43_sys_sethostid_args),
	    compat_43(sys_sethostid) },		/* 143 = compat_43 osethostid */
	{ 2, s(struct compat_43_sys_getrlimit_args),
	    compat_43(sys_getrlimit) },		/* 144 = compat_43 ogetrlimit */
	{ 2, s(struct compat_43_sys_setrlimit_args),
	    compat_43(sys_setrlimit) },		/* 145 = compat_43 osetrlimit */
	{ 2, s(struct compat_43_sys_killpg_args),
	    compat_43(sys_killpg) },		/* 146 = compat_43 okillpg */
	{ 0, 0,
	    sys_setsid },			/* 147 = setsid */
	{ 4, s(struct sys_quotactl_args),
	    sys_quotactl },			/* 148 = quotactl */
	{ 0, 0,
	    compat_43(sys_quota) },		/* 149 = compat_43 oquota */
	{ 3, s(struct compat_43_sys_getsockname_args),
	    compat_43(sys_getsockname) },	/* 150 = compat_43 ogetsockname */
	{ 0, 0,
	    sys_nosys },			/* 151 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 152 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 153 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 154 = unimplemented */
#if defined(NFSCLIENT) || defined(NFSSERVER)
	{ 2, s(struct sys_nfssvc_args),
	    sys_nfssvc },			/* 155 = nfssvc */
#else
	{ 0, 0,
	    sys_nosys },			/* 155 = unimplemented */
#endif
	{ 4, s(struct compat_43_sys_getdirentries_args),
	    compat_43(sys_getdirentries) },	/* 156 = compat_43 ogetdirentries */
	{ 2, s(struct sys_statfs_args),
	    sys_statfs },			/* 157 = statfs */
	{ 2, s(struct sys_fstatfs_args),
	    sys_fstatfs },			/* 158 = fstatfs */
	{ 0, 0,
	    sys_nosys },			/* 159 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 160 = unimplemented */
#if defined(NFSCLIENT) || defined(NFSSERVER)
	{ 2, s(struct sys_getfh_args),
	    sys_getfh },			/* 161 = getfh */
#else
	{ 0, 0,
	    sys_nosys },			/* 161 = unimplemented getfh */
#endif
	{ 2, s(struct compat_09_sys_getdomainname_args),
	    compat_09(sys_getdomainname) },	/* 162 = compat_09 ogetdomainname */
	{ 2, s(struct compat_09_sys_setdomainname_args),
	    compat_09(sys_setdomainname) },	/* 163 = compat_09 osetdomainname */
	{ 1, s(struct compat_09_sys_uname_args),
	    compat_09(sys_uname) },		/* 164 = compat_09 ouname */
	{ 2, s(struct sys_sysarch_args),
	    sys_sysarch },			/* 165 = sysarch */
	{ 0, 0,
	    sys_nosys },			/* 166 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 167 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 168 = unimplemented */
#if defined(SYSVSEM) && !defined(alpha)
	{ 5, s(struct compat_10_sys_semsys_args),
	    compat_10(sys_semsys) },		/* 169 = compat_10 osemsys */
#else
	{ 0, 0,
	    sys_nosys },			/* 169 = unimplemented 1.0 semsys */
#endif
#if defined(SYSVMSG) && !defined(alpha)
	{ 6, s(struct compat_10_sys_msgsys_args),
	    compat_10(sys_msgsys) },		/* 170 = compat_10 omsgsys */
#else
	{ 0, 0,
	    sys_nosys },			/* 170 = unimplemented 1.0 msgsys */
#endif
#if defined(SYSVSHM) && !defined(alpha)
	{ 4, s(struct compat_10_sys_shmsys_args),
	    compat_10(sys_shmsys) },		/* 171 = compat_10 oshmsys */
#else
	{ 0, 0,
	    sys_nosys },			/* 171 = unimplemented 1.0 shmsys */
#endif
	{ 0, 0,
	    sys_nosys },			/* 172 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 173 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 174 = unimplemented */
	{ 1, s(struct sys_ntp_gettime_args),
	    sys_ntp_gettime },			/* 175 = ntp_gettime */
	{ 1, s(struct sys_ntp_adjtime_args),
	    sys_ntp_adjtime },			/* 176 = ntp_adjtime */
	{ 0, 0,
	    sys_nosys },			/* 177 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 178 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 179 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 180 = unimplemented */
	{ 1, s(struct sys_setgid_args),
	    sys_setgid },			/* 181 = setgid */
	{ 1, s(struct sys_setegid_args),
	    sys_setegid },			/* 182 = setegid */
	{ 1, s(struct sys_seteuid_args),
	    sys_seteuid },			/* 183 = seteuid */
#ifdef LFS
	{ 3, s(struct lfs_bmapv_args),
	    lfs_bmapv },			/* 184 = lfs_bmapv */
	{ 3, s(struct lfs_markv_args),
	    lfs_markv },			/* 185 = lfs_markv */
	{ 2, s(struct lfs_segclean_args),
	    lfs_segclean },			/* 186 = lfs_segclean */
	{ 2, s(struct lfs_segwait_args),
	    lfs_segwait },			/* 187 = lfs_segwait */
#else
	{ 0, 0,
	    sys_nosys },			/* 184 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 185 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 186 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 187 = unimplemented */
#endif
	{ 2, s(struct sys_stat_args),
	    sys_stat },				/* 188 = stat */
	{ 2, s(struct sys_fstat_args),
	    sys_fstat },			/* 189 = fstat */
	{ 2, s(struct sys_lstat_args),
	    sys_lstat },			/* 190 = lstat */
	{ 2, s(struct sys_pathconf_args),
	    sys_pathconf },			/* 191 = pathconf */
	{ 2, s(struct sys_fpathconf_args),
	    sys_fpathconf },			/* 192 = fpathconf */
	{ 0, 0,
	    sys_nosys },			/* 193 = unimplemented */
	{ 2, s(struct sys_getrlimit_args),
	    sys_getrlimit },			/* 194 = getrlimit */
	{ 2, s(struct sys_setrlimit_args),
	    sys_setrlimit },			/* 195 = setrlimit */
	{ 4, s(struct sys_getdirentries_args),
	    sys_getdirentries },		/* 196 = getdirentries */
	{ 7, s(struct sys_mmap_args),
	    sys_mmap },				/* 197 = mmap */
	{ 0, 0,
	    sys_nosys },			/* 198 = __syscall (indir) */
	{ 4, s(struct sys_lseek_args),
	    sys_lseek },			/* 199 = lseek */
	{ 3, s(struct sys_truncate_args),
	    sys_truncate },			/* 200 = truncate */
	{ 3, s(struct sys_ftruncate_args),
	    sys_ftruncate },			/* 201 = ftruncate */
	{ 6, s(struct sys___sysctl_args),
	    sys___sysctl },			/* 202 = __sysctl */
	{ 2, s(struct sys_mlock_args),
	    sys_mlock },			/* 203 = mlock */
	{ 2, s(struct sys_munlock_args),
	    sys_munlock },			/* 204 = munlock */
	{ 1, s(struct sys_undelete_args),
	    sys_undelete },			/* 205 = undelete */
	{ 2, s(struct sys_futimes_args),
	    sys_futimes },			/* 206 = futimes */
	{ 1, s(struct sys_getpgid_args),
	    sys_getpgid },			/* 207 = getpgid */
	{ 2, s(struct sys_reboot_args),
	    sys_reboot },			/* 208 = reboot */
	{ 3, s(struct sys_poll_args),
	    sys_poll },				/* 209 = poll */
#ifdef LKM
	{ 0, 0,
	    sys_lkmnosys },			/* 210 = lkmnosys */
	{ 0, 0,
	    sys_lkmnosys },			/* 211 = lkmnosys */
	{ 0, 0,
	    sys_lkmnosys },			/* 212 = lkmnosys */
	{ 0, 0,
	    sys_lkmnosys },			/* 213 = lkmnosys */
	{ 0, 0,
	    sys_lkmnosys },			/* 214 = lkmnosys */
	{ 0, 0,
	    sys_lkmnosys },			/* 215 = lkmnosys */
	{ 0, 0,
	    sys_lkmnosys },			/* 216 = lkmnosys */
	{ 0, 0,
	    sys_lkmnosys },			/* 217 = lkmnosys */
	{ 0, 0,
	    sys_lkmnosys },			/* 218 = lkmnosys */
	{ 0, 0,
	    sys_lkmnosys },			/* 219 = lkmnosys */
#else	/* !LKM */
	{ 0, 0,
	    sys_nosys },			/* 210 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 211 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 212 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 213 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 214 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 215 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 216 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 217 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 218 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 219 = unimplemented */
#endif	/* !LKM */
#ifdef SYSVSEM
	{ 4, s(struct sys___semctl_args),
	    sys___semctl },			/* 220 = __semctl */
	{ 3, s(struct sys_semget_args),
	    sys_semget },			/* 221 = semget */
	{ 3, s(struct sys_semop_args),
	    sys_semop },			/* 222 = semop */
	{ 1, s(struct sys_semconfig_args),
	    sys_semconfig },			/* 223 = semconfig */
#else
	{ 0, 0,
	    sys_nosys },			/* 220 = unimplemented semctl */
	{ 0, 0,
	    sys_nosys },			/* 221 = unimplemented semget */
	{ 0, 0,
	    sys_nosys },			/* 222 = unimplemented semop */
	{ 0, 0,
	    sys_nosys },			/* 223 = unimplemented semconfig */
#endif
#ifdef SYSVMSG
	{ 3, s(struct sys_msgctl_args),
	    sys_msgctl },			/* 224 = msgctl */
	{ 2, s(struct sys_msgget_args),
	    sys_msgget },			/* 225 = msgget */
	{ 4, s(struct sys_msgsnd_args),
	    sys_msgsnd },			/* 226 = msgsnd */
	{ 5, s(struct sys_msgrcv_args),
	    sys_msgrcv },			/* 227 = msgrcv */
#else
	{ 0, 0,
	    sys_nosys },			/* 224 = unimplemented msgctl */
	{ 0, 0,
	    sys_nosys },			/* 225 = unimplemented msgget */
	{ 0, 0,
	    sys_nosys },			/* 226 = unimplemented msgsnd */
	{ 0, 0,
	    sys_nosys },			/* 227 = unimplemented msgrcv */
#endif
#ifdef SYSVSHM
	{ 3, s(struct sys_shmat_args),
	    sys_shmat },			/* 228 = shmat */
	{ 3, s(struct sys_shmctl_args),
	    sys_shmctl },			/* 229 = shmctl */
	{ 1, s(struct sys_shmdt_args),
	    sys_shmdt },			/* 230 = shmdt */
	{ 3, s(struct sys_shmget_args),
	    sys_shmget },			/* 231 = shmget */
#else
	{ 0, 0,
	    sys_nosys },			/* 228 = unimplemented shmat */
	{ 0, 0,
	    sys_nosys },			/* 229 = unimplemented shmctl */
	{ 0, 0,
	    sys_nosys },			/* 230 = unimplemented shmdt */
	{ 0, 0,
	    sys_nosys },			/* 231 = unimplemented shmget */
#endif
	{ 2, s(struct sys_clock_gettime_args),
	    sys_clock_gettime },		/* 232 = clock_gettime */
	{ 2, s(struct sys_clock_settime_args),
	    sys_clock_settime },		/* 233 = clock_settime */
	{ 2, s(struct sys_clock_getres_args),
	    sys_clock_getres },			/* 234 = clock_getres */
	{ 0, 0,
	    sys_nosys },			/* 235 = unimplemented timer_create */
	{ 0, 0,
	    sys_nosys },			/* 236 = unimplemented timer_delete */
	{ 0, 0,
	    sys_nosys },			/* 237 = unimplemented timer_settime */
	{ 0, 0,
	    sys_nosys },			/* 238 = unimplemented timer_gettime */
	{ 0, 0,
	    sys_nosys },			/* 239 = unimplemented timer_getoverrun */
	{ 0, 0,
	    sys_nosys },			/* 240 = unimplemented nanosleep */
};


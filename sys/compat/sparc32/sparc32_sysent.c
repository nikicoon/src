/*	$NetBSD: sparc32_sysent.c,v 1.1 1998/08/26 10:20:37 mrg Exp $	*/

/*
 * System call switch table.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD
 */

#include "opt_ktrace.h"
#include "opt_nfsserver.h"
#include "opt_compat_netbsd.h"
#include "fs_lfs.h"
#include "fs_nfs.h"
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/syscallargs.h>
#include <compat/sparc32/sparc32.h>
#include <compat/sparc32/sparc32_syscallargs.h>

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

#ifdef COMPAT_13
#define compat_13(func) __CONCAT(compat_13_,func)
#else
#define compat_13(func) sys_nosys
#endif

#define	s(type)	sizeof(type)

struct sysent sparc32_sysent[] = {
	{ 0, 0,
	    sys_nosys },			/* 0 = syscall (indir) */
	{ 1, s(struct sys_exit_args),
	    sys_exit },				/* 1 = exit */
	{ 0, 0,
	    sys_fork },				/* 2 = fork */
	{ 3, s(struct compat_sparc32_read_args),
	    compat_sparc32_read },		/* 3 = compat_sparc32_read */
	{ 3, s(struct compat_sparc32_write_args),
	    compat_sparc32_write },		/* 4 = compat_sparc32_write */
	{ 3, s(struct compat_sparc32_open_args),
	    compat_sparc32_open },		/* 5 = compat_sparc32_open */
	{ 1, s(struct sys_close_args),
	    sys_close },			/* 6 = close */
	{ 4, s(struct compat_sparc32_wait4_args),
	    compat_sparc32_wait4 },		/* 7 = compat_sparc32_wait4 */
	{ 2, s(struct compat_43_compat_sparc32_ocreat_args),
	    compat_43(compat_sparc32_ocreat) },	/* 8 = compat_43 compat_sparc32_ocreat */
	{ 2, s(struct compat_sparc32_link_args),
	    compat_sparc32_link },		/* 9 = compat_sparc32_link */
	{ 1, s(struct compat_sparc32_unlink_args),
	    compat_sparc32_unlink },		/* 10 = compat_sparc32_unlink */
	{ 0, 0,
	    sys_nosys },			/* 11 = obsolete execv */
	{ 1, s(struct compat_sparc32_chdir_args),
	    compat_sparc32_chdir },		/* 12 = compat_sparc32_chdir */
	{ 1, s(struct sys_fchdir_args),
	    sys_fchdir },			/* 13 = fchdir */
	{ 3, s(struct compat_sparc32_mknod_args),
	    compat_sparc32_mknod },		/* 14 = compat_sparc32_mknod */
	{ 2, s(struct compat_sparc32_chmod_args),
	    compat_sparc32_chmod },		/* 15 = compat_sparc32_chmod */
	{ 3, s(struct compat_sparc32_chown_args),
	    compat_sparc32_chown },		/* 16 = compat_sparc32_chown */
	{ 1, s(struct compat_sparc32_break_args),
	    compat_sparc32_break },		/* 17 = compat_sparc32_break */
	{ 3, s(struct compat_sparc32_getfsstat_args),
	    compat_sparc32_getfsstat },		/* 18 = compat_sparc32_getfsstat */
	{ 3, s(struct compat_43_compat_sparc32_olseek_args),
	    compat_43(compat_sparc32_olseek) },	/* 19 = compat_43 compat_sparc32_olseek */
	{ 0, 0,
	    sys_getpid },			/* 20 = getpid */
	{ 4, s(struct compat_sparc32_mount_args),
	    compat_sparc32_mount },		/* 21 = compat_sparc32_mount */
	{ 2, s(struct compat_sparc32_unmount_args),
	    compat_sparc32_unmount },		/* 22 = compat_sparc32_unmount */
	{ 1, s(struct sys_setuid_args),
	    sys_setuid },			/* 23 = setuid */
	{ 0, 0,
	    sys_getuid },			/* 24 = getuid */
	{ 0, 0,
	    sys_geteuid },			/* 25 = geteuid */
	{ 4, s(struct compat_sparc32_ptrace_args),
	    compat_sparc32_ptrace },		/* 26 = compat_sparc32_ptrace */
	{ 3, s(struct compat_sparc32_recvmsg_args),
	    compat_sparc32_recvmsg },		/* 27 = compat_sparc32_recvmsg */
	{ 3, s(struct compat_sparc32_sendmsg_args),
	    compat_sparc32_sendmsg },		/* 28 = compat_sparc32_sendmsg */
	{ 6, s(struct compat_sparc32_recvfrom_args),
	    compat_sparc32_recvfrom },		/* 29 = compat_sparc32_recvfrom */
	{ 3, s(struct compat_sparc32_accept_args),
	    compat_sparc32_accept },		/* 30 = compat_sparc32_accept */
	{ 3, s(struct compat_sparc32_getpeername_args),
	    compat_sparc32_getpeername },	/* 31 = compat_sparc32_getpeername */
	{ 3, s(struct compat_sparc32_getsockname_args),
	    compat_sparc32_getsockname },	/* 32 = compat_sparc32_getsockname */
	{ 2, s(struct compat_sparc32_access_args),
	    compat_sparc32_access },		/* 33 = compat_sparc32_access */
	{ 2, s(struct compat_sparc32_chflags_args),
	    compat_sparc32_chflags },		/* 34 = compat_sparc32_chflags */
	{ 2, s(struct compat_sparc32_fchflags_args),
	    compat_sparc32_fchflags },		/* 35 = compat_sparc32_fchflags */
	{ 0, 0,
	    sys_sync },				/* 36 = sync */
	{ 2, s(struct sys_kill_args),
	    sys_kill },				/* 37 = kill */
	{ 2, s(struct compat_43_compat_sparc32_stat43_args),
	    compat_43(compat_sparc32_stat43) },	/* 38 = compat_43 compat_sparc32_stat43 */
	{ 0, 0,
	    sys_getppid },			/* 39 = getppid */
	{ 2, s(struct compat_43_compat_sparc32_lstat43_args),
	    compat_43(compat_sparc32_lstat43) },/* 40 = compat_43 compat_sparc32_lstat43 */
	{ 1, s(struct sys_dup_args),
	    sys_dup },				/* 41 = dup */
	{ 0, 0,
	    sys_pipe },				/* 42 = pipe */
	{ 0, 0,
	    sys_getegid },			/* 43 = getegid */
	{ 4, s(struct compat_sparc32_profil_args),
	    compat_sparc32_profil },		/* 44 = compat_sparc32_profil */
#ifdef KTRACE
	{ 4, s(struct compat_sparc32_ktrace_args),
	    compat_sparc32_ktrace },		/* 45 = compat_sparc32_ktrace */
#else
	{ 0, 0,
	    sys_nosys },			/* 45 = unimplemented compat_sparc32_ktrace */
#endif
	{ 3, s(struct compat_sparc32_sigaction_args),
	    compat_sparc32_sigaction },		/* 46 = compat_sparc32_sigaction */
	{ 0, 0,
	    sys_getgid },			/* 47 = getgid */
	{ 2, s(struct sys_sigprocmask_args),
	    sys_sigprocmask },			/* 48 = sigprocmask */
	{ 2, s(struct compat_sparc32___getlogin_args),
	    compat_sparc32___getlogin },	/* 49 = compat_sparc32___getlogin */
	{ 1, s(struct compat_sparc32_setlogin_args),
	    compat_sparc32_setlogin },		/* 50 = compat_sparc32_setlogin */
	{ 1, s(struct compat_sparc32_acct_args),
	    compat_sparc32_acct },		/* 51 = compat_sparc32_acct */
	{ 0, 0,
	    sys_sigpending },			/* 52 = sigpending */
	{ 2, s(struct compat_13_compat_sparc32_sigaltstack13_args),
	    compat_13(compat_sparc32_sigaltstack13) },/* 53 = compat_13 compat_sparc32_sigaltstack13 */
	{ 3, s(struct compat_sparc32_ioctl_args),
	    compat_sparc32_ioctl },		/* 54 = compat_sparc32_ioctl */
	{ 1, s(struct compat_12_oreboot_args),
	    compat_12(oreboot) },		/* 55 = compat_12 oreboot */
	{ 1, s(struct compat_sparc32_revoke_args),
	    compat_sparc32_revoke },		/* 56 = compat_sparc32_revoke */
	{ 2, s(struct compat_sparc32_symlink_args),
	    compat_sparc32_symlink },		/* 57 = compat_sparc32_symlink */
	{ 3, s(struct compat_sparc32_readlink_args),
	    compat_sparc32_readlink },		/* 58 = compat_sparc32_readlink */
	{ 3, s(struct compat_sparc32_execve_args),
	    compat_sparc32_execve },		/* 59 = compat_sparc32_execve */
	{ 1, s(struct sys_umask_args),
	    sys_umask },			/* 60 = umask */
	{ 1, s(struct compat_sparc32_chroot_args),
	    compat_sparc32_chroot },		/* 61 = compat_sparc32_chroot */
	{ 2, s(struct compat_43_compat_sparc32_fstat43_args),
	    compat_43(compat_sparc32_fstat43) },/* 62 = compat_43 compat_sparc32_fstat43 */
	{ 4, s(struct compat_43_compat_sparc32_ogetkerninfo_args),
	    compat_43(compat_sparc32_ogetkerninfo) },/* 63 = compat_43 compat_sparc32_ogetkerninfo */
	{ 0, 0,
	    compat_43(sys_getpagesize) },	/* 64 = compat_43 ogetpagesize */
	{ 2, s(struct compat_12_compat_sparc32_msync_args),
	    compat_12(compat_sparc32_msync) },	/* 65 = compat_12 compat_sparc32_msync */
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
	{ 6, s(struct compat_43_compat_sparc32_ommap_args),
	    compat_43(compat_sparc32_ommap) },	/* 71 = compat_43 compat_sparc32_ommap */
	{ 1, s(struct sys_ovadvise_args),
	    sys_ovadvise },			/* 72 = vadvise */
	{ 2, s(struct compat_sparc32_munmap_args),
	    compat_sparc32_munmap },		/* 73 = compat_sparc32_munmap */
	{ 3, s(struct compat_sparc32_mprotect_args),
	    compat_sparc32_mprotect },		/* 74 = compat_sparc32_mprotect */
	{ 3, s(struct compat_sparc32_madvise_args),
	    compat_sparc32_madvise },		/* 75 = compat_sparc32_madvise */
	{ 0, 0,
	    sys_nosys },			/* 76 = obsolete vhangup */
	{ 0, 0,
	    sys_nosys },			/* 77 = obsolete vlimit */
	{ 3, s(struct compat_sparc32_mincore_args),
	    compat_sparc32_mincore },		/* 78 = compat_sparc32_mincore */
	{ 2, s(struct compat_sparc32_getgroups_args),
	    compat_sparc32_getgroups },		/* 79 = compat_sparc32_getgroups */
	{ 2, s(struct compat_sparc32_setgroups_args),
	    compat_sparc32_setgroups },		/* 80 = compat_sparc32_setgroups */
	{ 0, 0,
	    sys_getpgrp },			/* 81 = getpgrp */
	{ 2, s(struct sys_setpgid_args),
	    sys_setpgid },			/* 82 = setpgid */
	{ 3, s(struct compat_sparc32_setitimer_args),
	    compat_sparc32_setitimer },		/* 83 = compat_sparc32_setitimer */
	{ 0, 0,
	    compat_43(sys_wait) },		/* 84 = compat_43 owait */
	{ 1, s(struct compat_12_compat_sparc32_oswapon_args),
	    compat_12(compat_sparc32_oswapon) },/* 85 = compat_12 compat_sparc32_oswapon */
	{ 2, s(struct compat_sparc32_getitimer_args),
	    compat_sparc32_getitimer },		/* 86 = compat_sparc32_getitimer */
	{ 2, s(struct compat_43_compat_sparc32_ogethostname_args),
	    compat_43(compat_sparc32_ogethostname) },/* 87 = compat_43 compat_sparc32_ogethostname */
	{ 2, s(struct compat_43_compat_sparc32_osethostname_args),
	    compat_43(compat_sparc32_osethostname) },/* 88 = compat_43 compat_sparc32_osethostname */
	{ 0, 0,
	    compat_43(sys_getdtablesize) },	/* 89 = compat_43 ogetdtablesize */
	{ 2, s(struct sys_dup2_args),
	    sys_dup2 },				/* 90 = dup2 */
	{ 0, 0,
	    sys_nosys },			/* 91 = unimplemented getdopt */
	{ 3, s(struct compat_sparc32_fcntl_args),
	    compat_sparc32_fcntl },		/* 92 = compat_sparc32_fcntl */
	{ 5, s(struct compat_sparc32_select_args),
	    compat_sparc32_select },		/* 93 = compat_sparc32_select */
	{ 0, 0,
	    sys_nosys },			/* 94 = unimplemented setdopt */
	{ 1, s(struct sys_fsync_args),
	    sys_fsync },			/* 95 = fsync */
	{ 3, s(struct sys_setpriority_args),
	    sys_setpriority },			/* 96 = setpriority */
	{ 3, s(struct sys_socket_args),
	    sys_socket },			/* 97 = socket */
	{ 3, s(struct compat_sparc32_connect_args),
	    compat_sparc32_connect },		/* 98 = compat_sparc32_connect */
	{ 3, s(struct compat_43_compat_sparc32_oaccept_args),
	    compat_43(compat_sparc32_oaccept) },/* 99 = compat_43 compat_sparc32_oaccept */
	{ 2, s(struct sys_getpriority_args),
	    sys_getpriority },			/* 100 = getpriority */
	{ 4, s(struct compat_43_compat_sparc32_osend_args),
	    compat_43(compat_sparc32_osend) },	/* 101 = compat_43 compat_sparc32_osend */
	{ 4, s(struct compat_43_compat_sparc32_orecv_args),
	    compat_43(compat_sparc32_orecv) },	/* 102 = compat_43 compat_sparc32_orecv */
	{ 1, s(struct compat_sparc32_sigreturn_args),
	    compat_sparc32_sigreturn },		/* 103 = compat_sparc32_sigreturn */
	{ 3, s(struct compat_sparc32_bind_args),
	    compat_sparc32_bind },		/* 104 = compat_sparc32_bind */
	{ 5, s(struct compat_sparc32_setsockopt_args),
	    compat_sparc32_setsockopt },	/* 105 = compat_sparc32_setsockopt */
	{ 2, s(struct sys_listen_args),
	    sys_listen },			/* 106 = listen */
	{ 0, 0,
	    sys_nosys },			/* 107 = obsolete vtimes */
	{ 3, s(struct compat_43_compat_sparc32_osigvec_args),
	    compat_43(compat_sparc32_osigvec) },/* 108 = compat_43 compat_sparc32_osigvec */
#ifdef COMPAT_43
	{ 1, s(struct compat_43_sys_sigblock_args),
	    compat_43_sys_sigblock },		/* 109 = sigblock */
	{ 1, s(struct compat_43_sys_sigsetmask_args),
	    compat_43_sys_sigsetmask },		/* 110 = sigsetmask */
#else
	{ 0, 0,
	    sys_nosys },			/* 109 = obsolete sigblock */
	{ 0, 0,
	    sys_nosys },			/* 110 = obsolete sigsetmask */
#endif
	{ 1, s(struct sys_sigsuspend_args),
	    sys_sigsuspend },			/* 111 = sigsuspend */
	{ 2, s(struct compat_43_compat_sparc32_osigstack_args),
	    compat_43(compat_sparc32_osigstack) },/* 112 = compat_43 compat_sparc32_osigstack */
	{ 3, s(struct compat_43_compat_sparc32_orecvmsg_args),
	    compat_43(compat_sparc32_orecvmsg) },/* 113 = compat_43 compat_sparc32_orecvmsg */
	{ 3, s(struct compat_43_compat_sparc32_osendmsg_args),
	    compat_43(compat_sparc32_osendmsg) },/* 114 = compat_43 compat_sparc32_osendmsg */
#ifdef TRACE
	{ 2, s(struct sys_vtrace_args),
	    sys_vtrace },			/* 115 = vtrace */
#else
	{ 0, 0,
	    sys_nosys },			/* 115 = obsolete vtrace */
#endif
	{ 2, s(struct compat_sparc32_gettimeofday_args),
	    compat_sparc32_gettimeofday },	/* 116 = compat_sparc32_gettimeofday */
	{ 2, s(struct compat_sparc32_getrusage_args),
	    compat_sparc32_getrusage },		/* 117 = compat_sparc32_getrusage */
	{ 5, s(struct compat_sparc32_getsockopt_args),
	    compat_sparc32_getsockopt },	/* 118 = compat_sparc32_getsockopt */
	{ 0, 0,
	    sys_nosys },			/* 119 = obsolete resuba */
	{ 3, s(struct compat_sparc32_readv_args),
	    compat_sparc32_readv },		/* 120 = compat_sparc32_readv */
	{ 3, s(struct compat_sparc32_writev_args),
	    compat_sparc32_writev },		/* 121 = compat_sparc32_writev */
	{ 2, s(struct compat_sparc32_settimeofday_args),
	    compat_sparc32_settimeofday },	/* 122 = compat_sparc32_settimeofday */
	{ 3, s(struct sys_fchown_args),
	    sys_fchown },			/* 123 = fchown */
	{ 2, s(struct sys_fchmod_args),
	    sys_fchmod },			/* 124 = fchmod */
	{ 6, s(struct compat_43_compat_sparc32_orecvfrom_args),
	    compat_43(compat_sparc32_orecvfrom) },/* 125 = compat_43 compat_sparc32_orecvfrom */
	{ 2, s(struct sys_setreuid_args),
	    sys_setreuid },			/* 126 = setreuid */
	{ 2, s(struct sys_setregid_args),
	    sys_setregid },			/* 127 = setregid */
	{ 2, s(struct compat_sparc32_rename_args),
	    compat_sparc32_rename },		/* 128 = compat_sparc32_rename */
	{ 2, s(struct compat_43_compat_sparc32_otruncate_args),
	    compat_43(compat_sparc32_otruncate) },/* 129 = compat_43 compat_sparc32_otruncate */
	{ 2, s(struct compat_43_compat_sparc32_oftruncate_args),
	    compat_43(compat_sparc32_oftruncate) },/* 130 = compat_43 compat_sparc32_oftruncate */
	{ 2, s(struct sys_flock_args),
	    sys_flock },			/* 131 = flock */
	{ 2, s(struct compat_sparc32_mkfifo_args),
	    compat_sparc32_mkfifo },		/* 132 = compat_sparc32_mkfifo */
	{ 6, s(struct compat_sparc32_sendto_args),
	    compat_sparc32_sendto },		/* 133 = compat_sparc32_sendto */
	{ 2, s(struct sys_shutdown_args),
	    sys_shutdown },			/* 134 = shutdown */
	{ 4, s(struct compat_sparc32_socketpair_args),
	    compat_sparc32_socketpair },	/* 135 = compat_sparc32_socketpair */
	{ 2, s(struct compat_sparc32_mkdir_args),
	    compat_sparc32_mkdir },		/* 136 = compat_sparc32_mkdir */
	{ 1, s(struct compat_sparc32_rmdir_args),
	    compat_sparc32_rmdir },		/* 137 = compat_sparc32_rmdir */
	{ 2, s(struct compat_sparc32_utimes_args),
	    compat_sparc32_utimes },		/* 138 = compat_sparc32_utimes */
	{ 0, 0,
	    sys_nosys },			/* 139 = obsolete 4.2 sigreturn */
	{ 2, s(struct compat_sparc32_adjtime_args),
	    compat_sparc32_adjtime },		/* 140 = compat_sparc32_adjtime */
	{ 3, s(struct compat_43_compat_sparc32_ogetpeername_args),
	    compat_43(compat_sparc32_ogetpeername) },/* 141 = compat_43 compat_sparc32_ogetpeername */
	{ 0, 0,
	    compat_43(sys_gethostid) },		/* 142 = compat_43 ogethostid */
#ifdef COMPAT_43
	{ 1, s(struct compat_43_sys_sethostid_args),
	    compat_43_sys_sethostid },		/* 143 = sethostid */
#else
	{ 0, 0,
	    sys_nosys },			/* 143 = obsolete sethostid */
#endif
	{ 2, s(struct compat_43_compat_sparc32_ogetrlimit_args),
	    compat_43(compat_sparc32_ogetrlimit) },/* 144 = compat_43 compat_sparc32_ogetrlimit */
	{ 2, s(struct compat_43_compat_sparc32_osetrlimit_args),
	    compat_43(compat_sparc32_osetrlimit) },/* 145 = compat_43 compat_sparc32_osetrlimit */
#ifdef COMPAT_43
	{ 2, s(struct compat_43_sys_killpg_args),
	    compat_43_sys_killpg },		/* 146 = killpg */
#else
	{ 0, 0,
	    sys_nosys },			/* 146 = obsolete killpg */
#endif
	{ 0, 0,
	    sys_setsid },			/* 147 = setsid */
	{ 4, s(struct compat_sparc32_quotactl_args),
	    compat_sparc32_quotactl },		/* 148 = compat_sparc32_quotactl */
	{ 0, 0,
	    compat_43(sys_quota) },		/* 149 = compat_43 oquota */
	{ 3, s(struct compat_43_compat_sparc32_ogetsockname_args),
	    compat_43(compat_sparc32_ogetsockname) },/* 150 = compat_43 compat_sparc32_ogetsockname */
	{ 0, 0,
	    sys_nosys },			/* 151 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 152 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 153 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 154 = unimplemented */
#if defined(NFS) || defined(NFSSERVER)
	{ 2, s(struct compat_sparc32_nfssvc_args),
	    compat_sparc32_nfssvc },		/* 155 = compat_sparc32_nfssvc */
#else
	{ 0, 0,
	    sys_nosys },			/* 155 = unimplemented */
#endif
	{ 4, s(struct compat_43_compat_sparc32_ogetdirentries_args),
	    compat_43(compat_sparc32_ogetdirentries) },/* 156 = compat_43 compat_sparc32_ogetdirentries */
	{ 2, s(struct compat_sparc32_statfs_args),
	    compat_sparc32_statfs },		/* 157 = compat_sparc32_statfs */
	{ 2, s(struct compat_sparc32_fstatfs_args),
	    compat_sparc32_fstatfs },		/* 158 = compat_sparc32_fstatfs */
	{ 0, 0,
	    sys_nosys },			/* 159 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 160 = unimplemented */
#if defined(NFS) || defined(NFSSERVER)
	{ 2, s(struct compat_sparc32_getfh_args),
	    compat_sparc32_getfh },		/* 161 = compat_sparc32_getfh */
#else
	{ 0, 0,
	    sys_nosys },			/* 161 = unimplemented compat_sparc32_getfh */
#endif
	{ 2, s(struct compat_09_compat_sparc32_ogetdomainname_args),
	    compat_09(compat_sparc32_ogetdomainname) },/* 162 = compat_09 compat_sparc32_ogetdomainname */
	{ 2, s(struct compat_09_compat_sparc32_osetdomainname_args),
	    compat_09(compat_sparc32_osetdomainname) },/* 163 = compat_09 compat_sparc32_osetdomainname */
	{ 1, s(struct compat_09_compat_sparc32_uname_args),
	    compat_09(compat_sparc32_uname) },	/* 164 = compat_09 compat_sparc32_uname */
	{ 2, s(struct compat_sparc32_sysarch_args),
	    compat_sparc32_sysarch },		/* 165 = compat_sparc32_sysarch */
	{ 0, 0,
	    sys_nosys },			/* 166 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 167 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 168 = unimplemented */
#if defined(SYSVSEM) && !defined(alpha)
	{ 5, s(struct compat_10_sys_semsys_args),
	    compat_10_sys_semsys },		/* 169 = osemsys */
#else
	{ 0, 0,
	    sys_nosys },			/* 169 = unimplemented 1.0 semsys */
#endif
#if defined(SYSVMSG) && !defined(alpha)
	{ 6, s(struct compat_10_sys_msgsys_args),
	    compat_10_sys_msgsys },		/* 170 = omsgsys */
#else
	{ 0, 0,
	    sys_nosys },			/* 170 = unimplemented 1.0 msgsys */
#endif
#if defined(SYSVSHM) && !defined(alpha)
	{ 4, s(struct compat_10_sys_shmsys_args),
	    compat_10_sys_shmsys },		/* 171 = oshmsys */
#else
	{ 0, 0,
	    sys_nosys },			/* 171 = unimplemented 1.0 shmsys */
#endif
	{ 0, 0,
	    sys_nosys },			/* 172 = unimplemented */
	{ 5, s(struct compat_sparc32_pread_args),
	    compat_sparc32_pread },		/* 173 = compat_sparc32_pread */
	{ 5, s(struct compat_sparc32_pwrite_args),
	    compat_sparc32_pwrite },		/* 174 = compat_sparc32_pwrite */
	{ 1, s(struct compat_sparc32_ntp_gettime_args),
	    compat_sparc32_ntp_gettime },	/* 175 = compat_sparc32_ntp_gettime */
	{ 1, s(struct compat_sparc32_ntp_adjtime_args),
	    compat_sparc32_ntp_adjtime },	/* 176 = compat_sparc32_ntp_adjtime */
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
	{ 3, s(struct compat_sparc32_lfs_bmapv_args),
	    compat_sparc32_lfs_bmapv },		/* 184 = compat_sparc32_lfs_bmapv */
	{ 3, s(struct compat_sparc32_lfs_markv_args),
	    compat_sparc32_lfs_markv },		/* 185 = compat_sparc32_lfs_markv */
	{ 2, s(struct compat_sparc32_lfs_segclean_args),
	    compat_sparc32_lfs_segclean },	/* 186 = compat_sparc32_lfs_segclean */
	{ 2, s(struct compat_sparc32_lfs_segwait_args),
	    compat_sparc32_lfs_segwait },	/* 187 = compat_sparc32_lfs_segwait */
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
	{ 2, s(struct compat_12_compat_sparc32_stat12_args),
	    compat_12(compat_sparc32_stat12) },	/* 188 = compat_12 compat_sparc32_stat12 */
	{ 2, s(struct compat_12_compat_sparc32_fstat12_args),
	    compat_12(compat_sparc32_fstat12) },/* 189 = compat_12 compat_sparc32_fstat12 */
	{ 2, s(struct compat_12_compat_sparc32_lstat12_args),
	    compat_12(compat_sparc32_lstat12) },/* 190 = compat_12 compat_sparc32_lstat12 */
	{ 2, s(struct compat_sparc32_pathconf_args),
	    compat_sparc32_pathconf },		/* 191 = compat_sparc32_pathconf */
	{ 2, s(struct compat_sparc32_fpathconf_args),
	    compat_sparc32_fpathconf },		/* 192 = compat_sparc32_fpathconf */
	{ 0, 0,
	    sys_nosys },			/* 193 = unimplemented */
	{ 2, s(struct compat_sparc32_getrlimit_args),
	    compat_sparc32_getrlimit },		/* 194 = compat_sparc32_getrlimit */
	{ 2, s(struct compat_sparc32_setrlimit_args),
	    compat_sparc32_setrlimit },		/* 195 = compat_sparc32_setrlimit */
	{ 4, s(struct compat_12_compat_sparc32_getdirentries_args),
	    compat_12(compat_sparc32_getdirentries) },/* 196 = compat_12 compat_sparc32_getdirentries */
	{ 7, s(struct compat_sparc32_mmap_args),
	    compat_sparc32_mmap },		/* 197 = compat_sparc32_mmap */
	{ 0, 0,
	    sys_nosys },			/* 198 = __syscall (indir) */
	{ 4, s(struct sys_lseek_args),
	    sys_lseek },			/* 199 = lseek */
	{ 3, s(struct compat_sparc32_truncate_args),
	    compat_sparc32_truncate },		/* 200 = compat_sparc32_truncate */
	{ 3, s(struct sys_ftruncate_args),
	    sys_ftruncate },			/* 201 = ftruncate */
	{ 6, s(struct compat_sparc32___sysctl_args),
	    compat_sparc32___sysctl },		/* 202 = compat_sparc32___sysctl */
	{ 2, s(struct compat_sparc32_mlock_args),
	    compat_sparc32_mlock },		/* 203 = compat_sparc32_mlock */
	{ 2, s(struct compat_sparc32_munlock_args),
	    compat_sparc32_munlock },		/* 204 = compat_sparc32_munlock */
	{ 1, s(struct compat_sparc32_undelete_args),
	    compat_sparc32_undelete },		/* 205 = compat_sparc32_undelete */
	{ 2, s(struct compat_sparc32_futimes_args),
	    compat_sparc32_futimes },		/* 206 = compat_sparc32_futimes */
	{ 1, s(struct sys_getpgid_args),
	    sys_getpgid },			/* 207 = getpgid */
	{ 2, s(struct compat_sparc32_reboot_args),
	    compat_sparc32_reboot },		/* 208 = compat_sparc32_reboot */
	{ 3, s(struct compat_sparc32_poll_args),
	    compat_sparc32_poll },		/* 209 = compat_sparc32_poll */
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
	{ 4, s(struct compat_sparc32___semctl_args),
	    compat_sparc32___semctl },		/* 220 = compat_sparc32___semctl */
	{ 3, s(struct compat_sparc32_semget_args),
	    compat_sparc32_semget },		/* 221 = compat_sparc32_semget */
	{ 3, s(struct compat_sparc32_semop_args),
	    compat_sparc32_semop },		/* 222 = compat_sparc32_semop */
	{ 1, s(struct sys_semconfig_args),
	    sys_semconfig },			/* 223 = semconfig */
#else
	{ 0, 0,
	    sys_nosys },			/* 220 = unimplemented compat_sparc32_semctl */
	{ 0, 0,
	    sys_nosys },			/* 221 = unimplemented compat_sparc32_semget */
	{ 0, 0,
	    sys_nosys },			/* 222 = unimplemented compat_sparc32_semop */
	{ 0, 0,
	    sys_nosys },			/* 223 = unimplemented compat_sparc32_semconfig */
#endif
#ifdef SYSVMSG
	{ 3, s(struct compat_sparc32_msgctl_args),
	    compat_sparc32_msgctl },		/* 224 = compat_sparc32_msgctl */
	{ 2, s(struct compat_sparc32_msgget_args),
	    compat_sparc32_msgget },		/* 225 = compat_sparc32_msgget */
	{ 4, s(struct compat_sparc32_msgsnd_args),
	    compat_sparc32_msgsnd },		/* 226 = compat_sparc32_msgsnd */
	{ 5, s(struct compat_sparc32_msgrcv_args),
	    compat_sparc32_msgrcv },		/* 227 = compat_sparc32_msgrcv */
#else
	{ 0, 0,
	    sys_nosys },			/* 224 = unimplemented compat_sparc32_msgctl */
	{ 0, 0,
	    sys_nosys },			/* 225 = unimplemented compat_sparc32_msgget */
	{ 0, 0,
	    sys_nosys },			/* 226 = unimplemented compat_sparc32_msgsnd */
	{ 0, 0,
	    sys_nosys },			/* 227 = unimplemented compat_sparc32_msgrcv */
#endif
#ifdef SYSVSHM
	{ 3, s(struct compat_sparc32_shmat_args),
	    compat_sparc32_shmat },		/* 228 = compat_sparc32_shmat */
	{ 3, s(struct compat_sparc32_shmctl_args),
	    compat_sparc32_shmctl },		/* 229 = compat_sparc32_shmctl */
	{ 1, s(struct compat_sparc32_shmdt_args),
	    compat_sparc32_shmdt },		/* 230 = compat_sparc32_shmdt */
	{ 3, s(struct compat_sparc32_shmget_args),
	    compat_sparc32_shmget },		/* 231 = compat_sparc32_shmget */
#else
	{ 0, 0,
	    sys_nosys },			/* 228 = unimplemented compat_sparc32_shmat */
	{ 0, 0,
	    sys_nosys },			/* 229 = unimplemented compat_sparc32_shmctl */
	{ 0, 0,
	    sys_nosys },			/* 230 = unimplemented compat_sparc32_shmdt */
	{ 0, 0,
	    sys_nosys },			/* 231 = unimplemented compat_sparc32_shmget */
#endif
	{ 2, s(struct compat_sparc32_clock_gettime_args),
	    compat_sparc32_clock_gettime },	/* 232 = compat_sparc32_clock_gettime */
	{ 2, s(struct compat_sparc32_clock_settime_args),
	    compat_sparc32_clock_settime },	/* 233 = compat_sparc32_clock_settime */
	{ 2, s(struct compat_sparc32_clock_getres_args),
	    compat_sparc32_clock_getres },	/* 234 = compat_sparc32_clock_getres */
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
	{ 2, s(struct compat_sparc32_nanosleep_args),
	    compat_sparc32_nanosleep },		/* 240 = compat_sparc32_nanosleep */
	{ 1, s(struct sys_fdatasync_args),
	    sys_fdatasync },			/* 241 = fdatasync */
	{ 0, 0,
	    sys_nosys },			/* 242 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 243 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 244 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 245 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 246 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 247 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 248 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 249 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 250 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 251 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 252 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 253 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 254 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 255 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 256 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 257 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 258 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 259 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 260 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 261 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 262 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 263 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 264 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 265 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 266 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 267 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 268 = unimplemented */
	{ 0, 0,
	    sys_nosys },			/* 269 = unimplemented */
	{ 2, s(struct compat_sparc32___posix_rename_args),
	    compat_sparc32___posix_rename },	/* 270 = compat_sparc32___posix_rename */
	{ 3, s(struct compat_sparc32_swapctl_args),
	    compat_sparc32_swapctl },		/* 271 = compat_sparc32_swapctl */
	{ 3, s(struct compat_sparc32_getdents_args),
	    compat_sparc32_getdents },		/* 272 = compat_sparc32_getdents */
	{ 3, s(struct compat_sparc32_minherit_args),
	    compat_sparc32_minherit },		/* 273 = compat_sparc32_minherit */
	{ 2, s(struct compat_sparc32_lchmod_args),
	    compat_sparc32_lchmod },		/* 274 = compat_sparc32_lchmod */
	{ 3, s(struct compat_sparc32_lchown_args),
	    compat_sparc32_lchown },		/* 275 = compat_sparc32_lchown */
	{ 2, s(struct compat_sparc32_lutimes_args),
	    compat_sparc32_lutimes },		/* 276 = compat_sparc32_lutimes */
	{ 3, s(struct compat_sparc32___msync13_args),
	    compat_sparc32___msync13 },		/* 277 = compat_sparc32___msync13 */
	{ 2, s(struct compat_sparc32___stat13_args),
	    compat_sparc32___stat13 },		/* 278 = compat_sparc32___stat13 */
	{ 2, s(struct compat_sparc32___fstat13_args),
	    compat_sparc32___fstat13 },		/* 279 = compat_sparc32___fstat13 */
	{ 2, s(struct compat_sparc32___lstat13_args),
	    compat_sparc32___lstat13 },		/* 280 = compat_sparc32___lstat13 */
	{ 2, s(struct compat_sparc32___sigaltstack14_args),
	    compat_sparc32___sigaltstack14 },	/* 281 = compat_sparc32___sigaltstack14 */
	{ 0, 0,
	    sys___vfork14 },			/* 282 = __vfork14 */
	{ 3, s(struct compat_sparc32___posix_chown_args),
	    compat_sparc32___posix_chown },	/* 283 = compat_sparc32___posix_chown */
	{ 3, s(struct sys___posix_fchown_args),
	    sys___posix_fchown },		/* 284 = __posix_fchown */
	{ 3, s(struct compat_sparc32___posix_lchown_args),
	    compat_sparc32___posix_lchown },	/* 285 = compat_sparc32___posix_lchown */
	{ 1, s(struct sys_getsid_args),
	    sys_getsid },			/* 286 = getsid */
	{ 0, 0,
	    sys_nosys },			/* 287 = unimplemented */
#ifdef KTRACE
	{ 4, s(struct sys_fktrace_args),
	    sys_fktrace },			/* 288 = fktrace */
#else
	{ 0, 0,
	    sys_nosys },			/* 288 = unimplemented */
#endif
	{ 5, s(struct compat_sparc32_preadv_args),
	    compat_sparc32_preadv },		/* 289 = compat_sparc32_preadv */
	{ 5, s(struct compat_sparc32_pwritev_args),
	    compat_sparc32_pwritev },		/* 290 = compat_sparc32_pwritev */
};


/* $NetBSD: sunos_sysent.c,v 1.75 2008/06/21 13:22:04 tsutsui Exp $ */

/*
 * System call switch table.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.69 2008/06/21 13:20:46 tsutsui Exp
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: sunos_sysent.c,v 1.75 2008/06/21 13:22:04 tsutsui Exp $");

#if defined(_KERNEL_OPT)
#include "opt_nfsserver.h"
#include "opt_sysv.h"
#include "fs_nfs.h"
#endif
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/poll.h>
#include <sys/sched.h>
#include <sys/syscallargs.h>
#include <compat/sunos/sunos.h>
#include <compat/sunos/sunos_syscallargs.h>

#define	s(type)	sizeof(type)
#define	n(type)	(sizeof(type)/sizeof (register_t))
#define	ns(type)	n(type), s(type)

struct sysent sunos_sysent[] = {
	{ 0, 0, 0,
	    (sy_call_t *)sys_nosys },		/* 0 = syscall */
	{ ns(struct sys_exit_args), 0,
	    (sy_call_t *)sys_exit },		/* 1 = exit */
	{ 0, 0, 0,
	    (sy_call_t *)sys_fork },		/* 2 = fork */
	{ ns(struct sys_read_args), 0,
	    (sy_call_t *)sys_read },		/* 3 = read */
	{ ns(struct sys_write_args), 0,
	    (sy_call_t *)sys_write },		/* 4 = write */
	{ ns(struct sunos_sys_open_args), 0,
	    (sy_call_t *)sunos_sys_open },	/* 5 = open */
	{ ns(struct sys_close_args), 0,
	    (sy_call_t *)sys_close },		/* 6 = close */
	{ ns(struct sunos_sys_wait4_args), 0,
	    (sy_call_t *)sunos_sys_wait4 },	/* 7 = wait4 */
	{ ns(struct sunos_sys_creat_args), 0,
	    (sy_call_t *)sunos_sys_creat },	/* 8 = creat */
	{ ns(struct sys_link_args), 0,
	    (sy_call_t *)sys_link },		/* 9 = link */
	{ ns(struct sys_unlink_args), 0,
	    (sy_call_t *)sys_unlink },		/* 10 = unlink */
	{ ns(struct sunos_sys_execv_args), 0,
	    (sy_call_t *)sunos_sys_execv },	/* 11 = execv */
	{ ns(struct sys_chdir_args), 0,
	    (sy_call_t *)sys_chdir },		/* 12 = chdir */
	{ 0, 0, 0,
	    sys_nosys },			/* 13 = obsolete old_time */
	{ ns(struct sunos_sys_mknod_args), 0,
	    (sy_call_t *)sunos_sys_mknod },	/* 14 = mknod */
	{ ns(struct sys_chmod_args), 0,
	    (sy_call_t *)sys_chmod },		/* 15 = chmod */
	{ ns(struct sys_chown_args), 0,
	    (sy_call_t *)sys_chown },		/* 16 = chown */
	{ ns(struct sys_obreak_args), 0,
	    (sy_call_t *)sys_obreak },		/* 17 = break */
	{ 0, 0, 0,
	    sys_nosys },			/* 18 = obsolete old_stat */
	{ ns(struct compat_43_sys_lseek_args), 0,
	    (sy_call_t *)compat_43_sys_lseek },	/* 19 = lseek */
	{ 0, 0, 0,
	    (sy_call_t *)sys_getpid_with_ppid },/* 20 = getpid_with_ppid */
	{ 0, 0, 0,
	    sys_nosys },			/* 21 = obsolete sunos_old_mount */
	{ 0, 0, 0,
	    sys_nosys },			/* 22 = unimplemented System V umount */
	{ ns(struct sys_setuid_args), 0,
	    (sy_call_t *)sys_setuid },		/* 23 = setuid */
	{ 0, 0, 0,
	    (sy_call_t *)sys_getuid_with_euid },/* 24 = getuid_with_euid */
	{ ns(struct sunos_sys_stime_args), 0,
	    (sy_call_t *)sunos_sys_stime },	/* 25 = stime */
	{ ns(struct sunos_sys_ptrace_args), 0,
	    (sy_call_t *)sunos_sys_ptrace },	/* 26 = ptrace */
	{ 0, 0, 0,
	    sys_nosys },			/* 27 = unimplemented old_sunos_alarm */
	{ 0, 0, 0,
	    sys_nosys },			/* 28 = unimplemented old_sunos_fstat */
	{ 0, 0, 0,
	    sys_nosys },			/* 29 = unimplemented old_sunos_pause */
	{ 0, 0, 0,
	    sys_nosys },			/* 30 = unimplemented old_sunos_utime */
	{ 0, 0, 0,
	    sys_nosys },			/* 31 = unimplemented old_sunos_stty */
	{ 0, 0, 0,
	    sys_nosys },			/* 32 = unimplemented old_sunos_gtty */
	{ ns(struct sys_access_args), 0,
	    (sy_call_t *)sys_access },		/* 33 = access */
	{ 0, 0, 0,
	    sys_nosys },			/* 34 = unimplemented old_sunos_nice */
	{ 0, 0, 0,
	    sys_nosys },			/* 35 = unimplemented old_sunos_ftime */
	{ 0, 0, 0,
	    (sy_call_t *)sys_sync },		/* 36 = sync */
	{ ns(struct sys_kill_args), 0,
	    (sy_call_t *)sys_kill },		/* 37 = kill */
	{ ns(struct compat_43_sys_stat_args), 0,
	    (sy_call_t *)compat_43_sys_stat },	/* 38 = stat */
	{ 0, 0, 0,
	    sys_nosys },			/* 39 = unimplemented sunos_setpgrp */
	{ ns(struct compat_43_sys_lstat_args), 0,
	    (sy_call_t *)compat_43_sys_lstat },	/* 40 = lstat */
	{ ns(struct sys_dup_args), 0,
	    (sy_call_t *)sys_dup },		/* 41 = dup */
	{ 0, 0, 0,
	    (sy_call_t *)sys_pipe },		/* 42 = pipe */
	{ 0, 0, 0,
	    sys_nosys },			/* 43 = unimplemented sunos_times */
	{ ns(struct sys_profil_args), 0,
	    (sy_call_t *)sys_profil },		/* 44 = profil */
	{ 0, 0, 0,
	    sys_nosys },			/* 45 = unimplemented */
	{ ns(struct sys_setgid_args), 0,
	    (sy_call_t *)sys_setgid },		/* 46 = setgid */
	{ 0, 0, 0,
	    (sy_call_t *)sys_getgid_with_egid },/* 47 = getgid_with_egid */
	{ 0, 0, 0,
	    sys_nosys },			/* 48 = unimplemented sunos_ssig */
	{ 0, 0, 0,
	    sys_nosys },			/* 49 = unimplemented reserved for USG */
	{ 0, 0, 0,
	    sys_nosys },			/* 50 = unimplemented reserved for USG */
	{ ns(struct sys_acct_args), 0,
	    (sy_call_t *)sys_acct },		/* 51 = acct */
	{ 0, 0, 0,
	    sys_nosys },			/* 52 = unimplemented */
	{ ns(struct sunos_sys_mctl_args), 0,
	    (sy_call_t *)sunos_sys_mctl },	/* 53 = mctl */
	{ ns(struct sunos_sys_ioctl_args), 0,
	    (sy_call_t *)sunos_sys_ioctl },	/* 54 = ioctl */
	{ ns(struct sunos_sys_reboot_args), 0,
	    (sy_call_t *)sunos_sys_reboot },	/* 55 = reboot */
	{ 0, 0, 0,
	    sys_nosys },			/* 56 = obsolete sunos_owait3 */
	{ ns(struct sys_symlink_args), 0,
	    (sy_call_t *)sys_symlink },		/* 57 = symlink */
	{ ns(struct sys_readlink_args), 0,
	    (sy_call_t *)sys_readlink },	/* 58 = readlink */
	{ ns(struct sunos_sys_execve_args), 0,
	    (sy_call_t *)sunos_sys_execve },	/* 59 = execve */
	{ ns(struct sys_umask_args), 0,
	    (sy_call_t *)sys_umask },		/* 60 = umask */
	{ ns(struct sys_chroot_args), 0,
	    (sy_call_t *)sys_chroot },		/* 61 = chroot */
	{ ns(struct compat_43_sys_fstat_args), 0,
	    (sy_call_t *)compat_43_sys_fstat },	/* 62 = fstat */
	{ 0, 0, 0,
	    sys_nosys },			/* 63 = unimplemented */
	{ 0, 0, 0,
	    (sy_call_t *)compat_43_sys_getpagesize },/* 64 = getpagesize */
	{ ns(struct sunos_sys_omsync_args), 0,
	    (sy_call_t *)sunos_sys_omsync },	/* 65 = omsync */
	{ 0, 0, 0,
	    (sy_call_t *)sys_vfork },		/* 66 = vfork */
	{ 0, 0, 0,
	    sys_nosys },			/* 67 = obsolete vread */
	{ 0, 0, 0,
	    sys_nosys },			/* 68 = obsolete vwrite */
	{ ns(struct sys_sbrk_args), 0,
	    (sy_call_t *)sys_sbrk },		/* 69 = sbrk */
	{ ns(struct sys_sstk_args), 0,
	    (sy_call_t *)sys_sstk },		/* 70 = sstk */
	{ ns(struct sunos_sys_mmap_args), 0,
	    (sy_call_t *)sunos_sys_mmap },	/* 71 = mmap */
	{ ns(struct sys_ovadvise_args), 0,
	    (sy_call_t *)sys_ovadvise },	/* 72 = vadvise */
	{ ns(struct sys_munmap_args), 0,
	    (sy_call_t *)sys_munmap },		/* 73 = munmap */
	{ ns(struct sys_mprotect_args), 0,
	    (sy_call_t *)sys_mprotect },	/* 74 = mprotect */
	{ ns(struct sys_madvise_args), 0,
	    (sy_call_t *)sys_madvise },		/* 75 = madvise */
	{ 0, 0, 0,
	    (sy_call_t *)sunos_sys_vhangup },	/* 76 = vhangup */
	{ 0, 0, 0,
	    sys_nosys },			/* 77 = unimplemented vlimit */
	{ ns(struct sys_mincore_args), 0,
	    (sy_call_t *)sys_mincore },		/* 78 = mincore */
	{ ns(struct sys_getgroups_args), 0,
	    (sy_call_t *)sys_getgroups },	/* 79 = getgroups */
	{ ns(struct sys_setgroups_args), 0,
	    (sy_call_t *)sys_setgroups },	/* 80 = setgroups */
	{ 0, 0, 0,
	    (sy_call_t *)sys_getpgrp },		/* 81 = getpgrp */
	{ ns(struct sunos_sys_setpgrp_args), 0,
	    (sy_call_t *)sunos_sys_setpgrp },	/* 82 = setpgrp */
	{ ns(struct sys_setitimer_args), 0,
	    (sy_call_t *)sys_setitimer },	/* 83 = setitimer */
	{ 0, 0, 0,
	    sys_nosys },			/* 84 = unimplemented { int sunos_sys_wait ( void ) ; } */
	{ ns(struct compat_12_sys_swapon_args), 0,
	    (sy_call_t *)compat_12_sys_swapon },/* 85 = swapon */
	{ ns(struct sys_getitimer_args), 0,
	    (sy_call_t *)sys_getitimer },	/* 86 = getitimer */
	{ ns(struct compat_43_sys_gethostname_args), 0,
	    (sy_call_t *)compat_43_sys_gethostname },/* 87 = gethostname */
	{ ns(struct compat_43_sys_sethostname_args), 0,
	    (sy_call_t *)compat_43_sys_sethostname },/* 88 = sethostname */
	{ 0, 0, 0,
	    (sy_call_t *)compat_43_sys_getdtablesize },/* 89 = getdtablesize */
	{ ns(struct sys_dup2_args), 0,
	    (sy_call_t *)sys_dup2 },		/* 90 = dup2 */
	{ 0, 0, 0,
	    sys_nosys },			/* 91 = unimplemented getdopt */
	{ ns(struct sunos_sys_fcntl_args), 0,
	    (sy_call_t *)sunos_sys_fcntl },	/* 92 = fcntl */
	{ ns(struct sys_select_args), 0,
	    (sy_call_t *)sys_select },		/* 93 = select */
	{ 0, 0, 0,
	    sys_nosys },			/* 94 = unimplemented setdopt */
	{ ns(struct sys_fsync_args), 0,
	    (sy_call_t *)sys_fsync },		/* 95 = fsync */
	{ ns(struct sys_setpriority_args), 0,
	    (sy_call_t *)sys_setpriority },	/* 96 = setpriority */
	{ ns(struct sunos_sys_socket_args), 0,
	    (sy_call_t *)sunos_sys_socket },	/* 97 = socket */
	{ ns(struct sys_connect_args), 0,
	    (sy_call_t *)sys_connect },		/* 98 = connect */
	{ ns(struct compat_43_sys_accept_args), 0,
	    (sy_call_t *)compat_43_sys_accept },/* 99 = accept */
	{ ns(struct sys_getpriority_args), 0,
	    (sy_call_t *)sys_getpriority },	/* 100 = getpriority */
	{ ns(struct compat_43_sys_send_args), 0,
	    (sy_call_t *)compat_43_sys_send },	/* 101 = send */
	{ ns(struct compat_43_sys_recv_args), 0,
	    (sy_call_t *)compat_43_sys_recv },	/* 102 = recv */
	{ 0, 0, 0,
	    sys_nosys },			/* 103 = unimplemented old socketaddr */
	{ ns(struct sys_bind_args), 0,
	    (sy_call_t *)sys_bind },		/* 104 = bind */
	{ ns(struct sunos_sys_setsockopt_args), 0,
	    (sy_call_t *)sunos_sys_setsockopt },/* 105 = setsockopt */
	{ ns(struct sys_listen_args), 0,
	    (sy_call_t *)sys_listen },		/* 106 = listen */
	{ 0, 0, 0,
	    sys_nosys },			/* 107 = unimplemented vtimes */
	{ ns(struct sunos_sys_sigvec_args), 0,
	    (sy_call_t *)sunos_sys_sigvec },	/* 108 = sigvec */
	{ ns(struct compat_43_sys_sigblock_args), 0,
	    (sy_call_t *)compat_43_sys_sigblock },/* 109 = sigblock */
	{ ns(struct compat_43_sys_sigsetmask_args), 0,
	    (sy_call_t *)compat_43_sys_sigsetmask },/* 110 = sigsetmask */
	{ ns(struct sunos_sys_sigsuspend_args), 0,
	    (sy_call_t *)sunos_sys_sigsuspend },/* 111 = sigsuspend */
	{ ns(struct compat_43_sys_sigstack_args), 0,
	    (sy_call_t *)compat_43_sys_sigstack },/* 112 = sigstack */
	{ ns(struct compat_43_sys_recvmsg_args), 0,
	    (sy_call_t *)compat_43_sys_recvmsg },/* 113 = recvmsg */
	{ ns(struct compat_43_sys_sendmsg_args), 0,
	    (sy_call_t *)compat_43_sys_sendmsg },/* 114 = sendmsg */
	{ 0, 0, 0,
	    sys_nosys },			/* 115 = obsolete vtrace */
	{ ns(struct sys_gettimeofday_args), 0,
	    (sy_call_t *)sys_gettimeofday },	/* 116 = gettimeofday */
	{ ns(struct sys_getrusage_args), 0,
	    (sy_call_t *)sys_getrusage },	/* 117 = getrusage */
	{ ns(struct sys_getsockopt_args), 0,
	    (sy_call_t *)sys_getsockopt },	/* 118 = getsockopt */
	{ 0, 0, 0,
	    sys_nosys },			/* 119 = unimplemented */
	{ ns(struct sys_readv_args), 0,
	    (sy_call_t *)sys_readv },		/* 120 = readv */
	{ ns(struct sys_writev_args), 0,
	    (sy_call_t *)sys_writev },		/* 121 = writev */
	{ ns(struct sys_settimeofday_args), 0,
	    (sy_call_t *)sys_settimeofday },	/* 122 = settimeofday */
	{ ns(struct sys_fchown_args), 0,
	    (sy_call_t *)sys_fchown },		/* 123 = fchown */
	{ ns(struct sys_fchmod_args), 0,
	    (sy_call_t *)sys_fchmod },		/* 124 = fchmod */
	{ ns(struct compat_43_sys_recvfrom_args), 0,
	    (sy_call_t *)compat_43_sys_recvfrom },/* 125 = recvfrom */
	{ ns(struct sys_setreuid_args), 0,
	    (sy_call_t *)sys_setreuid },	/* 126 = setreuid */
	{ ns(struct sys_setregid_args), 0,
	    (sy_call_t *)sys_setregid },	/* 127 = setregid */
	{ ns(struct sys_rename_args), 0,
	    (sy_call_t *)sys_rename },		/* 128 = rename */
	{ ns(struct compat_43_sys_truncate_args), 0,
	    (sy_call_t *)compat_43_sys_truncate },/* 129 = truncate */
	{ ns(struct compat_43_sys_ftruncate_args), 0,
	    (sy_call_t *)compat_43_sys_ftruncate },/* 130 = ftruncate */
	{ ns(struct sys_flock_args), 0,
	    (sy_call_t *)sys_flock },		/* 131 = flock */
	{ 0, 0, 0,
	    sys_nosys },			/* 132 = unimplemented */
	{ ns(struct sys_sendto_args), 0,
	    (sy_call_t *)sys_sendto },		/* 133 = sendto */
	{ ns(struct sys_shutdown_args), 0,
	    (sy_call_t *)sys_shutdown },	/* 134 = shutdown */
	{ ns(struct sunos_sys_socketpair_args), 0,
	    (sy_call_t *)sunos_sys_socketpair },/* 135 = socketpair */
	{ ns(struct sys_mkdir_args), 0,
	    (sy_call_t *)sys_mkdir },		/* 136 = mkdir */
	{ ns(struct sys_rmdir_args), 0,
	    (sy_call_t *)sys_rmdir },		/* 137 = rmdir */
	{ ns(struct sys_utimes_args), 0,
	    (sy_call_t *)sys_utimes },		/* 138 = utimes */
	{ ns(struct sunos_sys_sigreturn_args), 0,
	    (sy_call_t *)sunos_sys_sigreturn },	/* 139 = sigreturn */
	{ ns(struct sys_adjtime_args), 0,
	    (sy_call_t *)sys_adjtime },		/* 140 = adjtime */
	{ ns(struct compat_43_sys_getpeername_args), 0,
	    (sy_call_t *)compat_43_sys_getpeername },/* 141 = getpeername */
	{ 0, 0, 0,
	    (sy_call_t *)compat_43_sys_gethostid },/* 142 = gethostid */
	{ 0, 0, 0,
	    sys_nosys },			/* 143 = unimplemented old sethostid */
	{ ns(struct sunos_sys_getrlimit_args), 0,
	    (sy_call_t *)sunos_sys_getrlimit },	/* 144 = getrlimit */
	{ ns(struct sunos_sys_setrlimit_args), 0,
	    (sy_call_t *)sunos_sys_setrlimit },	/* 145 = setrlimit */
	{ ns(struct compat_43_sys_killpg_args), 0,
	    (sy_call_t *)compat_43_sys_killpg },/* 146 = killpg */
	{ 0, 0, 0,
	    sys_nosys },			/* 147 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 148 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 149 = unimplemented */
	{ ns(struct compat_43_sys_getsockname_args), 0,
	    (sy_call_t *)compat_43_sys_getsockname },/* 150 = getsockname */
	{ 0, 0, 0,
	    sys_nosys },			/* 151 = unimplemented getmsg */
	{ 0, 0, 0,
	    sys_nosys },			/* 152 = unimplemented putmsg */
	{ ns(struct sys_poll_args), 0,
	    (sy_call_t *)sys_poll },		/* 153 = poll */
	{ 0, 0, 0,
	    sys_nosys },			/* 154 = unimplemented */
#ifdef NFSSERVER
	{ ns(struct sunos_sys_nfssvc_args), 0,
	    (sy_call_t *)sunos_sys_nfssvc },	/* 155 = nfssvc */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 155 = unimplemented */
#endif
	{ ns(struct compat_12_sys_getdirentries_args), 0,
	    (sy_call_t *)compat_12_sys_getdirentries },/* 156 = getdirentries */
	{ ns(struct sunos_sys_statfs_args), 0,
	    (sy_call_t *)sunos_sys_statfs },	/* 157 = statfs */
	{ ns(struct sunos_sys_fstatfs_args), 0,
	    (sy_call_t *)sunos_sys_fstatfs },	/* 158 = fstatfs */
	{ ns(struct sunos_sys_unmount_args), 0,
	    (sy_call_t *)sunos_sys_unmount },	/* 159 = unmount */
#ifdef NFS
	{ 0, 0, 0,
	    (sy_call_t *)async_daemon },	/* 160 = async_daemon */
	{ ns(struct compat_30_sys_getfh_args), 0,
	    (sy_call_t *)compat_30_sys_getfh },	/* 161 = getfh */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 160 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 161 = unimplemented */
#endif
	{ ns(struct compat_09_sys_getdomainname_args), 0,
	    (sy_call_t *)compat_09_sys_getdomainname },/* 162 = getdomainname */
	{ ns(struct compat_09_sys_setdomainname_args), 0,
	    (sy_call_t *)compat_09_sys_setdomainname },/* 163 = setdomainname */
	{ 0, 0, 0,
	    sys_nosys },			/* 164 = unimplemented rtschedule */
	{ ns(struct sunos_sys_quotactl_args), 0,
	    (sy_call_t *)sunos_sys_quotactl },	/* 165 = quotactl */
	{ ns(struct sunos_sys_exportfs_args), 0,
	    (sy_call_t *)sunos_sys_exportfs },	/* 166 = exportfs */
	{ ns(struct sunos_sys_mount_args), 0,
	    (sy_call_t *)sunos_sys_mount },	/* 167 = mount */
	{ ns(struct sunos_sys_ustat_args), 0,
	    (sy_call_t *)sunos_sys_ustat },	/* 168 = ustat */
#ifdef SYSVSEM
	{ ns(struct compat_10_sys_semsys_args), 0,
	    (sy_call_t *)compat_10_sys_semsys },/* 169 = semsys */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 169 = unimplemented semsys */
#endif
#ifdef SYSVMSG
	{ ns(struct compat_10_sys_msgsys_args), 0,
	    (sy_call_t *)compat_10_sys_msgsys },/* 170 = msgsys */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 170 = unimplemented msgsys */
#endif
#ifdef SYSVSHM
	{ ns(struct compat_10_sys_shmsys_args), 0,
	    (sy_call_t *)compat_10_sys_shmsys },/* 171 = shmsys */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 171 = unimplemented shmsys */
#endif
	{ ns(struct sunos_sys_auditsys_args), 0,
	    (sy_call_t *)sunos_sys_auditsys },	/* 172 = auditsys */
	{ 0, 0, 0,
	    sys_nosys },			/* 173 = unimplemented rfssys */
	{ ns(struct sunos_sys_getdents_args), 0,
	    (sy_call_t *)sunos_sys_getdents },	/* 174 = getdents */
	{ 0, 0, 0,
	    (sy_call_t *)sys_setsid },		/* 175 = setsid */
	{ ns(struct sys_fchdir_args), 0,
	    (sy_call_t *)sys_fchdir },		/* 176 = fchdir */
	{ ns(struct sys_fchroot_args), 0,
	    (sy_call_t *)sys_fchroot },		/* 177 = fchroot */
	{ 0, 0, 0,
	    sys_nosys },			/* 178 = unimplemented vpixsys */
	{ 0, 0, 0,
	    sys_nosys },			/* 179 = unimplemented aioread */
	{ 0, 0, 0,
	    sys_nosys },			/* 180 = unimplemented aiowrite */
	{ 0, 0, 0,
	    sys_nosys },			/* 181 = unimplemented aiowait */
	{ 0, 0, 0,
	    sys_nosys },			/* 182 = unimplemented aiocancel */
	{ ns(struct sunos_sys_sigpending_args), 0,
	    (sy_call_t *)sunos_sys_sigpending },/* 183 = sigpending */
	{ 0, 0, 0,
	    sys_nosys },			/* 184 = unimplemented */
	{ ns(struct sys_setpgid_args), 0,
	    (sy_call_t *)sys_setpgid },		/* 185 = setpgid */
	{ ns(struct sys_pathconf_args), 0,
	    (sy_call_t *)sys_pathconf },	/* 186 = pathconf */
	{ ns(struct sys_fpathconf_args), 0,
	    (sy_call_t *)sys_fpathconf },	/* 187 = fpathconf */
	{ ns(struct sunos_sys_sysconf_args), 0,
	    (sy_call_t *)sunos_sys_sysconf },	/* 188 = sysconf */
	{ ns(struct sunos_sys_uname_args), 0,
	    (sy_call_t *)sunos_sys_uname },	/* 189 = uname */
	{ 0, 0, 0,
	    sys_nosys },			/* 190 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 191 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 192 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 193 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 194 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 195 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 196 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 197 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 198 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 199 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 200 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 201 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 202 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 203 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 204 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 205 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 206 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 207 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 208 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 209 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 210 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 211 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 212 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 213 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 214 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 215 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 216 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 217 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 218 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 219 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 220 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 221 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 222 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 223 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 224 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 225 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 226 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 227 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 228 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 229 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 230 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 231 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 232 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 233 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 234 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 235 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 236 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 237 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 238 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 239 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 240 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 241 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 242 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 243 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 244 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 245 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 246 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 247 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 248 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 249 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 250 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 251 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 252 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 253 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 254 = filler */
	{ 0, 0, 0,
	    sys_nosys },			/* 255 = filler */
};


/* $NetBSD: svr4_32_syscalls.c,v 1.11 2005/02/26 23:58:20 perry Exp $ */

/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.7 2003/12/07 01:36:58 dmcmahill Exp
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: svr4_32_syscalls.c,v 1.11 2005/02/26 23:58:20 perry Exp $");

#if defined(_KERNEL_OPT)
#if defined(_KERNEL_OPT)
#include "opt_ntp.h"
#include "opt_sysv.h"
#endif
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/poll.h>
#include <sys/sa.h>
#include <sys/syscallargs.h>
#include <compat/svr4_32/svr4_32_types.h>
#include <compat/netbsd32/netbsd32_syscallargs.h>
#include <compat/svr4_32/svr4_32_time.h>
#include <compat/svr4_32/svr4_32_signal.h>
#include <compat/svr4_32/svr4_32_ucontext.h>
#include <compat/svr4_32/svr4_32_lwp.h>
#include <compat/svr4_32/svr4_32_syscallargs.h>
#include <compat/svr4_32/svr4_32_statvfs.h>
#include <compat/svr4_32/svr4_32_resource.h>
#include <compat/svr4_32/svr4_32_acl.h>
#include <compat/svr4_32/svr4_32_schedctl.h>
#endif /* _KERNEL_OPT */

const char *const svr4_32_syscallnames[] = {
	"syscall",			/* 0 = syscall */
	"netbsd32_exit",			/* 1 = netbsd32_exit */
	"fork",			/* 2 = fork */
	"netbsd32_read",			/* 3 = netbsd32_read */
	"netbsd32_write",			/* 4 = netbsd32_write */
	"open",			/* 5 = open */
	"netbsd32_close",			/* 6 = netbsd32_close */
	"wait",			/* 7 = wait */
	"creat",			/* 8 = creat */
	"netbsd32_link",			/* 9 = netbsd32_link */
	"netbsd32_unlink",			/* 10 = netbsd32_unlink */
	"execv",			/* 11 = execv */
	"netbsd32_chdir",			/* 12 = netbsd32_chdir */
	"time",			/* 13 = time */
	"mknod",			/* 14 = mknod */
	"netbsd32_chmod",			/* 15 = netbsd32_chmod */
	"chown",			/* 16 = chown */
	"break",			/* 17 = break */
	"stat",			/* 18 = stat */
	"compat_43_netbsd32_olseek",			/* 19 = compat_43_netbsd32_olseek */
	"getpid",			/* 20 = getpid */
	"#21 (unimplemented old_mount)",		/* 21 = unimplemented old_mount */
	"#22 (unimplemented System V umount)",		/* 22 = unimplemented System V umount */
	"netbsd32_setuid",			/* 23 = netbsd32_setuid */
	"getuid_with_euid",			/* 24 = getuid_with_euid */
	"#25 (unimplemented stime)",		/* 25 = unimplemented stime */
	"#26 (unimplemented pcsample)",		/* 26 = unimplemented pcsample */
	"alarm",			/* 27 = alarm */
	"fstat",			/* 28 = fstat */
	"pause",			/* 29 = pause */
	"utime",			/* 30 = utime */
	"#31 (unimplemented was stty)",		/* 31 = unimplemented was stty */
	"#32 (unimplemented was gtty)",		/* 32 = unimplemented was gtty */
	"access",			/* 33 = access */
	"nice",			/* 34 = nice */
	"#35 (unimplemented statfs)",		/* 35 = unimplemented statfs */
	"sync",			/* 36 = sync */
	"kill",			/* 37 = kill */
	"#38 (unimplemented fstatfs)",		/* 38 = unimplemented fstatfs */
	"pgrpsys",			/* 39 = pgrpsys */
	"#40 (unimplemented xenix)",		/* 40 = unimplemented xenix */
	"netbsd32_dup",			/* 41 = netbsd32_dup */
	"pipe",			/* 42 = pipe */
	"times",			/* 43 = times */
	"#44 (unimplemented profil)",		/* 44 = unimplemented profil */
	"#45 (unimplemented plock)",		/* 45 = unimplemented plock */
	"netbsd32_setgid",			/* 46 = netbsd32_setgid */
	"getgid_with_egid",			/* 47 = getgid_with_egid */
	"signal",			/* 48 = signal */
#ifdef SYSVMSG
	"msgsys",			/* 49 = msgsys */
#else
	"#49 (unimplemented msgsys)",		/* 49 = unimplemented msgsys */
#endif
	"sysarch",			/* 50 = sysarch */
	"#51 (unimplemented acct)",		/* 51 = unimplemented acct */
#ifdef SYSVSHM
	"shmsys",			/* 52 = shmsys */
#else
	"#52 (unimplemented shmsys)",		/* 52 = unimplemented shmsys */
#endif
#ifdef SYSVSEM
	"semsys",			/* 53 = semsys */
#else
	"#53 (unimplemented semsys)",		/* 53 = unimplemented semsys */
#endif
	"ioctl",			/* 54 = ioctl */
	"#55 (unimplemented uadmin)",		/* 55 = unimplemented uadmin */
	"#56 (unimplemented exch)",		/* 56 = unimplemented exch */
	"utssys",			/* 57 = utssys */
	"netbsd32_fsync",			/* 58 = netbsd32_fsync */
	"netbsd32_execve",			/* 59 = netbsd32_execve */
	"netbsd32_umask",			/* 60 = netbsd32_umask */
	"netbsd32_chroot",			/* 61 = netbsd32_chroot */
	"fcntl",			/* 62 = fcntl */
	"ulimit",			/* 63 = ulimit */
	"#64 (unimplemented reserved for unix/pc)",		/* 64 = unimplemented reserved for unix/pc */
	"#65 (unimplemented reserved for unix/pc)",		/* 65 = unimplemented reserved for unix/pc */
	"#66 (unimplemented reserved for unix/pc)",		/* 66 = unimplemented reserved for unix/pc */
	"#67 (unimplemented reserved for unix/pc)",		/* 67 = unimplemented reserved for unix/pc */
	"#68 (unimplemented reserved for unix/pc)",		/* 68 = unimplemented reserved for unix/pc */
	"#69 (unimplemented reserved for unix/pc)",		/* 69 = unimplemented reserved for unix/pc */
	"#70 (obsolete advfs)",		/* 70 = obsolete advfs */
	"#71 (obsolete unadvfs)",		/* 71 = obsolete unadvfs */
	"#72 (obsolete rmount)",		/* 72 = obsolete rmount */
	"#73 (obsolete rumount)",		/* 73 = obsolete rumount */
	"#74 (obsolete rfstart)",		/* 74 = obsolete rfstart */
	"#75 (obsolete sigret)",		/* 75 = obsolete sigret */
	"#76 (obsolete rdebug)",		/* 76 = obsolete rdebug */
	"#77 (obsolete rfstop)",		/* 77 = obsolete rfstop */
	"#78 (unimplemented rfsys)",		/* 78 = unimplemented rfsys */
	"netbsd32_rmdir",			/* 79 = netbsd32_rmdir */
	"netbsd32_mkdir",			/* 80 = netbsd32_mkdir */
	"getdents",			/* 81 = getdents */
	"#82 (obsolete libattach)",		/* 82 = obsolete libattach */
	"#83 (obsolete libdetach)",		/* 83 = obsolete libdetach */
	"#84 (unimplemented sysfs)",		/* 84 = unimplemented sysfs */
	"getmsg",			/* 85 = getmsg */
	"putmsg",			/* 86 = putmsg */
	"netbsd32_poll",			/* 87 = netbsd32_poll */
	"lstat",			/* 88 = lstat */
	"netbsd32_symlink",			/* 89 = netbsd32_symlink */
	"netbsd32_readlink",			/* 90 = netbsd32_readlink */
	"netbsd32_getgroups",			/* 91 = netbsd32_getgroups */
	"netbsd32_setgroups",			/* 92 = netbsd32_setgroups */
	"netbsd32_fchmod",			/* 93 = netbsd32_fchmod */
	"fchown",			/* 94 = fchown */
	"sigprocmask",			/* 95 = sigprocmask */
	"sigsuspend",			/* 96 = sigsuspend */
	"sigaltstack",			/* 97 = sigaltstack */
	"sigaction",			/* 98 = sigaction */
	"sigpending",			/* 99 = sigpending */
	"context",			/* 100 = context */
	"#101 (unimplemented evsys)",		/* 101 = unimplemented evsys */
	"#102 (unimplemented evtrapret)",		/* 102 = unimplemented evtrapret */
	"statvfs",			/* 103 = statvfs */
	"fstatvfs",			/* 104 = fstatvfs */
	"#105 (unimplemented getloadavg)",		/* 105 = unimplemented getloadavg */
	"#106 (unimplemented nfssvc)",		/* 106 = unimplemented nfssvc */
	"waitsys",			/* 107 = waitsys */
	"#108 (unimplemented sigsendsys)",		/* 108 = unimplemented sigsendsys */
	"hrtsys",			/* 109 = hrtsys */
	"#110 (unimplemented acancel)",		/* 110 = unimplemented acancel */
	"#111 (unimplemented async)",		/* 111 = unimplemented async */
	"#112 (unimplemented priocntlsys)",		/* 112 = unimplemented priocntlsys */
	"pathconf",			/* 113 = pathconf */
	"#114 (unimplemented mincore)",		/* 114 = unimplemented mincore */
	"mmap",			/* 115 = mmap */
	"netbsd32_mprotect",			/* 116 = netbsd32_mprotect */
	"netbsd32_munmap",			/* 117 = netbsd32_munmap */
	"fpathconf",			/* 118 = fpathconf */
	"vfork",			/* 119 = vfork */
	"netbsd32_fchdir",			/* 120 = netbsd32_fchdir */
	"netbsd32_readv",			/* 121 = netbsd32_readv */
	"netbsd32_writev",			/* 122 = netbsd32_writev */
	"xstat",			/* 123 = xstat */
	"lxstat",			/* 124 = lxstat */
	"fxstat",			/* 125 = fxstat */
	"xmknod",			/* 126 = xmknod */
	"#127 (unimplemented clocal)",		/* 127 = unimplemented clocal */
	"setrlimit",			/* 128 = setrlimit */
	"getrlimit",			/* 129 = getrlimit */
	"lchown",			/* 130 = lchown */
	"memcntl",			/* 131 = memcntl */
	"#132 (unimplemented getpmsg)",		/* 132 = unimplemented getpmsg */
	"#133 (unimplemented putpmsg)",		/* 133 = unimplemented putpmsg */
	"rename",			/* 134 = rename */
	"uname",			/* 135 = uname */
	"netbsd32_setegid",			/* 136 = netbsd32_setegid */
	"sysconfig",			/* 137 = sysconfig */
	"netbsd32_adjtime",			/* 138 = netbsd32_adjtime */
	"systeminfo",			/* 139 = systeminfo */
	"#140 (unimplemented)",		/* 140 = unimplemented */
	"netbsd32_seteuid",			/* 141 = netbsd32_seteuid */
	"#142 (unimplemented vtrace)",		/* 142 = unimplemented vtrace */
	"fork1",			/* 143 = fork1 */
	"#144 (unimplemented sigtimedwait)",		/* 144 = unimplemented sigtimedwait */
	"_lwp_info",			/* 145 = _lwp_info */
	"#146 (unimplemented yield)",		/* 146 = unimplemented yield */
	"#147 (unimplemented lwp_sema_wait)",		/* 147 = unimplemented lwp_sema_wait */
	"#148 (unimplemented lwp_sema_post)",		/* 148 = unimplemented lwp_sema_post */
	"#149 (unimplemented lwp_sema_trywait)",		/* 149 = unimplemented lwp_sema_trywait */
	"#150 (unimplemented)",		/* 150 = unimplemented */
	"#151 (unimplemented corectl)",		/* 151 = unimplemented corectl */
	"#152 (unimplemented modctl)",		/* 152 = unimplemented modctl */
	"netbsd32_fchroot",			/* 153 = netbsd32_fchroot */
	"utimes",			/* 154 = utimes */
	"vhangup",			/* 155 = vhangup */
	"gettimeofday",			/* 156 = gettimeofday */
	"netbsd32_getitimer",			/* 157 = netbsd32_getitimer */
	"netbsd32_setitimer",			/* 158 = netbsd32_setitimer */
	"_lwp_create",			/* 159 = _lwp_create */
	"_lwp_exit",			/* 160 = _lwp_exit */
	"_lwp_suspend",			/* 161 = _lwp_suspend */
	"_lwp_continue",			/* 162 = _lwp_continue */
	"_lwp_kill",			/* 163 = _lwp_kill */
	"_lwp_self",			/* 164 = _lwp_self */
	"_lwp_getprivate",			/* 165 = _lwp_getprivate */
	"_lwp_setprivate",			/* 166 = _lwp_setprivate */
	"_lwp_wait",			/* 167 = _lwp_wait */
	"#168 (unimplemented lwp_mutex_wakeup)",		/* 168 = unimplemented lwp_mutex_wakeup */
	"#169 (unimplemented lwp_mutex_lock)",		/* 169 = unimplemented lwp_mutex_lock */
	"#170 (unimplemented lwp_cond_wait)",		/* 170 = unimplemented lwp_cond_wait */
	"#171 (unimplemented lwp_cond_signal)",		/* 171 = unimplemented lwp_cond_signal */
	"#172 (unimplemented lwp_cond_broadcast)",		/* 172 = unimplemented lwp_cond_broadcast */
	"pread",			/* 173 = pread */
	"pwrite",			/* 174 = pwrite */
	"llseek",			/* 175 = llseek */
	"#176 (unimplemented inst_sync)",		/* 176 = unimplemented inst_sync */
	"#177 (unimplemented srmlimitsys)",		/* 177 = unimplemented srmlimitsys */
	"#178 (unimplemented kaio)",		/* 178 = unimplemented kaio */
	"#179 (unimplemented cpc)",		/* 179 = unimplemented cpc */
	"#180 (unimplemented)",		/* 180 = unimplemented */
	"#181 (unimplemented)",		/* 181 = unimplemented */
	"#182 (unimplemented)",		/* 182 = unimplemented */
	"#183 (unimplemented)",		/* 183 = unimplemented */
	"#184 (unimplemented tsolsys)",		/* 184 = unimplemented tsolsys */
	"acl",			/* 185 = acl */
	"auditsys",			/* 186 = auditsys */
	"#187 (unimplemented processor_bind)",		/* 187 = unimplemented processor_bind */
	"#188 (unimplemented processor_info)",		/* 188 = unimplemented processor_info */
	"#189 (unimplemented p_online)",		/* 189 = unimplemented p_online */
	"#190 (unimplemented sigqueue)",		/* 190 = unimplemented sigqueue */
	"#191 (unimplemented clock_gettime)",		/* 191 = unimplemented clock_gettime */
	"#192 (unimplemented clock_settime)",		/* 192 = unimplemented clock_settime */
	"#193 (unimplemented clock_getres)",		/* 193 = unimplemented clock_getres */
	"#194 (unimplemented timer_create)",		/* 194 = unimplemented timer_create */
	"#195 (unimplemented timer_delete)",		/* 195 = unimplemented timer_delete */
	"#196 (unimplemented timer_settime)",		/* 196 = unimplemented timer_settime */
	"#197 (unimplemented timer_gettime)",		/* 197 = unimplemented timer_gettime */
	"#198 (unimplemented timer_getoverrun)",		/* 198 = unimplemented timer_getoverrun */
	"netbsd32_nanosleep",			/* 199 = netbsd32_nanosleep */
	"facl",			/* 200 = facl */
	"#201 (unimplemented door)",		/* 201 = unimplemented door */
	"netbsd32_setreuid",			/* 202 = netbsd32_setreuid */
	"netbsd32_setregid",			/* 203 = netbsd32_setregid */
	"#204 (unimplemented install_utrap)",		/* 204 = unimplemented install_utrap */
	"#205 (unimplemented signotify)",		/* 205 = unimplemented signotify */
	"schedctl",			/* 206 = schedctl */
	"#207 (unimplemented pset)",		/* 207 = unimplemented pset */
	"#208 (unimplemented sparc_utrap_install)",		/* 208 = unimplemented sparc_utrap_install */
	"resolvepath",			/* 209 = resolvepath */
	"#210 (unimplemented signotifywait)",		/* 210 = unimplemented signotifywait */
	"#211 (unimplemented lwp_sigredirect)",		/* 211 = unimplemented lwp_sigredirect */
	"#212 (unimplemented lwp_alarm)",		/* 212 = unimplemented lwp_alarm */
	"getdents64",			/* 213 = getdents64 */
	"mmap64",			/* 214 = mmap64 */
	"stat64",			/* 215 = stat64 */
	"lstat64",			/* 216 = lstat64 */
	"fstat64",			/* 217 = fstat64 */
	"statvfs64",			/* 218 = statvfs64 */
	"fstatvfs64",			/* 219 = fstatvfs64 */
	"setrlimit64",			/* 220 = setrlimit64 */
	"getrlimit64",			/* 221 = getrlimit64 */
	"pread64",			/* 222 = pread64 */
	"pwrite64",			/* 223 = pwrite64 */
	"creat64",			/* 224 = creat64 */
	"open64",			/* 225 = open64 */
	"#226 (unimplemented rpcsys)",		/* 226 = unimplemented rpcsys */
	"#227 (unimplemented)",		/* 227 = unimplemented */
	"#228 (unimplemented)",		/* 228 = unimplemented */
	"#229 (unimplemented)",		/* 229 = unimplemented */
	"socket",			/* 230 = socket */
	"netbsd32_socketpair",			/* 231 = netbsd32_socketpair */
	"netbsd32_bind",			/* 232 = netbsd32_bind */
	"netbsd32_listen",			/* 233 = netbsd32_listen */
	"compat_43_netbsd32_oaccept",			/* 234 = compat_43_netbsd32_oaccept */
	"netbsd32_connect",			/* 235 = netbsd32_connect */
	"netbsd32_shutdown",			/* 236 = netbsd32_shutdown */
	"compat_43_netbsd32_orecv",			/* 237 = compat_43_netbsd32_orecv */
	"compat_43_netbsd32_orecvfrom",			/* 238 = compat_43_netbsd32_orecvfrom */
	"compat_43_netbsd32_orecvmsg",			/* 239 = compat_43_netbsd32_orecvmsg */
	"compat_43_netbsd32_osend",			/* 240 = compat_43_netbsd32_osend */
	"compat_43_netbsd32_osendmsg",			/* 241 = compat_43_netbsd32_osendmsg */
	"netbsd32_sendto",			/* 242 = netbsd32_sendto */
	"compat_43_netbsd32_ogetpeername",			/* 243 = compat_43_netbsd32_ogetpeername */
	"compat_43_netbsd32_ogetsockname",			/* 244 = compat_43_netbsd32_ogetsockname */
	"netbsd32_getsockopt",			/* 245 = netbsd32_getsockopt */
	"netbsd32_setsockopt",			/* 246 = netbsd32_setsockopt */
	"#247 (unimplemented sockconfig)",		/* 247 = unimplemented sockconfig */
	"netbsd32_ntp_gettime",			/* 248 = netbsd32_ntp_gettime */
#if defined(NTP) || !defined(_KERNEL)
	"netbsd32_ntp_adjtime",			/* 249 = netbsd32_ntp_adjtime */
#else
	"#249 (excluded ntp_adjtime)",		/* 249 = excluded ntp_adjtime */
#endif
	"#250 (unimplemented lwp_mutex_unlock)",		/* 250 = unimplemented lwp_mutex_unlock */
	"#251 (unimplemented lwp_mutex_trylock)",		/* 251 = unimplemented lwp_mutex_trylock */
	"#252 (unimplemented lwp_mutex_init)",		/* 252 = unimplemented lwp_mutex_init */
	"#253 (unimplemented cladm)",		/* 253 = unimplemented cladm */
	"#254 (unimplemented lwp_sigtimedwait)",		/* 254 = unimplemented lwp_sigtimedwait */
	"#255 (unimplemented umount2)",		/* 255 = unimplemented umount2 */
};

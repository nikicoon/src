/* $NetBSD: syscalls.c,v 1.131 2003/01/18 10:08:01 thorpej Exp $ */

/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: syscalls.c,v 1.131 2003/01/18 10:08:01 thorpej Exp $");

#if defined(_KERNEL_OPT)
#include "opt_ktrace.h"
#include "opt_nfsserver.h"
#include "opt_ntp.h"
#include "opt_compat_netbsd.h"
#include "opt_sysv.h"
#include "opt_compat_43.h"
#include "fs_lfs.h"
#include "fs_nfs.h"
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/sa.h>
#include <sys/syscallargs.h>
#endif /* _KERNEL_OPT */

const char *const syscallnames[] = {
	"syscall",			/* 0 = syscall */
	"exit",			/* 1 = exit */
	"fork",			/* 2 = fork */
	"read",			/* 3 = read */
	"write",			/* 4 = write */
	"open",			/* 5 = open */
	"close",			/* 6 = close */
	"wait4",			/* 7 = wait4 */
	"compat_43_ocreat",	/* 8 = compat_43 ocreat */
	"link",			/* 9 = link */
	"unlink",			/* 10 = unlink */
	"#11 (obsolete execv)",		/* 11 = obsolete execv */
	"chdir",			/* 12 = chdir */
	"fchdir",			/* 13 = fchdir */
	"mknod",			/* 14 = mknod */
	"chmod",			/* 15 = chmod */
	"chown",			/* 16 = chown */
	"break",			/* 17 = break */
	"getfsstat",			/* 18 = getfsstat */
	"compat_43_olseek",	/* 19 = compat_43 olseek */
#ifdef COMPAT_43
	"getpid",			/* 20 = getpid */
#else
	"getpid",			/* 20 = getpid */
#endif
	"mount",			/* 21 = mount */
	"unmount",			/* 22 = unmount */
	"setuid",			/* 23 = setuid */
#ifdef COMPAT_43
	"getuid",			/* 24 = getuid */
#else
	"getuid",			/* 24 = getuid */
#endif
	"geteuid",			/* 25 = geteuid */
	"ptrace",			/* 26 = ptrace */
	"recvmsg",			/* 27 = recvmsg */
	"sendmsg",			/* 28 = sendmsg */
	"recvfrom",			/* 29 = recvfrom */
	"accept",			/* 30 = accept */
	"getpeername",			/* 31 = getpeername */
	"getsockname",			/* 32 = getsockname */
	"access",			/* 33 = access */
	"chflags",			/* 34 = chflags */
	"fchflags",			/* 35 = fchflags */
	"sync",			/* 36 = sync */
	"kill",			/* 37 = kill */
	"compat_43_stat43",	/* 38 = compat_43 stat43 */
	"getppid",			/* 39 = getppid */
	"compat_43_lstat43",	/* 40 = compat_43 lstat43 */
	"dup",			/* 41 = dup */
	"pipe",			/* 42 = pipe */
	"getegid",			/* 43 = getegid */
	"profil",			/* 44 = profil */
#if defined(KTRACE) || !defined(_KERNEL)
	"ktrace",			/* 45 = ktrace */
#else
	"#45 (excluded ktrace)",		/* 45 = excluded ktrace */
#endif
	"compat_13_sigaction13",	/* 46 = compat_13 sigaction13 */
#ifdef COMPAT_43
	"getgid",			/* 47 = getgid */
#else
	"getgid",			/* 47 = getgid */
#endif
	"compat_13_sigprocmask13",	/* 48 = compat_13 sigprocmask13 */
	"__getlogin",			/* 49 = __getlogin */
	"__setlogin",			/* 50 = __setlogin */
	"acct",			/* 51 = acct */
	"compat_13_sigpending13",	/* 52 = compat_13 sigpending13 */
	"compat_13_sigaltstack13",	/* 53 = compat_13 sigaltstack13 */
	"ioctl",			/* 54 = ioctl */
	"compat_12_oreboot",	/* 55 = compat_12 oreboot */
	"revoke",			/* 56 = revoke */
	"symlink",			/* 57 = symlink */
	"readlink",			/* 58 = readlink */
	"execve",			/* 59 = execve */
	"umask",			/* 60 = umask */
	"chroot",			/* 61 = chroot */
	"compat_43_fstat43",	/* 62 = compat_43 fstat43 */
	"compat_43_ogetkerninfo",	/* 63 = compat_43 ogetkerninfo */
	"compat_43_ogetpagesize",	/* 64 = compat_43 ogetpagesize */
	"compat_12_msync",	/* 65 = compat_12 msync */
	"vfork",			/* 66 = vfork */
	"#67 (obsolete vread)",		/* 67 = obsolete vread */
	"#68 (obsolete vwrite)",		/* 68 = obsolete vwrite */
	"sbrk",			/* 69 = sbrk */
	"sstk",			/* 70 = sstk */
	"compat_43_ommap",	/* 71 = compat_43 ommap */
	"vadvise",			/* 72 = vadvise */
	"munmap",			/* 73 = munmap */
	"mprotect",			/* 74 = mprotect */
	"madvise",			/* 75 = madvise */
	"#76 (obsolete vhangup)",		/* 76 = obsolete vhangup */
	"#77 (obsolete vlimit)",		/* 77 = obsolete vlimit */
	"mincore",			/* 78 = mincore */
	"getgroups",			/* 79 = getgroups */
	"setgroups",			/* 80 = setgroups */
	"getpgrp",			/* 81 = getpgrp */
	"setpgid",			/* 82 = setpgid */
	"setitimer",			/* 83 = setitimer */
	"compat_43_owait",	/* 84 = compat_43 owait */
	"compat_12_oswapon",	/* 85 = compat_12 oswapon */
	"getitimer",			/* 86 = getitimer */
	"compat_43_ogethostname",	/* 87 = compat_43 ogethostname */
	"compat_43_osethostname",	/* 88 = compat_43 osethostname */
	"compat_43_ogetdtablesize",	/* 89 = compat_43 ogetdtablesize */
	"dup2",			/* 90 = dup2 */
	"#91 (unimplemented getdopt)",		/* 91 = unimplemented getdopt */
	"fcntl",			/* 92 = fcntl */
	"select",			/* 93 = select */
	"#94 (unimplemented setdopt)",		/* 94 = unimplemented setdopt */
	"fsync",			/* 95 = fsync */
	"setpriority",			/* 96 = setpriority */
	"socket",			/* 97 = socket */
	"connect",			/* 98 = connect */
	"compat_43_oaccept",	/* 99 = compat_43 oaccept */
	"getpriority",			/* 100 = getpriority */
	"compat_43_osend",	/* 101 = compat_43 osend */
	"compat_43_orecv",	/* 102 = compat_43 orecv */
	"compat_13_sigreturn13",	/* 103 = compat_13 sigreturn13 */
	"bind",			/* 104 = bind */
	"setsockopt",			/* 105 = setsockopt */
	"listen",			/* 106 = listen */
	"#107 (obsolete vtimes)",		/* 107 = obsolete vtimes */
	"compat_43_osigvec",	/* 108 = compat_43 osigvec */
	"compat_43_osigblock",	/* 109 = compat_43 osigblock */
	"compat_43_osigsetmask",	/* 110 = compat_43 osigsetmask */
	"compat_13_sigsuspend13",	/* 111 = compat_13 sigsuspend13 */
	"compat_43_osigstack",	/* 112 = compat_43 osigstack */
	"compat_43_orecvmsg",	/* 113 = compat_43 orecvmsg */
	"compat_43_osendmsg",	/* 114 = compat_43 osendmsg */
	"#115 (obsolete vtrace)",		/* 115 = obsolete vtrace */
	"gettimeofday",			/* 116 = gettimeofday */
	"getrusage",			/* 117 = getrusage */
	"getsockopt",			/* 118 = getsockopt */
	"#119 (obsolete resuba)",		/* 119 = obsolete resuba */
	"readv",			/* 120 = readv */
	"writev",			/* 121 = writev */
	"settimeofday",			/* 122 = settimeofday */
	"fchown",			/* 123 = fchown */
	"fchmod",			/* 124 = fchmod */
	"compat_43_orecvfrom",	/* 125 = compat_43 orecvfrom */
	"setreuid",			/* 126 = setreuid */
	"setregid",			/* 127 = setregid */
	"rename",			/* 128 = rename */
	"compat_43_otruncate",	/* 129 = compat_43 otruncate */
	"compat_43_oftruncate",	/* 130 = compat_43 oftruncate */
	"flock",			/* 131 = flock */
	"mkfifo",			/* 132 = mkfifo */
	"sendto",			/* 133 = sendto */
	"shutdown",			/* 134 = shutdown */
	"socketpair",			/* 135 = socketpair */
	"mkdir",			/* 136 = mkdir */
	"rmdir",			/* 137 = rmdir */
	"utimes",			/* 138 = utimes */
	"#139 (obsolete 4.2 sigreturn)",		/* 139 = obsolete 4.2 sigreturn */
	"adjtime",			/* 140 = adjtime */
	"compat_43_ogetpeername",	/* 141 = compat_43 ogetpeername */
	"compat_43_ogethostid",	/* 142 = compat_43 ogethostid */
	"compat_43_osethostid",	/* 143 = compat_43 osethostid */
	"compat_43_ogetrlimit",	/* 144 = compat_43 ogetrlimit */
	"compat_43_osetrlimit",	/* 145 = compat_43 osetrlimit */
	"compat_43_okillpg",	/* 146 = compat_43 okillpg */
	"setsid",			/* 147 = setsid */
	"quotactl",			/* 148 = quotactl */
	"compat_43_oquota",	/* 149 = compat_43 oquota */
	"compat_43_ogetsockname",	/* 150 = compat_43 ogetsockname */
	"#151 (unimplemented)",		/* 151 = unimplemented */
	"#152 (unimplemented)",		/* 152 = unimplemented */
	"#153 (unimplemented)",		/* 153 = unimplemented */
	"#154 (unimplemented)",		/* 154 = unimplemented */
#if defined(NFS) || defined(NFSSERVER) || !defined(_KERNEL)
	"nfssvc",			/* 155 = nfssvc */
#else
	"#155 (excluded nfssvc)",		/* 155 = excluded nfssvc */
#endif
	"compat_43_ogetdirentries",	/* 156 = compat_43 ogetdirentries */
	"statfs",			/* 157 = statfs */
	"fstatfs",			/* 158 = fstatfs */
	"#159 (unimplemented)",		/* 159 = unimplemented */
	"#160 (unimplemented)",		/* 160 = unimplemented */
	"getfh",			/* 161 = getfh */
	"compat_09_ogetdomainname",	/* 162 = compat_09 ogetdomainname */
	"compat_09_osetdomainname",	/* 163 = compat_09 osetdomainname */
	"compat_09_ouname",	/* 164 = compat_09 ouname */
	"sysarch",			/* 165 = sysarch */
	"#166 (unimplemented)",		/* 166 = unimplemented */
	"#167 (unimplemented)",		/* 167 = unimplemented */
	"#168 (unimplemented)",		/* 168 = unimplemented */
#if (defined(SYSVSEM) || !defined(_KERNEL)) && !defined(_LP64)
	"compat_10_osemsys",	/* 169 = compat_10 osemsys */
#else
	"#169 (excluded 1.0 semsys)",		/* 169 = excluded 1.0 semsys */
#endif
#if (defined(SYSVMSG) || !defined(_KERNEL)) && !defined(_LP64)
	"compat_10_omsgsys",	/* 170 = compat_10 omsgsys */
#else
	"#170 (excluded 1.0 msgsys)",		/* 170 = excluded 1.0 msgsys */
#endif
#if (defined(SYSVSHM) || !defined(_KERNEL)) && !defined(_LP64)
	"compat_10_oshmsys",	/* 171 = compat_10 oshmsys */
#else
	"#171 (excluded 1.0 shmsys)",		/* 171 = excluded 1.0 shmsys */
#endif
	"#172 (unimplemented)",		/* 172 = unimplemented */
	"pread",			/* 173 = pread */
	"pwrite",			/* 174 = pwrite */
	"ntp_gettime",			/* 175 = ntp_gettime */
#if defined(NTP) || !defined(_KERNEL)
	"ntp_adjtime",			/* 176 = ntp_adjtime */
#else
	"#176 (excluded ntp_adjtime)",		/* 176 = excluded ntp_adjtime */
#endif
	"#177 (unimplemented)",		/* 177 = unimplemented */
	"#178 (unimplemented)",		/* 178 = unimplemented */
	"#179 (unimplemented)",		/* 179 = unimplemented */
	"#180 (unimplemented)",		/* 180 = unimplemented */
	"setgid",			/* 181 = setgid */
	"setegid",			/* 182 = setegid */
	"seteuid",			/* 183 = seteuid */
#if defined(LFS) || !defined(_KERNEL)
	"lfs_bmapv",			/* 184 = lfs_bmapv */
	"lfs_markv",			/* 185 = lfs_markv */
	"lfs_segclean",			/* 186 = lfs_segclean */
	"lfs_segwait",			/* 187 = lfs_segwait */
#else
	"#184 (excluded lfs_bmapv)",		/* 184 = excluded lfs_bmapv */
	"#185 (excluded lfs_markv)",		/* 185 = excluded lfs_markv */
	"#186 (excluded lfs_segclean)",		/* 186 = excluded lfs_segclean */
	"#187 (excluded lfs_segwait)",		/* 187 = excluded lfs_segwait */
#endif
	"compat_12_stat12",	/* 188 = compat_12 stat12 */
	"compat_12_fstat12",	/* 189 = compat_12 fstat12 */
	"compat_12_lstat12",	/* 190 = compat_12 lstat12 */
	"pathconf",			/* 191 = pathconf */
	"fpathconf",			/* 192 = fpathconf */
	"#193 (unimplemented)",		/* 193 = unimplemented */
	"getrlimit",			/* 194 = getrlimit */
	"setrlimit",			/* 195 = setrlimit */
	"compat_12_getdirentries",	/* 196 = compat_12 getdirentries */
	"mmap",			/* 197 = mmap */
	"__syscall",			/* 198 = __syscall */
	"lseek",			/* 199 = lseek */
	"truncate",			/* 200 = truncate */
	"ftruncate",			/* 201 = ftruncate */
	"__sysctl",			/* 202 = __sysctl */
	"mlock",			/* 203 = mlock */
	"munlock",			/* 204 = munlock */
	"undelete",			/* 205 = undelete */
	"futimes",			/* 206 = futimes */
	"getpgid",			/* 207 = getpgid */
	"reboot",			/* 208 = reboot */
	"poll",			/* 209 = poll */
#if defined(LKM) || !defined(_KERNEL)
	"lkmnosys",			/* 210 = lkmnosys */
	"lkmnosys",			/* 211 = lkmnosys */
	"lkmnosys",			/* 212 = lkmnosys */
	"lkmnosys",			/* 213 = lkmnosys */
	"lkmnosys",			/* 214 = lkmnosys */
	"lkmnosys",			/* 215 = lkmnosys */
	"lkmnosys",			/* 216 = lkmnosys */
	"lkmnosys",			/* 217 = lkmnosys */
	"lkmnosys",			/* 218 = lkmnosys */
	"lkmnosys",			/* 219 = lkmnosys */
#else	/* !LKM */
	"#210 (excluded lkmnosys)",		/* 210 = excluded lkmnosys */
	"#211 (excluded lkmnosys)",		/* 211 = excluded lkmnosys */
	"#212 (excluded lkmnosys)",		/* 212 = excluded lkmnosys */
	"#213 (excluded lkmnosys)",		/* 213 = excluded lkmnosys */
	"#214 (excluded lkmnosys)",		/* 214 = excluded lkmnosys */
	"#215 (excluded lkmnosys)",		/* 215 = excluded lkmnosys */
	"#216 (excluded lkmnosys)",		/* 216 = excluded lkmnosys */
	"#217 (excluded lkmnosys)",		/* 217 = excluded lkmnosys */
	"#218 (excluded lkmnosys)",		/* 218 = excluded lkmnosys */
	"#219 (excluded lkmnosys)",		/* 219 = excluded lkmnosys */
#endif	/* !LKM */
#if defined(SYSVSEM) || !defined(_KERNEL)
	"compat_14___semctl",	/* 220 = compat_14 __semctl */
	"semget",			/* 221 = semget */
	"semop",			/* 222 = semop */
	"semconfig",			/* 223 = semconfig */
#else
	"#220 (excluded compat_14_semctl)",		/* 220 = excluded compat_14_semctl */
	"#221 (excluded semget)",		/* 221 = excluded semget */
	"#222 (excluded semop)",		/* 222 = excluded semop */
	"#223 (excluded semconfig)",		/* 223 = excluded semconfig */
#endif
#if defined(SYSVMSG) || !defined(_KERNEL)
	"compat_14_msgctl",	/* 224 = compat_14 msgctl */
	"msgget",			/* 225 = msgget */
	"msgsnd",			/* 226 = msgsnd */
	"msgrcv",			/* 227 = msgrcv */
#else
	"#224 (excluded compat_14_msgctl)",		/* 224 = excluded compat_14_msgctl */
	"#225 (excluded msgget)",		/* 225 = excluded msgget */
	"#226 (excluded msgsnd)",		/* 226 = excluded msgsnd */
	"#227 (excluded msgrcv)",		/* 227 = excluded msgrcv */
#endif
#if defined(SYSVSHM) || !defined(_KERNEL)
	"shmat",			/* 228 = shmat */
	"compat_14_shmctl",	/* 229 = compat_14 shmctl */
	"shmdt",			/* 230 = shmdt */
	"shmget",			/* 231 = shmget */
#else
	"#228 (excluded shmat)",		/* 228 = excluded shmat */
	"#229 (excluded compat_14_shmctl)",		/* 229 = excluded compat_14_shmctl */
	"#230 (excluded shmdt)",		/* 230 = excluded shmdt */
	"#231 (excluded shmget)",		/* 231 = excluded shmget */
#endif
	"clock_gettime",			/* 232 = clock_gettime */
	"clock_settime",			/* 233 = clock_settime */
	"clock_getres",			/* 234 = clock_getres */
	"timer_create",			/* 235 = timer_create */
	"timer_delete",			/* 236 = timer_delete */
	"timer_settime",			/* 237 = timer_settime */
	"timer_gettime",			/* 238 = timer_gettime */
	"timer_getoverrun",			/* 239 = timer_getoverrun */
	"nanosleep",			/* 240 = nanosleep */
	"fdatasync",			/* 241 = fdatasync */
	"mlockall",			/* 242 = mlockall */
	"munlockall",			/* 243 = munlockall */
	"#244 (unimplemented sys_sigwaitinfo)",		/* 244 = unimplemented sys_sigwaitinfo */
	"#245 (unimplemented sys_sigtimedwait)",		/* 245 = unimplemented sys_sigtimedwait */
	"#246 (unimplemented sys_sigqueue)",		/* 246 = unimplemented sys_sigqueue */
	"#247 (unimplemented)",		/* 247 = unimplemented */
	"#248 (unimplemented)",		/* 248 = unimplemented */
	"#249 (unimplemented)",		/* 249 = unimplemented */
	"#250 (unimplemented)",		/* 250 = unimplemented */
	"#251 (unimplemented)",		/* 251 = unimplemented */
	"#252 (unimplemented)",		/* 252 = unimplemented */
	"#253 (unimplemented)",		/* 253 = unimplemented */
	"#254 (unimplemented)",		/* 254 = unimplemented */
	"#255 (unimplemented)",		/* 255 = unimplemented */
	"#256 (unimplemented)",		/* 256 = unimplemented */
	"#257 (unimplemented)",		/* 257 = unimplemented */
	"#258 (unimplemented)",		/* 258 = unimplemented */
	"#259 (unimplemented)",		/* 259 = unimplemented */
	"#260 (unimplemented)",		/* 260 = unimplemented */
	"#261 (unimplemented)",		/* 261 = unimplemented */
	"#262 (unimplemented)",		/* 262 = unimplemented */
	"#263 (unimplemented)",		/* 263 = unimplemented */
	"#264 (unimplemented)",		/* 264 = unimplemented */
	"#265 (unimplemented)",		/* 265 = unimplemented */
	"#266 (unimplemented)",		/* 266 = unimplemented */
	"#267 (unimplemented)",		/* 267 = unimplemented */
	"#268 (unimplemented)",		/* 268 = unimplemented */
	"#269 (unimplemented)",		/* 269 = unimplemented */
	"__posix_rename",			/* 270 = __posix_rename */
	"swapctl",			/* 271 = swapctl */
	"getdents",			/* 272 = getdents */
	"minherit",			/* 273 = minherit */
	"lchmod",			/* 274 = lchmod */
	"lchown",			/* 275 = lchown */
	"lutimes",			/* 276 = lutimes */
	"__msync13",			/* 277 = __msync13 */
	"__stat13",			/* 278 = __stat13 */
	"__fstat13",			/* 279 = __fstat13 */
	"__lstat13",			/* 280 = __lstat13 */
	"__sigaltstack14",			/* 281 = __sigaltstack14 */
	"__vfork14",			/* 282 = __vfork14 */
	"__posix_chown",			/* 283 = __posix_chown */
	"__posix_fchown",			/* 284 = __posix_fchown */
	"__posix_lchown",			/* 285 = __posix_lchown */
	"getsid",			/* 286 = getsid */
	"__clone",			/* 287 = __clone */
#if defined(KTRACE) || !defined(_KERNEL)
	"fktrace",			/* 288 = fktrace */
#else
	"#288 (excluded ktrace)",		/* 288 = excluded ktrace */
#endif
	"preadv",			/* 289 = preadv */
	"pwritev",			/* 290 = pwritev */
	"__sigaction14",			/* 291 = __sigaction14 */
	"__sigpending14",			/* 292 = __sigpending14 */
	"__sigprocmask14",			/* 293 = __sigprocmask14 */
	"__sigsuspend14",			/* 294 = __sigsuspend14 */
	"__sigreturn14",			/* 295 = __sigreturn14 */
	"__getcwd",			/* 296 = __getcwd */
	"fchroot",			/* 297 = fchroot */
	"fhopen",			/* 298 = fhopen */
	"fhstat",			/* 299 = fhstat */
	"fhstatfs",			/* 300 = fhstatfs */
#if defined(SYSVSEM) || !defined(_KERNEL)
	"____semctl13",			/* 301 = ____semctl13 */
#else
	"#301 (excluded ____semctl13)",		/* 301 = excluded ____semctl13 */
#endif
#if defined(SYSVMSG) || !defined(_KERNEL)
	"__msgctl13",			/* 302 = __msgctl13 */
#else
	"#302 (excluded __msgctl13)",		/* 302 = excluded __msgctl13 */
#endif
#if defined(SYSVSHM) || !defined(_KERNEL)
	"__shmctl13",			/* 303 = __shmctl13 */
#else
	"#303 (excluded __shmctl13)",		/* 303 = excluded __shmctl13 */
#endif
	"lchflags",			/* 304 = lchflags */
	"issetugid",			/* 305 = issetugid */
	"utrace",			/* 306 = utrace */
	"getcontext",			/* 307 = getcontext */
	"setcontext",			/* 308 = setcontext */
	"_lwp_create",			/* 309 = _lwp_create */
	"_lwp_exit",			/* 310 = _lwp_exit */
	"_lwp_self",			/* 311 = _lwp_self */
	"_lwp_wait",			/* 312 = _lwp_wait */
	"_lwp_suspend",			/* 313 = _lwp_suspend */
	"_lwp_continue",			/* 314 = _lwp_continue */
	"_lwp_wakeup",			/* 315 = _lwp_wakeup */
	"_lwp_getprivate",			/* 316 = _lwp_getprivate */
	"_lwp_setprivate",			/* 317 = _lwp_setprivate */
	"#318 (unimplemented)",		/* 318 = unimplemented */
	"#319 (unimplemented)",		/* 319 = unimplemented */
	"#320 (unimplemented)",		/* 320 = unimplemented */
	"#321 (unimplemented)",		/* 321 = unimplemented */
	"#322 (unimplemented)",		/* 322 = unimplemented */
	"#323 (unimplemented)",		/* 323 = unimplemented */
	"#324 (unimplemented)",		/* 324 = unimplemented */
	"#325 (unimplemented)",		/* 325 = unimplemented */
	"#326 (unimplemented)",		/* 326 = unimplemented */
	"#327 (unimplemented)",		/* 327 = unimplemented */
	"#328 (unimplemented)",		/* 328 = unimplemented */
	"#329 (unimplemented)",		/* 329 = unimplemented */
	"sa_register",			/* 330 = sa_register */
	"sa_stacks",			/* 331 = sa_stacks */
	"sa_enable",			/* 332 = sa_enable */
	"sa_setconcurrency",			/* 333 = sa_setconcurrency */
	"sa_yield",			/* 334 = sa_yield */
	"sa_preempt",			/* 335 = sa_preempt */
	"#336 (unimplemented)",		/* 336 = unimplemented */
	"#337 (unimplemented)",		/* 337 = unimplemented */
	"#338 (unimplemented)",		/* 338 = unimplemented */
	"#339 (unimplemented)",		/* 339 = unimplemented */
	"__sigaction_sigtramp",			/* 340 = __sigaction_sigtramp */
	"pmc_get_info",			/* 341 = pmc_get_info */
	"pmc_control",			/* 342 = pmc_control */
	"rasctl",			/* 343 = rasctl */
	"kqueue",			/* 344 = kqueue */
	"kevent",			/* 345 = kevent */
};

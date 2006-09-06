/* $NetBSD: linux32_syscalls.c,v 1.9 2006/09/06 12:01:23 manu Exp $ */

/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.5 2006/09/06 08:32:00 manu Exp
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: linux32_syscalls.c,v 1.9 2006/09/06 12:01:23 manu Exp $");

#if defined(_KERNEL_OPT)
#if defined(_KERNEL_OPT)
#include "opt_compat_43.h"
#endif
#include <sys/param.h>
#include <sys/poll.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/sa.h>
#include <sys/syscallargs.h>
#include <machine/netbsd32_machdep.h>
#include <compat/netbsd32/netbsd32.h>
#include <compat/netbsd32/netbsd32_syscallargs.h>
#include <compat/linux32/common/linux32_types.h>
#include <compat/linux32/common/linux32_signal.h>
#include <compat/linux32/arch/amd64/linux32_missing.h>
#include <compat/linux32/linux32_syscallargs.h>
#include <compat/linux/common/linux_types.h>
#include <compat/linux/common/linux_mmap.h>
#include <compat/linux/common/linux_signal.h>
#include <compat/linux/common/linux_siginfo.h>
#include <compat/linux/common/linux_machdep.h>
#include <compat/linux/linux_syscallargs.h>
#endif /* _KERNEL_OPT */

const char *const linux32_syscallnames[] = {
	"syscall",			/* 0 = syscall */
	"exit",			/* 1 = exit */
	"fork",			/* 2 = fork */
	"netbsd32_read",			/* 3 = netbsd32_read */
	"netbsd32_write",			/* 4 = netbsd32_write */
	"open",			/* 5 = open */
	"close",			/* 6 = close */
	"waitpid",			/* 7 = waitpid */
	"creat",			/* 8 = creat */
	"link",			/* 9 = link */
	"unlink",			/* 10 = unlink */
	"execve",			/* 11 = execve */
	"chdir",			/* 12 = chdir */
	"time",			/* 13 = time */
	"mknod",			/* 14 = mknod */
	"chmod",			/* 15 = chmod */
	"lchown16",			/* 16 = lchown16 */
	"break",			/* 17 = break */
	"#18 (obsolete ostat)",		/* 18 = obsolete ostat */
	"compat_43_netbsd32_olseek",			/* 19 = compat_43_netbsd32_olseek */
	"getpid",			/* 20 = getpid */
	"#21 (unimplemented mount)",		/* 21 = unimplemented mount */
	"#22 (unimplemented umount)",		/* 22 = unimplemented umount */
	"linux_setuid16",			/* 23 = linux_setuid16 */
	"linux_getuid16",			/* 24 = linux_getuid16 */
	"stime",			/* 25 = stime */
	"#26 (unimplemented ptrace)",		/* 26 = unimplemented ptrace */
	"alarm",			/* 27 = alarm */
	"#28 (obsolete ofstat)",		/* 28 = obsolete ofstat */
	"pause",			/* 29 = pause */
	"utime",			/* 30 = utime */
	"#31 (obsolete stty)",		/* 31 = obsolete stty */
	"#32 (obsolete gtty)",		/* 32 = obsolete gtty */
	"access",			/* 33 = access */
	"nice",			/* 34 = nice */
	"#35 (obsolete ftime)",		/* 35 = obsolete ftime */
	"sync",			/* 36 = sync */
	"kill",			/* 37 = kill */
	"rename",			/* 38 = rename */
	"mkdir",			/* 39 = mkdir */
	"rmdir",			/* 40 = rmdir */
	"netbsd32_dup",			/* 41 = netbsd32_dup */
	"pipe",			/* 42 = pipe */
	"times",			/* 43 = times */
	"#44 (obsolete prof)",		/* 44 = obsolete prof */
	"brk",			/* 45 = brk */
	"linux_setgid16",			/* 46 = linux_setgid16 */
	"linux_getgid16",			/* 47 = linux_getgid16 */
	"signal",			/* 48 = signal */
	"linux_geteuid16",			/* 49 = linux_geteuid16 */
	"linux_getegid16",			/* 50 = linux_getegid16 */
	"acct",			/* 51 = acct */
	"#52 (obsolete phys)",		/* 52 = obsolete phys */
	"#53 (obsolete lock)",		/* 53 = obsolete lock */
	"ioctl",			/* 54 = ioctl */
	"fcntl",			/* 55 = fcntl */
	"#56 (obsolete mpx)",		/* 56 = obsolete mpx */
	"setpgid",			/* 57 = setpgid */
	"#58 (obsolete ulimit)",		/* 58 = obsolete ulimit */
	"oldolduname",			/* 59 = oldolduname */
	"umask",			/* 60 = umask */
	"chroot",			/* 61 = chroot */
	"#62 (unimplemented ustat)",		/* 62 = unimplemented ustat */
	"netbsd32_dup2",			/* 63 = netbsd32_dup2 */
	"getppid",			/* 64 = getppid */
	"getpgrp",			/* 65 = getpgrp */
	"setsid",			/* 66 = setsid */
	"#67 (unimplemented sigaction)",		/* 67 = unimplemented sigaction */
	"#68 (unimplemented siggetmask)",		/* 68 = unimplemented siggetmask */
	"#69 (unimplemented sigsetmask)",		/* 69 = unimplemented sigsetmask */
	"setreuid16",			/* 70 = setreuid16 */
	"setregid16",			/* 71 = setregid16 */
	"#72 (unimplemented sigsuspend)",		/* 72 = unimplemented sigsuspend */
	"#73 (unimplemented sigpending)",		/* 73 = unimplemented sigpending */
	"compat_43_netbsd32_osethostname",			/* 74 = compat_43_netbsd32_osethostname */
	"setrlimit",			/* 75 = setrlimit */
	"getrlimit",			/* 76 = getrlimit */
	"getrusage",			/* 77 = getrusage */
	"gettimeofday",			/* 78 = gettimeofday */
	"settimeofday",			/* 79 = settimeofday */
	"getgroups16",			/* 80 = getgroups16 */
	"setgroups16",			/* 81 = setgroups16 */
	"oldselect",			/* 82 = oldselect */
	"symlink",			/* 83 = symlink */
	"lstat",			/* 84 = lstat */
	"readlink",			/* 85 = readlink */
	"#86 (unimplemented uselib)",		/* 86 = unimplemented uselib */
	"swapon",			/* 87 = swapon */
	"reboot",			/* 88 = reboot */
	"#89 (unimplemented readdir)",		/* 89 = unimplemented readdir */
	"old_mmap",			/* 90 = old_mmap */
	"munmap",			/* 91 = munmap */
	"truncate",			/* 92 = truncate */
	"ftruncate",			/* 93 = ftruncate */
	"fchmod",			/* 94 = fchmod */
	"fchown16",			/* 95 = fchown16 */
	"getpriority",			/* 96 = getpriority */
	"setpriority",			/* 97 = setpriority */
	"#98 (unimplemented profil)",		/* 98 = unimplemented profil */
	"#99 (unimplemented statfs)",		/* 99 = unimplemented statfs */
	"#100 (unimplemented fstatfs)",		/* 100 = unimplemented fstatfs */
	"#101 (unimplemented ioperm)",		/* 101 = unimplemented ioperm */
	"socketcall",			/* 102 = socketcall */
	"#103 (unimplemented syslog)",		/* 103 = unimplemented syslog */
	"setitimer",			/* 104 = setitimer */
	"getitimer",			/* 105 = getitimer */
	"#106 (unimplemented stat)",		/* 106 = unimplemented stat */
	"#107 (unimplemented lstat)",		/* 107 = unimplemented lstat */
	"#108 (unimplemented fstat)",		/* 108 = unimplemented fstat */
	"#109 (unimplemented olduname)",		/* 109 = unimplemented olduname */
	"#110 (unimplemented iopl)",		/* 110 = unimplemented iopl */
	"#111 (unimplemented vhangup)",		/* 111 = unimplemented vhangup */
	"#112 (unimplemented idle)",		/* 112 = unimplemented idle */
	"#113 (unimplemented vm86old)",		/* 113 = unimplemented vm86old */
	"wait4",			/* 114 = wait4 */
	"swapoff",			/* 115 = swapoff */
	"#116 (unimplemented sysinfo)",		/* 116 = unimplemented sysinfo */
	"#117 (unimplemented ipc)",		/* 117 = unimplemented ipc */
	"fsync",			/* 118 = fsync */
	"sigreturn",			/* 119 = sigreturn */
	"clone",			/* 120 = clone */
	"#121 (unimplemented setdomainname)",		/* 121 = unimplemented setdomainname */
	"uname",			/* 122 = uname */
	"#123 (unimplemented modify_ldt)",		/* 123 = unimplemented modify_ldt */
	"#124 (unimplemented adjtimex)",		/* 124 = unimplemented adjtimex */
	"mprotect",			/* 125 = mprotect */
	"#126 (unimplemented sigprocmask)",		/* 126 = unimplemented sigprocmask */
	"#127 (unimplemented create_module)",		/* 127 = unimplemented create_module */
	"#128 (unimplemented init_module)",		/* 128 = unimplemented init_module */
	"#129 (unimplemented delete_module)",		/* 129 = unimplemented delete_module */
	"#130 (unimplemented get_kernel_syms)",		/* 130 = unimplemented get_kernel_syms */
	"#131 (unimplemented quotactl)",		/* 131 = unimplemented quotactl */
	"#132 (unimplemented getpgid)",		/* 132 = unimplemented getpgid */
	"fchdir",			/* 133 = fchdir */
	"#134 (unimplemented bdflush)",		/* 134 = unimplemented bdflush */
	"#135 (unimplemented sysfs)",		/* 135 = unimplemented sysfs */
	"#136 (unimplemented personality)",		/* 136 = unimplemented personality */
	"#137 (unimplemented afs_syscall)",		/* 137 = unimplemented afs_syscall */
	"#138 (unimplemented int)",		/* 138 = unimplemented int */
	"#139 (unimplemented int)",		/* 139 = unimplemented int */
	"llseek",			/* 140 = llseek */
	"getdents",			/* 141 = getdents */
	"select",			/* 142 = select */
	"flock",			/* 143 = flock */
	"#144 (unimplemented msync)",		/* 144 = unimplemented msync */
	"readv",			/* 145 = readv */
	"writev",			/* 146 = writev */
	"getsid",			/* 147 = getsid */
	"fdatasync",			/* 148 = fdatasync */
	"__sysctl",			/* 149 = __sysctl */
	"#150 (unimplemented mlock)",		/* 150 = unimplemented mlock */
	"#151 (unimplemented munlock)",		/* 151 = unimplemented munlock */
	"mlockall",			/* 152 = mlockall */
	"munlockall",			/* 153 = munlockall */
	"#154 (unimplemented sched_setparam)",		/* 154 = unimplemented sched_setparam */
	"#155 (unimplemented sched_getparam)",		/* 155 = unimplemented sched_getparam */
	"sched_setscheduler",			/* 156 = sched_setscheduler */
	"#157 (unimplemented sched_getscheduler)",		/* 157 = unimplemented sched_getscheduler */
	"#158 (unimplemented sched_yield)",		/* 158 = unimplemented sched_yield */
	"#159 (unimplemented sched_get_priority_max)",		/* 159 = unimplemented sched_get_priority_max */
	"#160 (unimplemented sched_get_priority_min)",		/* 160 = unimplemented sched_get_priority_min */
	"#161 (unimplemented sched_rr_get_interval)",		/* 161 = unimplemented sched_rr_get_interval */
	"nanosleep",			/* 162 = nanosleep */
	"#163 (unimplemented * mremap)",		/* 163 = unimplemented * mremap */
	"setresuid16",			/* 164 = setresuid16 */
	"#165 (unimplemented getresuid16)",		/* 165 = unimplemented getresuid16 */
	"#166 (unimplemented vm86)",		/* 166 = unimplemented vm86 */
	"#167 (unimplemented query_module)",		/* 167 = unimplemented query_module */
	"poll",			/* 168 = poll */
	"#169 (unimplemented nfsservctl)",		/* 169 = unimplemented nfsservctl */
	"setresgid16",			/* 170 = setresgid16 */
	"#171 (unimplemented int)",		/* 171 = unimplemented int */
	"#172 (unimplemented prctl)",		/* 172 = unimplemented prctl */
	"rt_sigreturn",			/* 173 = rt_sigreturn */
	"rt_sigaction",			/* 174 = rt_sigaction */
	"rt_sigprocmask",			/* 175 = rt_sigprocmask */
	"#176 (unimplemented rt_sigpending)",		/* 176 = unimplemented rt_sigpending */
	"#177 (unimplemented rt_sigtimedwait)",		/* 177 = unimplemented rt_sigtimedwait */
	"#178 (unimplemented rt_queueinfo)",		/* 178 = unimplemented rt_queueinfo */
	"rt_sigsuspend",			/* 179 = rt_sigsuspend */
	"#180 (unimplemented pread)",		/* 180 = unimplemented pread */
	"#181 (unimplemented pwrite)",		/* 181 = unimplemented pwrite */
	"#182 (unimplemented chown16)",		/* 182 = unimplemented chown16 */
	"netbsd32___getcwd",			/* 183 = netbsd32___getcwd */
	"#184 (unimplemented capget)",		/* 184 = unimplemented capget */
	"#185 (unimplemented capset)",		/* 185 = unimplemented capset */
	"#186 (unimplemented sigaltstack)",		/* 186 = unimplemented sigaltstack */
	"#187 (unimplemented sendfile)",		/* 187 = unimplemented sendfile */
	"#188 (unimplemented getpmsg)",		/* 188 = unimplemented getpmsg */
	"#189 (unimplemented putpmsg)",		/* 189 = unimplemented putpmsg */
	"__vfork14",			/* 190 = __vfork14 */
	"ugetrlimit",			/* 191 = ugetrlimit */
	"#192 (unimplemented mmap2)",		/* 192 = unimplemented mmap2 */
	"#193 (unimplemented truncate64)",		/* 193 = unimplemented truncate64 */
	"#194 (unimplemented ftruncate64)",		/* 194 = unimplemented ftruncate64 */
	"stat64",			/* 195 = stat64 */
	"lstat64",			/* 196 = lstat64 */
	"fstat64",			/* 197 = fstat64 */
	"#198 (unimplemented lchown)",		/* 198 = unimplemented lchown */
	"getuid",			/* 199 = getuid */
	"getgid",			/* 200 = getgid */
	"geteuid",			/* 201 = geteuid */
	"getegid",			/* 202 = getegid */
	"setreuid",			/* 203 = setreuid */
	"setregid",			/* 204 = setregid */
	"netbsd32_getgroups",			/* 205 = netbsd32_getgroups */
	"netbsd32_setgroups",			/* 206 = netbsd32_setgroups */
	"__posix_fchown",			/* 207 = __posix_fchown */
	"setresuid",			/* 208 = setresuid */
	"#209 (unimplemented getresuid)",		/* 209 = unimplemented getresuid */
	"setresgid",			/* 210 = setresgid */
	"#211 (unimplemented getresgid)",		/* 211 = unimplemented getresgid */
	"#212 (unimplemented chown)",		/* 212 = unimplemented chown */
	"setuid",			/* 213 = setuid */
	"setgid",			/* 214 = setgid */
	"#215 (unimplemented setfsuid)",		/* 215 = unimplemented setfsuid */
	"#216 (unimplemented getfsuid)",		/* 216 = unimplemented getfsuid */
	"#217 (unimplemented pivot_root)",		/* 217 = unimplemented pivot_root */
	"#218 (unimplemented mincore)",		/* 218 = unimplemented mincore */
	"#219 (unimplemented madvise)",		/* 219 = unimplemented madvise */
	"getdents64",			/* 220 = getdents64 */
	"fcntl64",			/* 221 = fcntl64 */
	"#222 (unimplemented / * unused * /)",		/* 222 = unimplemented / * unused * / */
	"#223 (unimplemented / * unused * /)",		/* 223 = unimplemented / * unused * / */
	"gettid",			/* 224 = gettid */
	"#225 (unimplemented readahead)",		/* 225 = unimplemented readahead */
	"#226 (unimplemented setxattr)",		/* 226 = unimplemented setxattr */
	"#227 (unimplemented lsetxattr)",		/* 227 = unimplemented lsetxattr */
	"#228 (unimplemented fsetxattr)",		/* 228 = unimplemented fsetxattr */
	"#229 (unimplemented getxattr)",		/* 229 = unimplemented getxattr */
	"#230 (unimplemented lgetxattr)",		/* 230 = unimplemented lgetxattr */
	"#231 (unimplemented fgetxattr)",		/* 231 = unimplemented fgetxattr */
	"#232 (unimplemented listxattr)",		/* 232 = unimplemented listxattr */
	"#233 (unimplemented llistxattr)",		/* 233 = unimplemented llistxattr */
	"#234 (unimplemented flistxattr)",		/* 234 = unimplemented flistxattr */
	"#235 (unimplemented removexattr)",		/* 235 = unimplemented removexattr */
	"#236 (unimplemented lremovexattr)",		/* 236 = unimplemented lremovexattr */
	"#237 (unimplemented fremovexattr)",		/* 237 = unimplemented fremovexattr */
	"#238 (unimplemented tkill)",		/* 238 = unimplemented tkill */
	"#239 (unimplemented sendfile64)",		/* 239 = unimplemented sendfile64 */
	"#240 (unimplemented futex)",		/* 240 = unimplemented futex */
	"#241 (unimplemented sched_setaffinity)",		/* 241 = unimplemented sched_setaffinity */
	"#242 (unimplemented sched_getaffinity)",		/* 242 = unimplemented sched_getaffinity */
	"#243 (unimplemented set_thread_area)",		/* 243 = unimplemented set_thread_area */
	"#244 (unimplemented get_thread_area)",		/* 244 = unimplemented get_thread_area */
	"#245 (unimplemented io_setup)",		/* 245 = unimplemented io_setup */
	"#246 (unimplemented io_destroy)",		/* 246 = unimplemented io_destroy */
	"#247 (unimplemented io_getevents)",		/* 247 = unimplemented io_getevents */
	"#248 (unimplemented io_submit)",		/* 248 = unimplemented io_submit */
	"#249 (unimplemented io_cancel)",		/* 249 = unimplemented io_cancel */
	"#250 (unimplemented fadvise64)",		/* 250 = unimplemented fadvise64 */
	"#251 (unimplemented / * unused * /)",		/* 251 = unimplemented / * unused * / */
	"exit_group",			/* 252 = exit_group */
	"#253 (unimplemented lookup_dcookie)",		/* 253 = unimplemented lookup_dcookie */
	"#254 (unimplemented epoll_create)",		/* 254 = unimplemented epoll_create */
	"#255 (unimplemented epoll_ctl)",		/* 255 = unimplemented epoll_ctl */
	"#256 (unimplemented epoll_wait)",		/* 256 = unimplemented epoll_wait */
	"#257 (unimplemented remap_file_pages)",		/* 257 = unimplemented remap_file_pages */
	"#258 (unimplemented set_tid_address)",		/* 258 = unimplemented set_tid_address */
	"#259 (unimplemented timer_create)",		/* 259 = unimplemented timer_create */
	"#260 (unimplemented timer_settime)",		/* 260 = unimplemented timer_settime */
	"#261 (unimplemented timer_gettime)",		/* 261 = unimplemented timer_gettime */
	"#262 (unimplemented timer_getoverrun)",		/* 262 = unimplemented timer_getoverrun */
	"#263 (unimplemented timer_delete)",		/* 263 = unimplemented timer_delete */
	"#264 (unimplemented clock_settime)",		/* 264 = unimplemented clock_settime */
	"#265 (unimplemented clock_gettime)",		/* 265 = unimplemented clock_gettime */
	"#266 (unimplemented clock_getres)",		/* 266 = unimplemented clock_getres */
	"#267 (unimplemented clock_nanosleep)",		/* 267 = unimplemented clock_nanosleep */
	"#268 (unimplemented statfs64)",		/* 268 = unimplemented statfs64 */
	"#269 (unimplemented fstatfs64)",		/* 269 = unimplemented fstatfs64 */
	"#270 (unimplemented tgkill)",		/* 270 = unimplemented tgkill */
	"#271 (unimplemented utimes)",		/* 271 = unimplemented utimes */
	"#272 (unimplemented fadvise64_64)",		/* 272 = unimplemented fadvise64_64 */
	"#273 (unimplemented vserver)",		/* 273 = unimplemented vserver */
	"#274 (unimplemented mbind)",		/* 274 = unimplemented mbind */
	"#275 (unimplemented get_mempolicy)",		/* 275 = unimplemented get_mempolicy */
	"#276 (unimplemented set_mempolicy)",		/* 276 = unimplemented set_mempolicy */
	"#277 (unimplemented mq_open)",		/* 277 = unimplemented mq_open */
	"#278 (unimplemented mq_unlink)",		/* 278 = unimplemented mq_unlink */
	"#279 (unimplemented mq_timedsend)",		/* 279 = unimplemented mq_timedsend */
	"#280 (unimplemented mq_timedreceive)",		/* 280 = unimplemented mq_timedreceive */
	"#281 (unimplemented mq_notify)",		/* 281 = unimplemented mq_notify */
	"#282 (unimplemented mq_getsetattr)",		/* 282 = unimplemented mq_getsetattr */
	"#283 (unimplemented kexec_load)",		/* 283 = unimplemented kexec_load */
};

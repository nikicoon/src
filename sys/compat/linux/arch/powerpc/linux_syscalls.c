/* $NetBSD: linux_syscalls.c,v 1.9 2001/05/30 11:37:27 mrg Exp $ */

/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.5 2001/05/13 20:54:45 manu Exp   
 */

#if defined(_KERNEL_OPT)
#if defined(_KERNEL_OPT)
#include "opt_compat_netbsd.h"
#include "opt_compat_43.h"
#endif
#include <sys/param.h>
#include <sys/poll.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/syscallargs.h>
#include <compat/linux/common/linux_types.h>
#include <compat/linux/common/linux_signal.h>
#include <compat/linux/common/linux_siginfo.h>
#include <compat/linux/common/linux_machdep.h>
#include <compat/linux/common/linux_mmap.h>
#include <compat/linux/linux_syscallargs.h>
#endif /* _KERNEL_OPT */

const char *const linux_syscallnames[] = {
	"syscall",			/* 0 = syscall */
	"exit",			/* 1 = exit */
	"fork",			/* 2 = fork */
	"read",			/* 3 = read */
	"write",			/* 4 = write */
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
	"lchown",			/* 16 = lchown */
	"#17 (obsolete break)",		/* 17 = obsolete break */
	"#18 (obsolete ostat)",		/* 18 = obsolete ostat */
	"lseek",			/* 19 = lseek */
	"getpid",			/* 20 = getpid */
	"#21 (unimplemented mount)",		/* 21 = unimplemented mount */
	"#22 (obsolete umount)",		/* 22 = obsolete umount */
	"setuid",			/* 23 = setuid */
	"getuid",			/* 24 = getuid */
	"stime",			/* 25 = stime */
	"ptrace",			/* 26 = ptrace */
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
	"dup",			/* 41 = dup */
	"pipe",			/* 42 = pipe */
	"times",			/* 43 = times */
	"#44 (obsolete prof)",		/* 44 = obsolete prof */
	"brk",			/* 45 = brk */
	"setgid",			/* 46 = setgid */
	"getgid",			/* 47 = getgid */
	"signal",			/* 48 = signal */
	"geteuid",			/* 49 = geteuid */
	"getegid",			/* 50 = getegid */
	"acct",			/* 51 = acct */
	"#52 (unimplemented umount)",		/* 52 = unimplemented umount */
	"#53 (obsolete lock)",		/* 53 = obsolete lock */
	"ioctl",			/* 54 = ioctl */
	"fcntl",			/* 55 = fcntl */
	"#56 (obsolete mpx)",		/* 56 = obsolete mpx */
	"setpgid",			/* 57 = setpgid */
	"#58 (obsolete ulimit)",		/* 58 = obsolete ulimit */
	"olduname",			/* 59 = olduname */
	"umask",			/* 60 = umask */
	"chroot",			/* 61 = chroot */
	"#62 (unimplemented ustat)",		/* 62 = unimplemented ustat */
	"dup2",			/* 63 = dup2 */
	"getppid",			/* 64 = getppid */
	"getpgrp",			/* 65 = getpgrp */
	"setsid",			/* 66 = setsid */
	"sigaction",			/* 67 = sigaction */
	"siggetmask",			/* 68 = siggetmask */
	"sigsetmask",			/* 69 = sigsetmask */
	"setreuid",			/* 70 = setreuid */
	"setregid",			/* 71 = setregid */
	"sigsuspend",			/* 72 = sigsuspend */
	"sigpending",			/* 73 = sigpending */
	"sethostname",			/* 74 = sethostname */
	"setrlimit",			/* 75 = setrlimit */
	"getrlimit",			/* 76 = getrlimit */
	"getrusage",			/* 77 = getrusage */
	"gettimeofday",			/* 78 = gettimeofday */
	"settimeofday",			/* 79 = settimeofday */
	"getgroups",			/* 80 = getgroups */
	"setgroups",			/* 81 = setgroups */
	"select",			/* 82 = select */
	"symlink",			/* 83 = symlink */
	"oolstat",			/* 84 = oolstat */
	"readlink",			/* 85 = readlink */
	"uselib",			/* 86 = uselib */
	"swapon",			/* 87 = swapon */
	"reboot",			/* 88 = reboot */
	"readdir",			/* 89 = readdir */
	"powerpc_mmap",			/* 90 = powerpc_mmap */
	"munmap",			/* 91 = munmap */
	"truncate",			/* 92 = truncate */
	"ftruncate",			/* 93 = ftruncate */
	"fchmod",			/* 94 = fchmod */
	"__posix_fchown",			/* 95 = __posix_fchown */
	"getpriority",			/* 96 = getpriority */
	"setpriority",			/* 97 = setpriority */
	"profil",			/* 98 = profil */
	"statfs",			/* 99 = statfs */
	"fstatfs",			/* 100 = fstatfs */
	"ioperm",			/* 101 = ioperm */
	"socketcall",			/* 102 = socketcall */
	"#103 (unimplemented syslog)",		/* 103 = unimplemented syslog */
	"setitimer",			/* 104 = setitimer */
	"getitimer",			/* 105 = getitimer */
	"stat",			/* 106 = stat */
	"lstat",			/* 107 = lstat */
	"fstat",			/* 108 = fstat */
	"uname",			/* 109 = uname */
	"#110 (unimplemented iopl)",		/* 110 = unimplemented iopl */
	"#111 (unimplemented vhangup)",		/* 111 = unimplemented vhangup */
	"#112 (unimplemented idle)",		/* 112 = unimplemented idle */
	"#113 (unimplemented vm86old)",		/* 113 = unimplemented vm86old */
	"wait4",			/* 114 = wait4 */
	"swapoff",			/* 115 = swapoff */
	"sysinfo",			/* 116 = sysinfo */
	"ipc",			/* 117 = ipc */
	"fsync",			/* 118 = fsync */
	"sigreturn",			/* 119 = sigreturn */
	"clone",			/* 120 = clone */
	"setdomainname",			/* 121 = setdomainname */
	"new_uname",			/* 122 = new_uname */
	"#123 (unimplemented modify_ldt)",		/* 123 = unimplemented modify_ldt */
	"#124 (unimplemented adjtimex)",		/* 124 = unimplemented adjtimex */
	"mprotect",			/* 125 = mprotect */
	"sigprocmask",			/* 126 = sigprocmask */
	"#127 (unimplemented create_module)",		/* 127 = unimplemented create_module */
	"#128 (unimplemented init_module)",		/* 128 = unimplemented init_module */
	"#129 (unimplemented delete_module)",		/* 129 = unimplemented delete_module */
	"#130 (unimplemented get_kernel_syms)",		/* 130 = unimplemented get_kernel_syms */
	"#131 (unimplemented quotactl)",		/* 131 = unimplemented quotactl */
	"getpgid",			/* 132 = getpgid */
	"fchdir",			/* 133 = fchdir */
	"#134 (unimplemented bdflush)",		/* 134 = unimplemented bdflush */
	"#135 (unimplemented sysfs)",		/* 135 = unimplemented sysfs */
	"personality",			/* 136 = personality */
	"#137 (unimplemented afs_syscall)",		/* 137 = unimplemented afs_syscall */
	"setfsuid",			/* 138 = setfsuid */
	"getfsuid",			/* 139 = getfsuid */
	"llseek",			/* 140 = llseek */
	"getdents",			/* 141 = getdents */
	"new_select",			/* 142 = new_select */
	"flock",			/* 143 = flock */
	"msync",			/* 144 = msync */
	"readv",			/* 145 = readv */
	"writev",			/* 146 = writev */
	"getsid",			/* 147 = getsid */
	"fdatasync",			/* 148 = fdatasync */
	"__sysctl",			/* 149 = __sysctl */
	"mlock",			/* 150 = mlock */
	"munlock",			/* 151 = munlock */
	"mlockall",			/* 152 = mlockall */
	"munlockall",			/* 153 = munlockall */
	"sched_setparam",			/* 154 = sched_setparam */
	"sched_getparam",			/* 155 = sched_getparam */
	"sched_setscheduler",			/* 156 = sched_setscheduler */
	"sched_getscheduler",			/* 157 = sched_getscheduler */
	"sched_yield",			/* 158 = sched_yield */
	"sched_get_priority_max",			/* 159 = sched_get_priority_max */
	"sched_get_priority_min",			/* 160 = sched_get_priority_min */
	"#161 (unimplemented sched_rr_get_interval)",		/* 161 = unimplemented sched_rr_get_interval */
	"nanosleep",			/* 162 = nanosleep */
	"mremap",			/* 163 = mremap */
	"setresuid",			/* 164 = setresuid */
	"getresuid",			/* 165 = getresuid */
	"#166 (unimplemented query_module)",		/* 166 = unimplemented query_module */
	"poll",			/* 167 = poll */
	"#168 (unimplemented nfsservctl)",		/* 168 = unimplemented nfsservctl */
	"setresgid",			/* 169 = setresgid */
	"getresgid",			/* 170 = getresgid */
	"#171 (unimplemented prctl)",		/* 171 = unimplemented prctl */
	"rt_sigreturn",			/* 172 = rt_sigreturn */
	"rt_sigaction",			/* 173 = rt_sigaction */
	"rt_sigprocmask",			/* 174 = rt_sigprocmask */
	"rt_sigpending",			/* 175 = rt_sigpending */
	"#176 (unimplemented rt_sigtimedwait)",		/* 176 = unimplemented rt_sigtimedwait */
	"rt_queueinfo",			/* 177 = rt_queueinfo */
	"rt_sigsuspend",			/* 178 = rt_sigsuspend */
	"pread",			/* 179 = pread */
	"pwrite",			/* 180 = pwrite */
	"chown",			/* 181 = chown */
	"__getcwd",			/* 182 = __getcwd */
	"#183 (unimplemented capget)",		/* 183 = unimplemented capget */
	"#184 (unimplemented capset)",		/* 184 = unimplemented capset */
	"sigaltstack",			/* 185 = sigaltstack */
	"#186 (unimplemented sendfile)",		/* 186 = unimplemented sendfile */
	"#187 (unimplemented getpmsg)",		/* 187 = unimplemented getpmsg */
	"#188 (unimplemented putpmsg)",		/* 188 = unimplemented putpmsg */
	"__vfork14",			/* 189 = __vfork14 */
	"#190 (unimplemented sys_getrlimit)",		/* 190 = unimplemented sys_getrlimit */
	"#191 (unimplemented / * unused * /)",		/* 191 = unimplemented / * unused * / */
	"#192 (unimplemented mmap2)",		/* 192 = unimplemented mmap2 */
	"#193 (unimplemented truncate64)",		/* 193 = unimplemented truncate64 */
	"#194 (unimplemented ftruncate64)",		/* 194 = unimplemented ftruncate64 */
	"#195 (unimplemented stat64)",		/* 195 = unimplemented stat64 */
	"#196 (unimplemented lstat64)",		/* 196 = unimplemented lstat64 */
	"#197 (unimplemented fstat64)",		/* 197 = unimplemented fstat64 */
	"#198 (unimplemented sys_pciconfig_read)",		/* 198 = unimplemented sys_pciconfig_read */
	"#199 (unimplemented sys_pciconfig_write)",		/* 199 = unimplemented sys_pciconfig_write */
	"#200 (unimplemented sys_pciconfig_iobase)",		/* 200 = unimplemented sys_pciconfig_iobase */
	"#201 (unimplemented / * Unused ( MacOnLinux project ) * /)",		/* 201 = unimplemented / * Unused ( MacOnLinux project ) * / */
	"#202 (unimplemented sys_getdents64)",		/* 202 = unimplemented sys_getdents64 */
	"#203 (unimplemented pivot_root)",		/* 203 = unimplemented pivot_root */
	"#204 (unimplemented fcntl64)",		/* 204 = unimplemented fcntl64 */
	"#205 (unimplemented madvise)",		/* 205 = unimplemented madvise */
	"#206 (unimplemented mincore)",		/* 206 = unimplemented mincore */
};

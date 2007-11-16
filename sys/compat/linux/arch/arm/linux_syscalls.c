/* $NetBSD: linux_syscalls.c,v 1.30 2007/11/16 12:53:58 njoly Exp $ */

/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.29 2007/11/16 12:51:55 njoly Exp
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: linux_syscalls.c,v 1.30 2007/11/16 12:53:58 njoly Exp $");

#if defined(_KERNEL_OPT)
#if defined(_KERNEL_OPT)
#include "opt_compat_43.h"
#endif
#include <sys/param.h>
#include <sys/poll.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/syscallargs.h>
#include <compat/linux/common/linux_types.h>
#include <compat/linux/common/linux_mmap.h>
#include <compat/linux/common/linux_signal.h>
#include <compat/linux/common/linux_siginfo.h>
#include <compat/linux/common/linux_machdep.h>
#include <compat/linux/linux_syscallargs.h>
#endif /* _KERNEL_OPT */

const char *const linux_syscallnames[] = {
	/*   0 */	"nosys",
	/*   1 */	"exit",
	/*   2 */	"fork",
	/*   3 */	"read",
	/*   4 */	"write",
	/*   5 */	"open",
	/*   6 */	"close",
	/*   7 */	"waitpid",
	/*   8 */	"creat",
	/*   9 */	"link",
	/*  10 */	"unlink",
	/*  11 */	"execve",
	/*  12 */	"chdir",
	/*  13 */	"time",
	/*  14 */	"mknod",
	/*  15 */	"chmod",
	/*  16 */	"lchown16",
	/*  17 */	"#17 (obsolete break)",
	/*  18 */	"#18 (obsolete ostat)",
	/*  19 */	"lseek",
	/*  20 */	"getpid",
	/*  21 */	"#21 (unimplemented mount)",
	/*  22 */	"#22 (obsolete umount)",
	/*  23 */	"setuid",
	/*  24 */	"getuid",
	/*  25 */	"stime",
	/*  26 */	"ptrace",
	/*  27 */	"alarm",
	/*  28 */	"#28 (obsolete ofstat)",
	/*  29 */	"pause",
	/*  30 */	"utime",
	/*  31 */	"#31 (obsolete stty)",
	/*  32 */	"#32 (obsolete gtty)",
	/*  33 */	"access",
	/*  34 */	"nice",
	/*  35 */	"#35 (obsolete ftime)",
	/*  36 */	"sync",
	/*  37 */	"kill",
	/*  38 */	"__posix_rename",
	/*  39 */	"mkdir",
	/*  40 */	"rmdir",
	/*  41 */	"dup",
	/*  42 */	"pipe",
	/*  43 */	"times",
	/*  44 */	"#44 (obsolete prof)",
	/*  45 */	"brk",
	/*  46 */	"setgid",
	/*  47 */	"getgid",
	/*  48 */	"signal",
	/*  49 */	"geteuid",
	/*  50 */	"getegid",
	/*  51 */	"acct",
	/*  52 */	"#52 (unimplemented umount)",
	/*  53 */	"#53 (obsolete lock)",
	/*  54 */	"ioctl",
	/*  55 */	"fcntl",
	/*  56 */	"#56 (obsolete mpx)",
	/*  57 */	"setpgid",
	/*  58 */	"#58 (obsolete ulimit)",
	/*  59 */	"oldolduname",
	/*  60 */	"umask",
	/*  61 */	"chroot",
	/*  62 */	"#62 (unimplemented ustat)",
	/*  63 */	"dup2",
	/*  64 */	"getppid",
	/*  65 */	"getpgrp",
	/*  66 */	"setsid",
	/*  67 */	"sigaction",
	/*  68 */	"siggetmask",
	/*  69 */	"sigsetmask",
	/*  70 */	"setreuid",
	/*  71 */	"setregid",
	/*  72 */	"sigsuspend",
	/*  73 */	"sigpending",
	/*  74 */	"sethostname",
	/*  75 */	"setrlimit",
	/*  76 */	"getrlimit",
	/*  77 */	"getrusage",
	/*  78 */	"gettimeofday",
	/*  79 */	"settimeofday",
	/*  80 */	"getgroups",
	/*  81 */	"setgroups",
	/*  82 */	"oldselect",
	/*  83 */	"symlink",
	/*  84 */	"oolstat",
	/*  85 */	"readlink",
#ifdef EXEC_AOUT
	/*  86 */	"uselib",
#else
	/*  86 */	"#86 (excluded uselib)",
#endif
	/*  87 */	"swapon",
	/*  88 */	"reboot",
	/*  89 */	"readdir",
	/*  90 */	"old_mmap",
	/*  91 */	"munmap",
	/*  92 */	"truncate",
	/*  93 */	"ftruncate",
	/*  94 */	"fchmod",
	/*  95 */	"fchown16",
	/*  96 */	"getpriority",
	/*  97 */	"setpriority",
	/*  98 */	"profil",
	/*  99 */	"statfs",
	/* 100 */	"fstatfs",
	/* 101 */	"#101 (unimplemented)",
	/* 102 */	"socketcall",
	/* 103 */	"#103 (unimplemented syslog)",
	/* 104 */	"setitimer",
	/* 105 */	"getitimer",
	/* 106 */	"stat",
	/* 107 */	"lstat",
	/* 108 */	"fstat",
	/* 109 */	"olduname",
	/* 110 */	"#110 (unimplemented)",
	/* 111 */	"#111 (unimplemented vhangup)",
	/* 112 */	"#112 (unimplemented idle)",
	/* 113 */	"#113 (unimplemented syscall)",
	/* 114 */	"wait4",
	/* 115 */	"swapoff",
	/* 116 */	"sysinfo",
	/* 117 */	"ipc",
	/* 118 */	"fsync",
	/* 119 */	"sigreturn",
	/* 120 */	"clone",
	/* 121 */	"setdomainname",
	/* 122 */	"uname",
	/* 123 */	"#123 (unimplemented modify_ldt)",
	/* 124 */	"#124 (unimplemented adjtimex)",
	/* 125 */	"mprotect",
	/* 126 */	"sigprocmask",
	/* 127 */	"#127 (unimplemented create_module)",
	/* 128 */	"#128 (unimplemented init_module)",
	/* 129 */	"#129 (unimplemented delete_module)",
	/* 130 */	"#130 (unimplemented get_kernel_syms)",
	/* 131 */	"#131 (unimplemented quotactl)",
	/* 132 */	"getpgid",
	/* 133 */	"fchdir",
	/* 134 */	"#134 (unimplemented bdflush)",
	/* 135 */	"#135 (unimplemented sysfs)",
	/* 136 */	"personality",
	/* 137 */	"#137 (unimplemented afs_syscall)",
	/* 138 */	"setfsuid",
	/* 139 */	"getfsuid",
	/* 140 */	"llseek",
	/* 141 */	"getdents",
	/* 142 */	"select",
	/* 143 */	"flock",
	/* 144 */	"msync",
	/* 145 */	"readv",
	/* 146 */	"writev",
	/* 147 */	"getsid",
	/* 148 */	"fdatasync",
	/* 149 */	"__sysctl",
	/* 150 */	"mlock",
	/* 151 */	"munlock",
	/* 152 */	"mlockall",
	/* 153 */	"munlockall",
	/* 154 */	"sched_setparam",
	/* 155 */	"sched_getparam",
	/* 156 */	"sched_setscheduler",
	/* 157 */	"sched_getscheduler",
	/* 158 */	"sched_yield",
	/* 159 */	"sched_get_priority_max",
	/* 160 */	"sched_get_priority_min",
	/* 161 */	"#161 (unimplemented sched_rr_get_interval)",
	/* 162 */	"nanosleep",
	/* 163 */	"mremap",
	/* 164 */	"setresuid",
	/* 165 */	"getresuid",
	/* 166 */	"#166 (unimplemented)",
	/* 167 */	"#167 (unimplemented query_module)",
	/* 168 */	"poll",
	/* 169 */	"#169 (unimplemented nfsservctl)",
	/* 170 */	"setresgid",
	/* 171 */	"getresgid",
	/* 172 */	"#172 (unimplemented prctl)",
	/* 173 */	"#173 (unimplemented rt_sigreturn)",
	/* 174 */	"rt_sigaction",
	/* 175 */	"rt_sigprocmask",
	/* 176 */	"rt_sigpending",
	/* 177 */	"#177 (unimplemented rt_sigtimedwait)",
	/* 178 */	"rt_queueinfo",
	/* 179 */	"rt_sigsuspend",
	/* 180 */	"pread",
	/* 181 */	"pwrite",
	/* 182 */	"chown16",
	/* 183 */	"__getcwd",
	/* 184 */	"#184 (unimplemented capget)",
	/* 185 */	"#185 (unimplemented capset)",
	/* 186 */	"sigaltstack",
	/* 187 */	"#187 (unimplemented sendfile)",
	/* 188 */	"#188 (unimplemented getpmsg)",
	/* 189 */	"#189 (unimplemented putpmsg)",
	/* 190 */	"vfork",
	/* 191 */	"ugetrlimit",
	/* 192 */	"mmap2",
	/* 193 */	"truncate64",
	/* 194 */	"ftruncate64",
	/* 195 */	"stat64",
	/* 196 */	"lstat64",
	/* 197 */	"fstat64",
	/* 198 */	"__posix_lchown",
	/* 199 */	"getuid32",
	/* 200 */	"getgid32",
	/* 201 */	"geteuid32",
	/* 202 */	"getegid32",
	/* 203 */	"setreuid32",
	/* 204 */	"setregid32",
	/* 205 */	"getgroups32",
	/* 206 */	"setgroups32",
	/* 207 */	"fchown32",
	/* 208 */	"setresuid32",
	/* 209 */	"getresuid32",
	/* 210 */	"setresgid32",
	/* 211 */	"getresgid32",
	/* 212 */	"chown32",
	/* 213 */	"setuid32",
	/* 214 */	"setgid32",
	/* 215 */	"setfsuid32",
	/* 216 */	"getfsuid32",
	/* 217 */	"getdents64",
	/* 218 */	"#218 (unimplemented pivot_root)",
	/* 219 */	"mincore",
	/* 220 */	"madvise",
	/* 221 */	"fcntl64",
	/* 222 */	"#222 (unimplemented / * for tux * /)",
	/* 223 */	"#223 (unimplemented / * unused * /)",
	/* 224 */	"#224 (unimplemented gettid)",
	/* 225 */	"#225 (unimplemented readahead)",
	/* 226 */	"setxattr",
	/* 227 */	"lsetxattr",
	/* 228 */	"fsetxattr",
	/* 229 */	"getxattr",
	/* 230 */	"lgetxattr",
	/* 231 */	"fgetxattr",
	/* 232 */	"listxattr",
	/* 233 */	"llistxattr",
	/* 234 */	"flistxattr",
	/* 235 */	"removexattr",
	/* 236 */	"lremovexattr",
	/* 237 */	"fremovexattr",
	/* 238 */	"#238 (unimplemented tkill)",
	/* 239 */	"#239 (unimplemented sendfile64)",
	/* 240 */	"#240 (unimplemented futex)",
	/* 241 */	"#241 (unimplemented sched_setaffinity)",
	/* 242 */	"#242 (unimplemented sched_getaffinity)",
	/* 243 */	"#243 (unimplemented io_setup)",
	/* 244 */	"#244 (unimplemented io_destroy)",
	/* 245 */	"#245 (unimplemented io_getevents)",
	/* 246 */	"#246 (unimplemented io_submit)",
	/* 247 */	"#247 (unimplemented io_cancel)",
	/* 248 */	"exit_group",
	/* 249 */	"#249 (unimplemented lookup_dcookie)",
	/* 250 */	"#250 (unimplemented epoll_create)",
	/* 251 */	"#251 (unimplemented epoll_ctl)",
	/* 252 */	"#252 (unimplemented epoll_wait)",
	/* 253 */	"#253 (unimplemented remap_file_pages)",
	/* 254 */	"#254 (unimplemented / * for set_thread_area * /)",
	/* 255 */	"#255 (unimplemented / * for get_thread_area * /)",
	/* 256 */	"#256 (unimplemented / * for set_tid_address * /)",
	/* 257 */	"#257 (unimplemented timer_create)",
	/* 258 */	"#258 (unimplemented timer_settime)",
	/* 259 */	"#259 (unimplemented timer_gettime)",
	/* 260 */	"#260 (unimplemented timer_getoverrun)",
	/* 261 */	"#261 (unimplemented timer_delete)",
	/* 262 */	"clock_settime",
	/* 263 */	"clock_gettime",
	/* 264 */	"clock_getres",
	/* 265 */	"clock_nanosleep",
	/* 266 */	"statfs64",
	/* 267 */	"fstatfs64",
	/* 268 */	"#268 (unimplemented tgkill)",
	/* 269 */	"#269 (unimplemented utimes)",
	/* 270 */	"#270 (unimplemented fadvise64_64)",
	/* 271 */	"#271 (unimplemented pciconfig_iobase)",
	/* 272 */	"#272 (unimplemented pciconfig_read)",
	/* 273 */	"#273 (unimplemented pciconfig_write)",
	/* 274 */	"#274 (unimplemented)",
	/* 275 */	"#275 (unimplemented)",
	/* 276 */	"#276 (unimplemented)",
	/* 277 */	"#277 (unimplemented)",
	/* 278 */	"#278 (unimplemented)",
	/* 279 */	"#279 (unimplemented)",
	/* 280 */	"#280 (unimplemented)",
	/* 281 */	"#281 (unimplemented)",
	/* 282 */	"#282 (unimplemented)",
	/* 283 */	"#283 (unimplemented)",
	/* 284 */	"#284 (unimplemented)",
	/* 285 */	"#285 (unimplemented)",
	/* 286 */	"#286 (unimplemented)",
	/* 287 */	"#287 (unimplemented)",
	/* 288 */	"#288 (unimplemented)",
	/* 289 */	"#289 (unimplemented)",
	/* 290 */	"#290 (unimplemented)",
	/* 291 */	"#291 (unimplemented)",
	/* 292 */	"#292 (unimplemented)",
	/* 293 */	"#293 (unimplemented)",
	/* 294 */	"#294 (unimplemented)",
	/* 295 */	"#295 (unimplemented)",
	/* 296 */	"#296 (unimplemented)",
	/* 297 */	"#297 (unimplemented)",
	/* 298 */	"#298 (unimplemented)",
	/* 299 */	"#299 (unimplemented)",
	/* 300 */	"#300 (unimplemented)",
	/* 301 */	"#301 (unimplemented)",
	/* 302 */	"#302 (unimplemented)",
	/* 303 */	"#303 (unimplemented)",
	/* 304 */	"#304 (unimplemented)",
	/* 305 */	"#305 (unimplemented)",
	/* 306 */	"#306 (unimplemented)",
	/* 307 */	"#307 (unimplemented)",
	/* 308 */	"#308 (unimplemented)",
	/* 309 */	"#309 (unimplemented)",
	/* 310 */	"#310 (unimplemented)",
	/* 311 */	"#311 (unimplemented)",
	/* 312 */	"#312 (unimplemented)",
	/* 313 */	"#313 (unimplemented)",
	/* 314 */	"#314 (unimplemented)",
	/* 315 */	"#315 (unimplemented)",
	/* 316 */	"#316 (unimplemented)",
	/* 317 */	"#317 (unimplemented)",
	/* 318 */	"#318 (unimplemented)",
	/* 319 */	"#319 (unimplemented)",
	/* 320 */	"#320 (unimplemented)",
	/* 321 */	"#321 (unimplemented)",
	/* 322 */	"#322 (unimplemented)",
	/* 323 */	"#323 (unimplemented)",
	/* 324 */	"#324 (unimplemented)",
	/* 325 */	"#325 (unimplemented)",
	/* 326 */	"#326 (unimplemented)",
	/* 327 */	"#327 (unimplemented)",
	/* 328 */	"#328 (unimplemented)",
	/* 329 */	"#329 (unimplemented)",
	/* 330 */	"#330 (unimplemented)",
	/* 331 */	"#331 (unimplemented)",
	/* 332 */	"#332 (unimplemented)",
	/* 333 */	"#333 (unimplemented)",
	/* 334 */	"#334 (unimplemented)",
	/* 335 */	"#335 (unimplemented)",
	/* 336 */	"#336 (unimplemented)",
	/* 337 */	"#337 (unimplemented)",
	/* 338 */	"#338 (unimplemented)",
	/* 339 */	"#339 (unimplemented)",
	/* 340 */	"#340 (unimplemented)",
	/* 341 */	"#341 (unimplemented)",
	/* 342 */	"#342 (unimplemented)",
	/* 343 */	"#343 (unimplemented)",
	/* 344 */	"#344 (unimplemented)",
	/* 345 */	"#345 (unimplemented)",
	/* 346 */	"#346 (unimplemented)",
	/* 347 */	"#347 (unimplemented)",
	/* 348 */	"#348 (unimplemented)",
	/* 349 */	"#349 (unimplemented)",
	/* 350 */	"#350 (unimplemented)",
	/* 351 */	"#351 (unimplemented)",
	/* 352 */	"#352 (unimplemented)",
	/* 353 */	"#353 (unimplemented)",
	/* 354 */	"#354 (unimplemented)",
	/* 355 */	"#355 (unimplemented)",
	/* 356 */	"#356 (unimplemented)",
	/* 357 */	"#357 (unimplemented)",
	/* 358 */	"#358 (unimplemented)",
	/* 359 */	"#359 (unimplemented)",
	/* 360 */	"#360 (unimplemented)",
	/* 361 */	"#361 (unimplemented)",
	/* 362 */	"#362 (unimplemented)",
	/* 363 */	"#363 (unimplemented)",
	/* 364 */	"#364 (unimplemented)",
	/* 365 */	"#365 (unimplemented)",
	/* 366 */	"#366 (unimplemented)",
	/* 367 */	"#367 (unimplemented)",
	/* 368 */	"#368 (unimplemented)",
	/* 369 */	"#369 (unimplemented)",
	/* 370 */	"#370 (unimplemented)",
	/* 371 */	"#371 (unimplemented)",
	/* 372 */	"#372 (unimplemented)",
	/* 373 */	"#373 (unimplemented)",
	/* 374 */	"#374 (unimplemented)",
	/* 375 */	"#375 (unimplemented)",
	/* 376 */	"#376 (unimplemented)",
	/* 377 */	"#377 (unimplemented)",
	/* 378 */	"#378 (unimplemented)",
	/* 379 */	"#379 (unimplemented)",
	/* 380 */	"#380 (unimplemented)",
	/* 381 */	"#381 (unimplemented)",
	/* 382 */	"#382 (unimplemented)",
	/* 383 */	"#383 (unimplemented)",
	/* 384 */	"#384 (unimplemented / * base * /)",
	/* 385 */	"breakpoint",
	/* 386 */	"cacheflush",
	/* 387 */	"#387 (unimplemented usr26)",
	/* 388 */	"#388 (unimplemented usr32)",
};

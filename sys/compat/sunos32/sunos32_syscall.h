/* $NetBSD: sunos32_syscall.h,v 1.5 2001/05/30 11:37:31 mrg Exp $ */

/*
 * System call numbers.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from;	NetBSD: syscalls.master,v 1.4 2001/02/06 15:41:53 mrg Exp 
 */

/* syscall: "syscall" ret: "int" args: */
#define	SUNOS32_SYS_syscall	0

/* syscall: "netbsd32_exit" ret: "int" args: "int" */
#define	SUNOS32_SYS_netbsd32_exit	1

/* syscall: "fork" ret: "int" args: */
#define	SUNOS32_SYS_fork	2

/* syscall: "netbsd32_read" ret: "netbsd32_ssize_t" args: "int" "netbsd32_voidp" "netbsd32_size_t" */
#define	SUNOS32_SYS_netbsd32_read	3

/* syscall: "netbsd32_write" ret: "netbsd32_ssize_t" args: "int" "const netbsd32_voidp" "netbsd32_size_t" */
#define	SUNOS32_SYS_netbsd32_write	4

/* syscall: "open" ret: "int" args: "const netbsd32_charp" "int" "int" */
#define	SUNOS32_SYS_open	5

/* syscall: "netbsd32_close" ret: "int" args: "int" */
#define	SUNOS32_SYS_netbsd32_close	6

/* syscall: "wait4" ret: "int" args: "int" "netbsd32_intp" "int" "netbsd32_rusagep_t" */
#define	SUNOS32_SYS_wait4	7

/* syscall: "creat" ret: "int" args: "const netbsd32_charp" "int" */
#define	SUNOS32_SYS_creat	8

/* syscall: "netbsd32_link" ret: "int" args: "netbsd32_charp" "netbsd32_charp" */
#define	SUNOS32_SYS_netbsd32_link	9

/* syscall: "netbsd32_unlink" ret: "int" args: "netbsd32_charp" */
#define	SUNOS32_SYS_netbsd32_unlink	10

/* syscall: "execv" ret: "int" args: "const netbsd32_charp" "netbsd32_charpp" */
#define	SUNOS32_SYS_execv	11

/* syscall: "netbsd32_chdir" ret: "int" args: "netbsd32_charp" */
#define	SUNOS32_SYS_netbsd32_chdir	12

				/* 13 is obsolete old_time */
/* syscall: "mknod" ret: "int" args: "const netbsd32_charp" "int" "int" */
#define	SUNOS32_SYS_mknod	14

/* syscall: "netbsd32_chmod" ret: "int" args: "netbsd32_charp" "int" */
#define	SUNOS32_SYS_netbsd32_chmod	15

/* syscall: "netbsd32_chown" ret: "int" args: "netbsd32_charp" "int" "int" */
#define	SUNOS32_SYS_netbsd32_chown	16

/* syscall: "netbsd32_break" ret: "int" args: "netbsd32_charp" */
#define	SUNOS32_SYS_netbsd32_break	17

				/* 18 is obsolete old_stat */
/* syscall: "compat_43_netbsd32_olseek" ret: "netbsd32_long" args: "int" "netbsd32_long" "int" */
#define	SUNOS32_SYS_compat_43_netbsd32_olseek	19

/* syscall: "getpid_with_ppid" ret: "pid_t" args: */
#define	SUNOS32_SYS_getpid_with_ppid	20

				/* 21 is obsolete sunos_old_mount */
/* syscall: "netbsd32_setuid" ret: "int" args: "uid_t" */
#define	SUNOS32_SYS_netbsd32_setuid	23

/* syscall: "getuid_with_euid" ret: "uid_t" args: */
#define	SUNOS32_SYS_getuid_with_euid	24

/* syscall: "stime" ret: "int" args: "sunos32_time_tp" */
#define	SUNOS32_SYS_stime	25

/* syscall: "ptrace" ret: "netbsd32_long" args: "int" "pid_t" "netbsd32_caddr_t" "int" "netbsd32_charp" */
#define	SUNOS32_SYS_ptrace	26

/* syscall: "access" ret: "int" args: "const netbsd32_charp" "int" */
#define	SUNOS32_SYS_access	33

/* syscall: "sync" ret: "int" args: */
#define	SUNOS32_SYS_sync	36

/* syscall: "netbsd32_kill" ret: "int" args: "int" "int" */
#define	SUNOS32_SYS_netbsd32_kill	37

/* syscall: "stat" ret: "int" args: "const netbsd32_charp" "netbsd32_stat43p_t" */
#define	SUNOS32_SYS_stat	38

/* syscall: "lstat" ret: "int" args: "const netbsd32_charp" "netbsd32_stat43p_t" */
#define	SUNOS32_SYS_lstat	40

/* syscall: "netbsd32_dup" ret: "int" args: "u_int" */
#define	SUNOS32_SYS_netbsd32_dup	41

/* syscall: "pipe" ret: "int" args: */
#define	SUNOS32_SYS_pipe	42

/* syscall: "netbsd32_profil" ret: "int" args: "netbsd32_caddr_t" "u_int" "u_int" "u_int" */
#define	SUNOS32_SYS_netbsd32_profil	44

/* syscall: "netbsd32_setgid" ret: "int" args: "uid_t" */
#define	SUNOS32_SYS_netbsd32_setgid	46

/* syscall: "getgid_with_egid" ret: "gid_t" args: */
#define	SUNOS32_SYS_getgid_with_egid	47

/* syscall: "netbsd32_acct" ret: "int" args: "netbsd32_charp" */
#define	SUNOS32_SYS_netbsd32_acct	51

/* syscall: "mctl" ret: "int" args: "netbsd32_voidp" "int" "int" "netbsd32_voidp" */
#define	SUNOS32_SYS_mctl	53

/* syscall: "ioctl" ret: "int" args: "int" "netbsd32_u_long" "netbsd32_caddr_t" */
#define	SUNOS32_SYS_ioctl	54

/* syscall: "reboot" ret: "int" args: "int" "netbsd32_charp" */
#define	SUNOS32_SYS_reboot	55

				/* 56 is obsolete sunos_owait3 */
/* syscall: "netbsd32_symlink" ret: "int" args: "netbsd32_charp" "netbsd32_charp" */
#define	SUNOS32_SYS_netbsd32_symlink	57

/* syscall: "netbsd32_readlink" ret: "int" args: "netbsd32_charp" "netbsd32_charp" "int" */
#define	SUNOS32_SYS_netbsd32_readlink	58

/* syscall: "execve" ret: "int" args: "const netbsd32_charp" "netbsd32_charpp" "netbsd32_charpp" */
#define	SUNOS32_SYS_execve	59

/* syscall: "netbsd32_umask" ret: "int" args: "int" */
#define	SUNOS32_SYS_netbsd32_umask	60

/* syscall: "netbsd32_chroot" ret: "int" args: "netbsd32_charp" */
#define	SUNOS32_SYS_netbsd32_chroot	61

/* syscall: "compat_43_netbsd32_fstat43" ret: "int" args: "int" "netbsd32_stat43p_t" */
#define	SUNOS32_SYS_compat_43_netbsd32_fstat43	62

/* syscall: "ogetpagesize" ret: "int" args: */
#define	SUNOS32_SYS_ogetpagesize	64

/* syscall: "omsync" ret: "int" args: "netbsd32_caddr_t" "netbsd32_size_t" "int" */
#define	SUNOS32_SYS_omsync	65

/* syscall: "vfork" ret: "int" args: */
#define	SUNOS32_SYS_vfork	66

				/* 67 is obsolete vread */
				/* 68 is obsolete vwrite */
/* syscall: "netbsd32_sbrk" ret: "int" args: "netbsd32_intptr_t" */
#define	SUNOS32_SYS_netbsd32_sbrk	69

/* syscall: "netbsd32_sstk" ret: "int" args: "int" */
#define	SUNOS32_SYS_netbsd32_sstk	70

/* syscall: "mmap" ret: "netbsd32_voidp" args: "netbsd32_voidp" "netbsd32_size_t" "int" "int" "int" "netbsd32_long" */
#define	SUNOS32_SYS_mmap	71

/* syscall: "vadvise" ret: "int" args: "int" */
#define	SUNOS32_SYS_vadvise	72

/* syscall: "netbsd32_munmap" ret: "int" args: "netbsd32_voidp" "netbsd32_size_t" */
#define	SUNOS32_SYS_netbsd32_munmap	73

/* syscall: "netbsd32_mprotect" ret: "int" args: "netbsd32_voidp" "netbsd32_size_t" "int" */
#define	SUNOS32_SYS_netbsd32_mprotect	74

/* syscall: "netbsd32_madvise" ret: "int" args: "netbsd32_voidp" "netbsd32_size_t" "int" */
#define	SUNOS32_SYS_netbsd32_madvise	75

/* syscall: "vhangup" ret: "int" args: */
#define	SUNOS32_SYS_vhangup	76

/* syscall: "netbsd32_mincore" ret: "int" args: "netbsd32_caddr_t" "netbsd32_size_t" "netbsd32_charp" */
#define	SUNOS32_SYS_netbsd32_mincore	78

/* syscall: "netbsd32_getgroups" ret: "int" args: "u_int" "netbsd32_gid_tp" */
#define	SUNOS32_SYS_netbsd32_getgroups	79

/* syscall: "netbsd32_setgroups" ret: "int" args: "u_int" "netbsd32_gid_tp" */
#define	SUNOS32_SYS_netbsd32_setgroups	80

/* syscall: "getpgrp" ret: "int" args: */
#define	SUNOS32_SYS_getpgrp	81

/* syscall: "setpgrp" ret: "int" args: "int" "int" */
#define	SUNOS32_SYS_setpgrp	82

/* syscall: "netbsd32_setitimer" ret: "int" args: "u_int" "netbsd32_itimervalp_t" "netbsd32_itimervalp_t" */
#define	SUNOS32_SYS_netbsd32_setitimer	83

/* syscall: "compat_12_netbsd32_oswapon" ret: "int" args: "netbsd32_charp" */
#define	SUNOS32_SYS_compat_12_netbsd32_oswapon	85

/* syscall: "netbsd32_getitimer" ret: "int" args: "u_int" "netbsd32_itimervalp_t" */
#define	SUNOS32_SYS_netbsd32_getitimer	86

/* syscall: "compat_43_netbsd32_ogethostname" ret: "int" args: "netbsd32_charp" "u_int" */
#define	SUNOS32_SYS_compat_43_netbsd32_ogethostname	87

/* syscall: "compat_43_netbsd32_osethostname" ret: "int" args: "netbsd32_charp" "u_int" */
#define	SUNOS32_SYS_compat_43_netbsd32_osethostname	88

/* syscall: "ogetdtablesize" ret: "int" args: */
#define	SUNOS32_SYS_ogetdtablesize	89

/* syscall: "netbsd32_dup2" ret: "int" args: "u_int" "u_int" */
#define	SUNOS32_SYS_netbsd32_dup2	90

/* syscall: "fcntl" ret: "int" args: "int" "int" "netbsd32_voidp" */
#define	SUNOS32_SYS_fcntl	92

/* syscall: "netbsd32_select" ret: "int" args: "u_int" "netbsd32_fd_setp_t" "netbsd32_fd_setp_t" "netbsd32_fd_setp_t" "netbsd32_timevalp_t" */
#define	SUNOS32_SYS_netbsd32_select	93

/* syscall: "netbsd32_fsync" ret: "int" args: "int" */
#define	SUNOS32_SYS_netbsd32_fsync	95

/* syscall: "netbsd32_setpriority" ret: "int" args: "int" "int" "int" */
#define	SUNOS32_SYS_netbsd32_setpriority	96

/* syscall: "netbsd32_socket" ret: "int" args: "int" "int" "int" */
#define	SUNOS32_SYS_netbsd32_socket	97

/* syscall: "netbsd32_connect" ret: "int" args: "int" "netbsd32_caddr_t" "int" */
#define	SUNOS32_SYS_netbsd32_connect	98

/* syscall: "compat_43_netbsd32_oaccept" ret: "int" args: "int" "netbsd32_caddr_t" "netbsd32_intp" */
#define	SUNOS32_SYS_compat_43_netbsd32_oaccept	99

/* syscall: "netbsd32_getpriority" ret: "int" args: "int" "int" */
#define	SUNOS32_SYS_netbsd32_getpriority	100

/* syscall: "compat_43_netbsd32_osend" ret: "int" args: "int" "netbsd32_caddr_t" "int" "int" */
#define	SUNOS32_SYS_compat_43_netbsd32_osend	101

/* syscall: "compat_43_netbsd32_orecv" ret: "int" args: "int" "netbsd32_caddr_t" "int" "int" */
#define	SUNOS32_SYS_compat_43_netbsd32_orecv	102

/* syscall: "netbsd32_bind" ret: "int" args: "int" "netbsd32_caddr_t" "int" */
#define	SUNOS32_SYS_netbsd32_bind	104

/* syscall: "setsockopt" ret: "int" args: "int" "int" "int" "netbsd32_caddr_t" "int" */
#define	SUNOS32_SYS_setsockopt	105

/* syscall: "netbsd32_listen" ret: "int" args: "int" "int" */
#define	SUNOS32_SYS_netbsd32_listen	106

/* syscall: "sigvec" ret: "int" args: "int" "netbsd32_sigvecp_t" "netbsd32_sigvecp_t" */
#define	SUNOS32_SYS_sigvec	108

/* syscall: "compat_43_netbsd32_sigblock" ret: "int" args: "int" */
#define	SUNOS32_SYS_compat_43_netbsd32_sigblock	109

/* syscall: "compat_43_netbsd32_sigsetmask" ret: "int" args: "int" */
#define	SUNOS32_SYS_compat_43_netbsd32_sigsetmask	110

/* syscall: "sigsuspend" ret: "int" args: "int" */
#define	SUNOS32_SYS_sigsuspend	111

/* syscall: "compat_43_netbsd32_osigstack" ret: "int" args: "netbsd32_sigstackp_t" "netbsd32_sigstackp_t" */
#define	SUNOS32_SYS_compat_43_netbsd32_osigstack	112

/* syscall: "compat_43_netbsd32_orecvmsg" ret: "int" args: "int" "netbsd32_omsghdrp_t" "int" */
#define	SUNOS32_SYS_compat_43_netbsd32_orecvmsg	113

/* syscall: "compat_43_netbsd32_osendmsg" ret: "int" args: "int" "netbsd32_caddr_t" "int" */
#define	SUNOS32_SYS_compat_43_netbsd32_osendmsg	114

				/* 115 is obsolete vtrace */
/* syscall: "netbsd32_gettimeofday" ret: "int" args: "netbsd32_timevalp_t" "netbsd32_timezonep_t" */
#define	SUNOS32_SYS_netbsd32_gettimeofday	116

/* syscall: "netbsd32_getrusage" ret: "int" args: "int" "netbsd32_rusagep_t" */
#define	SUNOS32_SYS_netbsd32_getrusage	117

/* syscall: "netbsd32_getsockopt" ret: "int" args: "int" "int" "int" "netbsd32_caddr_t" "netbsd32_intp" */
#define	SUNOS32_SYS_netbsd32_getsockopt	118

/* syscall: "netbsd32_readv" ret: "int" args: "int" "netbsd32_iovecp_t" "u_int" */
#define	SUNOS32_SYS_netbsd32_readv	120

/* syscall: "netbsd32_writev" ret: "int" args: "int" "netbsd32_iovecp_t" "u_int" */
#define	SUNOS32_SYS_netbsd32_writev	121

/* syscall: "netbsd32_settimeofday" ret: "int" args: "netbsd32_timevalp_t" "netbsd32_timezonep_t" */
#define	SUNOS32_SYS_netbsd32_settimeofday	122

/* syscall: "netbsd32_fchown" ret: "int" args: "int" "int" "int" */
#define	SUNOS32_SYS_netbsd32_fchown	123

/* syscall: "netbsd32_fchmod" ret: "int" args: "int" "int" */
#define	SUNOS32_SYS_netbsd32_fchmod	124

/* syscall: "compat_43_netbsd32_orecvfrom" ret: "int" args: "int" "netbsd32_caddr_t" "netbsd32_size_t" "int" "netbsd32_caddr_t" "netbsd32_intp" */
#define	SUNOS32_SYS_compat_43_netbsd32_orecvfrom	125

/* syscall: "netbsd32_setreuid" ret: "int" args: "int" "int" */
#define	SUNOS32_SYS_netbsd32_setreuid	126

/* syscall: "netbsd32_setregid" ret: "int" args: "int" "int" */
#define	SUNOS32_SYS_netbsd32_setregid	127

/* syscall: "netbsd32_rename" ret: "int" args: "netbsd32_charp" "netbsd32_charp" */
#define	SUNOS32_SYS_netbsd32_rename	128

/* syscall: "compat_43_netbsd32_otruncate" ret: "int" args: "netbsd32_charp" "netbsd32_long" */
#define	SUNOS32_SYS_compat_43_netbsd32_otruncate	129

/* syscall: "compat_43_netbsd32_oftruncate" ret: "int" args: "int" "netbsd32_long" */
#define	SUNOS32_SYS_compat_43_netbsd32_oftruncate	130

/* syscall: "netbsd32_flock" ret: "int" args: "int" "int" */
#define	SUNOS32_SYS_netbsd32_flock	131

/* syscall: "netbsd32_sendto" ret: "int" args: "int" "netbsd32_caddr_t" "netbsd32_size_t" "int" "netbsd32_caddr_t" "int" */
#define	SUNOS32_SYS_netbsd32_sendto	133

/* syscall: "netbsd32_shutdown" ret: "int" args: "int" "int" */
#define	SUNOS32_SYS_netbsd32_shutdown	134

/* syscall: "netbsd32_socketpair" ret: "int" args: "int" "int" "int" "netbsd32_intp" */
#define	SUNOS32_SYS_netbsd32_socketpair	135

/* syscall: "netbsd32_mkdir" ret: "int" args: "netbsd32_charp" "int" */
#define	SUNOS32_SYS_netbsd32_mkdir	136

/* syscall: "netbsd32_rmdir" ret: "int" args: "netbsd32_charp" */
#define	SUNOS32_SYS_netbsd32_rmdir	137

/* syscall: "netbsd32_utimes" ret: "int" args: "netbsd32_charp" "netbsd32_timevalp_t" */
#define	SUNOS32_SYS_netbsd32_utimes	138

/* syscall: "sigreturn" ret: "int" args: "netbsd32_sigcontextp_t" */
#define	SUNOS32_SYS_sigreturn	139

/* syscall: "netbsd32_adjtime" ret: "int" args: "netbsd32_timevalp_t" "netbsd32_timevalp_t" */
#define	SUNOS32_SYS_netbsd32_adjtime	140

/* syscall: "compat_43_netbsd32_ogetpeername" ret: "int" args: "int" "netbsd32_caddr_t" "netbsd32_intp" */
#define	SUNOS32_SYS_compat_43_netbsd32_ogetpeername	141

/* syscall: "gethostid" ret: "int" args: */
#define	SUNOS32_SYS_gethostid	142

/* syscall: "getrlimit" ret: "int" args: "u_int" "netbsd32_orlimitp_t" */
#define	SUNOS32_SYS_getrlimit	144

/* syscall: "setrlimit" ret: "int" args: "u_int" "netbsd32_orlimitp_t" */
#define	SUNOS32_SYS_setrlimit	145

/* syscall: "compat_43_netbsd32_killpg" ret: "int" args: "int" "int" */
#define	SUNOS32_SYS_compat_43_netbsd32_killpg	146

/* syscall: "compat_43_netbsd32_ogetsockname" ret: "int" args: "int" "netbsd32_caddr_t" "netbsd32_intp" */
#define	SUNOS32_SYS_compat_43_netbsd32_ogetsockname	150

/* syscall: "netbsd32_poll" ret: "int" args: "netbsd32_pollfdp_t" "u_int" "int" */
#define	SUNOS32_SYS_netbsd32_poll	153

/* syscall: "nfssvc" ret: "int" args: "int" */
#define	SUNOS32_SYS_nfssvc	155

/* syscall: "compat_43_netbsd32_ogetdirentries" ret: "int" args: "int" "netbsd32_charp" "u_int" "netbsd32_longp" */
#define	SUNOS32_SYS_compat_43_netbsd32_ogetdirentries	156

/* syscall: "statfs" ret: "int" args: "const netbsd32_charp" "sunos32_statfsp_t" */
#define	SUNOS32_SYS_statfs	157

/* syscall: "fstatfs" ret: "int" args: "int" "sunos32_statfsp_t" */
#define	SUNOS32_SYS_fstatfs	158

/* syscall: "unmount" ret: "int" args: "netbsd32_charp" */
#define	SUNOS32_SYS_unmount	159

/* syscall: "async_daemon" ret: "int" args: */
#define	SUNOS32_SYS_async_daemon	160

/* syscall: "getfh" ret: "int" args: "netbsd32_charp" "netbsd32_fhandlep_t" */
#define	SUNOS32_SYS_getfh	161

/* syscall: "compat_09_netbsd32_ogetdomainname" ret: "int" args: "netbsd32_charp" "int" */
#define	SUNOS32_SYS_compat_09_netbsd32_ogetdomainname	162

/* syscall: "compat_09_netbsd32_osetdomainname" ret: "int" args: "netbsd32_charp" "int" */
#define	SUNOS32_SYS_compat_09_netbsd32_osetdomainname	163

/* syscall: "quotactl" ret: "int" args: "int" "netbsd32_charp" "int" "netbsd32_caddr_t" */
#define	SUNOS32_SYS_quotactl	165

/* syscall: "exportfs" ret: "int" args: "netbsd32_charp" "netbsd32_charp" */
#define	SUNOS32_SYS_exportfs	166

/* syscall: "mount" ret: "int" args: "netbsd32_charp" "netbsd32_charp" "int" "netbsd32_caddr_t" */
#define	SUNOS32_SYS_mount	167

/* syscall: "ustat" ret: "int" args: "int" "sunos32_ustatp_t" */
#define	SUNOS32_SYS_ustat	168

/* syscall: "osemsys" ret: "int" args: "int" "int" "int" "int" "int" */
#define	SUNOS32_SYS_osemsys	169

/* syscall: "omsgsys" ret: "int" args: "int" "int" "int" "int" "int" "int" */
#define	SUNOS32_SYS_omsgsys	170

/* syscall: "oshmsys" ret: "int" args: "int" "int" "int" "int" */
#define	SUNOS32_SYS_oshmsys	171

/* syscall: "auditsys" ret: "int" args: "netbsd32_charp" */
#define	SUNOS32_SYS_auditsys	172

/* syscall: "getdents" ret: "int" args: "int" "netbsd32_charp" "int" */
#define	SUNOS32_SYS_getdents	174

/* syscall: "setsid" ret: "int" args: */
#define	SUNOS32_SYS_setsid	175

/* syscall: "netbsd32_fchdir" ret: "int" args: "int" */
#define	SUNOS32_SYS_netbsd32_fchdir	176

/* syscall: "netbsd32_fchroot" ret: "int" args: "int" */
#define	SUNOS32_SYS_netbsd32_fchroot	177

/* syscall: "sigpending" ret: "int" args: "netbsd32_intp" */
#define	SUNOS32_SYS_sigpending	183

/* syscall: "netbsd32_setpgid" ret: "int" args: "int" "int" */
#define	SUNOS32_SYS_netbsd32_setpgid	185

/* syscall: "netbsd32_pathconf" ret: "netbsd32_long" args: "netbsd32_charp" "int" */
#define	SUNOS32_SYS_netbsd32_pathconf	186

/* syscall: "netbsd32_fpathconf" ret: "netbsd32_long" args: "int" "int" */
#define	SUNOS32_SYS_netbsd32_fpathconf	187

/* syscall: "sysconf" ret: "int" args: "int" */
#define	SUNOS32_SYS_sysconf	188

/* syscall: "uname" ret: "int" args: "sunos32_utsnamep_t" */
#define	SUNOS32_SYS_uname	189

#define	SUNOS32_SYS_MAXSYSCALL	190

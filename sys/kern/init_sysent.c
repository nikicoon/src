/*
 * System call switch table.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from: NetBSD syscalls.master,v 1.22 1994/06/29 06:33:10
 */

#include <sys/param.h>
#include <sys/systm.h>

int	nosys();

int	nosys();
int	exit();
int	fork();
int	read();
int	write();
int	open();
int	close();
int	wait4();
int	link();
int	unlink();
int	chdir();
int	fchdir();
int	mknod();
int	chmod();
int	chown();
int	obreak();
int	getfsstat();
int	getpid();
int	mount();
int	unmount();
int	setuid();
int	getuid();
int	geteuid();
int	ptrace();
int	recvmsg();
int	sendmsg();
int	recvfrom();
int	accept();
int	getpeername();
int	getsockname();
int	access();
int	chflags();
int	fchflags();
int	sync();
int	kill();
int	getppid();
int	dup();
int	pipe();
int	getegid();
int	profil();
#ifdef KTRACE
int	ktrace();
#else
#endif
int	sigaction();
int	getgid();
int	sigprocmask();
int	getlogin();
int	setlogin();
int	acct();
int	sigpending();
int	sigaltstack();
int	ioctl();
int	reboot();
int	revoke();
int	symlink();
int	readlink();
int	execve();
int	umask();
int	chroot();
int	msync();
int	vfork();
int	sbrk();
int	sstk();
int	ovadvise();
int	munmap();
int	mprotect();
int	madvise();
int	mincore();
int	getgroups();
int	setgroups();
int	getpgrp();
int	setpgid();
int	setitimer();
int	swapon();
int	getitimer();
int	dup2();
int	fcntl();
int	select();
int	fsync();
int	setpriority();
int	socket();
int	connect();
int	getpriority();
int	sigreturn();
int	bind();
int	setsockopt();
int	listen();
int	sigsuspend();
#ifdef TRACE
int	vtrace();
#else
#endif
int	gettimeofday();
int	getrusage();
int	getsockopt();
#ifdef vax
int	resuba();
#else
#endif
int	readv();
int	writev();
int	settimeofday();
int	fchown();
int	fchmod();
int	rename();
int	flock();
int	mkfifo();
int	sendto();
int	shutdown();
int	socketpair();
int	mkdir();
int	rmdir();
int	utimes();
int	adjtime();
int	setsid();
int	quotactl();
#if defined(NFSCLIENT) || defined(NFSSERVER)
int	nfssvc();
#else
#endif
int	statfs();
int	fstatfs();
#ifdef NFSCLIENT
int	getfh();
#else
#endif
int	sysarch();
#ifdef SYSVSEM
int	semsys();
#else
#endif
#ifdef SYSVMSG
int	msgsys();
#else
#endif
#ifdef SYSVSHM
int	shmsys();
#else
#endif
int	setgid();
int	setegid();
int	seteuid();
#ifdef LFS
int	lfs_bmapv();
int	lfs_markv();
int	lfs_segclean();
int	lfs_segwait();
#else
#endif
int	stat();
int	fstat();
int	lstat();
int	pathconf();
int	fpathconf();
int	getrlimit();
int	setrlimit();
int	getdirentries();
int	mmap();
int	nosys();
int	lseek();
int	truncate();
int	ftruncate();
int	__sysctl();
int	mlock();
int	munlock();
#ifdef LKM
int	lkmnosys();
int	lkmnosys();
int	lkmnosys();
int	lkmnosys();
int	lkmnosys();
int	lkmnosys();
int	lkmnosys();
int	lkmnosys();
int	lkmnosys();
int	lkmnosys();
#else	/* !LKM */
#endif	/* !LKM */

#ifdef COMPAT_43
#define compat(n, name) n, __CONCAT(o,name)

int	ocreat();
int	olseek();
int	ostat();
int	olstat();
#ifdef KTRACE
#else
#endif
int	ofstat();
int	ogetkerninfo();
int	ogetpagesize();
int	ommap();
int	owait();
int	ogethostname();
int	osethostname();
int	ogetdtablesize();
int	oaccept();
int	osend();
int	orecv();
int	osigvec();
int	osigblock();
int	osigsetmask();
int	osigstack();
int	orecvmsg();
int	osendmsg();
#ifdef TRACE
#else
#endif
#ifdef vax
#else
#endif
int	orecvfrom();
int	osetreuid();
int	osetregid();
int	otruncate();
int	oftruncate();
int	ogetpeername();
int	ogethostid();
int	osethostid();
int	ogetrlimit();
int	osetrlimit();
int	okillpg();
int	oquota();
int	ogetsockname();
#if defined(NFSCLIENT) || defined(NFSSERVER)
#else
#endif
int	ogetdirentries();
#ifdef NFSCLIENT
#else
#endif
#ifdef SYSVSEM
#else
#endif
#ifdef SYSVMSG
#else
#endif
#ifdef SYSVSHM
#else
#endif
#ifdef LFS
#else
#endif
#ifdef LKM
#else	/* !LKM */
#endif	/* !LKM */

#else /* COMPAT_43 */
#define compat(n, name) 0, nosys
#endif /* COMPAT_43 */


#ifdef COMPAT_09
#define ncompat(n, name) n, __CONCAT(o,name)

int	ogetdomainname();
int	osetdomainname();
int	ouname();

#else /* COMPAT_43 */
#define ncompat(n, name) 0, nosys
#endif /* COMPAT_43 */

struct sysent sysent[] = {
	{ 0, nosys },			/* 0 = syscall */
	{ 1, exit },			/* 1 = exit */
	{ 0, fork },			/* 2 = fork */
	{ 3, read },			/* 3 = read */
	{ 3, write },			/* 4 = write */
	{ 3, open },			/* 5 = open */
	{ 1, close },			/* 6 = close */
	{ 4, wait4 },			/* 7 = wait4 */
	{ compat(2,creat) },		/* 8 = old creat */
	{ 2, link },			/* 9 = link */
	{ 1, unlink },			/* 10 = unlink */
	{ 0, nosys },			/* 11 = obsolete execv */
	{ 1, chdir },			/* 12 = chdir */
	{ 1, fchdir },			/* 13 = fchdir */
	{ 3, mknod },			/* 14 = mknod */
	{ 2, chmod },			/* 15 = chmod */
	{ 3, chown },			/* 16 = chown */
	{ 1, obreak },			/* 17 = break */
	{ 3, getfsstat },			/* 18 = getfsstat */
	{ compat(3,lseek) },		/* 19 = old lseek */
	{ 0, getpid },			/* 20 = getpid */
	{ 4, mount },			/* 21 = mount */
	{ 2, unmount },			/* 22 = unmount */
	{ 1, setuid },			/* 23 = setuid */
	{ 0, getuid },			/* 24 = getuid */
	{ 0, geteuid },			/* 25 = geteuid */
	{ 4, ptrace },			/* 26 = ptrace */
	{ 3, recvmsg },			/* 27 = recvmsg */
	{ 3, sendmsg },			/* 28 = sendmsg */
	{ 6, recvfrom },			/* 29 = recvfrom */
	{ 3, accept },			/* 30 = accept */
	{ 3, getpeername },			/* 31 = getpeername */
	{ 3, getsockname },			/* 32 = getsockname */
	{ 2, access },			/* 33 = access */
	{ 2, chflags },			/* 34 = chflags */
	{ 2, fchflags },			/* 35 = fchflags */
	{ 0, sync },			/* 36 = sync */
	{ 2, kill },			/* 37 = kill */
	{ compat(2,stat) },		/* 38 = old stat */
	{ 0, getppid },			/* 39 = getppid */
	{ compat(2,lstat) },		/* 40 = old lstat */
	{ 2, dup },			/* 41 = dup */
	{ 0, pipe },			/* 42 = pipe */
	{ 0, getegid },			/* 43 = getegid */
	{ 4, profil },			/* 44 = profil */
#ifdef KTRACE
	{ 4, ktrace },			/* 45 = ktrace */
#else
	{ 0, nosys },			/* 45 = ktrace */
#endif
	{ 3, sigaction },			/* 46 = sigaction */
	{ 0, getgid },			/* 47 = getgid */
	{ 2, sigprocmask },			/* 48 = sigprocmask */
	{ 2, getlogin },			/* 49 = getlogin */
	{ 1, setlogin },			/* 50 = setlogin */
	{ 1, acct },			/* 51 = acct */
	{ 0, sigpending },			/* 52 = sigpending */
	{ 2, sigaltstack },			/* 53 = sigaltstack */
	{ 3, ioctl },			/* 54 = ioctl */
	{ 1, reboot },			/* 55 = reboot */
	{ 1, revoke },			/* 56 = revoke */
	{ 2, symlink },			/* 57 = symlink */
	{ 3, readlink },			/* 58 = readlink */
	{ 3, execve },			/* 59 = execve */
	{ 1, umask },			/* 60 = umask */
	{ 1, chroot },			/* 61 = chroot */
	{ compat(2,fstat) },		/* 62 = old fstat */
	{ compat(4,getkerninfo) },		/* 63 = old getkerninfo */
	{ compat(0,getpagesize) },		/* 64 = old getpagesize */
	{ 2, msync },			/* 65 = msync */
	{ 0, vfork },			/* 66 = vfork */
	{ 0, nosys },			/* 67 = obsolete vread */
	{ 0, nosys },			/* 68 = obsolete vwrite */
	{ 1, sbrk },			/* 69 = sbrk */
	{ 1, sstk },			/* 70 = sstk */
	{ compat(7,mmap) },		/* 71 = old mmap */
	{ 1, ovadvise },			/* 72 = vadvise */
	{ 2, munmap },			/* 73 = munmap */
	{ 3, mprotect },			/* 74 = mprotect */
	{ 3, madvise },			/* 75 = madvise */
	{ 0, nosys },			/* 76 = obsolete vhangup */
	{ 0, nosys },			/* 77 = obsolete vlimit */
	{ 3, mincore },			/* 78 = mincore */
	{ 2, getgroups },			/* 79 = getgroups */
	{ 2, setgroups },			/* 80 = setgroups */
	{ 0, getpgrp },			/* 81 = getpgrp */
	{ 2, setpgid },			/* 82 = setpgid */
	{ 3, setitimer },			/* 83 = setitimer */
	{ compat(0,wait) },		/* 84 = old wait */
	{ 1, swapon },			/* 85 = swapon */
	{ 2, getitimer },			/* 86 = getitimer */
	{ compat(2,gethostname) },		/* 87 = old gethostname */
	{ compat(2,sethostname) },		/* 88 = old sethostname */
	{ compat(0,getdtablesize) },		/* 89 = old getdtablesize */
	{ 2, dup2 },			/* 90 = dup2 */
	{ 0, nosys },			/* 91 = getdopt */
	{ 3, fcntl },			/* 92 = fcntl */
	{ 5, select },			/* 93 = select */
	{ 0, nosys },			/* 94 = setdopt */
	{ 1, fsync },			/* 95 = fsync */
	{ 3, setpriority },			/* 96 = setpriority */
	{ 3, socket },			/* 97 = socket */
	{ 3, connect },			/* 98 = connect */
	{ compat(3,accept) },		/* 99 = old accept */
	{ 2, getpriority },			/* 100 = getpriority */
	{ compat(4,send) },		/* 101 = old send */
	{ compat(4,recv) },		/* 102 = old recv */
	{ 1, sigreturn },			/* 103 = sigreturn */
	{ 3, bind },			/* 104 = bind */
	{ 5, setsockopt },			/* 105 = setsockopt */
	{ 2, listen },			/* 106 = listen */
	{ 0, nosys },			/* 107 = obsolete vtimes */
	{ compat(3,sigvec) },		/* 108 = old sigvec */
	{ compat(1,sigblock) },		/* 109 = old sigblock */
	{ compat(1,sigsetmask) },		/* 110 = old sigsetmask */
	{ 1, sigsuspend },			/* 111 = sigsuspend */
	{ compat(2,sigstack) },		/* 112 = old sigstack */
	{ compat(3,recvmsg) },		/* 113 = old recvmsg */
	{ compat(3,sendmsg) },		/* 114 = old sendmsg */
#ifdef TRACE
	{ 2, vtrace },			/* 115 = vtrace */
#else
	{ 0, nosys },			/* 115 = obsolete vtrace */
#endif
	{ 2, gettimeofday },			/* 116 = gettimeofday */
	{ 2, getrusage },			/* 117 = getrusage */
	{ 5, getsockopt },			/* 118 = getsockopt */
#ifdef vax
	{ 1, resuba },			/* 119 = resuba */
#else
	{ 0, nosys },			/* 119 = nosys */
#endif
	{ 3, readv },			/* 120 = readv */
	{ 3, writev },			/* 121 = writev */
	{ 2, settimeofday },			/* 122 = settimeofday */
	{ 3, fchown },			/* 123 = fchown */
	{ 2, fchmod },			/* 124 = fchmod */
	{ compat(6,recvfrom) },		/* 125 = old recvfrom */
	{ compat(2,setreuid) },		/* 126 = old setreuid */
	{ compat(2,setregid) },		/* 127 = old setregid */
	{ 2, rename },			/* 128 = rename */
	{ compat(2,truncate) },		/* 129 = old truncate */
	{ compat(2,ftruncate) },		/* 130 = old ftruncate */
	{ 2, flock },			/* 131 = flock */
	{ 2, mkfifo },			/* 132 = mkfifo */
	{ 6, sendto },			/* 133 = sendto */
	{ 2, shutdown },			/* 134 = shutdown */
	{ 5, socketpair },			/* 135 = socketpair */
	{ 2, mkdir },			/* 136 = mkdir */
	{ 1, rmdir },			/* 137 = rmdir */
	{ 2, utimes },			/* 138 = utimes */
	{ 0, nosys },			/* 139 = obsolete 4.2 sigreturn */
	{ 2, adjtime },			/* 140 = adjtime */
	{ compat(3,getpeername) },		/* 141 = old getpeername */
	{ compat(0,gethostid) },		/* 142 = old gethostid */
	{ compat(1,sethostid) },		/* 143 = old sethostid */
	{ compat(2,getrlimit) },		/* 144 = old getrlimit */
	{ compat(2,setrlimit) },		/* 145 = old setrlimit */
	{ compat(2,killpg) },		/* 146 = old killpg */
	{ 0, setsid },			/* 147 = setsid */
	{ 4, quotactl },			/* 148 = quotactl */
	{ compat(4,quota) },		/* 149 = old quota */
	{ compat(3,getsockname) },		/* 150 = old getsockname */
	{ 0, nosys },			/* 151 = nosys */
	{ 0, nosys },			/* 152 = nosys */
	{ 0, nosys },			/* 153 = nosys */
	{ 0, nosys },			/* 154 = nosys */
#if defined(NFSCLIENT) || defined(NFSSERVER)
	{ 2, nfssvc },			/* 155 = nfssvc */
#else
	{ 0, nosys },			/* 155 = nosys */
#endif
	{ compat(4,getdirentries) },		/* 156 = old getdirentries */
	{ 2, statfs },			/* 157 = statfs */
	{ 2, fstatfs },			/* 158 = fstatfs */
	{ 0, nosys },			/* 159 = nosys */
	{ 0, nosys },			/* 160 = nosys */
#ifdef NFSCLIENT
	{ 2, getfh },			/* 161 = getfh */
#else
	{ 0, nosys },			/* 161 = nosys */
#endif
	{ ncompat(2,getdomainname) },		/* 162 = old getdomainname */
	{ ncompat(2,setdomainname) },		/* 163 = old setdomainname */
	{ ncompat(1,uname) },		/* 164 = old uname */
	{ 2, sysarch },			/* 165 = sysarch */
	{ 0, nosys },			/* 166 = nosys */
	{ 0, nosys },			/* 167 = nosys */
	{ 0, nosys },			/* 168 = nosys */
#ifdef SYSVSEM
	{ 5, semsys },			/* 169 = semsys */
#else
	{ 0, nosys },			/* 169 = nosys */
#endif
#ifdef SYSVMSG
	{ 6, msgsys },			/* 170 = msgsys */
#else
	{ 0, nosys },			/* 170 = nosys */
#endif
#ifdef SYSVSHM
	{ 4, shmsys },			/* 171 = shmsys */
#else
	{ 0, nosys },			/* 171 = nosys */
#endif
	{ 0, nosys },			/* 172 = nosys */
	{ 0, nosys },			/* 173 = nosys */
	{ 0, nosys },			/* 174 = nosys */
	{ 0, nosys },			/* 175 = nosys */
	{ 0, nosys },			/* 176 = nosys */
	{ 0, nosys },			/* 177 = nosys */
	{ 0, nosys },			/* 178 = nosys */
	{ 0, nosys },			/* 179 = nosys */
	{ 0, nosys },			/* 180 = nosys */
	{ 1, setgid },			/* 181 = setgid */
	{ 1, setegid },			/* 182 = setegid */
	{ 1, seteuid },			/* 183 = seteuid */
#ifdef LFS
	{ 3, lfs_bmapv },			/* 184 = lfs_bmapv */
	{ 3, lfs_markv },			/* 185 = lfs_markv */
	{ 2, lfs_segclean },			/* 186 = lfs_segclean */
	{ 2, lfs_segwait },			/* 187 = lfs_segwait */
#else
	{ 0, nosys },			/* 184 = nosys */
	{ 0, nosys },			/* 185 = nosys */
	{ 0, nosys },			/* 186 = nosys */
	{ 0, nosys },			/* 187 = nosys */
#endif
	{ 2, stat },			/* 188 = stat */
	{ 2, fstat },			/* 189 = fstat */
	{ 2, lstat },			/* 190 = lstat */
	{ 2, pathconf },			/* 191 = pathconf */
	{ 2, fpathconf },			/* 192 = fpathconf */
	{ 0, nosys },			/* 193 = nosys */
	{ 2, getrlimit },			/* 194 = getrlimit */
	{ 2, setrlimit },			/* 195 = setrlimit */
	{ 4, getdirentries },			/* 196 = getdirentries */
	{ 8, mmap },			/* 197 = mmap */
	{ 0, nosys },			/* 198 = __syscall */
	{ 5, lseek },			/* 199 = lseek */
	{ 4, truncate },			/* 200 = truncate */
	{ 4, ftruncate },			/* 201 = ftruncate */
	{ 6, __sysctl },			/* 202 = __sysctl */
	{ 2, mlock },			/* 203 = mlock */
	{ 2, munlock },			/* 204 = munlock */
	{ 0, nosys },			/* 205 = nosys */
	{ 0, nosys },			/* 206 = nosys */
	{ 0, nosys },			/* 207 = nosys */
	{ 0, nosys },			/* 208 = nosys */
	{ 0, nosys },			/* 209 = nosys */
#ifdef LKM
	{ 0, lkmnosys },			/* 210 = lkmnosys */
	{ 0, lkmnosys },			/* 211 = lkmnosys */
	{ 0, lkmnosys },			/* 212 = lkmnosys */
	{ 0, lkmnosys },			/* 213 = lkmnosys */
	{ 0, lkmnosys },			/* 214 = lkmnosys */
	{ 0, lkmnosys },			/* 215 = lkmnosys */
	{ 0, lkmnosys },			/* 216 = lkmnosys */
	{ 0, lkmnosys },			/* 217 = lkmnosys */
	{ 0, lkmnosys },			/* 218 = lkmnosys */
	{ 0, lkmnosys },			/* 219 = lkmnosys */
#else	/* !LKM */
	{ 0, nosys },			/* 210 = nosys */
	{ 0, nosys },			/* 211 = nosys */
	{ 0, nosys },			/* 212 = nosys */
	{ 0, nosys },			/* 213 = nosys */
	{ 0, nosys },			/* 214 = nosys */
	{ 0, nosys },			/* 215 = nosys */
	{ 0, nosys },			/* 216 = nosys */
	{ 0, nosys },			/* 217 = nosys */
	{ 0, nosys },			/* 218 = nosys */
	{ 0, nosys },			/* 219 = nosys */
#endif	/* !LKM */
};

int	nsysent = sizeof(sysent) / sizeof(sysent[0]);

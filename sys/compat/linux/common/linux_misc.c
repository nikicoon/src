/*	$NetBSD: linux_misc.c,v 1.59 1999/07/15 23:18:41 thorpej Exp $	*/

/*-
 * Copyright (c) 1995, 1998, 1999 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Frank van der Linden and Eric Haszlakiewicz; by Jason R. Thorpe
 * of the Numerical Aerospace Simulation Facility, NASA Ames Research Center.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the NetBSD
 *	Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Linux compatibility module. Try to deal with various Linux system calls.
 */

/*
 * These functions have been moved to multiarch to allow
 * selection of which machines include them to be 
 * determined by the individual files.linux_<arch> files.
 *
 * Function in multiarch:
 *	linux_sys_break			: linux_break.c
 *	linux_sys_alarm			: linux_misc_notalpha.c
 *	linux_sys_getresgid		: linux_misc_notalpha.c
 *	linux_sys_nice			: linux_misc_notalpha.c
 *	linux_sys_readdir		: linux_misc_notalpha.c
 *	linux_sys_setresgid		: linux_misc_notalpha.c
 *	linux_sys_time			: linux_misc_notalpha.c
 *	linux_sys_utime			: linux_misc_notalpha.c
 *	linux_sys_waitpid		: linux_misc_notalpha.c
 *	linux_sys_old_mmap		: linux_oldmmap.c
 *	linux_sys_oldolduname		: linux_oldolduname.c
 *	linux_sys_oldselect		: linux_oldselect.c
 *	linux_sys_olduname		: linux_olduname.c
 *	linux_sys_pipe			: linux_pipe.c
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/namei.h>
#include <sys/proc.h>
#include <sys/dirent.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/filedesc.h>
#include <sys/ioctl.h>
#include <sys/kernel.h>
#include <sys/malloc.h>
#include <sys/mbuf.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <sys/ptrace.h>
#include <sys/resource.h>
#include <sys/resourcevar.h>
#include <sys/signal.h>
#include <sys/signalvar.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/vnode.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sys/unistd.h>

#include <sys/syscallargs.h>

#include <vm/vm.h>
#include <vm/vm_param.h>

#include <compat/linux/common/linux_types.h>
#include <compat/linux/common/linux_signal.h>

#include <compat/linux/linux_syscallargs.h>

#include <compat/linux/common/linux_fcntl.h>
#include <compat/linux/common/linux_mmap.h>
#include <compat/linux/common/linux_dirent.h>
#include <compat/linux/common/linux_util.h>
#include <compat/linux/common/linux_misc.h>
#include <compat/linux/common/linux_sched.h>


/* Local linux_misc.c functions: */
static void bsd_to_linux_statfs __P((struct statfs *, struct linux_statfs *));

/*
 * The information on a terminated (or stopped) process needs
 * to be converted in order for Linux binaries to get a valid signal
 * number out of it.
 */
void
bsd_to_linux_wstat(st)
	int *st;
{

	int sig;

	if (WIFSIGNALED(*st)) {
		sig = WTERMSIG(*st);
		if (sig >= 0 && sig < NSIG)
			*st= (*st& ~0177) | native_to_linux_sig[sig];
	} else if (WIFSTOPPED(*st)) {
		sig = WSTOPSIG(*st);
		if (sig >= 0 && sig < NSIG)
			*st = (*st & ~0xff00) | (native_to_linux_sig[sig] << 8);
	}
}

/*
 * This is very much the same as waitpid()
 */
int
linux_sys_wait4(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_wait4_args /* {
		syscallarg(int) pid;
		syscallarg(int *) status;
		syscallarg(int) options;
		syscallarg(struct rusage *) rusage;
	} */ *uap = v;
	struct sys_wait4_args w4a;
	int error, *status, tstat, options, linux_options;
	caddr_t sg;

	if (SCARG(uap, status) != NULL) {
		sg = stackgap_init(p->p_emul);
		status = (int *) stackgap_alloc(&sg, sizeof *status);
	} else
		status = NULL;

	linux_options = SCARG(uap, options);
	options = 0;
	if (linux_options &
	    ~(LINUX_WAIT4_WNOHANG|LINUX_WAIT4_WUNTRACED|LINUX_WAIT4_WCLONE))
		return (EINVAL);

	if (linux_options & LINUX_WAIT4_WNOHANG)
		options |= WNOHANG;
	if (linux_options & LINUX_WAIT4_WUNTRACED)
		options |= WUNTRACED;
	if (linux_options & LINUX_WAIT4_WCLONE)
		options |= WALTSIG;

	SCARG(&w4a, pid) = SCARG(uap, pid);
	SCARG(&w4a, status) = status;
	SCARG(&w4a, options) = options;
	SCARG(&w4a, rusage) = SCARG(uap, rusage);

	if ((error = sys_wait4(p, &w4a, retval)))
		return error;

	sigdelset(&p->p_siglist, SIGCHLD);

	if (status != NULL) {
		if ((error = copyin(status, &tstat, sizeof tstat)))
			return error;

		bsd_to_linux_wstat(&tstat);
		return copyout(&tstat, SCARG(uap, status), sizeof tstat);
	}

	return 0;
}

/*
 * Linux brk(2). The check if the new address is >= the old one is
 * done in the kernel in Linux. NetBSD does it in the library.
 */
int
linux_sys_brk(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_brk_args /* {
		syscallarg(char *) nsize;
	} */ *uap = v;
	char *nbrk = SCARG(uap, nsize);
	struct sys_obreak_args oba;
	struct vmspace *vm = p->p_vmspace;
	caddr_t oldbrk;

	oldbrk = vm->vm_daddr + ctob(vm->vm_dsize);
	/*
	 * XXX inconsistent.. Linux always returns at least the old
	 * brk value, but it will be page-aligned if this fails,
	 * and possibly not page aligned if it succeeds (the user
	 * supplied pointer is returned).
	 */
	SCARG(&oba, nsize) = nbrk;

	if ((caddr_t) nbrk > vm->vm_daddr && sys_obreak(p, &oba, retval) == 0)
		retval[0] = (register_t)nbrk;
	else
		retval[0] = (register_t)oldbrk;

	return 0;
}

/*
 * Convert BSD statfs structure to Linux statfs structure.
 * The Linux structure has less fields, and it also wants
 * the length of a name in a dir entry in a field, which
 * we fake (probably the wrong way).
 */
static void
bsd_to_linux_statfs(bsp, lsp)
	struct statfs *bsp;
	struct linux_statfs *lsp;
{

	lsp->l_ftype = bsp->f_type;
	lsp->l_fbsize = bsp->f_bsize;
	lsp->l_fblocks = bsp->f_blocks;
	lsp->l_fbfree = bsp->f_bfree;
	lsp->l_fbavail = bsp->f_bavail;
	lsp->l_ffiles = bsp->f_files;
	lsp->l_fffree = bsp->f_ffree;
	lsp->l_ffsid.val[0] = bsp->f_fsid.val[0];
	lsp->l_ffsid.val[1] = bsp->f_fsid.val[1];
	lsp->l_fnamelen = MAXNAMLEN;	/* XXX */
}

/*
 * Implement the fs stat functions. Straightforward.
 */
int
linux_sys_statfs(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_statfs_args /* {
		syscallarg(const char *) path;
		syscallarg(struct linux_statfs *) sp;
	} */ *uap = v;
	struct statfs btmp, *bsp;
	struct linux_statfs ltmp;
	struct sys_statfs_args bsa;
	caddr_t sg;
	int error;

	sg = stackgap_init(p->p_emul);
	bsp = (struct statfs *) stackgap_alloc(&sg, sizeof (struct statfs));

	LINUX_CHECK_ALT_EXIST(p, &sg, SCARG(uap, path));

	SCARG(&bsa, path) = SCARG(uap, path);
	SCARG(&bsa, buf) = bsp;

	if ((error = sys_statfs(p, &bsa, retval)))
		return error;

	if ((error = copyin((caddr_t) bsp, (caddr_t) &btmp, sizeof btmp)))
		return error;

	bsd_to_linux_statfs(&btmp, &ltmp);

	return copyout((caddr_t) &ltmp, (caddr_t) SCARG(uap, sp), sizeof ltmp);
}

int
linux_sys_fstatfs(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_fstatfs_args /* {
		syscallarg(int) fd;
		syscallarg(struct linux_statfs *) sp;
	} */ *uap = v;
	struct statfs btmp, *bsp;
	struct linux_statfs ltmp;
	struct sys_fstatfs_args bsa;
	caddr_t sg;
	int error;

	sg = stackgap_init(p->p_emul);
	bsp = (struct statfs *) stackgap_alloc(&sg, sizeof (struct statfs));

	SCARG(&bsa, fd) = SCARG(uap, fd);
	SCARG(&bsa, buf) = bsp;

	if ((error = sys_fstatfs(p, &bsa, retval)))
		return error;

	if ((error = copyin((caddr_t) bsp, (caddr_t) &btmp, sizeof btmp)))
		return error;

	bsd_to_linux_statfs(&btmp, &ltmp);

	return copyout((caddr_t) &ltmp, (caddr_t) SCARG(uap, sp), sizeof ltmp);
}

/*
 * uname(). Just copy the info from the various strings stored in the
 * kernel, and put it in the Linux utsname structure. That structure
 * is almost the same as the NetBSD one, only it has fields 65 characters
 * long, and an extra domainname field.
 */
int
linux_sys_uname(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_uname_args /* {
		syscallarg(struct linux_utsname *) up;
	} */ *uap = v;
	extern char ostype[], hostname[], osrelease[], version[], machine[],
	    domainname[];
	struct linux_utsname luts;
	int len;
	char *cp;

	strncpy(luts.l_sysname, ostype, sizeof(luts.l_sysname));
	strncpy(luts.l_nodename, hostname, sizeof(luts.l_nodename));
	strncpy(luts.l_release, osrelease, sizeof(luts.l_release));
	strncpy(luts.l_version, version, sizeof(luts.l_version));
	strncpy(luts.l_machine, machine, sizeof(luts.l_machine));
	strncpy(luts.l_domainname, domainname, sizeof(luts.l_domainname));

	/* This part taken from the the uname() in libc */
	len = sizeof(luts.l_version);
	for (cp = luts.l_version; len--; ++cp) {
		if (*cp == '\n' || *cp == '\t') {
			if (len > 1)
				*cp = ' ';
			else
				*cp = '\0';
		}
	}

	return copyout(&luts, SCARG(uap, up), sizeof(luts));
}

/* Used directly on: alpha, mips, ppc, sparc, sparc64 */
/* Used indirectly on: arm, i386, m68k */

/*
 * New type Linux mmap call.
 * Only called directly on machines with >= 6 free regs.
 */
int
linux_sys_mmap(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_mmap_args /* {
		syscallarg(unsigned long) addr;
		syscallarg(size_t) len;
		syscallarg(int) prot;
		syscallarg(int) flags;
		syscallarg(int) fd;
		syscallarg(off_t) offset;
	} */ *uap = v;
	struct sys_mmap_args cma;
	int flags;
	
	flags = 0;
	flags |= cvtto_bsd_mask(SCARG(uap,flags), LINUX_MAP_SHARED, MAP_SHARED);
	flags |= cvtto_bsd_mask(SCARG(uap,flags), LINUX_MAP_PRIVATE, MAP_PRIVATE);
	flags |= cvtto_bsd_mask(SCARG(uap,flags), LINUX_MAP_FIXED, MAP_FIXED);
	flags |= cvtto_bsd_mask(SCARG(uap,flags), LINUX_MAP_ANON, MAP_ANON);
	/* XXX XAX ERH: Any other flags here?  There are more defined... */

	SCARG(&cma,addr) = (void *)SCARG(uap, addr);
	SCARG(&cma,len) = SCARG(uap, len);
	SCARG(&cma,prot) = SCARG(uap, prot);
	if (SCARG(&cma,prot) & VM_PROT_WRITE) /* XXX */
		SCARG(&cma,prot) |= VM_PROT_READ;
	SCARG(&cma,flags) = flags;
	SCARG(&cma,fd) = SCARG(uap, fd);
	SCARG(&cma,pad) = 0;
	SCARG(&cma,pos) = SCARG(uap, offset);

	return sys_mmap(p, &cma, retval);
}

int
linux_sys_mremap(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_mremap_args /* {
		syscallarg(void *) old_address;
		syscallarg(size_t) old_size;
		syscallarg(size_t) new_size;
		syscallarg(u_long) flags;
	} */ *uap = v;
	struct sys_munmap_args mua;
	size_t old_size, new_size;
	int error;

	old_size = round_page(SCARG(uap, old_size));
	new_size = round_page(SCARG(uap, new_size));

	/*
	 * Growing mapped region.
	 */
	if (new_size > old_size) {
		/*
		 * XXX Implement me.  What we probably want to do is
		 * XXX dig out the guts of the old mapping, mmap that
		 * XXX object again with the new size, then munmap
		 * XXX the old mapping.
		 */
		*retval = 0;
		return (ENOMEM);
	}

	/*
	 * Shrinking mapped region.
	 */
	if (new_size < old_size) {
		SCARG(&mua, addr) = (caddr_t)SCARG(uap, old_address) +
		    new_size;
		SCARG(&mua, len) = old_size - new_size;
		error = sys_munmap(p, &mua, retval);
		*retval = error ? 0 : (register_t)SCARG(uap, old_address);
		return (error);
	}

	/*
	 * No change.
	 */
	*retval = (register_t)SCARG(uap, old_address);
	return (0);
}

int
linux_sys_msync(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_msync_args /* {
		syscallarg(caddr_t) addr;
		syscallarg(int) len;
		syscallarg(int) fl;
	} */ *uap = v;

	struct sys___msync13_args bma;

	/* flags are ignored */
	SCARG(&bma, addr) = SCARG(uap, addr);
	SCARG(&bma, len) = SCARG(uap, len);
	SCARG(&bma, flags) = SCARG(uap, fl);

	return sys___msync13(p, &bma, retval);
}

/*
 * This code is partly stolen from src/lib/libc/compat-43/times.c
 * XXX - CLK_TCK isn't declared in /sys, just in <time.h>, done here
 */

#define CLK_TCK 100
#define	CONVTCK(r)	(r.tv_sec * CLK_TCK + r.tv_usec / (1000000 / CLK_TCK))

int
linux_sys_times(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_times_args /* {
		syscallarg(struct times *) tms;
	} */ *uap = v;
	struct timeval t;
	struct linux_tms ltms;
	struct rusage ru;
	int error, s;

	calcru(p, &ru.ru_utime, &ru.ru_stime, NULL);
	ltms.ltms_utime = CONVTCK(ru.ru_utime);
	ltms.ltms_stime = CONVTCK(ru.ru_stime);

	ltms.ltms_cutime = CONVTCK(p->p_stats->p_cru.ru_utime);
	ltms.ltms_cstime = CONVTCK(p->p_stats->p_cru.ru_stime);

	if ((error = copyout(&ltms, SCARG(uap, tms), sizeof ltms)))
		return error;

	s = splclock();
	timersub(&time, &boottime, &t);
	splx(s);

	retval[0] = ((linux_clock_t)(CONVTCK(t)));
	return 0;
}

/*
 * Linux 'readdir' call. This code is mostly taken from the
 * SunOS getdents call (see compat/sunos/sunos_misc.c), though
 * an attempt has been made to keep it a little cleaner (failing
 * miserably, because of the cruft needed if count 1 is passed).
 *
 * The d_off field should contain the offset of the next valid entry,
 * but in Linux it has the offset of the entry itself. We emulate
 * that bug here.
 *
 * Read in BSD-style entries, convert them, and copy them out.
 *
 * Note that this doesn't handle union-mounted filesystems.
 */
int
linux_sys_getdents(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_getdents_args /* {
		syscallarg(int) fd;
		syscallarg(struct linux_dirent *) dent;
		syscallarg(unsigned int) count;
	} */ *uap = v;
	register struct dirent *bdp;
	struct vnode *vp;
	caddr_t	inp, buf;		/* BSD-format */
	int len, reclen;		/* BSD-format */
	caddr_t outp;			/* Linux-format */
	int resid, linux_reclen = 0;	/* Linux-format */
	struct file *fp;
	struct uio auio;
	struct iovec aiov;
	struct linux_dirent idb;
	off_t off;		/* true file offset */
	int buflen, error, eofflag, nbytes, oldcall;
	struct vattr va;
	off_t *cookiebuf = NULL, *cookie;
	int ncookies;

	/* getvnode() will use the descriptor for us */
	if ((error = getvnode(p->p_fd, SCARG(uap, fd), &fp)) != 0)
		return (error);

	if ((fp->f_flag & FREAD) == 0) {
		error = EBADF;
		goto out1;
	}

	vp = (struct vnode *)fp->f_data;
	if (vp->v_type != VDIR) {
		error = EINVAL;
		goto out1;
	}

	if ((error = VOP_GETATTR(vp, &va, p->p_ucred, p)))
		goto out1;

	nbytes = SCARG(uap, count);
	if (nbytes == 1) {	/* emulating old, broken behaviour */
		nbytes = sizeof (struct linux_dirent);
		buflen = max(va.va_blocksize, nbytes);
		oldcall = 1;
	} else {
		buflen = min(MAXBSIZE, nbytes);
		if (buflen < va.va_blocksize)
			buflen = va.va_blocksize;
		oldcall = 0;
	}
	buf = malloc(buflen, M_TEMP, M_WAITOK);

	vn_lock(vp, LK_EXCLUSIVE | LK_RETRY);
	off = fp->f_offset;
again:
	aiov.iov_base = buf;
	aiov.iov_len = buflen;
	auio.uio_iov = &aiov;
	auio.uio_iovcnt = 1;
	auio.uio_rw = UIO_READ;
	auio.uio_segflg = UIO_SYSSPACE;
	auio.uio_procp = p;
	auio.uio_resid = buflen;
	auio.uio_offset = off;
	/*
         * First we read into the malloc'ed buffer, then
         * we massage it into user space, one record at a time.
         */
	error = VOP_READDIR(vp, &auio, fp->f_cred, &eofflag, &cookiebuf,
	    &ncookies);
	if (error)
		goto out;

	inp = buf;
	outp = (caddr_t)SCARG(uap, dent);
	resid = nbytes;
	if ((len = buflen - auio.uio_resid) == 0)
		goto eof;

	for (cookie = cookiebuf; len > 0; len -= reclen) {
		bdp = (struct dirent *)inp;
		reclen = bdp->d_reclen;
		if (reclen & 3)
			panic("linux_readdir");
		if (bdp->d_fileno == 0) {
			inp += reclen;	/* it is a hole; squish it out */
			off = *cookie++;
			continue;
		}
		linux_reclen = LINUX_RECLEN(&idb, bdp->d_namlen);
		if (reclen > len || resid < linux_reclen) {
			/* entry too big for buffer, so just stop */
			outp++;
			break;
		}
		/*
		 * Massage in place to make a Linux-shaped dirent (otherwise
		 * we have to worry about touching user memory outside of
		 * the copyout() call).
		 */
		idb.d_ino = (linux_ino_t)bdp->d_fileno;
		/*
		 * The old readdir() call misuses the offset and reclen fields.
		 */
		if (oldcall) {
			idb.d_off = (linux_off_t)linux_reclen;
			idb.d_reclen = (u_short)bdp->d_namlen;
		} else {
			if (sizeof (linux_off_t) < 4 && (off >> 32) != 0) {
				compat_offseterr(vp, "linux_getdents");
				error = EINVAL;
				goto out;
			}
			idb.d_off = (linux_off_t)off;
			idb.d_reclen = (u_short)linux_reclen;
		}
		strcpy(idb.d_name, bdp->d_name);
		if ((error = copyout((caddr_t)&idb, outp, linux_reclen)))
			goto out;
		/* advance past this real entry */
		inp += reclen;
		off = *cookie++;	/* each entry points to itself */
		/* advance output past Linux-shaped entry */
		outp += linux_reclen;
		resid -= linux_reclen;
		if (oldcall)
			break;
	}

	/* if we squished out the whole block, try again */
	if (outp == (caddr_t)SCARG(uap, dent))
		goto again;
	fp->f_offset = off;	/* update the vnode offset */

	if (oldcall)
		nbytes = resid + linux_reclen;

eof:
	*retval = nbytes - resid;
out:
	VOP_UNLOCK(vp, 0);
	if (cookiebuf)
		free(cookiebuf, M_TEMP);
	free(buf, M_TEMP);
 out1:
	FILE_UNUSE(fp, p);
	return error;
}

/*
 * Even when just using registers to pass arguments to syscalls you can
 * have 5 of them on the i386. So this newer version of select() does
 * this.
 */
int
linux_sys_select(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_select_args /* {
		syscallarg(int) nfds;
		syscallarg(fd_set *) readfds;
		syscallarg(fd_set *) writefds;
		syscallarg(fd_set *) exceptfds;
		syscallarg(struct timeval *) timeout;
	} */ *uap = v;

	return linux_select1(p, retval, SCARG(uap, nfds), SCARG(uap, readfds),
	    SCARG(uap, writefds), SCARG(uap, exceptfds), SCARG(uap, timeout));
}

/*
 * Common code for the old and new versions of select(). A couple of
 * things are important:
 * 1) return the amount of time left in the 'timeout' parameter
 * 2) select never returns ERESTART on Linux, always return EINTR
 */
int
linux_select1(p, retval, nfds, readfds, writefds, exceptfds, timeout)
	struct proc *p;
	register_t *retval;
	int nfds;
	fd_set *readfds, *writefds, *exceptfds;
	struct timeval *timeout;
{
	struct sys_select_args bsa;
	struct timeval tv0, tv1, utv, *tvp;
	caddr_t sg;
	int error;

	SCARG(&bsa, nd) = nfds;
	SCARG(&bsa, in) = readfds;
	SCARG(&bsa, ou) = writefds;
	SCARG(&bsa, ex) = exceptfds;
	SCARG(&bsa, tv) = timeout;

	/*
	 * Store current time for computation of the amount of
	 * time left.
	 */
	if (timeout) {
		if ((error = copyin(timeout, &utv, sizeof(utv))))
			return error;
		if (itimerfix(&utv)) {
			/*
			 * The timeval was invalid.  Convert it to something
			 * valid that will act as it does under Linux.
			 */
			sg = stackgap_init(p->p_emul);
			tvp = stackgap_alloc(&sg, sizeof(utv));
			utv.tv_sec += utv.tv_usec / 1000000;
			utv.tv_usec %= 1000000;
			if (utv.tv_usec < 0) {
				utv.tv_sec -= 1;
				utv.tv_usec += 1000000;
			}
			if (utv.tv_sec < 0)
				timerclear(&utv);
			if ((error = copyout(&utv, tvp, sizeof(utv))))
				return error;
			SCARG(&bsa, tv) = tvp;
		}
		microtime(&tv0);
	}

	error = sys_select(p, &bsa, retval);
	if (error) {
		/*
		 * See fs/select.c in the Linux kernel.  Without this,
		 * Maelstrom doesn't work.
		 */
		if (error == ERESTART)
			error = EINTR;
		return error;
	}

	if (timeout) {
		if (*retval) {
			/*
			 * Compute how much time was left of the timeout,
			 * by subtracting the current time and the time
			 * before we started the call, and subtracting
			 * that result from the user-supplied value.
			 */
			microtime(&tv1);
			timersub(&tv1, &tv0, &tv1);
			timersub(&utv, &tv1, &utv);
			if (utv.tv_sec < 0)
				timerclear(&utv);
		} else
			timerclear(&utv);
		if ((error = copyout(&utv, timeout, sizeof(utv))))
			return error;
	}

	return 0;
}

/*
 * Get the process group of a certain process. Look it up
 * and return the value.
 */
int
linux_sys_getpgid(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_getpgid_args /* {
		syscallarg(int) pid;
	} */ *uap = v;
	struct proc *targp;

	if (SCARG(uap, pid) != 0 && SCARG(uap, pid) != p->p_pid) {
		if ((targp = pfind(SCARG(uap, pid))) == 0)
			return ESRCH;
	}
	else
		targp = p;

	retval[0] = targp->p_pgid;
	return 0;
}

/*
 * Set the 'personality' (emulation mode) for the current process. Only
 * accept the Linux personality here (0). This call is needed because
 * the Linux ELF crt0 issues it in an ugly kludge to make sure that
 * ELF binaries run in Linux mode, not SVR4 mode.
 */
int
linux_sys_personality(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_personality_args /* {
		syscallarg(int) per;
	} */ *uap = v;

	if (SCARG(uap, per) != 0)
		return EINVAL;
	retval[0] = 0;
	return 0;
}

/*
 * The calls are here because of type conversions.
 */
int
linux_sys_setreuid(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_setreuid_args /* {
		syscallarg(int) ruid;
		syscallarg(int) euid;
	} */ *uap = v;
	struct sys_setreuid_args bsa;
	
	SCARG(&bsa, ruid) = ((linux_uid_t)SCARG(uap, ruid) == (linux_uid_t)-1) ?
		(uid_t)-1 : SCARG(uap, ruid);
	SCARG(&bsa, euid) = ((linux_uid_t)SCARG(uap, euid) == (linux_uid_t)-1) ?
		(uid_t)-1 : SCARG(uap, euid);

	return sys_setreuid(p, &bsa, retval);
}

int
linux_sys_setregid(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_setregid_args /* {
		syscallarg(int) rgid;
		syscallarg(int) egid;
	} */ *uap = v;
	struct sys_setregid_args bsa;
	
	SCARG(&bsa, rgid) = ((linux_gid_t)SCARG(uap, rgid) == (linux_gid_t)-1) ?
		(uid_t)-1 : SCARG(uap, rgid);
	SCARG(&bsa, egid) = ((linux_gid_t)SCARG(uap, egid) == (linux_gid_t)-1) ?
		(uid_t)-1 : SCARG(uap, egid);

	return sys_setregid(p, &bsa, retval);
}

int
linux_sys___sysctl(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys___sysctl_args /* {
		syscallarg(struct linux___sysctl *) lsp;
	} */ *uap = v;
	struct linux___sysctl ls;
	struct sys___sysctl_args bsa;
	int error;

	if ((error = copyin(SCARG(uap, lsp), &ls, sizeof ls)))
		return error;
	SCARG(&bsa, name) = ls.name;
	SCARG(&bsa, namelen) = ls.namelen;
	SCARG(&bsa, old) = ls.old;
	SCARG(&bsa, oldlenp) = ls.oldlenp;
	SCARG(&bsa, new) = ls.new;
	SCARG(&bsa, newlen) = ls.newlen;

	return sys___sysctl(p, &bsa, retval);
}

int
linux_sys_clone(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_clone_args /* {
		syscallarg(int) flags;
		syscallarg(void *) stack;
	} */ *uap = v;
	int flags, sig;

	/*
	 * We don't support the Linux CLONE_PID or CLONE_PTRACE flags.
	 */
	if (SCARG(uap, flags) & (LINUX_CLONE_PID|LINUX_CLONE_PTRACE))
		return (EINVAL);

	flags = 0;

	if (SCARG(uap, flags) & LINUX_CLONE_VM)
		flags |= FORK_SHAREVM;
	if (SCARG(uap, flags) & LINUX_CLONE_FS)
		flags |= FORK_SHARECWD;
	if (SCARG(uap, flags) & LINUX_CLONE_FILES)
		flags |= FORK_SHAREFILES;
	if (SCARG(uap, flags) & LINUX_CLONE_SIGHAND)
		flags |= FORK_SHARESIGS;
	if (SCARG(uap, flags) & LINUX_CLONE_VFORK)
		flags |= FORK_PPWAIT;

	sig = SCARG(uap, flags) & LINUX_CLONE_CSIGNAL;
	if (sig < 0 || sig >= LINUX_NSIG)
		return (EINVAL);
	sig = linux_to_native_sig[sig];

	/*
	 * Note that Linux does not provide a portable way of specifying
	 * the stack area; the caller must know if the stack grows up
	 * or down.  So, we pass a stack size of 0, so that the code
	 * that makes this adjustment is a noop.
	 */
	return (fork1(p, flags, sig, SCARG(uap, stack), 0, retval, NULL));
}

int
linux_sys_setresuid(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_setresuid_args /* {
		syscallarg(uid_t) ruid;
		syscallarg(uid_t) euid;
		syscallarg(uid_t) suid;
	} */ *uap = v;
	struct pcred *pc = p->p_cred;
	uid_t ruid, euid, suid;
	int error;

	ruid = SCARG(uap, ruid);
	euid = SCARG(uap, euid);
	suid = SCARG(uap, suid);

	/*
	 * Note: These checks are a little different than the NetBSD
	 * setreuid(2) call performs.  This precisely follows the
	 * behavior of the Linux kernel.
	 */
	if (ruid != (uid_t)-1 &&
	    ruid != pc->p_ruid &&
	    ruid != pc->pc_ucred->cr_uid &&
	    ruid != pc->p_svuid &&
	    (error = suser(pc->pc_ucred, &p->p_acflag)))
		return (error);

	if (euid != (uid_t)-1 &&
	    euid != pc->p_ruid &&
	    euid != pc->pc_ucred->cr_uid &&
	    euid != pc->p_svuid &&
	    (error = suser(pc->pc_ucred, &p->p_acflag)))
		return (error);

	if (suid != (uid_t)-1 &&
	    suid != pc->p_ruid &&
	    suid != pc->pc_ucred->cr_uid &&
	    suid != pc->p_svuid &&
	    (error = suser(pc->pc_ucred, &p->p_acflag)))
		return (error);

	/*
	 * Now assign the new real, effective, and saved UIDs.
	 * Note that Linux, unlike NetBSD in setreuid(2), does not
	 * set the saved UID in this call unless the user specifies
	 * it.
	 */
	if (ruid != (uid_t)-1) {
		(void)chgproccnt(pc->p_ruid, -1);
		(void)chgproccnt(ruid, 1);
		pc->p_ruid = ruid;
	}

	if (euid != (uid_t)-1) {
		pc->pc_ucred = crcopy(pc->pc_ucred);
		pc->pc_ucred->cr_uid = euid;
	}

	if (suid != (uid_t)-1)
		pc->p_svuid = suid;

	if (ruid != (uid_t)-1 && euid != (uid_t)-1 && suid != (uid_t)-1)
		p->p_flag |= P_SUGID;
	return (0);
}

int
linux_sys_getresuid(p, v, retval)
	struct proc *p;
	void *v;
	register_t *retval;
{
	struct linux_sys_getresuid_args /* {
		syscallarg(uid_t *) ruid;
		syscallarg(uid_t *) euid;
		syscallarg(uid_t *) suid;
	} */ *uap = v;
	struct pcred *pc = p->p_cred;
	int error;

	/*
	 * Linux copies these values out to userspace like so:
	 *
	 *	1. Copy out ruid.
	 *	2. If that succeeds, copy out euid.
	 *	3. If both of those succeed, copy out suid.
	 */
	if ((error = copyout(&pc->p_ruid, SCARG(uap, ruid),
			     sizeof(uid_t))) != 0)
		return (error);

	if ((error = copyout(&pc->pc_ucred->cr_uid, SCARG(uap, euid),
			     sizeof(uid_t))) != 0)
		return (error);

	return (copyout(&pc->p_svuid, SCARG(uap, suid), sizeof(uid_t)));
}

/*	$NetBSD: svr4_stat.c,v 1.6 1994/11/18 02:54:02 christos Exp $	 */

/*
 * Copyright (c) 1994 Christos Zoulas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/namei.h>
#include <sys/proc.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/filedesc.h>
#include <sys/ioctl.h>
#include <sys/kernel.h>
#include <sys/mount.h>
#include <sys/malloc.h>

#include <sys/time.h>
#include <sys/ucred.h>
#include <vm/vm.h>
#include <sys/sysctl.h>

#include <sys/syscallargs.h>

#include <compat/svr4/svr4_types.h>
#include <compat/svr4/svr4_signal.h>
#include <compat/svr4/svr4_syscallargs.h>
#include <compat/svr4/svr4_util.h>
#include <compat/svr4/svr4_stat.h>
#include <compat/svr4/svr4_ustat.h>
#include <compat/svr4/svr4_fuser.h>
#include <compat/svr4/svr4_utsname.h>
#include <compat/svr4/svr4_systeminfo.h>

#define syscallarg(x)	union { x datum; register_t pad; }

static void
bsd_to_svr4_stat(st, st4)
	struct stat		*st;
	struct svr4_stat 	*st4;
{
	bzero(st4, sizeof(*st4));
	st4->st_dev = st->st_dev;
	st4->st_ino = st->st_ino;
	st4->st_mode = st->st_mode;
	st4->st_nlink = st->st_nlink;
	st4->st_uid = st->st_uid;
	st4->st_gid = st->st_gid;
	st4->st_size = st->st_size;
	st4->st_atim = st->st_atimespec.ts_sec;
	st4->st_mtim = st->st_mtimespec.ts_sec;
	st4->st_ctim = st->st_ctimespec.ts_sec;
}


static void
bsd_to_svr4_xstat(st, st4)
	struct stat		*st;
	struct svr4_xstat	*st4;
{
	bzero(st4, sizeof(*st4));
	st4->st_dev = st->st_dev;
	st4->st_ino = st->st_ino;
	st4->st_mode = st->st_mode;
	st4->st_nlink = st->st_nlink;
	st4->st_uid = st->st_uid;
	st4->st_gid = st->st_gid;
	st4->st_rdev = st->st_rdev;
	st4->st_size = st->st_size;
	st4->st_atim = st->st_atimespec;
	st4->st_mtim = st->st_mtimespec;
	st4->st_ctim = st->st_ctimespec;
	st4->st_blksize = st->st_blksize;
	st4->st_blocks = st->st_blocks;
	strcpy(st4->st_fstype, "unknown");
}


int
svr4_stat(p, uap, retval)
	register struct proc		*p;
	register struct svr4_stat_args	*uap;
	register_t			*retval;
{
	struct stat		st;
	struct svr4_stat	svr4_st;
	struct stat_args	cup;
	int			error;

	caddr_t sg = stackgap_init();
	CHECKALT(p, &sg, SCARG(uap, path));

	SCARG(&cup, path) = SCARG(uap, path);
	SCARG(&cup, ub) = stackgap_alloc(&sg, sizeof(struct stat));


	if ((error = stat(p, &cup, retval)) != 0)
		return error;

	if ((error = copyin(SCARG(&cup, ub), &st, sizeof st)) != 0)
		return error;

	bsd_to_svr4_stat(&st, &svr4_st);

	if ((error = copyout(&svr4_st, SCARG(uap, ub), sizeof svr4_st)) != 0)
		return error;

	return 0;
}

int
svr4_lstat(p, uap, retval)
	register struct proc		*p;
	register struct svr4_stat_args 	*uap;
	register_t			*retval;
{
	struct stat		st;
	struct svr4_stat	svr4_st;
	struct stat_args	cup;
	int			error;

	caddr_t sg = stackgap_init();
	CHECKALT(p, &sg, SCARG(uap, path));

	SCARG(&cup, path) = SCARG(uap, path);
	SCARG(&cup, ub) = stackgap_alloc(&sg, sizeof(struct stat));

	if ((error = lstat(p, &cup, retval)) != 0)
		return error;

	if ((error = copyin(SCARG(&cup, ub), &st, sizeof st)) != 0)
		return error;

	bsd_to_svr4_stat(&st, &svr4_st);

	if ((error = copyout(&svr4_st, SCARG(uap, ub), sizeof svr4_st)) != 0)
		return error;

	return 0;
}

int
svr4_fstat(p, uap, retval)
	register struct proc		*p;
	register struct svr4_fstat_args	*uap;
	register_t			*retval;
{
	struct stat		st;
	struct svr4_stat	svr4_st;
	struct fstat_args	cup;
	int			error;

	caddr_t sg = stackgap_init();

	SCARG(&cup, fd) = SCARG(uap, fd);
	SCARG(&cup, sb) = stackgap_alloc(&sg, sizeof(struct stat));

	if ((error = fstat(p, &cup, retval)) != 0)
		return error;

	if ((error = copyin(SCARG(&cup, sb), &st, sizeof st)) != 0)
		return error;

	bsd_to_svr4_stat(&st, &svr4_st);

	if ((error = copyout(&svr4_st, SCARG(uap, sb), sizeof svr4_st)) != 0)
		return error;

	return 0;
}


int
svr4_xstat(p, uap, retval)
	register struct proc		*p;
	register struct svr4_xstat_args	*uap;
	register_t			*retval;
{
	struct stat		st;
	struct svr4_xstat	svr4_st;
	struct stat_args	cup;
	int			error;

	caddr_t sg = stackgap_init();
	CHECKALT(p, &sg, SCARG(uap, path));

	SCARG(&cup, path) = SCARG(uap, path);
	SCARG(&cup, ub) = stackgap_alloc(&sg, sizeof(struct stat));

	if ((error = stat(p, &cup, retval)) != 0)
		return error;

	if ((error = copyin(SCARG(&cup, ub), &st, sizeof st)) != 0)
		return error;

	bsd_to_svr4_xstat(&st, &svr4_st);

	if ((error = copyout(&svr4_st, SCARG(uap, ub), sizeof svr4_st)) != 0)
		return error;

	return 0;
}

int
svr4_lxstat(p, uap, retval)
	register struct proc		*p;
	register struct svr4_xstat_args	*uap;
	register_t			*retval;
{
	struct stat		st;
	struct svr4_xstat	svr4_st;
	struct stat_args	cup;
	int			error;

	caddr_t sg = stackgap_init();
	CHECKALT(p, &sg, SCARG(uap, path));

	SCARG(&cup, path) = SCARG(uap, path);
	SCARG(&cup, ub) = stackgap_alloc(&sg, sizeof(struct stat));

	if ((error = lstat(p, &cup, retval)) != 0)
		return error;

	if ((error = copyin(SCARG(&cup, ub), &st, sizeof st)) != 0)
		return error;

	bsd_to_svr4_xstat(&st, &svr4_st);

	if ((error = copyout(&svr4_st, SCARG(uap, ub), sizeof svr4_st)) != 0)
		return error;

	return 0;
}

int
svr4_fxstat(p, uap, retval)
	register struct proc			*p;
	register struct svr4_fxstat_args	*uap;
	register_t				*retval;
{
	struct stat		st;
	struct svr4_xstat	svr4_st;
	struct fstat_args	cup;
	int			error;

	caddr_t sg = stackgap_init();

	SCARG(&cup, fd) = SCARG(uap, fd);
	SCARG(&cup, sb) = stackgap_alloc(&sg, sizeof(struct stat));

	if ((error = fstat(p, &cup, retval)) != 0)
		return error;

	if ((error = copyin(SCARG(&cup, sb), &st, sizeof st)) != 0)
		return error;

	bsd_to_svr4_xstat(&st, &svr4_st);

	if ((error = copyout(&svr4_st, SCARG(uap, sb), sizeof svr4_st)) != 0)
		return error;

	return 0;
}

struct svr4_ustat_args {
	syscallarg(struct svr4_ustat *) name;
};

int
svr4_ustat(p, uap, retval)
	register struct proc		*p;
	register struct svr4_ustat_args	*uap;
	register_t			*retval;
{
	struct svr4_ustat	us;
	int			error;

	bzero(&us, sizeof us);

	/*
         * XXX: should set f_tfree and f_tinode at least
         * How do we translate dev -> fstat? (and then to svr4_ustat)
         */
	if (error = copyout(&us, SCARG(uap, name), sizeof us))
		return (error);

	return 0;
}



int
svr4_uname(p, uap, retval)
	register struct proc		*p;
	register struct svr4_uname_args	*uap;
	register_t			*retval;
{
	struct svr4_utsname	sut;
	extern struct utsname	utsname;

	/* first update utsname just as with NetBSD uname() */
	bcopy(hostname, utsname.nodename, sizeof(utsname.nodename));
	utsname.nodename[sizeof(utsname.nodename) - 1] = '\0';

	/* then copy it over into SVR4 struct utsname */
	bzero(&sut, sizeof(sut));
	bcopy(utsname.sysname, sut.sysname, sizeof(utsname.sysname));
	bcopy(utsname.nodename, sut.nodename, sizeof(utsname.nodename));
	bcopy(utsname.release, sut.release, sizeof(utsname.release));
	bcopy(utsname.version, sut.version, sizeof(utsname.version));
	bcopy(utsname.machine, sut.machine, sizeof(utsname.machine));

	return copyout((caddr_t) & sut, (caddr_t) SCARG(uap, name),
		       sizeof(struct svr4_utsname));
}

int
svr4_systeminfo(p, uap, retval)
	register struct proc			*p;
	register struct svr4_systeminfo_args	*uap;
	register_t				*retval;
{
	extern struct utsname	utsname;
	char *str;
	int name;
	int error;
	long len;
	u_int rlen = SCARG(uap, len);

	switch (SCARG(uap, what)) {
	case SVR4_SI_SYSNAME:
		str = utsname.sysname;
		break;

	case SVR4_SI_HOSTNAME:
		name = KERN_HOSTNAME;
		return kern_sysctl(&name, 1, SCARG(uap, buf), &rlen, 0, 0);

	case SVR4_SI_RELEASE:
		str = utsname.release;
		break;

	case SVR4_SI_VERSION:
		str = utsname.version;
		break;

	case SVR4_SI_MACHINE:
		str = utsname.machine;
		break;

	case SVR4_SI_ARCHITECTURE:
		str = utsname.machine;
		break;

	case SVR4_SI_HW_SERIAL:
		str = "0";
		break;

	case SVR4_SI_HW_PROVIDER:
		str = utsname.sysname;
		break;

	case SVR4_SI_SRPC_DOMAIN:
		name = KERN_DOMAINNAME;
		return kern_sysctl(&name, 1, SCARG(uap, buf), &rlen, 0, 0);

	case SVR4_SI_SET_HOSTNAME:
		if (error = suser(p->p_ucred, &p->p_acflag))
			return error;
		name = KERN_HOSTNAME;
		return kern_sysctl(&name, 1, 0, 0, SCARG(uap, buf), rlen);

	case SVR4_SI_SET_SRPC_DOMAIN:
		if (error = suser(p->p_ucred, &p->p_acflag))
			return error;
		name = KERN_DOMAINNAME;
		return kern_sysctl(&name, 1, 0, 0, SCARG(uap, buf), rlen);

	default:
		DPRINTF(("Bad systeminfo command %d\n", SCARG(uap, what)));
		return ENOSYS;
	}

	len = strlen(str) + 1;
	if (len > SCARG(uap, len))
		len = SCARG(uap, len);

	return copyout(str, SCARG(uap, buf), len);
}

#ifdef notyet
struct svr4_utssys_args {
	union {
		struct svr4_ustat_args ustat_args;
		struct svr4_uname_args uname_args;
		struct svr4_fuser_args fuser_args;
	} u;
	int sel;
	struct svr4_f_user *fuser;
};
#endif


int
svr4_utssys(p, uap, retval)
	register struct proc 			*p;
	register struct svr4_utssys_args	*uap;
	register_t 				*retval;
{
#ifdef notyet
	switch (SCARG(uap, sel)) {
	case 0:		/* uname(2) */
		return svr4_uname(p, &SCARG(uap, u).uname_args, retval);
	case 2:		/* ustat(2) */
		return svr4_ustat(p, &SCARG(uap, u).ustat_args, retval);

	default:
		return ENOSYS;
	}
#endif
	return ENOSYS;
}

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/mount.h>
#include <sys/namei.h>
#include <sys/vnode.h>
#include <sys/malloc.h>
#include <sys/select.h>

#include <miscfs/genfs/genfs.h>

int
genfs_select(v)
	void *v;
{
#if 0
	struct vop_select_args /* {
		struct vnode *a_vp;
		int a_which;
		int a_fflags;
		struct ucred *a_cred;
		struct proc *a_p;
	} */ *ap = v;
#endif

	return (1);
}

int
genfs_fsync(v)
	void *v;
{
	struct vop_fsync_args /* {
		struct vnode *a_vp;
		struct ucred *a_cred;
		int a_waitfor;
		struct proc *a_p;
	} */ *ap = v;
	register struct vnode *vp = ap->a_vp;
	struct timespec ts;

	vflushbuf(vp, ap->a_waitfor == MNT_WAIT);
	TIMEVAL_TO_TIMESPEC(&time, &ts);
	return (VOP_UPDATE(ap->a_vp, &ts, &ts, ap->a_waitfor == MNT_WAIT));
}

int
genfs_abortop(v)
	void *v;
{
	struct vop_abortop_args /* {
		struct vnode *a_dvp;
		struct componentname *a_cnp;
	} */ *ap = v;
 
	if ((ap->a_cnp->cn_flags & (HASBUF | SAVESTART)) == HASBUF)
		FREE(ap->a_cnp->cn_pnbuf, M_NAMEI);
	return (0);
}

/*ARGSUSED*/
int
genfs_badop(v)
	void *v;
{

	panic("genfs: bad op");
}

/*ARGSUSED*/
int
genfs_nullop(v)
	void *v;
{

	return (0);
}

/*ARGSUSED*/
int
genfs_eopnotsupp(v)
	void *v;
{

	return (EOPNOTSUPP);
}

/*ARGSUSED*/
int
genfs_ebadf(v)
	void *v;
{

	return (EBADF);
}

/*	$NetBSD: lfs_balloc.c,v 1.16 2000/05/05 20:59:21 perseant Exp $	*/

/*-
 * Copyright (c) 1999 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Konrad E. Schroder <perseant@hhhh.org>.
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
 *      This product includes software developed by the NetBSD
 *      Foundation, Inc. and its contributors.
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
 * Copyright (c) 1989, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)lfs_balloc.c	8.4 (Berkeley) 5/8/95
 */

#if defined(_KERNEL) && !defined(_LKM)
#include "opt_quota.h"
#endif

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/proc.h>
#include <sys/vnode.h>
#include <sys/mount.h>
#include <sys/resourcevar.h>
#include <sys/trace.h>

#include <miscfs/specfs/specdev.h>

#include <ufs/ufs/quota.h>
#include <ufs/ufs/inode.h>
#include <ufs/ufs/ufsmount.h>
#include <ufs/ufs/ufs_extern.h>

#include <ufs/lfs/lfs.h>
#include <ufs/lfs/lfs_extern.h>

#include <vm/vm.h>

#include <uvm/uvm_extern.h>

int lfs_fragextend __P((struct vnode *, int, int, ufs_daddr_t, struct buf **));

/*
 * Allocate a block, and to inode and filesystem block accounting for it
 * and for any indirect blocks the may need to be created in order for
 * this block to be created.
 *
 * Blocks which have never been accounted for (i.e., which "do not exist")
 * have disk address 0, which is translated by ufs_bmap to the special value
 * UNASSIGNED == -1, as in the historical UFS.
 * 
 * Blocks which have been accounted for but which have not yet been written
 * to disk are given the new special disk address UNWRITTEN == -2, so that
 * they can be differentiated from completely new blocks.
 */
int
lfs_balloc(v)
	void *v;
{
	struct vop_balloc_args /* {
		struct vnode *a_vp;
		off_t a_startoffset;
		int a_size;
		struct ucred *a_cred;
		int a_flags;
		struct buf *a_bpp;
	} */ *ap = v;
	struct vnode *vp;
	int offset;
	u_long iosize;
	daddr_t lbn;
	struct buf *ibp, *bp;
	struct inode *ip;
	struct lfs *fs;
	struct indir indirs[NIADDR+2], *idp;
	ufs_daddr_t	daddr, lastblock;
	int bb;		/* number of disk blocks in a block disk blocks */
	int error, frags, i, nsize, osize, num;

	vp = ap->a_vp;	
	ip = VTOI(vp);
	fs = ip->i_lfs;
	offset = blkoff(fs, ap->a_startoffset);
	iosize = ap->a_size;
	lbn = lblkno(fs, ap->a_startoffset);
	(void)lfs_check(vp, lbn, 0);
	
#ifdef DEBUG
	if(!VOP_ISLOCKED(vp)) {
		printf("lfs_balloc: warning: ino %d not locked\n",ip->i_number);
	}
#endif
	
	/* 
	 * Three cases: it's a block beyond the end of file, it's a block in
	 * the file that may or may not have been assigned a disk address or
	 * we're writing an entire block.  Note, if the daddr is unassigned,
	 * the block might still have existed in the cache (if it was read
	 * or written earlier).	 If it did, make sure we don't count it as a
	 * new block or zero out its contents.	If it did not, make sure
	 * we allocate any necessary indirect blocks.
	 * If we are writing a block beyond the end of the file, we need to
	 * check if the old last block was a fragment.	If it was, we need
	 * to rewrite it.
	 */
	
	*ap->a_bpp = NULL;
	error = ufs_bmaparray(vp, lbn, &daddr, &indirs[0], &num, NULL );
	if (error)
		return (error);
	
	/* Check for block beyond end of file and fragment extension needed. */
	lastblock = lblkno(fs, ip->i_ffs_size);
	if (lastblock < NDADDR && lastblock < lbn) {
		osize = blksize(fs, ip, lastblock);
		if (osize < fs->lfs_bsize && osize > 0) {
			if ((error = lfs_fragextend(vp, osize, fs->lfs_bsize,
						    lastblock, &bp)))
				return(error);
			ip->i_ffs_size = (lastblock + 1) * fs->lfs_bsize;
			uvm_vnp_setsize(vp, ip->i_ffs_size);
			ip->i_flag |= IN_CHANGE | IN_UPDATE;
			VOP_BWRITE(bp);
		}
	}
	
	bb = VFSTOUFS(vp->v_mount)->um_seqinc;
	if (daddr == UNASSIGNED) {
		if (num > 0 && ip->i_ffs_ib[indirs[0].in_off] == 0) {
			ip->i_ffs_ib[indirs[0].in_off] = UNWRITTEN;
		}
		/* May need to allocate indirect blocks */
		for (i = 1; i < num; ++i) {
			ibp = getblk(vp, indirs[i].in_lbn, fs->lfs_bsize, 0,0);
			if (!indirs[i].in_exists) {
#ifdef DIAGNOSTIC
				if ((ibp->b_flags & (B_DONE | B_DELWRI)))
					panic ("Indirect block should not exist");
#endif

				if (!ISSPACE(fs, bb, curproc->p_ucred)){
					ibp->b_flags |= B_INVAL;
					brelse(ibp);
					/* XXX might leave some UNWRITTENs hanging, do this better */
					return(ENOSPC);
				} else {
					ip->i_ffs_blocks += bb;
					ip->i_lfs->lfs_bfree -= bb;
					clrbuf(ibp);
					((ufs_daddr_t *)ibp->b_data)[indirs[i].in_off] = UNWRITTEN;
					ibp->b_blkno = UNWRITTEN;
				}
			} else {
				/*
				 * This block exists, but the next one may not.
				 * If that is the case mark it UNWRITTEN to
				 * keep the accounting straight.
				 */
				if ( ((ufs_daddr_t *)ibp->b_data)[indirs[i].in_off] == 0) {
					((ufs_daddr_t *)ibp->b_data)[indirs[i].in_off] = UNWRITTEN;
				}
			}
			if ((error = VOP_BWRITE(ibp)))
				return(error);
		}
	}	
	/*
	 * If the block we are writing is a direct block, it's the last
	 * block in the file, and offset + iosize is less than a full
	 * block, we can write one or more fragments.  There are two cases:
	 * the block is brand new and we should allocate it the correct
	 * size or it already exists and contains some fragments and
	 * may need to extend it.
	 */
	if (lbn < NDADDR && lblkno(fs, ip->i_ffs_size) <= lbn) {
		osize = blksize(fs, ip, lbn);
		nsize = fragroundup(fs, offset + iosize);
		frags = numfrags(fs, nsize);
		bb = fragstodb(fs, frags);
		if (lblktosize(fs, lbn) >= ip->i_ffs_size)
			/* Brand new block or fragment */
			*ap->a_bpp = bp = getblk(vp, lbn, nsize, 0, 0);
		else {
			if (nsize <= osize) {
				/* No need to extend */
				/* XXX KS - Are we wasting space? */
				if ((error = bread(vp, lbn, osize, NOCRED, &bp)))
					return error;
			} else {
				/* Extend existing block */
				if ((error =
				     lfs_fragextend(vp, osize, nsize, lbn, &bp)))
					return(error);
			}
			*ap->a_bpp = bp;
		}
	} else {
		/*
		 * Get the existing block from the cache either because the
		 * block 1) is not a direct block or 2) is not the last
		 * block in the file.
		 */
		frags = dbtofrags(fs, bb);
		*ap->a_bpp = bp = getblk(vp, lbn, blksize(fs, ip, lbn), 0, 0);
	}
	
	/* 
	 * The block we are writing may be a brand new block
	 * in which case we need to do accounting (i.e. check
	 * for free space and update the inode number of blocks.
	 *
	 * We can tell a truly new block because (1) ufs_bmaparray
	 * will say it is UNASSIGNED.  Once we allocate it we will assign
	 * it the disk address UNWRITTEN.
	 */
	if (daddr == UNASSIGNED) {
		if (!ISSPACE(fs, bb, curproc->p_ucred)) {
			bp->b_flags |= B_INVAL;
			brelse(bp);
			return(ENOSPC);
		} else {
			ip->i_ffs_blocks += bb;
			ip->i_lfs->lfs_bfree -= bb;
			if (iosize != fs->lfs_bsize)
				clrbuf(bp);

			/* Note the new address */
			bp->b_blkno = UNWRITTEN;
			
			switch (num) {
			    case 0:
				ip->i_ffs_db[lbn] = UNWRITTEN;
				break;
			    case 1:
				ip->i_ffs_ib[indirs[0].in_off] = UNWRITTEN;
				break;
			    default:
				idp = &indirs[num - 1];
				if (bread(vp, idp->in_lbn, fs->lfs_bsize,
					  NOCRED, &ibp))
					panic("lfs_balloc: bread bno %d",
					      idp->in_lbn);
				((ufs_daddr_t *)ibp->b_data)[idp->in_off] =
					UNWRITTEN;
				VOP_BWRITE(ibp);
			}
		}
	} else if (!(bp->b_flags & (B_DONE|B_DELWRI))) {
		/*
		 * Not a brand new block, also not in the cache;
		 * read it in from disk.
		 */
		if (iosize == fs->lfs_bsize)
			/* Optimization: I/O is unnecessary. */
			bp->b_blkno = daddr;
		else {
			/*
			 * We need to read the block to preserve the
			 * existing bytes.
			 */
			bp->b_blkno = daddr;
			bp->b_flags |= B_READ;
			VOP_STRATEGY(bp);
			return(biowait(bp));
		}
	}
	
	return (0);
}

int
lfs_fragextend(vp, osize, nsize, lbn, bpp)
	struct vnode *vp;
	int osize;
	int nsize;
	ufs_daddr_t lbn;
	struct buf **bpp;
{
	struct inode *ip;
	struct lfs *fs;
	long bb;
	int error;
	extern long locked_queue_bytes;
	struct buf *ibp;
	SEGUSE *sup;

	ip = VTOI(vp);
	fs = ip->i_lfs;
	
	bb = (long)fragstodb(fs, numfrags(fs, nsize - osize));
 top:
	if (!ISSPACE(fs, bb, curproc->p_ucred)) {
		return(ENOSPC);
	}
	if ((error = bread(vp, lbn, osize, NOCRED, bpp))) {
		brelse(*bpp);
		return(error);
	}

	/*
 	 * Fix the allocation for this fragment so that it looks like the
         * source segment contained a block of the new size.  This overcounts;
	 * but the overcount only lasts until the block in question
	 * is written, so the on-disk live bytes count is always correct.
	 */
	LFS_SEGENTRY(sup, fs, datosn(fs,(*bpp)->b_blkno), ibp);
	sup->su_nbytes += (nsize-osize);
	VOP_BWRITE(ibp);

#ifdef QUOTA
	if ((error = chkdq(ip, bb, curproc->p_ucred, 0))) {
		brelse(*bpp);
		return (error);
	}
#endif
	/*
	 * XXX - KS - Don't change size while we're gathered, as we could
	 * then overlap another buffer in lfs_writeseg.
	 */
	if((*bpp)->b_flags & B_GATHERED) {
		(*bpp)->b_flags |= B_NEEDCOMMIT; /* XXX KS - what flag to use? */
		brelse(*bpp);
		tsleep(*bpp, (PRIBIO+1), "lfs_fragextend", 0);
		goto top;
	}
	ip->i_ffs_blocks += bb;
	ip->i_flag |= IN_CHANGE | IN_UPDATE;
	fs->lfs_bfree -= fragstodb(fs, numfrags(fs, (nsize - osize)));
	if((*bpp)->b_flags & B_LOCKED)
		locked_queue_bytes += (nsize - osize);
	allocbuf(*bpp, nsize);
	bzero((char *)((*bpp)->b_data) + osize, (u_int)(nsize - osize));
	
	return(0);
}

/*	$NetBSD: linux32_misc.c,v 1.6 2007/04/22 08:29:57 dsl Exp $	*/

/*-
 * Copyright (c) 1995, 1998, 1999 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Frank van der Linden and Eric Haszlakiewicz; by Jason R. Thorpe
 * of the Numerical Aerospace Simulation Facility, NASA Ames Research Center;
 * by Edgar Fu\ss, Mathematisches Institut der Uni Bonn.
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

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: linux32_misc.c,v 1.6 2007/04/22 08:29:57 dsl Exp $");

#include <sys/param.h>
#include <sys/proc.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/malloc.h>

#include <compat/netbsd32/netbsd32.h>
#include <compat/netbsd32/netbsd32_syscallargs.h>

#include <compat/linux32/common/linux32_types.h>
#include <compat/linux32/common/linux32_signal.h>
#include <compat/linux32/linux32_syscallargs.h>

#include <compat/linux/common/linux_types.h>
#include <compat/linux/common/linux_signal.h>
#include <compat/linux/common/linux_misc.h>
#include <compat/linux/common/linux_statfs.h>
#include <compat/linux/linux_syscallargs.h>

extern const struct linux_mnttypes linux_fstypes[];
extern const int linux_fstypes_cnt;


/*
 * Implement the fs stat functions. Straightforward.
 */
int
linux32_sys_statfs(l, v, retval)
	struct lwp *l;
	void *v;
	register_t *retval;
{
	struct linux32_sys_statfs_args /* {
		syscallarg(const netbsd32_charp char) path;
		syscallarg(linux32_statfsp) sp;
	} */ *uap = v;
	struct proc *p = l->l_proc;
	struct statvfs *btmp, *bsp;
	struct linux_statfs ltmp;
	struct sys_statvfs1_args bsa;
	void *sg;
	int error;

	sg = stackgap_init(p, 0);
	bsp = stackgap_alloc(p, &sg, sizeof (struct statvfs));

	NETBSD32TOP(uap, &bsa, path, const char);

	SCARG(&bsa, buf) = bsp;
	SCARG(&bsa, flags) = ST_WAIT;

	if ((error = sys_statvfs1(l, &bsa, retval)))
		return error;

	btmp = STATVFSBUF_GET();
	error = copyin(bsp, btmp, sizeof(*btmp));
	if (error) {
		goto out;
	}
	bsd_to_linux_statfs(btmp, &ltmp);
	error = copyout(&ltmp, SCARG_P32(uap, sp), sizeof ltmp);
out:
	STATVFSBUF_PUT(btmp);
	return error;
}

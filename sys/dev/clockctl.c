/*      $NetBSD: clockctl.c,v 1.26 2009/02/18 17:57:11 mrg Exp $ */

/*-
 * Copyright (c) 2001 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Emmanuel Dreyfus.
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
 *    derived from this software without specific prior written permission.
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

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: clockctl.c,v 1.26 2009/02/18 17:57:11 mrg Exp $");

#include "opt_ntp.h"
#include "opt_compat_netbsd.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/device.h>
#include <sys/time.h>
#include <sys/conf.h>
#ifdef NTP
#include <sys/timex.h>
#endif /* NTP */

#include <sys/clockctl.h>
#ifdef COMPAT_50
#include <compat/sys/clockctl.h>
#endif

struct clockctl_softc {
	struct device   clockctl_dev;
};

dev_type_ioctl(clockctlioctl);

const struct cdevsw clockctl_cdevsw = {
	nullopen, nullclose, noread, nowrite, clockctlioctl,
	nostop, notty, nopoll, nommap, nokqfilter, D_OTHER,
};

/*ARGSUSED*/
void
clockctlattach(int num)
{
	/* Nothing to set up before open is called */
	return;
}

int
clockctlioctl(
    dev_t dev,
    u_long cmd,
    void *data,
    int flags,
    struct lwp *l)
{
	int error = 0;

	switch (cmd) {
	case CLOCKCTL_SETTIMEOFDAY: {
		struct clockctl_settimeofday *args = data;

		error = settimeofday1(args->tv, true, args->tzp, l, false);
		break;
	}
	case CLOCKCTL_ADJTIME: {
		struct timeval atv, oldatv;
		struct clockctl_adjtime *args = data;

		if (args->delta) {
			error = copyin(args->delta, &atv, sizeof(*args->delta));
			if (error)
				return (error);
		}
		adjtime1(args->delta ? &atv : NULL,
		    args->olddelta ? &oldatv : NULL, l->l_proc);
		if (args->olddelta)
			error = copyout(&oldatv, args->olddelta,
			    sizeof(args->olddelta));
		break;
	}
	case CLOCKCTL_CLOCK_SETTIME: {
		struct clockctl_clock_settime *args = data;
		struct timespec ts;

		error = copyin(args->tp, &ts, sizeof ts);
		if (error)
			return (error);
		error = clock_settime1(l->l_proc, args->clock_id, &ts, false);
		break;
	}
#ifdef NTP
	case CLOCKCTL_NTP_ADJTIME: {
		struct clockctl_ntp_adjtime *args = data;
		struct timex ntv;
		register_t retval;

		error = copyin(args->tp, &ntv, sizeof(ntv));
		if (error)
			return (error);

		ntp_adjtime1(&ntv);

		error = copyout(&ntv, args->tp, sizeof(ntv));
		if (error == 0)
			error = copyout(&retval, &args->retval, sizeof(retval));
	}
#endif /* NTP */
	default:
#ifdef COMPAT_50
		error = compat50_clockctlioctl(dev, cmd, data, flags, l);
#else
		error = EINVAL;
#endif
	}

	return (error);
}



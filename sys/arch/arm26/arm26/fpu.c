/*	$NetBSD: fpu.c,v 1.1 2000/12/23 13:37:03 bjh21 Exp $	*/

/*-
 * Copyright (c) 2000 Ben Harris
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
/* This file is part of NetBSD/arm26 -- a port of NetBSD to ARM2/3 machines. */
/*
 * fpu.c - Floating point unit support
 */

#include <sys/param.h>

__KERNEL_RCSID(0, "$NetBSD: fpu.c,v 1.1 2000/12/23 13:37:03 bjh21 Exp $");

#include <sys/device.h>
#include <sys/proc.h>
#include <sys/systm.h>
#include <sys/user.h>
#include <machine/fpureg.h>
#include <machine/pcb.h>

#include "opt_fputypes.h"

static int fpu_match(struct device *, struct cfdata *, void *);
static void fpu_attach(struct device *, struct device *, void *);
static register_t fpu_identify(void);

register_t fpu_type;

struct fpu_softc {
	struct device sc_dev;
};

struct cfattach fpu_ca = {
	sizeof(struct fpu_softc), fpu_match, fpu_attach
};

/* cf_flags bits */
#define CFF_ENABLE	0x00000001

static int
fpu_match(struct device *parent, struct cfdata *cf, void *aux)
{

	if (cf->cf_unit != 0)
		return 0;
	return fpu_identify() != 0;
}

static void
fpu_attach(struct device *parent, struct device *self, void *aux)
{
	int supported;

	printf(": ");
	fpu_type = fpu_identify();
	supported = 0;
	switch (fpu_type) {
	case FPSR_SYSID_FPPC:
		printf("FPPC/WE32206");
#ifdef FPU_FPPC
		/* XXX Uncomment when we have minimal support. */
		/* supported = 1; */
#endif
		break;
	case FPSR_SYSID_FPA:
		printf("FPA");
#ifdef FPU_FPA
		/* XXX Uncomment when we have minimal support. */
		/* supported = 1; */
#endif
		break;
	default:
		printf("Unknown type, ID=0x%02x", fpu_type >> 24);
		break;
	}
	printf("\n");
	if (!supported)
		printf("%s: WARNING: FPU type not supported by kernel\n",
		       self->dv_xname);
}
	
static register_t
fpu_identify()
{
	label_t here;
	volatile register_t fpsr;

	if (setjmp(&here) == 0) {
		curproc->p_addr->u_pcb.pcb_onundef_lj = &here;
		fpsr = 0;
		asm volatile ("rfs %0" : "=r" (fpsr));
	}
	curproc->p_addr->u_pcb.pcb_onundef_lj = NULL;
	return fpsr & FPSR_SYSID_MASK;
}

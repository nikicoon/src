/*	$NetBSD: pms_iomd.c,v 1.1 1997/10/14 11:16:38 mark Exp $	*/

/*-
 * Copyright (c) 1996 D.C. Tsen
 * Copyright (c) 1994 Charles Hannum.
 * Copyright (c) 1992, 1993 Erik Forsberg.
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the RiscBSD team.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	from:pms.c,v 1.24 1995/12/24 02:30:28 mycroft Exp
 */

/*
 * Ported from 386 version of PS/2 mouse driver.
 * D.C. Tsen
 */

#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/malloc.h>
#include <sys/proc.h>
#include <sys/device.h>
#include <sys/select.h>
#include <sys/tty.h>

#include <machine/bus.h>
#include <machine/irqhandler.h>
#include <arm32/dev/pmsvar.h>
#include <arm32/iomd/iomdvar.h>

static int  pms_iomd_probe     __P((struct device *, struct cfdata *, void *));
static void pms_iomd_attach    __P((struct device *, struct device *, void *));
static void pms_iomd_intenable __P((struct pms_softc *sc, int enable));

struct cfattach pms_iomd_ca = {
	sizeof(struct pms_softc), pms_iomd_probe, pms_iomd_attach
};

static int
pms_iomd_probe(parent, cf, aux)
	struct device *parent;
	struct cfdata *cf;
	void *aux;
{
	struct pms_attach_args *pa = aux;

	if (strcmp(pa->pa_name, "pms") == 0)
		return(1);

	return(0);
}
static void
pms_iomd_attach(parent, self, aux)
	struct device *parent, *self;
	void *aux;
{
	struct pms_softc *sc = (void *)self;
	struct pms_attach_args *pa = aux;

	sc->sc_iot = pa->pa_iot;
	sc->sc_ioh = pa->pa_ioh;
	sc->sc_irqnum = pa->pa_irq;

	sc->sc_intenable = pms_iomd_intenable;

	if (pmsinit(sc) != 1) {
		printf("Mouse not present\n");
	}

	printf("\n");
}

static void
pms_iomd_intenable(sc, enable)
	struct pms_softc *sc;
	int enable;
{
	if (enable) {
		sc->sc_ih = intr_claim(sc->sc_irqnum, IPL_TTY, "pms", pmsintr, sc);
		if (!sc->sc_ih)
			panic("%s: Cannot claim interrupt\n", sc->sc_dev.dv_xname);
	} else {
		if (intr_release(sc->sc_ih) != 0)
			panic("%s: Cannot release IRQ\n", sc->sc_dev.dv_xname);
	}
}

/* End of pms_iomd.c */

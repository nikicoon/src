/*	$NetBSD: com_obio.c,v 1.15 2003/07/15 00:04:53 lukem Exp $	*/

/*-
 * Copyright (c) 1998 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Charles M. Hannum.
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
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
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

/*-
 * Copyright (c) 1991 The Regents of the University of California.
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
 *	@(#)com.c	7.5 (Berkeley) 5/16/91
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: com_obio.c,v 1.15 2003/07/15 00:04:53 lukem Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/tty.h>
#include <sys/proc.h>
#include <sys/user.h>
#include <sys/conf.h>
#include <sys/file.h>
#include <sys/uio.h>
#include <sys/kernel.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/device.h>
#include <sys/termios.h>

#include <machine/bus.h>
#include <machine/autoconf.h>
#include <machine/intr.h>

#include <dev/ic/comreg.h>
#include <dev/ic/comvar.h>

#include <sparc/sparc/auxreg.h>

struct com_obio_softc {
	struct com_softc osc_com;	/* real "com" softc */

	int osc_tadpole;		/* is this on a tadpole */
	/* OBIO-specific goo. */
	struct evcnt osc_intrcnt;	/* interrupt counting */
};

static int com_obio_match __P((struct device *, struct cfdata *, void *));
static void com_obio_attach __P((struct device *, struct device *, void *));
static void com_obio_cleanup __P((void *));

CFATTACH_DECL(com_obio, sizeof(struct com_obio_softc),
    com_obio_match, com_obio_attach, NULL, NULL);

static int
com_obio_match(parent, cf, aux)
	struct device *parent;
	struct cfdata *cf;
	void *aux;
{
	union obio_attach_args *uoba = aux;
	struct sbus_attach_args *sa = &uoba->uoba_sbus;
	int tadpole = 0;
	int need_probe = 0;
	int rv = 0;
	u_int8_t auxregval;

	if (uoba->uoba_isobio4 != 0) {
		return (0);
	}

	/* Tadpole 3GX/3GS uses "modem" for a 16450 port
	 * (We need to enable it before probing)
	 */
	if (strcmp("modem", sa->sa_name) == 0) {
		auxregval = *AUXIO4M_REG;
		*AUXIO4M_REG = auxregval | (AUXIO4M_LED|AUXIO4M_LTE);
		DELAY(100);
		tadpole = 1;
		need_probe = 1;
	}

	/* Sun JavaStation 1 uses "su" for a 16550 port
	 */
	if (strcmp("su", sa->sa_name) == 0) {
		need_probe = 1;
	}

	if (need_probe) {
		bus_space_handle_t ioh;

		if (sbus_bus_map(sa->sa_bustag,
				 sa->sa_slot, sa->sa_offset, sa->sa_size,
				 BUS_SPACE_MAP_LINEAR, &ioh) == 0) {
			rv = comprobe1(sa->sa_bustag, ioh);
#if 0
			printf("modem: probe: lcr=0x%02x iir=0x%02x\n",
				bus_space_read_1(sa->sa_bustag, ioh, 3),
				bus_space_read_1(sa->sa_bustag, ioh, 2));
#endif
			bus_space_unmap(sa->sa_bustag, ioh, sa->sa_size);
		}
	}

	/* Disable the com port if tadpole */
	if (tadpole)
		*AUXIO4M_REG = auxregval;

	return (rv);
}

static void
com_obio_attach(parent, self, aux)
	struct device *parent, *self;
	void *aux;
{
	struct com_obio_softc *osc = (void *)self;
	struct com_softc *sc = &osc->osc_com;
	union obio_attach_args *uoba = aux;
	struct sbus_attach_args *sa = &uoba->uoba_sbus;

	if (strcmp("modem", sa->sa_name) == 0) {
		osc->osc_tadpole = 1;
	}

	/*
	 * We're living on an obio that looks like an sbus slot.
	 */
	sc->sc_iot = sa->sa_bustag;
	sc->sc_iobase = sa->sa_offset;
	sc->sc_frequency = COM_FREQ;

	/*
	 * XXX: It would be nice to be able to split console input and
	 * output to different devices.  For now switch to serial
	 * console if PROM stdin is on serial (so that we can use DDB).
	 */
	if (prom_instance_to_package(prom_stdin()) == sa->sa_node)
		comcnattach(sc->sc_iot, sc->sc_iobase,
			    B9600, sc->sc_frequency, COM_TYPE_NORMAL,
			    (CLOCAL | CREAD | CS8));

	if (!com_is_console(sc->sc_iot, sc->sc_iobase, &sc->sc_ioh) &&
	    sbus_bus_map(sc->sc_iot,
			 sa->sa_slot, sc->sc_iobase, sa->sa_size,
			 BUS_SPACE_MAP_LINEAR, &sc->sc_ioh) != 0) {
		printf(": can't map registers\n");
		return;
	}

	if (osc->osc_tadpole) {
		*AUXIO4M_REG |= (AUXIO4M_LED|AUXIO4M_LTE);
		do {
			DELAY(100);
		} while (!comprobe1(sc->sc_iot, sc->sc_ioh));
#if 0
		printf("modem: attach: lcr=0x%02x iir=0x%02x\n",
			bus_space_read_1(sc->sc_iot, sc->sc_ioh, 3),
			bus_space_read_1(sc->sc_iot, sc->sc_ioh, 2));
#endif
	}

	com_attach_subr(sc);

	if (sa->sa_nintr != 0) {
		(void)bus_intr_establish(sc->sc_iot, sa->sa_pri, IPL_SERIAL,
					 comintr, sc);
		evcnt_attach_dynamic(&osc->osc_intrcnt, EVCNT_TYPE_INTR, NULL,
		    osc->osc_com.sc_dev.dv_xname, "intr");
	}

	/*
	 * Shutdown hook for buggy BIOSs that don't recognize the UART
	 * without a disabled FIFO.
	 */
	if (shutdownhook_establish(com_obio_cleanup, sc) == NULL) {
		panic("com_obio_attach: could not establish shutdown hook");
	}
}

static void
com_obio_cleanup(arg)
	void *arg;
{
	struct com_softc *sc = arg;

	if (ISSET(sc->sc_hwflags, COM_HW_FIFO))
		bus_space_write_1(sc->sc_iot, sc->sc_ioh, com_fifo, 0);
}

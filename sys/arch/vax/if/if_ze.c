/*      $NetBSD: if_ze.c,v 1.1 1999/08/08 11:45:02 ragge Exp $ */
/*
 * Copyright (c) 1999 Ludd, University of Lule}, Sweden. All rights reserved.
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
 *      This product includes software developed at Ludd, University of 
 *      Lule}, Sweden and its contributors.
 * 4. The name of the author may not be used to endorse or promote products
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
#include <sys/socket.h>
#include <sys/device.h>
#include <sys/systm.h>
#include <sys/sockio.h>

#include <net/if.h>
#include <net/if_ether.h>
#include <net/if_dl.h>

#include <netinet/in.h>
#include <netinet/if_inarp.h>

#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif

#include <machine/bus.h>
#include <machine/nexus.h>
#include <machine/cpu.h>
#include <machine/scb.h>

#include <dev/ic/sgecreg.h>
#include <dev/ic/sgecvar.h>

#include "ioconf.h"
/*
 * Adresses.
 */
#define SGECADDR        0x20008000
#define NISA_ROM        0x20084000
#define	SGECVEC		0x108

static	int	zematch __P((struct device *, struct cfdata *, void *));
static	void	zeattach __P((struct device *, struct device *, void *));
static	void	zeintr __P((int));

struct	cfattach ze_ca = {
	sizeof(struct ze_softc), zematch, zeattach
};

/*
 * Check for present SGEC.
 */
int
zematch(parent, cf, aux)
	struct	device *parent;
	struct	cfdata *cf;
	void	*aux;
{
	struct bp_conf *bp = aux;

	/*
	 * Should some more intelligent checking be done???
	 */
	if (strcmp(bp->type, "sgec") == 0)
		return 1;
	return 0;
}

/*
 * Interface exists: make available by filling in network interface
 * record.  System will initialize the interface when it is ready
 * to accept packets.
 */
void
zeattach(parent, self, aux)
	struct	device *parent, *self;
	void	*aux;
{
	extern struct vax_bus_dma_tag vax_bus_dma_tag;
	struct	ze_softc *sc = (struct ze_softc *)self;
	int *ea, i;

	/*
	 * Map in SGEC registers.
	 */
	sc->sc_ioh = vax_map_physmem(SGECADDR, 1);
	sc->sc_iot = 0; /* :-) */
	sc->sc_dmat = &vax_bus_dma_tag;

	sc->sc_intvec = SGECVEC;

	/*
	 * Map in, read and release ethernet rom address.
	 */
	ea = (int *)vax_map_physmem(NISA_ROM, 1);
	for (i = 0; i < ETHER_ADDR_LEN; i++)
		sc->sc_enaddr[i] = (ea[i] >> 8) & 0377;
	vax_unmap_physmem((vaddr_t)ea, 1);

	scb_vecalloc(SGECVEC, zeintr, sc->sc_dev.dv_unit, SCB_ISTACK);

	sgec_attach(sc);
}

void
zeintr(unit)
	int	unit;
{
	struct ze_softc *sc = ze_cd.cd_devs[unit];

	sgec_intr(sc);
}

/*	$NetBSD: if_tr_isapnp.c,v 1.2 1999/03/22 23:01:37 bad Exp $	*/

/*
 * Copyright (c) 1999 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Onno van der Linden.
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
 *        This product includes software developed by The NetBSD
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

#include "opt_inet.h"
#include "opt_ns.h"
#include "bpfilter.h" 

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/mbuf.h> 
#include <sys/socket.h> 
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <sys/syslog.h>
#include <sys/select.h>
#include <sys/device.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_ether.h>
#include <net/if_media.h>

#ifdef INET
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/in_var.h>
#include <netinet/ip.h> 
#endif

#ifdef NS
#include <netns/ns.h>
#include <netns/ns_if.h>
#endif

#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif

#include <machine/cpu.h>
#include <machine/bus.h>
#include <machine/intr.h>

#include <dev/ic/tropicreg.h>
#include <dev/ic/tropicvar.h>

#include <dev/isa/isavar.h>

#include <dev/isapnp/isapnpreg.h>
#include <dev/isapnp/isapnpvar.h>
#include <dev/isapnp/isapnpdevs.h>

int	tr_isapnp_match __P((struct device *, struct cfdata *, void *));
void	tr_isapnp_attach __P((struct device *, struct device *, void *));

struct cfattach tr_isapnp_ca = {
	sizeof(struct tr_softc), tr_isapnp_match, tr_isapnp_attach
};

int
tr_isapnp_match(parent, match, aux)
	struct device *parent;
	struct cfdata *match;
	void *aux;
{
	int pri, variant;

	pri =  isapnp_devmatch(aux, &isapnp_tr_devinfo, &variant);
	if (pri && variant > 0)
		pri = 0;
	return pri;
}


void
tr_isapnp_attach(parent, self, aux)
	struct device *parent, *self;
	void *aux;
{
	struct tr_softc *sc = (void *)self;
	struct isapnp_attach_args *ipa = aux;
	int i;
	int mmioidx, sramidx;
	bus_size_t init_resp;

	printf("\n");

	if (isapnp_config(ipa->ipa_iot, ipa->ipa_memt, ipa)) {
		printf("%s: error in region allocation\n", sc->sc_dev.dv_xname);
		return;
	}

	printf("%s: %s %s", sc->sc_dev.dv_xname, ipa->ipa_devident,
	    ipa->ipa_devclass);

	sc->sc_piot = ipa->ipa_iot;
	sc->sc_pioh = ipa->ipa_io[0].h;

	if (strcmp(ipa->ipa_devlogic, "TCM3190") == 0) {
		mmioidx = 0;
		sramidx = 1;
	}
	else {	/* Default */
		mmioidx = 1;
		sramidx = 0;
	}
	sc->sc_memt = ipa->ipa_memt;
	sc->sc_mmioh = ipa->ipa_mem[mmioidx].h;
	sc->sc_sramh = ipa->ipa_mem[sramidx].h;
	sc->sc_memwinsz = ipa->ipa_mem[sramidx].length;
	/*
	 * Determine total RAM on adapter and decide how much to use.
	 * XXX Since we don't use RAM paging, use sc_memwinsz for now.
	 */
	sc->sc_memsize = sc->sc_memwinsz;
	sc->sc_memreserved = 0;

	sc->sc_aca = TR_ACA_OFFSET;
	/* 
	 * Reset the card.
	 */
	/* Latch on an unconditional adapter reset. */
	bus_space_write_1(sc->sc_piot, sc->sc_pioh, TR_RESET, 0);
	delay(50000); /* delay 50ms */
	/*
	 * XXX Set paging if we have the right type of card.
	 */
	/* Turn off adapter reset. */
	bus_space_write_1(sc->sc_piot, sc->sc_pioh, TR_RELEASE, 0);

	/* Enable interrupts. */

	ACA_SETB(sc, ACA_ISRP_e, INT_ENABLE);

	/* Wait for an answer from the adapter. */

	for (i = 0; i < 35000; i++) {
		if (ACA_RDB(sc, ACA_ISRP_o) & SRB_RESP_INT)
			break;
		delay(100);
	}

	if ((ACA_RDB(sc, ACA_ISRP_o) & SRB_RESP_INT) == 0) {
		printf("No response from adapter after reset\n");
		return;
	}
	ACA_RSTB(sc, ACA_ISRP_o, ~(SRB_RESP_INT));

	/* XXXchb Must be set after reset. */
	ACA_OUTB(sc, ACA_RRR_e, (ipa->ipa_mem[sramidx].base >> 12));

	sc->sc_srb = ACA_RDW(sc, ACA_WRBR);
	init_resp = sc->sc_srb;
	if (SRB_INB(sc, init_resp, SRB_CMD) != 0x80) {
		printf("\nInitialization incomplete, status: %02x\n",
			SRB_INB(sc, init_resp, SRB_CMD));
		return;
	}
	if (SRB_INB(sc, init_resp, SRB_INIT_BUC) != 0) {
		printf("\nBring Up Code %02x\n",
		    SRB_INB(sc, init_resp, SRB_INIT_BUC));
		return;
	}

	sc->sc_mediastatus = NULL;
	sc->sc_mediachange = NULL;

	if (tr_config(sc))
		return;

	sc->sc_ih = isa_intr_establish(ipa->ipa_ic, ipa->ipa_irq[0].num,
		ipa->ipa_irq[0].type, IPL_NET, tr_intr, sc);
}

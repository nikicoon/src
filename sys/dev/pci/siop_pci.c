/*	$NetBSD: siop_pci.c,v 1.4 2000/05/02 19:03:04 bouyer Exp $	*/

/*
 * Copyright (c) 2000 Manuel Bouyer.
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
 *	This product includes software developed by Manuel Bouyer
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* SYM53c8xx PCI-SCSI I/O Processors driver: PCI front-end */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/device.h>
#include <sys/malloc.h>
#include <sys/buf.h>
#include <sys/kernel.h>

#include <machine/endian.h>

#include <dev/pci/pcireg.h>
#include <dev/pci/pcivar.h>
#include <dev/pci/pcidevs.h>

#include <dev/scsipi/scsipi_all.h>
#include <dev/scsipi/scsipiconf.h>

#include <dev/ic/siopvar.h>

/* structure describing each chip */
struct siop_product_desc {
	u_int32_t product;
	int	revision;
	const char *name;
	int	features; /* features are defined in siopvar.h */
	u_int8_t maxburst;
	u_int8_t maxoff;  /* maximum supported offset */
	u_int8_t clock_div; /* clock divider to use for async. logic */
	u_int8_t clock_period; /* clock period (ns * 10) */
};

/* List (array, really :) of chips we know how to handle */
const struct siop_product_desc siop_products[] = {
	{ PCI_PRODUCT_SYMBIOS_810,
	0x00,
	"Symbios Logic 53c810 (fast scsi)",
	SF_PCI_RL,
	4, 8, 3, 250
	},
	{ PCI_PRODUCT_SYMBIOS_810,
	0x10,
	"Symbios Logic 53c810a (fast scsi)",
	SF_PCI_RL | SF_PCI_BOF | SF_CHIP_PF,
	4, 8, 3, 250
	},
	{ PCI_PRODUCT_SYMBIOS_815,
	0x00,
	"Symbios Logic 53c815 (fast scsi)",
	SF_PCI_RL | SF_PCI_BOF,
	4, 8, 3, 250
	},
	{ PCI_PRODUCT_SYMBIOS_820,
	0x00,
	"Symbios Logic 53c820 (fast wide scsi)",
	SF_PCI_RL | SF_BUS_WIDE,
	4, 8, 3, 250
	},
	{ PCI_PRODUCT_SYMBIOS_825,
	0x00,
	"Symbios Logic 53c825 (fast wide scsi)",
	SF_PCI_RL | SF_PCI_BOF | SF_BUS_WIDE,
	4, 8, 3, 250
	},
	{ PCI_PRODUCT_SYMBIOS_825,
	0x10,
	"Symbios Logic 53c825a (fast wide scsi)",
	SF_PCI_RL | SF_PCI_CLS | SF_PCI_WRI | SF_PCI_RM |
	SF_CHIP_FIFO | SF_CHIP_PF | SF_CHIP_RAM | 
	SF_BUS_WIDE,
	7, 8, 3, 250
	},
	{ PCI_PRODUCT_SYMBIOS_860,
	0x00,
	"Symbios Logic 53c860 (ultra scsi)",
	SF_PCI_RL | SF_PCI_CLS | SF_PCI_WRI | SF_PCI_RM |
	SF_CHIP_PF |
	SF_BUS_ULTRA,
	4, 8, 5, 125
	},
	{ PCI_PRODUCT_SYMBIOS_875,
	0x00,
	"Symbios Logic 53c875 (ultra-wide scsi)",
	SF_PCI_RL | SF_PCI_CLS | SF_PCI_WRI | SF_PCI_RM |
	SF_CHIP_FIFO | SF_CHIP_PF | SF_CHIP_RAM |
	SF_BUS_ULTRA | SF_BUS_WIDE,
	7, 16, 5, 125
	},
	{ PCI_PRODUCT_SYMBIOS_875,
	0x02,
	"Symbios Logic 53c875 (ultra-wide scsi)",
	SF_PCI_RL | SF_PCI_CLS | SF_PCI_WRI | SF_PCI_RM |
	SF_CHIP_FIFO | SF_CHIP_PF | SF_CHIP_RAM | SF_CHIP_DBLR |
	SF_BUS_ULTRA | SF_BUS_WIDE,
	7, 16, 5, 125
	},
	{ PCI_PRODUCT_SYMBIOS_875J,
	0x00,
	"Symbios Logic 53c875j (ultra-wide scsi)",
	SF_PCI_RL | SF_PCI_CLS | SF_PCI_WRI | SF_PCI_RM |
	SF_CHIP_FIFO | SF_CHIP_PF | SF_CHIP_RAM | SF_CHIP_DBLR |
	SF_BUS_ULTRA | SF_BUS_WIDE,
	7, 16, 5, 125
	},
	{ PCI_PRODUCT_SYMBIOS_885,
	0x00,
	"Symbios Logic 53c885 (ultra-wide scsi)",
	SF_PCI_RL | SF_PCI_CLS | SF_PCI_WRI | SF_PCI_RM |
	SF_CHIP_FIFO | SF_CHIP_PF | SF_CHIP_RAM | SF_CHIP_DBLR |
	SF_BUS_ULTRA | SF_BUS_WIDE,
	7, 16, 5, 125
	},
	{ PCI_PRODUCT_SYMBIOS_895,
	0x00,
	"Symbios Logic 53c895 (ultra2-wide scsi)",
	SF_PCI_RL | SF_PCI_CLS | SF_PCI_WRI | SF_PCI_RM |
	SF_CHIP_FIFO | SF_CHIP_PF | SF_CHIP_RAM | SF_CHIP_QUAD |
	SF_BUS_ULTRA2 | SF_BUS_WIDE,
	7, 31, 7, 62
	},
	{ PCI_PRODUCT_SYMBIOS_896,
	0x00,
	"Symbios Logic 53c896 (ultra2-wide scsi)",
	SF_PCI_RL | SF_PCI_CLS | SF_PCI_WRI | SF_PCI_RM |
	SF_CHIP_FIFO | SF_CHIP_PF | SF_CHIP_RAM | SF_CHIP_QUAD |
	SF_BUS_ULTRA2 | SF_BUS_WIDE,
	7, 31, 7, 62
	},
	{ 0,
	0x00,
	NULL,
	0x00,
	0, 0, 0, 0
	},
};

const struct siop_product_desc * siop_lookup_product __P((u_int32_t, int));

const struct siop_product_desc *
siop_lookup_product(id, rev)
	u_int32_t id;
	int rev;
{
	const struct siop_product_desc *pp;
	const struct siop_product_desc *rp = NULL;

	if (PCI_VENDOR(id) != PCI_VENDOR_SYMBIOS)
		return NULL;

	for (pp = siop_products; pp->name != NULL; pp++) {
		if (PCI_PRODUCT(id) == pp->product && pp->revision <= rev)
			if (rp == NULL || pp->revision > rp->revision)
				rp = pp;
	}
	return rp;
}

/* Driver internal state */
struct siop_pci_softc {
	struct siop_softc siop;
	pci_chipset_tag_t	sc_pc;	/* PCI registers info */
	pcitag_t		sc_tag;
	void			*sc_ih;	/* PCI interrupt handle */
	const struct siop_product_desc *sc_pp; /* Adapter description */
};

int     siop_pci_match __P((struct device *, struct cfdata *, void *));
void    siop_pci_attach __P((struct device *, struct device *, void *));

struct cfattach siop_pci_ca = {
	sizeof(struct siop_pci_softc), siop_pci_match, siop_pci_attach
};

int
siop_pci_match(parent, match, aux)
	struct device *parent;
	struct cfdata *match;
	void *aux;
{
	struct pci_attach_args *pa = aux;
	const struct siop_product_desc *pp;

	/* look if it's a known product */
	pp = siop_lookup_product(pa->pa_id, PCI_REVISION(pa->pa_class));
	if (pp)
		return 1;
	return 0;
}

void
siop_pci_attach(parent, self, aux)
	struct device *parent, *self;
	void *aux;
{
	struct pci_attach_args *pa = aux;
	pci_chipset_tag_t pc = pa->pa_pc;
	pcitag_t tag = pa->pa_tag;    
	struct siop_pci_softc *sc = (struct siop_pci_softc *)self;
	const char *intrstr;
	pci_intr_handle_t intrhandle;

	sc->sc_pp = siop_lookup_product(pa->pa_id, PCI_REVISION(pa->pa_class));
	if (sc->sc_pp == NULL) {
		printf("sym: broken match/attach!!\n");
		return;
	}
	printf(": %s\n", sc->sc_pp->name);
	sc->sc_pc = pc;
	sc->sc_tag = tag;
	sc->siop.sc_dmat = pa->pa_dmat;
	if (pci_mapreg_map(pa, 0x10, PCI_MAPREG_TYPE_IO, 0,
	    &sc->siop.sc_rt, &sc->siop.sc_rh, &sc->siop.sc_raddr, NULL) != 0) {
		/* Try to map memory addr */
		if (pci_mapreg_map(pa, 0x14,
		    PCI_MAPREG_TYPE_MEM | PCI_MAPREG_MEM_TYPE_32BIT, 0,
		    &sc->siop.sc_rt, &sc->siop.sc_rh,
			&sc->siop.sc_raddr, NULL) != 0) {
			printf("%s: unable to map device registers\n",
			    sc->siop.sc_dev.dv_xname);
		}
	}
	if (pci_intr_map(pa->pa_pc, pa->pa_intrtag, pa->pa_intrpin,
	    pa->pa_intrline, &intrhandle) != 0) {
		printf("%s: couldn't map native-PCI interrupt\n",
		    sc->siop.sc_dev.dv_xname);
		return;
	}
	intrstr = pci_intr_string(pa->pa_pc, intrhandle);
	sc->sc_ih = pci_intr_establish(pa->pa_pc, intrhandle, IPL_BIO,
	    siop_intr, &sc->siop);
	if (sc->sc_ih != NULL) {
		printf("%s: using %s for native-PCI interrupt\n",
		    sc->siop.sc_dev.dv_xname,
		    intrstr ? intrstr : "unknown interrupt");
	} else {
		printf("%s: couldn't establish interrupt",
		    sc->siop.sc_dev.dv_xname);
		if (intrstr != NULL)
			printf(" at %s", intrstr);
		printf("\n");
		return;
	}
	/* copy interesting infos about the chip */
	sc->siop.features = sc->sc_pp->features;
	sc->siop.maxburst = sc->sc_pp->maxburst;
	sc->siop.maxoff = sc->sc_pp->maxoff;
	sc->siop.clock_div = sc->sc_pp->clock_div;
	sc->siop.clock_period = sc->sc_pp->clock_period;
	/* attach generic code */
	siop_attach(&sc->siop);
}

/*	$NetBSD: pcib.c,v 1.30 2002/10/02 05:47:15 thorpej Exp $	*/

/*-
 * Copyright (c) 1996, 1998 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe.
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

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: pcib.c,v 1.30 2002/10/02 05:47:15 thorpej Exp $");

#include <sys/types.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/device.h>

#include <machine/bus.h>

#include <dev/isa/isavar.h>

#include <dev/pci/pcivar.h>
#include <dev/pci/pcireg.h>

#include <dev/pci/pcidevs.h>

#include "isa.h"

int	pcibmatch __P((struct device *, struct cfdata *, void *));
void	pcibattach __P((struct device *, struct device *, void *));

CFATTACH_DECL(pcib, sizeof(struct device),
    pcibmatch, pcibattach, NULL, NULL);

void	pcib_callback __P((struct device *));
int	pcib_print __P((void *, const char *));

int
pcibmatch(parent, match, aux)
	struct device *parent;
	struct cfdata *match;
	void *aux;
{
	struct pci_attach_args *pa = aux;

#if 0
	/*
	 * PCI-ISA bridges are matched on class/subclass.
	 * This list contains only the bridges where correct
	 * (or incorrect) behaviour is not yet confirmed.
	 */
	switch (PCI_VENDOR(pa->pa_id)) {
	case PCI_VENDOR_INTEL:
		switch (PCI_PRODUCT(pa->pa_id)) {
		case PCI_PRODUCT_INTEL_82426EX:
		case PCI_PRODUCT_INTEL_82380AB:
			return (1);
		}
		break;

	case PCI_VENDOR_UMC:
		switch (PCI_PRODUCT(pa->pa_id)) {
		case PCI_PRODUCT_UMC_UM8886F:
		case PCI_PRODUCT_UMC_UM82C886:
			return (1);
		}
		break;
	case PCI_VENDOR_ALI:
		switch (PCI_PRODUCT(pa->pa_id)) {
		case PCI_PRODUCT_ALI_M1449:
		case PCI_PRODUCT_ALI_M1543:
			return (1);
		}
		break;
	case PCI_VENDOR_COMPAQ:
		switch (PCI_PRODUCT(pa->pa_id)) {
		case PCI_PRODUCT_COMPAQ_PCI_ISA_BRIDGE:
			return (1);
		}
		break;
	case PCI_VENDOR_VIATECH:
		switch (PCI_PRODUCT(pa->pa_id)) {
		case PCI_PRODUCT_VIATECH_VT82C570MV:
		case PCI_PRODUCT_VIATECH_VT82C586_ISA:
			return (1);
		}
		break;
	}
#endif

	/*
	 * some special cases:
	 */
	switch (PCI_VENDOR(pa->pa_id)) {
	case PCI_VENDOR_INTEL:
		switch (PCI_PRODUCT(pa->pa_id)) {
		case PCI_PRODUCT_INTEL_SIO:
			/*
			 * The Intel SIO identifies itself as a
			 * miscellaneous prehistoric.
			 */
		case PCI_PRODUCT_INTEL_82371MX:
			/*
			 * The Intel 82371MX identifies itself erroneously as a
			 * miscellaneous bridge.
			 */
		case PCI_PRODUCT_INTEL_82371AB_ISA:
			/*
			 * Some Intel 82371AB PCI-ISA bridge identifies
			 * itself as miscellaneous bridge.
			 */
			return (1);
		}
		break;
	case PCI_VENDOR_SIS:
		switch (PCI_PRODUCT(pa->pa_id)) {
		case PCI_PRODUCT_SIS_85C503:
			/*
			 * The SIS 85C503 identifies itself as a
			 * miscellaneous prehistoric.
			 */
			return (1);
		}
		break;
	case PCI_VENDOR_VIATECH:
		switch (PCI_PRODUCT(pa->pa_id)) {
		case PCI_PRODUCT_VIATECH_VT82C686A_SMB:
			/*
			 * The VIA VT82C686A SMBus Controller itself as 
			 * ISA bridge, but it's wrong !
			 */
			return (0);
		}
	}

	if (PCI_CLASS(pa->pa_class) == PCI_CLASS_BRIDGE &&
	    PCI_SUBCLASS(pa->pa_class) == PCI_SUBCLASS_BRIDGE_ISA) {
		return (1);
	}

	return (0);
}

void
pcibattach(parent, self, aux)
	struct device *parent, *self;
	void *aux;
{
	struct pci_attach_args *pa = aux;
	char devinfo[256];

	printf("\n");

	/*
	 * Just print out a description and defer configuration
	 * until all PCI devices have been attached.
	 */
	pci_devinfo(pa->pa_id, pa->pa_class, 0, devinfo);
	printf("%s: %s (rev. 0x%02x)\n", self->dv_xname, devinfo,
	    PCI_REVISION(pa->pa_class));

	config_defer(self, pcib_callback);
}

void
pcib_callback(self)
	struct device *self;
{
	struct isabus_attach_args iba;

	/*
	 * Attach the ISA bus behind this bridge.
	 */
	memset(&iba, 0, sizeof(iba));
	iba.iba_busname = "isa";
	iba.iba_iot = I386_BUS_SPACE_IO;
	iba.iba_memt = I386_BUS_SPACE_MEM;
#if NISA > 0
	iba.iba_dmat = &isa_bus_dma_tag;
#endif
	config_found(self, &iba, pcib_print);
}

int
pcib_print(aux, pnp)
	void *aux;
	const char *pnp;
{

	/* Only ISAs can attach to pcib's; easy. */
	if (pnp)
		printf("isa at %s", pnp);
	return (UNCONF);
}

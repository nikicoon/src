/*	$NetBSD: sbus.c,v 1.19 1998/03/21 19:55:31 pk Exp $ */

/*
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This software was developed by the Computer Systems Engineering group
 * at Lawrence Berkeley Laboratory under DARPA contract BG 91-66 and
 * contributed to Berkeley.
 *
 * All advertising materials mentioning features or use of this software
 * must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Lawrence Berkeley Laboratory.
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
 *	@(#)sbus.c	8.1 (Berkeley) 6/11/93
 */

/*
 * Sbus stuff.
 */

#include <sys/param.h>
#include <sys/malloc.h>
#include <sys/systm.h>
#include <sys/device.h>
#include <vm/vm.h>

#include <machine/bus.h>
#include <sparc/dev/sbusreg.h>
#include <sparc/dev/sbusvar.h>

#include <sparc/sparc/iommuvar.h>
#include <machine/autoconf.h>


void sbusreset __P((int));

static bus_space_tag_t sbus_alloc_bustag __P((struct sbus_softc *));
static int sbus_get_intr __P((struct sbus_softc *, int, int *));
static int sbus_bus_mmap __P((void *, bus_type_t, bus_addr_t, int));


/* autoconfiguration driver */
int	sbus_match_mainbus __P((struct device *, struct cfdata *, void *));
int	sbus_match_iommu __P((struct device *, struct cfdata *, void *));
void	sbus_attach_mainbus __P((struct device *, struct device *, void *));
void	sbus_attach_iommu __P((struct device *, struct device *, void *));

struct cfattach sbus_mainbus_ca = {
	sizeof(struct sbus_softc), sbus_match_mainbus, sbus_attach_mainbus
};
struct cfattach sbus_iommu_ca = {
	sizeof(struct sbus_softc), sbus_match_iommu, sbus_attach_iommu
};

extern struct cfdriver sbus_cd;

/* If the PROM does not provide the `ranges' property, we make up our own */
struct rom_range sbus_translations[] = {
	/* Assume a maximum of 4 Sbus slots, all mapped to on-board io space */
	{ 0, 0, PMAP_OBIO, SBUS_ADDR(0,0), 1 << 25 },
	{ 1, 0, PMAP_OBIO, SBUS_ADDR(1,0), 1 << 25 },
	{ 2, 0, PMAP_OBIO, SBUS_ADDR(2,0), 1 << 25 },
	{ 3, 0, PMAP_OBIO, SBUS_ADDR(3,0), 1 << 25 }
};

/*
 * Child devices receive the Sbus interrupt level in their attach
 * arguments. We translate these to CPU IPLs using the following
 * tables. Note: obio bus interrupt levels are identical to the
 * processor IPL.
 *
 * The second set of tables is used when the Sbus interrupt level
 * cannot be had from the PROM as an `interrupt' property. We then
 * fall back on the `intr' property which contains the CPU IPL.
 */

/* Translate Sbus interrupt level to processor IPL */
static int intr_sbus2ipl_4c[] = {
	0, 1, 2, 3, 5, 7, 8, 9
};
static int intr_sbus2ipl_4m[] = {
	0, 2, 3, 5, 7, 9, 11, 13
};

#if 0
/* Table to translate `intr' property values to Sbus interrupt levels */
static int intr2sbus_4c[] = {
	0, 1, 2, 3, -1, 4, -1, 5, 6, 7, -1, -1, -1, -1, -1
};
static int intr2sbus_4m[] = {
	0, -1, 1, 2, -1, 3, -1, 4, -1, 5, -1, 6, -1, 7, -1, -1
};
#endif

#define SBUS_INTR_COMPAT	0x80000000


/*
 * Print the location of some sbus-attached device (called just
 * before attaching that device).  If `sbus' is not NULL, the
 * device was found but not configured; print the sbus as well.
 * Return UNCONF (config_find ignores this if the device was configured).
 */
int
sbus_print(args, busname)
	void *args;
	const char *busname;
{
	struct sbus_attach_args *sa = args;

	if (busname)
		printf("%s at %s", sa->sa_name, busname);
	printf(" slot %d offset 0x%x", sa->sa_slot, sa->sa_offset);
	if (sa->sa_pri) {
		int level = sa->sa_pri;
		struct sbus_softc *sc =
			(struct sbus_softc *) sa->sa_bustag->cookie;

		printf(" level %d", level & ~SBUS_INTR_COMPAT);
		if ((level & SBUS_INTR_COMPAT) == 0) {
			int ipl = sc->sc_intr2ipl[level];
			if (ipl != level)
				printf(" (ipl %d)", ipl);
		}
	}
	return (UNCONF);
}

int
sbus_match_mainbus(parent, cf, aux)
	struct device *parent;
	struct cfdata *cf;
	void *aux;
{
	struct mainbus_attach_args *ma = aux;

	if (CPU_ISSUN4)
		return (0);

	return (strcmp(cf->cf_driver->cd_name, ma->ma_name) == 0);
}

int
sbus_match_iommu(parent, cf, aux)
	struct device *parent;
	struct cfdata *cf;
	void *aux;
{
	struct iommu_attach_args *ia = aux;

	if (CPU_ISSUN4)
		return (0);

	return (strcmp(cf->cf_driver->cd_name, ia->iom_name) == 0);
}

/*
 * Attach an Sbus.
 */
void
sbus_attach_mainbus(parent, self, aux)
	struct device *parent;
	struct device *self;
	void *aux;
{
	struct sbus_softc *sc = (struct sbus_softc *)self;
	struct mainbus_attach_args *ma = aux;
	int node = ma->ma_node;

	/*
	 * XXX there is only one Sbus, for now -- do not know how to
	 * address children on others
	 */
	if (sc->sc_dev.dv_unit > 0) {
		printf(" unsupported\n");
		return;
	}

	sc->sc_bustag = ma->ma_bustag;
	sc->sc_dmatag = ma->ma_dmatag;

	/* Setup interrupt translation tables */
	sc->sc_intr2ipl = CPU_ISSUN4C
				? intr_sbus2ipl_4c
				: intr_sbus2ipl_4m;

#if 0 /* this won't work */
	sc->sc_intr_compat = CPU_ISSUN4C
				? intr2sbus_4c
				: intr2sbus_4m;
#endif

	/*
	 * Record clock frequency for synchronous SCSI.
	 * IS THIS THE CORRECT DEFAULT??
	 */
	sc->sc_clockfreq = getpropint(node, "clock-frequency", 25*1000*1000);
	printf(": clock = %s MHz\n", clockfreq(sc->sc_clockfreq));

	sbus_attach(sc, "sbus", node, ma->ma_bp, NULL);
}

void
sbus_attach_iommu(parent, self, aux)
	struct device *parent;
	struct device *self;
	void *aux;
{
	struct sbus_softc *sc = (struct sbus_softc *)self;
	struct iommu_attach_args *ia = aux;
	int node = ia->iom_node;

	sc->sc_bustag = ia->iom_bustag;
	sc->sc_dmatag = ia->iom_dmatag;

	/* Setup interrupt translation tables */
	sc->sc_intr2ipl = CPU_ISSUN4C ? intr_sbus2ipl_4c : intr_sbus2ipl_4m;

#if 0 /* this won't work */
	sc->sc_intr_compat = CPU_ISSUN4C ? intr2sbus_4c : intr2sbus_4m;
#endif

	/*
	 * Record clock frequency for synchronous SCSI.
	 * IS THIS THE CORRECT DEFAULT??
	 */
	sc->sc_clockfreq = getpropint(node, "clock-frequency", 25*1000*1000);
	printf(": clock = %s MHz\n", clockfreq(sc->sc_clockfreq));

	sbus_attach(sc, "sbus", node, ia->iom_bp, NULL);
}

void
sbus_attach(sc, busname, busnode, bp, specials)
	struct sbus_softc *sc;
	char *busname;
	int busnode;
	struct bootpath *bp;
	const char * const *specials;
{
	int node0, node, error;
	const char *sp;
	const char *const *ssp;
	bus_space_tag_t sbt;
	struct sbus_attach_args sa;

	sbt = sbus_alloc_bustag(sc);

	/*
	 * Get the SBus burst transfer size if burst transfers are supported
	 */
	sc->sc_burst = getpropint(busnode, "burst-sizes", 0);

	/* Propagate bootpath */
	if (bp != NULL && strcmp(bp->name, busname) == 0)
		bp++;
	else
		bp = NULL;

	/*
	 * Collect address translations from the OBP.
	 */
	error = getpropA(busnode, "ranges", sizeof(struct rom_range),
			 &sc->sc_nrange, (void **)&sc->sc_range);
	switch (error) {
	case 0:
		break;
	case ENOENT:
		/* Fall back to our own `range' construction */
		sc->sc_range = sbus_translations;
		sc->sc_nrange =
			sizeof(sbus_translations)/sizeof(sbus_translations[0]);
		break;
	default:
		panic("%s: error getting ranges property", sc->sc_dev.dv_xname);
	}

	/*
	 * Loop through ROM children, fixing any relative addresses
	 * and then configuring each device.
	 * `specials' is an array of device names that are treated
	 * specially:
	 */
	node0 = firstchild(busnode);
	for (ssp = specials ; ssp != NULL && *(sp = *ssp) != 0; ssp++) {
		if ((node = findnode(node0, sp)) == 0) {
			panic("could not find %s amongst %s devices",
				sp, busname);
		}

		if (sbus_setup_attach_args(sc, sbt, sc->sc_dmatag,
					   node, bp, &sa) != 0) {
			panic("sbus_attach: %s: incomplete", sp);
		}
		(void) config_found(&sc->sc_dev, (void *)&sa, sbus_print);
	}

	for (node = node0; node; node = nextsibling(node)) {
		char *name = getpropstring(node, "name");
		for (ssp = specials, sp = NULL;
		     ssp != NULL && (sp = *ssp) != NULL;
		     ssp++)
			if (strcmp(name, sp) == 0)
				break;

		if (sp != NULL)
			/* Already configured as an "early" device */
			continue;

		if (sbus_setup_attach_args(sc, sbt, sc->sc_dmatag,
					   node, bp, &sa) != 0) {
			printf("sbus_attach: %s: incomplete\n", name);
			continue;
		}
		(void) config_found(&sc->sc_dev, (void *)&sa, sbus_print);
	}
}

int
sbus_setup_attach_args(sc, bustag, dmatag, node, bp, sa)
	struct sbus_softc	*sc;
	bus_space_tag_t		bustag;
	bus_dma_tag_t		dmatag;
	int			node;
	struct bootpath		*bp;
	struct sbus_attach_args	*sa;
{
	struct	rom_reg romreg;
	int	base;
	int	error;

	bzero(sa, sizeof(struct sbus_attach_args));
	sa->sa_name = getpropstring(node, "name");
	sa->sa_bustag = bustag;
	sa->sa_dmatag = dmatag;
	sa->sa_node = node;
	sa->sa_bp = bp;

	if ((error = getprop_reg1(node, &romreg)) != 0)
		return (error);

	/* We pass only the first "reg" property */
	base = (int)romreg.rr_paddr;
	if (SBUS_ABS(base)) {
		sa->sa_slot = SBUS_ABS_TO_SLOT(base);
		sa->sa_offset = SBUS_ABS_TO_OFFSET(base);
	} else {
		sa->sa_slot = romreg.rr_iospace;
		sa->sa_offset = base;
	}
	sa->sa_size = romreg.rr_len;

	if ((error = sbus_get_intr(sc, node, &sa->sa_pri)) != 0)
		return (error);

	if ((error = getprop_address1(node, &sa->sa_promvaddr)) != 0)
		return (error);

	return (0);
}

int
sbus_bus_map(t, slot, offset, size, flags, vaddr, hp)
	bus_space_tag_t t;
	int slot, offset, size, flags;
	vm_offset_t vaddr;
	bus_space_handle_t *hp;
{
	struct sbus_softc *sc = t->cookie;
	int i;

	for (i = 0; i < sc->sc_nrange; i++) {
		bus_addr_t paddr;
		bus_type_t iospace;

		if (sc->sc_range[i].cspace != slot)
			continue;

		/* We've found the connection to the parent bus */
		paddr = sc->sc_range[i].poffset + offset;
		iospace = sc->sc_range[i].pspace;
		return (bus_space_map2(sc->sc_bustag, iospace, paddr,
					size, flags, vaddr, hp));
	}

	return (EINVAL);
}

int
sbus_bus_mmap(cookie, btype, paddr, flags)
	void *cookie;
	bus_type_t btype;
	bus_addr_t paddr;
	int flags;
{
	int slot = (int)btype;
	int offset = (int)paddr;
	struct sbus_softc *sc = cookie;
	int i;

	for (i = 0; i < sc->sc_nrange; i++) {
		bus_addr_t paddr;
		bus_addr_t iospace;

		if (sc->sc_range[i].cspace != slot)
			continue;

		paddr = sc->sc_range[i].poffset + offset;
		iospace = (bus_addr_t)sc->sc_range[i].pspace;
		return (bus_space_mmap(sc->sc_bustag, iospace, paddr, flags));
	}

	return (-1);
}


/*
 * Each attached device calls sbus_establish after it initializes
 * its sbusdev portion.
 */
void
sbus_establish(sd, dev)
	register struct sbusdev *sd;
	register struct device *dev;
{
	register struct sbus_softc *sc;
	register struct device *curdev;

	/*
	 * We have to look for the sbus by name, since it is not necessarily
	 * our immediate parent (i.e. sun4m /iommu/sbus/espdma/esp)
	 * We don't just use the device structure of the above-attached
	 * sbus, since we might (in the future) support multiple sbus's.
	 */
	for (curdev = dev->dv_parent; ; curdev = curdev->dv_parent) {
		if (!curdev || !curdev->dv_xname)
			panic("sbus_establish: can't find sbus parent for %s",
			      sd->sd_dev->dv_xname
					? sd->sd_dev->dv_xname
					: "<unknown>" );

		if (strncmp(curdev->dv_xname, "sbus", 4) == 0)
			break;
	}
	sc = (struct sbus_softc *) curdev;

	sd->sd_dev = dev;
	sd->sd_bchain = sc->sc_sbdev;
	sc->sc_sbdev = sd;
}

/*
 * Reset the given sbus. (???)
 */
void
sbusreset(sbus)
	int sbus;
{
	register struct sbusdev *sd;
	struct sbus_softc *sc = sbus_cd.cd_devs[sbus];
	struct device *dev;

	printf("reset %s:", sc->sc_dev.dv_xname);
	for (sd = sc->sc_sbdev; sd != NULL; sd = sd->sd_bchain) {
		if (sd->sd_reset) {
			dev = sd->sd_dev;
			(*sd->sd_reset)(dev);
			printf(" %s", dev->dv_xname);
		}
	}
}


/*
 * Get interrupt attributes for an Sbus device.
 */
int
sbus_get_intr(sc, node, ip)
	struct sbus_softc *sc;
	int node;
	int *ip;
{
	struct rom_intr *rip;
	int *ipl;
	int n;

	/*
	 * The `interrupts' property contains the Sbus interrupt level.
	 */
	ipl = NULL;
	if (getpropA(node, "interrupts", sizeof(int), &n, (void **)&ipl) == 0) {
		*ip = ipl[0];
		free(ipl, M_DEVBUF);
		return (0);
	}

	/*
	 * Fall back on `intr' property.
	 */
	rip = NULL;
	switch (getpropA(node, "intr", sizeof(*rip), &n, (void **)&rip)) {
	case 0:
		*ip = (rip[0].int_pri & 0xf) | SBUS_INTR_COMPAT;
		free(rip, M_DEVBUF);
		return (0);
	case ENOENT:
		*ip = 0;
		return (0);
	}

	return (-1);
}


/*
 * Install an interrupt handler for an Sbus device.
 */
void *
sbus_intr_establish(cookie, level, flags, handler, arg)
        void *cookie;
	int level;
	int flags;
	int (*handler) __P((void *));
	void *arg;
{
	struct sbus_softc *sc = cookie;
	struct intrhand *ih;
	int ipl;

	ih = (struct intrhand *)
		malloc(sizeof(struct intrhand), M_DEVBUF, M_NOWAIT);
	if (ih == NULL)
		return (NULL);

	if ((flags & BUS_INTR_ESTABLISH_SOFTINTR) != 0)
		ipl = level;
	else if ((level & SBUS_INTR_COMPAT) != 0)
		ipl = level & ~SBUS_INTR_COMPAT;
	else
		ipl = sc->sc_intr2ipl[level];

	ih->ih_fun = handler;
	ih->ih_arg = arg;
	if ((flags & BUS_INTR_ESTABLISH_FASTTRAP) != 0)
		intr_fasttrap(ipl, (void (*)__P((void)))handler);
	else
		intr_establish(ipl, ih);
	return (ih);
}

static bus_space_tag_t
sbus_alloc_bustag(sc)
	struct sbus_softc *sc;
{
	bus_space_tag_t sbt;

	sbt = (bus_space_tag_t)
		malloc(sizeof(struct sparc_bus_space_tag), M_DEVBUF, M_NOWAIT);
	if (sbt == NULL)
		return (NULL);

	bzero(sbt, sizeof *sbt);
	sbt->cookie = sc;
#if notyet
	sbt->sparc_bus_map = _sbus_bus_map;
#endif
	sbt->sparc_bus_mmap = sbus_bus_mmap;
	sbt->sparc_intr_establish = sbus_intr_establish;
	return (sbt);
}


/* $NetBSD: pmap.old.c,v 1.58 1998/03/18 19:21:50 thorpej Exp $ */

/*-
 * Copyright (c) 1998 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe of the Numerical Aerospace Simulation Facility,
 * NASA Ames Research Center and by Chris G. Demetriou.
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

/* 
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * the Systems Programming Group of the University of Utah Computer
 * Science Department.
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
 *	@(#)pmap.c	8.6 (Berkeley) 5/27/94
 */

/*
 * DEC Alpha physical map management code.
 *
 * History:
 *
 *	This pmap started life as a Motorola 68851/68030 pmap,
 *	written by Mike Hibler at the University of Utah.
 *
 *	It was modified for the DEC Alpha by Chris Demetriou
 *	at Carnegie Mellon University.
 *
 *	Support for non-contiguous physical memory was added by
 *	Jason R. Thorpe of the Numerical Aerospace Simulation
 *	Facility, NASA Ames Research Center and Chris Demetriou.
 *
 *	Page table management and a major cleanup were undertaken
 *	by Jason R. Thorpe, with lots of help from Ross Harvey of
 *	Avalon Computer Systems and from Chris Demetriou.
 *
 *	Support for the new UVM pmap interface was written by
 *	Jason R. Thorpe.
 *
 * Notes:
 *
 *	All page table access is done via K0SEG.  The one exception
 *	to this is for kernel mappings.  Since all kernel page
 *	tables are pre-allocated, we can use the Virtual Page Table
 *	to access PTEs that map K1SEG addresses.
 *
 *	Kernel page table pages are statically allocated in
 *	pmap_bootstrap(), and are never freed.  In the future,
 *	support for dynamically adding additional kernel page
 *	table pages may be added.  User page table pages are
 *	dynamically allocated and freed.
 *
 *	This pmap implementation only supports NBPG == PAGE_SIZE.
 *	In practice, this is not a problem since PAGE_SIZE is
 *	initialized to the hardware page size in alpha_init().
 *
 * Bugs/misfeatures:
 *
 *	Does not currently support ASNs.  Support for ASNs would
 *	eliminate a good deal of TLB invalidation.
 *
 *	Does not currently support multiple processors.  At the very
 *	least, TLB shootdown is needed.
 */

/*
 *	Manages physical address maps.
 *
 *	In addition to hardware address maps, this
 *	module is called upon to provide software-use-only
 *	maps which may or may not be stored in the same
 *	form as hardware maps.  These pseudo-maps are
 *	used to store intermediate results from copy
 *	operations to and from address spaces.
 *
 *	Since the information managed by this module is
 *	also stored by the logical address mapping module,
 *	this module may throw away valid virtual-to-physical
 *	mappings at almost any time.  However, invalidations
 *	of virtual-to-physical mappings must be done as
 *	requested.
 *
 *	In order to cope with hardware architectures which
 *	make virtual-to-physical map invalidates expensive,
 *	this module may delay invalidate or reduced protection
 *	operations until such time as they are actually
 *	necessary.  This module is given full information as
 *	to which processors are currently using which maps,
 *	and to when physical maps must be made correct.
 */

#include "opt_uvm.h"
#include "opt_pmap_new.h"

#include <sys/cdefs.h>			/* RCS ID & Copyright macro defns */

__KERNEL_RCSID(0, "$NetBSD: pmap.old.c,v 1.58 1998/03/18 19:21:50 thorpej Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/malloc.h>
#include <sys/user.h>
#include <sys/buf.h>
#ifdef SYSVSHM
#include <sys/shm.h>
#endif

#include <vm/vm.h>
#include <vm/vm_kern.h>
#include <vm/vm_page.h>

#if defined(UVM)
#include <uvm/uvm.h>
#endif

#include <machine/cpu.h>
#ifdef _PMAP_MAY_USE_PROM_CONSOLE
#include <machine/rpb.h>			/* XXX */
#endif

#ifdef PMAPSTATS
struct {
	int kernel;	/* entering kernel mapping */
	int user;	/* entering user mapping */
	int ptpneeded;	/* needed to allocate a PT page */
	int nochange;	/* no change at all */
	int pwchange;	/* no mapping change, just wiring or protection */
	int wchange;	/* no mapping change, just wiring */
	int pchange;	/* no mapping change, just protection */
	int mchange;	/* was mapped but mapping to different page */
	int managed;	/* a managed page */
	int firstpv;	/* first mapping for this PA */
	int secondpv;	/* second mapping for this PA */
	int ci;		/* cache inhibited */
	int unmanaged;	/* not a managed page */
	int flushes;	/* cache flushes */
} enter_stats;
struct {
	int calls;
	int removes;
	int pvfirst;
	int pvsearch;
	int ptinvalid;
	int uflushes;
	int sflushes;
} remove_stats;
struct {
	int calls;
	int changed;
	int alreadyro;
	int alreadyrw;
} protect_stats;
struct chgstats {
	int setcalls;
	int sethits;
	int setmiss;
	int clrcalls;
	int clrhits;
	int clrmiss;
} changebit_stats[16];
#endif

#ifdef DEBUG
#define PDB_FOLLOW	0x0001
#define PDB_INIT	0x0002
#define PDB_ENTER	0x0004
#define PDB_REMOVE	0x0008
#define PDB_CREATE	0x0010
#define PDB_PTPAGE	0x0020
#define PDB_BITS	0x0080
#define PDB_COLLECT	0x0100
#define PDB_PROTECT	0x0200
#define PDB_BOOTSTRAP	0x1000
#define PDB_PARANOIA	0x2000
#define PDB_WIRING	0x4000
#define PDB_PVDUMP	0x8000

int debugmap = 0;
int pmapdebug = PDB_PARANOIA;
#endif

/*
 * Given a map and a machine independent protection code,
 * convert to an alpha protection code.
 */
#define pte_prot(m, p)	(protection_codes[m == pmap_kernel() ? 0 : 1][p])
int	protection_codes[2][8];

/*
 * Lev1map:
 *
 *	Kernel level 1 page table.  This maps all kernel level 2
 *	page table pages, and is used as a template for all user
 *	pmap level 1 page tables.  When a new user level 1 page
 *	table is allocated, all Lev1map PTEs for kernel addresses
 *	are copied to the new map.
 *
 * Lev2map:
 *
 *	Initial set of kernel level 2 page table pages.  These
 *	map the kernel level 3 page table pages.  As kernel
 *	level 3 page table pages are added, more level 2 page
 *	table pages may be added to map them.  These pages are
 *	never freed.
 *
 * Lev3map:
 *
 *	Initial set of kernel level 3 page table pages.  These
 *	map pages in K1SEG.  More level 3 page table pages may
 *	be added at run-time if additional K1SEG address space
 *	is required.  These pages are never freed.
 *
 * Lev2mapsize:
 *
 *	Number of entries in the initial Lev2map.
 *
 * Lev3mapsize:
 *
 *	Number of entries in the initial Lev3map.
 *
 * NOTE: When mappings are inserted into the kernel pmap, all
 * level 2 and level 3 page table pages must already be allocated
 * and mapped into the parent page table.
 */
pt_entry_t	*Lev1map, *Lev2map, *Lev3map;
vm_size_t	Lev2mapsize, Lev3mapsize;

/*
 * Virtual Page Table.
 */
pt_entry_t	*VPT;

struct pmap	kernel_pmap_store;

vm_offset_t    	avail_start;	/* PA of first available physical page */
vm_offset_t	avail_end;	/* PA of last available physical page */
vm_offset_t	virtual_avail;  /* VA of first avail page (after kernel bss)*/
vm_offset_t	virtual_end;	/* VA of last avail page (end of kernel AS) */

boolean_t	pmap_initialized = FALSE;	/* Has pmap_init completed? */

/*
 * Storage for physical->virtual entries and page attributes.
 */
struct pv_head	*pv_table;
int		pv_table_npages;

/*
 * Free list of pages for use by the physical->virtual entry memory allocator.
 */
TAILQ_HEAD(pv_page_list, pv_page) pv_page_freelist;
int		pv_nfree;

/*
 * List of all pmaps, used to update them when e.g. additional kernel
 * page tables are allocated.
 */
LIST_HEAD(, pmap) pmap_all_pmaps;

#define	PAGE_IS_MANAGED(pa)	(vm_physseg_find(atop(pa), NULL) != -1)

static __inline struct pv_head *pa_to_pvh __P((vm_offset_t));

static __inline struct pv_head *
pa_to_pvh(pa)
	vm_offset_t pa;
{
	int bank, pg;

	bank = vm_physseg_find(atop(pa), &pg);
	return (&vm_physmem[bank].pmseg.pvhead[pg]);
}

/*
 * Internal routines
 */
void	alpha_protection_init __P((void));
void	pmap_remove_mapping __P((pmap_t, vm_offset_t, pt_entry_t *, boolean_t));
void	pmap_changebit __P((vm_offset_t, u_long, boolean_t));

/*
 * PT page management functions.
 */
void	pmap_create_lev1map __P((pmap_t));
void	pmap_destroy_lev1map __P((pmap_t));
void	pmap_alloc_ptpage __P((pmap_t, pt_entry_t *));
void	pmap_free_ptpage __P((pmap_t, pt_entry_t *));
int	pmap_ptpage_addref __P((pt_entry_t *));
int	pmap_ptpage_delref __P((pt_entry_t *));

/*
 * PV table management functions.
 */
void	pmap_enter_pv __P((pmap_t, vm_offset_t, vm_offset_t));
void	pmap_remove_pv __P((pmap_t, vm_offset_t, vm_offset_t));
struct	pv_entry *pmap_alloc_pv __P((void));
void	pmap_free_pv __P((struct pv_entry *));
void	pmap_collect_pv __P((void));

/*
 * Misc. functions.
 */
vm_offset_t pmap_alloc_physpage __P((void));
void	pmap_free_physpage __P((vm_offset_t));

#ifdef DEBUG
void	pmap_pvdump __P((vm_offset_t));
#endif

/*
 * PMAP_ACTIVATE:
 *
 *	This is essentially the guts of pmap_activate(), without
 *	ASN allocation.  This is used by pmap_activate(),
 *	pmap_create_lev1map(), and pmap_destroy_lev1map().
 */
#define	PMAP_ACTIVATE(pmap, p)						\
do {									\
	(p)->p_addr->u_pcb.pcb_hw.apcb_ptbr =				\
	    ALPHA_K0SEG_TO_PHYS((vm_offset_t)(pmap)->pm_lev1map) >> PGSHIFT; \
									\
	if ((p) == curproc) {						\
		/*							\
		 * Page table base register has changed; switch to	\
		 * our own context again so that it will take effect.	\
		 */							\
		(void) alpha_pal_swpctx((u_long)p->p_md.md_pcbpaddr);	\
									\
		/* XXX These go away if we use ASNs. */			\
		ALPHA_TBIAP();						\
		alpha_pal_imb();					\
	}								\
} while (0)

/*
 * pmap_bootstrap:
 *
 *	Bootstrap the system to run with virtual memory.
 */
void
pmap_bootstrap(ptaddr, maxasn)
	vm_offset_t ptaddr;
	u_int maxasn;
{
	pt_entry_t pte;
	int i;
	extern int physmem;

#ifdef DEBUG
	if (pmapdebug & (PDB_FOLLOW|PDB_BOOTSTRAP))
		printf("pmap_bootstrap(0x%lx, %u)\n", ptaddr, maxasn);
#endif

	/*
	 * Figure out how many PTE's are necessary to map the kernel.
	 * The '512' comes from PAGER_MAP_SIZE in vm_pager_init().
	 * This should be kept in sync.
	 * We also reserve space for kmem_alloc_pageable() for vm_fork().
	 */
	Lev3mapsize = (VM_KMEM_SIZE + VM_MBUF_SIZE + VM_PHYS_SIZE +
		nbuf * MAXBSIZE + 16 * NCARGS) / NBPG + 512 +
		(maxproc * UPAGES);

#ifdef SYSVSHM
	Lev3mapsize += shminfo.shmall;
#endif
	Lev3mapsize = roundup(Lev3mapsize, NPTEPG);

	/*
	 * Allocate a level 1 PTE table for the kernel.
	 * This is always one page long.
	 * IF THIS IS NOT A MULTIPLE OF NBPG, ALL WILL GO TO HELL.
	 */
	Lev1map = (pt_entry_t *)
	    pmap_steal_memory(sizeof(pt_entry_t) * NPTEPG, NULL, NULL);

	/*
	 * Allocate a level 2 PTE table for the kernel.
	 * These must map all of the level3 PTEs.
	 * IF THIS IS NOT A MULTIPLE OF NBPG, ALL WILL GO TO HELL.
	 */
	Lev2mapsize = roundup(howmany(Lev3mapsize, NPTEPG), NPTEPG);
	Lev2map = (pt_entry_t *)
	    pmap_steal_memory(sizeof(pt_entry_t) * Lev2mapsize, NULL, NULL);

	/*
	 * Allocate a level 3 PTE table for the kernel.
	 * Contains Lev3mapsize PTEs.
	 */
	Lev3map = (pt_entry_t *)
	    pmap_steal_memory(sizeof(pt_entry_t) * Lev3mapsize, NULL, NULL);

	/*
	 * Allocate memory for the pv_heads.  (A few more of the latter
	 * are allocated than are needed.)
	 *
	 * We could do this in pmap_init when we know the actual
	 * managed page pool size, but its better to use kseg0
	 * addresses rather than kernel virtual addresses mapped
	 * through the TLB.
	 */
	pv_table_npages = physmem;
	pv_table = (struct pv_head *)
	    pmap_steal_memory(sizeof(struct pv_head) * pv_table_npages,
	    NULL, NULL);

	/*
	 * ...and intialize the pv_entry list headers.
	 */
	for (i = 0; i < pv_table_npages; i++)
		LIST_INIT(&pv_table[i].pvh_list);

	/*
	 * Set up level 1 page table
	 */

#ifdef _PMAP_MAY_USE_PROM_CONSOLE
    {
	extern pt_entry_t rom_pte;			/* XXX */
	extern int prom_mapped;				/* XXX */

	if (pmap_uses_prom_console()) {
		/* XXX save old pte so that we can remap prom if necessary */
		rom_pte = *(pt_entry_t *)ptaddr & ~PG_ASM;	/* XXX */
	}
	prom_mapped = 0;

	/*
	 * Actually, this code lies.  The prom is still mapped, and will
	 * remain so until the context switch after alpha_init() returns.
	 * Printfs using the firmware before then will end up frobbing
	 * Lev1map unnecessarily, but that's OK.
	 */
    }
#endif

	/* Map all of the level 2 pte pages */
	for (i = 0; i < howmany(Lev2mapsize, NPTEPG); i++) {
		pte = (ALPHA_K0SEG_TO_PHYS(((vm_offset_t)Lev2map) +
		    (i*PAGE_SIZE)) >> PGSHIFT) << PG_SHIFT;
		pte |= PG_V | PG_ASM | PG_KRE | PG_KWE | PG_WIRED;
		Lev1map[l1pte_index(VM_MIN_KERNEL_ADDRESS +
		    (i*PAGE_SIZE*NPTEPG*NPTEPG))] = pte;
	}

	/* Map the virtual page table */
	pte = (ALPHA_K0SEG_TO_PHYS((vm_offset_t)Lev1map) >> PGSHIFT)
	    << PG_SHIFT;
	pte |= PG_V | PG_KRE | PG_KWE; /* NOTE NO ASM */
	Lev1map[l1pte_index(VPTBASE)] = pte;
	VPT = (pt_entry_t *)VPTBASE;

	/*
	 * Set up level 2 page table.
	 */
	/* Map all of the level 3 pte pages */
	for (i = 0; i < howmany(Lev3mapsize, NPTEPG); i++) {
		pte = (ALPHA_K0SEG_TO_PHYS(((vm_offset_t)Lev3map) +
		    (i*PAGE_SIZE)) >> PGSHIFT) << PG_SHIFT;
		pte |= PG_V | PG_ASM | PG_KRE | PG_KWE | PG_WIRED;
		Lev2map[l2pte_index(VM_MIN_KERNEL_ADDRESS+
		    (i*PAGE_SIZE*NPTEPG))] = pte;
	}

	/*
	 * Set up level three page table (Lev3map)
	 */
	/* Nothing to do; it's already zero'd */

	/*
	 * Initialize `FYI' variables.  Note we're relying on
	 * the fact that BSEARCH sorts the vm_physmem[] array
	 * for us.
	 */
	avail_start = ptoa(vm_physmem[0].start);
	avail_end = ptoa(vm_physmem[vm_nphysseg - 1].end);
	virtual_avail = VM_MIN_KERNEL_ADDRESS;
	virtual_end = VM_MIN_KERNEL_ADDRESS + Lev3mapsize * NBPG;

#if 0
	printf("avail_start = 0x%lx\n", avail_start);
	printf("avail_end = 0x%lx\n", avail_end);
	printf("virtual_avail = 0x%lx\n", virtual_avail);
	printf("virtual_end = 0x%lx\n", virtual_end);
#endif

	/*
	 * Intialize the pmap list.
	 */
	LIST_INIT(&pmap_all_pmaps);

	/*
	 * Initialize kernel pmap.
	 */
	pmap_kernel()->pm_lev1map = Lev1map;
	pmap_kernel()->pm_count = 1;
	simple_lock_init(&pmap_kernel()->pm_lock);
	LIST_INSERT_HEAD(&pmap_all_pmaps, pmap_kernel(), pm_list);

	/*
	 * Set up proc0's PCB such that the ptbr points to the right place.
	 */
	proc0.p_addr->u_pcb.pcb_hw.apcb_ptbr =
	    ALPHA_K0SEG_TO_PHYS((vm_offset_t)Lev1map) >> PGSHIFT;
}

#ifdef _PMAP_MAY_USE_PROM_CONSOLE
int
pmap_uses_prom_console()
{
	extern int cputype;

	return (cputype == ST_DEC_21000
	     || cputype == ST_DEC_3000_300
	     || cputype == ST_DEC_3000_500);
}
#endif _PMAP_MAY_USE_PROM_CONSOLE

/*
 * pmap_steal_memory:
 *
 *	Bootstrap memory allocator (alternative to vm_bootstrap_steal_memory()).
 *	This function allows for early dynamic memory allocation until the
 *	virtual memory system has been bootstrapped.  After that point, either
 *	kmem_alloc or malloc should be used.  This function works by stealing
 *	pages from the (to be) managed page pool, then implicitly mapping the
 *	pages (by using their k0seg addresses) and zeroing them.
 *
 *	It may be used once the physical memory segments have been pre-loaded
 *	into the vm_physmem[] array.  Early memory allocation MUST use this
 *	interface!  This cannot be used after vm_page_startup(), and will
 *	generate a panic if tried.
 *
 *	Note that this memory will never be freed, and in essence it is wired
 *	down.
 */
vm_offset_t
pmap_steal_memory(size, vstartp, vendp)
	vm_size_t size;
	vm_offset_t *vstartp, *vendp;
{
	int bank, npgs, x;
	vm_offset_t pa, va;

	size = round_page(size);
	npgs = atop(size);

	for (bank = 0; bank < vm_nphysseg; bank++) {
		if (vm_physmem[bank].pgs)
			panic("pmap_steal_memory: called _after_ bootstrap");

		if (vm_physmem[bank].avail_start != vm_physmem[bank].start ||
		    vm_physmem[bank].avail_start >= vm_physmem[bank].avail_end)
			continue;

		if ((vm_physmem[bank].avail_end - vm_physmem[bank].avail_start)
		    < npgs)
			continue;

		/*
		 * There are enough pages here; steal them!
		 */
		pa = ptoa(vm_physmem[bank].avail_start);
		vm_physmem[bank].avail_start += npgs;
		vm_physmem[bank].start += npgs;

		/*
		 * Have we used up this segment?
		 */
		if (vm_physmem[bank].avail_start == vm_physmem[bank].end) {
			if (vm_nphysseg == 1)
				panic("pmap_steal_memory: out of memory!");

			/* Remove this segment from the list. */
			vm_nphysseg--;
			for (x = bank; x < vm_nphysseg; x++) {
				/* structure copy */
				vm_physmem[x] = vm_physmem[x + 1];
			}
		}

		/*
		 * Fill these in for the caller; we don't modify them,
		 * but the upper layers still want to know.
		 */
		if (vstartp)
			*vstartp = round_page(virtual_avail);
		if (vendp)
			*vendp = trunc_page(virtual_end);

		va = ALPHA_PHYS_TO_K0SEG(pa);
		bzero((caddr_t)va, size);
		return (va);
	}

	/*
	 * If we got here, this was no memory left.
	 */
	panic("pmap_steal_memory: no memory to steal");
}

/*
 * pmap_init:
 *
 *	Initialize the pmap module.  Called by vm_init(), to initialize any
 *	structures that the pmap system needs to map virtual memory.
 */
void
pmap_init()
{
	vm_size_t	s;
	int		bank;
	struct pv_head	*pvh;

#ifdef DEBUG
        if (pmapdebug & PDB_FOLLOW)
                printf("pmap_init()\n");
#endif

	/* initialize protection array */
	alpha_protection_init();

	/*
	 * Memory for the pv heads has already been allocated.
	 * Initialize the physical memory segments.
	 */
	pvh = pv_table;
	for (bank = 0; bank < vm_nphysseg; bank++) {
		s = vm_physmem[bank].end - vm_physmem[bank].start;
		vm_physmem[bank].pmseg.pvhead = pvh;
		pvh += s;
	}

	/*
	 * Intialize the pv_page free list.
	 */
	TAILQ_INIT(&pv_page_freelist);

	/*
	 * Now it is safe to enable pv entry recording.
	 */
	pmap_initialized = TRUE;

#if 0
	for (bank = 0; bank < vm_nphysseg; bank++) {
		printf("bank %d\n", bank);
		printf("\tstart = 0x%x\n", ptoa(vm_physmem[bank].start));
		printf("\tend = 0x%x\n", ptoa(vm_physmem[bank].end));
		printf("\tavail_start = 0x%x\n",
		    ptoa(vm_physmem[bank].avail_start));
		printf("\tavail_end = 0x%x\n",
		    ptoa(vm_physmem[bank].avail_end));
	}
#endif
}

/*
 * pmap_create:
 *
 *	Create and return a physical map.
 */
#if defined(PMAP_NEW)
pmap_t
pmap_create()
#else /* ! PMAP_NEW */
pmap_t
pmap_create(size)
	vm_size_t	size;
#endif /* PMAP_NEW */
{
	register pmap_t pmap;

#if defined(PMAP_NEW)
#ifdef DEBUG
	if (pmapdebug & (PDB_FOLLOW|PDB_CREATE))
		printf("pmap_create()\n");
#endif
#else /* ! PMAP_NEW */
#ifdef DEBUG
	if (pmapdebug & (PDB_FOLLOW|PDB_CREATE))
		printf("pmap_create(%lx)\n", size);
#endif
	/*
	 * Software use map does not need a pmap
	 */
	if (size)
		return(NULL);
#endif /* PMAP_NEW */

	/* XXX: is it ok to wait here? */
	pmap = (pmap_t) malloc(sizeof *pmap, M_VMPMAP, M_WAITOK);
#ifdef notifwewait
	if (pmap == NULL)
		panic("pmap_create: cannot allocate a pmap");
#endif
	bzero(pmap, sizeof(*pmap));
	pmap_pinit(pmap);
	return (pmap);
}

/*
 * pmap_pinit:
 *
 *	Initialize a preallocated and zeroed pmap structure.
 */
void
pmap_pinit(pmap)
	register struct pmap *pmap;
{

#ifdef DEBUG
	if (pmapdebug & (PDB_FOLLOW|PDB_CREATE))
		printf("pmap_pinit(%p)\n", pmap);
#endif

	/*
	 * Defer allocation of a new level 1 page table until
	 * the first new mapping is entered; just take a reference
	 * to the kernel Lev1map.
	 */
	pmap->pm_lev1map = Lev1map;

	pmap->pm_count = 1;
	simple_lock_init(&pmap->pm_lock);
	LIST_INSERT_HEAD(&pmap_all_pmaps, pmap, pm_list);
}

/*
 * pmap_destroy:
 *
 *	Drop the reference count on the specified pmap, releasing
 *	all resources if the reference count drops to zero.
 */
void
pmap_destroy(pmap)
	pmap_t pmap;
{

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_destroy(%p)\n", pmap);
#endif
	if (pmap == NULL)
		return;

	simple_lock(&pmap->pm_lock);
	if (--pmap->pm_count > 0) {
		simple_unlock(&pmap->pm_lock);
		return;
	}

	/*
	 * Reference count is zero: free all resources held by this pmap.
	 */
	pmap_release(pmap);
	free(pmap, M_VMPMAP);
}

/*
 * pmap_release:
 *
 *	Release any resources held by the given physical map.
 *	Called when a pmap initialized by pmap_pinit is being released.
 *	Should only be called if the map contains no valid mappings.
 *
 *	Note: pmap should be locked.
 */
void
pmap_release(pmap)
	pmap_t pmap;
{

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_release(%p)\n", pmap);
#endif

	/*
	 * Remove it from the global list of all pmaps.
	 */
	LIST_REMOVE(pmap, pm_list);

#ifdef DIAGNOSTIC
	/*
	 * Since the pmap is supposed to contain no valid
	 * mappings at this point, this should never happen.
	 */
	if (pmap->pm_lev1map != Lev1map) {
		printf("pmap_release: pmap still contains valid mappings!\n");
		if (pmap->pm_nlev2)
			printf("pmap_release: %ld level 2 tables left\n",
			    pmap->pm_nlev2);
		if (pmap->pm_nlev3)
			printf("pmap_release: %ld level 3 tables left\n",
			    pmap->pm_nlev3);
		pmap_remove(pmap, VM_MIN_ADDRESS, VM_MAX_ADDRESS);
		if (pmap->pm_lev1map != Lev1map)
			panic("pmap_release: pmap_remove() didn't");
	}
#endif
}

/*
 * pmap_reference:
 *
 *	Add a reference to the specified pmap.
 */
void
pmap_reference(pmap)
	pmap_t	pmap;
{
#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_reference(%p)\n", pmap);
#endif
	if (pmap != NULL) {
		simple_lock(&pmap->pm_lock);
		pmap->pm_count++;
		simple_unlock(&pmap->pm_lock);
	}
}

/*
 * pmap_remove:
 *
 *	Remove the given range of addresses from the specified map.
 *
 *	It is assumed that the start and end are properly
 *	rounded to the page size.
 *
 *	XXX This routine could be optimized with regard to its
 *	handling of level 3 PTEs.
 */
void
pmap_remove(pmap, sva, eva)
	register pmap_t pmap;
	register vm_offset_t sva, eva;
{
	register pt_entry_t *pte;
	boolean_t tflush;

#ifdef DEBUG
	if (pmapdebug & (PDB_FOLLOW|PDB_REMOVE|PDB_PROTECT))
		printf("pmap_remove(%p, %lx, %lx)\n", pmap, sva, eva);
#endif

	if (pmap == NULL)
		return;

#ifdef PMAPSTATS
	remove_stats.calls++;
#endif

	tflush = active_pmap(pmap) ? TRUE : FALSE;

	while (sva < eva) {
		/*
		 * If level 1 mapping is invalid, just skip it.
		 */
		if (pmap_pte_v(pmap_l1pte(pmap, sva)) == 0) {
			sva = alpha_trunc_l1seg(sva) + ALPHA_L1SEG_SIZE;
			continue;
		}

		/*
		 * If level 2 mapping is invalid, just skip it.
		 */
		if (pmap_pte_v(pmap_l2pte(pmap, sva)) == 0) {
			sva = alpha_trunc_l2seg(sva) + ALPHA_L2SEG_SIZE;
			continue;
		}

		/*
		 * Invalidate the mapping if it's valid.
		 */
		pte = pmap_l3pte(pmap, sva);
		if (pmap_pte_v(pte))
			pmap_remove_mapping(pmap, sva, pte, tflush);
		sva += PAGE_SIZE;
	}
}

/*
 * pmap_page_protect:
 *
 *	Lower the permission for all mappings to a given page to
 *	the permissions specified.
 */
#if defined(PMAP_NEW)
void
pmap_page_protect(pg, prot)
	struct vm_page *pg;
	vm_prot_t prot;
#else
void
pmap_page_protect(pa, prot)
	vm_offset_t	pa;
	vm_prot_t	prot;
#endif /* PMAP_NEW */
{
	struct pv_head *pvh;
	pv_entry_t pv, nextpv;
	int s;
#if defined(PMAP_NEW)
	vm_offset_t pa = VM_PAGE_TO_PHYS(pg);

#ifdef DEBUG
	if ((pmapdebug & (PDB_FOLLOW|PDB_PROTECT)) ||
	    (prot == VM_PROT_NONE && (pmapdebug & PDB_REMOVE)))
		printf("pmap_page_protect(%p, %x)\n", pg, prot);
#endif

#else /* ! PMAP_NEW */

#ifdef DEBUG
	if ((pmapdebug & (PDB_FOLLOW|PDB_PROTECT)) ||
	    (prot == VM_PROT_NONE && (pmapdebug & PDB_REMOVE)))
		printf("pmap_page_protect(%lx, %x)\n", pa, prot);
#endif
	if (!PAGE_IS_MANAGED(pa))
		return;
#endif /* PMAP_NEW */

	/*
	 * Even though we don't change the mapping of the page,
	 * we still flush the I-cache if VM_PROT_EXECUTE is set
	 * because we might be "adding" execute permissions to
	 * a previously non-execute page.
	 */

	switch (prot) {
	case VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE:
		alpha_pal_imb();
	case VM_PROT_READ|VM_PROT_WRITE:
		return;
	/* copy_on_write */
	case VM_PROT_READ|VM_PROT_EXECUTE:
		alpha_pal_imb();
	case VM_PROT_READ:
/* XXX */	pmap_changebit(pa, PG_KWE | PG_UWE, FALSE);
		return;
	/* remove_all */
	default:
		break;
	}
	pvh = pa_to_pvh(pa);
	s = splimp();
	for (pv = LIST_FIRST(&pvh->pvh_list); pv != NULL; pv = nextpv) {
		register pt_entry_t *pte;

		nextpv = LIST_NEXT(pv, pv_list);

		pte = pmap_l3pte(pv->pv_pmap, pv->pv_va);
#ifdef DEBUG
		if (pmap_pte_v(pmap_l2pte(pv->pv_pmap, pv->pv_va)) == 0 ||
		    pmap_pte_pa(pte) != pa)
			panic("pmap_page_protect: bad mapping");
#endif
		if (!pmap_pte_w(pte))
			pmap_remove_mapping(pv->pv_pmap, pv->pv_va,
					    pte, TRUE);
#ifdef DEBUG
		else {
			if (pmapdebug & PDB_PARANOIA) {
				printf("%s wired mapping for %lx not removed\n",
				       "pmap_page_protect:", pa);
				printf("vm wire count %d\n", 
					PHYS_TO_VM_PAGE(pa)->wire_count);
			}
		}
#endif
	}
	splx(s);
}

/*
 * pmap_protect:
 *
 *	Set the physical protection on the specified range of this map
 *	as requested.
 *
 *	XXX This routine could be optimized with regard to its
 *	handling of level 3 PTEs.
 */
void
pmap_protect(pmap, sva, eva, prot)
	register pmap_t	pmap;
	register vm_offset_t sva, eva;
	vm_prot_t prot;
{
	register pt_entry_t *pte, bits;
	boolean_t needtflush;

#ifdef DEBUG
	if (pmapdebug & (PDB_FOLLOW|PDB_PROTECT))
		printf("pmap_protect(%p, %lx, %lx, %x)\n",
		    pmap, sva, eva, prot);
#endif

	if (pmap == NULL)
		return;

#ifdef PMAPSTATS
	protect_stats.calls++;
#endif

	if ((prot & VM_PROT_READ) == VM_PROT_NONE) {
		pmap_remove(pmap, sva, eva);
		return;
	}

	if (prot & VM_PROT_WRITE)
		return;

	bits = pte_prot(pmap, prot);
	needtflush = active_pmap(pmap);

	while (sva < eva) {
		/*
		 * If level 1 mapping is invalid, just skip it.
		 */
		if (pmap_pte_v(pmap_l1pte(pmap, sva)) == 0) {
			sva = alpha_trunc_l1seg(sva) + ALPHA_L1SEG_SIZE;
			continue;
		}

		/*
		 * If level 2 mapping is invalid, just skip it.
		 */
		if (pmap_pte_v(pmap_l2pte(pmap, sva)) == 0) {
			sva = alpha_trunc_l2seg(sva) + ALPHA_L2SEG_SIZE;
			continue;
		}

		/*
		 * Change protection on mapping if it is valid and doesn't
		 * already have the correct protection.
		 */
		pte = pmap_l3pte(pmap, sva);
		if (pmap_pte_v(pte) && pmap_pte_prot_chg(pte, bits)) {
			pmap_pte_set_prot(pte, bits);
			if (needtflush) {
				ALPHA_TBIS(sva);
				if (prot & VM_PROT_EXECUTE)
					alpha_pal_imb();
			}
#ifdef PMAPSTATS
			protect_stats.changed++;
#endif
		}
#ifdef PMAPSTATS
		else if (pmap_pte_v(pte)) {
			if (isro)
				protect_stats.alreadyro++;
			else
				protect_stats.alreadyrw++;
		}
#endif
		sva += PAGE_SIZE;
	}
}

/*
 * pmap_enter:
 *
 *	Insert the given physical page (p) at
 *	the specified virtual address (v) in the
 *	target physical map with the protection requested.
 *
 *	If specified, the page will be wired down, meaning
 *	that the related pte can not be reclaimed.
 *
 *	Note:  This is the only routine which MAY NOT lazy-evaluate
 *	or lose information.  That is, this routine must actually
 *	insert this page into the given map NOW.
 */
void
pmap_enter(pmap, va, pa, prot, wired)
	register pmap_t pmap;
	vm_offset_t va, pa;
	vm_prot_t prot;
	boolean_t wired;
{
	boolean_t managed;
	pt_entry_t *pte, npte;
	vm_offset_t opa;
	boolean_t wiring_only = FALSE;

#ifdef DEBUG
	if (pmapdebug & (PDB_FOLLOW|PDB_ENTER))
		printf("pmap_enter(%p, %lx, %lx, %x, %x)\n",
		       pmap, va, pa, prot, wired);
#endif
	if (pmap == NULL)
		return;

	managed = PAGE_IS_MANAGED(pa);

	if (pmap == pmap_kernel()) {
#ifdef PMAPSTATS
		enter_stats.kernel++
#endif
#ifdef DEBUG
		/*
		 * All kernel level 2 and 3 page tables are
		 * pre-allocated and mapped in.  Therefore, the
		 * level 1 and level 2 PTEs must already be valid.
		 */
		if (pmap_pte_v(pmap_l1pte(pmap, va)) == 0)
			panic("pmap_enter: kernel level 1 PTE not valid");
		if (pmap_pte_v(pmap_l2pte(pmap, va)) == 0)
			panic("pmap_enter: kernel level 2 PTE not valid");

		/*
		 * Get the PTE that will map the page.
		 */
		pte = pmap_l3pte(pmap, va);
#else
		/*
		 * All kernel level 2 and 3 page tables are
		 * pre-allocated and mapped in, so we can use
		 * the Virtual Page Table to get the PTE for
		 * kernel mappings.
		 */
		pte = &VPT[VPT_INDEX(va)];
#endif
	} else {
		pt_entry_t *l1pte, *l2pte;

#ifdef PMAPSTATS
		enter_stats.user++;
#endif
		/*
		 * If we're still referencing the kernel Lev1map, create
		 * a new level 1 page table.  A reference will be added
		 * to the level 1 table when the level 2 table is
		 * created.
		 */
		if (pmap->pm_lev1map == Lev1map)
			pmap_create_lev1map(pmap);

		/*
		 * Check to see if the level 1 PTE is valid, and
		 * allocate a new level 2 page table page if it's not.
		 * A reference will be added to the level 2 table when
		 * the level 3 table is created.
		 */
		l1pte = pmap_l1pte(pmap, va);
		if (pmap_pte_v(l1pte) == 0) {
			pmap_alloc_ptpage(pmap, l1pte);
			pmap_ptpage_addref(l1pte);
			pmap->pm_nlev2++;
#ifdef DEBUG
			if (pmapdebug & PDB_PTPAGE)
				printf("pmap_enter: new level 2 table at "
				    "0x%lx\n", pmap_pte_pa(l1pte));
#endif
		}

		/*
		 * Check to see if the level 2 PTE is valid, and
		 * allocate a new level 3 page table page if it's not.
		 * A reference will be added to the level 3 table when
		 * the mapping is validated.
		 */
		l2pte = pmap_l2pte(pmap, va);
		if (pmap_pte_v(l2pte) == 0) {
			pmap_alloc_ptpage(pmap, l2pte);
			pmap_ptpage_addref(l2pte);
			pmap->pm_nlev3++;
#ifdef DEBUG
			if (pmapdebug & PDB_PTPAGE)
				printf("pmap_enter: new level 3 table at "
				    "0x%lx\n", pmap_pte_pa(l2pte));
#endif
		}

		/*
		 * Get the PTE that will map the page.
		 */
		pte = pmap_l3pte(pmap, va);
	}

	/*
	 * Check to see if the old mapping is valid.  If not, validate the
	 * new one immediately.
	 */
	if (pmap_pte_v(pte) == 0) {
		if (pmap != pmap_kernel()) {
			/*
			 * New mappings gain a reference on the level 3
			 * table.
			 */
			pmap_ptpage_addref(pte);
		}
		goto validate_enterpv;
	}

	opa = pmap_pte_pa(pte);
	if (opa == pa) {
		/*
		 * Mapping has not changed; must be a protection or
		 * wiring change.
		 */
#ifdef PMAPSTATS
		enter_stats.pwchange++;
#endif
		if (pmap_pte_w_chg(pte, wired ? PG_WIRED : 0)) {
#ifdef DEBUG
			if (pmapdebug & PDB_ENTER)
				printf("pmap_enter: wiring change -> %d\n",
				    wired);
#endif
			/*
			 * Adjust the wiring count.
			 */
			if (wired)
				pmap->pm_stats.wired_count++;
			else
				pmap->pm_stats.wired_count--;

			/*
			 * Check to see if this is a wiring-only
			 * change.
			 */
			if (pmap_pte_prot(pte) == pte_prot(pmap, prot)) {
				wiring_only = TRUE;
#ifdef PMAPSTATS
				enter_stats.wchange++;
#endif
			}
		}
#ifdef PMAPSTATS
		else if (pmap_pte_prot(pte) != pte_prot(pmap, prot))
			enter_stats.pchange++;
		else
			enter_stats.nochange++;
#endif
		/*
		 * Set the PTE.
		 */
		goto validate;
	}

	/*
	 * The mapping has changed.  We need to invalidate the
	 * old mapping before creating the new one.
	 */
#ifdef DEBUG
	if (pmapdebug & PDB_ENTER)
		printf("pmap_enter: removing old mapping 0x%lx\n", va);
#endif
	if (pmap != pmap_kernel()) {
		/*
		 * Gain an extra reference on the level 3 table.
		 * pmap_remove_mapping() will delete a reference,
		 * and we don't want the table to be erroneously
		 * freed.
		 */
		pmap_ptpage_addref(pte);
	}
	pmap_remove_mapping(pmap, va, pte, TRUE);
#ifdef PMAPSTATS
	enter_stats.mchange++;
#endif

 validate_enterpv:
	/*
	 * Enter the mapping into the pv_table if appropriate.
	 */
	if (managed) {
		int s = splimp();
		pmap_enter_pv(pmap, pa, va);
		splx(s);
	}

	/*
	 * Increment counters.
	 */
	pmap->pm_stats.resident_count++;
	if (wired)
		pmap->pm_stats.wired_count++;

 validate:
	/*
	 * Build the new PTE.
	 */
	npte = ((pa >> PGSHIFT) << PG_SHIFT) | pte_prot(pmap, prot) | PG_V;
	if (managed) {
		struct pv_head *pvh = pa_to_pvh(pa);

		/*
		 * Set up referenced/modified emulation for new mapping.
		 */
		if ((pvh->pvh_attrs & PMAP_ATTR_REF) == 0)
			npte |= PG_FOR | PG_FOW | PG_FOE;
		else if ((pvh->pvh_attrs & PMAP_ATTR_MOD) == 0)
			npte |= PG_FOW;

		/*
		 * Mapping was entered on PV list.
		 */
		npte |= PG_PVLIST;
	}
	if (wired)
		npte |= PG_WIRED;
#ifdef DEBUG
	if (pmapdebug & PDB_ENTER)
		printf("pmap_enter: new pte = 0x%lx\n", npte);
#endif

	/*
	 * Set the new PTE.
	 */
	*pte = npte;

	/*
	 * Invalidate the TLB entry for this VA and any appropriate
	 * caches.  Note that this is not necessary for wiring-only
	 * changes.
	 */
	if (wiring_only == FALSE && active_pmap(pmap)) {
		ALPHA_TBIS(va);
		if (prot & VM_PROT_EXECUTE)
			alpha_pal_imb();
	}
}

#if defined(PMAP_NEW)
/*
 * pmap_kenter_pa:
 *
 *	Enter a va -> pa mapping into the kernel pmap without any
 *	physical->virtual tracking.
 */
void
pmap_kenter_pa(va, pa, prot)
	vm_offset_t va;
	vm_offset_t pa;
	vm_prot_t prot;
{
	pt_entry_t *pte, npte;

#ifdef DEBUG
	if (pmapdebug & (PDB_FOLLOW|PDB_ENTER))
		printf("pmap_kenter_pa(%lx, %lx, %x)\n",
		    va, pa, prot);

	/*
	 * All kernel level 2 and 3 page tables are
	 * pre-allocated and mapped in.  Therefore, the
	 * level 1 and level 2 PTEs must already be valid.
	 */
	if (pmap_pte_v(pmap_l1pte(pmap_kernel(), va)) == 0)
		panic("pmap_kenter_pa: kernel level 1 PTE not valid");
	if (pmap_pte_v(pmap_l2pte(pmap_kernel(), va)) == 0)
		panic("pmap_kenter_pa: kernel level 2 PTE not valid");

	/*
	 * Get the PTE that will map the page.
	 */
	pte = pmap_l3pte(pmap_kernel(), va);
#else
	/*
	 * All kernel level 2 and 3 page tables are
	 * pre-allocated and mapped in, so we can use
	 * the Virtual Page Table to get the PTE for
	 * kernel mappings.
	 */
	pte = &VPT[VPT_INDEX(va)];
#endif

	/*
	 * Build the new PTE.
	 */
	npte = ((pa >> PGSHIFT) << PG_SHIFT) | pte_prot(pmap_kernel(), prot) |
	    PG_V | PG_WIRED;

	/*
	 * Set the new PTE.
	 */
	*pte = npte;

	/*
	 * Invalidate the TLB entry for this VA and any appropriate
	 * caches.
	 */
	ALPHA_TBIS(va);
	if (prot & VM_PROT_EXECUTE)
		alpha_pal_imb();
}

/*
 * pmap_kenter_pgs:
 *
 *	Enter a va -> pa mapping for the array of vm_page's into the
 *	kernel pmap without any physical->virtual tracking, starting
 *	at address va, for npgs pages.
 */
void
pmap_kenter_pgs(va, pgs, npgs)
	vm_offset_t va;
	vm_page_t *pgs;
	int npgs;
{
	int i;

#ifdef DEBUG
	if (pmapdebug & (PDB_FOLLOW|PDB_ENTER))
		printf("pmap_kenter_pgs(%lx, %p, %d)\n",
		    va, pgs, npgs);
#endif

	for (i = 0; i < npgs; i++)
		pmap_kenter_pa(va + (PAGE_SIZE * i),
		    VM_PAGE_TO_PHYS(pgs[i]),
		    VM_PROT_READ|VM_PROT_WRITE);
}

/*
 * pmap_kremove:
 *
 *	Remove a mapping entered with pmap_kenter_pa() or pmap_kenter_pgs()
 *	starting at va, for size bytes (assumed to be page rounded).
 */
void
pmap_kremove(va, size)
	vm_offset_t va;
	vm_size_t size;
{
	pt_entry_t *pte;

#ifdef DEBUG
	if (pmapdebug & (PDB_FOLLOW|PDB_ENTER))
		printf("pmap_kremove(%lx, %lx)\n",
		    va, size);
#endif

	for (; size != 0; size -= PAGE_SIZE, va += PAGE_SIZE) {
#ifdef DEBUG
		/*
		 * All kernel level 2 and 3 page tables are
		 * pre-allocated and mapped in.  Therefore, the
		 * level 1 and level 2 PTEs must already be valid.
		 */
		if (pmap_pte_v(pmap_l1pte(pmap_kernel(), va)) == 0)
			panic("pmap_kremove: kernel level 1 PTE not valid");
		if (pmap_pte_v(pmap_l2pte(pmap_kernel(), va)) == 0)
			panic("pmap_kremove: kernel level 2 PTE not valid");

		/*
		 * Get the PTE that will map the page.
		 */
		pte = pmap_l3pte(pmap_kernel(), va);
#else
		/*
		 * All kernel level 2 and 3 page tables are
		 * pre-allocated and mapped in, so we can use
		 * the Virtual Page Table to get the PTE for
		 * kernel mappings.
		 */
		pte = &VPT[VPT_INDEX(va)];
#endif
		*pte = PG_NV;
		ALPHA_TBIS(va);
	}

	/* Probably isn't needed, but what the hell... */
	alpha_pal_imb();
}
#endif /* PMAP_NEW */

/*
 * pmap_change_wiring:
 *
 *	Change the wiring attribute for a map/virtual-address pair.
 *
 *	The mapping must already exist in the pmap.
 */
void
pmap_change_wiring(pmap, va, wired)
	register pmap_t	pmap;
	vm_offset_t	va;
	boolean_t	wired;
{
	register pt_entry_t *pte;

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_change_wiring(%p, %lx, %x)\n", pmap, va, wired);
#endif
	if (pmap == NULL)
		return;

	pte = pmap_l3pte(pmap, va);
#ifdef DEBUG
	/*
	 * Page table page is not allocated.
	 * Should this ever happen?  Ignore it for now,
	 * we don't want to force allocation of unnecessary PTE pages.
	 */
	if (pmap_pte_v(pmap_l2pte(pmap, va)) == 0) {
		if (pmapdebug & PDB_PARANOIA)
			printf("pmap_change_wiring: invalid STE for %lx\n", va);
		return;
	}
	/*
	 * Page not valid.  Should this ever happen?
	 * Just continue and change wiring anyway.
	 */
	if (!pmap_pte_v(pte)) {
		if (pmapdebug & PDB_PARANOIA)
			printf("pmap_change_wiring: invalid PTE for %lx\n", va);
	}
#endif
	/*
	 * If wiring actually changed (always?) set the wire bit and
	 * update the wire count.  Note that wiring is not a hardware
	 * characteristic so there is no need to invalidate the TLB.
	 */
	if (pmap_pte_w_chg(pte, wired ? PG_WIRED : 0)) {
		pmap_pte_set_w(pte, wired);
		if (wired)
			pmap->pm_stats.wired_count++;
		else
			pmap->pm_stats.wired_count--;
	}
}

/*
 * pmap_extract:
 *
 *	Extract the physical address associated with the given
 *	pmap/virtual address pair.
 */
vm_offset_t
pmap_extract(pmap, va)
	register pmap_t	pmap;
	vm_offset_t va;
{
	pt_entry_t *pte;
	register vm_offset_t pa;

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_extract(%p, %lx) -> ", pmap, va);
#endif
	pa = 0;

	if (pmap_pte_v(pmap_l1pte(pmap, va)) == 0)
		goto out;
	if (pmap_pte_v(pmap_l2pte(pmap, va)) == 0)
		goto out;

	pte = pmap_l3pte(pmap, va);
	if (pmap_pte_v(pte) == 0)
		goto out;

	pa = pmap_pte_pa(pte) | (va & PGOFSET);

 out:
#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("0x%lx\n", pa);
#endif
	return (pa);
}

/*
 * pmap_copy:
 *
 *	Copy the mapping range specified by src_addr/len
 *	from the source map to the range dst_addr/len
 *	in the destination map.
 *
 *	This routine is only advisory and need not do anything.
 */
void
pmap_copy(dst_pmap, src_pmap, dst_addr, len, src_addr)
	pmap_t		dst_pmap;
	pmap_t		src_pmap;
	vm_offset_t	dst_addr;
	vm_size_t	len;
	vm_offset_t	src_addr;
{
#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_copy(%p, %p, %lx, %lx, %lx)\n",
		       dst_pmap, src_pmap, dst_addr, len, src_addr);
#endif
}

/*
 * pmap_update:
 *
 *	Require that all active physical maps contain no
 *	incorrect entries NOW.  [This update includes
 *	forcing updates of any address map caching.]
 *
 *	Generally used to insure that a thread about
 *	to run will see a semantically correct world.
 */
void
pmap_update()
{
#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_update()\n");
#endif
	ALPHA_TBIA();
	alpha_pal_imb();
}

/*
 * pmap_collect:
 *
 *	Garbage collects the physical map system for pages which are no
 *	longer used.  Success need not be guaranteed -- that is, there
 *	may well be pages which are not referenced, but others may be
 *	collected.
 *
 *	Called by the pageout daemon when pages are scarce.
 */
void
pmap_collect(pmap)
	pmap_t		pmap;
{

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_collect(%p)\n", pmap);
#endif

#ifdef notyet
	/* Go compact and garbage-collect the pv_table. */
	pmap_collect_pv();
#endif
}

/*
 * pmap_activate:
 *
 *	Activate the pmap used by the specified process.  This includes
 *	reloading the MMU context if the current process, and marking
 *	the pmap in use by the processor.
 */
void
pmap_activate(p)
	struct proc *p;
{
	struct pmap *pmap = p->p_vmspace->vm_map.pmap;

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_activate(%p)\n", p);
#endif

	/* XXX Allocate an ASN. */

	PMAP_ACTIVATE(pmap, p);
}

/*
 * pmap_deactivate:
 *
 *	Mark that the pmap used by the specified process is no longer
 *	in use by the processor.
 */
void
pmap_deactivate(p)
	struct proc *p;
{

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_deactivate(%p)\n", p);
#endif

	/* XXX Free the ASN. */
}

/*
 * pmap_zero_page:
 *
 *	Zero the specified (machine independent) page by mapping the page
 *	into virtual memory and using bzero to clear its contents, one
 *	machine dependent page at a time.
 */
void
pmap_zero_page(phys)
	vm_offset_t phys;
{
	caddr_t p;

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_zero_page(%lx)\n", phys);
#endif
	p = (caddr_t)ALPHA_PHYS_TO_K0SEG(phys);
	bzero(p, PAGE_SIZE);
}

/*
 * pmap_copy_page:
 *
 *	Copy the specified (machine independent) page by mapping the page
 *	into virtual memory and using bcopy to copy the page, one machine
 *	dependent page at a time.
 */
void
pmap_copy_page(src, dst)
	vm_offset_t src, dst;
{
	caddr_t s, d;

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_copy_page(%lx, %lx)\n", src, dst);
#endif
        s = (caddr_t)ALPHA_PHYS_TO_K0SEG(src);
        d = (caddr_t)ALPHA_PHYS_TO_K0SEG(dst);
	bcopy(s, d, PAGE_SIZE);
}

/*
 * pmap_pageable:
 *
 *	Make the specified pages (by pmap, offset) pageable (or not) as
 *	requested.
 *
 *	A page which is not pageable may not take a fault; therefore,
 *	its page table entry must remain valid for the duration.
 *
 *	This routine is merely advisory; pmap_enter() will specify that
 *	these pages are to be wired down (or not) as appropriate.
 */
void
pmap_pageable(pmap, sva, eva, pageable)
	pmap_t		pmap;
	vm_offset_t	sva, eva;
	boolean_t	pageable;
{

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_pageable(%p, %lx, %lx, %x)\n",
		       pmap, sva, eva, pageable);
#endif
}

/*
 * pmap_clear_modify:
 *
 *	Clear the modify bits on the specified physical page.
 */
#if defined(PMAP_NEW)
boolean_t
pmap_clear_modify(pg)
	struct vm_page *pg;
{
	struct pv_head *pvh;
	vm_offset_t pa = VM_PAGE_TO_PHYS(pg);
	boolean_t rv = FALSE;

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_clear_modify(%p)\n", pg);
#endif

	pvh = pa_to_pvh(pa);
	if (pvh->pvh_attrs & PMAP_ATTR_MOD) {
		rv = TRUE;
		pmap_changebit(pa, PG_FOW, TRUE);
		pvh->pvh_attrs &= ~PMAP_ATTR_MOD;
	}

	return (rv);
}
#else /* ! PMAP_NEW */
void
pmap_clear_modify(pa)
	vm_offset_t	pa;
{
	struct pv_head *pvh;

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_clear_modify(%lx)\n", pa);
#endif
	if (!PAGE_IS_MANAGED(pa))		/* XXX why not panic? */
		return;
	pvh = pa_to_pvh(pa);
	if (pvh->pvh_attrs & PMAP_ATTR_MOD) {
		pmap_changebit(pa, PG_FOW, TRUE); 
		pvh->pvh_attrs &= ~PMAP_ATTR_MOD;
	}
}
#endif /* PMAP_NEW */

/*
 * pmap_clear_reference:
 *
 *	Clear the reference bit on the specified physical page.
 */
#if defined(PMAP_NEW)
boolean_t
pmap_clear_reference(pg)
	struct vm_page *pg;
{
	struct pv_head *pvh;
	vm_offset_t pa = VM_PAGE_TO_PHYS(pg);
	boolean_t rv = FALSE;

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_clear_reference(%p)\n", pg);
#endif

	pvh = pa_to_pvh(pa);
	if (pvh->pvh_attrs & PMAP_ATTR_REF) {
		rv = TRUE;
		pmap_changebit(pa, PG_FOR | PG_FOW | PG_FOE, TRUE);
		pvh->pvh_attrs &= ~PMAP_ATTR_REF;
	}

	return (rv);
}
#else /* ! PMAP_NEW */
void
pmap_clear_reference(pa)
	vm_offset_t	pa;
{
	struct pv_head *pvh;

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_clear_reference(%lx)\n", pa);
#endif
	if (!PAGE_IS_MANAGED(pa))		/* XXX why not panic? */
		return;
	pvh = pa_to_pvh(pa);
	if (pvh->pvh_attrs & PMAP_ATTR_REF) {
		pmap_changebit(pa, PG_FOR | PG_FOW | PG_FOE, TRUE);
		pvh->pvh_attrs &= ~PMAP_ATTR_REF;
	}
}
#endif /* PMAP_NEW */

/*
 * pmap_is_referenced:
 *
 *	Return whether or not the specified physical page is referenced
 *	by any physical maps.
 */
#if defined(PMAP_NEW)
boolean_t
pmap_is_referenced(pg)
	struct vm_page *pg;
{
	struct pv_head *pvh;
	vm_offset_t pa = VM_PAGE_TO_PHYS(pg);
	boolean_t rv;

	pvh = pa_to_pvh(pa);
	rv = ((pvh->pvh_attrs & PMAP_ATTR_REF) != 0);
#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW) {
		printf("pmap_is_referenced(%p) -> %c\n", pg, "FT"[rv]);
	}
#endif
	return (rv);
}
#else /* ! PMAP_NEW */
boolean_t
pmap_is_referenced(pa)
	vm_offset_t	pa;
{
	struct pv_head *pvh;
	boolean_t rv;

	if (!PAGE_IS_MANAGED(pa))		/* XXX why not panic? */
		return 0;

	pvh = pa_to_pvh(pa);
	rv = ((pvh->pvh_attrs & PMAP_ATTR_REF) != 0);
#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW) {
		printf("pmap_is_referenced(%lx) -> %c\n", pa, "FT"[rv]);
	}
#endif
	return rv;
}
#endif /* PMAP_NEW */

/*
 * pmap_is_modified:
 *
 *	Return whether or not the specified physical page is modified
 *	by any physical maps.
 */
#if defined(PMAP_NEW)
boolean_t
pmap_is_modified(pg)
	struct vm_page *pg;
{
	struct pv_head *pvh;
	vm_offset_t pa = VM_PAGE_TO_PHYS(pg);
	boolean_t rv;

	pvh = pa_to_pvh(pa);
	rv = ((pvh->pvh_attrs & PMAP_ATTR_MOD) != 0);
#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW) {
		printf("pmap_is_modified(%p) -> %c\n", pg, "FT"[rv]);
	}
#endif
	return (rv);
}
#else /* ! PMAP_NEW */
boolean_t
pmap_is_modified(pa)
	vm_offset_t	pa;
{
	struct pv_head *pvh;
	boolean_t rv;

	if (!PAGE_IS_MANAGED(pa))		/* XXX why not panic? */
		return 0;

	pvh = pa_to_pvh(pa);
	rv = ((pvh->pvh_attrs & PMAP_ATTR_MOD) != 0);
#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW) {
		printf("pmap_is_modified(%lx) -> %c\n", pa, "FT"[rv]);
	}
#endif
	return rv;
}
#endif /* PMAP_NEW */

/*
 * pmap_phys_address:
 *
 *	Return the physical address corresponding to the specified
 *	cookie.  Used by the device pager to decode a device driver's
 *	mmap entry point return value.
 */
vm_offset_t
pmap_phys_address(ppn)
	int ppn;
{

	return (alpha_ptob(ppn));
}

/*
 * Miscellaneous support routines follow
 */

/*
 * alpha_protection_init:
 *
 *	Initialize Alpha protection code array.
 */
/* static */
void
alpha_protection_init()
{
	int prot, *kp, *up;

	kp = protection_codes[0];
	up = protection_codes[1];

	for (prot = 0; prot < 8; prot++) {
		switch (prot) {
		case VM_PROT_NONE | VM_PROT_NONE | VM_PROT_NONE:
			*kp++ = PG_ASM;
			*up++ = 0;
			break;
		case VM_PROT_READ | VM_PROT_NONE | VM_PROT_NONE:
		case VM_PROT_READ | VM_PROT_NONE | VM_PROT_EXECUTE:
		case VM_PROT_NONE | VM_PROT_NONE | VM_PROT_EXECUTE:
			*kp++ = PG_ASM | PG_KRE;
			*up++ = PG_URE | PG_KRE;
			break;
		case VM_PROT_NONE | VM_PROT_WRITE | VM_PROT_NONE:
			*kp++ = PG_ASM | PG_KWE;
			*up++ = PG_UWE | PG_KWE;
			break;
		case VM_PROT_NONE | VM_PROT_WRITE | VM_PROT_EXECUTE:
		case VM_PROT_READ | VM_PROT_WRITE | VM_PROT_NONE:
		case VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE:
			*kp++ = PG_ASM | PG_KWE | PG_KRE;
			*up++ = PG_UWE | PG_URE | PG_KWE | PG_KRE;
			break;
		}
	}
}

/*
 * pmap_remove_mapping:
 *
 *	Invalidate a single page denoted by pmap/va.
 *
 *	If (pte != NULL), it is the already computed PTE for the page.
 */
/* static */
void
pmap_remove_mapping(pmap, va, pte, tflush)
	register pmap_t pmap;
	register vm_offset_t va;
	register pt_entry_t *pte;
	boolean_t tflush;
{
	vm_offset_t pa;
	int s;
	boolean_t onpv;

#ifdef DEBUG
	if (pmapdebug & (PDB_FOLLOW|PDB_REMOVE|PDB_PROTECT))
		printf("pmap_remove_mapping(%p, %lx, %p, %d)\n",
		       pmap, va, pte, tflush);
#endif

	/*
	 * PTE not provided, compute it from pmap and va.
	 */
	if (pte == PT_ENTRY_NULL) {
		pte = pmap_l3pte(pmap, va);
		if (pmap_pte_v(pte) == 0)
			return;
	}

	pa = pmap_pte_pa(pte);
	onpv = (pmap_pte_pv(pte) != 0);

#ifdef PMAPSTATS
	remove_stats.removes++;
#endif
	/*
	 * Update statistics
	 */
	if (pmap_pte_w(pte))
		pmap->pm_stats.wired_count--;
	pmap->pm_stats.resident_count--;

	/*
	 * Invalidate the PTE after saving the reference modify info.
	 */
#ifdef DEBUG
	if (pmapdebug & PDB_REMOVE)
		printf("remove: invalidating pte at %p\n", pte);
#endif
	*pte = PG_NV;

	if (tflush && active_pmap(pmap)) {
		ALPHA_TBIS(va);
		alpha_pal_imb();
	}

	/*
	 * If we're removing a user mapping, check to see if we
	 * can free page table pages.
	 */
	if (pmap != pmap_kernel()) {
		pt_entry_t *l1pte, *l2pte;

		l1pte = pmap_l1pte(pmap, va);
		l2pte = pmap_l2pte(pmap, va);

		/*
		 * Delete the reference on the level 3 table.
		 */
		if (pmap_ptpage_delref(pte) == 0) {
			/*
			 * No more mappings; we can free the level 3 table.
			 */
#ifdef DEBUG
			if (pmapdebug & PDB_PTPAGE)
				printf("pmap_remove_mapping: freeing level 3 "
				    "table at 0x%lx\n", pmap_pte_pa(l2pte));
#endif
			pmap_free_ptpage(pmap, l2pte);
			pmap->pm_nlev3--;

			/*
			 * We've freed a level 3 table, so we must
			 * invalidate the TLB entry for that PT page
			 * in the Virtual Page Table VA range, because
			 * otherwise the PALcode will service a TLB
			 * miss using the stale VPT TLB entry it entered
			 * behind our back to shortcut to the VA's PTE.
			 */
			ALPHA_TBIS((vm_offset_t)(&VPT[VPT_INDEX(va)]));

			/*
			 * We've freed a level 3 table, so delete the
			 * reference on the level 2 table.
			 */
			if (pmap_ptpage_delref(l2pte) == 0) {
				/*
				 * No more mappings in this segment; we
				 * can free the level 2 table.
				 */
#ifdef DEBUG
				if (pmapdebug & PDB_PTPAGE)
					printf("pmap_remove_mapping: freeing "
					    "level 2 table at 0x%lx\n",
					    pmap_pte_pa(l1pte));
#endif
				pmap_free_ptpage(pmap, l1pte);
				pmap->pm_nlev2--;

				/*
				 * We've freed a level 2 table, so delete
				 * the reference on the level 1 table.
				 */
				if (pmap_ptpage_delref(l1pte) == 0) {
					/*
					 * No more level 2 tables left,
					 * go back to global Lev1map.
					 */
					pmap_destroy_lev1map(pmap);
				}
			}
		}
	}

	/*
	 * If the mapping wasn't enterd on the PV list, we're all done.
	 */
	if (onpv == FALSE)
		return;

	/*
	 * Otherwise remove it from the PV table
	 * (raise IPL since we may be called at interrupt time).
	 */
	s = splimp();
	pmap_remove_pv(pmap, pa, va);
	splx(s);
}

/*
 * pmap_changebit:
 *
 *	Set or clear the specified PTE bits for all mappings on the
 *	specified page.
 */
/* static */
void
pmap_changebit(pa, bit, setem)
	register vm_offset_t pa;
	u_long bit;
	boolean_t setem;
{
	struct pv_head *pvh;
	pv_entry_t pv;
	pt_entry_t *pte, npte;
	vm_offset_t va;
	int s;
#ifdef PMAPSTATS
	struct chgstats *chgp;
#endif

#ifdef DEBUG
	if (pmapdebug & PDB_BITS)
		printf("pmap_changebit(%lx, %lx, %s)\n",
		       pa, bit, setem ? "set" : "clear");
#endif
	if (!PAGE_IS_MANAGED(pa))
		return;

#ifdef PMAPSTATS
	chgp = &changebit_stats[(bit>>2)-1];
	if (setem)
		chgp->setcalls++;
	else
		chgp->clrcalls++;
#endif
	pvh = pa_to_pvh(pa);
	s = splimp();
	/*
	 * Loop over all current mappings setting/clearing as appropos.
	 */
	for (pv = LIST_FIRST(&pvh->pvh_list); pv != NULL;
	     pv = LIST_NEXT(pv, pv_list)) {
		va = pv->pv_va;

		/*
		 * XXX don't write protect pager mappings
		 */
/* XXX */	if (bit == (PG_UWE | PG_KWE)) {
#if defined(UVM)
			if (va >= uvm.pager_sva && va < uvm.pager_eva)
				continue;
#else
			extern vm_offset_t pager_sva, pager_eva;

			if (va >= pager_sva && va < pager_eva)
				continue;
#endif
		}

		pte = pmap_l3pte(pv->pv_pmap, va);
		if (setem)
			npte = *pte | bit;
		else
			npte = *pte & ~bit;
		if (*pte != npte) {
			*pte = npte;
			if (active_pmap(pv->pv_pmap))
				ALPHA_TBIS(va);
#ifdef PMAPSTATS
			if (setem)
				chgp->sethits++;
			else
				chgp->clrhits++;
#endif
		}
#ifdef PMAPSTATS
		else {
			if (setem)
				chgp->setmiss++;
			else
				chgp->clrmiss++;
		}
#endif
	}
	splx(s);
}

/*
 * pmap_emulate_reference:
 *
 *	Emulate reference and/or modified bit hits.
 */
void
pmap_emulate_reference(p, v, user, write)
	struct proc *p;
	vm_offset_t v;
	int user;
	int write;
{
	pt_entry_t faultoff, *pte;
	vm_offset_t pa;
	struct pv_head *pvh;

#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("pmap_emulate_reference: %p, 0x%lx, %d, %d\n",
		    p, v, user, write);
#endif

	/*
	 * Convert process and virtual address to physical address.
	 */
	if (v >= VM_MIN_KERNEL_ADDRESS) {
		if (user)
			panic("pmap_emulate_reference: user ref to kernel");
#ifdef DEBUG
		pte = pmap_l3pte(pmap_kernel(), v);
#else
		pte = &VPT[VPT_INDEX(v)];
#endif
	} else {
#ifdef DIAGNOSTIC
		if (p == NULL)
			panic("pmap_emulate_reference: bad proc");
		if (p->p_vmspace == NULL)
			panic("pmap_emulate_reference: bad p_vmspace");
#endif
		pte = pmap_l3pte(p->p_vmspace->vm_map.pmap, v);
	}
#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW) {
		printf("\tpte = %p, ", pte);
		printf("*pte = 0x%lx\n", *pte);
	}
#endif
#ifdef DEBUG				/* These checks are more expensive */
	if (!pmap_pte_v(pte))
		panic("pmap_emulate_reference: invalid pte");
#if 0
	/*
	 * Can't do these, because cpu_fork and cpu_swapin call
	 * pmap_emulate_reference(), and the bits aren't guaranteed,
	 * for them...
	 */
	if (write) {
		if (!(*pte & (user ? PG_UWE : PG_UWE | PG_KWE)))
			panic("pmap_emulate_reference: write but unwritable");
		if (!(*pte & PG_FOW))
			panic("pmap_emulate_reference: write but not FOW");
	} else {
		if (!(*pte & (user ? PG_URE : PG_URE | PG_KRE)))
			panic("pmap_emulate_reference: !write but unreadable");
		if (!(*pte & (PG_FOR | PG_FOE)))
			panic("pmap_emulate_reference: !write but not FOR|FOE");
	}
#endif
	/* Other diagnostics? */
#endif
	pa = pmap_pte_pa(pte);
#ifdef DEBUG
	if (pmapdebug & PDB_FOLLOW)
		printf("\tpa = 0x%lx\n", pa);
#endif
#ifdef DIAGNOSTIC
	if (!PAGE_IS_MANAGED(pa))
		panic("pmap_emulate_reference(%p, 0x%lx, %d, %d): pa 0x%lx not managed", p, v, user, write, pa);
#endif

	/*
	 * Twiddle the appropriate bits to reflect the reference
	 * and/or modification..
	 *
	 * The rules:
	 * 	(1) always mark page as used, and
	 *	(2) if it was a write fault, mark page as modified.
	 */
	pvh = pa_to_pvh(pa);
	pvh->pvh_attrs = PMAP_ATTR_REF;
	faultoff = PG_FOR | PG_FOE;
	if (write) {
		pvh->pvh_attrs |= PMAP_ATTR_MOD;
		faultoff |= PG_FOW;
	}
	pmap_changebit(pa, faultoff, FALSE);
	if ((*pte & faultoff) != 0) {
#if 0
		/*
		 * This is apparently normal.  Why? -- cgd
		 * XXX because was being called on unmanaged pages?
		 */
		printf("warning: pmap_changebit didn't.");
#endif
		*pte &= ~faultoff;
		ALPHA_TBIS(v);
	}
}

#ifdef DEBUG
/*
 * pmap_pvdump:
 *
 *	Dump the physical->virtual data for the specified page.
 */
/* static */
void
pmap_pvdump(pa)
	vm_offset_t pa;
{
	struct pv_head *pvh;
	pv_entry_t pv;

	pvh = pa_to_pvh(pa);

	printf("pa %lx (attrs = 0x%x):\n", pa, pvh->pvh_attrs);
	for (pv = LIST_FIRST(&pvh->pvh_list); pv != NULL;
	     pv = LIST_NEXT(pv, pv_list))
		printf("     pmap %p, va %lx\n",
		    pv->pv_pmap, pv->pv_va);
	printf("\n");
}
#endif
 
/*
 * vtophys:
 *
 *	Return the physical address corresponding to the K0SEG or
 *	K1SEG address provided.
 */
vm_offset_t
vtophys(vaddr)
	vm_offset_t vaddr;
{
	vm_offset_t paddr;

	if (vaddr < ALPHA_K0SEG_BASE) {
		printf("vtophys: invalid vaddr 0x%lx", vaddr);
		paddr = vaddr;
	} else if (vaddr <= ALPHA_K0SEG_END)
		paddr = ALPHA_K0SEG_TO_PHYS(vaddr);
	else
		paddr = pmap_extract(pmap_kernel(), vaddr);

#if 0
	printf("vtophys(0x%lx) -> 0x%lx\n", vaddr, paddr);
#endif

	return (paddr);
}

/******************** pv_entry management ********************/

/*
 * pmap_enter_pv:
 *
 *	Add a physical->virtual entry to the pv_table.
 *	Must be called at splimp()!
 */
void
pmap_enter_pv(pmap, pa, va)
	pmap_t pmap;
	vm_offset_t pa, va;
{
	struct pv_head *pvh;
	pv_entry_t pv;

	pvh = pa_to_pvh(pa);

#ifdef DEBUG
	/*
	 * Make sure the entry doens't already exist.
	 */
	for (pv = LIST_FIRST(&pvh->pvh_list); pv != NULL;
	     pv = LIST_NEXT(pv, pv_list))
		if (pmap == pv->pv_pmap && va == pv->pv_va) {
			printf("pmap = %p, va = 0x%lx\n", pmap, va);
			panic("pmap_enter_pv: already in pv table");
		}
#endif

	/*
	 * Allocate and fill in the new pv_entry.
	 */
	pv = pmap_alloc_pv();
	pv->pv_va = va;
	pv->pv_pmap = pmap;

	/*
	 * ...and put it in the list.
	 */
#ifdef PMAPSTATS
	if (LIST_FIRST(&pvh->pvh_list) == NULL)
		enter_stats.firstpv++;
	else if (LIST_NEXT(LIST_FIRST(&pvh->pvh_list)) == NULL)
		enter_stats.secondpv++;
#endif
	LIST_INSERT_HEAD(&pvh->pvh_list, pv, pv_list);
}

/*
 * pmap_remove_pv:
 *
 *	Remove a physical->virtual entry from the pv_table.
 *	Must be called at splimp()!
 */
void
pmap_remove_pv(pmap, pa, va)
	pmap_t pmap;
	vm_offset_t pa, va;
{
	struct pv_head *pvh;
	pv_entry_t pv;

	pvh = pa_to_pvh(pa);

	/*
	 * Find the entry to remove.
	 */
	for (pv = LIST_FIRST(&pvh->pvh_list); pv != NULL;
	     pv = LIST_NEXT(pv, pv_list))
		if (pmap == pv->pv_pmap && va == pv->pv_va)
			break;

#ifdef DEBUG
	if (pv == NULL)
		panic("pmap_remove_pv: not in pv table");
#endif

	LIST_REMOVE(pv, pv_list);

	/*
	 * ...and free the pv_entry.
	 */
	pmap_free_pv(pv);
}

/*
 * pmap_alloc_pv:
 *
 *	Allocate a pv_entry.
 */
struct pv_entry *
pmap_alloc_pv()
{
	vm_offset_t pvppa;
	struct pv_page *pvp;
	struct pv_entry *pv;
	int i;

	if (pv_nfree == 0) {
		/*
		 * No free pv_entry's; allocate a new pv_page.
		 */
		pvppa = pmap_alloc_physpage();
		pvp = (struct pv_page *)ALPHA_PHYS_TO_K0SEG(pvppa);
		LIST_INIT(&pvp->pvp_pgi.pgi_freelist);
		for (i = 0; i < NPVPPG; i++)
			LIST_INSERT_HEAD(&pvp->pvp_pgi.pgi_freelist,
			    &pvp->pvp_pv[i], pv_list);
		pv_nfree += pvp->pvp_pgi.pgi_nfree = NPVPPG;
		TAILQ_INSERT_HEAD(&pv_page_freelist, pvp, pvp_pgi.pgi_list);
	}

	--pv_nfree;
	pvp = TAILQ_FIRST(&pv_page_freelist);
	if (--pvp->pvp_pgi.pgi_nfree == 0)
		TAILQ_REMOVE(&pv_page_freelist, pvp, pvp_pgi.pgi_list);
	pv = LIST_FIRST(&pvp->pvp_pgi.pgi_freelist);
#ifdef DIAGNOSTIC
	if (pv == NULL)
		panic("pmap_alloc_pv: pgi_nfree inconsistent");
#endif
	LIST_REMOVE(pv, pv_list);
	return (pv);
}

/*
 * pmap_free_pv:
 *
 *	Free a pv_entry.
 */
void
pmap_free_pv(pv)
	struct pv_entry *pv;
{
	vm_offset_t pvppa;
	struct pv_page *pvp;

	pvp = (struct pv_page *) trunc_page(pv);
	switch (++pvp->pvp_pgi.pgi_nfree) {
	case 1:
		TAILQ_INSERT_TAIL(&pv_page_freelist, pvp, pvp_pgi.pgi_list);
	default:
		LIST_INSERT_HEAD(&pvp->pvp_pgi.pgi_freelist, pv, pv_list);
		++pv_nfree;
		break;
	case NPVPPG:
		/*
		 * We've freed all of the pv_entry's in this pv_page.
		 * Free the pv_page back to the system.
		 */
		pv_nfree -= NPVPPG - 1;
		TAILQ_REMOVE(&pv_page_freelist, pvp, pvp_pgi.pgi_list);
		pvppa = ALPHA_K0SEG_TO_PHYS((vm_offset_t)pvp);
		pmap_free_physpage(pvppa);
		break;
	}
}

/*
 * pmap_collect_pv:
 *
 *	Compact the pv_table and release any completely free
 *	pv_page's back to the system.
 */
void
pmap_collect_pv()
{
	struct pv_page_list pv_page_collectlist;
	struct pv_page *pvp, *npvp;
	struct pv_head *pvh;
	struct pv_entry *pv, *nextpv, *newpv;
	vm_offset_t pvppa;
	int s;

	TAILQ_INIT(&pv_page_collectlist);

	/*
	 * Find pv_page's that have more than 1/3 of their pv_entry's free.
	 * Remove them from the list of available pv_page's and place them
	 * on the collection list.
	 */
	for (pvp = TAILQ_FIRST(&pv_page_freelist); pvp != NULL; pvp = npvp) {
		/*
		 * Abort if we can't allocate enough new pv_entry's
		 * to clean up a pv_page.
		 */
		if (pv_nfree < NPVPPG)
			break;
		npvp = TAILQ_NEXT(pvp, pvp_pgi.pgi_list);
		if (pvp->pvp_pgi.pgi_nfree > NPVPPG / 3) {
			TAILQ_REMOVE(&pv_page_freelist, pvp, pvp_pgi.pgi_list);
			TAILQ_INSERT_TAIL(&pv_page_collectlist, pvp,
			    pvp_pgi.pgi_list);
			pv_nfree -= pvp->pvp_pgi.pgi_nfree;
			pvp->pvp_pgi.pgi_nfree = -1;
		}
	}

	if (TAILQ_FIRST(&pv_page_collectlist) == NULL)
		return;

	/*
	 * Now, scan all pv_entry's, looking for ones which live in one
	 * of the condemed pv_page's.
	 */
	for (pvh = &pv_table[pv_table_npages - 1]; pvh >= &pv_table[0]; pvh--) {
		if (LIST_FIRST(&pvh->pvh_list) == NULL)
			continue;
		s = splimp();
		for (pv = LIST_FIRST(&pvh->pvh_list); pv != NULL;
		     pv = nextpv) {
			nextpv = LIST_NEXT(pv, pv_list);
			pvp = (struct pv_page *) trunc_page(pv);
			if (pvp->pvp_pgi.pgi_nfree == -1) {
				/*
				 * Found one!  Grab a new pv_entry from
				 * an eligible pv_page.
				 */
				pvp = TAILQ_FIRST(&pv_page_freelist);
				if (--pvp->pvp_pgi.pgi_nfree == 0) {
					TAILQ_REMOVE(&pv_page_freelist, pvp,
					    pvp_pgi.pgi_list);
				}
				newpv = LIST_FIRST(&pvp->pvp_pgi.pgi_freelist);
				LIST_REMOVE(newpv, pv_list);
#ifdef DIAGNOSTIC
				if (newpv == NULL)
					panic("pmap_collect_pv: pgi_nfree inconsistent");
#endif
				/*
				 * Remove the doomed pv_entry from its
				 * pv_list and copy its contents to
				 * the new entry.
				 */
				LIST_REMOVE(pv, pv_list);
				*newpv = *pv;

				/*
				 * Now insert the new pv_entry into
				 * the list.
				 */
				LIST_INSERT_HEAD(&pvh->pvh_list, newpv,
				    pv_list);
			}
		}
		splx(s);
	}

	/*
	 * Now free all of the pages we've collected.
	 */
	for (pvp = TAILQ_FIRST(&pv_page_collectlist); pvp != NULL; pvp = npvp) {
		npvp = TAILQ_NEXT(pvp, pvp_pgi.pgi_list);
		pvppa = ALPHA_K0SEG_TO_PHYS((vm_offset_t)pvp);
		pmap_free_physpage(pvppa);
	}
}

/******************** misc. functions ********************/

/*
 * pmap_alloc_physpage:
 *
 *	Allocate a single page from the VM system and return the
 *	physical address for that page.
 */
vm_offset_t
pmap_alloc_physpage()
{
	struct vm_page *pg;
	vm_offset_t pa;

#if defined(UVM)
	if ((pg = uvm_pagealloc(NULL, 0, NULL)) == NULL) {
#else
	if ((pg = vm_page_alloc1()) == NULL) {
#endif
		/*
		 * XXX This is lame.  We can probably wait for the
		 * XXX memory to become available, or steal a PT
		 * XXX page from another pmap.
		 */
		panic("pmap_alloc_physpage: no pages available");
	}

	pa = VM_PAGE_TO_PHYS(pg);
	pmap_zero_page(pa);
	return (pa);
}

/*
 * pmap_free_physpage:
 *
 *	Free the single page table page at the specified physical address.
 */
void
pmap_free_physpage(ptpage)
	vm_offset_t ptpage;
{
	struct vm_page *pg;

	if ((pg = PHYS_TO_VM_PAGE(ptpage)) == NULL)
		panic("pmap_free_physpage: bogus physical page address");

#if defined(UVM)
	uvm_pagefree(pg);
#else
	vm_page_free1(pg);
#endif
}

/******************** page table page management ********************/

#if defined(PMAP_NEW)

	/* Implement pmap_growkernel() */

#endif /* PMAP_NEW */

/*
 * pmap_create_lev1map:
 *
 *	Create a new level 1 page table for the specified pmap.
 */
void
pmap_create_lev1map(pmap)
	pmap_t pmap;
{
	struct pv_head *pvh;
	vm_offset_t ptpa;
	pt_entry_t pte;
	int i;

#ifdef DIAGNOSTIC
	if (pmap == pmap_kernel())
		panic("pmap_create_lev1map: got kernel pmap");
#endif

	/*
	 * Allocate a page for the level 1 table.
	 */
	ptpa = pmap_alloc_physpage();
	pmap->pm_lev1map = (pt_entry_t *) ALPHA_PHYS_TO_K0SEG(ptpa);

	/*
	 * Initialize PT page bookkeeping.
	 */
	pvh = pa_to_pvh(ptpa);
	pvh->pvh_attrs |= PMAP_ATTR_PTPAGE;
	pvh->pvh_ptref = 0;

	/*
	 * Initialize the new level 1 table by copying the
	 * kernel mappings into it.
	 */
	for (i = l1pte_index(VM_MIN_KERNEL_ADDRESS);
	     i <= l1pte_index(VM_MAX_KERNEL_ADDRESS); i++)
		pmap->pm_lev1map[i] = Lev1map[i];

	/*
	 * Now, map the new virtual page table.  NOTE: NO ASM!
	 */
	pte = ((ptpa >> PGSHIFT) << PG_SHIFT) | PG_V | PG_KRE | PG_KWE;
	pmap->pm_lev1map[l1pte_index(VPTBASE)] = pte;

	/*
	 * The page table base has changed; if the pmap was active,
	 * reactivate it.
	 */
	if (active_user_pmap(pmap))
		PMAP_ACTIVATE(pmap, curproc);
}

/*
 * pmap_destroy_lev1map:
 *
 *	Destroy the level 1 page table for the specified pmap.
 */
void
pmap_destroy_lev1map(pmap)
	pmap_t pmap;
{
	struct pv_head *pvh;
	vm_offset_t ptpa;

#ifdef DIAGNOSTIC
	if (pmap == pmap_kernel())
		panic("pmap_destroy_lev1map: got kernel pmap");
#endif

	ptpa = ALPHA_K0SEG_TO_PHYS((vm_offset_t)pmap->pm_lev1map);

	/*
	 * Go back to referencing the global Lev1map.
	 */
	pmap->pm_lev1map = Lev1map;

	/*
	 * The page table base has changed; if the pmap was active,
	 * reactivate it.
	 */
	if (active_user_pmap(pmap))
		PMAP_ACTIVATE(pmap, curproc);

	/*
	 * Clear PT page bookkeeping.
	 */
	pvh = pa_to_pvh(ptpa);
	pvh->pvh_attrs &= ~PMAP_ATTR_PTPAGE;

	/*
	 * Free the old level 1 page table page.
	 */
	pmap_free_physpage(ptpa);
}

/*
 * pmap_alloc_ptpage:
 *
 *	Allocate a level 2 or level 3 page table page, and
 *	initialize the PTE that references it.
 */
void
pmap_alloc_ptpage(pmap, pte)
	pmap_t pmap;
	pt_entry_t *pte;
{
	struct pv_head *pvh;
	vm_offset_t ptpa;

	/*
	 * Allocate the page table page.
	 */
	ptpa = pmap_alloc_physpage();

	/*
	 * Initialize PT page bookkeeping.
	 */
	pvh = pa_to_pvh(ptpa);
	pvh->pvh_attrs |= PMAP_ATTR_PTPAGE;
	pvh->pvh_ptref = 0;

	/*
	 * Initialize the referencing PTE.
	 */
	*pte = ((ptpa >> PGSHIFT) << PG_SHIFT) | \
	    PG_V | PG_KRE | PG_KWE | PG_WIRED |
	    (pmap == pmap_kernel() ? PG_ASM : 0);
}

/*
 * pmap_free_ptpage:
 *
 *	Free the level 2 or level 3 page table page referenced
 *	be the provided PTE.
 */
void
pmap_free_ptpage(pmap, pte)
	pmap_t pmap;
	pt_entry_t *pte;
{
	struct pv_head *pvh;
	vm_offset_t ptpa;

	/*
	 * Extract the physical address of the page from the PTE
	 * and clear the entry.
	 */
	ptpa = pmap_pte_pa(pte);
	*pte = PG_NV;

#ifdef DEBUG
	pmap_zero_page(ptpa);
#endif

	/*
	 * Clear PT page bookkeeping.
	 */
	pvh = pa_to_pvh(ptpa);
	pvh->pvh_attrs &= ~PMAP_ATTR_PTPAGE;

	/*
	 * Free the page table page.
	 */
	pmap_free_physpage(ptpa);
}

/*
 * pmap_ptpage_addref:
 *
 *	Add a reference to the page table page in which the
 *	specified PTE lies.
 */
int
pmap_ptpage_addref(pte)
	pt_entry_t *pte;
{
	struct pv_head *pvh;
	vm_offset_t ptpa;

	ptpa = ALPHA_K0SEG_TO_PHYS(trunc_page(pte));
	pvh = pa_to_pvh(ptpa);

#ifdef DIAGNOSTIC
	if ((pvh->pvh_attrs & PMAP_ATTR_PTPAGE) == 0)
		panic("pmap_ptpage_addref: not a PT page");
#endif

	pvh->pvh_ptref++;

#ifdef DIAGNOSTIC
	/*
	 * Reference count may be as many as NPTEPG + 1, because
	 * pmap_enter() avoids an incorrect 0 reference count
	 * when calling pmap_remove_mapping().
	 */
	if (pvh->pvh_ptref > (NPTEPG + 1))
		panic("pmap_ptpage_addref: too many references");
#endif

	return (pvh->pvh_ptref);
}

/*
 * pmap_ptpage_delref:
 *
 *	Delete a reference to the page table page in which the
 *	specified PTE lies.
 */
int
pmap_ptpage_delref(pte)
	pt_entry_t *pte;
{
	struct pv_head *pvh;
	vm_offset_t ptpa;

	ptpa = ALPHA_K0SEG_TO_PHYS(trunc_page(pte));
	pvh = pa_to_pvh(ptpa);

#ifdef DIAGNOSTIC
	if ((pvh->pvh_attrs & PMAP_ATTR_PTPAGE) == 0)
		panic("pmap_ptpage_delref: not a PT page");
#endif

	pvh->pvh_ptref--;

#ifdef DIAGNOSTIC
	/*
	 * Make sure we never have a negative reference count.
	 */
	if (pvh->pvh_ptref < 0)
		panic("pmap_ptpage_delref: negative reference count");
#endif

	return (pvh->pvh_ptref);
}

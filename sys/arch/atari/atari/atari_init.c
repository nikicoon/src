/*	$NetBSD: atari_init.c,v 1.24 1997/01/04 00:06:21 leo Exp $	*/

/*
 * Copyright (c) 1995 Leo Weppelman
 * Copyright (c) 1994 Michael L. Hitch
 * Copyright (c) 1993 Markus Wild
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
 *      This product includes software developed by Markus Wild.
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
#include <sys/systm.h>
#include <sys/proc.h>
#include <vm/vm.h>
#include <sys/user.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/tty.h>
#include <sys/proc.h>
#include <sys/buf.h>
#include <sys/msgbuf.h>
#include <sys/mbuf.h>
#include <sys/protosw.h>
#include <sys/domain.h>
#include <sys/dkbad.h>
#include <sys/reboot.h>
#include <sys/exec.h>
#include <sys/core.h>
#include <sys/kcore.h>
#include <vm/pmap.h>
#include <machine/vmparam.h>
#include <machine/pte.h>
#include <machine/cpu.h>
#include <machine/iomap.h>
#include <machine/mfp.h>
#include <machine/scu.h>
#include <machine/kcore.h>
#include <atari/atari/intr.h>
#include <atari/atari/stalloc.h>
#include <atari/dev/ym2149reg.h>

void start_c __P((int, u_int, u_int, u_int, char *));
static void cpu_init_kcorehdr __P((u_long));
static void mmu030_setup __P((st_entry_t *, u_int, pt_entry_t *, u_int,
			      pt_entry_t *, u_int, u_int));
static void map_io_areas __P((pt_entry_t *, u_int, u_int));
static void set_machtype __P((void));

#if defined(M68040) || defined(M68060)
static void mmu040_setup __P((st_entry_t *, u_int, pt_entry_t *, u_int,
			      pt_entry_t *, u_int, u_int));
#endif

/*
 * All info needed to generate a panic dump. All fields are setup by
 * start_c().
 * XXX: Should sheck usage of phys_segs. There is some unwanted overlap
 *      here.... Also, the name is badly choosen. Phys_segs contains the
 *      segment descriptions _after_ reservations are made.
 * XXX: 'lowram' is obsoleted by the new panicdump format
 */
static cpu_kcore_hdr_t cpu_kcore_hdr;

extern u_int 	lowram;
extern u_int	Sysptsize, Sysseg_pa, proc0paddr;
extern pt_entry_t *Sysptmap;
extern st_entry_t *Sysseg;
u_int		*Sysmap;
int		machineid, mmutype, cputype, astpending;
char		*vmmap;
pv_entry_t	pv_table;
#if defined(M68040) || defined(M68060)
extern int	protostfree;
#endif

extern char		*esym;
extern struct pcb	*curpcb;

/*
 * This is the virtual address of physical page 0. Used by 'do_boot()'.
 */
vm_offset_t	page_zero;

/*
 * Crude support for allocation in ST-ram. Currently only used to allocate
 * video ram.
 * The physical address is also returned because the video init needs it to
 * setup the controller at the time the vm-system is not yet operational so
 * 'kvtop()' cannot be used.
 */
#ifndef ST_POOL_SIZE
#define	ST_POOL_SIZE	40			/* XXX: enough? */
#endif

u_long	st_pool_size = ST_POOL_SIZE * NBPG;	/* Patchable	*/
u_long	st_pool_virt, st_pool_phys;

/*
 * this is the C-level entry function, it's called from locore.s.
 * Preconditions:
 *	Interrupts are disabled
 *	PA == VA, we don't have to relocate addresses before enabling
 *		the MMU
 * 	Exec is no longer available (because we're loaded all over 
 *		low memory, no ExecBase is available anymore)
 *
 * It's purpose is:
 *	Do the things that are done in locore.s in the hp300 version, 
 *		this includes allocation of kernel maps and enabling the MMU.
 * 
 * Some of the code in here is `stolen' from Amiga MACH, and was 
 * written by Bryan Ford and Niklas Hallqvist.
 * 
 * Very crude 68040 support by Michael L. Hitch.
 */

void
start_c(id, ttphystart, ttphysize, stphysize, esym_addr)
int	id;			/* Machine id				*/
u_int	ttphystart, ttphysize;	/* Start address and size of TT-ram	*/
u_int	stphysize;		/* Size of ST-ram	 		*/
char	*esym_addr;		/* Address of kernel '_esym' symbol	*/
{
	extern char	end[];
	extern void	etext __P((void));
	extern u_long	protorp[2];
	u_int		pstart;		/* Next available physical address*/
	u_int		vstart;		/* Next available virtual address */
	u_int		avail;
	pt_entry_t	*pt;
	u_int		ptsize, ptextra;
	u_int		tc, i;
	u_int		*pg;
	u_int		pg_proto;
	u_int		end_loaded;
	u_long		kbase;
	u_int		kstsize;

	boot_segs[0].start       = 0;
	boot_segs[0].end         = stphysize;
	boot_segs[1].start       = ttphystart;
	boot_segs[1].end         = ttphystart + ttphysize;
	boot_segs[2].start = boot_segs[2].end = 0; /* End of segments! */

	/*
	 * The following is a hack. We do not know how much ST memory we
	 * really need until after configuration has finished. At this
	 * time I have no idea how to grab ST memory at that time.
	 * The round_page() call is ment to correct errors made by
	 * binpatching!
	 */
	st_pool_size   = atari_round_page(st_pool_size);
	st_pool_phys   = stphysize - st_pool_size;
	stphysize      = st_pool_phys;

	machineid      = id;
	esym           = esym_addr;

	/* 
	 * the kernel ends at end() or esym.
	 */
	if(esym == NULL)
		end_loaded = (u_int)end;
	else end_loaded = (u_int)esym;

	/*
	 * If we have enough fast-memory to put the kernel in, do it!
	 */
	if(ttphysize >= end_loaded)
		kbase = ttphystart;
	else kbase = 0;

	/*
	 * update these as soon as possible!
	 */
	PAGE_SIZE  = NBPG;
	PAGE_MASK  = NBPG-1;
	PAGE_SHIFT = PG_SHIFT;

	/*
	 * Determine the type of machine we are running on. This needs
	 * to be done early!
	 */
	set_machtype();

	/*
	 * We run the kernel from ST memory at the moment.
	 * The kernel segment table is put just behind the loaded image.
	 * pstart: start of usable ST memory
	 * avail : size of ST memory available.
	 */
	pstart = (u_int)end_loaded;
	pstart = atari_round_page(pstart);
	avail  = stphysize - pstart;
  
	/*
	 * Calculate the number of pages needed for Sysseg.
	 * For the 68030, we need 256 descriptors (segment-table-entries).
	 * This easily fits into one page.
	 * For the 68040, both the level-1 and level-2 descriptors are
	 * stored into Sysseg. We currently handle a maximum sum of MAXKL2SIZE
	 * level-1 & level-2 tables.
	 */
#if defined(M68040) || defined(M68060)
	if (mmutype == MMU_68040)
		kstsize = MAXKL2SIZE / (NPTEPG/SG4_LEV2SIZE);
	else
#endif
		kstsize = 1;
	/*
	 * allocate the kernel segment table
	 */
	Sysseg     = (st_entry_t *)pstart;
	Sysseg_pa  = (u_int)Sysseg + kbase;
	pstart    += kstsize * NBPG;
	avail     -= kstsize * NBPG;
  
	/*
	 * Determine the number of pte's we need for extra's like
	 * ST I/O map's.
	 */
	ptextra = btoc(STIO_SIZE);

	/*
	 * If present, add pci areas
	 */
	if (machineid & ATARI_HADES)
		ptextra += btoc(PCI_CONF_SIZE + PCI_IO_SIZE + PCI_MEM_SIZE);

	/*
	 * The 'pt' (the initial kernel pagetable) has to map the kernel and
	 * the I/O areas. The various I/O areas are mapped (virtually) at
	 * the top of the address space mapped by 'pt' (ie. just below Sysmap).
	 */
	pt      = (pt_entry_t *)pstart;
	ptsize  = (Sysptsize + howmany(ptextra, NPTEPG)) << PGSHIFT;
	pstart += ptsize;
	avail  -= ptsize;
  
	/*
	 * allocate kernel page table map
	 */
	Sysptmap = (pt_entry_t *)pstart;
	pstart  += NBPG;
	avail   -= NBPG;

	/*
	 * Set Sysmap; mapped after page table pages. Because I too (LWP)
	 * didn't understand the reason for this, I borrowed the following
	 * (sligthly modified) comment from mac68k/locore.s:
	 * LAK:  There seems to be some confusion here about the next line,
	 * so I'll explain.  The kernel needs some way of dynamically modifying
	 * the page tables for its own virtual memory.  What it does is that it
	 * has a page table map.  This page table map is mapped right after the
	 * kernel itself (in our implementation; in HP's it was after the I/O
	 * space). Therefore, the first three (or so) entries in the segment
	 * table point to the first three pages of the page tables (which
	 * point to the kernel) and the next entry in the segment table points
	 * to the page table map (this is done later).  Therefore, the value
	 * of the pointer "Sysmap" will be something like 16M*3 = 48M.  When
	 * the kernel addresses this pointer (e.g., Sysmap[0]), it will get
	 * the first longword of the first page map (== pt[0]).  Since the
	 * page map mirrors the segment table, addressing any index of Sysmap
	 * will give you a PTE of the page maps which map the kernel.
	 */
	Sysmap = (u_int *)(ptsize << (SEGSHIFT - PGSHIFT));

	/*
	 * Initialize segment tables
	 */
#if defined(M68040) || defined(M68060)
	if (mmutype == MMU_68040)
		mmu040_setup(Sysseg, kstsize, pt, ptsize, Sysptmap, 1, kbase);
	else
#endif /* defined(M68040) || defined(M68060) */
		mmu030_setup(Sysseg, kstsize, pt, ptsize, Sysptmap, 1, kbase);

	/*
	 * initialize kernel page table page(s).
	 * Assume load at VA 0.
	 * - Text pages are RO
	 * - Page zero is invalid
	 */
	pg_proto = (0 + kbase) | PG_RO | PG_V;
	pg       = pt;
	*pg++ = PG_NV; pg_proto += NBPG;
	for(i = NBPG; i < (u_int)etext; i += NBPG, pg_proto += NBPG)
		*pg++ = pg_proto;

	/* 
	 * data, bss and dynamic tables are read/write
	 */
	pg_proto = (pg_proto & PG_FRAME) | PG_RW | PG_V;

#if defined(M68040) || defined(M68060)
	/*
	 * Map the kernel segment table cache invalidated for 
	 * these machines (for the 68040 not strictly necessary, but
	 * recommended by Motorola; for the 68060 mandatory)
	 */
	if (mmutype == MMU_68040) {
	    for (; i < (u_int)Sysseg; i += NBPG, pg_proto += NBPG)
		*pg++ = pg_proto;
	    pg_proto = (pg_proto & ~PG_CCB) | PG_CI;
	    for (; i < (u_int)&Sysseg[kstsize * NPTEPG]; i += NBPG,
							 pg_proto += NBPG)
		*pg++ = pg_proto;
	    pg_proto = (pg_proto & ~PG_CI) | PG_CCB;
	}
#endif /* defined(M68040) || defined(M68060) */

	/*
	 * go till end of data allocated so far
	 * plus proc0 u-area (to be allocated)
	 */
	for(; i < pstart + USPACE; i += NBPG, pg_proto += NBPG)
		*pg++ = pg_proto;

	/*
	 * invalidate remainder of kernel PT
	 */
	while(pg < &pt[ptsize/sizeof(pt_entry_t)])
		*pg++ = PG_NV;

	/*
	 * Map various I/O areas
	 */
	map_io_areas(pt, ptsize, ptextra);

	/*
	 * Save KVA of proc0 user-area and allocate it
	 */
	proc0paddr = pstart;
	pstart    += USPACE;
	avail     -= USPACE;

	/*
	 * At this point, virtual and physical allocation starts to divert.
	 */
	vstart     = pstart;

	/*
	 * Map the allocated space in ST-ram now. In the contig-case, there
	 * is no need to make a distinction between virtual and physical
	 * adresses. But I make it anyway to be prepared.
	 * Physcal space is already reserved!
	 */
	st_pool_virt = vstart;
	pg           = &pt[vstart / NBPG];
	pg_proto     = st_pool_phys | PG_RW | PG_CI | PG_V;
	vstart      += st_pool_size;
	while(pg_proto < (st_pool_phys + st_pool_size)) {
		*pg++     = pg_proto;
		pg_proto += NBPG;
	}

	/*
	 * Map physical page_zero and page-zero+1 (First ST-ram page). We need
	 * to reference it in the reboot code. Two pages are mapped, because
	 * we must make sure 'doboot()' is contained in it (see the tricky
	 * copying there....).
	 */
	page_zero  = vstart;
	pg         = &pt[vstart / NBPG];
	*pg++      = PG_RW | PG_CI | PG_V;
	vstart    += NBPG;
	*pg        = PG_RW | PG_CI | PG_V | NBPG;
	vstart    += NBPG;

	lowram  = 0 >> PGSHIFT; /* XXX */

	/*
	 * Fill in usable segments. The page indexes will be initialized
	 * later when all reservations are made.
	 */
	usable_segs[0].start = 0;
	usable_segs[0].end   = stphysize;
	usable_segs[1].start = ttphystart;
	usable_segs[1].end   = ttphystart + ttphysize;
	usable_segs[2].start = usable_segs[2].end = 0; /* End of segments! */

	if(kbase) {
		/*
		 * First page of ST-ram is unusable, reserve the space
		 * for the kernel in the TT-ram segment.
		 * Note: Because physical page-zero is partially mapped to ROM
		 *       by hardware, it is unusable.
		 */
		usable_segs[0].start  = NBPG;
		usable_segs[1].start += pstart;
	}
	else usable_segs[0].start += pstart;

	/*
	 * As all segment sizes are now valid, calculate page indexes and
	 * available physical memory.
	 */
	usable_segs[0].first_page = 0;
	for (i = 1; usable_segs[i].start; i++) {
		usable_segs[i].first_page  = usable_segs[i-1].first_page;
		usable_segs[i].first_page +=
			(usable_segs[i-1].end - usable_segs[i-1].start) / NBPG;
	}
	for (i = 0, physmem = 0; usable_segs[i].start; i++)
		physmem += usable_segs[i].end - usable_segs[i].start;
	physmem >>= PGSHIFT;
  
	/*
	 * get the pmap module in sync with reality.
	 */
	pmap_bootstrap(vstart, stio_addr, ptextra);

	/*
	 * Prepare to enable the MMU.
	 * Setup and load SRP nolimit, share global, 4 byte PTE's
	 */
	protorp[0] = 0x80000202;
	protorp[1] = (u_int)Sysseg + kbase;	/* + segtable address */
	Sysseg_pa  = (u_int)Sysseg + kbase;

	cpu_init_kcorehdr(kbase);

	/*
	 * copy over the kernel (and all now initialized variables) 
	 * to fastram.  DONT use bcopy(), this beast is much larger 
	 * than 128k !
	 */
	if(kbase) {
		register u_long	*lp, *le, *fp;

		lp = (u_long *)0;
		le = (u_long *)pstart;
		fp = (u_long *)kbase;
		while(lp < le)
			*fp++ = *lp++;
	}
#if defined(M68040) || defined(M68060)
	if (mmutype == MMU_68040) {
		/*
		 * movel Sysseg_pa,a0;
		 * movec a0,SRP;
		 * pflusha;
		 * movel #$0xc000,d0;
		 * movec d0,TC
		 */
		asm volatile ("movel %0,a0;.word 0x4e7b,0x8807" : : "a" (Sysseg_pa) : "a0");
		asm volatile (".word 0xf518" : : );
		asm volatile ("movel #0xc000,d0; .word 0x4e7b,0x0003" : : :"d0" );
	} else
#endif
	{
		asm volatile ("pmove %0@,srp" : : "a" (&protorp[0]));
		/*
		 * setup and load TC register.
		 * enable_cpr, enable_srp, pagesize=8k,
		 * A = 8 bits, B = 11 bits
		 */
		tc = 0x82d08b00;
		asm volatile ("pmove %0@,tc" : : "a" (&tc));
	}
 
	/* Is this to fool the optimizer?? */
	i = *(int *)proc0paddr;
	*(volatile int *)proc0paddr = i;

	/*
	 * Initialize the "u-area" pages.
	 * Must initialize p_addr before autoconfig or the
	 * fault handler will get a NULL reference.
	 */
	bzero((u_char *)proc0paddr, USPACE);
	proc0.p_addr = (struct user *)proc0paddr;
	curproc = &proc0;
	curpcb  = &((struct user *)proc0paddr)->u_pcb;

	ym2149_init();

	/*
	 * Initialize both MFP chips (if both present!) to generate
	 * auto-vectored interrupts with EOI. The active-edge registers are
	 * set up. The interrupt enable registers are set to disable all
	 * interrupts.
	 * A test on presence on the second MFP determines if this is a
	 * TT030 or a Falcon. This is added to 'machineid'.
	 */
	MFP->mf_iera  = MFP->mf_ierb = 0;
	MFP->mf_imra  = MFP->mf_imrb = 0;
	MFP->mf_aer   = MFP->mf_ddr  = 0;
	MFP->mf_vr    = 0x40;
	if(machineid & (ATARI_TT|ATARI_HADES)) {
		MFP2->mf_iera = MFP2->mf_ierb = 0;
		MFP2->mf_imra = MFP2->mf_imrb = 0;
		MFP2->mf_aer  = 0x80;
		MFP2->mf_vr   = 0x50;
	}
	if(machineid & ATARI_TT) {
		/*
		 * Initialize the SCU, to enable interrupts on the SCC (ipl5),
		 * MFP (ipl6) and softints (ipl1).
		 */
		SCU->sys_mask = SCU_MFP | SCU_SCC | SCU_SYS_SOFT;
#ifdef DDB
		/*
		 * This allows people with the correct hardware modification
		 * to drop into the debugger from an NMI.
		 */
		SCU->sys_mask |= SCU_IRQ7;
#endif
		
	}

	/*
	 * Initialize stmem allocator
	 */
	init_stmem();

	/*
	 * Initialize interrupt mapping.
	 */
	intr_init();
}

/*
 * Try to figure out on what type of machine we are running
 * Note: This module runs *before* 
 */
static void
set_machtype()
{
	if(!badbaddr((caddr_t)(PCI_CONFB_PHYS + PCI_CONFM_PHYS)))
		machineid |= ATARI_HADES;
	else {
		if(!badbaddr((caddr_t)&MFP2->mf_gpip))
			machineid |= ATARI_TT;
		else machineid |= ATARI_FALCON;
	}
}

/*
 * Do the dull work of mapping the various I/O areas. They MUST be Cache
 * inhibited!
 * All I/O areas are virtually mapped at the end of the pt-table.
 */
static void
map_io_areas(pt, ptsize, ptextra)
pt_entry_t	*pt;
u_int		ptsize;		/* Size of 'pt' in bytes	*/
u_int		ptextra;	/* #of additional I/O pte's	*/
{
	vm_offset_t	ioaddr;
	pt_entry_t	*pg, *epg;
	pt_entry_t	pg_proto;
	u_long		mask;

	ioaddr = ((ptsize / sizeof(pt_entry_t)) - ptextra) * NBPG;

	/*
	 * Map ST-IO area
	 */
	stio_addr = ioaddr;
	ioaddr   += STIO_SIZE;
	pg        = &pt[stio_addr / NBPG];
	epg       = &pg[btoc(STIO_SIZE)];
	pg_proto  = STIO_PHYS | PG_RW | PG_CI | PG_V;
	while(pg < epg) {
		*pg++     = pg_proto;
		pg_proto += NBPG;
	}

	/*
	 * Map PCI areas
	 */
	if (machineid & ATARI_HADES) {

		pci_conf_addr = ioaddr;
		ioaddr       += PCI_CONF_SIZE;
		pg            = &pt[pci_conf_addr / NBPG];
		epg           = &pg[btoc(PCI_CONF_SIZE)];
		mask          = PCI_CONFM_PHYS;
		pg_proto      = PCI_CONFB_PHYS | PG_RW | PG_CI | PG_V;
		for(; pg < epg; mask >>= 1)
			*pg++ = pg_proto | mask;

		pci_io_addr   = ioaddr;
		ioaddr       += PCI_IO_SIZE;
		epg           = &pg[btoc(PCI_IO_SIZE)];
		pg_proto      = PCI_IO_PHYS | PG_RW | PG_CI | PG_V;
		while(pg < epg) {
			*pg++     = pg_proto;
			pg_proto += NBPG;
		}

		pci_mem_addr  = ioaddr;
		ioaddr       += PCI_MEM_SIZE;
		epg           = &pg[btoc(PCI_MEM_SIZE)];
		pg_proto      = PCI_MEM_PHYS | PG_RW | PG_CI | PG_V;
		while(pg < epg) {
			*pg++     = pg_proto;
			pg_proto += NBPG;
		}
	}
}

/*
 * Used by dumpconf() to get the size of the machine-dependent panic-dump
 * header in disk blocks.
 */
int
cpu_dumpsize()
{
	int	size;

	size = ALIGN(sizeof(kcore_seg_t)) + ALIGN(sizeof(cpu_kcore_hdr_t));
	return (btodb(roundup(size, dbtob(1))));
}

/*
 * Called by dumpsys() to dump the machine-dependent header.
 * XXX: Assumes that it will all fit in one diskblock.
 */
int
cpu_dump(dump, p_blkno)
int	(*dump) __P((dev_t, daddr_t, caddr_t, size_t));
daddr_t	*p_blkno;
{
	int		buf[dbtob(1)/sizeof(int)];
	int		error;
	kcore_seg_t	*kseg_p;
	cpu_kcore_hdr_t	*chdr_p;

	kseg_p = (kcore_seg_t *)buf;
	chdr_p = (cpu_kcore_hdr_t *)&buf[ALIGN(sizeof(*kseg_p)) / sizeof(int)];

	/*
	 * Generate a segment header
	 */
	CORE_SETMAGIC(*kseg_p, KCORE_MAGIC, MID_MACHINE, CORE_CPU);
	kseg_p->c_size = dbtob(1) - ALIGN(sizeof(*kseg_p));

	/*
	 * Add the md header
	 */
	*chdr_p = cpu_kcore_hdr;
	error = dump(dumpdev, *p_blkno, (caddr_t)buf, dbtob(1));
	*p_blkno += 1;
	return (error);
}

#if (NPHYS_RAM_SEGS < NMEM_SEGS)
#error "Configuration error: NPHYS_RAM_SEGS < NMEM_SEGS"
#endif
/*
 * Initialize the cpu_kcore_header.
 */
static void
cpu_init_kcorehdr(kbase)
u_long	kbase;
{
	int	i;

	for (i = 0; i < NMEM_SEGS; i++) {
		cpu_kcore_hdr.ram_segs[i].start = boot_segs[i].start;
		cpu_kcore_hdr.ram_segs[i].size  = boot_segs[i].end
							- boot_segs[i].start;
	}
	cpu_kcore_hdr.mmutype   = mmutype;
	cpu_kcore_hdr.kernel_pa = kbase;
	cpu_kcore_hdr.sysseg_pa = (st_entry_t *)((u_int)Sysseg + kbase);
}

void
mmu030_setup(sysseg, kstsize, pt, ptsize, sysptmap, sysptsize, kbase)
	st_entry_t	*sysseg;	/* System segment table		*/
	u_int		kstsize;	/* size of 'sysseg' in pages	*/
	pt_entry_t	*pt;		/* Kernel page table		*/
	u_int		ptsize;		/* size	of 'pt' in bytes	*/
	pt_entry_t	*sysptmap;	/* System page table		*/
	u_int		sysptsize;	/* size of 'sysptmap' in pages	*/
	u_int		kbase;
{
	st_entry_t	sg_proto, *sg;
	pt_entry_t	pg_proto, *pg, *epg;

	sg_proto = ((u_int)pt + kbase) | SG_RW | SG_V;
	pg_proto = ((u_int)pt + kbase) | PG_RW | PG_CI | PG_V;

	/*
	 * Map the page table pages in both the HW segment table
	 * and the software Sysptmap.  Note that Sysptmap is also
	 * considered a PT page, hence the +sysptsize.
	 */
	sg  = sysseg;
	pg  = sysptmap; 
	epg = &pg[(ptsize >> PGSHIFT) + sysptsize];
	while(pg < epg) {
		*sg++ = sg_proto;
		*pg++ = pg_proto;
		sg_proto += NBPG;
		pg_proto += NBPG;
	}

	/* 
	 * invalidate the remainder of the tables
	 */
	epg = &sysptmap[sysptsize * NPTEPG];
	while(pg < epg) {
		*sg++ = SG_NV;
		*pg++ = PG_NV;
	}
}

#if defined(M68040) || defined(M68060)
void
mmu040_setup(sysseg, kstsize, pt, ptsize, sysptmap, sysptsize, kbase)
	st_entry_t	*sysseg;	/* System segment table		*/
	u_int		kstsize;	/* size of 'sysseg' in pages	*/
	pt_entry_t	*pt;		/* Kernel page table		*/
	u_int		ptsize;		/* size	of 'pt' in bytes	*/
	pt_entry_t	*sysptmap;	/* System page table		*/
	u_int		sysptsize;	/* size of 'sysptmap' in pages	*/
	u_int		kbase;
{
	int		i;
	st_entry_t	sg_proto, *sg, *esg;
	pt_entry_t	pg_proto;

	/*
	 * First invalidate the entire "segment table" pages
	 * (levels 1 and 2 have the same "invalid" values).
	 */
	sg  = sysseg;
	esg = &sg[kstsize * NPTEPG];
	while (sg < esg)
		*sg++ = SG_NV;

	/*
	 * Initialize level 2 descriptors (which immediately
	 * follow the level 1 table). These should map 'pt' + 'sysptmap'.
	 * We need:
	 *	NPTEPG / SG4_LEV3SIZE
	 * level 2 descriptors to map each of the nptpages + 1
	 * pages of PTEs.  Note that we set the "used" bit
	 * now to save the HW the expense of doing it.
	 */
	i   = ((ptsize >> PGSHIFT) + sysptsize) * (NPTEPG / SG4_LEV3SIZE);
	sg  = &sysseg[SG4_LEV1SIZE];
	esg = &sg[i];
	sg_proto = ((u_int)pt + kbase) | SG_U | SG_RW | SG_V;
	while (sg < esg) {
		*sg++     = sg_proto;
		sg_proto += (SG4_LEV3SIZE * sizeof (st_entry_t));
	}

	/*
	 * Initialize level 1 descriptors.  We need:
	 *	roundup(num, SG4_LEV2SIZE) / SG4_LEVEL2SIZE
	 * level 1 descriptors to map the 'num' level 2's.
	 */
	i = roundup(i, SG4_LEV2SIZE) / SG4_LEV2SIZE;
	protostfree = (-1 << (i + 1)) /* & ~(-1 << MAXKL2SIZE) */;
	sg  = sysseg;
	esg = &sg[i];
	sg_proto = ((u_int)&sg[SG4_LEV1SIZE] + kbase) | SG_U | SG_RW |SG_V;
	while (sg < esg) {
		*sg++     = sg_proto;
		sg_proto += (SG4_LEV2SIZE * sizeof(st_entry_t));
	}

	/*
	 * Initialize sysptmap
	 */
	sg  = sysptmap;
	esg = &sg[(ptsize >> PGSHIFT) + sysptsize];
	pg_proto = ((u_int)pt + kbase) | PG_RW | PG_CI | PG_V;
	while (sg < esg) {
		*sg++     = pg_proto;
		pg_proto += NBPG;
	}
	/*
	 * Invalidate rest of Sysptmap page
	 */
	esg = &sysptmap[sysptsize * NPTEPG];
	while (sg < esg)
		*sg++ = SG_NV;
}
#endif /* M68040 */

#ifdef DEBUG
void
dump_segtable(stp)
	u_int *stp;
{
	u_int *s, *es;
	int shift, i;

	s = stp;
	{
		es = s + (ATARI_STSIZE >> 2);
		shift = SG_ISHIFT;
	}

	/* 
	 * XXX need changes for 68040 
	 */
	for (i = 0; s < es; s++, i++)
		if (*s & SG_V)
			printf("$%08lx: $%08lx\t", i << shift, *s & SG_FRAME);
	printf("\n");
}

void
dump_pagetable(ptp, i, n)
	u_int *ptp, i, n;
{
	u_int *p, *ep;

	p = ptp + i;
	ep = p + n;
	for (; p < ep; p++, i++)
		if (*p & PG_V)
			printf("$%08lx -> $%08lx\t", i, *p & PG_FRAME);
	printf("\n");
}

u_int
vmtophys(ste, vm)
	u_int *ste, vm;
{
	ste = (u_int *) (*(ste + (vm >> SEGSHIFT)) & SG_FRAME);
		ste += (vm & SG_PMASK) >> PGSHIFT;
	return((*ste & -NBPG) | (vm & (NBPG - 1)));
}

#endif

/*	$NetBSD: machdep.c,v 1.39 2002/03/17 17:55:23 uch Exp $	*/

/*-
 * Copyright (c) 1996, 1997, 1998 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Charles M. Hannum and by Jason R. Thorpe of the Numerical Aerospace
 * Simulation Facility, NASA Ames Research Center.
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

/*-
 * Copyright (c) 1982, 1987, 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz.
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
 *	@(#)machdep.c	7.4 (Berkeley) 6/3/91
 */

#include "opt_ddb.h"
#include "opt_kgdb.h"
#include "opt_syscall_debug.h"
#include "opt_memsize.h"
#include "opt_initbsc.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/user.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <sys/sysctl.h>
#include <sys/msgbuf.h>

#include <uvm/uvm_extern.h>

#include <dev/cons.h>
#include <machine/bus.h>
#include <sh3/bscreg.h>
#include <sh3/cpgreg.h>
#include <sh3/cache_sh3.h>

/* the following is used externally (sysctl_hw) */
char machine[] = MACHINE;		/* evbsh3 */
char machine_arch[] = MACHINE_ARCH;	/* sh3eb or sh3el */

paddr_t msgbuf_paddr;
extern paddr_t avail_start, avail_end;

#define IOM_RAM_END	((paddr_t)IOM_RAM_BEGIN + IOM_RAM_SIZE - 1)

void initSH3 __P((void *));
void LoadAndReset __P((char *));
void XLoadAndReset __P((char *));
void consinit __P((void));

extern char start[], etext[], edata[], end[];

/*
 * Machine-dependent startup code
 *
 * This is called from main() in kern/main.c.
 */
void
cpu_startup()
{

	sh_startup();
}

/*
 * machine dependent system variables.
 */
int
cpu_sysctl(name, namelen, oldp, oldlenp, newp, newlen, p)
	int *name;
	u_int namelen;
	void *oldp;
	size_t *oldlenp;
	void *newp;
	size_t newlen;
	struct proc *p;
{
	dev_t consdev;
	char *osimage;

	/* all sysctl names at this level are terminal */
	if (namelen != 1)
		return (ENOTDIR);		/* overloaded */

	switch (name[0]) {
	case CPU_CONSDEV:
		if (cn_tab != NULL)
			consdev = cn_tab->cn_dev;
		else
			consdev = NODEV;
		return (sysctl_rdstruct(oldp, oldlenp, newp, &consdev,
		    sizeof consdev));

	case CPU_LOADANDRESET:
		if (newp != NULL) {
			osimage = (char *)(*(u_long *)newp);

			LoadAndReset(osimage);
			/* not reach here */
		}
		return (0);

	default:
		return (EOPNOTSUPP);
	}
	/* NOTREACHED */
}

void
cpu_reboot(howto, bootstr)
	int howto;
	char *bootstr;
{
	static int waittime = -1;

	if (cold) {
		howto |= RB_HALT;
		goto haltsys;
	}

	boothowto = howto;
	if ((howto & RB_NOSYNC) == 0 && waittime < 0) {
		waittime = 0;
		vfs_shutdown();
		/*
		 * If we've been adjusting the clock, the todr
		 * will be out of synch; adjust it now.
		 */
		/* resettodr(); */
	}

	/* Disable interrupts. */
	splhigh();

	/* Do a dump if requested. */
	if ((howto & (RB_DUMP | RB_HALT)) == RB_DUMP)
		dumpsys();

haltsys:
	doshutdownhooks();

	if (howto & RB_HALT) {
		printf("\n");
		printf("The operating system has halted.\n");
		printf("Please press any key to reboot.\n\n");
		cngetc();
	}

	printf("rebooting...\n");
	cpu_reset();
	for(;;)
		;
	/*NOTREACHED*/
}

void
initSH3(void *pc)	/* XXX return address */
{
	vaddr_t kernend;
	vsize_t sz;

	kernend = sh3_round_page(end);

	/* Clear bss */
	memset(edata, 0, end - edata);

	/* Initilize CPU ops. */
#if defined(SH3) && defined(SH4)
#error "don't define both SH3 and SH4"
#elif defined(SH3)
	sh_cpu_init(CPU_ARCH_SH3, CPU_PRODUCT_UNKNOWN);	
#elif defined(SH4)
	sh_cpu_init(CPU_ARCH_SH4, CPU_PRODUCT_UNKNOWN);	
#else
#error "define SH3 or SH4"
#endif
	/* Initialize proc0 and enable MMU. */
	sz = sh_proc0_init(kernend, IOM_RAM_BEGIN, IOM_RAM_END);

	/* Number of pages of physmem addr space */
	physmem = atop(IOM_RAM_END - IOM_RAM_BEGIN + 1);

	/* avail_start is first available physical memory address */
	avail_start = kernend + sz;
	avail_end = IOM_RAM_END + 1;

	consinit();

	/* Call pmap initialization to make new kernel address space */
	pmap_bootstrap(VM_MIN_KERNEL_ADDRESS);

	/*
	 * Initialize error message buffer (at end of core).
	 */
	initmsgbuf((caddr_t)msgbuf_paddr, round_page(MSGBUFSIZE));

	/*
	 * XXX We can't return here, because we change stack pointer.
	 *     So jump to return address directly.
	 */
	__asm __volatile (
		"jmp	@%0;"
		"mov	%1, r15" :: "r"(pc), "r"(proc0.p_addr->u_pcb.pcb_sp));
}

/*
 * consinit:
 * initialize the system console.
 * XXX - shouldn't deal with this initted thing, but then,
 * it shouldn't be called from init386 either.
 */
void
consinit()
{
	static int initted;

	if (initted)
		return;
	initted = 1;

	cninit();

#ifdef DDB
	ddb_init();
#endif
}

int
bus_space_map (t, addr, size, flags, bshp)
	bus_space_tag_t t;
	bus_addr_t addr;
	bus_size_t size;
	int flags;
	bus_space_handle_t *bshp;
{

	*bshp = (bus_space_handle_t)addr;

	return 0;
}

int
sh_memio_subregion(t, bsh, offset, size, nbshp)
	bus_space_tag_t t;
	bus_space_handle_t bsh;
	bus_size_t offset, size;
	bus_space_handle_t *nbshp;
{

	*nbshp = bsh + offset;
	return (0);
}

int
sh_memio_alloc(t, rstart, rend, size, alignment, boundary, flags,
	       bpap, bshp)
	bus_space_tag_t t;
	bus_addr_t rstart, rend;
	bus_size_t size, alignment, boundary;
	int flags;
	bus_addr_t *bpap;
	bus_space_handle_t *bshp;
{
	*bshp = *bpap = rstart;

	return (0);
}

void
sh_memio_free(t, bsh, size)
	bus_space_tag_t t;
	bus_space_handle_t bsh;
	bus_size_t size;
{

}

void
sh_memio_unmap(t, bsh, size)
	bus_space_tag_t t;
	bus_space_handle_t bsh;
	bus_size_t size;
{
	return;
}

#ifdef SH4_PCMCIA

int
shpcmcia_memio_map(t, bpa, size, flags, bshp)
	bus_space_tag_t t;
	bus_addr_t bpa;
	bus_size_t size;
	int flags;
	bus_space_handle_t *bshp;
{
	int error;
	struct extent *ex;
	bus_space_tag_t pt = t & ~SH3_BUS_SPACE_PCMCIA_8BIT;

	if (pt != SH3_BUS_SPACE_PCMCIA_IO && 
	    pt != SH3_BUS_SPACE_PCMCIA_MEM &&
	    pt != SH3_BUS_SPACE_PCMCIA_ATT) {
		*bshp = (bus_space_handle_t)bpa;

		return 0;
	}

	ex = iomem_ex;

#if 0
	/*
	 * Before we go any further, let's make sure that this
	 * region is available.
	 */
	error = extent_alloc_region(ex, bpa, size,
				    EX_NOWAIT | EX_MALLOCOK );
	if (error){
		printf("sh3_pcmcia_memio_map:extent_alloc_region error\n");
		return (error);
	}
#endif

	/*
	 * For memory space, map the bus physical address to
	 * a kernel virtual address.
	 */
	error = shpcmcia_mem_add_mapping(bpa, size, (int)t, bshp );
#if 0
	if (error) {
		if (extent_free(ex, bpa, size, EX_NOWAIT | EX_MALLOCOK )) {
			printf("sh3_pcmcia_memio_map: pa 0x%lx, size 0x%lx\n",
			       bpa, size);
			printf("sh3_pcmcia_memio_map: can't free region\n");
		}
	}
#endif

	return (error);
}

int
shpcmcia_mem_add_mapping(bpa, size, type, bshp)
	bus_addr_t bpa;
	bus_size_t size;
	int type;
	bus_space_handle_t *bshp;
{
	u_long pa, endpa;
	vaddr_t va;
	pt_entry_t *pte;
	unsigned int m = 0;
	int io_type = type & ~SH3_BUS_SPACE_PCMCIA_8BIT;

	pa = sh3_trunc_page(bpa);
	endpa = sh3_round_page(bpa + size);

#ifdef DIAGNOSTIC
	if (endpa <= pa)
		panic("sh3_pcmcia_mem_add_mapping: overflow");
#endif

	va = uvm_km_valloc(kernel_map, endpa - pa);
	if (va == 0){
		printf("shpcmcia_add_mapping: nomem \n");
		return (ENOMEM);
	}

	*bshp = (bus_space_handle_t)(va + (bpa & PGOFSET));

#define MODE(t, s)							\
	(t) & SH3_BUS_SPACE_PCMCIA_8BIT ?				\
		_PG_PCMCIA_ ## s ## 8 :					\
		_PG_PCMCIA_ ## s ## 16
	switch (io_type) {
	default:
		panic("unknown pcmcia space.");
		/* NOTREACHED */
	case SH3_BUS_SPACE_PCMCIA_IO:
		m = MODE(type, IO);
		break;
	case SH3_BUS_SPACE_PCMCIA_MEM:
		m = MODE(type, MEM);
		break;
	case SH3_BUS_SPACE_PCMCIA_ATT:
		m = MODE(type, ATTR);
		break;
	}
#undef MODE

	for (; pa < endpa; pa += NBPG, va += NBPG) {
		pmap_enter(pmap_kernel(), va, pa,
		    VM_PROT_READ | VM_PROT_WRITE, PMAP_WIRED);

		pte = kvtopte(va);
		*pte &= ~PG_N;
		*pte |= m;
		pmap_update_pg(va);
	}
	pmap_update(pmap_kernel());
 
	return 0;
}

void
shpcmcia_memio_unmap(t, bsh, size)
	bus_space_tag_t t;
	bus_space_handle_t bsh;
	bus_size_t size;
{
	struct extent *ex;
	u_long va, endva;
	bus_addr_t bpa;
	bus_space_tag_t pt = t & ~SH3_BUS_SPACE_PCMCIA_8BIT;

	if (pt != SH3_BUS_SPACE_PCMCIA_IO && 
	    pt != SH3_BUS_SPACE_PCMCIA_MEM &&
	    pt != SH3_BUS_SPACE_PCMCIA_ATT) {
		return ;
	}

	ex = iomem_ex;

	va = sh3_trunc_page(bsh);
	endva = sh3_round_page(bsh + size);

#ifdef DIAGNOSTIC
	if (endva <= va)
		panic("sh3_pcmcia_memio_unmap: overflow");
#endif

	pmap_extract(pmap_kernel(), va, &bpa);
	bpa += bsh & PGOFSET;

	/*
	 * Free the kernel virtual mapping.
	 */
	uvm_km_free(kernel_map, va, endva - va);

#if 0
	if (extent_free(ex, bpa, size,
			EX_NOWAIT | EX_MALLOCOK)) {
		printf("sh3_pcmcia_memio_unmap: %s 0x%lx, size 0x%lx\n",
		       "pa", bpa, size);
		printf("sh3_pcmcia_memio_unmap: can't free region\n");
	}
#endif
}

void    
shpcmcia_memio_free(t, bsh, size)
	bus_space_tag_t t;
	bus_space_handle_t bsh;
	bus_size_t size;
{

	/* sh3_pcmcia_memio_unmap() does all that we need to do. */
	shpcmcia_memio_unmap(t, bsh, size);
}

int
shpcmcia_memio_subregion(t, bsh, offset, size, nbshp)
	bus_space_tag_t t;
	bus_space_handle_t bsh;
	bus_size_t offset, size;
	bus_space_handle_t *nbshp;
{

	*nbshp = bsh + offset;
	return (0);
}

#endif /* SH4_PCMCIA */

#if !defined(DONT_INIT_BSC)
/*
 * InitializeBsc
 * : BSC(Bus State Controler)
 */
void InitializeBsc __P((void));

void
InitializeBsc()
{

	/*
	 * Drive RAS,CAS in stand by mode and bus release mode
	 * Area0 = Normal memory, Area5,6=Normal(no burst)
	 * Area2 = Normal memory, Area3 = SDRAM, Area5 = Normal memory
	 * Area4 = Normal Memory
	 * Area6 = Normal memory
	 */
#if defined(SH3)
	_reg_write_2(SH3_BCR1, BSC_BCR1_VAL);
#elif defined(SH4)
	_reg_write_4(SH4_BCR1, BSC_BCR1_VAL);
#endif

	/*
	 * Bus Width
	 * Area4: Bus width = 16bit
	 * Area6,5 = 16bit
	 * Area1 = 8bit
	 * Area2,3: Bus width = 32bit
	 */
	_reg_write_2(SH_(BCR2), BSC_BCR2_VAL);

	/*
	 * Idle cycle number in transition area and read to write
	 * Area6 = 3, Area5 = 3, Area4 = 3, Area3 = 3, Area2 = 3
	 * Area1 = 3, Area0 = 3
	 */
#if defined(SH3)
	_reg_write_2(SH3_WCR1, BSC_WCR1_VAL);
#elif defined(SH4)
	_reg_write_4(SH4_WCR1, BSC_WCR1_VAL);
#endif

	/*
	 * Wait cycle
	 * Area 6 = 6
	 * Area 5 = 2
	 * Area 4 = 10
	 * Area 3 = 3
	 * Area 2,1 = 3
	 * Area 0 = 6
	 */
#if defined(SH3)
	_reg_write_2(SH3_WCR2, BSC_WCR2_VAL);
#elif defined(SH4)
	_reg_write_4(SH4_WCR2, BSC_WCR2_VAL);
#endif

#if defined(SH4) && defined(BSC_WCR3_VAL)
	_reg_write_4(SH4_WCR3, BSC_WCR3_VAL);
#endif

	/*
	 * RAS pre-charge = 2cycle, RAS-CAS delay = 3 cycle,
	 * write pre-charge=1cycle
	 * CAS before RAS refresh RAS assert time = 3 cycle
	 * Disable burst, Bus size=32bit, Column Address=10bit, Refresh ON
	 * CAS before RAS refresh ON, EDO DRAM
	 */
#if defined(SH3)
	_reg_write_2(SH3_MCR, BSC_MCR_VAL);
#elif defined(SH4)
	_reg_write_4(SH4_MCR, BSC_MCR_VAL);
#endif

#if defined(BSC_SDMR2_VAL)
	_reg_write_1(BSC_SDMR2_VAL, 0);
#endif

#if defined(BSC_SDMR3_VAL)
#if !(defined(COMPUTEXEVB) && defined(SH7709A))
	_reg_write_1(BSC_SDMR3_VAL, 0);
#else
	_reg_write_2(0x1a000000, 0);	/* ADDSET */
	_reg_write_1(BSC_SDMR3_VAL, 0);
	_reg_write_2(0x18000000, 0);	/* ADDRST */
#endif /* !(COMPUTEXEVB && SH7709A) */
#endif /* BSC_SDMR3_VAL */

	/*
	 * PCMCIA Control Register
	 * OE/WE assert delay 3.5 cycle
	 * OE/WE negate-address delay 3.5 cycle
	 */
#ifdef BSC_PCR_VAL
	_reg_write_2(SH_(PCR), BSC_PCR_VAL);
#endif

	/*
	 * Refresh Timer Control/Status Register
	 * Disable interrupt by CMF, closk 1/16, Disable OVF interrupt
	 * Count Limit = 1024
	 * In following statement, the reason why high byte = 0xa5(a4 in RFCR)
	 * is the rule of SH3 in writing these register.
	 */
	_reg_write_2(SH_(RTCSR), BSC_RTCSR_VAL);

	/*
	 * Refresh Timer Counter
	 * Initialize to 0
	 */
#ifdef BSC_RTCNT_VAL
	_reg_write_2(SH_(RTCNT), BSC_RTCNT_VAL);
#endif

	/* set Refresh Time Constant Register */
	_reg_write_2(SH_(RTCOR), BSC_RTCOR_VAL);

	/* init Refresh Count Register */
#ifdef BSC_RFCR_VAL
	_reg_write_2(SH_(RFCR), BSC_RFCR_VAL);
#endif

	/*
	 * Clock Pulse Generator
	 */
	/* Set Clock mode (make internal clock double speed) */
	_reg_write_2(SH_(FRQCR), FRQCR_VAL);

	/*
	 * Cache
	 */
#ifndef CACHE_DISABLE
	/* Cache ON */
	_reg_write_4(SH_(CCR), 0x1);
#endif
}
#endif /* !DONT_INIT_BSC */


 /* XXX This value depends on physical available memory */
#define OSIMAGE_BUF_ADDR	(IOM_RAM_BEGIN + 0x00400000)

void
LoadAndReset(osimage)
	char *osimage;
{
	void *buf_addr;
	u_long size;
	u_long *src;
	u_long *dest;
	u_long csum = 0;
	u_long csum2 = 0;
	u_long size2;

	printf("LoadAndReset: copy start\n");
	buf_addr = (void *)OSIMAGE_BUF_ADDR;

	size = *(u_long *)osimage;
	src = (u_long *)osimage;
	dest = buf_addr;

	size = (size + sizeof(u_long) * 2 + 3) >> 2;
	size2 = size;

	while (size--) {
		csum += *src;
		*dest++ = *src++;
	}

	dest = buf_addr;
	while (size2--)
		csum2 += *dest++;

	printf("LoadAndReset: copy end[%lx,%lx]\n", csum, csum2);
	printf("start XLoadAndReset\n");

	/* mask all externel interrupt (XXX) */

	XLoadAndReset(buf_addr);
}

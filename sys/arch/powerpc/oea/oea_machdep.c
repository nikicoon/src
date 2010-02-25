/*	$NetBSD: oea_machdep.c,v 1.53 2010/02/25 23:31:47 matt Exp $	*/

/*
 * Copyright (C) 2002 Matt Thomas
 * Copyright (C) 1995, 1996 Wolfgang Solfrank.
 * Copyright (C) 1995, 1996 TooLs GmbH.
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
 *	This product includes software developed by TooLs GmbH.
 * 4. The name of TooLs GmbH may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY TOOLS GMBH ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TOOLS GMBH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: oea_machdep.c,v 1.53 2010/02/25 23:31:47 matt Exp $");

#include "opt_ppcarch.h"
#include "opt_compat_netbsd.h"
#include "opt_ddb.h"
#include "opt_kgdb.h"
#include "opt_ipkdb.h"
#include "opt_multiprocessor.h"
#include "opt_altivec.h"

#include <sys/param.h>
#include <sys/buf.h>
#include <sys/exec.h>
#include <sys/malloc.h>
#include <sys/mbuf.h>
#include <sys/mount.h>
#include <sys/msgbuf.h>
#include <sys/proc.h>
#include <sys/reboot.h>
#include <sys/syscallargs.h>
#include <sys/syslog.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/boot_flag.h>

#include <uvm/uvm_extern.h>

#include <net/netisr.h>

#ifdef DDB
#include <machine/db_machdep.h>
#include <ddb/db_extern.h>
#endif

#ifdef KGDB
#include <sys/kgdb.h>
#endif
 
#ifdef IPKDB
#include <ipkdb/ipkdb.h>
#endif

#include <powerpc/trap.h>
#include <powerpc/stdarg.h>
#include <powerpc/spr.h>
#include <powerpc/pte.h>
#include <powerpc/altivec.h>
#include <machine/powerpc.h>

#include <powerpc/oea/spr.h>
#include <powerpc/oea/bat.h>
#include <powerpc/oea/sr_601.h>
#include <powerpc/oea/cpufeat.h>

char machine[] = MACHINE;		/* from <machine/param.h> */
char machine_arch[] = MACHINE_ARCH;	/* from <machine/param.h> */

struct vm_map *phys_map = NULL;

/*
 * Global variables used here and there
 */
static void trap0(void *);

/* XXXSL: The battable is not initialized to non-zero for PPC_OEA64 and PPC_OEA64_BRIDGE */
struct bat battable[512];

register_t iosrtable[16];	/* I/O segments, for kernel_pmap setup */
#ifndef MSGBUFADDR
paddr_t msgbuf_paddr;
#endif

void
oea_init(void (*handler)(void))
{
	extern int trapcode[], trapsize[];
	extern int sctrap[], scsize[];
	extern int alitrap[], alisize[];
	extern int dsitrap[], dsisize[];
	extern int trapstart[], trapend[];
#ifdef PPC_OEA601
	extern int dsi601trap[], dsi601size[];
#endif
	extern int decrint[], decrsize[];
	extern int tlbimiss[], tlbimsize[];
	extern int tlbdlmiss[], tlbdlmsize[];
	extern int tlbdsmiss[], tlbdsmsize[];
#if defined(DDB) || defined(KGDB)
	extern int ddblow[], ddbsize[];
#endif
#ifdef IPKDB
	extern int ipkdblow[], ipkdbsize[];
#endif
#ifdef ALTIVEC
	register_t msr;
#endif
	uintptr_t exc, exc_base;
#if defined(ALTIVEC) || defined(PPC_OEA)
	register_t scratch;
#endif
	unsigned int cpuvers;
	size_t size;
	struct cpu_info * const ci = &cpu_info[0];

#ifdef PPC_HIGH_VEC
	exc_base = EXC_HIGHVEC;
#else
	exc_base = 0;
#endif
	mtspr(SPR_SPRG0, ci);
	cpuvers = mfpvr() >> 16;

	/*
	 * Initialize proc0 and current pcb and pmap pointers.
	 */
	KASSERT(ci != NULL);
	KASSERT(curcpu() == ci);
	lwp0.l_cpu = ci;

	curpcb = lwp_getpcb(&lwp0);
	memset(curpcb, 0, sizeof(struct pcb));

#ifdef ALTIVEC
	/*
	 * Initialize the vectors with NaNs
	 */
	for (scratch = 0; scratch < 32; scratch++) {
		curpcb->pcb_vr.vreg[scratch][0] = 0x7FFFDEAD;
		curpcb->pcb_vr.vreg[scratch][1] = 0x7FFFDEAD;
		curpcb->pcb_vr.vreg[scratch][2] = 0x7FFFDEAD;
		curpcb->pcb_vr.vreg[scratch][3] = 0x7FFFDEAD;
	}
	curpcb->pcb_vr.vscr = 0;
	curpcb->pcb_vr.vrsave = 0;
#endif
	curpm = curpcb->pcb_pm = pmap_kernel();

	/*
	 * Cause a PGM trap if we branch to 0.
	 *
	 * XXX GCC4.1 complains about memset on address zero, so
	 * don't use the builtin.
	 */
#undef memset
	memset(0, 0, 0x100);

	/*
	 * Set up trap vectors.  Don't assume vectors are on 0x100.
	 */
	for (exc = exc_base; exc <= exc_base + EXC_LAST; exc += 0x100) {
		switch (exc - exc_base) {
		default:
			size = (size_t)trapsize;
			memcpy((void *)exc, trapcode, size);
			break;
#if 0
		case EXC_EXI:
			/*
			 * This one is (potentially) installed during autoconf
			 */
			break;
#endif
		case EXC_SC:
			size = (size_t)scsize;
			memcpy((void *)exc, sctrap, size);
			break;
		case EXC_ALI:
			size = (size_t)alisize;
			memcpy((void *)exc, alitrap, size);
			break;
		case EXC_DSI:
#ifdef PPC_OEA601
			if (cpuvers == MPC601) {
				size = (size_t)dsi601size;
				memcpy((void *)exc, dsi601trap, size);
				break;
			} else
#endif /* PPC_OEA601 */
			if (oeacpufeat & OEACPU_NOBAT) {
				size = (size_t)alisize;
				memcpy((void *)exc, alitrap, size);
			} else {
				size = (size_t)dsisize;
				memcpy((void *)exc, dsitrap, size);
			}
			break;
		case EXC_DECR:
			size = (size_t)decrsize;
			memcpy((void *)exc, decrint, size);
			break;
		case EXC_IMISS:
			size = (size_t)tlbimsize;
			memcpy((void *)exc, tlbimiss, size);
			break;
		case EXC_DLMISS:
			size = (size_t)tlbdlmsize;
			memcpy((void *)exc, tlbdlmiss, size);
			break;
		case EXC_DSMISS:
			size = (size_t)tlbdsmsize;
			memcpy((void *)exc, tlbdsmiss, size);
			break;
		case EXC_PERF:
			size = (size_t)trapsize;
			memcpy((void *)exc, trapcode, size);
			memcpy((void *)(exc_base + EXC_VEC),  trapcode, size);
			break;
#if defined(DDB) || defined(IPKDB) || defined(KGDB)
		case EXC_RUNMODETRC:
#ifdef PPC_OEA601
			if (cpuvers != MPC601) {
#endif
				size = (size_t)trapsize;
				memcpy((void *)exc, trapcode, size);
				break;
#ifdef PPC_OEA601
			}
			/* FALLTHROUGH */
#endif
		case EXC_PGM:
		case EXC_TRC:
		case EXC_BPT:
#if defined(DDB) || defined(KGDB)
			size = (size_t)ddbsize;
			memcpy((void *)exc, ddblow, size);
#if defined(IPKDB)
#error "cannot enable IPKDB with DDB or KGDB"
#endif
#else
			size = (size_t)ipkdbsize;
			memcpy((void *)exc, ipkdblow, size);
#endif
			break;
#endif /* DDB || IPKDB || KGDB */
		}
#if 0
		exc += roundup(size, 32);
#endif
	}

	/*
	 * Install a branch absolute to trap0 to force a panic.
	 */
	if ((uintptr_t)trap0 < 0x2000000) {
		*(uint32_t *) 0 = 0x7c6802a6;
		*(uint32_t *) 4 = 0x48000002 | (uintptr_t) trap0;
	}

	/*
	 * Get the cache sizes because install_extint calls __syncicache.
	 */
	cpu_probe_cache();

#define	MxSPR_MASK	0x7c1fffff
#define	MFSPR_MQ	0x7c0002a6
#define	MTSPR_MQ	0x7c0003a6
#define	MTSPR_IBAT0L	0x7c1183a6
#define	MTSPR_IBAT1L	0x7c1383a6
#define	NOP		0x60000000
#define	B		0x48000000
#define	TLBSYNC		0x7c00046c
#define	SYNC		0x7c0004ac

#ifdef ALTIVEC
#define	MFSPR_VRSAVE	0x7c0042a6
#define	MTSPR_VRSAVE	0x7c0043a6
	
	/*
	 * Try to set the VEC bit in the MSR.  If it doesn't get set, we are
	 * not on a AltiVec capable processor.
	 */
	__asm volatile (
	    "mfmsr %0; oris %1,%0,%2@h; mtmsr %1; isync; "
		"mfmsr %1; mtmsr %0; isync"
	    :	"=r"(msr), "=r"(scratch)
	    :	"J"(PSL_VEC));

	/*
	 * If we aren't on an AltiVec capable processor, we need to zap any of
	 * the sequences we save/restore the VRSAVE SPR into NOPs.
	 */
	if (scratch & PSL_VEC) {
		cpu_altivec = 1;
	} else {
		int *ip = trapstart;
		
		for (; ip < trapend; ip++) {
			if ((ip[0] & MxSPR_MASK) == MFSPR_VRSAVE) {
				ip[0] = NOP;	/* mfspr */
				ip[1] = NOP;	/* stw */
			} else if ((ip[0] & MxSPR_MASK) == MTSPR_VRSAVE) {
				ip[-1] = NOP;	/* lwz */
				ip[0] = NOP;	/* mtspr */
			}
		}
	}
#endif

	/* XXX It would seem like this code could be elided ifndef 601, but
	 * doing so breaks my power3 machine.
	 */
	/*
	 * If we aren't on a MPC601 processor, we need to zap any of the
	 * sequences we save/restore the MQ SPR into NOPs, and skip over the
	 * sequences where we zap/restore BAT registers on kernel exit/entry.
	 */
	if (cpuvers != MPC601) {
		int *ip = trapstart;
		
		for (; ip < trapend; ip++) {
			if ((ip[0] & MxSPR_MASK) == MFSPR_MQ) {
				ip[0] = NOP;	/* mfspr */
				ip[1] = NOP;	/* stw */
			} else if ((ip[0] & MxSPR_MASK) == MTSPR_MQ) {
				ip[-1] = NOP;	/* lwz */
				ip[0] = NOP;	/* mtspr */
			} else if ((ip[0] & MxSPR_MASK) == MTSPR_IBAT0L) {
				if ((ip[1] & MxSPR_MASK) == MTSPR_IBAT1L)
					ip[-1] = B | 0x14;	/* li */
				else
					ip[-4] = B | 0x24;	/* lis */
			}
		}
	}

	/*
	 * Sync the changed instructions.
	 */
	__syncicache((void *) trapstart,
	    (uintptr_t) trapend - (uintptr_t) trapstart);
#ifdef PPC_OEA601

	/*
	 * If we are on a MPC601 processor, we need to zap any tlbsync
	 * instructions into sync.  This differs from the above in
	 * examing all kernel text, as opposed to just the exception handling.
	 * We sync the icache on every instruction found since there are
	 * only very few of them.
	 */
	if (cpuvers == MPC601) {
		extern int kernel_text[], etext[];
		int *ip;

		for (ip = kernel_text; ip < etext; ip++)
			if (*ip == TLBSYNC) {
				*ip = SYNC;
				__syncicache(ip, sizeof(*ip));
		}
	}
#endif /* PPC_OEA601 */

        /*
	 * Configure a PSL user mask matching this processor.
 	 */
	cpu_psluserset = PSL_EE | PSL_PR | PSL_ME | PSL_IR | PSL_DR | PSL_RI;
	cpu_pslusermod = PSL_FP | PSL_FE0 | PSL_FE1 | PSL_LE | PSL_SE | PSL_BE;
#ifdef PPC_OEA601
	if (cpuvers == MPC601) {
		cpu_psluserset &= PSL_601_MASK;
		cpu_pslusermod &= PSL_601_MASK;
	}
#endif
#ifdef ALTIVEC
	if (cpu_altivec)
		cpu_pslusermod |= PSL_VEC;
#endif
#ifdef PPC_HIGH_VEC
	cpu_psluserset |= PSL_IP;	/* XXX ok? */
#endif

	/*
	 * external interrupt handler install
	 */
	if (handler)
		oea_install_extint(handler);

	__syncicache((void *)exc_base, EXC_LAST + 0x100);

	/*
	 * Now enable translation (and machine checks/recoverable interrupts).
	 */
#ifdef PPC_OEA
	__asm volatile ("sync; mfmsr %0; ori %0,%0,%1; mtmsr %0; isync"
	    : "=r"(scratch)
	    : "K"(PSL_IR|PSL_DR|PSL_ME|PSL_RI));
#endif

	KASSERT(curcpu() == ci);
}

#ifdef PPC_OEA601
void
mpc601_ioseg_add(paddr_t pa, register_t len)
{
	const u_int i = pa >> ADDR_SR_SHFT;

	if (len != BAT_BL_256M)
		panic("mpc601_ioseg_add: len != 256M");

	/*
	 * Translate into an I/O segment, load it, and stash away for use
	 * in pmap_bootstrap().
	 */
	iosrtable[i] = SR601(SR601_Ks, SR601_BUID_MEMFORCED, 0, i);
	__asm volatile ("mtsrin %0,%1"
	    ::	"r"(iosrtable[i]),
		"r"(pa));
}
#endif /* PPC_OEA601 */

#if defined (PPC_OEA) || defined (PPC_OEA64_BRIDGE)
void
oea_iobat_add(paddr_t pa, register_t len)
{
	static int n = 1;
	const u_int i = pa >> 28;
	battable[i].batl = BATL(pa, BAT_I|BAT_G, BAT_PP_RW);
	battable[i].batu = BATU(pa, len, BAT_Vs);

	/*
	 * Let's start loading the BAT registers.
	 */
	switch (n) {
	case 1:
		__asm volatile ("mtdbatl 1,%0; mtdbatu 1,%1;"
		    ::	"r"(battable[i].batl),
			"r"(battable[i].batu));
		n = 2;
		break;
	case 2:
		__asm volatile ("mtdbatl 2,%0; mtdbatu 2,%1;"
		    ::	"r"(battable[i].batl),
			"r"(battable[i].batu));
		n = 3;
		break;
	case 3:
		__asm volatile ("mtdbatl 3,%0; mtdbatu 3,%1;"
		    ::	"r"(battable[i].batl),
			"r"(battable[i].batu));
		n = 4;
		break;
	default:
		break;
	}
}

void
oea_iobat_remove(paddr_t pa)
{
	register_t batu;
	int i, n;

	n = pa >> ADDR_SR_SHFT;
	if (!BAT_VA_MATCH_P(battable[n].batu, pa) ||
	    !BAT_VALID_P(battable[n].batu, PSL_PR))
		return;
	battable[n].batl = 0;
	battable[n].batu = 0;
#define	BAT_RESET(n) \
	__asm volatile("mtdbatu %0,%1; mtdbatl %0,%1" :: "n"(n), "r"(0))
#define	BATU_GET(n, r)	__asm volatile("mfdbatu %0,%1" : "=r"(r) : "n"(n))

	for (i=1 ; i<4 ; i++) {
		switch (i) {
		case 1:
			BATU_GET(1, batu);
			if (BAT_VA_MATCH_P(batu, pa) &&
			    BAT_VALID_P(batu, PSL_PR))
				BAT_RESET(1);
			break;
		case 2:
			BATU_GET(2, batu);
			if (BAT_VA_MATCH_P(batu, pa) &&
			    BAT_VALID_P(batu, PSL_PR))
				BAT_RESET(2);
			break;
		case 3:
			BATU_GET(3, batu);
			if (BAT_VA_MATCH_P(batu, pa) &&
			    BAT_VALID_P(batu, PSL_PR))
				BAT_RESET(3);
			break;
		default:
			break;
		}
	}
}

void
oea_batinit(paddr_t pa, ...)
{
	struct mem_region *allmem, *availmem, *mp;
	unsigned int cpuvers;
	register_t msr = mfmsr();
	va_list ap;

	cpuvers = mfpvr() >> 16;

	/*
	 * Initialize BAT registers to unmapped to not generate
	 * overlapping mappings below.
	 *
	 * The 601's implementation differs in the Valid bit being situated
	 * in the lower BAT register, and in being a unified BAT only whose
	 * four entries are accessed through the IBAT[0-3] SPRs.
	 *
	 * Also, while the 601 does distinguish between supervisor/user
	 * protection keys, it does _not_ distinguish between validity in
	 * supervisor/user mode.
	 */
	if ((msr & (PSL_IR|PSL_DR)) == 0) {
#ifdef PPC_OEA601
		if (cpuvers == MPC601) {
			__asm volatile ("mtibatl 0,%0" :: "r"(0));
			__asm volatile ("mtibatl 1,%0" :: "r"(0));
			__asm volatile ("mtibatl 2,%0" :: "r"(0));
			__asm volatile ("mtibatl 3,%0" :: "r"(0));
		} else
#endif /* PPC_OEA601 */
		{
			__asm volatile ("mtibatu 0,%0" :: "r"(0));
			__asm volatile ("mtibatu 1,%0" :: "r"(0));
			__asm volatile ("mtibatu 2,%0" :: "r"(0));
			__asm volatile ("mtibatu 3,%0" :: "r"(0));
			__asm volatile ("mtdbatu 0,%0" :: "r"(0));
			__asm volatile ("mtdbatu 1,%0" :: "r"(0));
			__asm volatile ("mtdbatu 2,%0" :: "r"(0));
			__asm volatile ("mtdbatu 3,%0" :: "r"(0));
		}
	}

	/*
	 * Set up BAT to map physical memory
	 */
#ifdef PPC_OEA601
	if (cpuvers == MPC601) {
		int i;
		
		/*
		 * Set up battable to map the lowest 256 MB area.
		 * Map the lowest 32 MB area via BAT[0-3];
		 * BAT[01] are fixed, BAT[23] are floating.
		 */
		for (i = 0; i < 32; i++) {
			battable[i].batl = BATL601(i << 23,
			   BAT601_BSM_8M, BAT601_V);
			battable[i].batu = BATU601(i << 23,
			    BAT601_M, BAT601_Ku, BAT601_PP_NONE);
		}
		__asm volatile ("mtibatu 0,%1; mtibatl 0,%0"
		    :: "r"(battable[0x00000000 >> 23].batl),
		       "r"(battable[0x00000000 >> 23].batu));
		__asm volatile ("mtibatu 1,%1; mtibatl 1,%0"
		    :: "r"(battable[0x00800000 >> 23].batl),
		       "r"(battable[0x00800000 >> 23].batu));
		__asm volatile ("mtibatu 2,%1; mtibatl 2,%0"
		    :: "r"(battable[0x01000000 >> 23].batl),
		       "r"(battable[0x01000000 >> 23].batu));
		__asm volatile ("mtibatu 3,%1; mtibatl 3,%0"
		    :: "r"(battable[0x01800000 >> 23].batl),
		       "r"(battable[0x01800000 >> 23].batu));
	} else
#endif /* PPC_OEA601 */
	{
		/*
		 * Set up BAT0 to only map the lowest 256 MB area
		 */
		battable[0].batl = BATL(0x00000000, BAT_M, BAT_PP_RW);
		battable[0].batu = BATU(0x00000000, BAT_BL_256M, BAT_Vs);

		__asm volatile ("mtibatl 0,%0; mtibatu 0,%1;"
				  "mtdbatl 0,%0; mtdbatu 0,%1;"
		    ::	"r"(battable[0].batl), "r"(battable[0].batu));
	}

	/*
	 * Now setup other fixed bat registers
	 *
	 * Note that we still run in real mode, and the BAT
	 * registers were cleared above.
	 */

	va_start(ap, pa);

	/*
	 * Add any I/O BATs specificed;
	 * use I/O segments on the BAT-starved 601.
	 */
#ifdef PPC_OEA601
	if (cpuvers == MPC601) {
		while (pa != 0) {
			register_t len = va_arg(ap, register_t);
			mpc601_ioseg_add(pa, len);
			pa = va_arg(ap, paddr_t);
		}
	} else
#endif
	{
		while (pa != 0) {
			register_t len = va_arg(ap, register_t);
			oea_iobat_add(pa, len);
			pa = va_arg(ap, paddr_t);
		}
	}

	va_end(ap);

	/*
	 * Set up battable to map all RAM regions.
	 * This is here because mem_regions() call needs bat0 set up.
	 */
	mem_regions(&allmem, &availmem);
#ifdef PPC_OEA601
	if (cpuvers == MPC601) {
		for (mp = allmem; mp->size; mp++) {
			paddr_t paddr = mp->start & 0xff800000;
			paddr_t end = mp->start + mp->size;

			do {
				u_int ix = paddr >> 23;

				battable[ix].batl =
				    BATL601(paddr, BAT601_BSM_8M, BAT601_V);
				battable[ix].batu =
				    BATU601(paddr, BAT601_M, BAT601_Ku, BAT601_PP_NONE);
				paddr += (1 << 23);
			} while (paddr < end);
		}
	} else
#endif
	{
		for (mp = allmem; mp->size; mp++) {
			paddr_t paddr = mp->start & 0xf0000000;
			paddr_t end = mp->start + mp->size;

			do {
				u_int ix = paddr >> 28;

				battable[ix].batl =
				    BATL(paddr, BAT_M, BAT_PP_RW);
				battable[ix].batu =
				    BATU(paddr, BAT_BL_256M, BAT_Vs);
				paddr += SEGMENT_LENGTH;
			} while (paddr < end);
		}
	}
}
#endif /* PPC_OEA || PPC_OEA64_BRIDGE */

void
oea_install_extint(void (*handler)(void))
{
	extern int extint[], extsize[];
	extern int extint_call[];
	uintptr_t offset = (uintptr_t)handler - (uintptr_t)extint_call;
	int omsr, msr;

#ifdef	DIAGNOSTIC
	if (offset > 0x1ffffff)
		panic("install_extint: %p too far away (%#lx)", handler,
		    (unsigned long) offset);
#endif
	__asm volatile ("mfmsr %0; andi. %1,%0,%2; mtmsr %1"
	    :	"=r" (omsr), "=r" (msr)
	    :	"K" ((u_short)~PSL_EE));
	extint_call[0] = (extint_call[0] & 0xfc000003) | offset;
	__syncicache((void *)extint_call, sizeof extint_call[0]);
#ifdef PPC_HIGH_VEC
	memcpy((void *)(EXC_HIGHVEC + EXC_EXI), extint, (size_t)extsize);
	__syncicache((void *)(EXC_HIGHVEC + EXC_EXI), (int)extsize);
#else
	memcpy((void *)EXC_EXI, extint, (size_t)extsize);
	__syncicache((void *)EXC_EXI, (int)extsize);
#endif
	__asm volatile ("mtmsr %0" :: "r"(omsr));
}

/*
 * Machine dependent startup code.
 */
void
oea_startup(const char *model)
{
	uintptr_t sz;
	void *v;
	vaddr_t minaddr, maxaddr;
	char pbuf[9];

	KASSERT(curcpu() != NULL);
	KASSERT(lwp0.l_cpu != NULL);
	KASSERT(curcpu()->ci_intstk != 0);
	KASSERT(curcpu()->ci_intrdepth == -1);

	sz = round_page(MSGBUFSIZE);
#ifdef MSGBUFADDR
	v = (void *) MSGBUFADDR;
#else
	/*
	 * If the msgbuf is not in segment 0, allocate KVA for it and access
	 * it via mapped pages.  [This prevents unneeded BAT switches.]
	 */
	v = (void *) msgbuf_paddr;
	if (msgbuf_paddr + sz > SEGMENT_LENGTH) {
		u_int i;

		minaddr = 0;
		if (uvm_map(kernel_map, &minaddr, sz,
				NULL, UVM_UNKNOWN_OFFSET, 0,
				UVM_MAPFLAG(UVM_PROT_NONE, UVM_PROT_NONE,
				    UVM_INH_NONE, UVM_ADV_NORMAL, 0)) != 0)
			panic("startup: cannot allocate VM for msgbuf");
		v = (void *)minaddr;
		for (i = 0; i < sz; i += PAGE_SIZE) {
			pmap_kenter_pa(minaddr + i, msgbuf_paddr + i,
			    VM_PROT_READ|VM_PROT_WRITE, 0);
		}
		pmap_update(pmap_kernel());
	}
#endif
	initmsgbuf(v, sz);

	printf("%s%s", copyright, version);
	if (model != NULL)
		printf("Model: %s\n", model);
	cpu_identify(NULL, 0);

	format_bytes(pbuf, sizeof(pbuf), ctob((u_int)physmem));
	printf("total memory = %s\n", pbuf);

	/*
	 * Allocate away the pages that map to 0xDEA[CDE]xxxx.  Do this after
	 * the bufpages are allocated in case they overlap since it's not
	 * fatal if we can't allocate these.
	 */
	if (KERNEL_SR == 13 || KERNEL2_SR == 14) {
		int error;
		minaddr = 0xDEAC0000;
		error = uvm_map(kernel_map, &minaddr, 0x30000,
		    NULL, UVM_UNKNOWN_OFFSET, 0,
		    UVM_MAPFLAG(UVM_PROT_NONE, UVM_PROT_NONE, UVM_INH_NONE,  
				UVM_ADV_NORMAL, UVM_FLAG_FIXED));
		if (error != 0 || minaddr != 0xDEAC0000)
			printf("oea_startup: failed to allocate DEAD "
			    "ZONE: error=%d\n", error);
	}
 
	minaddr = 0;

	/*
	 * Allocate a submap for physio
	 */
	phys_map = uvm_km_suballoc(kernel_map, &minaddr, &maxaddr,
				 VM_PHYS_SIZE, 0, false, NULL);

	format_bytes(pbuf, sizeof(pbuf), ptoa(uvmexp.free));
	printf("avail memory = %s\n", pbuf);
}

/*
 * Crash dump handling.
 */

void
oea_dumpsys(void)
{
	printf("dumpsys: TBD\n");
}

/*
 * Convert kernel VA to physical address
 */
paddr_t
kvtop(void *addr)
{
	vaddr_t va;
	paddr_t pa;
	uintptr_t off;
	extern char end[];

	if (addr < (void *)end)
		return (paddr_t)addr;

	va = trunc_page((vaddr_t)addr);
	off = (uintptr_t)addr - va;

	if (pmap_extract(pmap_kernel(), va, &pa) == false) {
		/*printf("kvtop: zero page frame (va=0x%x)\n", addr);*/
		return (paddr_t)addr;
	}

	return(pa + off);
}

/*
 * Allocate vm space and mapin the I/O address
 */
void *
mapiodev(paddr_t pa, psize_t len)
{
	paddr_t faddr;
	vaddr_t taddr, va;
	int off;

	faddr = trunc_page(pa);
	off = pa - faddr;
	len = round_page(off + len);
	va = taddr = uvm_km_alloc(kernel_map, len, 0, UVM_KMF_VAONLY);

	if (va == 0)
		return NULL;

	for (; len > 0; len -= PAGE_SIZE) {
		pmap_kenter_pa(taddr, faddr, VM_PROT_READ | VM_PROT_WRITE, 0);
		faddr += PAGE_SIZE;
		taddr += PAGE_SIZE;
	}
	pmap_update(pmap_kernel());
	return (void *)(va + off);
}

void
unmapiodev(vaddr_t va, vsize_t len)
{
	paddr_t faddr;

	if (! va)
		return;

	faddr = trunc_page(va);
	len = round_page(va - faddr + len);

	pmap_kremove(faddr, len);
	pmap_update(pmap_kernel());
	uvm_km_free(kernel_map, faddr, len, UVM_KMF_VAONLY);
}

void
trap0(void *lr)
{
	panic("call to null-ptr from %p", lr);
}

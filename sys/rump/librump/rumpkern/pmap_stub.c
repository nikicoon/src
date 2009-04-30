/*	$NetBSD: pmap_stub.c,v 1.13 2009/04/30 15:31:16 skrll Exp $	*/

/*
 * Copyright (c) 2008 Antti Kantee.  All Rights Reserved.
 *
 * Development of this software was supported by the
 * Finnish Cultural Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: pmap_stub.c,v 1.13 2009/04/30 15:31:16 skrll Exp $");

#include <sys/param.h>

#include <uvm/uvm_extern.h>

struct pmap *const kernel_pmap_ptr = (struct pmap *const)0x66;

/*
 * Provide a userspace pmap with the headers the kernel gives us.
 * This results in some arch-specific frobbling which
 * cannot really be easily avoided until the pmap interface is
 * specified with something else than a .h and autogenerated into C.
 */

#if defined(__sparc__) && !defined(__sparc64__)
#undef pmap_is_modified
#undef pmap_clear_modify
#undef pmap_kenter_pa
#undef pmap_kremove
#undef pmap_page_protect
#undef pmap_extract

bool pmap_is_modified(struct vm_page *);
bool pmap_clear_modify(struct vm_page *);
void pmap_kenter_pa(vaddr_t, paddr_t, vm_prot_t);
void pmap_kremove(vaddr_t, vsize_t);
void pmap_page_protect(struct vm_page *, vm_prot_t);
bool pmap_extract(pmap_t pmap, vaddr_t va, paddr_t *);
#endif

#if !defined(pmap_is_modified) && !defined(__vax__)
bool
pmap_is_modified(struct vm_page *pg)
{

	return true;
}
#endif

#if !defined(pmap_clear_modify) && !defined(__vax__)
bool
pmap_clear_modify(struct vm_page *pg)
{

	return true;
}
#endif

#ifndef pmap_update
void
pmap_update(pmap_t pmap)
{

}
#endif

void
pmap_kenter_pa(vaddr_t va, paddr_t pa, vm_prot_t prot)
{

	panic("%s: unavailable", __func__);
}

void
pmap_kremove(vaddr_t va, vsize_t size)
{

	panic("%s: unavailable", __func__);
}

#ifndef __vax__
bool
pmap_extract(pmap_t pmap, vaddr_t va, paddr_t *pap)
{

	*pap = va;
	return true;
}
#endif

/*
 * Begin MD stubs
 */
#if !defined(__i386__) && !defined(__x86_64__) && \
    !defined(__hppa__) && \
    !defined(__vax__) 
void
pmap_page_protect(struct vm_page *pg, vm_prot_t prot)
{

	/* nada */
}
#endif

#ifdef __hppa__
void
pmap_page_remove(struct vm_page *pg)
{

}

bool
pmap_changebit(struct vm_page *pg, unsigned int set, unsigned int clear)
{

	return true;
}

bool
pmap_testbit(struct vm_page *pg, unsigned int bit)
{

	return true;
}
#endif

#if defined(__i386__) || defined(__x86_64__)
bool
pmap_clear_attrs(struct vm_page *pg, unsigned what)
{

	return false;
}

void
pmap_page_remove(struct vm_page *pg)
{

}

bool
pmap_test_attrs(struct vm_page *pg, unsigned what)
{

	return true;
}

paddr_t
vtophys(vaddr_t va)
{

	return va;
}
#endif

#ifdef __vax__
/*
 * XXX: this won't work on vax.  But I'm not terribly
 * uberinterested in that for the time being.
 */
struct pv_entry *pv_table;
bool
pmap_clear_modify_long(struct pv_entry *pv)
{

	return true;
}

bool
pmap_clear_reference_long(struct pv_entry *pv)
{

	return true;
}

bool
pmap_is_modified_long(struct pv_entry *pv)
{

	return true;
}

void
pmap_page_protect_long(struct pv_entry *pv, vm_prot_t prot)
{

}

void
pmap_protect_long(pmap_t pmap, vaddr_t va1, vaddr_t va2, vm_prot_t prot)
{

}
#endif

#ifdef PPC_OEA
bool
pmap_clear_bit(struct vm_page *pg, int ptebit)
{

	return true;
}

bool
pmap_query_bit(struct vm_page *pg, int ptebit)
{

	return true;
}
#endif

#if defined(__sparc__) && !defined(__sparc64__)
bool     (*pmap_clear_modify_p)(struct vm_page *) = pmap_clear_modify;
bool     (*pmap_is_modified_p)(struct vm_page *) = pmap_is_modified;
void     (*pmap_kenter_pa_p)(vaddr_t, paddr_t, vm_prot_t) = pmap_kenter_pa;
void     (*pmap_kremove_p)(vaddr_t, vsize_t) = pmap_kremove;
void     (*pmap_page_protect_p)(struct vm_page *, vm_prot_t)=pmap_page_protect;
bool     (*pmap_extract_p)(pmap_t, vaddr_t, paddr_t *) = pmap_extract;
#endif

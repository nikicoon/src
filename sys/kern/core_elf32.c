/*	$NetBSD: core_elf32.c,v 1.1 2001/12/09 23:05:59 thorpej Exp $	*/

/*
 * Copyright (c) 2001 Wasabi Systems, Inc.
 * All rights reserved.
 *
 * Written by Jason R. Thorpe for Wasabi Systems, Inc.
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
 *	This product includes software developed for the NetBSD Project by
 *	Wasabi Systems, Inc.
 * 4. The name of Wasabi Systems, Inc. may not be used to endorse
 *    or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY WASABI SYSTEMS, INC. ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL WASABI SYSTEMS, INC
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * core_elf32.c/core_elf64.c: Support for the Elf32/Elf64 core file format.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(1, "$NetBSD: core_elf32.c,v 1.1 2001/12/09 23:05:59 thorpej Exp $");

/* If not included by core_elf64.c, ELFSIZE won't be defined. */
#ifndef ELFSIZE
#define	ELFSIZE		32
#endif

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/vnode.h>
#include <sys/exec_elf.h>
#include <sys/ptrace.h>

#include <machine/reg.h>

#include <uvm/uvm.h>

int	ELFNAMEEND(coredump_notes)(struct proc *, struct vnode *,
	    struct ucred *, int *, off_t);

#define	ELFROUNDSIZE	4	/* XXX Should it be sizeof(Elf_Word)? */
#define	elfround(x)	roundup((x), ELFROUNDSIZE)

int
ELFNAMEEND(coredump)(struct proc *p, struct vnode *vp, struct ucred *cred)
{
	Elf_Ehdr ehdr;
	Elf_Phdr phdr;
	struct vmspace *vm = p->p_vmspace;
	struct vm_map *map = &vm->vm_map;
	struct vm_map_entry *entry;
	vaddr_t start, end, maxstack;
	vsize_t size;
	off_t offset, secoff, notestart, secstart;
	int npsections, notesize, error;

	maxstack = trunc_page(USRSTACK - ctob(vm->vm_ssize));

	/*
	 * We have to make a total of 3 passes across the map:
	 *
	 *	1. Count the number of map entries (the number of
	 *	   PT_LOAD sections).
	 *
	 *	2. Write the P-section headers.
	 *
	 *	3. Write the P-sections.
	 */

	/* Pass 1: count the entries. */
	npsections = 0;
	for (entry = map->header.next; entry != &map->header;
	     entry = entry->next) {
		/* Should never happen for a user process. */
		if (UVM_ET_ISSUBMAP(entry))
			panic("coredump_elf: user process with submap?");

		if (entry->start >= VM_MAXUSER_ADDRESS)
			continue;

		if (entry->start >= (vaddr_t)vm->vm_maxsaddr &&
		    entry->end <= maxstack)
			continue;

		npsections++;
	}

	/* Get the size of the notes. */
	error = ELFNAMEEND(coredump_notes)(p, vp, cred, &notesize, 0);
	if (error)
		return (error);

	/* Count the PT_NOTE section. */
	npsections++;

	memcpy(ehdr.e_ident, ELFMAG, SELFMAG);
#if ELFSIZE == 32
	ehdr.e_ident[EI_CLASS] = ELFCLASS32;
#elif ELFSIZE == 64
	ehdr.e_ident[EI_CLASS] = ELFCLASS64;
#endif
	ehdr.e_ident[EI_DATA] = ELFDEFNNAME(MACHDEP_ENDIANNESS);
	ehdr.e_ident[EI_VERSION] = EV_CURRENT;
	/* XXX Should be the OSABI/ABI version of the executable. */
	ehdr.e_ident[EI_OSABI] = ELFOSABI_SYSV;
	ehdr.e_ident[EI_ABIVERSION] = 1;

	ehdr.e_type = ET_CORE;
	/* XXX This should be the e_machine of the executable. */
	ehdr.e_machine = ELFDEFNNAME(MACHDEP_ID);
	ehdr.e_version = EV_CURRENT;
	ehdr.e_entry = 0;
	ehdr.e_phoff = sizeof(ehdr);
	ehdr.e_shoff = 0;
	ehdr.e_flags = 0;
	ehdr.e_ehsize = sizeof(ehdr);
	ehdr.e_phentsize = sizeof(Elf_Phdr);
	ehdr.e_phnum = npsections;
	ehdr.e_shentsize = 0;
	ehdr.e_shnum = 0;
	ehdr.e_shstrndx = 0;

	/* Write out the ELF header. */
	error = vn_rdwr(UIO_WRITE, vp, (caddr_t)&ehdr,
	    (int)sizeof(ehdr), (off_t)0,
	    UIO_SYSSPACE, IO_NODELOCKED|IO_UNIT, cred, NULL, p);

	offset = ehdr.e_phoff;
	notestart = offset + (sizeof(phdr) * npsections);
	secstart = round_page(notestart + notesize);

	/*
	 * Now write the P-section headers.
	 */
	secoff = secstart;
	for (entry = map->header.next; entry != &map->header;
	     entry = entry->next) {
		start = entry->start;
		end = entry->end;

		if (start >= VM_MAXUSER_ADDRESS)
			continue;

		if (end > VM_MAXUSER_ADDRESS)
			end = VM_MAXUSER_ADDRESS;

		if (start >= (vaddr_t)vm->vm_maxsaddr) {
			if (end <= maxstack)
				continue;
			if (start < maxstack)
				start = maxstack; 
		}

		size = end - start;

		phdr.p_type = PT_LOAD;
		phdr.p_offset = secoff;
		phdr.p_vaddr = start;
		phdr.p_paddr = 0;
		phdr.p_filesz = (entry->protection & VM_PROT_WRITE) ? size : 0;
		phdr.p_memsz = size;
		phdr.p_flags = 0;
		if (entry->protection & VM_PROT_READ)
			phdr.p_flags |= PF_R;
		if (entry->protection & VM_PROT_WRITE)
			phdr.p_flags |= PF_W;
		if (entry->protection & VM_PROT_EXECUTE)
			phdr.p_flags |= PF_X;
		phdr.p_align = PAGE_SIZE;

		error = vn_rdwr(UIO_WRITE, vp,
		    (caddr_t)&phdr, sizeof(phdr),
		    offset, UIO_SYSSPACE,
		    IO_NODELOCKED|IO_UNIT, cred, NULL, p);
		if (error)
			return (error);

		offset += sizeof(phdr);
		secoff += phdr.p_filesz;
	}

	/* Write out the PT_NOTE header. */
	phdr.p_type = PT_NOTE;
	phdr.p_offset = notestart;
	phdr.p_vaddr = 0;
	phdr.p_paddr = 0;
	phdr.p_filesz = notesize;
	phdr.p_memsz = 0;
	phdr.p_flags = PF_R;
	phdr.p_align = ELFROUNDSIZE;

	error = vn_rdwr(UIO_WRITE, vp,
	    (caddr_t)&phdr, sizeof(phdr),
	    offset, UIO_SYSSPACE,
	    IO_NODELOCKED|IO_UNIT, cred, NULL, p);
	if (error)
		return (error);

	offset += sizeof(phdr);

	KASSERT(offset == notestart);

	/* Write out the notes. */
	error = ELFNAMEEND(coredump_notes)(p, vp, cred, &notesize, offset);
	if (error)
		return (error);

	offset += notesize;

	/* ...and write out the sections. */
	secoff = secstart;
	for (entry = map->header.next; entry != &map->header;
	     entry = entry->next) {
		start = entry->start;
		end = entry->end;

		if (start >= VM_MAXUSER_ADDRESS)
			continue;

		if (end > VM_MAXUSER_ADDRESS)
			end = VM_MAXUSER_ADDRESS;

		if (start >= (vaddr_t)vm->vm_maxsaddr) {
			if (end <= maxstack)
				continue;
			if (start < maxstack)
				start = maxstack; 
		}

		size = end - start;

		if ((entry->protection & VM_PROT_WRITE) == 0) {
			/* Not actually written out. */
			continue;
		}

		error = vn_rdwr(UIO_WRITE, vp,
		    (caddr_t)start, size,
		    secoff, UIO_USERSPACE,
		    IO_NODELOCKED|IO_UNIT, cred, NULL, p);
		if (error)
			return (error);

		secoff += size;
	}

	return (0);
}

int
ELFNAMEEND(coredump_notes)(struct proc *p, struct vnode *vp,
    struct ucred *cred, int *sizep, off_t offset)
{
	struct netbsd_elfcore_procinfo cpi;
	Elf_Nhdr nhdr;
	int size, notesize, error;
	char name[64];
	int namesize;
	struct reg intreg;
#ifdef PT_GETFPREGS
	struct fpreg freg;
#endif

	size = 0;

	/* First, write an elfcore_procinfo. */
	notesize = sizeof(nhdr) + elfround(sizeof(ELF_NOTE_NETBSD_CORE_NAME)) +
	    elfround(sizeof(cpi));
	if (offset) {
		cpi.cpi_version = NETBSD_ELFCORE_PROCINFO_VERSION;
		cpi.cpi_cpisize = sizeof(cpi);
		cpi.cpi_signo = p->p_sigctx.ps_sig;
		cpi.cpi_sigcode = p->p_sigctx.ps_code;

		memcpy(&cpi.cpi_sigpend, &p->p_sigctx.ps_siglist,
		    sizeof(cpi.cpi_sigpend));
		memcpy(&cpi.cpi_sigmask, &p->p_sigctx.ps_sigmask,
		    sizeof(cpi.cpi_sigmask));
		memcpy(&cpi.cpi_sigignore, &p->p_sigctx.ps_sigignore,
		    sizeof(cpi.cpi_sigignore));
		memcpy(&cpi.cpi_sigcatch, &p->p_sigctx.ps_sigcatch,
		    sizeof(cpi.cpi_sigcatch));

		cpi.cpi_pid = p->p_pid;
		cpi.cpi_ppid = p->p_pptr->p_pid;
		cpi.cpi_pgrp = p->p_pgid;
		cpi.cpi_sid = p->p_session->s_sid;

		cpi.cpi_ruid = p->p_cred->p_ruid;
		cpi.cpi_euid = p->p_ucred->cr_uid;
		cpi.cpi_svuid = p->p_cred->p_svuid;

		cpi.cpi_rgid = p->p_cred->p_rgid;
		cpi.cpi_egid = p->p_ucred->cr_gid;
		cpi.cpi_svgid = p->p_cred->p_svgid;

		cpi.cpi_nlwps = 1;	/* XXX for now */
		strcpy(cpi.cpi_name, p->p_comm);

		nhdr.n_namesz = sizeof(ELF_NOTE_NETBSD_CORE_NAME);
		nhdr.n_descsz = sizeof(cpi);
		nhdr.n_type = ELF_NOTE_NETBSD_CORE_PROCINFO;

		error = ELFNAMEEND(coredump_writenote)(p, vp, cred, offset,
		    &nhdr, ELF_NOTE_NETBSD_CORE_NAME, &cpi);
		if (error)
			return (error);

		offset += notesize;
	}

	size += notesize;

	/* XXX Add hook for machdep per-proc notes. */

	/*
	 * Now, for each LWP, write the register info and any other
	 * per-LWP notes.
	 */
	do {
		/* XXX Only one LWP for now. */
		sprintf(name, "%s@%d", ELF_NOTE_NETBSD_CORE_NAME, 1);
		namesize = strlen(name) + 1;

		notesize = sizeof(nhdr) + elfround(namesize) +
		    elfround(sizeof(intreg));
		if (offset) {
			error = process_read_regs(p, &intreg);
			if (error)
				return (error);

			nhdr.n_namesz = namesize;
			nhdr.n_descsz = sizeof(intreg);
			nhdr.n_type = PT_GETREGS;

			error = ELFNAMEEND(coredump_writenote)(p, vp, cred,
			    offset, &nhdr, name, &intreg);
			if (error)
				return (error);

			offset += notesize;
		}
		size += notesize;

#ifdef PT_GETFPREGS
		notesize = sizeof(nhdr) + elfround(namesize) +
		    elfround(sizeof(freg));
		if (offset) {
			error = process_read_fpregs(p, &freg);
			if (error)
				return (error);

			nhdr.n_namesz = namesize;
			nhdr.n_descsz = sizeof(freg);
			nhdr.n_type = PT_GETFPREGS;

			error = ELFNAMEEND(coredump_writenote)(p, vp, cred,
			    offset, &nhdr, name, &freg);
			if (error)
				return (error);

			offset += notesize;
		}
		size += notesize;
#endif
		/* XXX Add hook for machdep per-LWP notes. */
	} while (/*CONSTCOND*/0);

	*sizep = size;
	return (0);
}

int
ELFNAMEEND(coredump_writenote)(struct proc *p, struct vnode *vp,
    struct ucred *cred, off_t offset, Elf_Nhdr *nhdr, const char *name,
    void *data)
{
	int error;

	error = vn_rdwr(UIO_WRITE, vp,
	    (caddr_t) nhdr, sizeof(*nhdr),
	    offset, UIO_SYSSPACE,
	    IO_NODELOCKED|IO_UNIT, cred, NULL, p);
	if (error)
		return (error);

	offset += sizeof(*nhdr);

	error = vn_rdwr(UIO_WRITE, vp,
	    (caddr_t)name, nhdr->n_namesz,
	    offset, UIO_SYSSPACE,
	    IO_NODELOCKED|IO_UNIT, cred, NULL, p);
	if (error)
		return (error);

	offset += elfround(nhdr->n_namesz);

	error = vn_rdwr(UIO_WRITE, vp,
	    data, nhdr->n_descsz,
	    offset, UIO_SYSSPACE,
	    IO_NODELOCKED|IO_UNIT, cred, NULL, p);

	return (error);
}

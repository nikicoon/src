/* $NetBSD: dbsym.c,v 1.1 2001/11/09 05:52:21 thorpej Exp $ */

/*
 * Copyright (c) 2001 Simon Burge (for Wasabi Systems)
 * Copyright (c) 1996 Christopher G. Demetriou
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
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
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
 * 
 * <<Id: LICENSE_GC,v 1.1 2001/10/01 23:24:05 cgd Exp>>
 */

#include <sys/cdefs.h>
#ifndef lint
__COPYRIGHT(
    "@(#) Copyright (c) 1996 Christopher G. Demetriou, 2001 Simon Burge.\
  All rights reserved.\n");
#endif /* not lint */

#ifndef lint
__RCSID("$NetBSD: dbsym.c,v 1.1 2001/11/09 05:52:21 thorpej Exp $");
#endif /* not lint */

#include <sys/param.h>
#include <sys/exec_elf.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <bfd.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct symbols {
	char *name;
	bfd_vma vma;
	size_t offset;
} db_symtab_symbols[] = {
#define	X_DB_SYMTAB	0
	{ "_db_symtab", NULL, 0 },
#define	X_DB_SYMTABSIZE	1
	{ "_db_symtabsize", NULL, 0 },
	{ NULL }
};

int	main(int, char **);
void	usage(void) __attribute__((noreturn));
int	find_symtab(bfd *, struct symbols *);
int	load_symtab(int fd, char **, u_int32_t *);

int	verbose;

int
main(int argc, char **argv)
{
	int ch, kfd;
	struct stat ksb;
	size_t symtab_offset;
	u_int32_t symtab_space, symtabsize;
	const char *kfile;
	char *bfdname, *mappedkfile, *symtab;
	bfd *abfd;

	setprogname(argv[0]);

	bfdname = NULL;
	while ((ch = getopt(argc, argv, "b:v")) != -1)
		switch (ch) {
		case 'b':
			bfdname = optarg;
			break;
		case 'v':
			verbose = 1;
			break;
		case '?':
		default:
			usage();
	}
	argc -= optind;
	argv += optind;

	if (argc != 1)
		usage();
	kfile = argv[0];

	if ((kfd = open(kfile, O_RDWR, 0))  == -1)
		err(1, "open %s", kfile);

	bfd_init();
	if ((abfd = bfd_fdopenr(kfile, bfdname, kfd)) == NULL) {
		bfd_perror("open");
		exit(1);
	}
	if (!bfd_check_format(abfd, bfd_object)) {
		bfd_perror("check format");
		exit(1);
	}

	if (!(bfd_get_file_flags(abfd) & HAS_SYMS))
		errx(1, "no symbol table in %s", kfile);

	if (find_symtab(abfd, db_symtab_symbols) != 0)
		errx(1, "could not find symbols in %s", kfile);
	if (verbose)
		fprintf(stderr, "got symtab symbols from %s\n", kfile);

	if (load_symtab(kfd, &symtab, &symtabsize) != 0)
		errx(1, "could not load symbol table from %s", kfile);
	if (verbose)
		fprintf(stderr, "loaded symbol table from %s\n", kfile);

	if (fstat(kfd, &ksb) == -1)
		err(1, "fstat %s", kfile);
	if (ksb.st_size != (size_t)ksb.st_size)
		errx(1, "%s too big to map", kfile);

	if ((mappedkfile = mmap(NULL, ksb.st_size, PROT_READ | PROT_WRITE,
	    MAP_FILE | MAP_SHARED, kfd, 0)) == (caddr_t)-1)
		err(1, "mmap %s", kfile);
	if (verbose)
		fprintf(stderr, "mapped %s\n", kfile);

	symtab_offset = db_symtab_symbols[X_DB_SYMTAB].offset;
	symtab_space = bfd_get_32(abfd,
	    &mappedkfile[db_symtab_symbols[X_DB_SYMTABSIZE].offset]);

	if (symtabsize > symtab_space)
		errx(1, "symbol table (%u bytes) too big for buffer (%u bytes)",
		    symtabsize, symtab_space);

	if (verbose)
		fprintf(stderr, "symtab size %d, space available %d\n",
		    symtabsize, symtab_space);

	memcpy(mappedkfile + symtab_offset, symtab, symtabsize);

	if (verbose)
		fprintf(stderr, "done copying image to file offset %#lx\n",
		    (long)db_symtab_symbols[X_DB_SYMTAB].offset);
	
	bfd_put_32(abfd, symtabsize,
	    &mappedkfile[db_symtab_symbols[X_DB_SYMTABSIZE].offset]);

	munmap(mappedkfile, ksb.st_size);
	close(kfd);

	if (verbose)
		fprintf(stderr, "exiting\n");

	bfd_close(abfd);
	exit(0);
}

void
usage(void)
{
	const char **list;

	fprintf(stderr,
	    "usage: %s [-b bfdname] [-v] kernel_file\n",
	    getprogname());
	fprintf(stderr, "supported targets:");
	for (list = bfd_target_list(); *list != NULL; list++)
		fprintf(stderr, " %s", *list);
	fprintf(stderr, "\n");
	exit(1);
}

int
find_symtab(bfd *abfd, struct symbols *symbols)
{
	long i;
	long storage_needed;
	long number_of_symbols;
	asymbol **symbol_table = NULL;
	struct symbols *s;
	struct sec *p;

	storage_needed = bfd_get_symtab_upper_bound(abfd);
	if (storage_needed <= 0)
		return (1);

	if ((symbol_table = (asymbol **)malloc(storage_needed)) == NULL)
		return (1);

	number_of_symbols = bfd_canonicalize_symtab(abfd, symbol_table);
	if (number_of_symbols <= 0) {
		free(symbol_table);
		return (1);
	}

	for (i = 0; i < number_of_symbols; i++) {
		for (s = symbols; s->name != NULL; s++) {
			const char *sym = bfd_asymbol_name(symbol_table[i]);

			/*
			 * match symbol prefix '_' or ''.
			 * XXX: use bfd_get_symbol_leading_char() here?
			 */
			if (!strcmp(s->name, sym) ||
			    !strcmp(s->name + 1, sym)) {
				s->vma = bfd_asymbol_value(symbol_table[i]);
			}
		}
	}

	free(symbol_table);

	for (s = symbols; s->name != NULL; s++) {
		if (s->vma == NULL)
			return (1);

		for (p = abfd->sections; p != NULL; p = p->next) {
			if (p->vma > s->vma || p->vma + p->_raw_size < s->vma)
				continue;

			s->offset = (size_t)(p->filepos + (s->vma - p->vma));
		}

		if (s->offset == 0)
			return (1);
	}

	return (0);
}

/* --------------------------- ELF gunk follows --------------------------- */

/*
 * The format of the symbols loaded by the boot program is:
 *
 *      Elf exec header
 *      first section header
 *      . . .
 *      . . .
 *      last section header
 *      first symbol or string table section
 *      . . .
 *      . . .
 *      last symbol or string table section
 */


/* Note elftype is local to load_symtab()... */
#define	ELF_TYPE_64	0x01
#define	ELF_TYPE_BE	0x02
#define	ISELF64		(elftype & ELF_TYPE_64)
#define	ISELFBE		(elftype & ELF_TYPE_BE)

/*
 * Field sizes for the Elf exec header:
 *
 *    ELF32    ELF64
 *
 *    unsigned char      e_ident[ELF_NIDENT];    # Id bytes
 *     16       16       e_type;                 # file type
 *     16       16       e_machine;              # machine type
 *     32       32       e_version;              # version number
 *     32       64       e_entry;                # entry point
 *     32       64       e_phoff;                # Program hdr offset
 *     32       64       e_shoff;                # Section hdr offset
 *     32       32       e_flags;                # Processor flags
 *     16       16       e_ehsize;               # sizeof ehdr
 *     16       16       e_phentsize;            # Program header entry size
 *     16       16       e_phnum;                # Number of program headers
 *     16       16       e_shentsize;            # Section header entry size
 *     16       16       e_shnum;                # Number of section headers
 *     16       16       e_shstrndx;             # String table index
 */

typedef union {
	Elf32_Ehdr e32hdr;
	Elf64_Ehdr e64hdr;
	char e_ident[ELF_NIDENT];
} elf_ehdr;

#define	e32_hdr	ehdr.e32hdr
#define	e64_hdr	ehdr.e64hdr

/*
 * Field sizes for Elf section headers
 *
 *    ELF32    ELF64
 *
 *     32       32       sh_name;        # section name (.shstrtab index)
 *     32       32       sh_type;        # section type
 *     32       64       sh_flags;       # section flags
 *     32       64       sh_addr;        # virtual address
 *     32       64       sh_offset;      # file offset
 *     32       64       sh_size;        # section size
 *     32       32       sh_link;        # link to another
 *     32       32       sh_info;        # misc info
 *     32       64       sh_addralign;   # memory alignment
 *     32       64       sh_entsize;     # table entry size
 */

/* Extract a 32 bit field from Elf32_Shdr */
#define	SH_E32_32(x, n)	\
	(ISELFBE ? be32toh(s32hdr[(x)].n) : le32toh(s32hdr[(x)].n))
/* Extract a 32 bit field from Elf64_Shdr */
#define	SH_E64_32(x, n)	\
	(ISELFBE ? be32toh(s64hdr[(x)].n) : le32toh(s64hdr[(x)].n))
/* Extract a 64 bit field from Elf64_Shdr */
#define	SH_E64_64(x, n)	\
	(ISELFBE ? be64toh(s64hdr[(x)].n) : le64toh(s64hdr[(x)].n))

/* Extract a 32 bit field from either size Shdr */
#define	SH_E32E32(x, n)	(ISELF64 ? SH_E64_32(x, n) : SH_E32_32(x, n))

/* Extract a 32 bit field from Elf32_Shdr or 64 bit field from Elf64_Shdr */
#define	SH_E32E64(x, n)	(ISELF64 ? SH_E64_64(x, n) : SH_E32_32(x, n))

#define	SH_NAME(x)	SH_E32E32(x, sh_name)
#define	SH_TYPE(x)	SH_E32E32(x, sh_type)
#define	SH_FLAGS(x)	SH_E32E64(x, sh_flags)
#define	SH_ADDR(x)	SH_E32E64(x, sh_addr)
#define	SH_OFFSET(x)	SH_E32E64(x, sh_offset)
#define	SH_SIZE(x)	SH_E32E64(x, sh_size)
#define	SH_LINK(x)	SH_E32E32(x, sh_link)
#define	SH_INFO(x)	SH_E32E32(x, sh_info)
#define	SH_ADDRALIGN(x)	SH_E32E64(x, sh_addralign)
#define	SH_ENTSIZE(x)	SH_E32E64(x, sh_entsize)

int
load_symtab(int fd, char **symtab, u_int32_t *symtabsize)
{
	elf_ehdr ehdr;
	Elf32_Shdr *s32hdr;
	Elf64_Shdr *s64hdr;
	void *shdr;
	u_int32_t osymtabsize, sh_offset;
	int elftype, e_shnum, i, sh_size;
	off_t e_shoff;

	if (lseek(fd, 0, SEEK_SET) < 0)
		return (1);
	if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr))
		return (1);

	/*
	 * Check that we are targetting an Elf binary.
	 */
	if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0)
		return (1);

	/*
	 * Determine Elf size and endianness.
	 */
	elftype = 0;
	if (ehdr.e_ident[EI_CLASS] == ELFCLASS64)
		elftype |= ELF_TYPE_64;
	if (ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
		elftype |= ELF_TYPE_BE;

	/*
	 * Elf exec header.  Only need to allocate space for now,
	 * the header is copied into place at the end.
	 */
	*symtabsize = ISELF64 ? sizeof(Elf64_Ehdr) : sizeof(Elf32_Ehdr);
	*symtab = NULL;

	/*
	 * Section headers.  Allocate a temporary copy that will
	 * be copied into place at the end.
	 */
	sh_offset = osymtabsize = *symtabsize;
	e_shnum = (ISELF64
	    ? (ISELFBE ?  be16toh(e64_hdr.e_shnum) : le16toh(e64_hdr.e_shnum))
	    : (ISELFBE ?  be16toh(e32_hdr.e_shnum) : le16toh(e32_hdr.e_shnum)));
	sh_size = e_shnum * (ISELF64 ? sizeof(Elf64_Shdr) : sizeof(Elf32_Shdr));
	if ((shdr = malloc(sh_size)) == NULL)
		return (1);
	if (ISELF64)
		s64hdr = shdr;
	else 
		s32hdr = shdr;

	*symtabsize += roundup(sh_size, ISELF64 ? 8 : 4);

	e_shoff = (ISELF64
	   ? (ISELFBE ? be64toh(e64_hdr.e_shoff) : le64toh(e64_hdr.e_shoff))
	   : (ISELFBE ? be32toh(e32_hdr.e_shoff) : le32toh(e32_hdr.e_shoff)));
	if (lseek(fd, e_shoff, SEEK_SET) < 0)
		return (1);
	if (read(fd, shdr, sh_size) != sh_size)
		return (1);

	for (i = 0; i < e_shnum; i++) {
		if (SH_TYPE(i) == SHT_SYMTAB || SH_TYPE(i) == SHT_STRTAB) {
			osymtabsize = *symtabsize;
			*symtabsize += roundup(SH_SIZE(i), ISELF64 ? 8 : 4);
			if ((*symtab = realloc(*symtab, *symtabsize)) == NULL)
				return (1);

			if (lseek(fd, SH_OFFSET(i), SEEK_SET) < 0)
				return (1);
			if (read(fd, *symtab + osymtabsize, SH_SIZE(i)) !=
			    SH_SIZE(i))
				return (1);
			if (ISELF64) {
				if (ISELFBE)
					s64hdr[i].sh_offset =
					    htobe64(osymtabsize);
				else
					s64hdr[i].sh_offset =
					    htole64(osymtabsize);
			} else {
				if (ISELFBE)
					s32hdr[i].sh_offset =
					    htobe32(osymtabsize);
				else
					s32hdr[i].sh_offset =
					    htole32(osymtabsize);
			}
		}
	}

	/*
	 * Copy updated section headers.
	 */
	memcpy(*symtab + sh_offset, shdr, sh_size);

	/*
	 * Update and copy the exec header.
	 */
	if (ISELF64) {
		e64_hdr.e_phoff = 0;
		e64_hdr.e_shoff = (ISELFBE ?
		    htobe64(sizeof(Elf64_Ehdr)) : htole64(sizeof(Elf64_Ehdr)));
		e64_hdr.e_phentsize = 0;
		e64_hdr.e_phnum = 0;
	} else {
		e32_hdr.e_phoff = 0;
		e32_hdr.e_shoff = (ISELFBE ?
		    htobe32(sizeof(Elf32_Ehdr)) : htole32(sizeof(Elf32_Ehdr)));
		e32_hdr.e_phentsize = 0;
		e32_hdr.e_phnum = 0;
	}
	memcpy(*symtab, &ehdr, sizeof(ehdr));

	return (0);
}

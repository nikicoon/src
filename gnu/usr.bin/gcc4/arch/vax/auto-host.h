/* This file is automatically generated.  DO NOT EDIT! */
/* Generated from: 	NetBSD: mknative-gcc,v 1.22 2006/06/25 03:06:15 mrg Exp  */
/* Generated from: NetBSD: mknative.common,v 1.8 2006/05/26 19:17:21 mrg Exp  */

/* auto-host.h.  Generated by configure.  */
/* config.in.  Generated from configure.ac by autoheader.  */

/* Define as the number of bits in a byte, if \`limits.h' doesn't. */
#ifndef USED_FOR_TARGET
/* #undef CHAR_BIT */
#endif


/* Define 0/1 to force the choice for exception handling model. */
#ifndef USED_FOR_TARGET
/* #undef CONFIG_SJLJ_EXCEPTIONS */
#endif


/* Define to enable the use of a default assembler. */
#ifndef USED_FOR_TARGET
/* #undef DEFAULT_ASSEMBLER */
#endif


/* Define to enable the use of a default linker. */
#ifndef USED_FOR_TARGET
/* #undef DEFAULT_LINKER */
#endif


/* Define if you want to use __cxa_atexit, rather than atexit, to register C++
   destructors for local statics and global objects. This is essential for
   fully standards-compliant handling of destructors, but requires
   __cxa_atexit in libc. */
#ifndef USED_FOR_TARGET
/* #undef DEFAULT_USE_CXA_ATEXIT */
#endif


/* Define if you want assertions enabled. This is a cheap check. */
#ifndef USED_FOR_TARGET
#define ENABLE_ASSERT_CHECKING 1
#endif


/* Define if you want more run-time sanity checks. This one gets a grab bag of
   miscellaneous but relatively cheap checks. */
#ifndef USED_FOR_TARGET
/* #undef ENABLE_CHECKING */
#endif


/* Define if you want fold checked that it never destructs its argument. This
   is quite expensive. */
#ifndef USED_FOR_TARGET
/* #undef ENABLE_FOLD_CHECKING */
#endif


/* Define if you want the garbage collector to operate in maximally paranoid
   mode, validating the entire heap and collecting garbage at every
   opportunity. This is extremely expensive. */
#ifndef USED_FOR_TARGET
/* #undef ENABLE_GC_ALWAYS_COLLECT */
#endif


/* Define if you want the garbage collector to do object poisoning and other
   memory allocation checks. This is quite expensive. */
#ifndef USED_FOR_TARGET
/* #undef ENABLE_GC_CHECKING */
#endif


/* Define to 1 if translation of program messages to the user's native
   language is requested. */
#ifndef USED_FOR_TARGET
/* #undef ENABLE_NLS */
#endif


/* Define if you want all operations on RTL (the basic data structure of the
   optimizer and back end) to be checked for dynamic type safety at runtime.
   This is quite expensive. */
#ifndef USED_FOR_TARGET
/* #undef ENABLE_RTL_CHECKING */
#endif


/* Define if you want RTL flag accesses to be checked against the RTL codes
   that are supported for each access macro. This is relatively cheap. */
#ifndef USED_FOR_TARGET
/* #undef ENABLE_RTL_FLAG_CHECKING */
#endif


/* Define if you want runtime assertions enabled. This is a cheap check. */
#define ENABLE_RUNTIME_CHECKING 1

/* Define if you want all operations on trees (the basic data structure of the
   front ends) to be checked for dynamic type safety at runtime. This is
   moderately expensive. The tree browser debugging routines will also be
   enabled by this option. */
#ifndef USED_FOR_TARGET
/* #undef ENABLE_TREE_CHECKING */
#endif


/* Define if you want to run subprograms and generated programs through
   valgrind (a memory checker). This is extremely expensive. */
#ifndef USED_FOR_TARGET
/* #undef ENABLE_VALGRIND_CHECKING */
#endif


/* Define to 1 if installation paths should be looked up in the Windows
   Registry. Ignored on non-Windows hosts. */
#ifndef USED_FOR_TARGET
/* #undef ENABLE_WIN32_REGISTRY */
#endif


/* Define to the name of a file containing a list of extra machine modes for
   this architecture. */
#ifndef USED_FOR_TARGET
#define EXTRA_MODES_FILE "config/vax/vax-modes.def"
#endif


/* Define to enable detailed memory allocation stats gathering. */
#ifndef USED_FOR_TARGET
/* #undef GATHER_STATISTICS */
#endif


/* Define to the type of elements in the array set by `getgroups'. Usually
   this is either `int' or `gid_t'. */
#ifndef USED_FOR_TARGET
#define GETGROUPS_T gid_t
#endif


/* Define if the zone collector is in use */
#ifndef USED_FOR_TARGET
/* #undef GGC_ZONE */
#endif


/* Define to 1 if you have the `alphasort' function. */
#ifndef USED_FOR_TARGET
#define HAVE_ALPHASORT 1
#endif


/* Define if your assembler supports dwarf2 .file/.loc directives, and
   preserves file table indices exactly as given. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_DWARF2_DEBUG_LINE */
#endif


/* Define if your assembler supports explicit relocations. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_EXPLICIT_RELOCS */
#endif


/* Define if your assembler supports fprnd. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_FPRND */
#endif


/* Define if your assembler supports the --gdwarf2 option. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_GDWARF2_DEBUG_FLAG */
#endif


/* Define true if the assembler supports '.long foo@GOTOFF'. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_GOTOFF_IN_DATA */
#endif


/* Define if your assembler supports the --gstabs option. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_GSTABS_DEBUG_FLAG */
#endif


/* Define if your assembler supports the Sun syntax for cmov. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_IX86_CMOV_SUN_SYNTAX */
#endif


/* Define if your assembler supports the lituse_jsrdirect relocation. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_JSRDIRECT_RELOCS */
#endif


/* Define if your assembler supports .sleb128 and .uleb128. */
#ifndef USED_FOR_TARGET
#define HAVE_AS_LEB128 1
#endif


/* Define if your assembler supports ltoffx and ldxmov relocations. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_LTOFFX_LDXMOV_RELOCS */
#endif


/* Define if your assembler supports mfcr field. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_MFCRF */
#endif


/* Define if your assembler supports the -no-mul-bug-abort option. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_NO_MUL_BUG_ABORT_OPTION */
#endif


/* Define if your assembler supports offsetable %lo(). */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_OFFSETABLE_LO10 */
#endif


/* Define if your assembler supports popcntb field. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_POPCNTB */
#endif


/* Define if your assembler supports .register. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_REGISTER_PSEUDO_OP */
#endif


/* Define if your assembler supports R_PPC_REL16 relocs. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_REL16 */
#endif


/* Define if your assembler supports -relax option. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_RELAX_OPTION */
#endif


/* Define if your assembler and linker support unaligned PC relative relocs.
   */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_SPARC_UA_PCREL */
#endif


/* Define if your assembler and linker support unaligned PC relative relocs
   against hidden symbols. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_SPARC_UA_PCREL_HIDDEN */
#endif


/* Define if your assembler supports thread-local storage. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_AS_TLS */
#endif


/* Define to 1 if you have the `atoll' function. */
#ifndef USED_FOR_TARGET
#define HAVE_ATOLL 1
#endif


/* Define to 1 if you have the `atoq' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_ATOQ */
#endif


/* Define to 1 if you have the `clearerr_unlocked' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_CLEARERR_UNLOCKED */
#endif


/* Define to 1 if you have the `clock' function. */
#ifndef USED_FOR_TARGET
#define HAVE_CLOCK 1
#endif


/* Define if <time.h> defines clock_t. */
#ifndef USED_FOR_TARGET
#define HAVE_CLOCK_T 1
#endif


/* Define 0/1 if your assembler and linker support COMDAT groups. */
#ifndef USED_FOR_TARGET
#define HAVE_COMDAT_GROUP 0
#endif


/* Define to 1 if we found a declaration for 'abort', otherwise define to 0.
   */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_ABORT 1
#endif


/* Define to 1 if we found a declaration for 'asprintf', otherwise define to
   0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_ASPRINTF 1
#endif


/* Define to 1 if we found a declaration for 'atof', otherwise define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_ATOF 1
#endif


/* Define to 1 if we found a declaration for 'atol', otherwise define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_ATOL 1
#endif


/* Define to 1 if we found a declaration for 'basename', otherwise define to
   0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_BASENAME 0
#endif


/* Define to 1 if we found a declaration for 'calloc', otherwise define to 0.
   */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_CALLOC 1
#endif


/* Define to 1 if we found a declaration for 'clearerr_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_CLEARERR_UNLOCKED 0
#endif


/* Define to 1 if we found a declaration for 'clock', otherwise define to 0.
   */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_CLOCK 1
#endif


/* Define to 1 if we found a declaration for 'errno', otherwise define to 0.
   */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_ERRNO 1
#endif


/* Define to 1 if we found a declaration for 'feof_unlocked', otherwise define
   to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_FEOF_UNLOCKED 0
#endif


/* Define to 1 if we found a declaration for 'ferror_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_FERROR_UNLOCKED 0
#endif


/* Define to 1 if we found a declaration for 'fflush_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_FFLUSH_UNLOCKED 0
#endif


/* Define to 1 if we found a declaration for 'fgetc_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_FGETC_UNLOCKED 0
#endif


/* Define to 1 if we found a declaration for 'fgets_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_FGETS_UNLOCKED 0
#endif


/* Define to 1 if we found a declaration for 'fileno_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_FILENO_UNLOCKED 0
#endif


/* Define to 1 if we found a declaration for 'fprintf_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_FPRINTF_UNLOCKED 0
#endif


/* Define to 1 if we found a declaration for 'fputc_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_FPUTC_UNLOCKED 0
#endif


/* Define to 1 if we found a declaration for 'fputs_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_FPUTS_UNLOCKED 0
#endif


/* Define to 1 if we found a declaration for 'fread_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_FREAD_UNLOCKED 0
#endif


/* Define to 1 if we found a declaration for 'free', otherwise define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_FREE 1
#endif


/* Define to 1 if we found a declaration for 'fwrite_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_FWRITE_UNLOCKED 0
#endif


/* Define to 1 if we found a declaration for 'getchar_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_GETCHAR_UNLOCKED 1
#endif


/* Define to 1 if we found a declaration for 'getcwd', otherwise define to 0.
   */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_GETCWD 1
#endif


/* Define to 1 if we found a declaration for 'getc_unlocked', otherwise define
   to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_GETC_UNLOCKED 1
#endif


/* Define to 1 if we found a declaration for 'getenv', otherwise define to 0.
   */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_GETENV 1
#endif


/* Define to 1 if we found a declaration for 'getopt', otherwise define to 0.
   */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_GETOPT 1
#endif


/* Define to 1 if we found a declaration for 'getpagesize', otherwise define
   to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_GETPAGESIZE 1
#endif


/* Define to 1 if we found a declaration for 'getrlimit', otherwise define to
   0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_GETRLIMIT 1
#endif


/* Define to 1 if we found a declaration for 'getrusage', otherwise define to
   0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_GETRUSAGE 1
#endif


/* Define to 1 if we found a declaration for 'getwd', otherwise define to 0.
   */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_GETWD 1
#endif


/* Define to 1 if we found a declaration for 'ldgetname', otherwise define to
   0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_LDGETNAME 0
#endif


/* Define to 1 if we found a declaration for 'malloc', otherwise define to 0.
   */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_MALLOC 1
#endif


/* Define to 1 if we found a declaration for 'putchar_unlocked', otherwise
   define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_PUTCHAR_UNLOCKED 1
#endif


/* Define to 1 if we found a declaration for 'putc_unlocked', otherwise define
   to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_PUTC_UNLOCKED 1
#endif


/* Define to 1 if we found a declaration for 'realloc', otherwise define to 0.
   */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_REALLOC 1
#endif


/* Define to 1 if we found a declaration for 'sbrk', otherwise define to 0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_SBRK 1
#endif


/* Define to 1 if we found a declaration for 'setrlimit', otherwise define to
   0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_SETRLIMIT 1
#endif


/* Define to 1 if we found a declaration for 'snprintf', otherwise define to
   0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_SNPRINTF 1
#endif


/* Define to 1 if we found a declaration for 'strsignal', otherwise define to
   0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_STRSIGNAL 1
#endif


/* Define to 1 if we found a declaration for 'strstr', otherwise define to 0.
   */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_STRSTR 1
#endif


/* Define to 1 if we found a declaration for 'strverscmp', otherwise define to
   0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_STRVERSCMP 0
#endif


/* Define to 1 if we found a declaration for 'times', otherwise define to 0.
   */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_TIMES 1
#endif


/* Define to 1 if we found a declaration for 'vasprintf', otherwise define to
   0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_VASPRINTF 1
#endif


/* Define to 1 if we found a declaration for 'vsnprintf', otherwise define to
   0. */
#ifndef USED_FOR_TARGET
#define HAVE_DECL_VSNPRINTF 1
#endif


/* Define to 1 if you have the <direct.h> header file. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_DIRECT_H */
#endif


/* Define to 1 if you have the <fcntl.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_FCNTL_H 1
#endif


/* Define to 1 if you have the `feof_unlocked' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_FEOF_UNLOCKED */
#endif


/* Define to 1 if you have the `ferror_unlocked' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_FERROR_UNLOCKED */
#endif


/* Define to 1 if you have the `fflush_unlocked' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_FFLUSH_UNLOCKED */
#endif


/* Define to 1 if you have the `fgetc_unlocked' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_FGETC_UNLOCKED */
#endif


/* Define to 1 if you have the `fgets_unlocked' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_FGETS_UNLOCKED */
#endif


/* Define to 1 if you have the `fileno_unlocked' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_FILENO_UNLOCKED */
#endif


/* Define to 1 if you have the `fork' function. */
#ifndef USED_FOR_TARGET
#define HAVE_FORK 1
#endif


/* Define to 1 if you have the `fprintf_unlocked' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_FPRINTF_UNLOCKED */
#endif


/* Define to 1 if you have the `fputc_unlocked' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_FPUTC_UNLOCKED */
#endif


/* Define to 1 if you have the `fputs_unlocked' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_FPUTS_UNLOCKED */
#endif


/* Define to 1 if you have the `fread_unlocked' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_FREAD_UNLOCKED */
#endif


/* Define to 1 if you have the `fwrite_unlocked' function. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_FWRITE_UNLOCKED */
#endif


/* Define if your assembler supports .balign and .p2align. */
#ifndef USED_FOR_TARGET
#define HAVE_GAS_BALIGN_AND_P2ALIGN 1
#endif


/* Define if your assembler uses the new HImode fild and fist notation. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_GAS_FILDS_FISTS */
#endif


/* Define if your assembler and linker support .hidden. */
#define HAVE_GAS_HIDDEN 1

/* Define if your assembler supports specifying the maximum number of bytes to
   skip when using the GAS .p2align command. */
#ifndef USED_FOR_TARGET
#define HAVE_GAS_MAX_SKIP_P2ALIGN 1
#endif


/* Define if your assembler supports .nsubspa comdat option. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_GAS_NSUBSPA_COMDAT */
#endif


/* Define if your assembler and linker support 32-bit section relative relocs
   via '.secrel32 label'. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_GAS_PE_SECREL32_RELOC */
#endif


/* Define 0/1 if your assembler supports marking sections with SHF_MERGE flag.
   */
#ifndef USED_FOR_TARGET
#define HAVE_GAS_SHF_MERGE 1
#endif


/* Define if your assembler supports .subsection and .subsection -1 starts
   emitting at the beginning of your section. */
#ifndef USED_FOR_TARGET
#define HAVE_GAS_SUBSECTION_ORDERING 1
#endif


/* Define if your assembler supports .weak. */
#ifndef USED_FOR_TARGET
#define HAVE_GAS_WEAK 1
#endif


/* Define if your assembler supports .weakref. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_GAS_WEAKREF */
#endif


/* Define to 1 if you have the `getchar_unlocked' function. */
#ifndef USED_FOR_TARGET
#define HAVE_GETCHAR_UNLOCKED 1
#endif


/* Define to 1 if you have the `getc_unlocked' function. */
#ifndef USED_FOR_TARGET
#define HAVE_GETC_UNLOCKED 1
#endif


/* Define to 1 if you have the `getrlimit' function. */
#ifndef USED_FOR_TARGET
#define HAVE_GETRLIMIT 1
#endif


/* Define to 1 if you have the `getrusage' function. */
#ifndef USED_FOR_TARGET
#define HAVE_GETRUSAGE 1
#endif


/* Define to 1 if you have the `gettimeofday' function. */
#ifndef USED_FOR_TARGET
#define HAVE_GETTIMEOFDAY 1
#endif


/* Define if you have the iconv() function. */
#ifndef USED_FOR_TARGET
#define HAVE_ICONV 1
#endif


/* Define to 1 if you have the <iconv.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_ICONV_H 1
#endif


/* Define .init_array/.fini_array sections are available and working. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_INITFINI_ARRAY */
#endif


/* Define if you have a working <inttypes.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_INTTYPES_H 1
#endif


/* Define to 1 if you have the `kill' function. */
#ifndef USED_FOR_TARGET
#define HAVE_KILL 1
#endif


/* Define if you have <langinfo.h> and nl_langinfo(CODESET). */
#ifndef USED_FOR_TARGET
#define HAVE_LANGINFO_CODESET 1
#endif


/* Define to 1 if you have the <langinfo.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_LANGINFO_H 1
#endif


/* Define if your <locale.h> file defines LC_MESSAGES. */
#ifndef USED_FOR_TARGET
#define HAVE_LC_MESSAGES 1
#endif


/* Define to 1 if you have the <ldfcn.h> header file. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_LDFCN_H */
#endif


/* Define if your linker supports --as-needed and --no-as-needed options. */
#ifndef USED_FOR_TARGET
#define HAVE_LD_AS_NEEDED 1
#endif


/* Define if your linker supports --demangle option. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_LD_DEMANGLE */
#endif


/* Define if your linker supports --eh-frame-hdr option. */
#define HAVE_LD_EH_FRAME_HDR 1

/* Define if your PowerPC64 linker only needs function descriptor syms. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_LD_NO_DOT_SYMS */
#endif


/* Define if your linker supports -pie option. */
#ifndef USED_FOR_TARGET
#define HAVE_LD_PIE 1
#endif


/* Define if your linker links a mix of read-only and read-write sections into
   a read-write section. */
#ifndef USED_FOR_TARGET
#define HAVE_LD_RO_RW_SECTION_MIXING 1
#endif


/* Define if your linker supports -Bstatic/-Bdynamic option. */
#ifndef USED_FOR_TARGET
#define HAVE_LD_STATIC_DYNAMIC 1
#endif


/* Define if your linker supports --sysroot. */
#ifndef USED_FOR_TARGET
#define HAVE_LD_SYSROOT 1
#endif


/* Define to 1 if you have the <limits.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_LIMITS_H 1
#endif


/* Define to 1 if you have the <locale.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_LOCALE_H 1
#endif


/* Define to 1 if the system has the type `long long'. */
#ifndef USED_FOR_TARGET
#define HAVE_LONG_LONG 1
#endif


/* Define to 1 if you have the <malloc.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_MALLOC_H 1
#endif


/* Define to 1 if you have the `mbstowcs' function. */
#ifndef USED_FOR_TARGET
#define HAVE_MBSTOWCS 1
#endif


/* Define if valgrind's memcheck.h header is installed. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_MEMCHECK_H */
#endif


/* Define to 1 if you have the <memory.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_MEMORY_H 1
#endif


/* Define to 1 if you have the `mincore' function. */
#ifndef USED_FOR_TARGET
#define HAVE_MINCORE 1
#endif


/* Define to 1 if you have the `mmap' function. */
#ifndef USED_FOR_TARGET
#define HAVE_MMAP 1
#endif


/* Define if mmap with MAP_ANON(YMOUS) works. */
#ifndef USED_FOR_TARGET
#define HAVE_MMAP_ANON 1
#endif


/* Define if mmap of /dev/zero works. */
#ifndef USED_FOR_TARGET
#define HAVE_MMAP_DEV_ZERO 1
#endif


/* Define if read-only mmap of a plain file works. */
#ifndef USED_FOR_TARGET
#define HAVE_MMAP_FILE 1
#endif


/* Define to 1 if you have the `nl_langinfo' function. */
#ifndef USED_FOR_TARGET
#define HAVE_NL_LANGINFO 1
#endif


/* Define to 1 if you have the `putchar_unlocked' function. */
#ifndef USED_FOR_TARGET
#define HAVE_PUTCHAR_UNLOCKED 1
#endif


/* Define to 1 if you have the `putc_unlocked' function. */
#ifndef USED_FOR_TARGET
#define HAVE_PUTC_UNLOCKED 1
#endif


/* Define to 1 if you have the `scandir' function. */
#ifndef USED_FOR_TARGET
#define HAVE_SCANDIR 1
#endif


/* Define to 1 if you have the `setlocale' function. */
#ifndef USED_FOR_TARGET
#define HAVE_SETLOCALE 1
#endif


/* Define to 1 if you have the `setrlimit' function. */
#ifndef USED_FOR_TARGET
#define HAVE_SETRLIMIT 1
#endif


/* Define to 1 if you have the <stddef.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_STDDEF_H 1
#endif


/* Define to 1 if you have the <stdint.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_STDINT_H 1
#endif


/* Define to 1 if you have the <stdlib.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_STDLIB_H 1
#endif


/* Define to 1 if you have the <strings.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_STRINGS_H 1
#endif


/* Define to 1 if you have the <string.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_STRING_H 1
#endif


/* Define to 1 if you have the `strsignal' function. */
#ifndef USED_FOR_TARGET
#define HAVE_STRSIGNAL 1
#endif


/* Define if <sys/times.h> defines struct tms. */
#ifndef USED_FOR_TARGET
#define HAVE_STRUCT_TMS 1
#endif


/* Define to 1 if you have the `sysconf' function. */
#ifndef USED_FOR_TARGET
#define HAVE_SYSCONF 1
#endif


/* Define to 1 if you have the <sys/file.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_SYS_FILE_H 1
#endif


/* Define to 1 if you have the <sys/mman.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_SYS_MMAN_H 1
#endif


/* Define to 1 if you have the <sys/param.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_SYS_PARAM_H 1
#endif


/* Define to 1 if you have the <sys/resource.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_SYS_RESOURCE_H 1
#endif


/* Define to 1 if you have the <sys/stat.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_SYS_STAT_H 1
#endif


/* Define to 1 if you have the <sys/times.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_SYS_TIMES_H 1
#endif


/* Define to 1 if you have the <sys/time.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_SYS_TIME_H 1
#endif


/* Define to 1 if you have the <sys/types.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_SYS_TYPES_H 1
#endif


/* Define to 1 if you have <sys/wait.h> that is POSIX.1 compatible. */
#ifndef USED_FOR_TARGET
#define HAVE_SYS_WAIT_H 1
#endif


/* Define to 1 if you have the `times' function. */
#ifndef USED_FOR_TARGET
#define HAVE_TIMES 1
#endif


/* Define to 1 if you have the <time.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_TIME_H 1
#endif


/* Define to 1 if you have the <unistd.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_UNISTD_H 1
#endif


/* Define if valgrind's valgrind/memcheck.h header is installed. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_VALGRIND_MEMCHECK_H */
#endif


/* Define to 1 if you have the `vfork' function. */
#ifndef USED_FOR_TARGET
#define HAVE_VFORK 1
#endif


/* Define to 1 if you have the <vfork.h> header file. */
#ifndef USED_FOR_TARGET
/* #undef HAVE_VFORK_H */
#endif


/* Define to 1 if you have the <wchar.h> header file. */
#ifndef USED_FOR_TARGET
#define HAVE_WCHAR_H 1
#endif


/* Define to 1 if you have the `wcswidth' function. */
#ifndef USED_FOR_TARGET
#define HAVE_WCSWIDTH 1
#endif


/* Define to 1 if `fork' works. */
#ifndef USED_FOR_TARGET
#define HAVE_WORKING_FORK 1
#endif


/* Define this macro if mbstowcs does not crash when its first argument is
   NULL. */
#ifndef USED_FOR_TARGET
#define HAVE_WORKING_MBSTOWCS 1
#endif


/* Define to 1 if `vfork' works. */
#ifndef USED_FOR_TARGET
#define HAVE_WORKING_VFORK 1
#endif


/* Define to 1 if the system has the type `__int64'. */
#ifndef USED_FOR_TARGET
/* #undef HAVE___INT64 */
#endif


/* Define as const if the declaration of iconv() needs const. */
#ifndef USED_FOR_TARGET
#define ICONV_CONST const
#endif


/* Define if host mkdir takes a single argument. */
#ifndef USED_FOR_TARGET
/* #undef MKDIR_TAKES_ONE_ARG */
#endif


/* Define to 1 if HOST_WIDE_INT must be 64 bits wide (see hwint.h). */
#ifndef USED_FOR_TARGET
/* #undef NEED_64BIT_HOST_WIDE_INT */
#endif


/* Define to 1 if your C compiler doesn't accept -c and -o together. */
#ifndef USED_FOR_TARGET
/* #undef NO_MINUS_C_MINUS_O */
#endif


/* Define to the address where bug reports for this package should be sent. */
#ifndef USED_FOR_TARGET
#define PACKAGE_BUGREPORT ""
#endif


/* Define to the full name of this package. */
#ifndef USED_FOR_TARGET
#define PACKAGE_NAME ""
#endif


/* Define to the full name and version of this package. */
#ifndef USED_FOR_TARGET
#define PACKAGE_STRING ""
#endif


/* Define to the one symbol short name of this package. */
#ifndef USED_FOR_TARGET
#define PACKAGE_TARNAME ""
#endif


/* Define to the version of this package. */
#ifndef USED_FOR_TARGET
#define PACKAGE_VERSION ""
#endif


/* Define to PREFIX/include if cpp should also search that directory. */
#ifndef USED_FOR_TARGET
#define PREFIX_INCLUDE_DIR "NONE/include"
#endif


/* The size of a `int', as computed by sizeof. */
#ifndef USED_FOR_TARGET
#define SIZEOF_INT 4
#endif


/* The size of a `long', as computed by sizeof. */
#ifndef USED_FOR_TARGET
#define SIZEOF_LONG 4
#endif


/* The size of a `long long', as computed by sizeof. */
#ifndef USED_FOR_TARGET
#define SIZEOF_LONG_LONG 8
#endif


/* The size of a `short', as computed by sizeof. */
#ifndef USED_FOR_TARGET
#define SIZEOF_SHORT 2
#endif


/* The size of a `void *', as computed by sizeof. */
#ifndef USED_FOR_TARGET
#define SIZEOF_VOID_P 4
#endif


/* The size of a `__int64', as computed by sizeof. */
#ifndef USED_FOR_TARGET
/* #undef SIZEOF___INT64 */
#endif


/* Define to 1 if you have the ANSI C header files. */
#ifndef USED_FOR_TARGET
#define STDC_HEADERS 1
#endif


/* Define if you can safely include both <string.h> and <strings.h>. */
#ifndef USED_FOR_TARGET
#define STRING_WITH_STRINGS 1
#endif


/* Define if your target C library provides stack protector support */
#ifndef USED_FOR_TARGET
/* #undef TARGET_LIBC_PROVIDES_SSP */
#endif


/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#ifndef USED_FOR_TARGET
#define TIME_WITH_SYS_TIME 1
#endif


/* Define if your assembler mis-optimizes .eh_frame data. */
#ifndef USED_FOR_TARGET
/* #undef USE_AS_TRADITIONAL_FORMAT */
#endif


/* Define to 1 if the 'long long' (or '__int64') is wider than 'long' but
   still efficiently supported by the host hardware. */
#ifndef USED_FOR_TARGET
/* #undef USE_LONG_LONG_FOR_WIDEST_FAST_INT */
#endif


/* Define if location_t is fileline integer cookie. */
#ifndef USED_FOR_TARGET
/* #undef USE_MAPPED_LOCATION */
#endif


/* Define to be the last component of the Windows registry key under which to
   look for installation paths. The full key used will be
   HKEY_LOCAL_MACHINE/SOFTWARE/Free Software Foundation/{WIN32_REGISTRY_KEY}.
   The default is the GCC version number. */
#ifndef USED_FOR_TARGET
/* #undef WIN32_REGISTRY_KEY */
#endif


/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
#ifndef USED_FOR_TARGET
/* #undef WORDS_BIGENDIAN */
#endif


/* Always define this when using the GNU C Library */
#ifndef USED_FOR_TARGET
/* #undef _GNU_SOURCE */
#endif


/* Define to `int' if <sys/types.h> doesn't define. */
#ifndef USED_FOR_TARGET
/* #undef gid_t */
#endif


/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `int' if <sys/types.h> does not define. */
#ifndef USED_FOR_TARGET
/* #undef pid_t */
#endif


/* Define to \`long' if <sys/resource.h> doesn't define. */
#ifndef USED_FOR_TARGET
/* #undef rlim_t */
#endif


/* Define to `int' if <sys/types.h> does not define. */
#ifndef USED_FOR_TARGET
/* #undef ssize_t */
#endif


/* Define to `int' if <sys/types.h> doesn't define. */
#ifndef USED_FOR_TARGET
/* #undef uid_t */
#endif


/* Define as `fork' if `vfork' does not work. */
#ifndef USED_FOR_TARGET
/* #undef vfork */
#endif


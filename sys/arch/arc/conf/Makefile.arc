#	$NetBSD: Makefile.arc,v 1.29 2000/01/23 20:08:55 soda Exp $

# Makefile for NetBSD
#
# This makefile is constructed from a machine description:
#	config machineid
# Most changes should be made in the machine description
#	/sys/arch/pica/conf/``machineid''
# after which you should do
#	config machineid
# Machine generic makefile changes should be made in
#	/sys/arch/pica/conf/Makefile.pica
# after which config should be rerun for all machines of that type.
#
# N.B.: NO DEPENDENCIES ON FOLLOWING FLAGS ARE VISIBLE TO MAKEFILE
#	IF YOU CHANGE THE DEFINITION OF ANY OF THESE RECOMPILE EVERYTHING
#
# -DTRACE	compile in kernel tracing hooks
# -DQUOTA	compile in file system quotas

# DEBUG is set to -g if debugging.
# PROF is set to -pg if profiling.

AR?=	ar
AS?=	as
CC?=	cc
CPP?=	cpp
LD?=	ld
LORDER?=lorder
MKDEP?=	mkdep
NM?=	nm
RANLIB?=ranlib
SIZE?=	size
STRIPPROG?=strip
TSORT?=	tsort -q

COPTS?=	-O2

# source tree is located via $S relative to the compilation directory
.ifndef S
#S!=	cd ../../../..; pwd
S=	../../../..
.endif
PICA=	$S/arch/pica
MIPS=	$S/arch/mips

HAVE_GCC28!=	${CC} --version | egrep "^(2\.8|egcs)" ; echo 
INCLUDES=	-I. -I$S/arch -I$S -nostdinc
CPPFLAGS=	${INCLUDES} ${IDENT} ${PARAM} -D_KERNEL -Dpica
CWARNFLAGS?=	-Wall -Wmissing-prototypes -Wstrict-prototypes -Wpointer-arith
.if (${HAVE_GCC28} != "")
CWARNFLAGS+=	-Wno-main
.endif
GP?=		-G 0
CFLAGS=		${DEBUG} ${COPTS} ${CWARNFLAGS} ${GP} \
		-mips2 -mcpu=r4000 -mno-abicalls -mno-half-pic
AFLAGS=		-x assembler-with-cpp -traditional-cpp -D_LOCORE
LINKFLAGS=	-T${MIPS}/conf/kern.ldscript -Ttext 80080000  -e start ${GP}
STRIPFLAGS=	-g -X -x

%INCLUDES

### find out what to use for libkern
.include "$S/lib/libkern/Makefile.inc"
.ifndef PROF
LIBKERN=	${KERNLIB}
.else
LIBKERN=	${KERNLIB_PROF}
.endif

### find out what to use for libcompat
.include "$S/compat/common/Makefile.inc"
.ifndef PROF
LIBCOMPAT=	${COMPATLIB}
.else
LIBCOMPAT=	${COMPATLIB_PROF}
.endif

# compile rules: rules are named ${TYPE}_${SUFFIX} where TYPE is NORMAL or
# HOSTED}, and SUFFIX is the file suffix, capitalized (e.g. C for a .c file).

NORMAL_C=	${CC} ${CFLAGS} ${CPPFLAGS} ${PROF} -c $<
NOPROF_C=	${CC} ${CFLAGS} ${CPPFLAGS} -c $<
NORMAL_S=	${CC} ${AFLAGS} ${CPPFLAGS} -c $<

%OBJS

%CFILES

%SFILES

# load lines for config "xxx" will be emitted as:
# xxx: ${SYSTEM_DEP} swapxxx.o
#	${SYSTEM_LD_HEAD}
#	${SYSTEM_LD} swapxxx.o
#	${SYSTEM_LD_TAIL}
SYSTEM_OBJ=	locore.o fp.o locore_machdep.o \
		param.o ioconf.o ${OBJS} ${LIBCOMPAT} ${LIBKERN}
SYSTEM_DEP=	Makefile ${SYSTEM_OBJ}
SYSTEM_LD_HEAD=	@rm -f $@
SYSTEM_LD=	@echo ${LD} ${LINKFLAGS} -o $@ '$${SYSTEM_OBJ}' vers.o ; \
		${LD} ${LINKFLAGS} -o $@ ${SYSTEM_OBJ} vers.o
SYSTEM_LD_TAIL=	@${SIZE} $@; chmod 755 $@

DEBUG?=
.if ${DEBUG} == "-g"
LINKFLAGS+=	-X
SYSTEM_LD_TAIL+=; \
		echo mv -f $@ $@.gdb; mv -f $@ $@.gdb; \
		echo ${STRIPPROG} ${STRIPFLAGS} -o $@ $@.gdb; \
		${STRIPPROG} ${STRIPFLAGS} -o $@ $@.gdb
.else
LINKFLAGS+=	-x
.endif

SYSTEM_LD_TAIL+=;\
		mv $@ $@.elf; \
		elf2aout $@.elf $@; \
		chmod 755 $@; \
		elf2ecoff $@.elf $@.ecoff

%LOAD

assym.h: $S/kern/genassym.sh ${MIPS}/mips/genassym.cf
	sh $S/kern/genassym.sh ${CC} ${CFLAGS} ${CPPFLAGS} ${PROF} \
	    < ${MIPS}/mips/genassym.cf > assym.h.tmp && \
	mv -f assym.h.tmp assym.h


param.c: $S/conf/param.c
	rm -f param.c
	cp $S/conf/param.c .

param.o: param.c Makefile
	${NORMAL_C}

ioconf.o: ioconf.c
	${NORMAL_C}

newvers: ${SYSTEM_DEP} ${SYSTEM_SWAP_DEP}
	sh $S/conf/newvers.sh
	${CC} ${CFLAGS} ${CPPFLAGS} ${PROF} -c vers.c

__CLEANKERNEL: .USE
	@echo "${.TARGET}ing the kernel objects"
	rm -f eddep *netbsd netbsd.gdb tags *.[io] [a-z]*.s \
	    [Ee]rrs linterrs makelinks assym.h.tmp assym.h

__CLEANDEPEND: .USE
	rm -f .depend

clean: __CLEANKERNEL

cleandir distclean: __CLEANKERNEL __CLEANDEPEND

lint:
	@lint -hbxncez -Dvolatile= ${CPPFLAGS} -UKGDB \
	    ${PICA}/pica/Locore.c ${CFILES} \
	    ioconf.c param.c | \
	    grep -v 'static function .* unused'

tags:
	@echo "see $S/kern/Makefile for tags"

links:
	egrep '#if' ${CFILES} | sed -f $S/conf/defines | \
	  sed -e 's/:.*//' -e 's/\.c/.o/' | sort -u > dontlink
	echo ${CFILES} | tr -s ' ' '\12' | sed 's/\.c/.o/' | \
	  sort -u | comm -23 - dontlink | \
	  sed 's,../.*/\(.*.o\),rm -f \1; ln -s ../GENERIC/\1 \1,' > makelinks
	sh makelinks && rm -f dontlink

SRCS=	${MIPS}/mips/locore.S ${MIPS}/mips/fp.S \
	${PICA}/pica/locore_machdep.S \
	param.c ioconf.c ${CFILES} ${SFILES}
depend: .depend
.depend: ${SRCS} assym.h param.c
	${MKDEP} ${AFLAGS} ${CPPFLAGS} ${MIPS}/mips/locore.S ${MIPS}/mips/fp.S
	${MKDEP} ${AFLAGS} ${CPPFLAGS} ${PICA}/pica/locore_machdep.S 
	${MKDEP} -a ${CFLAGS} ${CPPFLAGS} param.c ioconf.c ${CFILES}
	${MKDEP} -a ${AFLAGS} ${CPPFLAGS} ${SFILES}
	sh $S/kern/genassym.sh ${MKDEP} -f assym.dep ${CFLAGS} \
	  ${CPPFLAGS} < ${MIPS}/mips/genassym.cf
	@sed -e 's/.*\.o:.*\.c/assym.h:/' < assym.dep >> .depend
	@rm -f assym.dep


# depend on root or device configuration
autoconf.o conf.o: Makefile

# depend on network or filesystem configuration
uipc_proto.o vfs_conf.o: Makefile

# depend on maxusers
machdep.o: Makefile

# depend on CPU configuration
machdep.o clock.o: Makefile



locore.o: ${MIPS}/mips/locore.S ${MIPS}/mips/locore_r2000.S ${MIPS}/mips/locore_r4000.S assym.h
	${NORMAL_S}

fp.o: ${MIPS}/mips/fp.S assym.h
	${NORMAL_S}

locore_machdep.o: ${PICA}/pica/locore_machdep.S assym.h
	${NORMAL_S}

%RULES

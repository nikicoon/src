# This file is automatically generated.  DO NOT EDIT!
# Generated from: 	NetBSD: mknative-gcc,v 1.22 2006/06/25 03:06:15 mrg Exp 
# Generated from: NetBSD: mknative.common,v 1.8 2006/05/26 19:17:21 mrg Exp 
#
G_ALL_OPT_FILES=${GNUHOSTDIST}/gcc/c.opt ${GNUHOSTDIST}/gcc/common.opt ${GNUHOSTDIST}/gcc/config/mips/mips.opt
G_ALL_CFLAGS=-I. -I${GNUHOSTDIST}/libobjc   -O2   -W -Wall -Wwrite-strings -Wstrict-prototypes -DIN_GCC -DIN_TARGET_LIBS -fno-strict-aliasing -fexceptions
G_INCLUDES=-I${GNUHOSTDIST}/libobjc/objc  -I${GNUHOSTDIST}/libobjc/../gcc -I${GNUHOSTDIST}/libobjc/../gcc/config -I../.././gcc -I${GNUHOSTDIST}/libobjc/../include
G_OBJS=archive.lo class.lo encoding.lo gc.lo hash.lo init.lo linking.lo misc.lo nil_method.lo NXConstStr.lo Object.lo objects.lo Protocol.lo sarray.lo selector.lo sendmsg.lo thr.lo thr-objc.lo exception.lo hash_compat.lo
G_OBJC_H=hash.h objc-list.h sarray.h objc.h objc-api.h NXConstStr.h Object.h Protocol.h encoding.h typedstream.h thr.h objc-decls.h
G_UNWIND_H=${GNUHOSTDIST}/gcc/unwind-generic.h

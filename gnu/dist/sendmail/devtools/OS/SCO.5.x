#	Id: SCO.5.x,v 8.14 2002/03/21 23:59:26 gshapiro Exp
#	$NetBSD: SCO.5.x,v 1.4 2003/06/01 14:06:55 atatat Exp $

dnl	DO NOT EDIT THIS FILE.
dnl	Place personal settings in devtools/Site/site.config.m4

define(`confCC', `cc -b elf')
define(`confLIBS', `-lsocket -lndbm -lprot -lcurses -lm -lx -lgen')
define(`confMAPDEF', `-DMAP_REGEX -DNDBM')
define(`confSBINGRP', `bin')
define(`confMBINDIR', `/usr/lib')
define(`confSBINDIR', `/usr/etc')
define(`confUBINDIR', `/usr/bin')
define(`confINSTALL', `${BUILDBIN}/install.sh')

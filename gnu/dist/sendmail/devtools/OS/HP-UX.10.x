#	Id: HP-UX.10.x,v 8.19 2002/03/21 23:59:25 gshapiro Exp
#	$NetBSD: HP-UX.10.x,v 1.5 2003/06/01 14:06:54 atatat Exp $

dnl	DO NOT EDIT THIS FILE.
dnl	Place personal settings in devtools/Site/site.config.m4

define(`confCC', `cc -Aa')
define(`confMAPDEF', `-DNDBM -DNIS -DMAP_REGEX')
define(`confENVDEF', `-D_HPUX_SOURCE -DHPUX10 -DV4FS')
define(`confSM_OS_HEADER', `sm_os_hp')
define(`confOPTIMIZE', `+O3')
define(`confLIBS', `-lndbm')
define(`confSHELL', `/usr/bin/sh')
define(`confINSTALL', `${BUILDBIN}/install.sh')
define(`confSBINGRP', `mail')

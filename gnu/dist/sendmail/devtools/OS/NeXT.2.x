#	Id: NeXT.2.x,v 8.13 2002/03/21 23:59:25 gshapiro Exp
#	$NetBSD: NeXT.2.x,v 1.4 2003/06/01 14:06:55 atatat Exp $

dnl	DO NOT EDIT THIS FILE.
dnl	Place personal settings in devtools/Site/site.config.m4

define(`confSM_OS_HEADER', `sm_os_next')
define(`confBEFORE', `unistd.h dirent.h')
define(`confMAPDEF', `-DNDBM -DNIS -DNETINFO')
define(`confENVDEF', `-DNeXT')
define(`confLIBS', `-ldbm')
define(`confMBINDIR', `/usr/lib')
define(`confSBINDIR', `/usr/etc')
define(`confUBINDIR', `/usr/ucb')
define(`confEBINDIR', `/usr/lib')
define(`confINSTALL', `${BUILDBIN}/install.sh')
define(`confRANLIBOPTS', `-c')
PUSHDIVERT(3)
unistd.h:
	cp /dev/null unistd.h

dirent.h:
	echo "#include <sys/dir.h>" > dirent.h
	echo "#define dirent	direct" >> dirent.h
POPDIVERT

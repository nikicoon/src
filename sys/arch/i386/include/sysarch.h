/*	$NetBSD: sysarch.h,v 1.6 1995/06/05 19:36:23 pk Exp $	*/

#ifndef _I386_SYSARCH_H_
#define _I386_SYSARCH_H_

/*
 * Architecture specific syscalls (i386)
 */
#define I386_GET_LDT	0
#define I386_SET_LDT	1

struct i386_get_ldt_args {
	int start;
	union descriptor *desc;
	int num;
};

struct i386_set_ldt_args {
	int start;
	union descriptor *desc;
	int num;
};

#ifndef _KERNEL
int i386_get_ldt __P((int, union descriptor *, int));
int i386_set_ldt __P((int, union descriptor *, int));
int sysarch __P((int, char *));
#endif

#endif /* !_I386_SYSARCH_H_ */

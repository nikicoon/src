/*	$NetBSD: ofw.h,v 1.3 1998/07/08 07:56:13 thorpej Exp $	*/

/*
 * Copyright 1997
 * Digital Equipment Corporation. All rights reserved.
 *
 * This software is furnished under license and may be used and
 * copied only in accordance with the following terms and conditions.
 * Subject to these conditions, you may download, copy, install,
 * use, modify and distribute this software in source and/or binary
 * form. No title or ownership is transferred hereby.
 *
 * 1) Any source code used, modified or distributed must reproduce
 *    and retain this copyright notice and list of conditions as
 *    they appear in the source file.
 *
 * 2) No right is granted to use any trade name, trademark, or logo of
 *    Digital Equipment Corporation. Neither the "Digital Equipment
 *    Corporation" name nor any trademark or logo of Digital Equipment
 *    Corporation may be used to endorse or promote products derived
 *    from this software without the prior written permission of
 *    Digital Equipment Corporation.
 *
 * 3) This software is provided "AS-IS" and any express or implied
 *    warranties, including but not limited to, any implied warranties
 *    of merchantability, fitness for a particular purpose, or
 *    non-infringement are disclaimed. In no event shall DIGITAL be
 *    liable for any damages whatsoever, and in particular, DIGITAL
 *    shall not be liable for special, indirect, consequential, or
 *    incidental damages or damages for lost profits, loss of
 *    revenue or loss of use, whether such damages arise in contract,
 *    negligence, tort, under statute, in equity, at law or otherwise,
 *    even if advised of the possibility of such damage.
 */

#ifndef _MACHINE_OFW_H_
#define _MACHINE_OFW_H_


/* Virtual address range reserved for OFW. */
/* Maybe this should be elsewhere? -JJK */
#define	OFW_VIRT_BASE	0xF7000000
#define	OFW_VIRT_SIZE	0x01000000


/* OFW client services handle. */
typedef int (*ofw_handle_t)(void *);


/* Implemented in <ofw/ofw.c> */
void ofw_init __P((ofw_handle_t));
void ofw_boot __P((int, char *));
void ofw_getbootinfo __P((char **, char **));
void ofw_configmem __P((void));
void ofw_configisa __P((vm_offset_t *, vm_offset_t *));
void ofw_configisadma __P((vm_offset_t *));
int  ofw_isadmarangeintersect __P((vm_offset_t, vm_offset_t,
				   vm_offset_t *, vm_offset_t *));
vm_offset_t ofw_gettranslation __P((vm_offset_t));
vm_offset_t ofw_map __P((vm_offset_t, vm_size_t, int));
vm_offset_t ofw_getcleaninfo __P((void));

#ifdef	OFWGENCFG
/* Implemented in <ofw/ofwgencfg_machdep.c> */
extern int ofw_handleticks;
extern void cpu_reboot		__P((int, char *));
extern vm_offset_t initarm	__P((ofw_handle_t));
extern void ofrootfound		__P((void));
#endif

#endif /* !_MACHINE_OFW_H_ */

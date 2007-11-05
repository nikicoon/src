/* $NetBSD: isa_machdep.c,v 1.3 2007/11/05 15:49:03 garbled Exp $ */

/*-
 * Copyright (c) 2007 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Tim Rightnour
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
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: isa_machdep.c,v 1.3 2007/11/05 15:49:03 garbled Exp $");

#include <sys/param.h>

#include <machine/bus.h>
#include <machine/pio.h>

#include <dev/isa/isavar.h>
#include <dev/isa/isareg.h>

struct powerpc_isa_chipset genppc_ict;
static u_int32_t ofppc_isa_iobase;

/*
 * isa_inb and isa_outb are pretty much used to access the 8259's.  This isn't
 * ideal, but, whatever.  Because of this, this is an ultra-stripped down
 * version of those funcitons, just to satisfy the pic driver.
 * This is completely reliant on the fact that the region should be BAT mapped
 * in, and NOT mapped via bus_space_map.  If you bus_space_map a register
 * and then attempt to use isa_*b, you will lose.
 */

int
map_isa_ioregs(u_int32_t addr)
{
	if (addr) {
		ofppc_isa_iobase = addr;
		return 1;
	}
	return 0;
}

uint8_t
isa_inb(uint32_t addr)
{
	return(inb(ofppc_isa_iobase | addr));
}

void
isa_outb(uint32_t addr, uint8_t val)
{
	outb(ofppc_isa_iobase | addr, val);
}

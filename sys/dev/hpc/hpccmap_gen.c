/*	$NetBSD: hpccmap_gen.c,v 1.4 2003/07/14 15:40:37 lukem Exp $	*/

/*-
 * Copyright (c) 1999
 *         Shin Takemura and PocketBSD Project. All rights reserved.
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
 *	This product includes software developed by the PocketBSD project
 *	and its contributors.
 * 4. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sys/cdefs.h>
__RCSID("$NetBSD: hpccmap_gen.c,v 1.4 2003/07/14 15:40:37 lukem Exp $");

typedef unsigned char u_char;
typedef void (*output_func)(void*, int, u_char,  u_char, u_char);

void main(int ac, char *av[]);
void rgb_separate_out(void*, int, u_char, u_char, u_char);
void cmap_gen(output_func, void *);

unsigned char compo6[6] = {   0,  51, 102, 153, 204, 255 };
unsigned char compo7[7] = {   0,  42,  85, 127, 170, 212, 255 };

void
main(int ac, char *av[])
{
	int i;
	char *rgb = "rgb";

	printf("/*\n");
	printf(" *  Do not edit.\n");
	printf(" *  This file is automatically generated by hpccmap_gen.\n");
	printf(" */\n");
	printf("#include <dev/hpc/hpccmapar.h>\n");
	for (i = 0; i < 3; i++) {
		printf("unsigned char bivideo_cmap_%c[256] = {\n", rgb[i]);
		cmap_gen(rgb_separate_out, (void*)i);
		printf("};\n");
	}
}

void
rgb_separate_out(void *ctxx, int idx, unsigned char r, unsigned char g,
    unsigned char b)
{
	int rgb = (int)ctxx;

	if ((idx % 16) == 0)
		printf("\t");
	switch(rgb) {
	case 0:
		printf("%3d,", r);
		break;
	case 1:
		printf("%3d,", g);
		break;
	case 2:
		printf("%3d,", b);
		break;
	}
	if ((idx % 16) == 15)
		printf("\n");
}

void
cmap_gen(output_func func, void *ctx)
{
	int i, r, g, b;

	i = 0;

	/*
	 * 0 - 15, for ANSI escape sequence
	 * (see sys/dev/rasops/rasops.c)
	 */
	(*func)(ctx, i++, 0x00, 0x00, 0x00); /* black	*/
	(*func)(ctx, i++, 0x7f, 0x00, 0x00); /* red	*/
	(*func)(ctx, i++, 0x00, 0x7f, 0x00); /* green	*/
	(*func)(ctx, i++, 0x7f, 0x7f, 0x00); /* brown	*/
	(*func)(ctx, i++, 0x00, 0x00, 0x7f); /* blue	*/
	(*func)(ctx, i++, 0x7f, 0x00, 0x7f); /* magenta	*/
	(*func)(ctx, i++, 0x00, 0x7f, 0x7f); /* cyan	*/
	(*func)(ctx, i++, 0xc7, 0xc7, 0xc7); /* white	*/

	(*func)(ctx, i++, 0x7f, 0x7f, 0x7f); /* black	*/
	(*func)(ctx, i++, 0xff, 0x00, 0x00); /* red	*/
	(*func)(ctx, i++, 0x00, 0xff, 0x00); /* green	*/
	(*func)(ctx, i++, 0xff, 0xff, 0x00); /* brown	*/
	(*func)(ctx, i++, 0x00, 0x00, 0xff); /* blue	*/
	(*func)(ctx, i++, 0xff, 0x00, 0xff); /* magenta	*/
	(*func)(ctx, i++, 0x00, 0xff, 0xff); /* cyan	*/
	(*func)(ctx, i++, 0xff, 0xff, 0xff); /* white	*/

	/*
	 * 16 - 31, gray scale
	 */
	for (; i < 32; i++) {
		(*func)(ctx, i, (i - 16) * 17, (i - 16) * 17, (i - 16) * 17);
	}

	/*
	 * 32 - 247, RGB color
	 */
	for (r = 0; r < 6; r++) {
		for (g = 0; g < 6; g++) {
			for (b = 0; b < 6; b++) {
				(*func)(ctx, i,
				    compo6[r], compo6[g], compo6[b]);
				i++;
			}
		}
	}

	/*
	 * 248 - 255, just white
	 */
	for ( ; i < 256; i++) {
		(*func)(ctx, i, 255, 255, 255);
	}
}

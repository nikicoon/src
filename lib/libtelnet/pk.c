/*-
 * Copyright (c) 1991, 1993
 *      Dave Safford.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
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

#ifdef notdef
__FBSDID("$FreeBSD: src/contrib/telnet/libtelnet/pk.c,v 1.10 2002/08/22 06:19:07 nsayer Exp $");
#else
__RCSID("$NetBSD: pk.c,v 1.1 2005/02/19 21:55:52 christos Exp $");
#endif

/* public key routines */
/* functions:
	genkeys(char *public, char *secret)
	common_key(char *secret, char *public, desData *deskey)
        pk_encode(char *in, *out, DesData *deskey);
        pk_decode(char *in, *out, DesData *deskey);
      where
	char public[HEXKEYBYTES + 1];
	char secret[HEXKEYBYTES + 1];
 */

#include <sys/time.h>
#include <openssl/des.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mp.h"
#include "pk.h"
 
static void adjust(char keyout[HEXKEYBYTES+1], char *keyin);

/*
 * Choose top 128 bits of the common key to use as our idea key.
 */
static void
extractideakey(MINT *ck, IdeaData *ideakey)
{
        MINT *a;
        MINT *z;
        short r;
        int i;
        short base = (1 << 8);
        char *k;

        z = itom(0);
        a = itom(0);
        madd(ck, z, a);
        for (i = 0; i < ((KEYSIZE - 128) / 8); i++) {
                sdiv(a, base, a, &r);
        }
        k = (char *)ideakey;
        for (i = 0; i < 16; i++) {
                sdiv(a, base, a, &r);
                *k++ = r;
        }
	mfree(z);
        mfree(a);
}

/*
 * Choose middle 64 bits of the common key to use as our des key, possibly
 * overwriting the lower order bits by setting parity. 
 */
static void
extractdeskey(MINT *ck, DesData *deskey)
{
        MINT *a;
        MINT *z;
        short r;
        int i;
        short base = (1 << 8);
        char *k;

        z = itom(0);
        a = itom(0);
        madd(ck, z, a);
        for (i = 0; i < ((KEYSIZE - 64) / 2) / 8; i++) {
                sdiv(a, base, a, &r);
        }
        k = (char *)deskey;
        for (i = 0; i < 8; i++) {
                sdiv(a, base, a, &r);
                *k++ = r;
        }
	mfree(z);
        mfree(a);
}

/*
 * get common key from my secret key and his public key
 */
void
common_key(char *xsecret, char *xpublic, IdeaData *ideakey, DesData *deskey)
{
        MINT *public;
        MINT *secret;
        MINT *common;
	MINT *modulus = xtom(HEXMODULUS);

        public = xtom(xpublic);
        secret = xtom(xsecret);
        common = itom(0);
        pow(public, secret, modulus, common);
        extractdeskey(common, deskey);
        extractideakey(common, ideakey);
	des_set_odd_parity(deskey);
        mfree(common);
        mfree(secret);
        mfree(public);
	mfree(modulus);
}

/*
 * Generate a seed
 */
static void
getseed(char *seed, int seedsize)
{
	int i;

	srandomdev();
	for (i = 0; i < seedsize; i++) {
		seed[i] = random() & 0xff;
	}
}

/*
 * Generate a random public/secret key pair
 */
void
genkeys(char *public, char *secret)
{
        size_t i;
 
#       define BASEBITS (8*sizeof(short) - 1)
#       define BASE (1 << BASEBITS)
 
        MINT *pk = itom(0);
        MINT *sk = itom(0);
        MINT *tmp;
        MINT *base = itom(BASE);
        MINT *root = itom(PROOT);
        MINT *modulus = xtom(HEXMODULUS);
        short r;
        unsigned short seed[KEYSIZE/BASEBITS + 1];
        char *xkey;

        getseed((char *)seed, sizeof(seed));    
        for (i = 0; i < KEYSIZE/BASEBITS + 1; i++) {
                r = seed[i] % BASE;
                tmp = itom(r);
                mult(sk, base, sk);
                madd(sk, tmp, sk);
                mfree(tmp);  
        }
        tmp = itom(0);
        mdiv(sk, modulus, tmp, sk);
        mfree(tmp);
        pow(root, sk, modulus, pk); 
        xkey = mtox(sk);   
        adjust(secret, xkey);
        xkey = mtox(pk);
        adjust(public, xkey);
        mfree(sk);
        mfree(base);
        mfree(pk);
        mfree(root);
        mfree(modulus);
} 

/*
 * Adjust the input key so that it is 0-filled on the left
 */
static void
adjust(char keyout[HEXKEYBYTES+1], char *keyin)
{
        char *p;
        char *s;

        for (p = keyin; *p; p++) 
                ;
        for (s = keyout + HEXKEYBYTES; p >= keyin; p--, s--) {
                *s = *p;
        }
        while (s >= keyout) {
                *s-- = '0';
        }
}

static char hextab[17] = "0123456789ABCDEF";

/* given a DES key, cbc encrypt and translate input to terminated hex */
void
pk_encode(char *in, char *out, DesData *key)
{
	char buf[256];
	DesData i;
	des_key_schedule k;
	int l,op,deslen;

	memset(&i,0,sizeof(i));
	memset(buf,0,sizeof(buf));
	deslen = ((strlen(in) + 7)/8)*8;
	des_key_sched(key, k);
	des_cbc_encrypt(in,buf,deslen, k,&i,DES_ENCRYPT);
	for (l=0,op=0;l<deslen;l++) {
		out[op++] = hextab[(buf[l] & 0xf0) >> 4];
		out[op++] = hextab[(buf[l] & 0x0f)];
	}
	out[op] = '\0';
}

/* given a DES key, translate input from hex and decrypt */
void
pk_decode(char *in, char *out, DesData *key)
{
	char buf[256];
	DesData i;
	des_key_schedule k;
	int n1,n2,op;
	size_t l;

	memset(&i,0,sizeof(i));
	memset(buf,0,sizeof(buf));
	for (l=0,op=0;l<strlen(in)/2;l++,op+=2) {
		if (in[op] > '9')
			n1 = in[op] - 'A' + 10;
		else
			n1 = in[op] - '0';
		if (in[op+1] > '9')
			n2 = in[op+1] - 'A' + 10;
		else
			n2 = in[op+1] - '0';
		buf[l] = n1*16 +n2;
	}
	des_key_sched(key, k);
	des_cbc_encrypt(buf,out,strlen(in)/2, k,&i,DES_DECRYPT);
	out[strlen(in)/2] = '\0';
}

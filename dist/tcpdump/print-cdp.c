/*	$NetBSD: print-cdp.c,v 1.4 2002/05/31 09:45:45 itojun Exp $	*/

/*
 * Copyright (c) 1992, 1993, 1994, 1995, 1996, 1997
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Code by Gert Doering, SpaceNet GmbH, gert@space.net
 *
 * Reference documentation:
 *    http://www.cisco.com/univercd/cc/td/doc/product/lan/trsrb/frames.htm
 */

#include <sys/cdefs.h>
#ifndef lint
#if 0
static const char rcsid[] =
    "@(#) Header: /tcpdump/master/tcpdump/print-cdp.c,v 1.13 2002/04/26 09:51:34 guy Exp";
#else
__RCSID("$NetBSD: print-cdp.c,v 1.4 2002/05/31 09:45:45 itojun Exp $");
#endif
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/param.h>
#include <sys/time.h>

#include <netinet/in.h>

#include <ctype.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#include "interface.h"
#include "addrtoname.h"
#include "extract.h"			/* must come after interface.h */

static int cdp_print_addr(const u_char *, int);
static int cdp_print_prefixes(const u_char *, int);
static unsigned long cdp_get_number(const u_char *, int);

void
cdp_print(const u_char *p, u_int length, u_int caplen,
	  const u_char *esrc, const u_char *edst)
{
	u_int i;
	int type, len;

	/* Cisco Discovery Protocol */

	if (caplen < 4) {
		(void)printf("[|cdp]");
		return;
	}

	i = 0;		/* CDP data starts at offset 0 */
	printf("CDP v%u, ttl=%us", p[i], p[i + 1]);
	i += 4;		/* skip version, TTL and chksum */

	while (i < length) {
		if (i + 4 > caplen)
			goto trunc;
		type = (p[i] <<  8) + p[i + 1];
		len  = (p[i + 2] << 8) + p[i + 3];

		if (vflag > 1)
			printf("\n\t");

		if (vflag)
			printf(" %02x/%02x", type, len);

		if (i + len > caplen)
			goto trunc;

		switch (type) {
		case 0x00:
			printf(" Goodbye");
			break;
		case 0x01:
			printf(" DevID '%.*s'", len - 4, p + i + 4);
			break;
		case 0x02:
			printf(" Addr");
			if (cdp_print_addr(p + i + 4, len - 4) < 0)
				goto trunc;
			break;
		case 0x03:
			printf(" PortID '%.*s'", len - 4, p + i + 4);
			break;
		case 0x04:
			printf(" CAP 0x%02x", (unsigned) p[i + 7]);
			break;
		case 0x05:
			if (vflag > 1)
				printf(" Version:\n%.*s", len - 4, p + i + 4);
			else
				printf(" Version: (suppressed)");
			break;
		case 0x06:
			printf(" Platform: '%.*s'", len - 4, p + i + 4);
			break;
		case 0x07:
			if (cdp_print_prefixes(p + i + 4, len - 4) < 0)
				goto trunc;
			break;
		case 0x08:		/* guess - not documented */
			printf(" Protocol-Hello option" );
			break;
		case 0x09:		/* guess - not documented */
			printf(" VTP Management Domain: '%.*s'", len - 4,
			    p + i + 4);
			break;
		case 0x0a:		/* guess - not documented */
			printf(" Native VLAN ID: %d",
			    (p[i + 4] << 8) + p[i + 4 + 1] );
			break;
		case 0x0b:		/* guess - not documented */
			printf(" Duplex: %s", p[i + 4] ? "full": "half");
			break;
/* http://www.cisco.com/univercd/cc/td/doc/product/voice/ata/atarn/186rn21m.htm
 * plus more details from other sources 
 */
		case 0x0e:		/* incomplete doc. */
			printf(" ATA-186 VoIP VLAN request, app %d, vlan %d",
				p[i + 4], EXTRACT_16BITS(&p[i+4+1]));
			break;
		case 0x0f:		/* incomplete doc. */
			printf(" ATA-186 VoIP VLAN assignment" );
			break;
		case 0x10:		/* incomplete doc. */
			printf(" power consumption: %1.2fW", 
				cdp_get_number(p+i+4, len-4)/1000.0 );
			break;
		case 0x11:		/* guess - not documented */
			printf(" MTU %u bytes", EXTRACT_32BITS(&p[i+4]));
			break;
		case 0x12:		/* guess - not documented */
			printf(" AVVID trust bitmap 0x%02x", p[i+4] );
			break;
		case 0x13:		/* guess - not documented */
			printf(" AVVID untrusted ports CoS: 0x%02x", p[i+4]);
			break;
		case 0x14:		/* guess - not documented */
			printf(" sysName='%.*s'", len - 4, p + i + 4 );
			break;
		case 0x15:		/* guess - not documented */
			printf(" sysObjectID" );		/* TODO */
			break;
		case 0x16:		/* guess - not documented */
			printf(" management address(es)" );
			break;
		case 0x17:		/* guess - not documented */
			printf(" phys. location 0x%02x/%.*s",
				p[i+4], len - 5, p + i + 5 );
			break;
		default:
			printf(" unknown field type %02x, len %d", type, len);
			break;
		}

		/* avoid infinite loop */
		if (len == 0)
			break;
		i += len;
	}

	return;

trunc:
	printf("[|cdp]");
}

/*
 * Protocol type values.
 *
 * PT_NLPID means that the protocol type field contains an OSI NLPID.
 *
 * PT_IEEE_802_2 means that the protocol type field contains an IEEE 802.2
 * LLC header that specifies that the payload is for that protocol.
 */
#define PT_NLPID		1	/* OSI NLPID */
#define PT_IEEE_802_2		2	/* IEEE 802.2 LLC header */

static int
cdp_print_addr(const u_char * p, int l)
{
	int pt, pl, al, num;
	const u_char *endp = p + l;
#ifdef INET6
	static u_char prot_ipv6[] = {
		0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00, 0x86, 0xdd
	};
#endif

	num = EXTRACT_32BITS(p);
	p += 4;

	printf(" (%d): ", num);

	while (p < endp && num >= 0) {
		if (p + 2 > endp)
			goto trunc;
		pt = p[0];		/* type of "protocol" field */
		pl = p[1];		/* length of "protocol" field */
		p += 2;

		if (p + pl + 2 > endp)
			goto trunc;
		al = EXTRACT_16BITS(&p[pl]);	/* address length */

		if (pt == PT_NLPID && pl == 1 && *p == 0xcc && al == 4) {
			/*
			 * IPv4: protocol type = NLPID, protocol length = 1
			 * (1-byte NLPID), protocol = 0xcc (NLPID for IPv4),
			 * address length = 4
			 */
			p += 3;

			if (p + 4 > endp)
				goto trunc;
			printf("IPv4 %u.%u.%u.%u", p[0], p[1], p[2], p[3]);
			p += 4;
		}
#ifdef INET6
		else if (pt == PT_IEEE_802_2 && pl == 8 &&
		    memcmp(p, prot_ipv6, 8) == 0 && al == 16) {
			/*
			 * IPv6: protocol type = IEEE 802.2 header,
			 * protocol length = 8 (size of LLC+SNAP header),
			 * protocol = LLC+SNAP header with the IPv6
			 * Ethertype, address length = 16
			 */
			p += 10; 
			if (p + al > endp)
				goto trunc;

			printf("IPv6 %s", ip6addr_string(p));
			p += al;
		}
#endif
		else {
			/*
			 * Generic case: just print raw data
			 */
			if (p + pl > endp)
				goto trunc;
			printf("pt=0x%02x, pl=%d, pb=", *(p - 2), pl);
			while (pl-- > 0)
				printf(" %02x", *p++);
			if (p + 2 > endp)
				goto trunc;
			al = (*p << 8) + *(p + 1);
			printf(", al=%d, a=", al);
			p += 2;
			if (p + al > endp)
				goto trunc;
			while (al-- > 0)
				printf(" %02x", *p++);
		}
		num--;
		if (num)
			printf(" ");
	}

	return 0;

trunc:
	return -1;
}


static int
cdp_print_prefixes(const u_char * p, int l)
{
	if (l % 5)
		goto trunc;

	printf(" IPv4 Prefixes (%d):", l / 5);

	while (l > 0) {
		printf(" %u.%u.%u.%u/%u", p[0], p[1], p[2], p[3], p[4]);
		l -= 5;
		p += 5;
	}

	return 0;

trunc:
	return -1;
}

/* read in a <n>-byte number, MSB first
 * (of course this can handle max sizeof(long))
 */
static unsigned long cdp_get_number(const u_char * p, int l)
{
    unsigned long res=0;
    while( l>0 )
    {
	res = (res<<8) + *p;
	p++; l--;
    }
    return res;
}

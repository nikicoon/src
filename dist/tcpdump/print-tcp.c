/*	$NetBSD: print-tcp.c,v 1.3 2001/06/25 20:00:01 itojun Exp $	*/

/*
 * Copyright (c) 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997
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
 */

#include <sys/cdefs.h>
#ifndef lint
#if 0
static const char rcsid[] =
    "@(#) Header: /tcpdump/master/tcpdump/print-tcp.c,v 1.86 2001/05/09 01:16:57 fenner Exp (LBL)";
#else
__RCSID("$NetBSD: print-tcp.c,v 1.3 2001/06/25 20:00:01 itojun Exp $");
#endif
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/param.h>
#include <sys/time.h>

#include <rpc/rpc.h>

#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "interface.h"
#include "addrtoname.h"
#include "extract.h"

#include "tcp.h"

#include "ip.h"
#ifdef INET6
#include "ip6.h"
#endif

#include "nameser.h"

static void print_tcp_rst_data(register const u_char *sp, u_int length);

#define MAX_RST_DATA_LEN	30


struct tha {
#ifndef INET6
	struct in_addr src;
	struct in_addr dst;
#else
	struct in6_addr src;
	struct in6_addr dst;
#endif /*INET6*/
	u_int port;
};

struct tcp_seq_hash {
	struct tcp_seq_hash *nxt;
	struct tha addr;
	tcp_seq seq;
	tcp_seq ack;
};

#define TSEQ_HASHSIZE 919

/* These tcp optinos do not have the size octet */
#define ZEROLENOPT(o) ((o) == TCPOPT_EOL || (o) == TCPOPT_NOP)

static struct tcp_seq_hash tcp_seq_hash[TSEQ_HASHSIZE];


#ifndef TELNET_PORT
#define TELNET_PORT	23
#endif
#ifndef BGP_PORT
#define BGP_PORT	179
#endif
#define NETBIOS_SSN_PORT 139
#ifndef PPTP_PORT
#define PPTP_PORT	1723
#endif
#define BXXP_PORT        10288
#ifndef NFS_PORT
#define NFS_PORT	2049
#endif

static int tcp_cksum(register const struct ip *ip,
		     register const struct tcphdr *tp,
		     register int len)
{
	int i, tlen;
	union phu {
		struct phdr {
			u_int32_t src;
			u_int32_t dst;
			u_char mbz;
			u_char proto;
			u_int16_t len;
		} ph;
		u_int16_t pa[6];
	} phu;
	register const u_int16_t *sp;
	u_int32_t sum;
	tlen = ntohs(ip->ip_len) - ((const char *)tp-(const char*)ip);

	/* pseudo-header.. */
	phu.ph.len = htons(tlen);
	phu.ph.mbz = 0;
	phu.ph.proto = IPPROTO_TCP;
	memcpy(&phu.ph.src, &ip->ip_src.s_addr, sizeof(u_int32_t));
	memcpy(&phu.ph.dst, &ip->ip_dst.s_addr, sizeof(u_int32_t));

	sp = &phu.pa[0];
	sum = sp[0]+sp[1]+sp[2]+sp[3]+sp[4]+sp[5];

	sp = (const u_int16_t *)tp;

	for (i=0; i<(tlen&~1); i+= 2)
		sum += *sp++;

	if (tlen & 1) {
		sum += htons( (*(const u_int8_t *)sp) << 8);
	}

	while (sum > 0xffff)
		sum = (sum & 0xffff) + (sum >> 16);
	sum = ~sum & 0xffff;

	return (sum);
}

#ifdef INET6
static int tcp6_cksum(const struct ip6_hdr *ip6, const struct tcphdr *tp,
	int len)
{
	int i, tlen;
	register const u_int16_t *sp;
	u_int32_t sum;
	union {
		struct {
			struct in6_addr ph_src;
			struct in6_addr ph_dst;
			u_int32_t	ph_len;
			u_int8_t	ph_zero[3];
			u_int8_t	ph_nxt;
		} ph;
		u_int16_t pa[20];
	} phu;

	tlen = ntohs(ip6->ip6_plen) + sizeof(struct ip6_hdr) -
	    ((const char *)tp - (const char*)ip6);

	/* pseudo-header */
	memset(&phu, 0, sizeof(phu));
	phu.ph.ph_src = ip6->ip6_src;
	phu.ph.ph_dst = ip6->ip6_dst;
	phu.ph.ph_len = htonl(tlen);
	phu.ph.ph_nxt = IPPROTO_TCP;

	sum = 0;
	for (i = 0; i < sizeof(phu.pa) / sizeof(phu.pa[0]); i++)
		sum += phu.pa[i];

	sp = (const u_int16_t *)tp;

	for (i = 0; i < (tlen & ~1); i += 2)
		sum += *sp++;

	if (tlen & 1)
		sum += htons((*(const u_int8_t *)sp) << 8);

	while (sum > 0xffff)
		sum = (sum & 0xffff) + (sum >> 16);
	sum = ~sum & 0xffff;

	return (sum);
}
#endif

void
tcp_print(register const u_char *bp, register u_int length,
	  register const u_char *bp2, int fragmented)
{
	register const struct tcphdr *tp;
	register const struct ip *ip;
	register u_char flags;
	register int hlen;
	register char ch;
	u_int16_t sport, dport, win, urp;
	u_int32_t seq, ack, thseq, thack;
	int threv;
#ifdef INET6
	register const struct ip6_hdr *ip6;
#endif

	tp = (struct tcphdr *)bp;
	ip = (struct ip *)bp2;
#ifdef INET6
	if (IP_V(ip) == 6)
		ip6 = (struct ip6_hdr *)bp2;
	else
		ip6 = NULL;
#endif /*INET6*/
	ch = '\0';
	if (!TTEST(tp->th_dport)) {
		(void)printf("%s > %s: [|tcp]",
			ipaddr_string(&ip->ip_src),
			ipaddr_string(&ip->ip_dst));
		return;
	}

	sport = ntohs(tp->th_sport);
	dport = ntohs(tp->th_dport);


	hlen = TH_OFF(tp) * 4;

	/*
	 * If data present and NFS port used, assume NFS.
	 * Pass offset of data plus 4 bytes for RPC TCP msg length
	 * to NFS print routines.
	 */
	if (!qflag) {
		if ((u_char *)tp + 4 + sizeof(struct rpc_msg) <= snapend &&
		    dport == NFS_PORT) {
			nfsreq_print((u_char *)tp + hlen + 4, length - hlen,
				     (u_char *)ip);
			return;
		} else if ((u_char *)tp + 4 + sizeof(struct rpc_msg)
			   <= snapend &&
			   sport == NFS_PORT) {
			nfsreply_print((u_char *)tp + hlen + 4,length-hlen,
				       (u_char *)ip);
			return;
		}
	}
#ifdef INET6
	if (ip6) {
		if (ip6->ip6_nxt == IPPROTO_TCP) {
			(void)printf("%s.%s > %s.%s: ",
				ip6addr_string(&ip6->ip6_src),
				tcpport_string(sport),
				ip6addr_string(&ip6->ip6_dst),
				tcpport_string(dport));
		} else {
			(void)printf("%s > %s: ",
				tcpport_string(sport), tcpport_string(dport));
		}
	} else
#endif /*INET6*/
	{
		if (ip->ip_p == IPPROTO_TCP) {
			(void)printf("%s.%s > %s.%s: ",
				ipaddr_string(&ip->ip_src),
				tcpport_string(sport),
				ipaddr_string(&ip->ip_dst),
				tcpport_string(dport));
		} else {
			(void)printf("%s > %s: ",
				tcpport_string(sport), tcpport_string(dport));
		}
	}

	TCHECK(*tp);

	seq = (u_int32_t)ntohl(tp->th_seq);
	ack = (u_int32_t)ntohl(tp->th_ack);
	win = ntohs(tp->th_win);
	urp = ntohs(tp->th_urp);

	if (qflag) {
		(void)printf("tcp %d", length - TH_OFF(tp) * 4);
		return;
	}
	if ((flags = tp->th_flags) & (TH_SYN|TH_FIN|TH_RST|TH_PUSH|
				      TH_ECNECHO|TH_CWR)) {
		if (flags & TH_SYN)
			putchar('S');
		if (flags & TH_FIN)
			putchar('F');
		if (flags & TH_RST)
			putchar('R');
		if (flags & TH_PUSH)
			putchar('P');
		if (flags & TH_CWR)
			putchar('W');	/* congestion _W_indow reduced (ECN) */
		if (flags & TH_ECNECHO)
			putchar('E');	/* ecn _E_cho sent (ECN) */
	} else
		putchar('.');

	if (!Sflag && (flags & TH_ACK)) {
		register struct tcp_seq_hash *th;
		register int rev;
		struct tha tha;
		/*
		 * Find (or record) the initial sequence numbers for
		 * this conversation.  (we pick an arbitrary
		 * collating order so there's only one entry for
		 * both directions).
		 */
#ifdef INET6
		memset(&tha, 0, sizeof(tha));
		rev = 0;
		if (ip6) {
			if (sport > dport)
				rev = 1;
			else if (sport == dport) {
				int i;

				for (i = 0; i < 4; i++) {
					if (((u_int32_t *)(&ip6->ip6_src))[i] >
					    ((u_int32_t *)(&ip6->ip6_dst))[i]) {
						rev = 1;
						break;
					}
				}
			}
			if (rev) {
				tha.src = ip6->ip6_dst;
				tha.dst = ip6->ip6_src;
				tha.port = dport << 16 | sport;
			} else {
				tha.dst = ip6->ip6_dst;
				tha.src = ip6->ip6_src;
				tha.port = sport << 16 | dport;
			}
		} else {
			if (sport > dport ||
			    (sport == dport &&
			     ip->ip_src.s_addr > ip->ip_dst.s_addr)) {
				rev = 1;
			}
			if (rev) {
				*(struct in_addr *)&tha.src = ip->ip_dst;
				*(struct in_addr *)&tha.dst = ip->ip_src;
				tha.port = dport << 16 | sport;
			} else {
				*(struct in_addr *)&tha.dst = ip->ip_dst;
				*(struct in_addr *)&tha.src = ip->ip_src;
				tha.port = sport << 16 | dport;
			}
		}
#else
		if (sport < dport ||
		    (sport == dport &&
		     ip->ip_src.s_addr < ip->ip_dst.s_addr)) {
			tha.src = ip->ip_src, tha.dst = ip->ip_dst;
			tha.port = sport << 16 | dport;
			rev = 0;
		} else {
			tha.src = ip->ip_dst, tha.dst = ip->ip_src;
			tha.port = dport << 16 | sport;
			rev = 1;
		}
#endif

		threv = rev;
		for (th = &tcp_seq_hash[tha.port % TSEQ_HASHSIZE];
		     th->nxt; th = th->nxt)
			if (!memcmp((char *)&tha, (char *)&th->addr,
				  sizeof(th->addr)))
				break;

		if (!th->nxt || (flags & TH_SYN)) {
			/* didn't find it or new conversation */
			if (th->nxt == NULL) {
				th->nxt = (struct tcp_seq_hash *)
					calloc(1, sizeof(*th));
				if (th->nxt == NULL)
					error("tcp_print: calloc");
			}
			th->addr = tha;
			if (rev)
				th->ack = seq, th->seq = ack - 1;
			else
				th->seq = seq, th->ack = ack - 1;
		} else {
			if (rev)
				seq -= th->ack, ack -= th->seq;
			else
				seq -= th->seq, ack -= th->ack;
		}

		thseq = th->seq;
		thack = th->ack;
	} else {
		/*fool gcc*/
		thseq = thack = threv = 0;
	}
	if (hlen > length) {
		(void)printf(" [bad hdr length]");
		return;
	}

	if (IP_V(ip) == 4 && vflag && !fragmented) {
		int sum;
		if (TTEST2(tp->th_sport, length)) {
			sum = tcp_cksum(ip, tp, length);
			if (sum != 0)
				(void)printf(" [bad tcp cksum %x!]", sum);
			else
				(void)printf(" [tcp sum ok]");
		}
	}
#ifdef INET6
	if (IP_V(ip) == 6 && ip6->ip6_plen && vflag && !fragmented) {
		int sum;
		if (TTEST2(tp->th_sport, length)) {
			sum = tcp6_cksum(ip6, tp, length);
			if (sum != 0)
				(void)printf(" [bad tcp cksum %x!]", sum);
			else
				(void)printf(" [tcp sum ok]");
		}
	}
#endif

	length -= hlen;
	if (vflag > 1 || length > 0 || flags & (TH_SYN | TH_FIN | TH_RST))
		(void)printf(" %u:%u(%d)", seq, seq + length, length);
	if (flags & TH_ACK)
		(void)printf(" ack %u", ack);

	(void)printf(" win %d", win);

	if (flags & TH_URG)
		(void)printf(" urg %d", urp);
	/*
	 * Handle any options.
	 */
	if ((hlen -= sizeof(*tp)) > 0) {
		register const u_char *cp;
		register int i, opt, len, datalen;

		cp = (const u_char *)tp + sizeof(*tp);
		putchar(' ');
		ch = '<';
		while (hlen > 0) {
			putchar(ch);
			TCHECK(*cp);
			opt = *cp++;
			if (ZEROLENOPT(opt))
				len = 1;
			else {
				TCHECK(*cp);
				len = *cp++;	/* total including type, len */
				if (len < 2 || len > hlen)
					goto bad;
				--hlen;		/* account for length byte */
			}
			--hlen;			/* account for type byte */
			datalen = 0;

/* Bail if "l" bytes of data are not left or were not captured  */
#define LENCHECK(l) { if ((l) > hlen) goto bad; TCHECK2(*cp, l); }

			switch (opt) {

			case TCPOPT_MAXSEG:
				(void)printf("mss");
				datalen = 2;
				LENCHECK(datalen);
				(void)printf(" %u", EXTRACT_16BITS(cp));

				break;

			case TCPOPT_EOL:
				(void)printf("eol");
				break;

			case TCPOPT_NOP:
				(void)printf("nop");
				break;

			case TCPOPT_WSCALE:
				(void)printf("wscale");
				datalen = 1;
				LENCHECK(datalen);
				(void)printf(" %u", *cp);
				break;

			case TCPOPT_SACKOK:
				(void)printf("sackOK");
				break;

			case TCPOPT_SACK:
				(void)printf("sack");
				datalen = len - 2;
				if (datalen % 8 != 0) {
					(void)printf(" malformed sack ");
				} else {
					u_int32_t s, e;

					(void)printf(" sack %d ", datalen / 8);
					for (i = 0; i < datalen; i += 8) {
						LENCHECK(i + 4);
						s = EXTRACT_32BITS(cp + i);
						LENCHECK(i + 8);
						e = EXTRACT_32BITS(cp + i + 4);
						if (threv) {
							s -= thseq;
							e -= thseq;
						} else {
							s -= thack;
							e -= thack;
						}
						(void)printf("{%u:%u}", s, e);
					}
					(void)printf(" ");
				}
				break;

			case TCPOPT_ECHO:
				(void)printf("echo");
				datalen = 4;
				LENCHECK(datalen);
				(void)printf(" %u", EXTRACT_32BITS(cp));
				break;

			case TCPOPT_ECHOREPLY:
				(void)printf("echoreply");
				datalen = 4;
				LENCHECK(datalen);
				(void)printf(" %u", EXTRACT_32BITS(cp));
				break;

			case TCPOPT_TIMESTAMP:
				(void)printf("timestamp");
				datalen = 8;
				LENCHECK(4);
				(void)printf(" %u", EXTRACT_32BITS(cp));
				LENCHECK(datalen);
				(void)printf(" %u", EXTRACT_32BITS(cp + 4));
				break;

			case TCPOPT_CC:
				(void)printf("cc");
				datalen = 4;
				LENCHECK(datalen);
				(void)printf(" %u", EXTRACT_32BITS(cp));
				break;

			case TCPOPT_CCNEW:
				(void)printf("ccnew");
				datalen = 4;
				LENCHECK(datalen);
				(void)printf(" %u", EXTRACT_32BITS(cp));
				break;

			case TCPOPT_CCECHO:
				(void)printf("ccecho");
				datalen = 4;
				LENCHECK(datalen);
				(void)printf(" %u", EXTRACT_32BITS(cp));
				break;

			default:
				(void)printf("opt-%d:", opt);
				datalen = len - 2;
				for (i = 0; i < datalen; ++i) {
					LENCHECK(i);
					(void)printf("%02x", cp[i]);
				}
				break;
			}

			/* Account for data printed */
			cp += datalen;
			hlen -= datalen;

			/* Check specification against observed length */
			++datalen;			/* option octet */
			if (!ZEROLENOPT(opt))
				++datalen;		/* size octet */
			if (datalen != len)
				(void)printf("[len %d]", len);
			ch = ',';
			if (opt == TCPOPT_EOL)
				break;
		}
		putchar('>');
	}

	if (length <= 0)
		return;

	/*
	 * Decode payload if necessary.
	 */
	bp += TH_OFF(tp) * 4;
	if (flags & TH_RST) {
		if (vflag)
			print_tcp_rst_data(bp, length);
	} else {
		if (sport == TELNET_PORT || dport == TELNET_PORT) {
			if (!qflag && vflag)
				telnet_print(bp, length);
		} else if (sport == BGP_PORT || dport == BGP_PORT)
			bgp_print(bp, length);
		else if (sport == PPTP_PORT || dport == PPTP_PORT)
			pptp_print(bp, length);
#if 0
		else if (sport == NETBIOS_SSN_PORT || dport == NETBIOS_SSN_PORT)
			nbt_tcp_print(bp, length);
#endif
		else if (sport == BXXP_PORT || dport == BXXP_PORT)
			bxxp_print(bp, length);
		else if (length > 2 &&
		    (sport == NAMESERVER_PORT || dport == NAMESERVER_PORT)) {
			/* TCP DNS query has 2byte length at the head */
			ns_print(bp + 2, length - 2);
		}
	}
	return;
bad:
	fputs("[bad opt]", stdout);
	if (ch != '\0')
		putchar('>');
	return;
trunc:
	fputs("[|tcp]", stdout);
	if (ch != '\0')
		putchar('>');
}

/*
 * RFC1122 says the following on data in RST segments:
 *
 *         4.2.2.12  RST Segment: RFC-793 Section 3.4
 *
 *            A TCP SHOULD allow a received RST segment to include data.
 *
 *            DISCUSSION
 *                 It has been suggested that a RST segment could contain
 *                 ASCII text that encoded and explained the cause of the
 *                 RST.  No standard has yet been established for such
 *                 data.
 *
 */

static void
print_tcp_rst_data(register const u_char *sp, u_int length)
{
	int c;

	if (TTEST2(*sp, length))
		printf(" [RST");
	else
		printf(" [!RST");
	if (length > MAX_RST_DATA_LEN) {
		length = MAX_RST_DATA_LEN;	/* can use -X for longer */
		putchar('+');			/* indicate we truncate */
	}
	putchar(' ');
	while (length-- && sp <= snapend) {
		c = *sp++;
		safeputchar(c);
	}
	putchar(']');
}

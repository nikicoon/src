/*	$NetBSD: trylook.c,v 1.4 2002/07/14 00:26:18 wiz Exp $	*/

#include <sys/cdefs.h>
#ifndef lint
__RCSID("$NetBSD: trylook.c,v 1.4 2002/07/14 00:26:18 wiz Exp $");
#endif

/*
 * trylook.c - test program for lookup.c
 */

#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>

#include "report.h"
#include "lookup.h"

extern char *ether_ntoa(struct ether_addr *);
extern char *inet_ntoa(struct in_addr);

int debug = 0;
char *progname;

int
main(int argc, char **argv)
{
	int i;
	struct in_addr in;
	char *a;
	u_char *hwa;

	progname = argv[0];			/* for report */

	for (i = 1; i < argc; i++) {

		/* Host name */
		printf("%s:", argv[i]);

		/* IP addr */
		if (lookup_ipa(argv[i], &in.s_addr))
			a = "?";
		else
			a = inet_ntoa(in);
		printf(" ipa=%s", a);

		/* Ether addr */
		hwa = lookup_hwa(argv[i], 1);
		if (!hwa)
			a = "?";
		else
			a = ether_ntoa(hwa);
		printf(" hwa=%s\n", a);

	}
	exit(0);
}

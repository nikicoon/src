#ifndef lint
static char rcsid[] = "$Id: mount_isofs.c,v 1.8 1993/10/20 19:53:36 cgd Exp $";
#endif /* not lint */

#include <stdio.h>
#include <sys/types.h>
#define ISOFS
#include <sys/mount.h>

void
usage ()
{
	fprintf (stderr, "usage: mount_iso bdev dir\n");
	exit (1);
}
		
int
main (argc, argv)
int argc;
char **argv;
{
	char *dev;
	char *dir;
	struct iso_args args;
	int c;
	int opts = 0, mntflags = 0;

	argc--;
	argv++;
	while (argc > 2) {
		if (!strcmp("-F", argv[0])) {
			argc--; argv++;
			mntflags |= atoi(argv[0]);
			argc--; argv++;
		} else if (!strcmp(argv[0], "-norrip")) {
			opts |= ISOFSMNT_NORRIP;
			argc--; argv++;
		} else if (!strcmp(argv[0], "-gen")) {
			opts |= ISOFSMNT_GENS;
			argc--; argv++;
		} else if (!strcmp(argv[0], "-extattr")) {
			opts |= ISOFSMNT_EXTATT;
			argc--; argv++;
		} else
			usage();
	}

	dev = argv[0];
	dir = argv[1];

	args.fspec = dev;
	args.flags = opts;

	if (mount (MOUNT_ISOFS, dir, mntflags, &args) < 0) {
		perror ("mount");
		exit (1);
	}

	exit (0);
}


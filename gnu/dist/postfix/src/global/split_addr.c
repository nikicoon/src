/*++
/* NAME
/*	split_addr 3
/* SUMMARY
/*	recipient localpart address splitter
/* SYNOPSIS
/*	#include <split_addr.h>
/*
/*	char	*split_addr(localpart, delimiter)
/*	char	*localpart;
/*	int	delimiter;
/* DESCRIPTION
/*	split_addr() null-terminates \fIlocalpart\fR at the first
/*	occurrence of the \fIdelimiter\fR character found, and
/*	returns a pointer to the remainder.
/*
/*	Reserved addresses are not split: postmaster, mailer-daemon,
/*	double-bounce. Addresses that begin with owner-, or addresses
/*	that end in -request are not split when the owner_request_special
/*	parameter is set.
/* LICENSE
/* .ad
/* .fi
/*	The Secure Mailer license must be distributed with this software.
/* AUTHOR(S)
/*	Wietse Venema
/*	IBM T.J. Watson Research
/*	P.O. Box 704
/*	Yorktown Heights, NY 10598, USA
/*--*/

/* System library. */

#include <sys_defs.h>
#include <string.h>

#ifdef STRCASECMP_IN_STRINGS_H
#include <strings.h>
#endif

/* Utility library. */

#include <split_at.h>

/* Global library. */

#include <mail_params.h>
#include <mail_addr.h>
#include <split_addr.h>

/* split_addr - split address with extreme prejudice */

char   *split_addr(char *localpart, int delimiter)
{
    int     len;

    /*
     * Don't split these, regardless of what the delimiter is.
     */
    if (strcasecmp(localpart, MAIL_ADDR_POSTMASTER) == 0)
	return (0);
    if (strcasecmp(localpart, MAIL_ADDR_MAIL_DAEMON) == 0)
	return (0);
    if (strcasecmp(localpart, var_double_bounce_sender) == 0)
	return (0);

    /*
     * Backwards compatibility: don't split owner-foo or foo-request.
     */
    if (var_ownreq_special != 0) {
	if (strncasecmp(localpart, "owner-", 6) == 0)
	    return (0);
	if ((len = strlen(localpart) - 8) > 0
	    && strcasecmp(localpart + len, "-request") == 0)
	    return (0);
    }

    /*
     * Safe to split this address.
     */
    return (split_at(localpart, delimiter));
}

/*++
/* NAME
/*	dict_tcp 3
/* SUMMARY
/*	dictionary manager interface to tcp-based lookup tables
/* SYNOPSIS
/*	#include <dict_tcp.h>
/*
/*	DICT	*dict_tcp_open(map, dummy, dict_flags)
/*	const char *map;
/*	int	dummy;
/*	int	dict_flags;
/* DESCRIPTION
/*	dict_tcp_open() makes a TCP server accessible via the generic
/*	dictionary operations described in dict_open(3).
/*	The \fIdummy\fR argument is not used. The only implemented
/*	operation is dictionary lookup. This map type can be useful
/*	for simulating a dynamic lookup table.
/*
/*	Map names have the form host:port.
/*
/*	The TCP map class implements a very simple protocol: the client
/*	sends a request, and the server sends one reply. Requests and
/*	replies are sent as one line of ASCII text, terminated by the
/*	ASCII newline character. Request and reply parameters (see below)
/*	are separated by whitespace.
/* ENCODING
/* .ad
/* .fi
/*	In request and reply parameters, the character % and any non-printable
/*	characters (including whitespace) are replaced by %XX, XX being the
/*	corresponding ASCII hexadecimal character value. The hexadecimal codes
/*	can be specified in any case (upper, lower, mixed).
/* REQUEST FORMAT
/* .ad
/* .fi
/*	Requests are strings that serve as lookup key in the simulated
/*	table.
/* .IP "get SPACE key NEWLINE"
/*	Look up data under the specified key.
/* .IP "put SPACE key SPACE value NEWLINE"
/*	This request is currently not implemented.
/* REPLY FORMAT
/* .ad
/* .fi
/*      Replies can have the following form:
/* .IP "500 SPACE optional-text NEWLINE"
/*	In case of a lookup request, the requested data does not exist.
/*	In case of an update request, the request was rejected.
/* .IP "400 SPACE optional-text NEWLINE"
/*	This indicates an error condition. The text gives the nature of
/*	the problem. The client should retry the request later.
/* .IP "200 SPACE text NEWLINE"
/*	The request was successful. In the case of a lookup request,
/*	the text contains an encoded version of the requested data.
/*	Otherwise the text is optional.
/* SEE ALSO
/*	dict(3) generic dictionary manager
/*	hex_quote(3) http-style quoting
/* DIAGNOSTICS
/*	Fatal errors: out of memory, unknown host or service name,
/*	attempt to update or iterate over map.
/* BUGS
/*	Only the lookup method is currently implemented.
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

#include "sys_defs.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

/* Utility library. */

#include "msg.h"
#include "mymalloc.h"
#include "vstring.h"
#include "vstream.h"
#include "vstring_vstream.h"
#include "connect.h"
#include "hex_quote.h"
#include "dict.h"
#include "stringops.h"
#include "dict_tcp.h"

/* Application-specific. */

typedef struct {
    DICT    dict;			/* generic members */
    VSTRING *raw_buf;			/* raw I/O buffer */
    VSTRING *hex_buf;			/* quoted I/O buffer */
    VSTREAM *fp;			/* I/O stream */
} DICT_TCP;

#define DICT_TCP_MAXTRY	10
#define DICT_TCP_TMOUT	100

#define STR(x)		vstring_str(x)

/* dict_tcp_connect - connect to TCP server */

static int dict_tcp_connect(DICT_TCP *dict_tcp)
{
    int     fd;

    /*
     * Connect to the server. Enforce a time limit on read/write operations
     * so that we do not get stuck.
     */
    if ((fd = inet_connect(dict_tcp->dict.name, BLOCKING, 0)) < 0) {
	msg_warn("connect to TCP map %s: %m", dict_tcp->dict.name);
	return (-1);
    }
    dict_tcp->fp = vstream_fdopen(fd, O_RDWR);
    vstream_control(dict_tcp->fp,
		    VSTREAM_CTL_TIMEOUT, DICT_TCP_TMOUT,
		    VSTREAM_CTL_END);

    /*
     * Allocate per-map I/O buffers on the fly.
     */
    if (dict_tcp->raw_buf == 0) {
	dict_tcp->raw_buf = vstring_alloc(10);
	dict_tcp->hex_buf = vstring_alloc(10);
    }
    return (0);
}

/* dict_tcp_disconnect - disconnect from TCP server */

static void dict_tcp_disconnect(DICT_TCP *dict_tcp)
{
    (void) vstream_fclose(dict_tcp->fp);
    dict_tcp->fp = 0;
}

/* dict_tcp_lookup - request TCP server */

static const char *dict_tcp_lookup(DICT *dict, const char *key)
{
    DICT_TCP *dict_tcp = (DICT_TCP *) dict;
    char   *myname = "dict_tcp_lookup";
    int     tries;
    char   *start;

#define RETURN(errval, result) { dict_errno = errval; return (result); }

    if (msg_verbose)
	msg_info("%s: key %s", myname, key);

    for (tries = 0; /* see below */ ; /* see below */ ) {

	/*
	 * Connect to the server, or use an existing connection.
	 */
	if (dict_tcp->fp != 0 || dict_tcp_connect(dict_tcp) == 0) {

	    /*
	     * Send request and receive response. Both are %XX quoted and
	     * both are terminated by newline. This encoding is convenient
	     * for data that is mostly text.
	     */
	    hex_quote(dict_tcp->hex_buf, key);
	    vstream_fprintf(dict_tcp->fp, "get %s\n", STR(dict_tcp->hex_buf));
	    if (vstring_get_nonl(dict_tcp->hex_buf, dict_tcp->fp) > 0)
		break;

	    /*
	     * Disconnect from the server if it can't talk to us.
	     */
	    msg_warn("read TCP map reply from %s: unexpected EOF (%m)",
		     dict_tcp->dict.name);
	    dict_tcp_disconnect(dict_tcp);
	}

	/*
	 * Try to connect a limited number of times before giving up.
	 */
	if (++tries >= DICT_TCP_MAXTRY)
	    RETURN(DICT_ERR_RETRY, 0);

	/*
	 * Sleep between attempts, instead of hammering the server.
	 */
	sleep(1);
    }

    /*
     * Check the general reply syntax. If the reply is malformed, disconnect
     * and try again later.
     */
    if (start = STR(dict_tcp->hex_buf),
	!ISDIGIT(start[0]) || !ISDIGIT(start[1])
	|| !ISDIGIT(start[2]) || !ISSPACE(start[3])
	|| !hex_unquote(dict_tcp->raw_buf, start + 4)) {
	msg_warn("read TCP map reply from %s: malformed reply %.100s",
		 dict_tcp->dict.name, printable(STR(dict_tcp->hex_buf), '_'));
	dict_tcp_disconnect(dict_tcp);
	RETURN(DICT_ERR_RETRY, 0);
    }

    /*
     * Examine the reply status code. If the reply is malformed, disconnect
     * and try again later.
     */
    switch (start[0]) {
    default:
	msg_warn("read TCP map reply from %s: bad status code %.100s",
		 dict_tcp->dict.name, printable(STR(dict_tcp->hex_buf), '_'));
	dict_tcp_disconnect(dict_tcp);
	RETURN(DICT_ERR_RETRY, 0);
    case '4':
	if (msg_verbose)
	    msg_info("%s: soft error: %s",
		     myname, printable(STR(dict_tcp->raw_buf), '_'));
	dict_tcp_disconnect(dict_tcp);
	RETURN(DICT_ERR_RETRY, 0);
    case '5':
	if (msg_verbose)
	    msg_info("%s: not found: %s",
		     myname, printable(STR(dict_tcp->raw_buf), '_'));
	RETURN(DICT_ERR_NONE, 0);
    case '2':
	if (msg_verbose)
	    msg_info("%s: found: %s",
		     myname, printable(STR(dict_tcp->raw_buf), '_'));
	RETURN(DICT_ERR_NONE, STR(dict_tcp->raw_buf));
    }
}

/* dict_tcp_close - close TCP map */

static void dict_tcp_close(DICT *dict)
{
    DICT_TCP *dict_tcp = (DICT_TCP *) dict;

    if (dict_tcp->fp)
	(void) vstream_fclose(dict_tcp->fp);
    if (dict_tcp->raw_buf)
	vstring_free(dict_tcp->raw_buf);
    if (dict_tcp->hex_buf)
	vstring_free(dict_tcp->hex_buf);
    dict_free(dict);
}

/* dict_tcp_open - open TCP map */

DICT   *dict_tcp_open(const char *map, int unused_flags, int dict_flags)
{
    DICT_TCP *dict_tcp;

    dict_errno = 0;

    dict_tcp = (DICT_TCP *) dict_alloc(DICT_TYPE_TCP, map, sizeof(*dict_tcp));
    dict_tcp->fp = 0;
    dict_tcp->raw_buf = dict_tcp->hex_buf = 0;
    dict_tcp->dict.lookup = dict_tcp_lookup;
    dict_tcp->dict.close = dict_tcp_close;
    dict_tcp->dict.flags = dict_flags | DICT_FLAG_FIXED;
    return (DICT_DEBUG(&dict_tcp->dict));
}

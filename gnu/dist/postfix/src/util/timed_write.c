/*++
/* NAME
/*	timed_write 3
/* SUMMARY
/*	write operation with pre-write timeout
/* SYNOPSIS
/*	#include <iostuff.h>
/*
/*	int	timed_write(fd, buf, buf_len, timeout, context)
/*	int	fd;
/*	const void *buf;
/*	unsigned len;
/*	int	timeout;
/*	void	*context;
/* DESCRIPTION
/*	timed_write() performs a write() operation when the specified
/*	descriptor becomes writable within a user-specified deadline.
/*
/*	Arguments:
/* .IP fd
/*	File descriptor in the range 0..FD_SETSIZE.
/* .IP buf
/*	Write buffer pointer.
/* .IP buf_len
/*	Write buffer size.
/* .IP timeout
/*	The deadline in seconds. If this is <= 0, the deadline feature
/*	is disabled.
/* .IP context
/*	Application context. This parameter is unused. It exists only
/*	for the sake of VSTREAM compatibility.
/* DIAGNOSTICS
/*	When the operation does not complete within the deadline, the
/*	result value is -1, and errno is set to ETIMEDOUT.
/*	All other returns are identical to those of a write(2) operation.
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
#include <unistd.h>

/* Utility library. */

#include "iostuff.h"

/* timed_write - write with deadline */

int     timed_write(int fd, void *buf, unsigned len,
		            int timeout, void *unused_context)
{

    /*
     * Wait for a limited amount of time for something to happen. If nothing
     * happens, report an ETIMEDOUT error.
     */
    if (timeout > 0 && write_wait(fd, timeout) < 0)
	return (-1);
    else
	return (write(fd, buf, len));
}

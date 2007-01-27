#!/bin/sh
#
# Copyright (C) 2004  Internet Systems Consortium, Inc. ("ISC")
# Copyright (C) 2000, 2001  Internet Software Consortium.
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
# OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.

# Id: runall.sh,v 1.6 2004/03/05 04:59:12 marka Exp

#
# Run all the system tests.
#

SYSTEMTESTTOP=.
. $SYSTEMTESTTOP/conf.sh

status=0

for d in $SUBDIRS
do
	sh run.sh $d || status=1
done

$PERL testsock.pl || {
    cat <<EOF >&2
I:
I:NOTE: Many of the tests were skipped because they require that
I:      the IP addresses 10.53.0.1 through 10.53.0.5 are configured 
I:	as alias addresses on the loopback interface.  Please run
I:	"bin/tests/system/ifconfig.sh up" as root to configure them
I:	and rerun the tests.
EOF
    exit 0;
}

exit $status

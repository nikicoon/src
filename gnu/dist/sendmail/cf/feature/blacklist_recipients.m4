divert(-1)
#
# Copyright (c) 1998, 1999 Sendmail, Inc. and its suppliers.
#	All rights reserved.
#
# By using this file, you agree to the terms and conditions set
# forth in the LICENSE file which can be found at the top level of
# the sendmail distribution.
#
#

divert(0)
VERSIONID(`Id: blacklist_recipients.m4,v 8.13 1999/04/02 02:25:13 gshapiro Exp')
VERSIONID(`$NetBSD: blacklist_recipients.m4,v 1.3 2003/06/01 14:06:45 atatat Exp $')
divert(-1)

ifdef(`_ACCESS_TABLE_',
	`define(`_BLACKLIST_RCPT_', 1)',
	`errprint(`*** ERROR: FEATURE(blacklist_recipients) requires FEATURE(access_db)
')')

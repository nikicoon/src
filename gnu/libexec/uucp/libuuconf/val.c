/* val.c
   Validate a login name for a system.

   Copyright (C) 1992 Ian Lance Taylor

   This file is part of the Taylor UUCP uuconf library.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License
   as published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   The author of the program may be contacted at ian@airs.com or
   c/o Cygnus Support, Building 200, 1 Kendall Square, Cambridge, MA 02139.
   */

#include "uucnfi.h"

#if USE_RCS_ID
const char _uuconf_val_rcsid[] = "$Id: val.c,v 1.2 1994/10/24 22:26:02 jtc Exp $";
#endif

/* Validate a login name for a system.  */

/*ARGSUSED*/
int
uuconf_validate (pglobal, qsys, zlogin)
     pointer pglobal;
     const struct uuconf_system *qsys;
     const char *zlogin;
{
#if HAVE_TAYLOR_CONFIG
  return uuconf_taylor_validate (pglobal, qsys, zlogin);
#else
  return UUCONF_SUCCESS;
#endif
}

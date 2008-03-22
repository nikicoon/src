/*
  Name:     iprime.h
  Purpose:  Pseudoprimality testing routines
  Author:   M. J. Fromberger <http://www.dartmouth.edu/~sting/>
  Info:     $Id: iprime.h,v 1.1 2008/03/22 09:42:41 mlelstv Exp $

  Copyright (C) 2002 Michael J. Fromberger, All Rights Reserved.

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
 */

#ifndef IPRIME_H_
#define IPRIME_H_

#include "imath.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Test whether z is likely to be prime
   MP_YES means it is probably prime
   MP_NO  means it is definitely composite
 */
mp_result mp_int_is_prime(mp_int z);

/* Find the first apparent prime in ascending order from z */
mp_result mp_int_find_prime(mp_int z);

#ifdef __cplusplus
}
#endif
#endif /* IPRIME_H_ */

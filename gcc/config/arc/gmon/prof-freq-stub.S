/* This file contains code to do profiling.

   Copyright 2007-2012 Synopsys Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it under the
   terms of the GNU General Public License as published by the Free Software
   Foundation; either version 3, or (at your option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
   details.

   You should have received a copy of the GNU General Public License along
   with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */
#include "../asm.h"
/*  This file contains code to do profiling.  */
#ifndef __A4__
	.weak	__profile_frequency_value
	.global __profile_frequency_value
	.set	__profile_frequency_value, 1000
	.text
	.balign	4
	.global __profile_frequency
	FUNC(__profile_frequency)
__profile_frequency:
	mov_s	r0,__profile_frequency_value
	j_s	[blink]
	ENDFUNC(__profile_frequency)

#endif /* !A4 */

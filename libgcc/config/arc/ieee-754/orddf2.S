/* Copyright 2008 Free Software Foundation, Inc.  

   Copyright 2008-2012 Synopsys Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file into combinations with other programs,
and to distribute those combinations without any restriction coming
from the use of this file.  (The General Public License restrictions
do apply in other respects; for example, they cover modification of
the file, and distribution when not linked into a combine
executable.)

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "arc-ieee-754.h"
/* inputs: r0, r1
   output: c flag
   clobber: r12, flags
   For NaNs, bit 19 .. bit 30 must be set.  */
#if 0 /* DEBUG */
	.global __orddf2
	.balign 4
	FUNC(__orddf2)
__orddf2:
	st.a r11,[sp,-4]` push_s blink` st.a r10,[sp,-4]` st.a r9,[sp,-4]
	st.a r8,[sp,-4]` st.a r7,[sp,-4]` st.a r6,[sp,-4]` st.a r5,[sp,-4]
	st.a r4,[sp,-4]` push_s r3` push_s r2` push_s r1`
	bl.d __unorddf2_c` push_s r0
	mov r11,r0` pop_s r0` pop_s r1` pop_s r2` pop_s r3
	ld.ab r4,[sp,4]` ld.ab r5,[sp,4]` ld.ab r6,[sp,4]`
	ld.ab r7,[sp,4]` ld.ab r8,[sp,4]` ld.ab r9,[sp,4]
	bl.d __orddf2_asm` ld.ab r10,[sp,4]
	pop_s blink
	brne.d r11,0,0f
	ld.ab r11,[sp,4]
	jcc [blink]
	bl abort
0:	jcs [blink]
	bl abort
	ENDFUNC(__orddf2)
#define __orddf2 __orddf2_asm
#endif /* DEBUG */
	.global __orddf2
	.balign 4
	HIDDEN_FUNC(__orddf2)
__orddf2:
	bmsk r12,DBL0H,20
	add1.f	r12,r12,DBL0H /* clear z; set c if NaN.  */
	bmsk r12,DBL1H,20
	j_s.d	[blink]
	add1.cc.f r12,r12,DBL1H /* clear z; set c if NaN.  */
	ENDFUNC(__orddf2)

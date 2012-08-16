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
   output: z flag
   clobber: r12, flags
   For NaNs, bit 22 .. bit 30 must be set.  */
#if 0 /* DEBUG */
	.global __eqsf2
	.balign 4
	FUNC(__eqsf2)
__eqsf2:
	st.a r11,[sp,-4]` push_s blink` st.a r10,[sp,-4]` st.a r9,[sp,-4]
	st.a r8,[sp,-4]` st.a r7,[sp,-4]` st.a r6,[sp,-4]` st.a r5,[sp,-4]
	st.a r4,[sp,-4]` push_s r3` push_s r2` push_s r1`
	bl.d __eqsf2_c` push_s r0
	mov r11,r0` pop_s r0` pop_s r1` pop_s r2` pop_s r3
	ld.ab r4,[sp,4]` ld.ab r5,[sp,4]` ld.ab r6,[sp,4]`
	ld.ab r7,[sp,4]` ld.ab r8,[sp,4]` ld.ab r9,[sp,4]
	bl.d __eqsf2_asm` ld.ab r10,[sp,4]
	pop_s blink
	breq.d r11,0,0f
	ld.ab r11,[sp,4]
	jne_s [blink]
	bl abort
0:	jeq_s [blink]
	bl abort
	ENDFUNC(__eqsf2)
#define __eqsf2 __eqsf2_asm
#endif /* DEBUG */
	/* Good performance as long as the binary difference is
	   well predictable (as seen from the branch predictor).  */
	.global __eqsf2
	.balign 4
	HIDDEN_FUNC(__eqsf2)
__eqsf2:
	breq	r0, r1,.Lno_bdiff
	or	r12,r0,r1
	j_s.d	[blink]
	bmsk.f	0,r12,30
.Lno_bdiff:
	bmsk r12,r0,23
	add1.f	r12,r12,r0 /* set c iff NaN; also, clear z if NaN.  */
	j_s.d	[blink]
	cmp.cc	r0,r1
	ENDFUNC(__eqsf2)

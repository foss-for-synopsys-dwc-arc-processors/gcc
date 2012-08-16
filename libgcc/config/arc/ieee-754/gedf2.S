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
/* inputs: DBL0, DBL1
   output: c flags to be used for 'hs' condition
   clobber: r12, flags  */
/* For NaNs, bit 19.. bit 30 of the high word must be set.  */
#if 0 /* DEBUG */
	.global __gedf2
	.balign 4
	FUNC(__gedf2)
__gedf2:
	st.a r11,[sp,-4]` push_s blink` st.a r10,[sp,-4]` st.a r9,[sp,-4]
	st.a r8,[sp,-4]` st.a r7,[sp,-4]` st.a r6,[sp,-4]` st.a r5,[sp,-4]
	st.a r4,[sp,-4]` push_s r3` push_s r2` push_s r1`
	bl.d __gedf2_c` push_s r0
	mov r11,r0` pop_s r0` pop_s r1` pop_s r2` pop_s r3
	ld.ab r4,[sp,4]` ld.ab r5,[sp,4]` ld.ab r6,[sp,4]`
	ld.ab r7,[sp,4]` ld.ab r8,[sp,4]` ld.ab r9,[sp,4]
	bl.d __gedf2_asm` ld.ab r10,[sp,4]
	pop_s blink
	brge.d r11,0,0f
	ld.ab r11,[sp,4]
	jlo [blink]
	bl abort
0:	jhs [blink]
	bl abort
	ENDFUNC(__gedf2)
#define __gedf2 __gedf2_asm
#endif /* DEBUG */
	.global __gedf2
	.balign	4
	HIDDEN_FUNC(__gedf2)
__gedf2:
	or.f	r12,DBL0H,DBL1H
	bmi.d	.Lneg
	bmsk_s	r12,r12,20
	add1.f	0,r12,DBL0H	; clear z; set c iff NaN
	add1.cc.f r12,r12,DBL1H	; clear z; set c iff NaN
	bbit1	DBL0H,31,.Lneg
	cmp.cc	DBL0H,DBL1H
	j_s.d	[blink]
	cmp.eq	DBL0L,DBL1L
	.balign 4
.Lneg:	breq.d	DBL1H,0,.L0
	add1.f	0,r12,DBL0H
	add1.cc.f r12,r12,DBL1H
	cmp.cc	DBL1H,DBL0H
	j_s.d	[blink]
	cmp.eq	DBL1L,DBL0L
	.balign 4
.L0:
	bxor.f	0,DBL0H,31 ; check for high word of -0.
	beq_s	.Lcheck_0
	cmp.cc	DBL1H,DBL0H
	j_s.d	[blink]
	cmp.eq	DBL1L,DBL0L
.Lcheck_0:
	; high words suggest DBL0 may be -0, DBL1 +0; check low words.
	cmp_s	DBL1H,DBL0L
	j_s.d	[blink]
	cmp.cc	DBL1H,DBL1L
	ENDFUNC(__gedf2)

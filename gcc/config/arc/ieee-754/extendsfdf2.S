/* Copyright 2006, 2008 Free Software Foundation, Inc.  

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

#if 0 /* DEBUG */
	.global __extendsfdf2
	.balign 4
	FUNC(__extendsfdf2)
__extendsfdf2:
	push_s blink
	bl.d __extendsfdf2_c
	push_s r0
	ld_s r2,[sp]
	st_s r1,[sp]
	push_s r0
	bl.d __extendsfdf2_asm
	mov_s r0,r2
	pop_s r2
	pop_s r3
	pop_s blink
	cmp r0,r2
	cmp.eq r1,r3
	jeq_s [blink]
	bl abort
	ENDFUNC(__extendsfdf2)
#define __extendsfdf2 __extendsfdf2_asm
#endif /* DEBUG */
#if 0 /* ARC600 */
__extendsfdf2:
	lsr r2,r0,23
	tst r2,0xff
	bic.ne.f r2,0xff
	beq_s .Linf_nan_denorm_0
..
.Linf_nan_denorm:
	bbit1 r0,30,.Linf_nan
#endif
	.global __extendsfdf2
	.balign 4
	FUNC(__extendsfdf2)
__extendsfdf2:
	add.f r1,r0,r0
	norm r3,r1
#ifdef __LITTLE_ENDIAN__
	lsr_s DBL0H,r1,4
	brhs r3,7,.Linf_nan_denorm_0
	asl_s DBL0L,r0,29
	add_s DBL0H,DBL0H, \
		0x38000000
#else
	lsr r2,r1,4
	brhs r3,7,.Linf_nan_denorm_0
	asl_s DBL0L,r1,28
	add DBL0H,r2, \
		0x38000000
#endif
	j_s.d [blink]
	bxor.cs DBL0H,DBL0H,31
	.balign 4
.Linf_nan_denorm_0:
#ifdef __LITTLE_ENDIAN__
	mov_s DBL0H,r0
	jeq.d [blink]
	mov.eq DBL0L,0
#else
	jeq_s [blink]
#endif
	bmi .Linf_nan
	asl_s r0,r0,r3
	rsub r3,r3,0x380+6
#ifdef __LITTLE_ENDIAN__
	asl_s r3,r3,20
	lsr DBL0H,r0,9
	asl_s DBL0L,r0,23
	add_s DBL0H,DBL0H,r3
	j_s.d [blink]
	bxor.cs DBL0H,DBL0H,31
#else
	asl DBL0L,r0,23
	lsr_s DBL0H,r0,9
	asl_s r3,r3,20
	bxor.cs DBL0H,DBL0H,31
	j_s.d [blink]
	add_l DBL0H,DBL0H,r3
#endif
.Linf_nan:
#ifdef __LITTLE_ENDIAN__
	lsr DBL0H,r0,3

	or_s DBL0H,DBL0H,r0
	j_s.d [blink]
	mov_l DBL0L,0
#else
	lsr r3,r0,3
	mov_s DBL0L,0
	j_s.d [blink]
	or_l DBL0H,r0,r3
#endif
	ENDFUNC(__extendsfdf2)

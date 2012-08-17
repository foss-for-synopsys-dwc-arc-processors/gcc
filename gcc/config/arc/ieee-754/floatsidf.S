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

#if 0 /* DEBUG */
	.global __floatsidf
	.balign 4
	FUNC(__floatsidf)
__floatsidf:
	push_s blink
	bl.d __floatsidf_c
	push_s r0
	ld_s r2,[sp]
	st_s r1,[sp]
	push_s r0
	bl.d __floatsidf_asm
	mov_s r0,r2
	pop_s r2
	pop_s r3
	pop_s blink
	cmp r0,r2
	cmp.eq r1,r3
	jeq_s [blink]
	bl abort
	ENDFUNC(__floatsidf)
#define __floatsidf __floatsidf_asm
#endif /* DEBUG */

	.global __floatsidf
	.balign 4
	FUNC(__floatsidf)
__floatsidf:
	abs.f r1,r0
	jeq_s [blink]
	lsr r2,r1
	mov r12,-0x41d ; -(0x3ff+31-1)
	norm r2,r2
	bclr.cs r12,r12,11
	rsub.f r3,r2,11
	add_s r12,r2,r12
	add_s r2,r2,21
#ifdef __LITTLE_ENDIAN__
	asl DBL0L,r1,r2
	lsr_s DBL0H,r1,r3
#else
	lsr DBL0H,r1,r3
	asl_s DBL0L,r1,r2
#endif
	asl_s r12,r12,20
	mov.lo DBL0H,DBL0L
	sub_s DBL0H,DBL0H,r12
	j_s.d [blink]
	mov.ls DBL0L,0
	ENDFUNC(__floatsidf)

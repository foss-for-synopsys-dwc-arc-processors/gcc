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
	.global __floatsisf
	FUNC(__floatsisf)
	.balign 4
__floatsisf:
	push_s blink
	bl.d __floatsisf_c
	push_s r0
	ld_s r1,[sp]
	st_s r0,[sp]
	bl.d __floatsisf_asm
	mov_s r0,r1
	pop_s r1
	pop_s blink
	cmp r0,r1
	jeq_s [blink]
	bl abort
	ENDFUNC(__floatsisf)
	.global __floatunsisf
	FUNC(__floatunsisf)
	.balign 4
__floatunsisf:
	push_s blink
	bl.d __floatunsisf_c
	push_s r0
	ld_s r1,[sp]
	st_s r0,[sp]
	bl.d __floatunsisf_asm
	mov_s r0,r1
	pop_s r1
	pop_s blink
	cmp r0,r1
	jeq_s [blink]
	bl abort
	ENDFUNC(__floatunsisf)
#define __floatsisf __floatsisf_asm
#define __floatunsisf __floatunsisf_asm
#endif /* DEBUG */

	.global __floatunsisf
	.global __floatsisf
	FUNC(__floatsisf)
	FUNC(__floatunsisf)
	.balign 4
__floatunsisf:
	lsr_s r2,r0
	mov_l r12,0x9d ; 0x7f + 31 - 1
	norm r2,r2
	brne_l r0,0,0f
	j_s [blink]
	.balign 4
__floatsisf:
	abs.f r0,r0
	jeq_s [blink]
	lsr_s r2,r0
	mov_s r12,0x9d ; 0x7f + 31 - 1
	norm r2,r2
	bset.cs r12,r12,8
0:	rsub.f r3,r2,8
	bmsk r1,r0,r3
	ror r1,r1,r3
	lsr.pl r0,r0,r3
	neg_s r3,r3
	asl.mi r0,r0,r3
	sub_s r12,r12,r2
	asl_s r12,r12,23
	bxor.pl.f r1,r1,31
	add_s r0,r0,r12
	j_s.d [blink]
	add.pnz r0,r0,1
	ENDFUNC(__floatunsisf)
	ENDFUNC(__floatsisf)

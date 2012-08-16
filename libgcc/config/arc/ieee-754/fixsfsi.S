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
	.global __fixsfsi
	FUNC(__fixsfsi)
	.balign 4
__fixsfsi:
	push_s blink
	bl.d __fixsfsi_c
	push_s r0
	ld_s r1,[sp]
	st_s r0,[sp]
	bl.d __fixsfsi_asm
	mov_s r0,r1
	pop_s r1
	pop_s blink
	cmp r0,r1
	jeq_s [blink]
	bl abort
	ENDFUNC(__fixsfsi)
#define __fixsfsi __fixsfsi_asm
#endif /* DEBUG */

	.global __fixsfsi
	FUNC(__fixsfsi)
	.balign 4
__fixsfsi:
	bbit0 r0,30,.Lret0or1
	lsr r2,r0,23
	bmsk_s r0,r0,22
	bset_s r0,r0,23
	sub_s r2,r2,22;0x7f+23-0x80
	asl.f 0,r2,24
	neg r3,r2
	asl.mi r0,r0,r2
	lsr.pl r0,r0,r3
	j_s.d [blink]
	neg.cs r0,r0
.Lret0or1:
	add.f r0,r0,0x800000
	lsr_s r0,r0,30

	bmsk_s r0,r0,0
	j_s.d [blink]
	neg.mi r0,r0
	ENDFUNC(__fixsfsi)

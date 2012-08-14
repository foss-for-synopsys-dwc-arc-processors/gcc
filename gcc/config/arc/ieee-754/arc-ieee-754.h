/* Copyright 2008-2012 Synopsys Inc.

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
#ifdef __LITTLE_ENDIAN__
#define DBL0L r0
#define DBL0H r1
#define DBL1L r2
#define DBL1H r3
#else
#define DBL0L r1
#define DBL0H r0
#define DBL1L r3
#define DBL1H r2
#endif
#define add_l add
#define asr_l asr
#define j_l j
#define jne_l jne
#define jeq_l jeq
#define or_l or
#define mov_l mov
#define b_l b
#define beq_l beq
#define bne_l bne
#define brne_l brne
#define bset_l bset
#define sub_l sub
#define sub1_l sub1
#define lsr_l lsr
#define xor_l xor
#define bic_l bic
#define bmsk_l bmsk
#define bxor_l bxor
#define bcs_s blo_s

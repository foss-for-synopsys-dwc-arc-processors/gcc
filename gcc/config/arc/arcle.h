/* Copyright (C) 2017 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3, or (at your option) any later
   version.

   GCC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef _GCC_ARC_CLE_H
#define _GCC_ARC_CLE_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Recognized data types.  */
typedef char  __simd32_int4x8_t  __attribute__ ((vector_size (4)));
typedef short __simd32_int2x16_t __attribute__ ((vector_size (4)));
typedef short __simd64_int4x16_t __attribute__ ((vector_size (8)));
typedef int   __simd64_int2x32_t __attribute__ ((vector_size (8)));

/* Recognized extension functions.  */
__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_abss (int32_t __a)
{
  int32_t __dst;
  __asm__ ("abss	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline int16_t __attribute__ ((__always_inline__))
_abssh (int16_t __a)
{
  int16_t __dst;
  __asm__ ("abssh	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_adds (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("adds	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline void __attribute__ ((__always_inline__))
_aslacc (int32_t __a)
{
  __asm__ ("aslacc	%0\n"
	   :
	   : "r" (__a)
	   : "memory");
}

__extension__ static __inline void __attribute__ ((__always_inline__))
_aslsacc (int32_t __a)
{
  __asm__ ("aslsacc	%0\n"
	   :
	   : "r" (__a)
	   : "memory");
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_asls (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("asls	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_asrs (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("asrs	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_asrsr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("asrsr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_cbflyhf0r (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("cbflyhf0r	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b)
	   : "memory");
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_cbflyhf1r (int32_t __a)
{
  int32_t __dst;
  __asm__ ("cbflyhf1r	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a)
	   : "memory");
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_cmacchfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("cmacchfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_cmacchnfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("cmacchnfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_cmachfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("cmachfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_cmachnfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("cmachnfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_cmpychfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("cmpychfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_cmpychnfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("cmpychnfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_cmpyhfmr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("cmpyhfmr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_cmpyhfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("cmpyhfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_cmpyhnfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("cmpyhnfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmach (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  return __builtin_arc_dmach (__a, __b);
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmachbl (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmachbl	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmachbm (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmachbm	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmachf (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmachf	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmachfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmachfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmachu (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  return __builtin_arc_dmachu (__a, __b);
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_dmacwh (__simd64_int2x32_t __a, __simd32_int2x16_t __b)
{
  return __builtin_arc_dmacwh (__a, __b);
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_dmacwhu (__simd64_int2x32_t __a, __simd32_int2x16_t __b)
{
  return __builtin_arc_dmacwhu (__a, __b);
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmacwhf (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmacwhf	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmpyh (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  return __builtin_arc_dmpyh (__a, __b);
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmpyhbl (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmpyhbl	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmpyhbm (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmpyhbm	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmpyhf (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmpyhf	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmpyhfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmpyhfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmpyhu (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  return __builtin_arc_dmpyhu (__a, __b);
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmpyhwf (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmpyhwf	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmpywh (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmpywh	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmpywhu (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmpywhu	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_dmpywhf (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("dmpywhf	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline void __attribute__ ((__always_inline__))
_flagacc (int32_t __a)
{
  __asm__ ("flagacc	%0\n"
	   :
	   : "r" (__a)
	   : "memory" );
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_getacc (int32_t __a)
{
  int32_t __dst;
  __asm__ ("getacc	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline void __attribute__ ((__always_inline__))
_kflag (uint32_t __a)
{
  __builtin_arc_kflag (__a);
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_lr (int32_t __a)
{
  return __builtin_arc_lr (__a);
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_mac (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("mac	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b)
	   : "r58", "r59");
  return __dst;
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_macd (int32_t __a, int32_t __b)
{
  int64_t __dst;
  __asm__ ("macd	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b)
	   : "r58", "r59");
  return __dst;
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_macdf (int32_t __a, int32_t __b)
{
  int64_t __dst;
  __asm__ ("macdf	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_macdu (int32_t __a, int32_t __b)
{
  int64_t __dst;
  __asm__ ("macdu	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_macf (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("macf	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_macfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("macfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_macu (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("macu	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b)
	   : "r58", "r59");
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_macwhfm (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("macwhfm	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_macwhfmr (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("macwhfmr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_macwhl (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("macwhl	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_macwhul (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("macwhul	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_mpydf (int32_t __a, int32_t __b)
{
  int64_t __dst;
  __asm__ ("mpydf	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_mpyf (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("mpyf	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_mpyfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("mpyfr	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_mpywhfl (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("mpywhfl	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_mpywhflr (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("mpywhflr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_mpywhfm (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("mpywhfm	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_mpywhfmr (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("mpywhfmr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_mpywhl (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("mpywhl	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_mpywhul (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("mpywhul	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_msubdf (int32_t __a, int32_t __b)
{
  int64_t __dst;
  __asm__ ("msubdf	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_msubf (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("msubf	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_msubfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("msubfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_negs (int32_t __a)
{
  int32_t __dst;
  __asm__ ("negs	%0,%1\n"
	   : "=r" (__dst)
	   : "rCal" (__a));
  return __dst;
}

__extension__ static __inline int16_t __attribute__ ((__always_inline__))
_negsh (int16_t __a)
{
  int16_t __dst;
  __asm__ ("negsh	%0,%1\n"
	   : "=r" (__dst)
	   : "rCal" (__a));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_normacc (int32_t __a)
{
  int32_t __dst;
  __asm__ ("normacc	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_qmach (__simd64_int4x16_t __a, __simd64_int4x16_t __b)
{
  return __builtin_arc_qmach (__a, __b);
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_qmachu (__simd64_int4x16_t __a, __simd64_int4x16_t __b)
{
  return __builtin_arc_qmachu (__a, __b);
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_qmachf (int64_t __a, int64_t __b)
{
  int32_t __dst;
  __asm__ ("qmachf	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_qmpyh (__simd64_int4x16_t __a, __simd64_int4x16_t __b)
{
  return __builtin_arc_qmpyh (__a, __b);
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_qmpyhu (__simd64_int4x16_t __a, __simd64_int4x16_t __b)
{
  return __builtin_arc_qmpyhu (__a, __b);
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_qmpyhf (int64_t __a, int64_t __b)
{
  int32_t __dst;
  __asm__ ("qmpyhf	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline void __attribute__ ((__always_inline__))
_sr (int32_t __a, int32_t __b)
{
  __builtin_arc_sr (__a, __b);
}

__extension__ static __inline int16_t __attribute__ ((__always_inline__))
_rndh (int16_t __a)
{
  int16_t __dst;
  __asm__ ("rndh	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a) );
  return __dst;
}

__extension__ static __inline int16_t __attribute__ ((__always_inline__))
_sath (int32_t __a)
{
  int16_t __dst;
  __asm__ ("sath	%0,%1\n"
	   : "=r" (__dst)
	   : "rCal" (__a));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_setacc (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("setacc	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_subs (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("subs	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vabs2h (__simd32_int2x16_t __a)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vabs2h	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vabss2h (int32_t __a)
{
  int32_t __dst;
  __asm__ ("vabss2h	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline __simd64_int2x32_t __attribute__ ((__always_inline__))
_vadd2 (__simd64_int2x32_t __a, __simd64_int2x32_t __b)
{
  return __a + __b;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vadds2 (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vadds2	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vadd2h (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  return __a + __b;
}

__extension__ static __inline __simd32_int4x8_t __attribute__ ((__always_inline__))
_vadd4b (__simd32_int4x8_t __a, __simd32_int4x8_t __b)
{
  __simd32_int4x8_t __dst;
  __asm__ ("vadd4b	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd64_int4x16_t __attribute__ ((__always_inline__))
_vadd4h (__simd64_int4x16_t __a, __simd64_int4x16_t __b)
{
  return __a + __b;
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_vadds4h (int64_t __a, int64_t __b)
{
  int64_t __dst;
  __asm__ ("vadds4h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vadds2h (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vadds2h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd64_int2x32_t __attribute__ ((__always_inline__))
_vaddsub (__simd64_int2x32_t __a, __simd64_int2x32_t __b)
{
  return __builtin_arc_vaddsub (__a, __b);
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_vaddsubs (int64_t __a, int64_t __b)
{
  int64_t __dst;
  __asm__ ("vaddsubs	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vaddsub2h (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  return __builtin_arc_vaddsub2h (__a, __b);
}

__extension__ static __inline __simd64_int4x16_t __attribute__ ((__always_inline__))
_vaddsub4h (__simd64_int4x16_t __a, __simd64_int4x16_t __b)
{
  return __builtin_arc_vaddsub4h (__a, __b);
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_vaddsubs4h (int64_t __a, int64_t __b)
{
  int64_t __dst;
  __asm__ ("vaddsubs4h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vaddsubs2h (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vaddsubs2h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_valgn2h (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  __simd32_int2x16_t __dst;
  __asm__ ("valgn2h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vasl2h (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vasl2h	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vasls2h (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vasls2h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vasr2h (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vasr2h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vasrs2h (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vasrs2h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vasrsr2h (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vasrsr2h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int4x8_t __attribute__ ((__always_inline__))
_vext2bhl (__simd32_int4x8_t __a)
{
  __simd32_int4x8_t __dst;
  __asm__ ("vext2bhl	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline __simd32_int4x8_t __attribute__ ((__always_inline__))
_vext2bhm (__simd32_int4x8_t __a)
{
  __simd32_int4x8_t __dst;
  __asm__ ("vext2bhm	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vlsr2h (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vlsr2h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd64_int2x32_t __attribute__ ((__always_inline__))
_vmac2h (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  return __builtin_arc_vmac2h (__a, __b);
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vmac2hf (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vmac2hf	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vmac2hfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vmac2hfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vmac2hnfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vmac2hnfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd64_int2x32_t __attribute__ ((__always_inline__))
_vmac2hu (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  return __builtin_arc_vmac2hu (__a, __b);
}

__extension__ static __inline __simd64_int2x32_t __attribute__ ((__always_inline__))
_vmpy2h (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  return __builtin_arc_vmpy2h (__a, __b);
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vmpy2hf (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vmpy2hf	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vmpy2hfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vmpy2hfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd64_int2x32_t __attribute__ ((__always_inline__))
_vmpy2hu (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  return __builtin_arc_vmpy2hu (__a, __b);
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_vmpy2hwf (int32_t __a, int32_t __b)
{
  int64_t __dst;
  __asm__ ("vmpy2hwf	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "rl" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vmax2h (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vmax2h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vmin2h (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vmin2h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vmsub2hf (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vmsub2hf	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vmsub2hfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vmsub2hfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vmsub2hnfr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vmsub2hnfr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vneg2h (__simd32_int2x16_t __a)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vneg2h	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a) );
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vnegs2h (int32_t __a)
{
  int32_t __dst;
  __asm__ ("vnegs2h	%0,%1\n"
          : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vnorm2h (__simd32_int2x16_t __a)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vnorm2h	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vrep2hl (__simd32_int2x16_t __a)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vrep2hl	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vrep2hm (__simd32_int2x16_t __a)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vrep2hm	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vsext2bhl (__simd32_int4x8_t __a)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vsext2bhl	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vsext2bhm (__simd32_int4x8_t __a)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vsext2bhm	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline __simd64_int2x32_t __attribute__ ((__always_inline__))
_vsub2 (__simd64_int2x32_t __a, __simd64_int2x32_t __b)
{
  return __a - __b;
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_vsubs2 (int64_t __a, int64_t __b)
{
  int64_t __dst;
  __asm__ ("vsubs2	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vsub2h (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  return __a - __b;
}

__extension__ static __inline __simd32_int4x8_t __attribute__ ((__always_inline__))
_vsub4b (__simd32_int4x8_t __a, __simd32_int4x8_t __b)
{
  __simd32_int4x8_t __dst;
  __asm__ ("vsub4b	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd64_int4x16_t __attribute__ ((__always_inline__))
_vsub4h (__simd64_int4x16_t __a, __simd64_int4x16_t __b)
{
  return __a - __b;
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_vsubs4h (int64_t __a, int64_t __b)
{
  int64_t __dst;
  __asm__ ("vsubs4h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd64_int2x32_t __attribute__ ((__always_inline__))
_vsubadd (__simd64_int2x32_t __a, __simd64_int2x32_t __b)
{
  return __builtin_arc_vsubadd (__a, __b);
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_vsubadds (int64_t __a, int64_t __b)
{
  int64_t __dst;
  __asm__ ("vsubadds	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vsubadd2h (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  return __builtin_arc_vsubadd2h (__a, __b);
}

__extension__ static __inline __simd64_int4x16_t __attribute__ ((__always_inline__))
_vsubadd4h (__simd64_int4x16_t __a, __simd64_int4x16_t __b)
{
  return __builtin_arc_vsubadd4h (__a, __b);
}

__extension__ static __inline int64_t __attribute__ ((__always_inline__))
_vsubadds4h (int64_t __a, int64_t __b)
{
  int64_t __dst;
  __asm__ ("vsubadds4h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vsubadds2h (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vsubadds2h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vsubs2h (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vsubs2h	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_adcs (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("adcs	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_sbcs (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("sbcs	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "rCal" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_satf (int32_t __a)
{
  int32_t __dst;
  __asm__ ("satf	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_macwhfl (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("macwhfl	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_macwhflr (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("macwhflr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_macwhkl (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("macwhkl	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_macwhkul (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("macwhkul	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_mpywhkl (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("mpywhkl	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_mpywhkul (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("mpywhkul	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_msubwhfl (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("msubwhfl	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_msubwhflr (int32_t __a, int16_t __b)
{
  int32_t __dst;
  __asm__ ("msubwhflr	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_msubwhfm (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("msubwhfm	%0,%1,%2\n"
          : "=r" (__dst)
          : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_msubwhfmr (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("msubwhfmr	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline void __attribute__ ((__always_inline__))
_trap (unit32_t __a)
{
  __builtin_arc_trap_s (__a);
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vpack2hl (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vpack2hl	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int2x16_t __attribute__ ((__always_inline__))
_vpack2hm (__simd32_int2x16_t __a, __simd32_int2x16_t __b)
{
  __simd32_int2x16_t __dst;
  __asm__ ("vpack2hm	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vperm (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("vperm	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

__extension__ static __inline __simd32_int4x8_t __attribute__ ((__always_inline__))
_vpack2hbl (__simd32_int4x8_t __a)
{
  __simd32_int4x8_t __dst;
  __asm__ ("vpack2hbl	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline __simd32_int4x8_t __attribute__ ((__always_inline__))
_vpack2hbm (__simd32_int4x8_t __a)
{
  __simd32_int4x8_t __dst;
  __asm__ ("vpack2hbm	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vpack2hblf (int32_t __a)
{
  int32_t __dst;
  __asm__ ("vpack2hblf	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline __simd32_int4x8_t __attribute__ ((__always_inline__))
_vpack2hbmf (__simd32_int4x8_t __a)
{
  __simd32_int4x8_t __dst;
  __asm__ ("vpack2hbmf	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vext2bhlf (int32_t __a)
{
  int32_t __dst;
  __asm__ ("vext2bhlf	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_vext2bhmf (int32_t __a)
{
  int32_t __dst;
  __asm__ ("vext2bhmf	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline int16_t __attribute__ ((__always_inline__))
_sqrt (int32_t __a)
{
  int16_t __dst;
  __asm__ ("sqrt	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_sqrtf (int32_t __a)
{
  int32_t __dst;
  __asm__ ("sqrtf	%0,%1\n"
	   : "=r" (__dst)
	   : "r" (__a));
  return __dst;
}

__extension__ static __inline int32_t __attribute__ ((__always_inline__))
_divf (int32_t __a, int32_t __b)
{
  int32_t __dst;
  __asm__ ("divf	%0,%1,%2\n"
	   : "=r" (__dst)
	   : "r" (__a), "r" (__b));
  return __dst;
}

#ifdef __cplusplus
}
#endif

#endif

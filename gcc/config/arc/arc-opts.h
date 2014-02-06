/* GCC option-handling definitions for the Synopsys DesignWare ARC architecture.

   Copyright (C) 2007-2012 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

enum processor_type
{
  PROCESSOR_NONE,
  PROCESSOR_ARC600,
  PROCESSOR_ARC601,
  PROCESSOR_ARC700,
  PROCESSOR_ARCv2EM,
  PROCESSOR_ARCv2HS
};

#define FPU_SP    0x01 /* Single precision floating point. */
#define FPU_SF    0x02 /* Single precision fused floating point operations. */
#define FPU_SC    0x04 /* Single precision floating point format conversion operations. */
#define FPU_SD    0x08 /* Single precision floating point sqrt and div operations. */
#define FPU_DP    0x10 /* Double precision floating point. */
#define FPU_DF    0x20 /* Double precision fused floating point operations. */
#define FPU_DC    0x40 /* Double precision floating point format conversion operations. */
#define FPU_DD    0x80 /* Double precision floating point sqrt and div operations. */

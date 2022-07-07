/* Synopsys DesignWare ARC SIMD include file.
   Copyright (C) 2007-2022 Free Software Foundation, Inc.
   Written by Saurabh Verma (saurabh.verma@celunite.com) on behalf os Synopsys
   Inc.

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

/* As a special exception, if you include this header file into source
   files compiled by GCC, this header file does not by itself cause
   the resulting executable to be covered by the GNU General Public
   License.  This exception does not however invalidate any other
   reasons why the executable file might be covered by the GNU General
   Public License.  */

#ifndef _COMMON_DEFS_H
#define _COMMON_DEFS_H 1

const arc_arch_t arc_arch_types[] =
  {
    {"none", BASE_ARCH_NONE, 0, 0},
#define ARC_ARCH(NAME, ARCH, FLAGS, DFLAGS)    \
    {NAME, BASE_ARCH_##ARCH, FLAGS, DFLAGS},
#include "arc-arches.def"
#undef ARC_ARCH
    {NULL, BASE_ARCH_END, 0, 0}
  };

const arc_cpu_t arc_cpu_types[] =
  {
    {"none", NULL, PROCESSOR_NONE, 0, HAS_NONE, ARC_TUNE_NONE},
#define ARC_CPU(NAME, ARCH, FLAGS, EXTRA, TUNE)                                \
    {#NAME, &arc_arch_types [BASE_ARCH_##ARCH], PROCESSOR_##NAME, FLAGS, HAS_##EXTRA, ARC_TUNE_##TUNE },
#include "arc-cpus.def"
#undef ARC_CPU
    {NULL, NULL, PROCESSOR_NONE, 0, HAS_NONE, ARC_TUNE_NONE}
  };


#endif /* _COMMON_DEFS_H */

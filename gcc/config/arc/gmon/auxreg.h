/* Copyright 2007-2012 Synopsys Inc.

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
#define	LP_START	0x02
#define	LP_END		0x03
#define IDENTITY	0x04
#define STATUS32	0x0a
#define	COUNT0		0x21 /* Timer 0 count */
#define	CONTROL0	0x22 /* Timer 0 control */
#define	LIMIT0		0x23 /* Timer 0 limit */
#define INT_VECTOR_BASE	0x25
#define D_CACHE_BUILD	0x72
#define DC_FLDL		0x4c

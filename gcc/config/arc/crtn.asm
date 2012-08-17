/* Ensure .fini/.init return for the ARC ARCompact CPU.

   Copyright (C) 1994, 1995, 1997, 2004, 2007, 2008, 2009, 2010
   Free Software Foundation, Inc.

   Copyright 2007-2012 Synopsys Inc.

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

# This file just makes sure that the .fini and .init sections do in
# fact return. This file is the last thing linked into any executable.

	.section .init
	pop_s	blink
	j_s	[blink]


	.section .fini
	pop_s	blink
	j_s	[blink]

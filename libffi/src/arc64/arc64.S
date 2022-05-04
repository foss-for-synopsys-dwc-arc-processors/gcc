/* -----------------------------------------------------------------------
   arcompact.S - Copyright (c) 2013 Synposys, Inc. (www.synopsys.com)

   ARC64 Foreign Function Interface

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   ``Software''), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL RENESAS TECHNOLOGY BE LIABLE FOR ANY CLAIM, DAMAGES OR
   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   OTHER DEALINGS IN THE SOFTWARE.
   ----------------------------------------------------------------------- */

#define LIBFFI_ASM
#include <fficonfig.h>
#include <ffi.h>
#ifdef HAVE_MACHINE_ASM_H
#include <machine/asm.h>
#else
#define CNAME(x) x
#define ENTRY(x) .globl CNAME(x)` .type CNAME(x),%function` CNAME(x):
#endif

.text

	/* R0:   ffi_prep_args */
	/* R1:   &ecif */
	/* R2:   cif->bytes */
	/* R3:   fig->flags */
	/* R4:   ecif.rvalue */
	/* R5:   fn */
	/* R6:   stack args bytes */
ENTRY(ffi_call_ARC64)
	/* Save registers.  */
	pushl_s     fp		  /* fp + 40, fp */
	pushl_s     blink	  /* fp + 32, blink */
	pushl_s     r4		  /* fp + 24, ecif.rvalue */
	pushl_s     r3		  /* fp + 16, fig->flags */
	pushl_s     r5		  /* fp + 8, fn */
	pushl_s     r2		  /* fp + 0, cif->bytes */
	movl_s      fp, sp

	/* Make room for all of the new args.  */
	subl	sp, sp, r6
	subl	sp, sp, r2
	addl	r2, r2, r6

	/* Place all of the ffi_prep_args in position.  */
	/* ffi_prep_args(char *stack, extended_cif *ecif) */
	/* R1 and R2 already set.  */

	/* And call.  */
	jl_s.d     [r0]
	movl_s     r0, sp

	ldl.ab     r12, [fp, 8]	/* cif->bytes */
	ldl.ab     r11, [fp, 8]	/* fn */

	/* Move first 8 parameters in registers...  */
	ldl	r0, [sp]
	ldl	r1, [sp, 8]
	ldl	r2, [sp, 16]
	ldl	r3, [sp, 24]
	ldl	r4, [sp, 32]
	ldl	r5, [sp, 40]
	ldl	r6, [sp, 48]
	ldl	r7, [sp, 56]

	/* ...and adjust the stack.  */
	min	r12, r12, 64

	/* Call the function.  */
	jl.d       [r11]
	addl       sp, sp, r12

	movl       sp, fp
	popl_s     r3	/* fig->flags, return type */
	popl_s     r2	/* ecif.rvalue, pointer for return value */

	/* If the return value pointer is NULL, assume no return value.  */
	breql.d    r2, 0, epilogue
	popl_s     blink

	/* Return INT.  */
	brne       r3, FFI_TYPE_INT, return_double
	b.d	epilogue
	st	 r0, [r2]

return_double:
	brne       r3, FFI_TYPE_DOUBLE, epilogue
	stl	r0, [r2]

epilogue:
	j_s.d      [blink]
	ldl.ab     fp, [sp, 8]

ENTRY(ffi_closure_ARC64)
	stl.a      r0, [sp, -64]
	stl	r1, [sp, 8]
	stl	r2, [sp, 16]
	stl	r3, [sp, 24]
	stl	r4, [sp, 32]
	stl	r5, [sp, 40]
	stl	r6, [sp, 48]
	stl	r7, [sp, 56]

	/* pointer to arguments */
	movl_s     r2, sp

	/* return value goes here */
	subl       sp, sp, 8
	movl_s     r1, sp

	pushl      blink

	bl.d       ffi_closure_inner_ARC64
	movl_s     r0, r8	/* codeloc, set by trampoline */

	popl       blink

	/* set return value to r0 */
	popl       r0
	j_s.d      [blink]
	addl       sp, sp, 64

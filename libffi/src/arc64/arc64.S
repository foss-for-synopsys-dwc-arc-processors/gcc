/* -----------------------------------------------------------------------
   arc64.S - Copyright (c) 2022 Synposys, Inc. (www.synopsys.com)

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
#include <ffi_cfi.h>
#ifdef HAVE_MACHINE_ASM_H
#include <machine/asm.h>
#else
#define CNAME(x) x
#define ENTRY(x) .globl CNAME(x)` .type CNAME(x),%function` CNAME(x):
#endif

#if __SIZEOF_POINTER__ == 8
#define PTRS 8
#define FLTS 8
#define LARG ldl
#define SARG stl
#define ADDPTR addl
#define MOVPTR movl_s
#else
#define PTRS 4
#define FLTS 4
#define LARG ld
#define SARG st
#define ADDPTR add
#define MOVPTR mov_s
#endif

#if defined(__arc__)
  #undef FLTS
  #define FLTS 0
#else
  /* Floating point instruction macros. */
  #if defined(__ARC_FPU_DP__)
    #define FLARG fld64
    #define FSARG fst64
  #elif defined(__ARC_FPU_SP__)
    #define FLARG fld32
    #define FSARG fst32
  #else
    #undef FLTS
    #define FLTS 0
  #endif
#endif

#define FRAME_LEN (8 * FLTS + 8 * PTRS + 16)

.text
	/* R0:   alloc_base */
	/* R1:   cb.aregs */
	/* R2:   fn */
	/* R3:   closure */

ENTRY(ffi_call_asm)
	cfi_startproc
	/* Save registers.  */
	nop_s
	.cfi_def_cfa 1, FRAME_LEN
	SARG	fp, [r1, FRAME_LEN - 2*PTRS]
	.cfi_offset 27, -2*PTRS
	SARG	blink, [r1, FRAME_LEN - 1*PTRS]
	.cfi_offset 31, -1*PTRS

	ADDPTR	fp, r1, FRAME_LEN
	MOVPTR	sp, r0
	.cfi_def_cfa 27, 0 # our frame is fully set up

	MOVPTR 	r12, r2
	MOVPTR	r13, r3

#if FLTS
	FLARG	f0, [fp, -FRAME_LEN+0*FLTS]
	FLARG	f1, [fp, -FRAME_LEN+1*FLTS]
	FLARG	f2, [fp, -FRAME_LEN+2*FLTS]
	FLARG	f3, [fp, -FRAME_LEN+3*FLTS]
	FLARG	f4, [fp, -FRAME_LEN+4*FLTS]
	FLARG	f5, [fp, -FRAME_LEN+5*FLTS]
	FLARG	f6, [fp, -FRAME_LEN+6*FLTS]
	FLARG	f7, [fp, -FRAME_LEN+7*FLTS]
#endif

	/* Move first 8 parameters in registers...  */
	LARG	r0, [fp, -FRAME_LEN+8*FLTS+0*PTRS]
	LARG	r1, [fp, -FRAME_LEN+8*FLTS+1*PTRS]
	LARG	r2, [fp, -FRAME_LEN+8*FLTS+2*PTRS]
	LARG	r3, [fp, -FRAME_LEN+8*FLTS+3*PTRS]
	LARG	r4, [fp, -FRAME_LEN+8*FLTS+4*PTRS]
	LARG	r5, [fp, -FRAME_LEN+8*FLTS+5*PTRS]
	LARG	r6, [fp, -FRAME_LEN+8*FLTS+6*PTRS]
	LARG	r7, [fp, -FRAME_LEN+8*FLTS+7*PTRS]

	/* Call the function.  */
	jl      [r12]

	SARG	r0, [fp, -FRAME_LEN+0*PTRS]
	SARG	r1, [fp, -FRAME_LEN+1*PTRS]

	add	sp, fp, -FRAME_LEN
	.cfi_def_cfa 28, FRAME_LEN
	LARG	blink, [fp, -1*PTRS]
	.cfi_restore 31
	LARG	fp, [fp, -2*PTRS]
	.cfi_restore 27

	j_s      [blink]
	cfi_endproc

/*
  ffi_closure_asm. Expects address of the passed-in ffi_closure in t1.
  void ffi_closure_inner (ffi_cif *cif,
		          void (*fun) (ffi_cif *, void *, void **, void *),
		          void *user_data,
		          size_t *stackargs, struct call_context *regargs)
*/

    .globl ffi_closure_asm
    .hidden ffi_closure_asm
    .type ffi_closure_asm, @function
ffi_closure_asm:
    cfi_startproc

    ADDPTR  sp,  sp, -FRAME_LEN
    .cfi_def_cfa_offset FRAME_LEN

    /* make a frame */
    SARG    fp, [sp, FRAME_LEN - 2*PTRS]
    .cfi_offset 27, -2*PTRS
    SARG    blink, [sp, FRAME_LEN - 1*PTRS]
    .cfi_offset 31, -1*PTRS
    ADDPTR  fp, sp, FRAME_LEN

    /* save arguments */
#if FLTS
    FSARG   f0, [sp, 0*FLTS]
    FSARG   f1, [sp, 1*FLTS]
    FSARG   f2, [sp, 2*FLTS]
    FSARG   f3, [sp, 3*FLTS]
    FSARG   f4, [sp, 4*FLTS]
    FSARG   f5, [sp, 5*FLTS]
    FSARG   f6, [sp, 6*FLTS]
    FSARG   f7, [sp, 7*FLTS]
#endif

    SARG    r0, [sp, 8*FLTS+0*PTRS]
    SARG    r1, [sp, 8*FLTS+1*PTRS]
    SARG    r2, [sp, 8*FLTS+2*PTRS]
    SARG    r3, [sp, 8*FLTS+3*PTRS]
    SARG    r4, [sp, 8*FLTS+4*PTRS]
    SARG    r5, [sp, 8*FLTS+5*PTRS]
    SARG    r6, [sp, 8*FLTS+6*PTRS]
    SARG    r7, [sp, 8*FLTS+7*PTRS]

    /* enter C */
    LARG    r0, [r8, FFI_TRAMPOLINE_SIZE+0*PTRS]
    LARG    r1, [r8, FFI_TRAMPOLINE_SIZE+1*PTRS]
    LARG    r2, [r8, FFI_TRAMPOLINE_SIZE+2*PTRS]
    ADDPTR  r3, sp, FRAME_LEN
    MOVPTR  r4, sp

    bl      ffi_closure_inner

    /* return values */
#if FLTS
    FLARG   f0, [sp, 0*FLTS]
    FLARG   f1, [sp, 1*FLTS]
#endif

    LARG    r0, [sp, 8*FLTS+0*PTRS]
    LARG    r1, [sp, 8*FLTS+1*PTRS]

    /* restore and return */
    LARG    blink, [sp, FRAME_LEN-1*PTRS]
    .cfi_restore 31
    LARG    fp, [sp, FRAME_LEN-2*PTRS]
    .cfi_restore 27
    ADDPTR    sp, sp, FRAME_LEN
    .cfi_def_cfa_offset 0
    j_s	    [blink]
    cfi_endproc
    .size ffi_closure_asm, .-ffi_closure_asm


/*
  ffi_go_closure_asm.  Expects address of the passed-in ffi_go_closure in t2.
  void ffi_closure_inner (ffi_cif *cif,
		          void (*fun) (ffi_cif *, void *, void **, void *),
		          void *user_data,
		          size_t *stackargs, struct call_context *regargs)
*/

    .globl ffi_go_closure_asm
    .hidden ffi_go_closure_asm
    .type ffi_go_closure_asm, @function
ffi_go_closure_asm:
    cfi_startproc

    ADDPTR    sp,  sp, -FRAME_LEN
    .cfi_def_cfa_offset FRAME_LEN

    /* make a frame */
    SARG    fp, [sp, FRAME_LEN - 2*PTRS]
    .cfi_offset 27, -2*PTRS
    SARG    blink, [sp, FRAME_LEN - 1*PTRS]
    .cfi_offset 31, -1*PTRS
    ADDPTR    fp, sp, FRAME_LEN

    /* save arguments */
#if FLTS
    FSARG   f0, [sp, 0*FLTS]
    FSARG   f1, [sp, 1*FLTS]
    FSARG   f2, [sp, 2*FLTS]
    FSARG   f3, [sp, 3*FLTS]
    FSARG   f4, [sp, 4*FLTS]
    FSARG   f5, [sp, 5*FLTS]
    FSARG   f6, [sp, 6*FLTS]
    FSARG   f7, [sp, 7*FLTS]
#endif

    SARG    r0, [sp, 8*FLTS+0*PTRS]
    SARG    r1, [sp, 8*FLTS+1*PTRS]
    SARG    r2, [sp, 8*FLTS+2*PTRS]
    SARG    r3, [sp, 8*FLTS+3*PTRS]
    SARG    r4, [sp, 8*FLTS+4*PTRS]
    SARG    r5, [sp, 8*FLTS+5*PTRS]
    SARG    r6, [sp, 8*FLTS+6*PTRS]
    SARG    r7, [sp, 8*FLTS+7*PTRS]

    /* enter C */
    LARG    r0, [r13, 1*PTRS]
    LARG    r1, [r13, 2*PTRS]
    MOVPTR      r2, r13
    ADDPTR  r3, sp, FRAME_LEN
    MOVPTR      r4, sp

    bl      ffi_closure_inner

    /* return values */
#if FLTS
    FLARG   f0, [sp, 0*FLTS]
    FLARG   f1, [sp, 1*FLTS]
#endif

    LARG    r0, [sp, 8*FLTS+0*PTRS]
    LARG    r1, [sp, 8*FLTS+1*PTRS]

    /* restore and return */
    LARG    blink, [sp, FRAME_LEN-1*PTRS]
    .cfi_restore 31
    LARG    fp, [sp, FRAME_LEN-2*PTRS]
    .cfi_restore 27
    ADDPTR    sp, sp, FRAME_LEN
    .cfi_def_cfa_offset 0
    j_s	    [blink]
    cfi_endproc
    .size ffi_go_closure_asm, .-ffi_go_closure_asm

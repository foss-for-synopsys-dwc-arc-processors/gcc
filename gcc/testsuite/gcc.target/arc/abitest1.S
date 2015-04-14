/* { dg-do assemble } */
#ifndef ENTRY
#define ENTRY(nm)               \
        .text `                 \
        .align 4 `              \
        .globl nm `             \
        .type nm,@function `    \
nm:
#endif

#ifndef END
#define END(name)       .size name,.-name
#endif

ENTRY(pargs01)
ENTRY(pargs02)
	ld_s	%r1,[%sp,0]             ; @0x0
	ld_s	%r0,[%sp,4]             ; @0x2
	add	%r7,%r1,%r7             ; @0x4
	j_s.d	[%blink]                ; @0x8
	add_s	%r0,%r0,%r7             ; @0xa
END(pargs01)
END(pargs02)


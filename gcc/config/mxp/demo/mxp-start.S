/* Load a single overlay at program startup.  */
 	.text
load_overlay:
#include DMA_S
#if 0
	vrec SCM_START; start recording overlay
#else
	mov_s r12, SCM_START
	vrec r12; start recording overlay
#endif
	.section .ctors,"aw",@progbits
	.align	4
	.word	load_overlay

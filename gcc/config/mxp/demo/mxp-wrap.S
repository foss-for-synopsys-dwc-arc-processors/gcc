/* Wrapper to call @f on mxp.  */

/* 2D Transfer mode: use frame table entry 31, specify source
   and stride in DMA registers DR4 and DR5, i.e.
   SYS_SRC_ADR_REG and SYS_STRIDE_REG  */

	.global f
f:

#include "dma-f-s.s"
	vdmawait 1,127 ; Wait for dma transfer to finish.
; set up sepcial integer registers: i8: stack, i9: absolute addressing base address.
#if 0
	vmov.3 vr1,@__mxp____stack_top
#else
	vmovw vr1,@__mxp____stack_top,1
	vbic.2 vr1,vr1,vr1 ; clear i9
#endif
; use mxp @f code
	; ??? assembler rejects vrun @__mxp__f / 0x0
	mov_s r12,ENTRY
	vrun r12
; get the result out of vr2 by storing to sdm and using dma out.
	vst128r vr2,[r0] ; overwrites first eight bytes of input in sdm.
        vdowr dr0,r0 ; SDM start (little endian!)
        mov_s r12,4
        vdowr dr1,r12; SDM stride
        vdowr dr2,0x1f0104; copy 1 line of 2 bytes
        vdowr dr4,@result; main memory start
        vdowr dr5,r12; main memory stride
        vdorun pcl,pcl; start dma transfer
	vdmawait 127,1 ; Wait for dma transfer to finish.
.Lwait_for_mxp:
	lr r0,[SE_STAT]
	bbit1 r0,6,.Lwait_for_mxp
	j_s.d	[blink]
	ld	r0,[@result]

        .comm   result,2,2

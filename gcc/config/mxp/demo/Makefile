MXP_CC1=/scratch/gcc-4.4.0-20080716-arc-int/bld-mxp/gcc/cc1
MXP_AS=~irfanr/tasks/045_mxp2/binutils/build/gas/as-new
MXP_LD=~irfanr/tasks/045_mxp2/binutils/build/ld/ld-new
ARC_CC=/apps/gnu/arc-gnu-tools/rel2.1/elf32/bin/arc-elf32-gcc
ARC_AS=/apps/gnu/arc-gnu-tools/rel2.1/elf32/bin/arc-elf32-as
ARC_LD=/apps/gnu/arc-gnu-tools/rel2.1/elf32/bin/arc-elf32-ld
OBJCOPY=/apps/gnu/arc-gnu-tools/rel2.1/elf32/bin/arc-elf32-objcopy
OPTS=-O2 -ftree-vectorize -g

all: arc-demo mxp-demo

clean:
	rm -f *.s *.o mxp-max.x0 mxp-max.x1 arc-demo mxp-demo dma-gen

mxp-max-0.s: max.c demo.h
	$(MXP_CC1) $(OPTS) -g0 -quiet -mint16 -fno-common -mno-vim-label -o $@ $<

mxp-max.s: mxp-max-0.s
	sed -e 's/@.LC0]/0]/' -e 's/@s]/16]/' -e 's/vnop/vrec pcl/' -e 's/vjb vr31,pcl//' < $< > $@

mxp-max.o: mxp-max.s
	$(MXP_AS) -mA7 -msimd -o $@ $<

mxp-array.s: array.c demo.h
	$(MXP_CC1) $(OPTS) -g0 -quiet -mint16 -fno-common -mno-vim-label -o $@ $<

mxp-array.o: mxp-array.s
	$(MXP_AS) -mA7 -msimd -o $@ $<

crtrostart.o: crtrostart.S
	$(ARC_CC) -c -o $@ $<

crtroend.o: crtroend.S
	$(ARC_CC) -c -o $@ $<

mxp-max.x0: mxp.x

MXP_MAX_OBJS=crtrostart.o mxp-max.o mxp-array.o crtroend.o
mxp-max.x0: $(MXP_MAX_OBJS)
	$(MXP_LD) -o $@ $(MXP_MAX_OBJS) -T mxp.x -e f

mxp-max.x1: mxp-max.x0
	$(OBJCOPY) --prefix-symbols=__mxp__ -R .bss $< $@

dma-gen: dma-gen.c

dma-f-ro.s: mxp-max.x0 dma-gen
	echo @__mxp____dma_start 0 0x`nm $<|sed -e '/ __sdm_rodata_end$$/s/ .*//p' -e d`
	./dma-gen @__mxp____dma_start 0 0x`nm $<|sed -e '/ __sdm_rodata_end$$/s/ .*//p' -e d` > $@

mxp-start.s: mxp-start.S dma-f-ro.s
	$(ARC_CC) -S $< '-DDMA_S="dma-f-ro.s"' -DSCM_START=0 -E > $@

mxp-start.o: mxp-start.s
	$(ARC_AS) -mA7 -msimd -o $@ $<

dma-f-s.s: mxp-max.x0 array.o dma-gen
	./dma-gen @dma_start 0x`nm $<|sed -e '/ dma_start$$/s/ .*//p' -e d; size -A -d array.o|sed -e '/^\.bss\>/s/.*\<\([0-9][0-9]*\>\).*/\1/p' -e d` > $@
crtvend.o: crtvend.S
	$(ARC_AS) -mA7 -msimd -o $@ $<

demo.o: demo.c demo.h
	$(ARC_CC) -mA7 $(OPTS) -c $<

array.o: array.c demo.h
	$(ARC_CC) -mA7 $(OPTS) -c -fno-common $<

max.o: max.c demo.h
	$(ARC_CC) -mA7 $(OPTS) -c $<

arc-demo: demo.o array.o max.o
	$(ARC_CC) -mA7 $(OPTS) $^ -o $@

mxp-wrap.o: mxp-wrap.S dma-f-s.s mxp-max.x0
	$(ARC_CC) -c -mA7 -msimd $< -o $@ -DENTRY=0x`nm mxp-max.x0|sed -e '/ f$$/s/ .*//p' -e d`

mxp-demo: demo.o array.o mxp-wrap.o mxp-start.s mxp-max.x1 crtvend.S
	$(ARC_CC) -mA7 $(OPTS) -msimd $^ -o $@

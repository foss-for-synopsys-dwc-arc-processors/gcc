2013-08-22  Claudiu Zissulescu <claziss@synopsys.com>

	* libgcc/config.host: Unique build configuration settings for ARC
	processors.
	* libgcc/config/arc/gmon/dcache_linesz.S: Disable reading the cache
	configuration for EM.
	* libgcc/config/arc/gmon/profil.S: Disable using profiling functions
	for EM.
	* libgcc/config/arc/lib1funcs.S: Use custom ASM code to emulate
	div/rem, mult and floating point operations whenever these are not
	available for the selected V2 architecture.

2013-06-08  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/dp-hack.h
	[!__ARC700__ && !__ARC_MUL64__ && !__ARC_MUL32BY16__] (QUIET_NAN):
	Define to 0xfffffffffffffLL.
	[!__ARC700__ && !__ARC_MUL64__ && !__ARC_MUL32BY16__] (__muldf3):
	define as __muldf3_c.
	[!__ARC700__ && !__ARC_MUL64__ && !__ARC_MUL32BY16__] (__divdf3):
	define as __divdf3_c.
	* config/arc/ieee-754/arc600-dsp/muldf3.S (__muldf3):
	Add FUNC/ENDFUNC.
	* config/arc/ieee-754/arc600: Rename directory to ..
	* config/arc/ieee-754/arc600-mul64: .. this.
	* config/arc/ieee-754/arc600-mul64/mulsf3.S
	(.Ldenorm_dbl0_inf_nan_dbl1): Fix sign of subnormal*inf.
	* config/arc/ieee-754/arc600: New directory.
	* config/arc/lib1funcs.S: Reflect arc600-mul64 directory rename.
	[ARC_NORM]: Include ieee-754/arc600/mulsf3.S, ieee-754/arc600/divsf3.S.
	* config/arc/t-arc b/libgcc/config/arc/t-arc (dp-bit.c):
	Place dp-hack.h include after fp-bit.h include.

2013-06-07  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/ieee-754/mulsf3.S (Linf_nan_dbl0): Fix inf*inf handling.
	(Ldenorm_dbl0_inf_nan_dbl1): Fix subnormal*inf sign.  Shorten.
	(.Linf_nan_dbl1): Shorten.  All load offsets changed.

2013-04-12  Simon Cook  <simon.cook@embecosm.com>

	* config.host: Restore arc*-*-elf* definition.

2013-04-03  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/t-arc (LIB2FUNCS_EXTRA): Replace with:
	(LIB2ADD).

2013-03-16  Joern Rennecke  <joern.rennecke@embecosm.com>

	* lib1funcs.S (__mulsi3): Use mlo register name.
	(__umulsidi3): Use ARCompact instructions & re-schedule for
	ARC600 & ARC700 pipelines.  Provide __umulsi3_highpart symbol
	for __ARC700__, __ARC_MUL64__ and __ARC601__.
	(__umulsi3_highpart): New function.
	* config/arc/t-arc (LIB1ASMFUNCS): Add _umulsi3_highpart.
	* longlong.h [__arc__] (umul_ppmm): Remove.
	[__arc__] (__umulsidi3): Define.
	[__arc__ && __ARC_NORM__] (count_leading_zeroes): Define.
	[__arc__ && __ARC_NORM__] (COUNT_LEADING_ZEROS_0): Likewise.

2013-02-25  Simon Cook  <simon.cook@embecosm.com>

	* config.host: arc-*-elf* renamed to arc*-*-elf*.
	Added arc*-*-* definition as ARC cpu type.

2012-10-25  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/t-arc: Add comment on fp-bit.c / dp-bit.c use.

2012-10-18  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config.host (arc600-*-linux-uclibc, arc601-*-linux-uclibc): Delete.
	* config/arc/t-arc600-uClibc: Delete file.

2012-10-18  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc: Update Copyright and License of all files
	in anticipation of FSF submission.

2012-10-17  Joern Rennecke  <joern.rennecke@embecosm.com>

	* crtg.S, crtgend.S, gmon/dcache_linesz.S: Remove __A4__ code.
	* gmon/dcache_linesz.S, gmon/prof-freq-stub.S: Likewise.
	* gmon/profil.S, lib1funcs.S: Likewise.

2012-10-17  Joern Rennecke  <joern.rennecke@embecosm.com>

	* libgcc/config/arc/lib1funcs.S (entire file): Remove #ifdef __base__.

2012-10-13  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/t-arc (gmon.o): Use -fno-strict-aliasing.

2012-10-09  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/t-arc (crtg.o, crtgend.o): Use $(crt_compile).
	Don't depend on $(GCC_PASSES).
	(mcount.o, gmon.o, prof-freq-stub.o, prof-freq.o): Use $(gcc_compile).
	Don't depend on $(GCC_PASSES).
	(dcache_linesz.o, profil.o, profil-uclibc.o): Likewise.
	* config/arc/t-arc700-uClibc (SHLIB_LINK, SHLIB_INSTALL): Delete.
	* config.host (arc*-*-linux-uclibc*) <tmake_file>:
	Add t-slibgcc-libgcc t-slibgcc-nolc-override.

2012-10-09  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/t-arc700-uClibc (SHLIB_MAPFILES) <libgcc-std.ver>:
	Remove $(srcdir)/ prefix.

2012-10-08  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/t-arc-newlib: Remove multilib settings.
	* config/arc/t-arc600-uClibc: Likewise.
	* config/arc/t-arc700-uClibc: Likewise.

2012-08-17  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/ieee-754/gtdf2.S: Remove redundant check.

	* config.host (arc-*-elf*) <extra_parts>: Add libgmon.a, crtg.o and
	crtgend.o .
	(arc600-*-linux-uclibc*, (arc601-*-linux-uclibc*): Likewise.
	(arc*-*-linux-uclibc*): Likewise.
	* config/arc/t-arc (EXTRA_MULTILIB_PARTS): Remove.
	(crtg.o, crtgend.o): Update source file name.

2012-08-16  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config.host (arc-*-elf*, arc600-*-linux-uclibc*): New configurations.
	(arc601-*-linux-uclibc*, arc*-*-linux-uclibc*): Likewise.
	* config/arc: New Directory.

	* config/arc/crtg.asm, config/arc/crti.asm config/arc/crtn.asm:
	Move, respectively, to:
	* config/arc/crtg.S, config/arc/crti.S config/arc/crtn.S.
	* config/arc/crtgend.asm, config/arc/lib1funcs.asm:
	Move, respectively, to:
	* config/arc/crtgend.S, config/arc/lib1funcs.S.
	* config/arc/t-arc: Reflect above name changes.  Remove $(T).
	(crti.o, crtn.o): Remove rules.
	* config/arc/t-arc600-uClibc: Remove $(T).
	* config/arc/t-arc700-uClibc, config/arc/t-arc-newlib: Likewise.

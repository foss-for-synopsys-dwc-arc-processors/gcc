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

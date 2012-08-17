2012-08-17  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/ieee-754/gtdf2.S: Remove redundant check.

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

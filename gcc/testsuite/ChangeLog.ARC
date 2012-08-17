2012-02-15  Joern Rennecke  <joern.rennecke@embecosm.com>

	* gcc.dg/os-align.c: New test.

2011-12-16  Joern Rennecke  <joern.rennecke@embecosm.com>

	* gcc.target/arc/mul2condx-1.c: New test.
	* gcc.target/arc/mul2condx-2.c: Likewise.

2011-11-28  Joern Rennecke  <joern.rennecke@embecosm.com>

	* gcc.c-torture/execute/ieee/subnormal-adddf.c: New test.

2008-12-10  J"orn Rennecke  <joern.rennecke@arc.com>

	* gcc.target/arc/add_f.c: Use -O2.
	Allow mov.lo as alternative to mov.hs.
	* lib/scanasm.exp (scan-assembler-times):
	Translate control characters for printing.
	Fix regexp count in the presence of subexpressions.

2008-12-08  J"orn Rennecke  <joern.rennecke@arc.com>

	* gcc.dg/torture/pr37868.c: Skip for ARC.

2008-11-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* gcc.dg/pr28243.c: Only for target pic.
	* gcc/dg/pr35044.c: Don't put -mA7 into dg-options.

2008-11-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* gcc.target/arc: New directory.
	* gcc.target/arc/arc.exp: New driver.
	* gcc.target/arc/adc.c: New test.
	* gcc.target/arc/add_f.c: New test.
	* gcc.target/arc/sbc.c: New test.

2008-09-16  J"orn Rennecke  <joern.rennecke@arc.com>

	* gcc.dg/tree-ssa/ssa-store-ccp-2.c (dg-options): Add -fcommon.

2008-07-15  J"orn Rennecke  <joern.rennecke@arc.com>

	* gcc.c-torture/compile/20000804-1.c: Skip for mxp.
	* gcc.c-torture/compile/20010327-1.c: Likewise.
	* gcc.c-torture/compile/920501-4.c: Likewise.
	* gcc.c-torture/compile/20020604-1.c: Likewise.
	* gcc.c-torture/compile/20010518-2.c: Likewise.
	* gcc.c-torture/compile/920501-12.c: Likewise.
	* gcc.c-torture/compile/limits-blockid.c: Likewise.
	* gcc.c-torture/compile/20001226-1.c: Likewise.
	* gcc.c-torture/compile/961203-1.c: Likewise.
	* gcc.c-torture/compile/930506-2.c:
	* gcc.c-torture/compile/20050622-1.c: Likewise.
	Use dg-require-effective-target trampolines.
	* gcc.c-torture/compile/limits-fndefn.c: Use STACK_SIZE.
	* gcc.c-torture/compile/limits-fnargs.c: Likewise.

2008-04-04  J"orn Rennecke  <joern.rennecke@arc.com>

	* gcc.c-torture/execute/ieee/denorm-rand.c: New file.
	* gcc.dg/torture/fp-int-convert.h: Avoid undefined behaviour.

2008-01-31  J"orn Rennecke  <joern.rennecke@arc.com>

	* gcc.dg/pr35044.c: New file.

2007-04-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* gcc.dg/sibcall-3.c: Don't XFAIL.
	* gcc.dg/sibcall-4.c: Likewise.

2007-04-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* g++.dg/cpp/_Pragma1.c: Disable for arc-*-elf*.

2007-04-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* gcc.dg/cpp/_Pragma6.c: Disable for arc-*-elf*.

2007-04-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* gcc.dg/builtin-apply2.c (main): Make sure we have 64 bytes of
	stack to copy.

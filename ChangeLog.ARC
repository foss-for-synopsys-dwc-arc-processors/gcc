2012-04-30  Joern Rennecke  <joern.rennecke@embecosm.com>

        * config/arc/arc.md (doloop_begin_i): Use pessimistic size
        estimation.

2012-04-03  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config.sub (arceb, arc[67]0[01]eb): New configurations.
	* configure.ac (RPATH_ENVVAR): Set to LD_RUN_PATH for
	host_cpu != target_cpu
	* configure: Regenerate.

2011-11-24  Joern Rennecke  <joern.rennecke@embecosm.com>

	* configure.ac [ENABLE_PPL_CHECK]: Allow PPL_VERSION_MINOR > 10.
	* configure: Regenerate.

2010-10-11  Brendan Kehoe  <brendan@zen.org>

	* config.sub: Add support for arc600 or arc601 for
	value of $basic_machine.

2010-08-19  Michael Eager  <eager@eagercon.com>

	* config.sub: Add arc601-*-linux-uclibc configuration.

2010-08-05  Michael Eager  <eager@eagercon.com>

	* config.sub: Set target_cpu_default to with-cpu value.

2008-04-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* config.sub: Add mxp support.

2007-09-17  J"orn Rennecke  <joern.rennecke@arc.com>

	* configure.in (FLAGS_FOR_TARGET):
	Add -isystem $$s/newlib/libc/sys/'$target_cpu'/sys .
	* configure: Regenerate.

2007-04-17  J"orn Rennecke  <joern.rennecke@arc.com>

	* config-ml.in: Merge codito patches.

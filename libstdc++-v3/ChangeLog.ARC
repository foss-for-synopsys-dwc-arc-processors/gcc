2007-06-11  J"orn Rennecke  <joern.rennecke@arc.com>

	* libstdc++-v3/testsuite/lib/libstdc++.exp (v3-build_support):
	When compilation fails, tell why.
	* scripts/testsuite_flags.in (--build-cxx): Don't mistake -m* inside
	an identifier for an -m option.

2007-05-16  J"orn Rennecke  <joern.rennecke@arc.com>

	* scripts/testsuite_flags.in (--build-cxx): Remove -m options.

2007-05-10  J"orn Rennecke  <joern.rennecke@arc.com>

	* testsuite/ext/pb_ds/example/priority_queue_dijkstra.cc (main): Don't
	access deallocated nodes.

2007-04-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* testsuite/27_io/basic_stringbuf/overflow/char/1.cc:
	specify heap size for ARC.

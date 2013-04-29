2013-04-22 Claudiu Zissulescu <claziss@synopsys.com>

       * resource.c: Compute resources taking into account if a call is
       predicated or not

2013-04-27  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.h (ASM_PREFERRED_EH_DATA_FORMAT): Define.

2013-04-19  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.c (arc_ccfsm_advance): Check for SIMPLE_RETURN.
	* config/arc/arc.md (sibcall, sibcall_value): Use simple_return.
	(sibcall_insn, sibcall_value_insn, sibcall_prof): Likewise.
	(sibcall_value_prof, sibcall_epilogue, p_return_i): Likewise.

2013-04-17 Claudiu Zissulescu <claziss@synopsys.com>
	* doc/invoke.texi: Change the option -mcpu=EM into -mcpu=ARCv2EM,
	and change the -mEM driver option to -mav2em
	* common/config/arc/arc-common.c:  Likewise
	* config/arc/arc-opts.h: Likewise
	* config/arc/arc.c: Likewise
	* config/arc/arc.h: Likewise
	* config/arc/arc.opt: Likewise

2013-04-08  Joern Rennecke  <joern.rennecke@embecosm.com>

	* testsuite/gcc.dg/torture/stackalign/builtin-apply-2.c
	[__arc__] (STACK_ARGUMENTS_SIZE): Set to 0.

	* testsuite/gcc.dg/builtin-apply2.c
	[__arc__] (STACK_ARGUMENTS_SIZE): Set to 0.

2013-04-05 Claudiu Zissulescu <claziss@synopsys.com>
	* doc/invoke.texi: Add gcc-EM options to the documentation
	* config/arc/arcEM.md: add ARC-EM pipeline description
	* config/arc/arc.c: Add ARCompactV2 instructions. 
	* config/arc/arc.h: Likewise
	* config/arc/arc.opt: Likewise
	* config/arc/arc.md: Likewise
	* config/arc/constraints.md: Likewise
	* config/arc/predicates.md: Likewise
	* common/config/arc/arc-common.c:  Likewise

2013-04-04  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.c (arc_get_insn_variants) <TYPE_BRCC_NO_DELAY_SLOT>:
	Return 0 for a PARALLEL with a second SET.

2013-04-03  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.h (DRIVER_SELF_SPECS): Also remove -mA6 / -mA7.

	* lto-wrapper.c (run_gcc): Don't duplicate target options.

2013-04-03  Joern Rennecke  <joern.rennecke@embecosm.com>

	* common/config/arc/arc-common.c (DEFAULT_NO_SDATA): Use
	TARGET_SDATA_DEFAULT.
	* config.gcc (arc*-*-linux-uclibc) <tm_defines>: Set
	TARGET_SDATA_DEFAULT and TARGET_MMEDIUM_CALLS_DEFAULT.
	* config/arc/arc.h (TARGET_SDATA_DEFAULT): Provide default definition.
	(TARGET_MMEDIUM_CALLS_DEFAULT): Likewise.
	* config/arc/arc.opt (mmedium-calls): Add Init clause.

	* config/arc/arc.c (arc_ccfsm_advance): Return early for
	non-predicable call.
	* config/arc/arc.h (SFUNC_CHECK_PREDICABLE): Define.
	* config/arc/arc.md (attribute cond): Refine call processing.
	(mulsi3_600_lib): Set predicable attribute and add
	SFUNC_CHECK_PREDICABLE clause.
	(umulsi3_highpart_600_lib_le, umulsi3_highpart_600_lib_be): Likewise.
	(umulsidi3_600_lib, cmpsf_<cmp>, cmpdf_<cmp>): Likewise.

	* config/arc/arc.c (arc_spill_class): Comment out unused parameter
	name.

2013-03-16  Jeremy Bennett  <jeremy.bennett@embecosm.com>

	* config/arc/arc.h (DRIVER_SELF_SPECS): Insert space before
	concatenated -mcpu options.

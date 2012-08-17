2012-05-11  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.opt (mno-epilogue-cfi, mepilogue-cfi): New options.
	* config/arc/arc.c (arc_expand_epilogue): Unless TARGET_EPILOGUE_CFI
	is active, clear RTX_FRAME_RELATED_P in all instructions in the
	current sequence.

2012-05-10  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.md (doloop_begin_i): Call arc_clear_unalign
	when emitting an alignment directive.

2012-04-17  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.md (mulsi_600): Work around assembler bug.
	(mulsidi_600, umulsidi_600): Avoid assembler bug.

2012-04-12  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.c (arc_output_libcall):
	Fix -fpic -mlong_calls / -mmedium_calls conditional output.

	* config/arc/arc.h (DRIVER_SELF_SPECS): Translate -mARC601 to
	-mcpu=ARC601.

2012-04-06  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.opt (mvolatile_cache, mno-volatile-cache):
	Fix description.

2012-04-03  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.h (DRIVER_ENDIAN_SELF_SPECS): Provide default.
	(DRIVER_SELF_SPECS): Define.
	* config.gcc (arc*-*-linux-uclibc*): set DRIVER_ENDIAN_SELF_SPECS
	in tm_defines in accordance with with_endian.

2012-04-02  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.md (bbit_di): New define_insn_and_split.
	* config/arc/predicates.md (proper_comparison_operator): Reject DImode.

2012-04-02  Joern Rennecke  <joern.rennecke@embecosm.com>

	Merge from mainline:
	2011-04-29  Nicola Pero  <nicola.pero@meta-innovation.com>,
		    Mike Stump <mikestump@comcast.net>

	  * Make-lang.in ($(srcdir)/cp/cfns.h): Enable the dependency only
	  in maintainer mode.  Use the --output-file option of gperf instead
	  of > to prevent creating an empty cp/cfns.h when gperf is not
	  available.

2012-03-28  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.c (arc_output_libcall): Remove TARGET_NO_BLCC_SET
	test.
	* config/arc/arc.md (attributes blcc, enabled): Remove.
	* config/arc/arc.opt (mno-cond-exec): Update documentation.
	(mno-blcc): Remove.

2012-03-27  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.md (attribute "length"):
	Increase value for type sfunc.

2012-03-23  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/ieee-754/truncdfsf2.S (__truncdfsf2): Fix overflow
	range test.  Fix NaN test to be compatible with -mdpfp.

2012-03-22  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.md (call_i): Split alternative 0.
	* config/arc/arc.md (call_value_i): Likewise.

2012-03-14  Joern Rennecke  <joern.rennecke@embecosm.com>

	Back out this patch:
	2010-09-10  Brendan Kehoe  <brendan@zen.org>
	  Prevent the compiler from attempting nested LP zero-overhead
	  loops since there is only a single LP_COUNT register.
	  * config/arc/arc.md (doloop_begin_i): Mark that we will end
	  up clobbering LP_COUNT from here.
	  (doloop_end): Note the end of the loop uses LP_COUNT as well.

	* config/arc/arc.c (arc_reorg): Fix test for having not found
	a simple lp instruction.
	* config/arc/arc.md (doloop_end_i): Document operands.
	Don't zero size at loop start.

	* config/arc/arc.h (REG_CLASS_CONTENTS) <SIBCALL_REGS>:
	Remove lp_count.
	(IRA_COVER_CLASSES): Define.
	* config/arc/arc.md (add_n): Increase cost of LP_COUNT.

2012-02-20  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.c (arc_init): Set a higher multcost for -mno-mpy.
	(arc_rtx_costs) <optimizing for size>: Likewise.
	* config/arc/t-arc-newlib (MULTILIB_MATCHES): Add mcpu?ARC600=mno-mpy

2012-02-16  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.h (ASM_OUTPUT_CASE_END): Fix off-by-one error.

2012-02-15  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.h (ASM_OUTPUT_CASE_END): Define.

2012-02-13  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.c (arc_save_restore): Make millicode sibthunk
	epiplogue match prologue.

2012-01-27  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/ieee-754/addsf3.S (.Linf_nan): Return NaN for inf - inf.
	* config/arc/ieee-754/divsf3.S (.Linf_nan_fp1): Return 0 for x/inf.
	* config/arc/ieee-754/divsf3-stdmul.S (.Linf_nan_fp1): Likewise.
	* config/arc/ieee-754/arc600/divsf3.S (.Linf_nan_fp1): Likewise.
	* config/arc/ieee-754/arc600-dsp/divsf3.S (.Linf_nan_fp1): Likewise.

2011-12-31  Joern Rennecke  <joern.rennecke@embecosm.com>

	* Makefile.in (var-targets): New rule.
	* doc/install.texi: Document use of var-targets rule.

	* config/arc/lib1funcs.asm [ARC601] (mulsi3): Make branch agree
	with label name.  Fix middle bit test.

	* config/arc/lib1funcs.asm [ARC601] (clzsi3): Reimplement.

2011-12-16  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.md (flush_icache+13): Split into ..
	(flush_icache+13, flush_icache+14): .. these.
	Fix logic.  Allow for commutativity of mult.

2011-12-14  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/arc.md (noncommutative_binary_comparison_result_used):
	Allow MINUS irrespective of TARGET_SHIFTER.

	* config/arc/arc.md (commutative_binary_comparison_result_used):
	Don't depend on TARGET_SHIFTER.

	* config/arc/arc.md: Comment typo fix auxillary -> auxiliary.
	(casesi_compact_jump): Fix indentation.

2011-11-28  Joern Rennecke  <joern.rennecke@embecosm.com>

	Back out this change:
	  2010-10-11  Brendan Kehoe  <brendan@zen.org>
	  * config/arc/arc.md (*call_i, *call_value_i,
	  call_prof, call_value_prof): Remove Cbr cases.
	  (call_blcc, call_noblcc, call_value_blcc, call_value_noblcc,
	  call_prof_blcc, call_prof_noblcc, call_value_prof_blcc,
	  call_value_prof_noblcc): New patterns for Cbr cases.

	* config/arc/arc.md (blcc, enabled): New attributes.

2011-11-27  Joern Rennecke  <joern.rennecke@embecosm.com>

	* config/arc/ieee/lib1funcs.asm (mulsi3, clzsi): Write dedicated
	ARC601 implementation.
	(__udivmodsi4): Consolidate ARC601 processing.
	* config/arc/ieee/adddf3.S: (__adddf3): Remove special handling
	of 0.  Fix detection of subnormal numbers to accept pre-existing
	subnormals.  Streamline inf/NaN processing.
	* config/arc/ieee/muldf3.S: (__muldf3): Restore concise version.

2011-11-27  Joern Rennecke  <joern.rennecke@embecosm.com>

	* rtlanal.c (dead_or_set_regno_p): Fix COND_EXEC handling.
	* constraints.md (Rcc): New constraint.
	* config/arc/arc.md (noncommutative_binary_comparison):
	Allow MINUS irrespective of TARGET_SHIFTER.
	(bic_f_zn, bic_f, norm_f, clzsi2, ctzsi2): New patterns.
	(add_cond_exec, sub_cond_exec): Use Rcc constraint.
	(noncommutative_cond_exec): Likewise.
	(movsi_ne, movsi_cond_exec, commutative_cond_exec): Likewise.
	Remove bogus commutativity constraint modifier.
	(sub_cond_exec): Change predicate of operand 1 to nonmemory_operand.

2011-02-24  Brendan Kehoe  <brendan@zen.org>

	Revert this change; it fails when using an array cast to an
	aggregate type to dereference a data member.  e.g., busybox to
	get at the show_color member. */

	2009-02-11  J"orn Rennecke  <joern.rennecke@arc.com>
	* explow.c (memory_address): Call LEGITIMATE_ADDRESS also in the
	cse-driven case.

2011-02-23  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.md: Fix logic in peephole2 optimization for
	operations computing an index based solely on the 31st bit
	of a value.

2011-02-22  Brendan Kehoe  <brendan@zen.org>

	* config/arc/fpx.md (muldf3_insn): Add third alternative to
	avoid combine merging two conflicting (and unworkable) patterns,
	usually replacing a CONST_DOUBLE with a reg.
	(adddf3_insn, subdf3_insn): Likewise.
	(define_peepholder2): Add to both peep2.
	(*daddh_peep2_insn, *dmulh_peep2_insn, *dsubh_peep2_insn):
	Add USE placeholder.
	* config/arc/arc.md (*movdf_insn_nolrsr): Likewise.

2011-02-21  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.c (arc_secondary_reload): Don't be conditional
	on suggesting GENERAL_REGS for reloads involving DOUBLE_REGS;
	otherwise, memory loads/stores end up being attempted with
	the Dx regs and fail.

2011-02-10  Brendan Kehoe  <brendan@zen.org>

	Further fixes to domain error handling.
	* config/arc/ieee-754/adddf3.S: If the RHS operand is neither
	infinity nor NaN, just return the LHS operand.
	* config/arc/ieee-754/muldf3.S (Linf_nan): Rewrite to simplify
	and make sure arguments can be commutable.

2011-02-08  Brendan Kehoe  <brendan@zen.org>

	Add support for disabling the LR insn with FPX extension regs,
	in the process replacing the DFmode patterns with ones more
	accessible to the optimizer.
	* config/arc/arc.c (arc_init): Give an error if -mno-dpfp-lrsr
	is used without -mdpfp.
	(arc_register_move_cost): Make the cost of direct Dx->Dy moves
	to be 100 to prevent it from being generated.
	(arc_process_double_reg_moves): New function.
	(arc_split_move): Give back the resulting SET insns.
	(arc_secondary_reload): Make sure CONST_DOUBLE gets split up
	into normal registers, since we cannot explicitly load D1 or D2
	in a MOV insn.
	* config/arc/arc-protos.h (arc_split_move): Fix return type.
	* config/arc/arc.md (VUNSPEC_DEXCL, VUNSPEC_DEXCL_NORES,
	VUNSPEC_LR_HIGH): New constants for our unspec_volatile
	patterns.
	(*movdf_insn): Simplify.  Create accompanying define_split
	which will use arc_split_move() after reload_completed.
	(*movdf_insn_nolrsr): New insn and split pair to generate
	DADDHxy and DEXCLx insns to do the work LR would've done.
	* config/arc/fpx.md (*lr_double_lower): Only allow if
	!TARGET_DPFP_DISABLE_LRSR; require operand1 to not just be
	a register, but be arc_double_register_operand.
	(*lr_double_higher): New pattern.
	(*dexcl_3op_peep2_insn): Define new one as VUNSPEC_DEXCL.
	(*dexcl_3op_peep2_insn_lr): Original VUNSPEC_LR version as
	long as !TARGET_DPFP_DISABLE_LRSR.
	(*dexcl_3op_peep2_insn_nores): New pattern.
	(define_peephole2): Only allow patterns with VUNSPEC_LR if
	!TARGET_DPFP_DISABLE_LRSR.

2011-01-20  Brendan Kehoe  <brendan@zen.org>

	CR 101622
	* config/arc/ieee-754/adddf3.S (Lcheck_iszero): New code to
	handle either operand being zero.
	(__adddf3): Use it.

	CR 101612
	* config/arc/ieee-754/adddf3.S (Linf_nan): Fix to give NaN when
	the signs on values of infinity don't match.
	* config/arc/ieee-754/muldf3.S (Linf_nan): Fix to honor inf*0.0
	and properly give back NaN.

2011-01-11  Brendan Kehoe  <brendan@zen.org>

	* rtlanal.c (walk_stores): Move ARC-local change from here...
	* config/arc/arc.c: ...to here until we decide we want to
	try to submit it as a patch against rtlanal.c.
	* rtl.h (walk_stores): Delete decl.

2011-01-10  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.c (arc_conditional_register_usage): For DPFP,
	don't touch CHEAP_CORE_REGS and ALL_CORE_REGS unless
	!TARGET_ARGONAUT_SET.

2011-01-07  Michael Eager  <eager@eagercon.com>

	* config/arc/arc.h (TARGET_OPTFPE): Set for ARC601.
	
2011-01-05  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.c (get_arc_condition_code, case CC_FP_GTmode):
	Only set N or P if TARGET_ARGONAUT_SET for SPFP, otherwise do HI
	or LS.
	(arc_verify_short): Only police short with SPFP if
	!TARGET_ARGONAUT_SET.

2011-01-04  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.c (arc_init): Don't complain if a DPFP option
	was used without -margonaut.
	(arc_conditional_register_usage): With TARGET_DPFP, make sure
	WRITABLE_CORE_REGS and MPY_WRITABLE_CORE_REGS don't believe the
	Dx registers are usable unless TARGET_ARGONAUT_SET.
	(gen_compare_reg): For TARGET_ARGONAUT_SET, call the
	renamed gen_cmpdfpx_argonaut_raw.
	* config/arc/arc.h (TARGET_DPFP): Remove TARGET_ARGONAUT_SET.
	* config/arc/fpx.md (cmpfpx_raw) [!TARGET_ARGONAUT_SET]: Define
	new version which uses correct insn Dx register for dest and to
	not try to use Dx regs as operands.
	(cmpdfpx_argonaut_raw) [TARGET_ARGONAUT_SET]: Renamed.

	* config/arc/fpx.md (cmpdfpx_raw): Only keep TARGET_ARGONAUT_SET
	version.  In this and related patterns, add comment explaining
	why this implementation is flawed.
	(cmpdfpx_argonaut_raw): Deleted.
	(cmpsfpx_raw): Conditionalize on TARGET_ARGONAUT_SET.
	(*cmpfpx_gt, *cmpfpx_ge): Likewise.
	* config/arc/arc.md (*cmp{s,d}f_{eq,gt,ge,uneq,ord}): Only heed
	the -mspfp or -mdpfp flags if TARGET_ARGONAUT_SET; otherwise
	always allow these to result in calling comparison routines.
	* config/arc/arc.c (gen_compare_reg): Only use the approach
	with cmp{s,d}fpx_raw if TARGET_ARGONAUT_SET.
	(arc_select_cc_mode): Only allow the possibility of CC_FPXmode
	if TARGET_ARGONAUT_SET.

2010-12-31  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.c (arc_init): Don't complain if a DPFP option
	was used without -margonaut.
	(arc_conditional_register_usage): With TARGET_DPFP, make sure
	WRITABLE_CORE_REGS and MPY_WRITABLE_CORE_REGS don't believe the
	Dx registers are usable unless TARGET_ARGONAUT_SET.
	  **NOTE**: this currently breaks  (set (reg d1) (const_double))
	(gen_compare_reg): For TARGET_ARGONAUT_SET, call the
	renamed gen_cmpdfpx_argonaut_raw.
	* config/arc/arc.h (TARGET_DPFP): Remove TARGET_ARGONAUT_SET.
	* config/arc/fpx.md (cmpfpx_raw) [!TARGET_ARGONAUT_SET]: Define
	new version which uses correct insn Dx register for dest and to
	not try to use Dx regs as operands.
	(cmpdfpx_argonaut_raw) [TARGET_ARGONAUT_SET]: Renamed.

	* config/arc/fpx.md: Remove constraints from define_peephole2
	patterns.

	* config/arc/arc.c (arc_verify_short): For TARGET_SPFP,
	prevent letting specific unsupported conditions P, N, C, or NC
	be part of short Bcc_S branches; result in Bcc instead.
	(arc_reorg): Don't try to build a BRCC insn if the operands
	are SFmode.

	Isolate the Argonaut FPX DPFP implementation.
	* config/arc/arc.opt (margonaut): Add -margonaut option to
	enable the Argonaut FPX implementation.
	* config/arc/arc.h (TARGET_DPFP): For now, only allow this
	if TARGET_ARGONAUT_SET.
	* config/arc/arc.c (arc_init): Complain if either DPFP flag
	is used without also using -margonaut.

2010-12-29  Michael Eager  <eager@eagercon.com>

	* config/arc/arc.c (output_shift): Add nop in shift loop.
	* config/arc/arc.h (TARGET_CPU_CPP_BUILTINS): Define __ARC601__.
	* config/arc/arc.md (*commutative_binary_comparison_result_used,
	*noncommutative_binary_comparison_result_used, 
	*noncommutative_binary_comparison): patterns valid only with 
	TARGET_SHIFTER.
	* config/arc/lib1funcs.asm (__mulsi3, __udivmodsi4, __clzsi2):
	Add support for ARC601.
	
2010-11-09  Brendan Kehoe  <brendan@zen.org>

	* config/arc/predicates.md (proper_comparison_operator): Add
	missing cases for the CC_FP_* modes which can be set up by
	arc.c:arc_select_cc_mode.

	* config/arc/arc.md (cmpdf, cmpsf): Change constraints to
	both be register_operand, not general_operand, so MEM refs
	will get put into a reg for us.
	* config/arc/fpx.md (cmpdfpx_raw): Fix use to do %H.h and %L.l.

	* config/arc/arc.md (cmpdf): Don't conditionalize on
	TARGET_DPFP, or we'll end up with a libcall instead of
	actually using the FPX patterns.
	
	* config/arc/arc-protos.h (arc_secondary_reload): Delete
	unnecessary decl.

2010-11-08  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.c (arc_secondary_reload): For an input reload,
	any attempt to save to a DOUBLE_REGS register will require
	an intermediate register in order to use "*movdf_insn".
	Adjust to use MEM_P.  Rename from arc_secondary_reload_class.
	(TARGET_SECONDARY_RELOAD): Refer to arc_secondary_reload_class.
	(arc_conditional_register_usage): Make sure the d1/d2 registers
	don't get used for 'c' or 'Rac' constraints.
	
	* config/arc/arc.md (movdf): Make op0 be nonimmediate_operand
	instead of general_operand, to match the style of other ports.
	(*movdf_insn): Use gcc_unreachable.

2010-11-02  Brendan Kehoe  <brendan@zen.org>

	Bring this change in from the 2.3 release to avoid linux
	kernel download problems:
	2007-10-11  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/lib1funcs.asm (__udivsi3, __umodsi3): Comment out
	.__arc_profile_forward section data.

2010-10-27  Brendan Kehoe  <brendan@zen.org>

	* crtstuff.c: Undefine caddr_t to fix MSYS builds.

2010-10-26  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.md (*movdf_insn): Change order of
	constraints so the D class for DOUBLE_REGS is better
	than the w/c pair for generic registers.  Adjust order
	of cases approprately; add comments to the assembly to
	ease identification of which constraint matches.
	* config/arc/arc.c (arc_secondary_reload_class): Rename
	from arc_secondary_reload.
	(TARGET_SECONDARY_RELOAD): Define so it actually gets used.
	(arc_preferred_reload_class): New function, using what
	was in the macro.
	* config/arc/arc.h (PREFERRED_RELOAD_CLASS): Call
	arc_preferred_reload_class() instead.
	(arc_preferred_reload_class): Add decl.

	* config/arc/arc.c (prepare_move_operands): Rename NEW
	to _NEW.

2010-10-11  Brendan Kehoe  <brendan@zen.org>

	* config.gcc (${target}, case arc*-*-*): Add so arc600- or
	arc601- will have the right CPU_TYPE.

	* config/arc/arc.opt (mno-blcc): Add option -mno-blcc.
	* config/arc/arc.c (arc_output_libcall): Forbid BL insn if
	TARGET_NO_BLCC_SET.
	* config/arc/arc.md (*call_i, *call_value_i,
	call_prof, call_value_prof): Remove Cbr cases.
	(call_blcc, call_noblcc, call_value_blcc, call_value_noblcc,
	call_prof_blcc, call_prof_noblcc, call_value_prof_blcc,
	call_value_prof_noblcc): New patterns for Cbr cases.

2010-09-24  Michael Eager  <eager@eagercon.com>

	* config/arc/arc.c (output_shift): Redesign shift loop code.
	* config/arc/arc.md (*shift_si3): Change shift loop length.

2010-09-17  Michael Eager  <eager@eagercon.com>

	* config/arc/t-arc-newlib (MULTILIB_OPTIONS): switch to -mcpu=
	options.  (MULTILIB_MATCHES): Add -mcpu= options.
	* config/arc/arc.h (ASM_SPEC): Reorganize, add -mcpu= options.
	(LINK_SPEC): Add -mcpu=arc700 option.

2010-09-10  Brendan Kehoe  <brendan@zen.org>

	Prevent the compiler from attempting nested LP zero-overhead
	loops since there is only a single LP_COUNT register.
	* config/arc/arc.md (doloop_begin_i): Mark that we will end
	up clobbering LP_COUNT from here.
	(doloop_end): Note the end of the loop uses LP_COUNT as well.

2010-09-03  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.md (*zero_extendhisi2_i): Fix "cond" attr
	for the first alternative to be nocond, since extw.<cc> is
	not valid.
	(*zero_extendqihi2_i): Likewise, since extb.<cc> is not valid.
	(*zero_extendqisi2_ac): Likewise.

	* config/arc/arc.md (andsi3_i): Use 'W' constraint on
	alternative 18 to make sure LDB/LDW do not get LP_COUNT as
	their destination operand.

2010-08-27  Michael Eager  <eager@eagercon.com>

	* config/arc/arc.h (ASM_SPEC): Pass -mARC600 to asm for -mcpu=ARC600.

2010-08-20  Michael Eager  <eager@eagercon.com>

	* config/arc/arc.md: Change length for output_shift to 40.

2010-08-19  Michael Eager  <eager@eagercon.com>

	* config/arc/arc.c: Allow ARC601 to specify -mbarrel_shifter.

2010-08-19  Michael Eager  <eager@eagercon.com>

	* config/arc/predicates.md (long_immediate_operand): Remove
	TARGET_A4 reference.

	* config/arc/t-arc-newlib (MULTILIB_OPTIONS, MULTILIB_DIRNAMES,
	MULTILIB_EXCLUSIONS): Add arc601 multilib.
	
	* config/arc/arc.c: Prototype for arc_cpu_set(). New: arc_cpu,
	TARGET_601, TARGET_SHIFTER, barrel_shifter.
	(arc_init): Set TARGET_SHIFTER, check ARC601 & mmul32x16,
	check ARC601 & -mspfp_fast, check ARC601 & FPX.  
	(arc_cpu_types): Default PROCESSOR_NONE.
	(arc_cpu_set): Remove TARGET_A4, allow upper/lower case CPU type.
	(arc_handle_option): Handle -mbarrel-shifter, -mno-barrel-shifter.
	(output_shift): Generate correct load addr for loop start.

	* config/arc/arc.h (ASM_SPEC): Pass -mARC700 by default.
	ARCOMPACT always 1.  Remove: #define TARGET_SHIFTER.  
	(processor_type): Define PROCESSOR_NONE.
	New: TARGET_ARC601, TARGET_SHIFTER. 

	* config/arc/arc.md: Remove define_delay for TARGET_A4. 

	* config/arc/constraints.md (constraint I): remove TARGET_A4.

	* config/arc/arc.opt: Define -mtune=arc601.


2010-08-05  Michael Eager  <eager@eagercon.com>

	* config/arc/arc-protos.h: Prototype for override_options().

	* config/arc/arc.c: Remove support for A4 processor.
	New: TARGET_A4, TARGET_A5, TARGET_ARC600, TARGET_ARC700.
	New: arc_cpu_type map processor_type to string.
	New: arc_cpu_set() -- determine cpu variant
	New: arc_handle_option() -- process options & set TARGET_ flags.
	New: override_options() -- set cpu variant & set options.

	* config/arc/arc.h: Remove support for A4 processor.
	Remove define for __A4__.
	Pass assembly flags based on -mcpu= flags.
	Remove PROCESSOR_A4, add PROCESSOR_ARC601 to processor_type.

	* config/arc/arc.md: Remove support for A4 processor.
	Remove unused code.  

	* config/arc/arc.opt: Remove support for A4 processor.
	Eliminate auto-generated flags for -mA5, -mARC600, -mARC700.
	Add -mcpu= support.

2010-08-01  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.c (arc_register_move_cost): For the ARC600,
	make a distinctuion between MPY_WRITABLE_CORE_REGS (sans the
	LP_COUNT r60), versus LPCOUNT_REG and WRITABLE_CORE_REGS.
	* config/arc/arc.md (*addsi3_mixed): Use 'W' instead of 'w'
	so LP_COUNT isn't really a choice for add insns, given the
	flurry of delays which could occur around them.

2010-07-26  Brendan Kehoe  <brendan@zen.org>

	* config/arc/t-arc-newlib (MULTILIB_OPTIONS): Instead of
	mA6, use mARC600.
	(MULTILIB_DIRNAMES): Fix arc600 to be first to match the
	order of MULTILIB_OPTIONS.
	(MULTILIB_MATCHES): Use -mARC600 instead of -mA6; lose
	entry for -mARC700=-mA7.  Alias -mARC600 for -mA5.
	Add EL=mlittle-endian.
	(MULTILIB_EXCEPTIONS): Fix to be mnorm*.
	(MULTILIB_EXCLUSIONS): Lose individual !mnorm entry.

2010-07-16  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.md (casesi_compact_jump) <case QImode>:
	Make sure operand0 satisfies_constraint_Rcq() as well,
	otherwise we could end up doing things like trying to
	give R17 to ADD_S.

2010-06-14  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.md (define_peephole2): Restrict pattern to
	only use CC_REG instead of letting anything else get in.
	(negdi2): Fix mode for CC_REG to be CC not SI.
	
	* config/arc/arc.md: Replace all use of the specific register
	number 66 with the define_constants symbol CC_REG.

2010-06-04  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.c (arc_init_builtins): Adjust
	__builtin_arc_aligned to take a 'const void*' argument instead
	of just 'void*', so we don't cause warnings about discarding
	qualifiers.

2010-05-14  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.c (arc_expand_builtin,
	case BUILTIN_ARC_ALIGNED): Correct to use byte alignment for
	non-constants.  Account for get_pointer_alignment giving back 0.

2010-04-30  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.c (enum arc_builtins): Add BUILTIN_ARC_ALIGNED.
	(arc_init_builtins): Add __builtin_arc_aligned.
	(arc_expand_builtin, case BUILTIN_ARC_ALIGNED): Add case which
	implements for both CONST_INT rtx nodes and for non-const
	expressions.

2010-04-19  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.md (mulsi3_700): Restore the arc-int branch
	version; instead of "W" constraint, go with the predicate
	mpy_dest_reg_operand instead.

2010-04-12  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.md: Define a peephole2 optimization for
	operations computing an index based solely on the 31st bit
	of a value.

	* config/arc/arc.md (cbranchsi4_scratch): Correct lock_length
	and length to start with 244 instead of 248 to avoid tickling
	the edge of -256:255 with 256.  Move is_compact attr to the
	end to match *bbit, et al.  Add verify_short checks.
	
	* config.gcc (arc-*-elf*): Default to ARC700 instead of A6.
	(arc*-*-*): Likewise, not just for uclibc.

2010-04-10  Brendan Kehoe  <brendan@zen.org>

	* config.gcc (arc-*-elf*): Grok --with-cpu.
	* config/arc/t-arc-newlib (MULTILIB_DIRNAMES,
	MULTILIB_EXCEPTIONS, MULTILIB_EXCLUSIONS): Tweak for the arc700
	now as the default, with arc600 as the other.s

2010-04-09  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.md (mulsidi3_700): Revert 2008-12-05 change
	for now; the result always fails to split because *movdi_insn
	doesn't like what we're doing.

2010-03-22  Brendan Kehoe  <brendan@zen.org>

	Fix infinite loop compiling fs/buffer.c in the linux kernel
	when using flags '-O3 -mA7 -mlong-calls'.
	* config/arc/arc.md (*bbit): Use verify_short when determining
	the value for the lock_length attribute, to mimic most other
	uses.  ??? cbranchsi4_scratch is the only other insn which isn't
	consistent in its use.

2010-02-08  Brendan Kehoe  <brendan@zen.org>

	Restrict control of the LP_COUNT register to the MULT insn
	specifically, not all commutative operators.
	* config/arc/arc.md (*commutative_binary_comparison_result_used): Revert to 'w' constraint, but change the predicate to commutative_operator_sans_mult.
	(*commutative_binary_mult_comparison_result_used): New pattern similar to the other, but matching the mult_operand specifically, so we can use the 'W' constraint.
	* config/arc/predicates.md (commutative_operator_sans_mult):
	Copy of commutative_operator, without the "mult" TARGET_ARC700 insn.
	(mult_operator): New predicate.

2010-01-29  Brendan Kehoe  <brendan@zen.org>

	* config/arc/arc.c (arc_conditional_register_usage): If
	-ffixed-lp_count, make sure LP_COUNT is not available in any
	register class, nor does it have any acceptable regno mode.

2010-01-27  Brendan Kehoe  <brendan.kehoe@viragelogic.com>

	* config/arc/arc.md ("*commutative_binary_comparison_result_used"): 
	Use the 'W' constraint since MULT is one of the possible
	commutative operators.
	* config/arc/fpx.md: In both define_peephole2 using
	arc_dpfp_operator, specify "W" register class for the destination
	operand.

2010-01-22  Brendan Kehoe  <brendan.kehoe@viragelogic.com>

	* config/arc/constraints.md (MPY_WRITABLE_CORE_REGS): Define for 'W'.
	* config/arc/arc.h (enum reg_class): Add MPY_WRITABLE_CORE_REGS.
	(REG_CLASS_NAMES): Add "MPY_WRITABLE_CORE_REGS".
	(REG_CLASS_CONTENTS): Define similar to 'w' but with the bit for
	register 60 changed to 0.
	* config/arc/arc.md (mulsi3_700): Change constraint from 'w' to 'W'.
	Adjust each instruction's line to have a more helpful comment.
	* config/arc/predicates.md (mpy_dest_reg_operand): Define using
	MPY_WRITABLE_CORE_REGS as its bounds to make sure it cannot
	use LP_COUNT.

2009-08-26  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_expand_prologue): Cast pretend_size to
	HOST_WIDE_INT before negating.
	* config/arc/arc.md (umulsi3_highpart): Don't wrap a ZERO_EXTEND
	around a CONST_INT.

2009-08-19  J"orn Rennecke  <joern.rennecke@arc.com>

	PR regression/41188
	* loop-invariant.c (move_invariant_reg): Check changes for validity.

2009-08-04  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_conditional_register_usage): Fix
	regno_reg_class for ilink[12] on ARC700.

2009-07-27  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (LARGE_INT): Fix 64-bit behaviour.

2009-07-22  J"orn Rennecke  <joern.rennecke@arc.com>

	* doc/tm.texi (TARGET_VALID_OPTION_ATTRIBUTE_P): Change to:
	(TARGET_OPTION_VALID_ATTRIBUTE_P).

2009-05-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/rs6000/rs6000.md (doloop_end): Accept extra operand.

	* config/spu/spu.md (doloop_end): Accept extra operand.

	* optc-gen.awk: For all variables with an Init clause, set var_seen.

2009-05-13  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_expand_prologue): If fame_size_to_allocate
	is narrower than HOST_WIDE_INT, zero-extend it before negation.

2009-05-11  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (enum reg_class): Rename SP_REG to SP_REGS.
	Changed user.

2009-05-01  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_address_cost): Use INTVAL to refer to
	the value of a CONST_INT.

2009-04-07  J"orn Rennecke  <joern.rennecke@arc.com>

	* haifa-sched.c (insn_cost): No longer HAIFA_INLINE.

2009-04-03  J"orn Rennecke  <joern.rennecke@arc.com>

	* longlong.h [__ARC700__] (umul_ppmm): Add earlyclobber
	constraint modifier for operand 1.

2009-02-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* loop-doloop.c (doloop_valid_p): Rename to:
	(validize_doloop).  Try to fix up loops with conditons for infinite
	looping by enclosing them in an outer loop.
	Changed caller.
	(add_test): Add new parameter edgep.  Changed caller.

	* loop-iv.c (get_simple_loop_desc): Use XCNEW.

2009-02-11  J"orn Rennecke  <joern.rennecke@arc.com>

	* postreload.c (reload_combine): Also seek to combine a constant set
	into a single mem-ref use.
	* global.c (find_reg): Don't make allocations that are denied by
	DONT_REALLOC.
	* gcse.c (constprop_register): Don't propagate constants into
	memory accesses, or into binary operations where the rtx_cost is
	non-negligible.
	(one_cprop_pass): Make function calls to provide REG_N_REFS.
	* explow.c (memory_address): Call LEGITIMATE_ADDRESS also in the
	cse-driven case.
	* config/arc/arc-protos.h (arc_legitimize_address): Declare.
	* config/arc/arc.c (TARGET_MIN_ANCHOR_OFFSET): Redefine.
	(TARGET_MAX_ANCHOR_OFFSET): Likewise.
	(prepare_move_operands): Call arc_legitimize_address on memory
	addresses.
	(arc_legitimize_address): New function.
	* config/arc/arc.h (LEGITIMIZE_ADDRESS): Call arc_legitimize_address.
	(LEGITIMIZE_RELOAD_ADDRESS): Decompose reg+offset into
	anchor + small offset, and including non-allocated registers.
	(DONT_REALLOC): Define.
	* config/arc/arc.md (movsi): Replace VUsc/Cal with VUsc/C32 alternative.
	* config/arc/constraints.md (C32): New constraint.

	* config/arc/arc.c (arc_save_restore): Fix handling of extra registers
	for epilogue_p == 2.

2009-02-09  J"orn Rennecke  <joern.rennecke@arc.com>

	PR 39141:
	* tree-ssa-loop-manip.c (gimple_can_duplicate_loop_to_header_edge):
	New function.
	* tree-ssa-loop-ivcanon.c (enum unroll_level): New value
	UL_ESTIMATE_GROWTH.
	(try_unroll_loop_completely): Handle UL_ESTIMATE_GROWTH.
	(canonicalize_loop_induction_variables): Likewise.
	(tree_unroll_loops_completely): Don't completely unroll loops where
	the outer loop/function is larger than
	PARAM_MAX_COMPLETELY_PEELED_OUTER_INSNS, or will/would become thus
	due to unrolling.
	* cfgloop.h (enum li_flags): New value LI_REALLY_FROM_INNERMOST.
	(fel_init): Handle LI_REALLY_FROM_INNERMOST.
	* tree-flow.h (gimple_can_duplicate_loop_to_header_edge): Declare.
	* params.def (PARAM_MAX_COMPLETELY_PEELED_OUTER_INSNS): New parameter.

2009-02-02  J"orn Rennecke  <joern.rennecke@arc.com>

	PR 38785:
	* tree-ssa-pre.c (ppre_n_insert_for_speed_p): New function.
	* (do_partial_partial_insertion): Use it to throttle
	insert_into_preds_of_block calls.
	* common.opt (-ftree-pre-partial-partial-obliviously): New option.

2009-01-15  Steven Bosscher  <steven@gcc.gnu.org>

	http://gcc.gnu.org/ml/gcc-patches/2008-12/msg00199.html
	* opts.c (decode_options): Fix initialization of
	flag_tree_switch_conversion.  Don't set optimize_size in block
	that is conditional on optimize_size (sic).  Explicitly disable
	PRE when optimizing for size (and add comment for rationale).
	* tree-ssa-pre.c: Update outline of the algorithm.
	(bitmap_set_and): Prototype.
	(insert_into_preds_of_block): Don't report discovery of partial
	redundancies here, do so from the callers instead (see below).
	(do_regular_insertion): Add counter for an estimate for the number
	of inserts required to eliminate a partial redundancy.  If the
	current function is optimized for size, only perform the partial
	redundancy elimination if this requires inserting in only one
	predecessor.  Report all found partial redundancies from here.
	(do_partial_partial_insertion): Report them from here too.
	(insert_aux): Do not insert for partial-partial redundancies when
	optimizing for size.
	(execute_pre): Remove bogus ATTRIBUTE_UNUSED.
	(do_pre): Run FRE at least, if PRE is disabled.
	(gate_pre): Return true if flag_tree pre or flag_tree_fre is set.

2009-01-15  J"orn Rennecke  <joern.rennecke@arc.com>

	* common.opt (ftree-pre-partial-partial): New option.
	* opts.c (decode_options): Initialize flag_tree_pre_partial_partial.
	* tree-ssa-pre.c (execute_pre): Use flag_tree_pre_partial_partial.

2009-01-07  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/constraints.md (Rcw): Remove ARC700 conditional.
	(Rcr): New constraint.
	* config/arc/arc.md (mulsi3_700, mulsi3_highpart, umulsi3_highpart_i):
	Use "r" and "Rcr" constraints for destination.

2008-12-17  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_verify_short): For out-of-range
	brcc / bbit with short delay insn, prefer short compare.

2008-12-17  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/adddf3.S (Ladd_same_exp): Fix corner case with
	round-to-even obscuring or generating a carry.
	(Lpast_denorm_large_cancel_sub): Fix handling of shift by 32.
	* config/arc/ieee-754/arc600/muldf3.S (Linf_nan): Avoid clobbering
	DBL1L before we have checked its value.
	* config/arc/ieee-754/arc600-dsp/muldf3.S (Linf_nan): Likewise.
	* config/arc/ieee-754/muldf3.S (Linf_nan): Likewise.

2008-12-16  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_dead_or_set_postreload_p): Constify paramters.
	Don't clear reg while processing a sequence.  Use find_reg_fusage.
	* config/arc/arc-protos.h (arc_dead_or_set_postreload_p): Update.
	* config/arc/arc.md (flush_icache+3): Add comment on found use.
	Remove TARGET_DROS condition.

2008-12-16  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_dead_or_set_postreload_1): New function.
	(arc_dead_or_set_postreload_p): Likewise.
	* config/arc/arc-protos.h (arc_dead_or_set_postreload_p): Declare.
	* config/arc/arc.md (flush_icache+2, flush_icache+3): Use it.
	(flush_icache+4, flush_icache+7): Likewise.

2008-12-15  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (doloop_begin_i): When checking size of loop,
	verify that end belongs to the loop being examined.

2008-12-11  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_encode_section_info): Don't try to get
	TYPE_ATTRIBUTES from error_mark_node.

2008-12-11  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (doloop_end_i): Use %? to output nop of right size.
	Length is not zero when outputting a nop.

2008-12-11  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (movsicc_insn+1): Generate plus.
	Use REVERSE_CONDITION.

2008-12-11  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (adddi3_i): Fix case where we are adding
	a zero lowpart and the destination lowpart is the same as source
	highpart.

2008-12-10  J"orn Rennecke  <joern.rennecke@arc.com>

	* reorg.c (fill_slots_from_thread): Initialize crossing.

2008-12-10  J"orn Rennecke  <joern.rennecke@arc.com>

	* tree-ssa-loop-ivopts.c (get_computation_cost_at): Guard code using
	int_cst_value call with cst_and_fits_in_hwi check.

2008-12-10  Zdenek Dvorak  <ook@ucw.cz>
	    J"orn Rennecke  <joern.rennecke@arc.com>

	PR38440 / PR31849:
	* tree-ssa-loop-ivopts.c (CP_AUTOINC_OFFSET): Define.
	(struct iv_ca): New member cand_autoinc_distance.
	(get_address_cost): New parameter may_autoinc.  Changed all callers.
	(autoincrement_distance, cand_autoincrement_p): New functions.
	(recompute_autoinc_bonus): Likewise.
	(get_computation_cost_at): New parameter autoinc_distance.
	Changed all callers.
	(get_computation_cost): Likewise.
	(iv_ca_set_no_cp <USE_ADDRESS>): Call recompute_autoinc_bonus.
	(iv_ca_set_cp): Likewise.
	(iv_ca_new): Allocate nw->cand_autoinc_distance.
	(iv_ca_dump): Indicate autoincrement.
	(gt-tree-ssa-loop-ivopts.h): Include.
	* common.opt: Add new options fivopts-post-inc and fivopts-post-modify.
	* Makfile.in (GTFILES): Add $(srcdir)/tree-ssa-loop-ivopts.c .
	(tree-ssa-loop-ivopts.o): Depend on gt-tree-ssa-loop-ivopts.h .

2008-12-09  J"orn Rennecke  <joern.rennecke@arc.com>

	cr95949:
	* config/arc/arc.md (movqi_insn): Split w/cI alternative into
	w/cL and w/I.

2008-12-08  J"orn Rennecke  <joern.rennecke@arc.com>

	* hooks.c (hook_bool_const_rtx_const_rtx_true): New function.
	* hooks.h (hook_bool_const_rtx_const_rtx_true): Declare.
	* target.h (can_follow_jump): New hook.
	* target-def.h (TARGET_CAN_FOLLOW_JUMP): Define.
	(TARGET_INITIALIZER): Include it.
	* reorg.c (follow_jumps): New parameters jump and cp.
	Changed all callers.
	* config/arc/arc.c (arc_can_follow_jump): New function.
	(TARGET_CAN_FOLLOW_JUMP): Override.
	* config/arc/arc.md (jump_i): If a REG_CROSSING_JUMP is present,
	length is 4.

	* config/arc/arc.h (arc_compute_function_type): Declare.

2008-12-08  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_reorg): When giving up on zero-overhead
	loop only containing zero size asm, remove loop insn.

2008-12-07  J"orn Rennecke  <joern.rennecke@arc.com>

	* tree-ssa-loop-ch.c (copy_loop_headers): Fixed merge problems
	(tentatively).
	* config/arc/arc.h (OPTIMIZATION_OPPTIONS, OVERRIDE_OPTIONS):
	Fixed merge problem.

2008-12-05  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_need_delay): Don't attempt to get type of
	a SEQUENCE.

2008-12-05  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc700.md (umulti_SI): Split into:
	(umulti_xmac, umulti_std).
	(define_bypass): Remove bogus multiply bypasses.
	* config/arc/arc.md (mulsi3_700): Use mpyu instruction.
	Change type to umulti.
	(mulsidi3_700, umulsidi3_700): Now define_insn_and_split.
	(mulsi3_highpart): New pattern.
	(umulsi3_highpart_i, umulsi3_highpart_int): Now type multi.
	* config/arc/ieee-754/divdf3.S: Take reduced mpyu latency for
	standard multiplier block into account.
	* config/arc/ieee-754/muldf3.S: Likewise.
	* config/arc/ieee-754/divsf3-stdmul.S: Likewise.

2008-12-04  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_save_restore): Set RTX_FRAME_RELATED_P on
	the *emitted* sibthunk_insn.

2008-12-04  J"orn Rennecke  <joern.rennecke@arc.com>

	* varasm.c (get_unnamed_section): Case return value of xmalloc.
	(get_noswitch_section): Likewise.
	(pickle_in_section): Use GGC_NEW.
	(unpickle_in_section): Make definition a prototype.
	* config/arc/arc.c (arc_verify_short): Restore recog_data before
	returning early.

2008-12-04  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (define_delay [return]): Remove duplicate.
	(adddi3): Put parts into parallel.

2008-12-04  J"orn Rennecke  <joern.rennecke@arc.com>

	* genrecog.c (validate_pattern): Accept combinations of VOIDmode with
	MODE_CC operands like ones with CC0.
	* config/arc/predicates.md (zn_compare_operator): Now special predicate.
	* config/arc/arc.md (unary_comparison): Add mode to opreand 1.
	(noncommutative_binary_comparison): Add mode to operand 1 and 2.

	* config/sh/sh.md (doloop_end): Accept extra operand.

	* config/arc/arc.c (arc_compute_frame_size): Don't try to use
	REG_N_SETS.
	(arc_expand_epilogue): Update call to gen_return_i.
	* config/arc/arc.h (EPILOGUE_USES): Define.
	* config/arc/arc.md (in_ret_delay_slot): Reject insns that
	set / clobber the return address register.
	(sibcall, sibcall_insn): Remove explicit return address register use.
	(sibcall_value_insn, return_i): Likewise.

	* config/arc/arc.md (define_delay): Don't put an sfunc in a delay slot.

	* config/arc/arc.md (millicode_sibthunk_ld): Set is_SIBCALL.

	* config/arc/t-arc (gt-arc.h): Remove rule.

2008-12-03  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.opt (mexpand-adddi): New option.
	* config/arc/arc.md (adddi3, subdi3): Make expansion dependent
	on TARGET_EXPAND_ADDDI.

2008-12-03  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_init): Tweak mult_cost defaults.

2008-12-02  J"orn Rennecke  <joern.rennecke@arc.com>

	* combine.c (try_combine): Revert hack to suppress loosing
	'simplifications'.
	(undo_since): Fix loop.
	(combine_simplify_bittest): New function.
	(combine_simplify_rtx, simplify_if_then_else): Use it.
	* config/arc/arc.c (arc_rtx_costs): Check for bbit test.

2008-12-01  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/t-arc-newlib (MULTILIB_EXCEPTIONS): Allow mARC700*/mnorm*.
	Disallow mnorm*.
	(MULTILIB_EXCLUSIONS): Allow mARC700*/mnorm*.  Exclude mARC700/!mnorm.
	* config/arc/arc.h (DRIVER_SELF_SPECS): For -mA7 / -mARC700, provide
	-mnorm.

2008-11-28  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/lib1funcs.asm (__divsi3 [__ARC700__,MULDIV]):
	Avoid references of lp_count.

2008-11-27  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_print_operand): Clarify distinction between
	'#' and '*'.
	* config/arc/arc.md (define_delay <brcc>): Don't say we can annull
	the delay slot if TARGET_AT_DBR_COND_EXEC is set.
	(cbranchsi4_scratch): Change delay slot suffix output directive for
	fallback template to '*'.

2008-11-27  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_text_label): Tolerate NOTE_INSN_DELETED_LABEL.

2008-11-27  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_text_label): New function.
	* config/arc/arc-protos.h (arc_text_label): Declare.
	* config/arc/constraints.md (Clb): Require a text label.

2008-11-27  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (casesi_compact_jump): Fix offset calculation
	for unaligned add_s / ld / add_s / j_s cases.

2008-11-26  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (mixed_code_enabled): Delete.
	(arc_conditional_register_usage): Use TARGET_Q_CLASS.
	* config/arc/arc.h (mixed_code_enabled): Don't declare.
	(OPTIMIZATION_OPTIONS): Initialize TARGET_Q_CLASS.
	(OVERRIDE_OPTIONS): TARGET_MIXED_CODE implies TARGET_Q_CLASS.
	* config/arc/arc.opt (mq-class): New option.

	* config/arc/arc.c (arc_get_unalign, arc_toggle_unalign): New functions.
	* config/arc/arc.h (OPTIMIZATION_OPTIONS): Initialize
	TARGET_COMPACT_CASESI.
	(OVERRIDE_OPTIONS): TARGET_COMPACT_CASESI requires TARGET_Q_CLASS.
	TARGET_COMPACT_CASESI implies TARGET_CASE_VECTOR_PC_RELATIVE .
	(ASM_OUTPUT_ADDR_DIFF_ELT): Add 4 resp. 6 for TARGET_COMPACT_CASESI.
	(CASE_VECTOR_SHORTEN_MODE_1): New macro, broken out of:
	CASE_VECTOR_SHORTEN_MODE).  For TARGET_COMPACT_CASESI, add six to
	MAX_OFFSET before passing it on to CASE_VECTOR_SHORTEN_MODE_1.
	(cmpsi_cc_insn_mixed, casesi_jump, casesi_load): Generate expander.
	(casesi): Emit casesi_compact_jump for TARGET_COMPACT_CASESI.
	(casesi_compact_jump): New pattern.
	* config/arc/arc-protos.h (arc_get_unalign): Declare.
	(arc_toggle_unalign): Likewise.
	* config/arc/arc.opt (mcompact-casesi): New option.

	* config/arc/arc.c (arc_legitimate_pic_addr_p): LABEL_REF is ok.
	(arc_raw_symbolic_reference_mentioned_p): Don't flag LABEL_REF.
	(arc_legitimize_pic_address): Leave LABEL_REF alone.
	* config/arc/arc.md (movsi_insn): Add w/Clb alternative.
	* config/arc/constraints.md (Clb): New constraint.

2008-11-26  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_verify_short): Fix typo in test for next insn
	being compact.
	* config/arc/arc.md (ashlsi3_insn_mixed): Fix type in insn predicate.
	(ashrsi3_insn_mixed, lshrsi3_insn_mixed): Likewise.

2008-11-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_next_active_insn): Don't use label_to_alignment
	before arc_reorg.
	(arc_verify_short): Allow NEXT_INSN (insn) to be NULL before arc_reorg.
	* config/arc/arc.md (return_i): Make length 2 dependent on
	arc_verify_short.

2008-11-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_label_align): Don't call recog_memoized on
	non-insns.

2008-11-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* final.c (uid_lock_length): Now file-scope variable.
	(get_attr_lock_length): Use it.
	(shorten_branches): Clear uid_lock_length after freeing it.
	* config/arc/arc.c (arc_adjust_insn_length): Use get_attr_lock_length
	to find length of first part of a SEQUENCE.

	* config/arc/arc.c (arc_setup_incoming_varargs): Remove unused variable.

2008-11-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (delay_slot_length): New attribute.
	(branch_insn, rev_branch_insn, jump, cbranchsi4_scratch, bbit): Use it.

	* config/arc/arc.md (call_i): Refine cond attribute for Cbr alterative.
	(call_value_i): Likewise.

2008-11-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc-protos.h (arc_split_dilogic): Move declaration inside
	RTX_CODE conditional.
	* config/arc/ieee-754/arc600-dsp/mulsf3.S: Turn off debug code.
	* config/arc/ieee-754/arc600-dsp/muldf3.S: Likewise.
	* config/arc/ieee-754/arc600-dsp/divsf3.S: Likewise.
	* config/arc/ieee-754/arc600-dsp/divdf3.S: Likewise.

2008-11-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arg_setup_incoming_varargs): Don't add increase
	alignment.  Always set pretend_size.
	(arc_va_start): Delete.
	(arc_va_arg): TYPE_ALIGN should be no larger than BITS_PER_WORD.
	* config.arc/arc-protos.h (arc_va_start): Don't declare.
	* config/arc/arc.h (FUNCTION_ARG_BOUNDARY): Delete.
	(EXPAND_BUILTIN_VA_START): Delete.

2008-11-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (CASE_VECTOR_MODE): When not optimizing, use SImode.

2008-11-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_print_operand <!>): Print condition before _s.

2008-11-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_verify_short): Fix calculation of this_sequence.

2008-11-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_need_delay): Don't use num_delay_slots
	on non-insns.

2008-11-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/predicates.md (cc_use_register): New predicate.
	* config/arc/arc.c (arc_need_delay): New function.
	* config/arc/arc-protos.h (arc_need_delay): Declare.
	* config/arc/arc.md (addi3): Check first if l0 is const0_rtx.
	Fix length attribute.
	(add_f): Remove constant alternative constraint parts from operand1.
	Add Rcw/0/I alternative.
	(adc): Likewise.  Don't split if we anticipate this insn to go into
	a delay slot.  Simplify SET_SRC of first splitter output insn.
	(add_f_2): New pattern.
	(adc+1): Remove constraints.
	(subdi3): New define_expand.
	(subdi3_i, sbc_0, sbc, sub_f, sub_f+1, sub_f+2): New patterns.
	(negdi2): Delete.

2008-11-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_split_dilogic): New function.
	* config/arc/arc-protos.h (arc_split_dilogic): Declare.
	* config/arc/arc.md (adddi3): Avoid non-cononical rtl when low part
	of opperands[2] is const0_rtx.
	(add_f): Call extract_insn_cached at end of predicate test.
	(anddi3, iordi3, xordi3): New define_insn_and_split.  New alternatives.
	(negdi2): New alternatives.

2008-11-22  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (LABEL_ALIGN): Define.
	* config/arc/arc.c (arc_label_align): New function.
	* config/arc/arc-protos.h (label_align): Declare.

	* config/arc/arc.md (adc): Restore operands before emitting split.

2008-11-22  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (add_f): Fix pattern.
	(adc+2): Adjust generated pattern to add_f fix.
	* config/arc/arc.c (arc_sets_cc_p): When being passed a sequence,
	look at the delay slot insn.

2008-11-21  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (call_i, call_value_i): Fix length for potentially
	compact alternative.

2008-11-21  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (adddi3): Don't use w/c/i alternative for
	reloading.
	Try to change to subdi3 first.
	Use conditional add of 1 rather than add.f 0 / add.
	(subdi3): Allow partial overlap when adding a constant.

2008-11-21  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (addsi_compare_2): Change predicate for operand 2
	to nonmemory_operand.

2008-11-21  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_select_cc_mode): Use CC_ZNmode for test of
	(a+b) == 0.
	(arc_rtx_costs): Add some add.f / adc special cases.
	(arc_attr_type, arc_sets_cc_p): New functions.
	(arc_scheduling_not_expected): Likewise.
	* config/arc/arc-protos.h (arc_attr_type, arc_sets_cc_p): Declare.
	(arc_scheduling_not_expected): Likewise.
	* config/arc/arc700.md (core_insn): Split out:
	(cmove).
	(cc_arith): New reservation.
	(define_bypass): Mention more insns for which compare has latency 1.
	Add compare / cmove vypass with latency 2.
	* config/arc/arc.md (attribute type): Add value cc_arith
	(addsi_compare): Change CC_REG mode to CC_ZN.
	(addsi_compare_2, addsi_compare_3): New patterns.
	(adddi3): Now define_insn_and_split.  Add more alternatives.
	(add_f, adc, adc+1,adc+2): New patterns.
	(adddi3): Add more alternatives.
	(add_cond_exec, commutative_cond_exec, sub_cond_exec): Now type cmove.
	(noncommutative_cond_exec): Likewise.

	* config/arc/arc.c (arc_save_restore): Emit sibcall thunk insn as
	jump insn.

	* combine.c (combine_validate_cost): Revert to old behaviour when
	combining two insns to one.

2008-11-21  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/predicates.md (immediate_usidi_opperand): New predicate.
	* config/arc/arc.md (umulsi3_highpart_int): Use it.
	(umulsidi3_highpart): Avoid using negative CONST_INTs.

2008-11-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (casesi_jump): New pattern.
	(casesi): Use it.

2008-11-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_output_casesi_insn): Delete.
	* config/arc/arc-protos.h (arc_output_casesi_insn): Don't declare.
	* config/arc/arc.h (OPTIMIZATION_OPTIONS):
	Set TARGET_CASE_VECTOR_PC_RELATIVE to (SIZE !=0).
	(JUMP_TABLES_IN_TEXT_SECTION): Also true for
	TARGET_CASE_VECTOR_PC_RELATIVE.
	(ASM_OUTPUT_ADDR_DIFF_ELT): Use proper directives for QImode / Simode.
	(LABEL_ALIGN_AFTER_BARRIER): Always align at least to 2^1 after an
	ADDR_DIFF_VEC.
	(CASE_VECTOR_PC_RELATIVE, CASE_VECTOR_SHORTEN_MODE): Define.
	(ADDR_VEC_ALIGN): Likewise.
	(ASM_OUTPUT_BEFORE_CASE_LABEL): Override.
	* config/arc/arc.md (UNSPEC_CASESI): New constant.
	(indirect_jump): Expand q alternative to Rcqq.
	(casesi_insn): Deleted.
	(casesi_load): New pattern.
	(casesi): Rewrite to use casesi_load.
	* config/arc/arc.opt (mcase-vector-pcrel): New option.

	* config/arc/arc.md (VUNSPEC_EPILOGUE): Deleted.
	(ARC_UNSPEC_PROLOGUE_USE): Likewise.

2008-11-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_expand_epilogue): Don't use a sibcall thunk
	in a sibcall epilogue.

	* config/arc/arc.c (arc_regno_use_in): New function.
	* arc-protos.h (arc_regno_use_in): Declare.
	* config/arc/arc.md (in_call_delay_slot): New predicate.
	(in_sfunc_delay_slot): Use in_call_delay_slot and arc_regno_use_in.
	(define_delay): Use separate form for call, using in_call_delay_slot.

	* config/arc/arc.md (subsi3_insn): Fix output template for w/c/Cal
	alternative.

2008-11-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_compute_frame_size):
	Don't zero the frame size even if it's the same as extra_size.

2008-11-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_save_restore): Load offset for blink into r12.
	(arc_expand_epilogue): No sibthunk if there are pretend_args.

2008-11-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/predicates.md (millicode_load_operation):
	Pass 2 as second argument to arc_check_millicode.
	(millicode_load_clob_operation): New predicate.
	* config/arc/arc.c (arc_compute_millicode_save_restore_regs):
	Lower the required number of registers to save.
	(arc_save_restore): If epilogue_p is 2, emit a thunk sibcall.
	(arc_expand_epilogue): Check if we can emit a thunk sibcall.
	(arc_next_active_insn): Check for NULL / BARRIER immediately
	after advancing to next insn.
	(arc_check_millicode): For load_p == {0,1}, require a final clobber.
	Interpret load_p == 2 as no final clobber
	is required, and minimumcount reduced to 2.
	* config/arc/arc.md (millicode_thunk_st): Take final clobber into
	account.
	* config/arc/arc.md (millicode_thunk_ld): Use millicode_load_clob
	predicate.  Take final clobber into account.
	(millicode_sibthunk_ld): New pattern.
	* lib1funcs.asm (L_millicode_thunk): Split into:
	(L_millicode_thunk_ld, Lmillicode_thunk_st).  Bracket functions in
	HIDDEN_FUNC / END_FUNC.  Remove 1 / 2 register save / restore,
	and optimize scheduling of function end.
	(L_millicode_thunk_ld_ret): New part.
	* t-arc (LIB1ASMFUNCS): Replace _millicodethunk with
	_millicode_thunk_st, _millicodethunk_ld and  _millicodethunk_ret.

2008-11-19  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_save_restore): Allow larger values
	of *first_offset for millicode generation if generating an epilogue.
	(arc_expand_epilogue): Use cfun->machine->frame_info.millicode_end_reg
	to decide if using a millicode call.
	Don't make fram pointer use dependent on millicode generation.
	Refine decision on how to adjust stack.

2008-11-19  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (IS_POWEROF2_P): Reject 0.
	* config/arc/arc.md (attribute "length"):
	Shift of immidiate has length 8.
	* config/arc/arc.md (C1p): Reject 0.

2008-11-19  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (andsi3): Call arc_rewrite_small_data on
	operands[1] when indicated.

2008-11-19  J"orn Rennecke  <joern.rennecke@arc.com>

	Wrap up of changes going back to r59971
	* final.c (get_attr_length_1): Use direct recursion rather than
	calling get_attr_length.
	(get_attr_lock_length): New function.
	(INSN_VARIABLE_LENGTH_P): Define.
	(shorten_branches): Take HAVE_ATTR_lock_length into account.
	Don't overwrite non-delay slot insn lengths with the lengths of
	delay slot insns with same uid.
	* genattrtab.c (lock_length_str): New variable.
	(make_length_attrs): New parameter base.
	(main): Initialize lock_length_str.
	Generate lock_lengths attributes.
	* genattr.c (gen_attr): Emit declarations for lock_length attributes.

	* genoutput.c (process_template): Process '*' in '@' alternatives.

	* read-rtl.c (read_rtx_1): Terminate when reading EOF.

	Attempt at avoiding zero_extract->lshiftrt which didn't go far enough:
	* combine.c (undo_since): New function, broken out of:
	(undo_all).
	(try_combine): Don't do pure simplifications if they increase the cost.

	* combine.c (combine_validate_cost): Be more strict in !i1 case.
	(try_combine): Allow to combine-split two expensive insns into two less
	expensive insns.

	* config/arc/predicates.md (extend_operand): New predicate.
	(equality_comparison_operator, millicode_store_operation): Likewise.
	(millicode_load_operation): Likewise.
	(cc_register): Use CC_REG.
	* config/arc/arc.c (struct arc_ccfsm): New struct.
	(arc_ccfsm_state, arc_ccfsm_current_cc): Deleted.
	(arc_ccfsm_target_insn, arc_ccfsm_target_label): Likewise.
	(cfa_offset, cfa_store_offset, doing_dwarf): Likewise.
	(last_insn_set_cc_p): Likewise.
	(TARGET_ASM_FUNCTION_PROLOGUE): Likewise.
	(arc_ccfsm_current, ARC_CCFSM_BRANCH_DELETED_P): Define.
	(ARC_CCFSM_RECORD_BRANCH_DELETED, ARC_CCFSM_COND_EXEC_P): Likewise.
	(CCFSM_ISCOMPACT, CCFSM_DBR_ISCOMPACT): Likewise.
	(arc_output_function_prologue): Renamed to:
	(arc_expand_prologue).  No parameters.  Now output rtl.
	No longer static.
	(arc_init): Set arc_punct_chars['&'].
	(enum arc_cc_code_index): New enum.
	(get_arc_condition_code): Use it.
	(arc_select_cc_mode): Use CC_Zmode if we want to emit add / bmsk.f .
	(frame_insn, frame_move, frame_move_inc, frame_add): New functions.
	(frame_stack_add): Likewise.
	(struct arc_frame_info): Add GTY marker.
	New members millicode_start_reg and millicode_end_reg.
	(current_frame_info): Deleted.  Changed all users to use
	cfun->machine->frame_info instead.
	(machine_function): New struct / typedef.
	(arc_compute_function_type): Take a struct function * parameter.
	Initialize fun->machine->fn_type.
	Changed all callers.
	(ILINK1_REGNUM, ILINK2_REGNUM, RETURN_ADDR_REGNUM): Delete.
	(MUST_SAVE_RETURN_ADDR): Only require save if register is set.
	(arc_compute_millicode_save_restore_regs): Rewrite.
	Take struct arc_frame_info * parameter.  Changed all callers.
	(arc_save_restore): No longer take file, op, parity parameters.
	Add epilogue_p parameter.  Changed all callers.
	Emit rtl.
	(arc_return_address_regs): New global scope array.
	(arc_output_function_epilogue): Renamed to:
	(arc_expand_epilogue).  Take only sibcall_p parameter.
	Now output rtl.  Changed all callers.
	(arc_finalize_pic): Use Pmode for addresses.
	Don't add PROGRAM_COUNTER_REGNO.  Use gen_rtx_SET.
	Emit using emit_insn.  Don't emit a USE.  Return emitted insn.
	(arc_cond_exec_p): Rename to:
	(arc_ccfsm_cond_exec_p).  Changed all callers.
	Use cfun->machine->prescan_initialized / arc_ccfsm_current.
	(arc_print_operand): Don't emit delay suffix if delay insn has
	been deleted.  Take TARGET_AT_DBR_CONDEXEC into account.
	Use ARC_CCFSM_COND_EXEC_P.
	For '?' / '!', call output_short_suffix if the insn can be short.
	Add '&' case for TARGET_ANNOTATE_ALIGN.
	(record_cc_ref): Deleted.  Changed all callers.
	(arc_ccfsm_advance): New function, broken out of:
	(arc_final_prescan_insn).
	(arc_ccfsm_at_label): Add state parameter.  Changed all callers.
	Now static.
	(arc_ccfsm_record_condition, arc_ccfsm_post_advance): New functions.
	(arc_ccfsm_branch_deleted_p): Use ARC_CCFSM_RECORD_BRANCH_DELETED.
	(arc_ccfsm_advance_to, arc_next_active_insn): New functions.
	(arc_verify_short, output_short_suffix): Likewise.
	(arc_final_prescan_insn): Clear arc_ccfsm_current if this is the
	first call for the current function.
	Clear cfun->machine->size_reason.
	(branch_dest): Cope with PARALLEL.
	(arc_rtx_costs): Handle more cases of cheap constants.
	Reflect higher cost of shifting a constant.
	Add cases for btst / bit / bmsk.f .
	(arc_is_longcall_p): Remove call_symbol parameter.
	Changed all callers.
	(arc_reorg_in_progress): New variable.
	(arc_reorg): Set cfun->machine->arc_reorg_started and
	arc_reorg_in_progress.  Clear cfun->machine->ccfsm_current_insn after
	each shorten_branches call.
	Genrate bbit as well as brcc insn.  Express suitability for brcc_s
	with mode of clobber.  If brcc insn are present from before arc_reorg,
	wix up the clobber mode.
	Clear arc_reorg_in_progress at the end.
	(valid_brcc_with_delay_p): Update for changed cbranchsi4_scratch (brcc)
	pattern.  Use brcc_nolimm_operator.
	(arc_output_addsi): Don't check arc_size_opt_level.
	Use %? directive.  Add logic to exploit commutativity for
	matching constraint in reg/reg/reg case.
	(arc_output_libcall): Use %! directive.  Take TARGET_MEDIUM_CALLS into
	account.
	(arc_insn_length_adjustment): Rename to:
	(arc_adjust_insn_length).  Take length parameter.  Changed all callers.
	Add SEQUENCE case.
	Add code for TARGET_PAD_RETURN.
	Add code to take ccfsm actions into account.
	(arc_unalign_branch_p, arc_branch_size_unknown_p): New functions.
	(arc_pad_return, arc_init_machine_status): Likewise.
	(arc_init_expanders, arc_check_millicode, arc_clear_unalign): Likewise.
	(split_addsi, split_subsi, arc_split_move, arc_short_long): Likewise.
	(gt-arc.h): Include.
	* config/arc/arc.h (TARGET_UNALIGN_BRANCH, TARGET_UPSIZE_DBR): Define.
	(TARGET_PAD_RETURN, TARGET_AT_DBR_CONDEXEC): Likewise.
	(OPTIMIZATION_OPTIONS): Initialize TARGET_Rcq, TARGET_Rcw,
	TARGET_ALIGN_CALL, TARGET_EARLY_CBRANCHSI and TARGET_BBIT_PEEPHOLE.
	(ARC_STACK_ALIGN): Use STACK_BOUNDARY.
	(REG_CLASS_CONTENTS): Include ap / pcl in WRITABLE_CORE_REGS.
	(ASM_OUTPUT_ALIGN): Call arc_clear_unalign.
	(arc_return_address_regs): Declare.
	(CAN_DEBUG_WITHOUT_FP, CALL_ATTR, INIT_EXPANDERS): Define.
	(TARGET_DROSS, DROSS): Likewise.
	* config/arc/arc.md (SP_REG, ILINK1_REGNUM): New constants.
	(ILINK2_REGNUM, RETURN_ADDR_REGNUM, CC_REG): Likewise.
	(is_sfunc, is_CALL, is_SIBCALL, is_NON_SIBCALL): New attributes.
	(attribute type): New value jump.  Use special defauilt logic if
	isfunc attribute is "yes".
	(attribute iscompact): New values maybe, true_limm, maybe_limm.
	Default to "maybe" if type is "sfunc".
	(attribute cond): New values canuse_limm and canuse_limm_add.
	(verify_short, delay_slot_filled, delay_slot_present): New attributes.
	(lock_length): Likewise.
	(attribute length): Add no-op clause to mark compact insns as
	varying length, and make their length dependent on verify_short.
	Set length of stores that store an immediate to 8.
	Remove (eq_attr "cond" "set,set_zn,clob") clause.
	Insn of type call_no_delay_slot have length 8.
	Make use of new iscompact values.
	(attribute in_delay_slot): Check for type "jump".
	(cond_delay_insn, in_ret_delay_slot): New attributes.
	(cond_ret_delay_insn): Likewise.
	(define_delay forms): Check new insns types; add define_delay for
	type "return" insns.
	Separate branch/uncond_branch/jump define_delay and use
	cond_delay_insn where indicated.
	(all ARCompact patterns): Use %? / %! with %& instead of _s.  Add new
	attributes / use new attribute values.  Rearrange alternatives
	to make sure that short insns and conditional execution can be used.
	Remove dependency of short insns emitting on arc_size_opt_level.
	(movsi_pre_mod, tst, bset, bxor, bclr, btst): New patterns.
	(movdi_insn, movdf_insn, addsi3_mixed): New define_insn_and_split.
	(subsi3_insn): Likewise.
	(movsicc_insn+1, btst+1): New peephole2 patterns.
	(shift_and_add_insn_mixed, shift_and_add_insn): Delete.
	(shift_and_sub_insn, bset_insn_mixed, bclr_insn_mixed): Likewise.
	(bmsk_insn_mixed): Likewise.
	(andsi3): Renamed to:
	(andsi3_i). Add memory / register zero_extend alternatives.
	(andsi3): New expander.
	(andsi3_i+1): New splitter.
	(call_via_reg_mixed, call_via_label, call_via_imm): Merge to:
	(call_i).
	(call_value_via_reg_mixed, call_value_via_label, call_value_via_imm):
	Merge to:
	(call_value_i).
	(flush_icache+1, flush_icache+2, flush_icache+5): Mark as DROSS.
	(flush_icache+6, flush_icache+7, flush_icache+8): Likewise.
	(flush_icache+9, flush_icache+10, flush_icache+11): Likewise.
	(flush_icache+12): Likewise.
	(sibcall, sibcall_value, sibcall_insn): Add use of RETURN_ADDR_REGNUM.
	(sibcall_value_insn): Likewise.
	(prologue, return_i, bbit, millicode_thunk_st): New patterns.
	(millicode_thunk_ld): Likewise.
	(epilogue_i): Deleted.
	(cbranchsi4_scratch): Include a clobber of CC_REG.  The mode of the
	clobber indicates if brcc_s is possible.
	* config/arc/arc-protos.h (arc_finalize_pic): Update prototype.
	(arc_compute_function_type, arc_is_longcall_p): Likewise.
	(arc_ccfsm_at_label, arc_cond_exec_p): Remove prototype.
	(arc_insn_length_adjustment): Likewise.
	(arc_ccfsm_cond_exec_p, arc_adjust_insn_length): Add prototype.
	(arc_unalign_branch_p, arc_branch_size_unknown_p): Likewise.
	(arc_ccfsm_record_condition, arc_expand_prologue): Likewise.
	(arc_expand_epilogue, arc_init_expanders): Likewise.
	(arc_check_millicode, arc_clear_unalign, split_addsi): Likewise.
	(split_subsi, arc_pad_return, arc_split_move): Likewise.
	(arc_shorten_align, arc_ccfsm_advance_to, arc_verify_short): Likewise.
	(arc_short_long): Likewise.
	* config/arc/t-arc (gt-arc.h): New rule.
	($(out_object_file)): Depends on gt-arc.h.
	* config/arc/constraints.md (Rcw): Now also register constraint.
	(CnL, CmL, CL2, CM4, Csp, C0p, C1p, Ccp, Cux, Crr): New constraints.
	(Us<, Us>, Rcw, Rcb, Rck): Likewise.
	(Cbr): Use arc_is_longcall_p.
	(Rcq): Don't check arc_size_opt_level, but rather TARGET_Rcq.
	* config/arc/arc.opt (m2addr, munalign-prob-threshold=): New options.
	(mmedium-calls, mannotate-align, malign-call, mRcq, mRcw): Likewise.
	(mearly-cbranchsi, mbbit-peephole): Likewise.

	* config/arc/lib1funcs.asm (__clzsi2): Shorten the !__ARC_NORM__ case.

2008-11-18  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (one_cmpldi2): Change to a define_insn_and_split.

2008-11-18  J"orn Rennecke  <joern.rennecke@arc.com>

	cr95728:
	* config/arc/arc.md (sibcall, sibcall_value): If call address
	is for a longcall, load it into a register.

2008-11-10  J"orn Rennecke  <joern.rennecke@arc.com>

	Fix debug info problem with pretend_args:
	* config/arc/arc.h (CFA_FRAME_BASE_OFFSET, ARG_POINTER_CFA_OFFSET):
	Define.

2008-11-10  J"orn Rennecke  <joern.rennecke@arc.com>

	* genoutput.c (note_constraint):  Ignore redefinition.
	* genpreds.c (struct constraint_data): New member is_overloaded.
	(add_constraint): Allow constraint overloading.
	(write_enum_constraint_num): Don't emit duplicates for overloaded
	constraints.
	(write_lookup_constraint, write_insn_constraint_len): Likewise.

	* dwarf2out.c (dwarf2out_frame_debug_expr): Don't abort on
	expressions from function epilogue.

FIXME: Document changes since rev 59970

2008-10-10  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (*add_n): Fix length of c/c/Cal alternative.

2008-10-08  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_output_function_epilogue): Remove
	noepilogue logic.

	* config/arc/arc.md: (type): Add return.
	(toplevel): Add define_delay for "return" type patterns.
	(epilogue_i): Remove !optimize_size condition.  Set type to return.

2008-10-03  Khurram.Riaz@arc.com
	    J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (BITS_BIG_ENDIAN): Always 0.

2008-09-29  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (TARGET_ASM_FUNCTION_EPILOGUE): Don't redefine.
	(arc_output_function_epilogue): No longer static.
	Take new parameter sibcall_epilogue.
	* config/arc/arc-protos.h (arc_output_function_epilogue): Declare.
	* config/arc/arc.md (sibcall_epilogue): Now an expander.
	(epilogue_i, epilogue): New patterns.

2008-09-29  J"orn Rennecke  <joern.rennecke@arc.com>

	* target-def.h (TARGET_PRESERVE_RELOAD_P): Fix spelling.

2008-09-24  Muhammad Khurram Riaz <khurram.riaz@arc.com>

	* config/arc/predicates.md: (move_dest_operand) for MUL64 target 
	r57-r59 cannot be destination, the manual says they are read only.

2008-09-23  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/mxp/mxp.opt (mno-vld-label): New option.
	* config/arc/mxp.md (*mov<mode>_i): Support TARGET_NO_VLD_LABEL.

	* config/mxp/mxp.opt (mno-vim-label): New option.
	* config/arc/mxp.md (*mov<mode>_i): Use vmovw for HImode.
	(addhi3): Support TARGET_NO_VIM_LABEL.

2008-09-19  Muhammad Khurram Riaz <khurram.riaz@arc.com>

	* config/arc/arc.md : 
	(mulsi3): for MULMAC32x16 unsigned range can be upto 65535, and 
	mululw in enough (alongwith mov from acc2) to get the result (no
	need for use mac instruction) ; note that single mul instruction can not 
	be used with immediates less then 0 because 
	mululw 0,reg,limm ( and also mullw ) treats lower 16bits of imm operand 
	as unsigned, So in this case mac instruction is also required.
	(umul_600) : limm can be the last operand in dsp-mul instructions
	(mac_600, mul64_600, mac64_600, umul64_600, umac64_600) : Likewise

2008-09-18  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/mxp/mxp.md (store_scalars, load_scalars): Add type attribute.
	(mov<mode>_i): Fix type attribute.

2008-09-17  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/mxp/mxp.h (ASM_OUTPUT_SYMBOL_REF): Define.
	(ASM_OUTPUT_LABEL_REF): Likewise.

2008-09-17  J"orn Rennecke  <joern.rennecke@arc.com>

	Enable use of symbol / label references.
	* config/mxp/mxp.c (BINUTILS_FIXED): Define.

	* config/mxp/mxp.md (attribute type): Add load and store.
	(*mov<mode>_i): Set type of load/store instructions.
	(branch_true, branch_false): Set type.  Emit delay slot nops.
	(decrement_and_branch_until_zero, doloop_end): New patterns.
	(mxp): New scheduling automaton.
	* config/mxp/mxp.c (mxp_print_operand): When emitting just one
	delay slot nop, check if it should be between the two delay slot
	insns.

2008-09-15  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/mxp/mxp.md (smax<mode>3, smin<mode>3): New patterns.
	* config/mxp/mxp.opt (mint16): New option.
	* config/mxp/mxp.h (INT_TYPE_SIZE): Take TARGET_INT16 into account.

2008-09-15  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/ieee-754/arc600-dsp/mulsf3.S (.Ldenorm_dbl1):
	Fix register number of multiplicant.

2008-09-15  Muhammad Khurram Riaz <khurram.riaz@arc.com>

	* config/arc/arc.md : Bug fix in mul/mac instructions
	* config/arc/arc.c : Bug fix in mul/mac instructions

2008-09-13  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/ieee-754/arc600-dsp/mulsf3.S: Fix ld.as offsets.

	* config/arc/ieee-754/arc600/divsf3-stdmul.S: Rename to:
	* config/arc/ieee-754/arc600/divsf3.S.
	* config/arc/ieee-754/arc600-dsp/divsf3-stdmul.S: Rename to:
	* config/arc/ieee-754/arc600-dsp/divsf3.S.
	* config/arc/lib1funcs.asm (L_divsf3): update.

2008-09-12  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/ieee-754/arc600-dsp/muldf3.S: Fix offset for loading
	0x7ff00000.

	* config/arc/dp-hack.h: Fix #ifdef condition for L_make_df etc.
	* config/arc/ieee-754/arc600-dsp/divdf3.S: Fix issue where acc1 was
	used as input to mululw/machulw sequence.
	* config/arc/ieee-754/arc600-dsp/divsf3-stdmul.S: Likewise.
	Re-schedule denorm_fp1 handling to reduce code duplication.

	* config/arc/ieee-754/arc600/divsf3-stdmul.S (.Ldenorm_fp0):
	Move upwards to execute mulu64.

2008-09-12  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (TARGET_CPU_CPP_BUILTINS): If
	TARGET_MULMAC_32BY16_SET, define __ARC_MUL32BY16__ .
	* config/arc/lib1funcs.asm (L_muldf3, L_mulsf3, L_divdf3, L_divsf3):
	Add __ARC_MUL32BY16__ alternative.
	* config/arc/ieee-754/arc600-dsp/divdf3.S: New file.
	* config/arc/ieee-754/arc600-dsp/divsf3-stdmul.S: Likewise.

	* config/arc/ieee-754/divdf3.S: Move constant data to end of file,
	change code to address it to save limm.
	* config/arc/ieee-754/divsf3-stdmul.S: Hard-code sub3 argument to
	avoid limm.
	* config/arc/ieee-754/arc600/divsf3-stdmul.S: Likewise.
	* config/arc/ieee-754/arc600/divdf3.S: Likewise.  Fix flag setting
	problem when divisor is denormal.

	* config/arc/arc.c (arc_compute_millicode_save_restore_regs): Only
	consider save sequences starting with r13.

2008-09-05  Muhammad Khurram Riaz <khurram.riaz@arc.com>

	* config/arc/arc.h : WCHAR_T set to int type

2008-09-05  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (doloop_fallback): Operand 0 is "+r,!w".
	(doloop_fallback_m): Operand 0 is "+&r".

2008-09-04  J"orn Rennecke  <joern.rennecke@arc.com>

	* Makefile.in (targhooks.o): Depend on OPTABS_H.
	* config/arc/arc.md (mulsidi3): Remove some junk.
	(mulsidi3_700, umulsidi3_700): Remove 'J' Constraint.
	* config/arc/ieee-754/arc600-dsp: New directory.

2008-09-02  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/t-arc-newlib (MULTILIB_EXCLUSIONS): Re-enable multilibs
	with -mnorm.
	* config/arc/arc.h (ASM_SPEC): For -mmmul32x16, pass -mdsp.

	* config/arc/lib1funcs.asm (__mulsi3): Fix invalid insn issues.

	* config/arc/arc.md (*call_via_label): Replace %^%Pn with %Pn.
	(call_prof, *call_value_via_label, call_value_prof): Likewise.
	(*sibcall_insn, *sibcall_value_insn, sibcall_prof): Likewise.
	(sibcall_value_prof): Likewise.

	* config/arc/arc.md (doloop_begin_i): When n_insns is 0, output
	three nops for ARC600.

	* config/arc/predicates.md (arc_double_register_operand): Fix
	indentation.
	(vector_register_operand, vector_register_or_memory_operand): Likewise.
	(arc_dpfp_operator): Likewise.
	(acc1_operand, acc2_operand, mlo_operand, mhi_operand): New predicate.
	* config/arc/arc.c (arc_init): For ARC600 with TARGET_MUL64_SET,
	default multcost to 4.
	(rname56, rname57, rname58, rname59): New character arrays.
	(arc_init_regno_reg_class): Rename to:
	(arc_conditional_register_usage).  Add code from
	arc.h:CONDITIONAL_REGISTER_USAGE.  Establish special multiply result
	register names, and use register numbers that work well with
	the target endianness.
	For ARC700, disable use of lp_count for SFmode.
	(gen_acc1, gen-acc2, gen_mlo, gen_mhi): New functions.
	* config/arc/arc-protos.h (arc_init_regno_reg_class): Don't declare.
	(arc_conditional_register_usage): Declare.
	(gen_acc1, gen-acc2, gen_mlo, gen_mhi): Likewise.
	* config/arc/arc.h (CONDITIONAL_REGISTER_USAGE): Use
	arc_conditional_register_usage.
	(rname56, rname57, rname58, rname59): Declare.
	(REGISTER_NAMES): Use them.
	(DBX_REGISTER_NUMBER): Translate internal numbers for multiply
	result registers into true hardware register numbers.
	* config/arc/arc.md

	* testsuite/gcc.c-torture/execute/ieee/denorm-rand.c (main):
	Reduce iteration counts to 1000.
	* varasm.c (pickle_in_section): Make defintion a declaration.
	* config/arc/arc.c (arc_print_operand): Add 'o' to output
	symbol without '@'.
	(write_profile_sections): Use it.

	* config/arc/arc.c (arc_final_prescan_insn): Call
	extract_constrain_insn_cached after calling arc_hazard.
	(arc_reorg): Check second parameter of compare before feeding it
	to cbranchsi4_scratch.
	* config/arc/arc.md (*unary_comparison_result_used): Use higher
	operand numbers for match_operator than for ordinary operands.
	Use match_dup for duplicated expression in match_operator.
	(*commutative_binary_comparison_result_used): Likewise.
	(*noncommutative_binary_comparison_result_used): Likewise.
	(mulsi3): Generate expansion for TARGET_MUL64_SET.
	(mulsi3): Refer to multiply result registers with special generator
	/ recognizer functions to account for the endian-dependent numbers.
	(umul_600, smul_600, mac_600, mulsi_600, mulsidi3): Likewise.
	(umulsidi3): Likewise.
	(mulsi_600): Type multi.
	(*split_mulsi3_600): Delete.
	(mulsidi_600, umulsidi_600): New patterns.
	(mulsidi3, umulsidi3): Result is nonimmediate_operand.
	Add TARGET_MUL64_SET code.
	(mac64_600): Fix semantics description to be consistent with
	PARALLEL semantics.
	(umulsidi3_700): Operand 2 is register_operand.
	* config/arc/arc600.md (mul64_600): New cpu_unit.
	(load_DI_600, load_600): Fix attribute test.
	(mul_600_fast, mul_600_slow): New reservations.
	* config/arc/arc.h (TARGET_OPTFPE): True for TARGET_ARC600
	&& TARGET_NORM_SET && TARGET_MULMAC_32BY16_SET.

	* config/arc/arc.md (doloop_begin_i): Look inside SEQUENCEs.
	(doloop_end_i) Change alternative 2 of operand 2 to C_0.
	* config/arc/arc.h (HARD_REGNO_RENAME_OK): Delete.

2008-09-02  Muhammad Khurram Riaz <khurram.riaz@arc.com>

	* config/arc/arc.c (arc600_corereg_hazard): mul/mac and mul64 set the accum registers 
	and are not a hazard.
	* config/arc/arc.md :
	(*unary_comparison_result_used): added for flag setting instructions(unary) that also use result.
	(*addsi_compare): added for add.f ; needed by combiner.
	(*commutative_binary_comparison_result_used): added for flag setting instructions(binary,commutative) 
	that also use result.
	(*noncommutative_binary_comparison_result_used): added for flag setting instructions(binary,commutative) 
	that also use result.

2008-09-01  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/ieee-754/fixdfsi.S: Fix negative case.

2008-08-30  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c: Give up on zero overhead loop if loop setup
	precedes loop end with no label in-between.

2008-08-30  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_final_prescan_insn): Don't try to eliminate
	'insns' of TYPE_LOOP_END.
	* config/arc/arc.md (attr type): Replace loop with loop_start and
	loop_end.
	(attr in_delay_slot): Update.
	(movqi_insn, movhi_insn): Add alternatives to read ALL_CORE_REGS.
	(doloop_begin_i): Better estimate insn fetches with n_insns.
	Allow zero n_insns if there is a code_label.
	Don't emit nops for ARC600 with loop_top if n_insn is at least 3.
	Change type to loop_setup.
	(doloop_end_i): Set type to loop_end.

2008-08-30  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (doloop_begin_i): Start insn counting with label
	rather than label_ref.  Check that jump sucessor insns exists before
	calling recog_memoized on it.

2008-08-30  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (doloop_begin_i): Do separate calculation of
	minimum insns from loop setup to loop end.
	* config/arc/arc.c (arc_reorg): Fix setting of insn after deletion.

2008-08-29  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_reorg): Use the loop top label to check for
	empty loops.  When deleting an empty loop, put the set of the loop
	count before the loop end.

2008-08-29  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (HARD_REGNO_RENAME_OK): Prohibit renaming from
	LP_COUNT.
	* config/arc/arc.md (doloop_begin_i): Change alternative 2 of
	operand 1 to C_0.

2008-08-29  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/predicates.md (dest_reg_operand): Use ALL_CORE_REGS.
	config/arc/arc-protos.h (arc_init_regno_reg_class): Declare.
	* config/arc/arc.h: Fix merge error.
	(CONDITIONAL_REGISTER_USAGE): Call arc_init_regno_reg_class.
	config/arc/arc.c (arc_init_reg_tables): Break out arc_regno_reg_class
	initializing code into:
	(arc_init_regno_reg_class): New function.  Take new *CORE_REGS classes
	into account.
	* config/arc/arc.md (*umulsi3_highpart_int): Fix typo.

2008-08-29  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/crtg.asm: Use _init / _fini instead of __init / __fini.
	* config/arc/crti.asm: Likewise.

	* config/arc/arc.c (arc_register_move_cost): Express costs of
	writing / reading LP_COUNT register.
	* config/arc/arc.h (CONDITIONAL_REGISTER_USAGE): Prune CHEAP_CORE_REGS.
	(enum reg_class, REG_CLASS_NAMES, REG_CLASS_CONTENTS): Split CORE_REGS
	into CHEAP_CORE_REGS and ALL_CORE_REGS.
	(PREFERRED_RELOAD_CLASS): Check for CHEAP_CORE_REGS.
	(REGISTER_MOVE_COST): Use arc_register_move_cost.
	* config/arc/arc.md (movsi_insn): Add alternatives to read
	ALL_CORE_REGS.
	* config/arc/constraints.md ("c"): Now for CHEAP_CORE_REGS.
	("Rac"): New constraint.

	* loop-doloop.c (doloop_modify): Pass doloop_end pattern to
	gen_doloop_begin.
	* config/c4x/c4x.md (doloop_begin): Operand 4 is doloop_end pattern.
	* loop-doloop.c (doloop_optimize): Pass flag to indicate if loop is
	entered at top to gen_doloop_end.
	* config/arc/arc.c (arc_reorg): If we can't find the loop entry at
	the loop top, search entire function for a matching loop begin,
	and record information about it if found.
	* config/arc/arc.md (doloop_begin): Now takes five operands.
	(doloop_begin_i): Likewise.
	(doloop_end): Now takes 6 operands.

	* config/arc/arc.md (umulsi3_highpart_i): New pattern.
	(*umulsi3_highpart_int, umulsi3_highpart): Likewise.


	* target-def.h (TARGET_PRESERVE_RELOAD_P): Define.
	(TARGET_INITIALIZER): Add TARGET_PRESERVE_RELOAD_P.
	* target.h (struct gcc_target): New member preserve_reload_p.
	* reload.c (push_reload): Mark values flagged by
	targetm.preserve_reload_p as RELOAD_OTHER.
	* config/arc/arc.c (arc_preserve_reload_p): New function.
	(TARGET_PRESERVE_RELOAD_P): Redefine.
	* doc/tm.texi (TARGET_PRESERVE_RELOAD_P): Document.

	* config/arc/arc.h (ASM_OUTPUT_SYMBOL_REF): Define.
	(ASM_OUTPUT_LABEL_REF): Likewise.
	* config/arc/arc.c (arc_assemble_integer, arc_print_operand):
	Don't emit '@' before labels / symbols.
	(arc_print_operand_address): Likewise.

	* config/arc/arc.h (LINK_COMMAND_SPEC): When creating a shared
	library with -nostdlib, add -lgcc_s.

	* config/arc/fp-hack.c: Disable debug functions for ARC700.
	* config/arc/dp-hack.c: Likewise.

	* config/arc/arc.h (STATIC_LINK_SPEC): Make default to
	use dynamic libraries where available.
	* config.gcc (arc*-*-linux-uclibc*): Retain OS specific files in
	tmake_file.
	* config/arc/t-arc700-uClibc (SHLIB_MAPFILES): Fix pasto.
	(SHLIB_LINK: Override to use a linker script libgcc_s.so.
	(SHLIB_INSTALL): Likewise.
	* config/arc/lib1funcs.asm (__mulsi3): Use (HIDDEN_)FUNC / ENDFUNC.
	(__udivmodsi4, __umodsi3, __modsi3, __clzsi2): Likewise.
	(__umulsidi3): Likewise.  Add ARC700 optimized code.
	* config/arc/ieee-754/adddf3.S: Use FUNC / ENDFUNC.
	* config/arc/ieee-754/muldf3.S: Likewise.

2008-08-26  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/ieee-754/fixdfsi.S: Fix shift by zero case.

2008-08-08  Muhammad Khurram Riaz <khurram.riaz@arc.com>

	* config/arc/predicates.md (move_dest_operand): r56 r57 cannot be destination
	operand of move.
	* config/arc/arc.c (arc_init): error for -mmul32x16 used for !ARC600.
	* config/arc/arc.h (ASM_SPEC): mmul32x16 passed.
	* config/arc/arc.md (mulsi3):  added Arc600 dsp mul/mac also modified for mul64
	(mulsidi3): added Arc600 mul/mac dsp instructions
	* config/arc/arc600.md: scheduling info for new dsp mul/mac intructions.
	* config/arc/arc.opt: new option -mmul32x16
	* config/arc/t-arc-newlib: use -mmul32x16 for multilib

2008-08-08  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (doloop_begin_i): Count asm insns only as a
	single insn.

2008-07-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* dwarf2out.c (loc_descriptor): Pass entire MEM expressions
	through targetm.delegitimize_address.
	* config/arc/arc.c (arc_delegitimize_address): New function.
	(TARGET_DELEGITIMIZE_ADDRESS): Redefine.
	(arc_output_addr_const_extrao): Remove.
	* config/arc/arc.h (OUTPUT_ADDR_CONST_EXTRA): Remove.
	* config/arc/arc-protos.h (arc_output_addr_const_extra): Don't declare.

2008-07-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/ieee-754/eqdf2.S: Add some #ifdefed code for hardware
	floating point compatibility.

2008-07-23  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_output_libcall): Emit special sequence for
	-fpic -mlong-calls.
	* config/arc/arc.h (INSN_SETS_ARE_DELAYED): Also return true for
	TYPE_SFUNC.
	* config/arc/t-arc700-uClibc (SHLIB_MAPFILES): Define.
	* config/arc/arc.md (attribute type): Add sfunc.
	(attribute in_delay_slot): False for sfunc.
	(attribute in_sfunc_delay_slot): New.
	(toplevel): Add define_delay for sfunc.
	(cmpsf_eq, cmpdf_eq): Type sfunc for -fpic -mlong-call.
	(cmpsf_gt, cmpdf_gt, cmpsf_ge, cmpdf_ge: Likewise.
	(cmpsf_uneq, cmpdf_uneq, cmpsf_ord, cmpdf_ord): Likewise.
	* config/arc/lib1funcs.asm (HIDDEN_FUNC): Define.
	* config/arc/ieee-754/eqsf2.S, onfig/arc/ieee-754/orddf2.S: Use it.
	* config/arc/ieee-754/uneqsf2.S, config/arc/ieee-754/gtsf2.S: Likewise.
	* config/arc/ieee-754/gedf2.S, config/arc/ieee-754/ordsf2.S: Likewise.
	* config/arc/ieee-754/eqdf2.S, config/arc/ieee-754/uneqdf2.S: Likewise.
	* config/arc/ieee-754/gesf2.S, config/arc/ieee-754/gtdf2.S: Likewise.
	* config/arc/libgcc-excl.ver: New file.

	* config/arc/arc.c (arc_legitimize_pic_address): Use gen_const_mem.
	(arc_output_addr_const_extra): New function.
	* config/arc/arc-protos.h (arc_output_addr_const_extra): Declare.
	* config/arc/arc.h (LEGITIMIZE_ADDRESS): Don't pass OLDX.
	(OUTPUT_ADDR_CONST_EXTRA): Define.

2008-07-23  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (xorsi3): Use "w" constraint for output operand.
	(call_value): Don't write past end of operands.

	* gcc/config/arc/arc.c (arc_legitimate_pc_offset_p): New function.
	(arc_legitimize_pic_address): Don't make addition of pcl explicit.
	(arc_output_pic_addr_const): Also emit '+' for negative leading integer.
	Emit leading "pcl," for ARC_UNSPEC_GOT.
	* config/arc/arc-protos.h (arc_legitimate_pc_offset_p): Declare.
	* config/arc/constraints.md ("Cpc", "Cal"): New constraints.
	* config/arc/arc.md (entire file): Replace ARCompact "i", "J", "Ji" and
	"iJ" constraints with "Cal".
	(movsi_insn): Add missing 'S' output modifier.
	Add "?w"/"Cpc" alternative.
	(mulsi3_600, mulsi3_700, andsi3): Add missing 'S' output modifier.
	(xorsi3, indirect_jump, normw, mul64, divaw, flag, sr): Likewise.

2008-07-17  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/mxp/mxp.md (lshr<mode>3_imm): Rename to:
	(vec_shr_<mode>).
	(vec_unpacks_lo_v8hi, vec_unpacku_lo_v8hi): New patterns.
	(vec_unpacks_hi_v8hi, vec_unpacku_hi_v8hi): Likewise.
	(vec_pack_trunc_v4si): Likewise.

2008-07-17  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/mxp/mxp.md (or<mode>3): Rename to:
	(ior<mode>3).

2008-07-17  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/sh/sh.c (expand_block_move): Update call to
	can_move_by_pieces.

	* config/mxp/mxp.h (FUNCTION_VALUE_REGNO_P): Use FIRST_PARM_REG.

2008-07-16  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_final_prescan_insn): Fix bug in last change:
	check for JUMP_INSN.

2008-07-15  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_final_prescan_insn): Don't emit a nop
	in front of a delay slot insn.
	(arc_reorg): Use fall-back pattern for non-empty zero length loops.
	(arc600_corereg_hazard_1): Fix extension register range.
	(write_ext_corereg_1): Likewise.

	* config/arc/arc.c (disi_highpart): Return result.

2008-07-15  J"orn Rennecke  <joern.rennecke@arc.com>

	* rtl-factoring.c (compute_dummy_rtx_cost): New function.
	(compute_init_costs): Use it.

	* config/arc/arc.h (IS_ASM_LOGICAL_LINE_SEPARATOR): Add STR parameter.

2008-07-14  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/ieee-754/divdf3.S (.Lret0_NaN): Remove dead insn.
	(.Linf_nan_dbl1): Reduce size (and cycle count).
	Adjust affected offsets.
	* config/arc/ieee-754/mulsf3.S (.Linf_nan_dbl0): Shorten NaN check.
	Adjust affected offsets.
	* lib1funcs.asm (__mulsi3): Remove __base conditional.

	* config/arc/ieee-754/divdf3.S (.Linf_denorm): Fix handling of
	0x7fe / 0x7ff exponents.
	* config/arc/ieee-754/mulsf3.S (.Ldenorm_dbl1): Avoid clobbering r4.

	* config/arc/arc.md (umulqihi3, mulqihi3): Delete.

	* config/arc/t-arc-newlib (MULTILIB_OPTIONS): Add mmul64 and mnorm.
	(MULTILIB_DIRNAMES): Add mul64 and norm.
	(MULTILIB_EXCEPTIONS): Exclude combinations of mARC700 with mmul64
	and mnorm.
	(MULTILIB_EXCLUSIONS): Don't build multilibs with only mmul64 or
	only mnorm, but just one with both combined.
	* config/arc/predicates.md (dest_reg_operand): New predicate.
	(move_dest_operand): Use it.
	* config/arc/arc.h (TARGET_CPU_CPP_BUILTINS): Define __ARC_NORM__ and
	__ARC_MUL64__ when the eponymous instructions are available.
	(TARGET_OPTFPE): Define.
	* config/arc/arc.md (loadqi_update): Use dest_reg_operand.
	(load_zeroextendqisi_update, load_signextendqisi_update): Likewise.
	(load_zeroextendhisi_update, movsicc, movdicc, movsfcc): Likewise.
	(movdfcc, movsicc_insn, movdicc_insn, movsfcc_insn): Likewise.
	(movdfcc_insn, zero_extendqihi2_a4, zero_extendqihi2_i): Likewise.
	(zero_extendqihi2, zero_extendqisi2_a4, zero_extendqisi2_ac): Likewise.
	(zero_extendqisi2, zero_extendhisi2_a4, zero_extendhisi2_i): Likewise.
	(zero_extendhisi2, zero_extendhisi2, extendqihi2_a4): Likewise.
	(extendqihi2_i, extendqihi2, extendqisi2_a4, extendqisi2_ac): Likewise.
	(extendqisi2): Likewise.
	(extendhisi2_a4, extendhisi2_i, extendhisi2, abssi2): Likewise.
	(smaxsi3, addsi3_mixed, addsi3_mixed, mulsi3i, mulsi3_700): Likewise.
	(umulsidi3, mulsidi3, addsi3, addsi3_insn_a4, subsi3): Likewise.
	(subsi3_insn, add_n, sub_n, shift_and_add_insn): Likewise.
	(shift_and_sub_insn, bset_insn, bxor_insn, bclr_insn): Likewise.
	(bmsk_insn, andsi3_insn_a4, bicsi3_insn, iorsi3_a4, xorsi3): Likewise.
	(negsi2_a4, negsi2i, one_cmplsi2_a4, one_cmplsi2): Likewise.
	(one_cmpldi2_a4, ashlsi3, ashrsi3, lshrsi3, shift_si3): Likewise.
	(rotrsi3, seq, sne, sgt, sle, sge, slt, sgtu, sleu, sgeu): Likewise.
	(sltu, scc_insn, neg_scc_insn, not_scc_insn, movsi_ne): Likewise.
	(movsi_cond_exec, add_cond_exec, commutative_cond_exec): Likewise.
	(sub_cond_exec, noncommutative_cond_exec, call_prof): Likewise.
	(call_value_via_reg_mixed, call_value_via_label): Likewise.
	(call_value_via_imm, call_value_prof, flush_icache+1): Likewise.
	(flush_icache+2, flush_icache+3, flush_icache+4): Likewise.
	(flush_icache+5, flush_icache+6, flush_icache+7): Likewise.
	(flush_icache+8, flush_icache+9, flush_icache+10): Likewise.
	(flush_icache+11, flush_icache+12, norm, normw, swap): Likewise.
	(core_read, lr, sibcall_value, sibcall_value_insn): Likewise.
	(sibcall_value_prof, abssf2, negsf2): Likewise.
	(storeqi_update): Likewise.  Use "w" / "c" constraints.
	(loadhi_update, load_signextendhisi_update, storehi_update): Likewise.
	(loadsi_update, storesi_update, loadsf_update): Likewise.
	(storesf_update, sminsi3, mulsi3_600, adddi3, subdi3): Likewise.
	(anddi3, iordi3, xordi3, negdi2, one_cmpldi2): Likewise.
	(ashlsi3_insn_mixed, ashrsi3_insn_mixed, lshrsi3_insn_mixed): Likewise.
	(divaw): Likewise.
	(andsi3, iorsi3): Likewise.  Mark commutative.
	(cmpsf, cmpdf, cmpsf_eq, cmpdf_eq): Make conditional on TARGET_OPTFPE.
	(cmpsf_gt, cmpdf_gt, cmpsf_ge, cmpdf_ge, cmpsf_uneq): Likewise.
	(cmpdf_uneq, cmpsf_ord, cmpdf_ord): Likewise.
	* config/arc/dp-hack.h: Make optimized floating-point support
	dependent on __ARC_NORM__ instead of __ARC700__.
	* config/arc/fp-hack.h: Likewise.
	* longlong.h (count_leading_zeros): Make dependent on __ARC_NORM__
	instead of __ARC700__.
	* lib1funcs.asm (__clzsi2, L_adddf3, L_addsf3): Likewise.
	(L_extendsfdf2, L_truncdfsf2, L_floatsidf,  L_floatsisf): Likewise.
	(L_floatunsidf, L_fixdfsi, L_fixsfsi, L_fixunsdfsi, L_eqdf2): Likewise.
	(L_eqsf2, L_gtdf2, L_gtsf2, L_gedf2, L_gesf2, L_uneqdf2): Likewise.
	(L_uneqsf2, L_orddf2, L_ordsf2): Likewise.
	(L_muldf3): Provide variant for __ARC_NORM__ && __ARC_MUL64__.
	(L_mulsf3, L_divdf3, L_divsf3): Likewise.
	(__mulsi3): Provide new variants for __ARC_MUL64__,
	__ARC_NORM__ and !__OPTIMIZE_SIZE__.
	(__udivmodsi4): Provide new variant for __ARC_NORM__.  Fix variant for
	plain ARC600 to conform to lp instruction description of
	document 5115-31.
	* config/arc/ieee-754/arc600/divdf3.S: New file.
	* config/arc/ieee-754/arc600/divsf3-stdmul.S: Likewise.
	* config/arc/ieee-754/arc600/mulsf3.S: Likewise.
	* config/arc/ieee-754/arc600/muldf3.S: Likewise.
	* config/arc/arc.c (arc_select_cc_mode): Use TARGET_OPTFPE.
	(gen_compare_reg): Likewise.

	* config/arc/predicates.md (symbolic_memory_operand): Remove predicate.
	* config/arc/arc-protos.h (symbolic_memory_operand): Don't declare.

2008-07-07  J"orn Rennecke  <joern.rennecke@arc.com>

	* config.gcc (tm_defines): Set TARGET_CPU_DEFAULT_OPT to mA6.
	* t-arc-newlib (MULTILIB_OPTIONS): Replace mA5 with mA6.
	(MULTILIB_MATCHES): Likewise.
	* config/arc/lib1funcs.asm (__divnorm): Only provide for A4.
	Changed all callers.
	(__udivmodsi4): Add optimized ARC600 version.

2008-07-02  J"orn Rennecke  <joern.rennecke@arc.com>

	* final.c (asm_insn_count): Make empty asm templates have zero length.
	* config/arc/arc.c (arc_final_prescan_insn): Avoid calling arc_hazard
	on epilogue delay slots.
	(arc_reorg): When zero overhead loop is empty, replace it with a set
	of lp_count to zero.
	(arc600_corereg_hazard): Look into SEQUENCEs.
	(arc_hazard): Don't look into SEQUENCEs.
	* config/arc/arc.h (IS_ASM_LOGICAL_LINE_SEPARATOR): Define.
	* config/arc/arc.md (attr "in_delay_slot"): use prev/next_active_insn.
	(doloop_begin_i): Assert minimum number of insns, not size.

2008-07-01  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc-protos.h (arc_insn_length_adjustment): Declare.
	(arc_corereg_hazard, arc_hazard, arc_write_ext_corereg): Likewise.
	* config/arc/arc.c (arc_final_prescan_insn): Output a nop where
	necesary to avoid a hazards on ARC600.
	(arc600_corereg_hazard_1, arc600_corereg_hazard): New functions.
	(arc_hazard, arc_insn_length_adjustment): Likewise.
	(write_ext_corereg_1, arc_write_ext_corereg): Likewise.
	* config/arc/arc.md (in_delay_slot, doloop_begin_i, doloop_end_i):
	Handle ARC600 idiosyncrasies concerning zero overhead loops.
	(doloop_end): Enable for ARC600.

	* config/arc/arc.c (arc_secondary_reload): Also require reload
	for WRITABLE_CORE_REGS, but restrict LPCOUNT_REG / WRITABLE_CORE_REGS
	secondary reloads to reloads from memory.

	* config/arc/arc.c (arc_final_prescan_insn): Put TARGET_DUMPISIZE
	code after declarations.

	* config/arc/arc-protos.h (arc_output_mi_thunk): Unless long call
	semantics apply, use branch.

2008-07-15  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/mxp/mxp.md (mov<mode>_clob): Now define_insn_and_split.
	Remove stray comma.
	* config/mxp/mxp.c (TARGET_PROMOTE_FUNCTION_RETURN): Redefine.
	* config/mxp/mxp.h (PREFERRED_STACK_BOUNDARY): Define.
	(PREFERRED_RELOAD_CLASS): Change SffVff_REGS to Sff_REGS.
	(STATIC_CHAIN): Define.
	(HARD_REGNO_RENAME_OK): Define.

2008-06-26  J"orn Rennecke  <joern.rennecke@arc.com>

	* config.gcc (mxp-arc-elf): Change to:
	(mxp-*-elf).
	* regclass.c (struct reg_pref): Use short rather than char for members.
	* config/mxp/constraints.md: Rename constraints to satisfy new rule
	on smale length for same leading character.
	* config/mxp/mxp.md: Update to reflect constraint renaming.
	Reflect rename of respectively define_code_macro and
	define_mode_macro to define_code_iterator and define_mode_iterator.
	* UNITS_PER_SIMD_WORD: Add MODE parameter.

2008-06-26  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_expand_builtin) Use new CALL_EXPR accessors.
	(arc_expand_simd_builtin): Likewise.

	* config/arc/arc.c (arc_reorg): Don't try to create cbranchsi4_scratch
	if a REG_CROSSING_JUMP is seen.
	* config/arc/arc.md (jump): Rename to..
	(jump_i).  Reject for TARGET_LONG_CALLS_SET if REG_CROSSING_JUMP
	note is found.
	(jump): New define_expand.
	(cbranchsi4_scratch): Re-indent; Clean up length calculation.

2008-06-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (subsi3_insn): Alternative 4 is "nocond".

	* config/arc/arc.md (cbranchsi4_scratch): Show clobber of CC register
	in pattern.

	* config/arc/arc.h (LINK_SPEC): s/shared/shared:-shared/ .

	* config/arc/arc.h (IS_POWEROF2_P): Check for zero.
	* config/arc/arc.md (andsi3, iorsi3, xorsi3):
	Use 'w' instead of 'c' for output constraint.  Drop J in Ji.
	Use arc_size_opt_level.  Fix formatting.

	* config/arc/arc.md (anddi3): Fix result constraints to use 'w'.
	(iordi3, xordi3, negdi2, divaw): Likewise.

	* config/arc/arc-protos.h (disi_highpart): Declare.
	* config/arc/arc.h (disi_highpart): New function.
	* config/arc/arc.md (addf, adc, subf, sbc): New patterns.
	(anddi3, iordi3, xordi3): Make conditional on TARGET_OLD_DI_PATTERNS.
	(negdi3): Likewise.
	(*movsi_set_cc_insn, unary_comparison): Use cc_set_register.
	(*commutative_binary_comparison): Likewise.
	(*noncommutative_binary_comparison): Likewise.
	(adddi3, subdi3): Now define_expand.
	(*not_scc_insn): Use cc_use_register.
	* config/arc/arc.opt (mold-di-patterns): New options.
	* config/arc/predicates.md (cc_set_register): New predicates.
	(cc_use_register): Likewise.

	* config/arc/arc.md (subsi3): Allow constant operand 1.
	(subsi3_insn): Likewise.

2008-06-12  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (RTX_OK_FOR_OFFSET_P): Fix typo.

2008-06-09  J"orn Rennecke  <joern.rennecke@arc.com>

	* genattr.c (gen_attr): Include vec.h and statistics.h.
	* Makefile.in (INSN_ATTR_H): Add vecprim.h, vec.h and statistics.h.

	* expr.c (can_move_by_pieces): Fix logic.

	* rtlanal.c (walk_stores): New function, uncostified copy
	of note_stores.
	* rtl.h (walk_stores): Declare.

2008-06-09  J"orn Rennecke  <joern.rennecke@arc.com>

	* doc/tm.texi (CAN_MOVE_BY_PIECES): Document.
	* expr.c (emit_block_move_via_movmem): No longer static.
	(can_move_by_pieces): New argument consider_movmem.
	Changed all callers.
	(expand_constructor): Use can_move_by_pieces.
	* expr.h (emit_block_move_via_movmem): Declare.
	(can_move_by_pieces): Adjust declaration.
	* builtins.c (expand_builtin_mempcpy): Use target.
	Use emit_block_move_via_movmem.
	* config/arc/arc-protos.h (arc_expand_movmem): Declare.
	* config/arc/arc.c (force_offsettable): New function.
	(arc_expand_movmem): Likewise.
	* config/arc/arc.h (MOVE_MAX_PIECES): Don't define.
	(MOVE_BY_PIECES_P, CAN_MOVE_BY_PIECES, MOVE_RATIO): Define.
	* config/arc/arc.md (movmemsi): New pattern.

2008-06-06  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_init_reg_tables): Set proper
	arc_regno_reg_class for core regs / writable core regs.
	* config/arc/arc.h (CONDITIONAL_REGISTER_USAGE): If any of the
	registers in the range 32..59 has been made non-fixed, include
	them in WRITABLE_CORE_REGS.
	(enum reg_class, REG_CLASS_NAMES): Add WRITABLE_CORE_REGS.
	(REG_CLASS_CONTENTS): Likewise.
	(PREFERRED_RELOAD_CLASS): Check for WRITABLE_CORE_REGS.
	* config/arc/arc.md (*movqi_insn): Use 'w' constraint for
	writable core regs.
	(*movhi_insn, movsi, *movsi_set_cc_insn, *movdi_insn): Likewise.
	(*movsf_insn, *movdf_insn, *movsicc_insn, *movdicc_insn): Likewise.
	(*movdicc_insn, *movsfcc_insn, *movdfcc_insn): Likewise.
	(*zero_extendqihi2_i, *zero_extendqisi2_ac): Likewise.
	(*zero_extendhisi2_i, *extendqisi2_ac, *extendhisi2_i): Likewise.
	(abssi2, smaxsi3, sminsi3, subsi3_insn, *add_n, *sub_n): Likewise.
	(*shift_and_add_insn_mixed, *shift_and_add_insn): Likewise.
	(*shift_and_sub_insn, *bset_insn_mixed, *bset_insn): Likewise.
	(*bxor_insn, *bclr_insn, *bmsk_insn, *bicsi3_insn, iorsi3): Likewise.
	(negsi2, one_cmplsi2, rotrsi3, *scc_insn, *neg_scc_insn): Likewise.
	(*not_scc_insn, *movsi_ne, *movsi_cond_exec, *add_cond_exec): Likewise.
	(*commutative_cond_exec, *sub_cond_exec): Likewise.
	(*noncommutative_cond_exec, flush_icache+[23479], norm): Likewise.
	(normw, swap, *doloop_fallback, abssf2, negsf2, negsf2): Likewise.
	(mulsi3): Rename to:
	(*mulsi3_700).
	(mulsi3_600): New pattern.
	(mulsi3): New define_expand.
	* config/arc/arc.opt (mdynamic): New option.
	* config/arc/constraints.md ("w"): New constraint.

	* config/arc/arc.h (TARGET_CPU_CPP_BUILTINS): Fix ARC600 / ARC700 definitions.

2008-05-22  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (flush_icache+5): Fix tests.

2008-05-19  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h [UCLIBC_DEFAULT] (STATIC_LINK_SPEC): Define;
	default to static linking.
	(LINK_SPEC) [UCLIBC_DEFAULT]: Use STATIC_LINK_SPEC.

2008-05-19  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (LINK_SPEC) [UCLIBC_DEFAULT]: Pass '-shared'
	through.

2008-05-09  J"orn Rennecke  <joern.rennecke@arc.com>

	* config.gcc (arc-*-elf*): Add TARGET_CPU_DEFAULT_OPT to tm-defines.
	(arc600-*-linux-uclibc): Likewise; remove USE_UCLIBC.
	(arc*-*-linux-uclibc): Likewise.
	(with-cpu): Recognize arc600.
	* config/arc/arc.c: Use UCLIBC_DEFAULT instead of USE_UCLIBC.
	* config/arc/arc.h (UCLIBC_DEFAULT): Define if not already defined.
	Use instead of USE_UCLIBC.
	(CPP_SPEC): Move everything from here...
	(TARGET_CPU_CPP_BUILTINS): ... to here.
	(CPP_SPEC): Unify.
	(DRIVER_SELF_SPECS: Always define.  Use TARGET_CPU_DEFAULT_OPT.
	(MULTILIB_DEFAULTS): Use TARGET_CPU_DEFAULT_OPT.

2008-04-27  J"orn Rennecke  <joern.rennecke@arc.com>

	* mode-classes.def (MODE_VECTOR_CC): New mode class.
	* genmodes.c (vector_class): Support MODE_VECTOR_CC.
	(complete_mode): Allow bytesize to have been set for MODE_CC.
	Support MODE_VECTOR_CC.
	(SIZED_CC_MODE): New macro.
	(make_special_mode ): Return mode_data struct.
	*config.gcc (mxp-arc-elf*): Set extra_objs.

2008-04-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* config.gcc (mxp-arc-elf*): New configuration.
	* doc/mxp.texi: New file.
	* config/mxp: New directory.  FIXME: details.
	* mode-classes.def (MODE_CLASSES): Add MODE_VECTOR_PARTIAL_INT.
	* cse.c (cse_insn): Fix loop to stop at VOIDmode.
	* genmodes.c (vector_class): Support MODE_VECTOR_PARTIAL_INT.
	(complete_mode, emit_mode_adjustments): Likewise.
	* target-def.h (TARGET_PRESERVE_RELOAD): Fix spelling.

2008-04-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (LINK_SPEC) [USE_UCLIBC]: Add options
	-z max-page-size=0x1000 -z common-page-size=0x1000 .

2008-04-17  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_profile_call): Pass pointer as argument to
	_mcount_call.

2008-04-16  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (CONDITIONAL_REGISTER_USAGE): Adjust SIBCALL_REGS
	according to call_used_regs.
	* config/arc/gmon/mcount.c (_MCOUNT_DECL): Remove stray
	catomic_compare_and_exchange_bool_acq call.
	* testsuite/gcc.dg/func-ptr-prof.c: New file.

2008-04-09  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (LINK_SPEC) [USE_UCLIBC]: When profiling, use
	arclinux_prof emulation; else, use arclinux emulation.
	(LINK_SPEC) [!USE_UCLIBC]: When profiling, use arcelf_prof emulation.

2008-04-08  J"orn Rennecke  <joern.rennecke@arc.com>

	* config.gcc (arc600-*-linux-uclibc*): New configuration.
	* config/arc/t-arc600-uClibc: New file.

2008-04-05  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (gen_compare_reg): Swap operands for fpx
	CC_FP_GEmode comparisons.
	* config/arc/fpx.md (cmpsfpx_gt): Rename to:
	(cmpfpx_gt).  Also clear n flag if z flag is set.
	(cmpsfpx_ge): Rename to:
	(cmpfpx_ge).  Assume swapped comparison operands.  Don't
	setc flag if z flag is set.

2008-04-04  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/dp-hack.h (df_to_usi): Write as
	(tf_to_usi).

2008-04-04  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/fp-bit.c (_fpdiv_parts): Avoid double rounding.
	* config/arc/arc-protos.h (prepare_extend_operands): Move into part
	guarded by RTX_CODE.
	(arc_output_addsi, arc_expand_movmem, prepare_move_operands): Likewise.
	* config/arc/predicates.md (proper_comparison_operator):
	Fix CC_Cmode case.
	* config/arc/arc.c (branch_dest): Check for reversed branch.
	* config/arc/arc.h (LEGITIMATE_SCALED_ADDRESS_P): Add missing
	parentheses.
	* config/arc/arc.md (*movsi_scaled_insn): Remove pattern.
	(movdi_insn): Add %? output specifier.
	(movdf_insn): Likewise. Add cond attribute.

	* config/arc/arc-protos.h (gen_compare_reg): Update prototype.
	(arc_output_libcall): Declare.
	* config/arc/predicates.md (proper_comparison_operator):
	Add cases for floating point CCmodes, SFmode and DFmode.
	* config/arc/ieee-754/floatunsidf.S: New file.
	* config/arc/ieee-754/divdf3.S: Likewise.
	* config/arc/ieee-754/orddf2.S: Likewise.
	* config/arc/ieee-754/eqsf2.S: Likewise.
	* config/arc/ieee-754/truncdfsf2.S: Likewise.
	* config/arc/ieee-754/fixunsdfsi.S: Likewise.
	* config/arc/ieee-754/divtab-arc-df.c: Likewise.
	* config/arc/ieee-754/uneqsf2.S: Likewise.
	* config/arc/ieee-754/adddf3.S: Likewise.
	* config/arc/ieee-754/gtsf2.S: Likewise.
	* config/arc/ieee-754/gedf2.S: Likewise.
	* config/arc/ieee-754/floatsisf.S: Likewise.
	* config/arc/ieee-754/muldf3.S: Likewise.
	* config/arc/ieee-754/fixdfsi.S: Likewise.
	* config/arc/ieee-754/divsf3.S: Likewise.
	* config/arc/ieee-754/ordsf2.S: Likewise.
	* config/arc/ieee-754/eqdf2.S: Likewise.
	* config/arc/ieee-754/divtab-arc-sf.c: Likewise.
	* config/arc/ieee-754/divsf3-stdmul.S: Likewise.
	* config/arc/ieee-754/addsf3.S: Likewise.
	* config/arc/ieee-754/uneqdf2.S: Likewise.
	* config/arc/ieee-754/gesf2.S: Likewise.
	* config/arc/ieee-754/gtdf2.S: Likewise.
	* config/arc/ieee-754/mulsf3.S: Likewise.
	* config/arc/ieee-754/floatsidf.S: Likewise.
	* config/arc/ieee-754/fixsfsi.S: Likewise.
	* config/arc/ieee-754/arc-ieee-754.h: Likewise.
	* config/arc/ieee-754/extendsfdf2.S: Likewise.
	* config/arc/arc.c (get_arc_condition_code): Add floating point cases.
	(arc_select_cc_mode, arc_init_reg_tables): Likewise.
	(gen_compare_reg): Likewise.  Don't take comparison operands, but
	but result mode as parameter.  Return appropriate comparison of
	flags register against 0.  Changed all callers.
	(arc_output_libcall): New function.
	* config/arc/arc.h (INSN_SETS_ARE_DELAYED): Define.
	(INSN_REFERENCES_ARE_DELAYED, REVERSE_CONDITION): Likewise.
	* config/arc/fpx.md (cmpsfpx_raw, cmpdfpx_raw): New patterns.
	(cmpsfpx_gt, cmpsfpx_ge): Likewise.
	* config/arc/dp-hack.h: New file.
	* config/arc/fp-hack.h: New file.
	* config/arc/arc.md (movsicc_insn): Use proper_comparison_operator.
	(movdicc_insn, movsfcc_insn, movdfcc_insn, neg_scc_insn): Likewise.
	(not_scc_insn, movsi_cond_exec, add_cond_exec): Likewise.
	(commutative_cond_exec, sub_cond_exec): Likewise.
	(noncommutative_cond_exec): Likewise.
	(scc_insn): Use REVERSE_CONDITION.
	(bunge, bungt, bunle, bunlt, buneq, bltgt, bordered): New patterns.
	(bunordered, cmpsf, cmpdf, cmp_float, cmpsf_eq, cmpdf_eq): Likewise.
	(cmpsf_gt, cmpdf_gt, cmpsf_ge, cmpdf_ge, cmpsf_uneq: Likewise.
	(cmpdf_uneq, cmpsf_ord, cmpdf_ord, abssf2, negsf2): Likewise.
	* config/arc/lib1funcs.asm (FUNC, ENDFUNC0, ENDFUNC): New macros.
	(__udivsi3, __divsi3): Use FUNC / ENDFUNC.
	Toplevel: Add code to include files from ieee-754 subdirectory.
	* config/arc/t-arc (LIB1ASMFUNCS): Add floating-point functions.
	(dp-bit.c): cat config/arc/dp-hack.h.
	(fp-bit.c): cat config/arc/fp-hack.h.
	* config/arc/arc-modes.def (CC_FP_GT, CC_FP_GE, CC_FP_ORD): New modes.
	(CC_FP_UNEQ, CC_FPX): Likewise.

2008-03-27  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/t-arc700-uClibc (TARGET_LIBGCC2_CFLAGS): Add -fPIC.

2008-03-27  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (lr, sr): Enable r,r alternative.

2008-03-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (DRIVER_SELF_SPECS): Change condition to !mA*.

2008-03-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (DRIVER_SELF_SPECS): Change condition to !-mA.

2008-03-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (movqi_insn_mixed, loadqi_insn_mixed): Remove.
	(storeqi_insn_mixed, movhi_insn_mixed, loadhi_insn_mixed): Likewise.
	(storehi_insn_mixed, movsi_insn_mixed, loadsi_insn_mixed): Likewise.
	(storesi_insn_mixed): Likewise.
	(movhi_insn): Rewrite after model of movsi pattern.

2008-03-19  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_init): Fix default setting of processor
	type in target_flags.

2008-03-19  J"orn Rennecke  <joern.rennecke@arc.com>

	* reload.c (push_secondary_reload): Add missing break.
	* config/arc/arc.h (LEGITIMIZE_RELOAD_ADDRESS): Define.

2008-03-18  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (commutative_binary_comparison): Fix position
	of '%'.

2008-03-17  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (doloop_begin_i): Fix offset for loop end.

2008-03-06  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_encode_section_info): Use
	targetm.binds_local_p instead of TREE_PUBLIC.

2008-03-05  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/lib1funcs.asm (clzsi2) [!__ARC700__,!__A4__]:
	Re-implement to avoid undefined behaviour and increase performance.

	* config/arc/arc.h (CONDITIONAL_REGISTER_USAGE) <TARGET_ARC700>:
	Set fixed_regs[LP_COUNT].

2008-03-05  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (OVERRIDE_OPTIONS): Disable small data for -fPIC.
	(LEGITIMATE_SCALED_ADDRESS_P): For -fPIC, the only allowed constant
	bases are CONST_INT.

2008-02-27  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/constraints.md ("Cca", C2a): Check absolute value against
	maximum scaled constant.

2008-02-26  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/crti.asm, config/arc/crtn.asm: Fix stack layout.

2008-02-08  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/predicates.md (long_immediate_loadstore_operand):
	Also recognize that non-sdata SYMBOL_REFS require long immediates.

2008-02-07  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_print_operand_address): Always add @sda
	for small data symbols.  Use recursion.
	(prepare_move_operands): Also do small data address checks for
	floating point point modes.
	(prepare_extend_operands): New function.
	* config/arc/arc-protos.h (prepare_extend_operands): Declare.
	* config/arc/arc.md (zero_extendqihi2, zero_extendhisi2): Rename to:
	(*zero_extendqihi2_i, *zero_extendhisi2_i).
	(zero_extendqihi2, zero_extendhisi2): New expanders.
	(zero_extendqisi2, extendqisi2): Use prepare_extend_operands.
	(extendqihi2, extendhisi2): Rename to:
	(*extendqihi2_i, *extendhisi2_i).
	(extendqihi2i, extendhisi2): New expanders.

2008-02-07  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (doloop_begin_i): If loop is too large for lp
	instruction, emit code sequence using sr to set LOOP_START / LOOP_END
	appropriately.

2008-01-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (RTX_OK_FOR_OFFSET_P): Before applying scaling,
	check that offset is aligned.

2008-01-19  J"orn Rennecke  <joern.rennecke@arc.com>

	* resource.c (find_dead_or_set_registers): Don't consider sets inside
	a COND_EXEC to kill the value in a register.

2008-01-10  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/constraints.md ("Usc"): Reject small data.

	* config/arc/arc.md (movsi_insn): Fix predicate test for valid constant
	store address.

2008-01-08  J"orn Rennecke  <joern.rennecke@arc.com>

	* version.c (VERSUFFIX): Bump date.

2007-12-06  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc-protos.h (arc_asm_output_aligned_decl_common):
	Change to:
	(arc_asm_output_aligned_decl_local).
	* config/arc/arc.c: Likewise.
	* config/arc/arc.h (OPTIMIZATION_OPTIONS): Initialize flag_no_common
	as -1.
	(OVERRIDE_OPTIONS): If flag_no_common is still -1,  initialize according
	to TARGET_NO_SDATA_SET.
	(ASM_OUTPUT_ALIGNED_DECL_COMMON): Don't define.
	(ASM_OUTPUT_ALIGNED_DECL_LOCAL): Use arc_asm_output_aligned_decl_local.

2007-11-29  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (unspec_prof_hash): Only hash s1 string for
	SYMBOL_REF.
	(unspec_prof_htab_eq): Use rtx_equal_p.
	* config/arc/arc.md (call): Use emit_call_insn for profiling call.
	(call_value): Likewise.

2007-11-26  J"orn Rennecke  <joern.rennecke@arc.com>

	target/27758:
	* config/arc/arc.c (arc_output_pic_addr_const) <case SYMBOL_REF>:
	Use output_addr_const instead of assemble_name.

2007-11-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (movsi_insn): Add 'S' modifier for operand 1 of
	alternative 10.

2007-11-01  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc-protos.h (prepare_move_operands): Declare.
	* config/arc/predicates.md (move_dest_operand): Reject scaled index.
	(shouldbe_register_operand): New predicate.
	* config/arc/arc.c (arc_address_cost): Scaled index and plain index
	have low cost.
	(arc_print_operand): Print ".as" for scaled index.
	(arc_print_operand_address): Decompose scaled index.
	base can be CONST_INT.
	(arc_rewrite_small_data_1): If expression would get too complex,
	force small data address into register.
	(arc_rewrite_small_data): Pass &op as data.
	(prepare_move_operands): New function.
	* config/arc/arc.h (RTX_OK_FOR_INDEX_P): Enable.
	(LEGITIMATE_OFFSET_ADDRESS_P): Add INDEX parameter.  Changed all users.
	Require MODE size to be no larger than 4.
	(LEGITIMATE_SCALED_ADDRESS_P): Define.
	(GO_IF_LEGITIMATE_ADDRESS): Use LEGITIMATE_SCALED_ADDRESS_P.
	(GO_IF_MODE_DEPENDENT_ADDRESS): Only say REG-REG PRE/POST_MODIFY
	is mode dependent.
	Indexed addresses are mode dependent.
	* config/arc/arc.md (movqi, movhi, movsi, movdf): Use
	prepare_move_operands and prepare_move_operands.
	(movdi): Use move_dest_operand.
	(doloop_begin_i): Don't align if no lp instruction is used.
	(doloop_end_i): Now define_insn_and_split.
	* arc.opt (mindexed-load, mauto-modify-reg): New options.

2007-10-30  J"orn Rennecke  <joern.rennecke@arc.com>

	* Makefile.in (options.c): Depend on $(INSN_ATTR_H).  Include it.
	* config/arc/arc.h: Don't include insn-attr.h.

2007-10-26  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (doloop_fallback_m): Avoid unaligning code.

2007-10-26  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (doloop_begin_i): Add ????X alternative.
	(doloop_end_i): Add m alternative.
	(doloop_fallback_m): New pattern.
	* config/arc/arc.c (arc_reorg): Fix up doloop_end_i instructions
	that ended up with memory.  When a doloop_begin_i doesn't have the
	right register, try to find it in a preceding move.

2007-10-26  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_init): Provide default for arc_tune and
	arc_multcost.
	(arc_rtx_costs): Use arc_multcost.
	* arc.md: Move scheduling description to...
	* config/arc/arc700.md: ... here.  New file.
	* config/arc/arc600.md: New file.  Tie ARC700 scheduling description
	to tune_arc700 attribute.
	(mov_set_cc_insn, compare_insn): Combine to:
	(compare_700).
	(branch_700): New reservation.  Create new bypass.
	(multi_SI): Replace with:
	(multi_xmac, multi_std): New reservations.  Adjust bypasses.
	* arc.md: Update Copyright blurb.
	Include new files.
	(tune, tune_arc700): New attributes.
	(movsi_set_cc_insn): Set type to "compare".
	* config/arc/arc.h: Protect against multiple inclusion.
	Include "insn-attr.h".
	* arc.opt (multcost, mtune): New options.

2007-10-23  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (TRAMPOLINE_ADJUST_ADDRESS): Properly define.
	* config/arc/arc.md (sub_cond_exec): Add mode to set source.
	(doloop_begin_i): Add mode to operand 0.

	* config/arc/divtab-arc700.c (main): Emit reverse ordered table.
	* config/arc/lib1funcs.asm (__divsi3): Use pcl as base addressing
	register with a negative index.

	* config/arc/predicates.md (cc_register): Now a special_predicate.
	(_2_4_8_operand): New predicate.
	* config/arc/arc.c (arc_rtx_costs): Add description for add_n and
	sub_n costs.
	* config/arc/arc.md (add_n, sub_n): New patterns.
	(shift_and_add_insn_mixed): Type shift.

	* combine.c (gen_lowpart_for_combine): Use omode when generating
	clobber.

2007-10-18  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc-protos.h (arc_save_restore): Don't declare.
	* config/arc/predicates.md (p2_immediate_operand): New predicate.
	(zn_compare_operator, commutative_operator): Likewise.
	(noncommutative_operator, unary_operator): Likewise.
	(move_dest_operand): Reject PRE_MODIFY / POST_MODIFY with
	register-register increment.
	(proper_comparison_operator): Replace.
	* config/arc/arc.c ("tm-constrs.h", "reload.h"): #include.
	(void arc_init): Don't set arc_punct_chars['~'].
	(get_arc_condition_code): Take mode into account.
	(arc_select_cc_mode): Replace.
	(arc_init_reg_tables): Update CC*mode handling.
	(arc_address_cost): Replace costs.
	(arc_save_restore): New arguments parity and first_offset.
	(arc_output_function_prologue): Emit short instructions.
	Fold stack adjustments into pre-modify addressing.
	(arc_output_function_epilogue): Likewise.
	Don't emit a nop when delay slot remains unfilled.
	If the delay insn is short, make jump long.
	(output_scaled): New variable.
	(arc_print_operand): Remove '~' case.
	Add support for scaled address offsets.
	Add 'B' and 'O' cases.
	(record_cc_ref): Remove COND_SET_ZNC handling.
	(arc_final_prescan_insn): Support -misize.
	(arc_reorg): Handle case when end of zero-overhead loop has been
	duplicated.
	When generating brcc insn, check that first comparison operand is
	a register.
	(arc_output_addsi): New function.
	* config/arc/arc.h (OPTIMIZATION_OPTIONS): Set default for
	arc_size_opt_level.
	(OVERRIDE_OPTIONS): When (arc_size_opt_level == 3), set optimize_size.
	(DATA_ALIGNMENT): Don't add extra alignment when arc_size_opt_level
	is 3.
	(SMALL_INT_RAGE): New macro.
	(HAVE_PRE_MODIFY_REG, HAVE_POST_MODIFY_REG): Define.
	(RTX_OK_FOR_OFFSET_P): Allow scaled indices.
	(LEGITIMATE_OFFSET_ADDRESS_P): Allow register-register
	PRE_MODIFY / POST_MODIFY.
	(JUMP_ALIGN, MOVE_MAX_PIECES): Define.
	(LABEL_ALIGN_AFTER_BARRIER, LOOP_ALIGN): Set to JUMP_ALIGN.
	* config/arc/arc.md (mov_set_cc_insn,core_insn, compare_insn):
	Align write_port allocation with other insn_reservation definitions.
	(atribute cond): Remove set_znc value.  Changed all users.
	(movqi_insn, movsi_insni, zero_extendqihi2): Rebalance alternatives.
	(zero_extendqisi2_aci, addsi3_mixed, subsi3_insn): Likewise.
	(cmpsi_cc_insn_mixed): Likewise.
	(movsi_set_cc_insn): Match both CC_ZNmode and CC_Zmode.
	(*movsi_insn_mixed): Changed another 'r' to 'c'.
	(unary_comparison, commutative_binary_comparison): New patterns.
	(noncommutative_binary_comparison, (cmpsi_cc_zn_insn): Likewise.
	(cmpsi_cc_z_insn, cmpsi_cc_c_insn, movsi_ne): Likewise.
	(movsi_cond_exec, add_cond_exec, commutative_cond_exec): Likewise.
	(sub_cond_exec, noncommutative_cond_exec): Likewise.
	(movdi_insn, movdf_insn): Add suport for scaled offsets.
	(zero_extendhisi2, extendhisi2): Likewise.
	(movsicc_insn, cbranchsi4_scratch): Tweak alternatives.
	(zero_extendhisi2): Add suport for scaled offsets.
	(addsi3_mixed): Use arc_output_addsi.
	(cmpsi_ccznc_insn_a4, cmpsi_cczn_insn_mixed): Deleted.
	(cmpsi_cczn_insn): Likewise.
	(scc_insn): Now a define_split.
	(branch_insn, rev_branch_insn, call_via_label): Don't output nops.
	(call_prof, call_value_via_label, call_value_prof): Likewise.
	(sibcall_value_insn, sibcall_prof, sibcall_value_prof): Lkewise.
	(flush_icache+3): Use CC_ZN_mode.
	(flush_icache+4): Likewise.  Check that operand[4] dies.
	(sibcall_insn): Add short alternative.
	(doloop_begin_i): Align loop insn.
	* config/arc/constraints.md (C_0, Usc, Rcq, Rs5): New constraints.
	(U): Rename to:
	(Usd).
	* config/arc/arc.opt (-malign-loops,-mno-align-loops): Remove.
	(msize-level=): Add.
	* config/arc/arc-modes.def (CCZNC, CCZN): Remove.
	(CC_ZN, CC_Z, CC_C): Add.

2007-10-15  J"orn Rennecke  <joern.rennecke@arc.com>

	* tree-ssa-loop-ch.c (copy_loop_headers): Suppress one case of
	counter-productive copying.
	* config/arc/arc-protos.h (arc_cond_exec_p): Declare.
	(arc_secondary_reload, arc_register_move_cost): Likewise.
	* config/arc/predicates.md (move_double_src_operand): Allow
	auto inc/dec.
	* config/arc/arc.c (arc_invalid_within_doloop): New function.
	(arc_cond_exec_p, arc_secondary_reload): Likewise.
	(arc_register_move_cost): Likewise.
	(TARGET_INVALID_WITHIN_DOLOOP): Define.
	(arc_print_operand): Handle POST_INC / POST_DEC / PRE_MODIFY /
	POST_MODIFY.
	(arc_reorg): Fix up zero-overhead loops.
	* config/arc/arc.h (FIRST_PSEUDO_REGISTER): Now 146.
	(FIXED_REGS): Make LP_COUNT non-fixed.
	(FIXED_REGS, CALL_USED_REGS, REG_CLASS_CONTENTS): Add LP_START & LP_END.
	Make LP_COUNT non-fixed.
	(PREFERRED_RELOAD_CLASS): Substitute GENERAL_REGS for CORE_REGS.
	(HAVE_POST_INCREMENT, HAVE_POST_DECREMENT): Define.
	(HAVE_PRE_MODIFY_DISP, HAVE_POST_MODIFY_DISP): Likewise.
	(SECONDARY_RELOAD_CLASS): Likewie.
	(GO_IF_LEGITIMATE_ADDRESS, GO_IF_MODE_DEPENDENT_ADDRESS): Add handling
	for POST_DEC / POST_INC / PRE_MODIFY / POST_MODIFY.
	(REGISTER_NAMES): Add lp_start and lp_end.
	* config/arc/arc.md (UNSPEC_LP, LP_COUNT, LP_START, LP_END): New
	constants.
	(type): Add loop.
	(in_delay_slot): No loops.
	(entire file): Use "c" constraint.
	(movdi_insn): Handle POST_INC / POST_DEC / PRE_MODIFY / POST_MODIFY.
	(movdf_insn): Likewise.
	(addsi3_mixed): Make conditional execution take precedence over
	short insns.  Improve instruction selection.
	(subsi3_insn): Add rsub support.
	(doloop_begin, doloop_begin_i, doloop_end, doloop_end_i): New patterns.
	(doloop_fallback): Likewise.
	* config/arc/constraints.md (Cca, C2a): New constraints.
	* config/arc/arc.opt (mexperimental-mask): New option.

2007-10-08  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/t-arc-newlib (MULTILIB_OPTIONS): Remove mA4.
	(MULTILIB_DIRNAMES): Remove arc.
	* config/arc/arc.c (arc_init <!USE_UCLIB>): Default is A5.
	* config/arc/arc.h (CPP_SPEC): Default to __A5__.
	(ASM_SPEC): Default is -mA5.
	(MULTILIB_DEFAULTS): Set to { "mA5", "EL" } .

2007-10-03  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/gmon/gmon.c: #include <stddef.h> .

2007-10-01  J"orn Rennecke  <joern.rennecke@arc.com>

	* arc.h (LOCAL_ALIGNMENT): Define.

2007-10-01  J"orn Rennecke  <joern.rennecke@arc.com>

	* config.gcc (arc-*-elf*): Set tmake_file.
	(arc*-*-linux-uclibc*): Add arc/t-arc to tmake_file.
	* config/arc/t-arc: Use $@ / $<.
	(CRTSTUFF_T_CFLAGS_S): Remove.
	($(T)prof-freq-stub.o, $(T)dcache_linesz.o): New rules.
	($(T)profil-uclibc.o): Likewise.
	($(T)libgmon.a): Use $(PROFILE_OSDEP).
	(EXTRA_MULTILIB_PARTS): Remove mcount.o, gmon.o and prof-freq.o.
	(MULTILIB_OPTIONS, MULTILIB_DIRNAMES ULTILIB_MATCHES) Move to:
	* config/arc/t-arc-newlib: New file.
	(PPROFILE_OSDEP): Define.
	* config/arc/t-arc700-uClibc: Remove pieces redundant with t-arc.
	(PROFILE_OSDEP): Define.
	* config/arc/arc.h (STARTFILE_SPEC) <USE_UCLIBC>: Remove references
	to gcrt1.o.
	(ENDFILE_SPEC) <USE_UCLIBC> : Use crtgend.o for profiling.
	(LIB_SPEC) <USE_UCLIBC>: Use -lgmon -u profil --defsym __profil=profil
	for profiling.
	* config/arc/gmon/gmon.c (__profile_frequency): Remove.
	* config/arc/gmon/machine-gmon.h (_mcount_tailcall): Don't declare.
	* config/arc/gmon/profil.S (__profile_timer_cycles): New weak symbol.
	(__profil): Use it.  Fix FUNC usage.  Streamline ARC700 cache
	initialization a bit.
	(__dcache_linesz): Move to:
	* config/arc/gmon/dcache_linesz.S: New file.
	* config/arc/gmon/prof-freq-stub.S: New file.

2007-10-01  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/predicates.md (call_address_operand): Check CONSTANT_P
	before using LEGITIMATE_CONSTANT_P.

2007-09-28  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (DEFAULT_NO_SDATA): Define, set of USE_UCLIBC.
	(TARGET_DEFAULT_TARGET_FLAGS): Use it.

2007-09-19  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.md (sibcall_insn): Use call_address_operand.
	(sibcall_value_insn, sibcall_prof, sibcall_value_prof): Likewise.

2007-09-11  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c: Back out patches for callee-based profiling.
	* config/arc/arc.h: Likewise.

	* config/arc/arc-protos.h (arc_profile_call): Declare.
	* config/arc/arc.c (arc_initialize_trampoline): Generate oddly aligned
	trampoline.
	(write_profile_sections, unspec_prof_hash): New functions.
	(unspec_prof_htab_eq, arc_profile_call): Likewise.
	(arc_legitimate_constant_p): Allow UNSPEC_PROF.
	(arc_function_ok_for_sibcall): Allow long & indirect calls.
	Fix interrupt function check.
	(arc_reorg): Fix up UNSPEC_PROF and emit special profile sections.
	* config/arc/arc.h (ENDFILE_SPEC) <!USE_UCLIBC>: Add crtgend.
	(TRAMPOLINE_SIZE): Bump up to 20.
	(TRAMPOLINE_ADJUST_ADDRES): Define.
	* config/arc/gmon/gmon.c: Adjust for caller-based profiling.
	* config/arc/gmon/mcount.c, gmon/machine-gmon.h: Likewise.
	* config/arc/gmon/profil.S, config/arc/gmon/sys/gmon.h: Likewise.
	* config/arc/arc.md (UNSPEC_PROF): Define.
	(call, call_value, sibcall, sibcall_value): Add profiling code.
	(call_prof, call_value_prof, sibcall_prof): New patterns.
	(sibcall_value_prof): Likewise.
	(sibcall_insn, sibcall_value_insn): Enable long / indirect sibcalls.
	* config/arc/crtgend.asm: New file.
	* config/arc/lib1funcs.asm (__udivsi3, __umodsi3):
	Add .__arc_profile_forward entries.
	* config/arc/t-arc (crtgend.o) New target.
	(mcount.o): Remove -fcall-saved-r11.
	(gmon.o): Add -mno-sdata
	(EXTRA_MULTILIB_PARTS): Add crtgend.o
	* config/arc/constraints.md (Cbr): New constraint.
	* config/arc/crtg.asm: Add Start labels and alignment for
	profile sections.

	* config/arc/arc.h (enum reg_class): Add SIBCALL_REGS and CORE_REGS.
	(REG_CLASS_NAMES, REG_CLASS_CONTENTS): Likewise.
	* contraints.md (c): Change to:
	(Rgp).
	(c, Rsc): New constraints.
	* config/arc/arc.md (iorsi3): Use 'c' constraint.

2007-09-07  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (TARGET_VERSION): Fix spelling.
	* config/arc/t-arc700-uClibc (LIB1ASMFUNCS): Remove excess space.

	* config/arc/lib1funcs.asm (__modsi3): Make Ldivstart a local label.

	* config/arc/arc.h (STARTFILE_SPEC): Add -mkernel option.

	* config/arc/gmon/gmon.c (__profile_frequency): Define nonzero.
	(__monstartup): On error, clear p->tolimit.
	* config/arc/gmon/mcount.c (_MCOUNT_DECL): Don't use
	catomic_compare_and_exchange_bool_acq.
	* config/arc/gmon/machine-gmon.h: Don't use __builtin_frame_address.
	* config/arc/gmon/profil.S (__profil): Fix vector initialization.
	Enable interrrupts.
	(__profil_irq): Fix register assignment.  Make j.f explicit.

2007-08-29  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/t-arc700-uClibc (CRTSTUFF_T_CFLAGS): Add -mno-sdata .
	(MULTILIB_EXTRA_OPTS): Add mno-sdata .

2007-08-23  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (LINK_SPEC) [!USE_UCLIBC]: Use -marcelf.

	* config/arc/arc.h (STARTFILE_SPEC) [!USE_UCLIBC] [pg|p]: Add crtg.o.
	(LIB_SPEC) [!USE_UCLIBC]  [pg|p]: Add -lgmon.
	(REG_CLASS_CONTENTS): Add blink to GENERAL_REGS.
	(LARGE_INT): Fix cast type.
	(FUNCTION_PROFILER): Provide expansion.
	(PROFILE_BEFORE_PROLOGUE): Define.
	(NO_PROFILE_COUNTERS): Define.
	* config/arc/arc.c (MUST_SAVE_RETURN_ADDR): Take
	current_function_profile into account.
	(arc_compute_frame_size): Don't use MUST_SAVE_RETURN_ADDR as lvalue.
	arc_output_function_prologue): Don't push blink if it has already
	been pushed by the profiling code.
	* config/arc/gmon: New directory, containing atomic.h (empty),
	auxreg.h, gmon.c, machine-gmon.h, mcount.c, prof-freq.c, profil.S,
	sys/gmon.h, and sys/gmon_out.h .
	* config/arc/t-arc (crtg.o, mcount.o, gmon.o, prof-freq.o): New rules.
	(profil.o, libgmon.a): Likewise.
	(EXTRA_MULTILIB_PARTS): Add mcount.o, gmon.o, prof-freq.o, libgmon.a
	and crtg.o .
	* config/arc/asm.h, config/arc/crtg.asm: New files.
	

2007-05-29  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_decl_anon_ns_mem_p): New function, copied from
	cp/tree.c .
	(arc_in_small_data_p): Use default_binds_local_p_1 and
	arc_decl_anon_ns_mem_p to determine if a symbol binds locally.

2007-05-18  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/predicates.md (move_src_operand): Uncomment handling of
	CONST.

	* config/arc/arc.h (ASM_SPEC): Supply -mEA also for -mA7.

2007-05-14  J"orn Rennecke  <joern.rennecke@arc.com>

	* version.c: Add leading space.  Add suffix to denote ARC changes.

2007-05-14  J"orn Rennecke  <joern.rennecke@arc.com>

	Revert this patch:
	2007-04-19  J"orn Rennecke  <joern.rennecke@arc.com>
	  * output.h (get_unnamed_section_ggc): New macro.
	  * varasm.c (init_varasm_once): Use it.

	Instead, make section xmalloced:
	* c-pch.c (c_common_write_pch): Call pickle_in_section and
	unpickle_in_section.
	(c_common_read_pch): Call unpickle_in_section.
	* varasm.c (unnamed_sections): Remove GTY marker.
	(get_unnamed_section, get_noswitch_section): xmalloc section.
	(pickled_in_section): New static variable.
	(pickle_in_section, unpickle_in_section): New functions.
	* output.h (struct unnamed_section): Mark as GTY((skip)).
	(union section): Mark members unnamed_section and noswitch_section
	as GTY((skip)).
	(text_section, data_section, readonly_data_section): Remove GTY marker.
	(sdata_section, ctors_section, dtors_section, bss_section): Likewise.
	(sbss_section, tls_comm_section, comm_section): Likewise.
	(lcomm_section, bss_noswitch_section, in_section): Likewise.
	(pickle_in_section, unpickle_in_section): Declare.

	* config/arc/arc.c (arc_function_value): Mark unsignedp with
	ATTRIBUTE_UNUSED.

2007-04-26  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc-protos.h (arc_function_value): Declare.
	* config/arc/arc.h (FUNCTION_VALUE): Don't define.
	* config/arc/arc.c (TARGET_FUNCTION_VALUE): Define.
	(arc_function_value): New function.

2007-04-25  J"orn Rennecke  <joern.rennecke@arc.com>

	PR 20375
	* config/arc/arc.c (arc_setup_incoming_varargs): Don't abort on
	BLKmode.  Use arc_function_arg_advance.

2007-04-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (TARGET_MUST_PASS_IN_STACK):
	Set to must_pass_in_stack_var_size.

2007-04-25  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.h (TARGET_CPU_CPP_BUILTINS): Add __big_endian__ .
	(CPP_SPEC): Remove __big_endian__ .
	(ASM_SPEC, LINK_SPEC): For -mbig-endian, provide -EB.
	* config/arc/t-arc (MULTILIB_MATCHES): Add EB=mbig-endian .

2007-04-24  J"orn Rennecke  <joern.rennecke@arc.com>

	* config/arc/arc.c (arc_function_arg_advance): Only use
	ROUND_ADVANCE_CUM if the argument has a non-zero size.

2007-04-24  J"orn Rennecke  <joern.rennecke@arc.com>

	Fix profile based feedback:
	* config.gcc (arc-*-elf*): Don't use config/svr4.h.
	* config/arc/arc.h: Don't include config/svr4.h.

2007-04-20  J"orn Rennecke  <joern.rennecke@arc.com>

	* longlong.h (__arc__): For __ARC700__, define umul_ppmm, UMUL_TIME,
	__umulsidi3, count_leading_zeros, COUNT_LEADING_ZEROS_0 .
	(COUNT_TRAILING_ZEROS_0): Define this according to
	COUNT_LEADING_ZEROES_0 when defining count_trailing_zeros using
	count_leading_zeroes.
	* libgcc2.c (__ffsSI2): Use COUNT_TRAILING_ZEROS_0.

	* config/arc/divtab-arc700.c: New file.

	Fix nested functions:
	* gcc/config/arc/arc.c (emit_store_direct): New function.
	(arc_initialize_trampoline): Initialize trampoline.
	* gcc/config/arc/arc.h (TRAMPOLINE_TEMPLATE): Disable definition
	which uses the wrong architecture and ABI.
	(TRAMPOLINE_ALIGNMENT): Define.
	(INITIALIZE_TRAMPOLINE): Use arc_initialize_trampoline.

	Fix stdarg argument passing:
	* gcc/config/arc/arc.c (arc_va_arg, arc_pass_by_reference): Pass
	variable size and addressable type by reference.
	(arc_arg_partial_bytes): Adjust CUM according to alignment of argument.
	(arc_function_arg, arc_function_arg_advance): Likewise.
	* gcc/config/arc/arc.h (ROUND_ADVANCE_CUM): Round.

	* gcc/config/arc/arc.h (TARGET_CPU_CPP_BUILTINS): Define
	__BIG_ENDIAN__ or __LITTLE_ENDIAN__, as the case might be.

	*  gcc/config/arc/arc.h (ASM_SPEC): For mARC700, pass -mEA.

2007-04-19  J"orn Rennecke  <joern.rennecke@arc.com>

	* output.h (get_unnamed_section_ggc): New macro.
	* varasm.c (init_varasm_once): Use it.

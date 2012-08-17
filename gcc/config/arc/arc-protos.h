/* Definitions of target machine for GNU compiler, ARC ARCompact cpu.
   Copyright (C) 2000, 2007, 2008, 2009, 2010 Free Software Foundation, Inc.

   Copyright 2007-2012 Synopsys Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifdef RTX_CODE
#ifdef TREE_CODE
extern rtx arc_va_arg (tree, tree);
#endif /* TREE_CODE */

extern enum machine_mode arc_select_cc_mode (enum rtx_code, rtx, rtx);

/* Define the function that build the compare insn for scc and bcc.  */
extern struct rtx_def *gen_compare_reg (enum rtx_code, enum machine_mode);

/* Declarations for various fns used in the .md file.  */
extern void arc_output_function_epilogue (FILE *, HOST_WIDE_INT, int);
extern const char *output_shift (rtx *);
extern int compact_load_memory_operand (rtx op,enum machine_mode  mode);
extern int compact_sda_memory_operand (rtx op,enum machine_mode  mode);
extern int arc_valid_machine_decl_attribute (tree type,tree attributes,tree identifier,tree args);
extern int compact_store_memory_operand (rtx op,enum machine_mode  mode);
extern int u6_immediate_operand (rtx op,enum machine_mode mode);
extern int arc_double_register_operand (rtx op,enum machine_mode mode);
extern int symbolic_operand (rtx, enum machine_mode);
extern int arc_double_limm_p (rtx);
extern int arc_eligible_for_epilogue_delay (rtx, int);
extern void arc_initialize_trampoline (rtx, rtx, rtx);
extern void arc_print_operand (FILE *, rtx, int);
extern void arc_print_operand_address (FILE *, rtx);
extern void arc_final_prescan_insn (rtx, rtx *, int);
extern void arc_set_default_type_attributes(tree type);
extern int call_address_operand (rtx, enum machine_mode);
extern int call_operand (rtx, enum machine_mode);
extern int compact_register_operand (rtx op,enum machine_mode  mode);
extern int cc_register (rtx x, enum machine_mode mode);
extern int short_immediate_operand (rtx, enum machine_mode);
extern int long_immediate_operand (rtx, enum machine_mode);
extern int long_immediate_loadstore_operand (rtx, enum machine_mode);
extern int move_src_operand (rtx, enum machine_mode);
extern int move_double_src_operand (rtx, enum machine_mode);
extern int move_dest_operand (rtx, enum machine_mode);
extern int load_update_operand (rtx, enum machine_mode);
extern int store_update_operand (rtx, enum machine_mode);
extern int nonvol_nonimm_operand (rtx, enum machine_mode);
extern int const_sint32_operand (rtx, enum machine_mode);
extern int const_uint32_operand (rtx, enum machine_mode);
extern int proper_comparison_operator (rtx, enum machine_mode);
extern int shift_operator (rtx, enum machine_mode);
extern int arc_dpfp_operator (rtx, enum machine_mode);
extern int arc_emit_vector_const (FILE *, rtx);
extern const char *arc_output_libcall (const char *);
extern int prepare_extend_operands (rtx *operands, enum rtx_code code,
				    enum machine_mode omode);
extern const char *arc_output_addsi (rtx *operands, const char *);
extern int arc_expand_movmem (rtx *operands);
extern int prepare_move_operands (rtx *operands, enum machine_mode mode);
extern void arc_split_dilogic (rtx *, enum rtx_code);
extern void override_options (void);
#endif /* RTX_CODE */

#ifdef TREE_CODE
extern enum arc_function_type arc_compute_function_type (struct function *);
#endif /* TREE_CODE */


extern void arc_init (void);
extern unsigned int arc_compute_frame_size (int);
extern int arc_delay_slots_for_epilogue (void);
extern rtx arc_finalize_pic (void);
extern int arc_ccfsm_branch_deleted_p (void);
extern void arc_ccfsm_record_branch_deleted (void);

extern rtx arc_legitimize_pic_address (rtx, rtx);
extern int arc_function_arg_partial_nregs (CUMULATIVE_ARGS *, enum machine_mode, tree,int);
extern rtx arc_function_arg (CUMULATIVE_ARGS *, enum machine_mode, tree, int);
extern void arc_function_arg_advance (CUMULATIVE_ARGS *, enum machine_mode, tree, int);
void arc_asm_output_aligned_decl_local (FILE *, tree, const char *, 
					unsigned HOST_WIDE_INT, 
					unsigned HOST_WIDE_INT, 
					unsigned HOST_WIDE_INT);
extern rtx arc_return_addr_rtx (int , rtx );
extern int check_if_valid_regno_const (rtx *, int );
extern int check_if_valid_sleep_operand (rtx *, int );
extern int check_if_valid_sleep_operand (rtx *, int );
extern bool arc_legitimate_constant_p (rtx);
extern int arc_legitimate_pc_offset_p (rtx);
extern int arc_legitimate_pic_addr_p (rtx);
extern void arc_assemble_name (FILE *, const char*);
extern int symbolic_reference_mentioned_p (rtx);
extern void emit_pic_move (rtx *, enum machine_mode); 
extern int arc_raw_symbolic_reference_mentioned_p (rtx);
extern bool arc_legitimate_pic_operand_p (rtx);
extern const char * gen_bbit_insns(rtx *) ATTRIBUTE_UNUSED;
extern const char * gen_bbit_bic_insns(rtx *) ATTRIBUTE_UNUSED;
extern int valid_bbit_pattern_p (rtx *, rtx) ATTRIBUTE_UNUSED;
extern int arc_is_longcall_p (rtx);
extern int arc_profile_call (rtx callee);
extern int valid_brcc_with_delay_p (rtx *);
extern int small_data_pattern (rtx , enum machine_mode ATTRIBUTE_UNUSED);
extern rtx arc_rewrite_small_data (rtx);
extern int arc_ccfsm_cond_exec_p (void);
struct secondary_reload_info;
extern int arc_register_move_cost (enum machine_mode, enum reg_class,
				   enum reg_class);
extern rtx disi_highpart (rtx);
extern int arc_adjust_insn_length (rtx, int);
extern int arc_corereg_hazard (rtx, rtx);
extern int arc_hazard (rtx, rtx);
extern int arc_write_ext_corereg (rtx);
extern void arc_conditional_register_usage (void);
extern rtx gen_acc1 (void);
extern rtx gen_acc2 (void);
extern rtx gen_mlo (void);
extern rtx gen_mhi (void);
extern int arc_unalign_branch_p (rtx);
extern int arc_branch_size_unknown_p (void);
struct arc_ccfsm;
extern void arc_ccfsm_record_condition (rtx, int, rtx, struct arc_ccfsm *);
extern void arc_expand_prologue (void);
extern void arc_expand_epilogue (int);
extern void arc_init_expanders (void);
extern int arc_check_millicode (rtx op, int offset, int load_p);
extern int arc_get_unalign (void);
extern void arc_clear_unalign (void);
extern void arc_toggle_unalign (void);
extern void split_addsi (rtx *);
extern void split_subsi (rtx *);
extern void arc_pad_return (void);
extern rtx arc_split_move (rtx *);
extern char *arc_shorten_align (void);
extern void arc_ccfsm_advance_to (rtx);
extern int arc_verify_short (rtx insn, int unalign, int);
extern const char *arc_short_long (rtx insn, const char *, const char *);
extern rtx arc_regno_use_in (unsigned int, rtx);
extern int arc_attr_type (rtx);
extern int arc_scheduling_not_expected (void);
extern int arc_sets_cc_p (rtx insn);
extern int arc_label_align (rtx label);
extern int arc_need_delay (rtx insn);
extern int arc_text_label (rtx);
extern int arc_decl_pretend_args (tree decl);
extern int arc_dead_or_set_postreload_p (const_rtx insn, const_rtx reg);
extern rtx arc_legitimize_address (rtx x, rtx oldx, int mode);

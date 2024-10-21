/* CRC optimization.
   Copyright (C) 2022-2024 Free Software Foundation, Inc.
   Contributed by Mariam Arutunian <mariamarutunian@gmail.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.   */

/* This pass performs CRC optimization.  */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "tree.h"
#include "gimple.h"
#include "tree-pass.h"
#include "ssa.h"
#include "gimple-iterator.h"
#include "tree-cfg.h"
#include "tree-ssa-loop-niter.h"
#include "cfgloop.h"
#include "gimple-range.h"
#include "tree-scalar-evolution.h"
#include "hwint.h"
#include "internal-fn.h"
#include "tree-into-ssa.h"
#include "tree-ssa-loop-manip.h"
#include "predict.h"
#include "cfghooks.h"
#include "tree-ssa.h"
#include "tree-ssa-live.h"
#include "tree-dfa.h"
#include "dominance.h"
#include "tree-ssa-dce.h"
#include "tree-cfgcleanup.h"
#include "tree.h"
#include "gimple-pretty-print.h"

class crc_optimization {
 private:
  /* The statement doing shift 1 operation before/after xor operation.  */
  gimple *m_shift_stmt;

  /* Phi statement from the head of the loop for CRC.  */
  gphi *m_phi_for_crc;

  /* Phi statement for the data from the head of the loop if exists,
     otherwise - nullptr.  */
  gphi *m_phi_for_data;

  /* The loop, which probably calculates CRC.  */
  class loop *m_crc_loop;

  /* Depending on the value of M_IS_BIT_FORWARD, may be forward or reversed CRC.
     If M_IS_BIT_FORWARD, then it is bit-forward implementation,
     otherwise bit-reversed.  */
  bool m_is_bit_forward;

  /* Sets initial values for CRC analyses.  */
  void set_initial_values ();

  /* This is the main function that checks whether the given LOOP
     calculates CRC and extracts details of the CRC calculation.

     The main idea is to find the innermost loop with 8, 16, 24, 32, 64
     iterations and xor instruction (xor is the key operation for naive CRC
     calculation). Then, checks that the variable is shifted by one before/after
     being used in xor.
     Xor must be done under the condition of MSB/LSB being 1.  */
  bool loop_may_calculate_crc (class loop *loop);


  /* Returns true if there is only two conditional blocks in the loop
     (one may be for the CRC bit check and the other for the loop counter).
     This may filter out some real CRCs, where more than one condition
     is checked for the CRC calculation.  */
  static bool loop_contains_two_conditional_bb (basic_block *loop_bbs,
						unsigned loop_num_nodes);

  /* Checks the FUNC_LOOP loop's iteration number.
     The loop for CRC calculation may do 8, 16, 24, 32, 64 iterations.  */
  bool satisfies_crc_loop_iteration_count (class loop *func_loop);

  /* Checks whether found XOR_STMT is for calculating CRC.
     The function CRC_FUN calculates CRC only if there is a shift operation
     in the crc loop.  */
  bool xor_calculates_crc (function *crc_fun, basic_block *loop_bbs,
			   const gimple *xor_stmt);

  /* Returns true if we can get definition of the VARIABLE, and the definition
     it's not outside the loop.  Otherwise, returns false.  */
  bool passes_checks_for_def_chain (tree variable);

  /* This function goes up through the def-use chains of the parameter NAME.
     Gathers all the statements within the loop,
     from which the variable depends on and adds to the USE_DEFS.
     Returns false, if there is a statement that may not exist in the CRC
     loop.  Otherwise, returns true.  */
  bool set_defs (tree name, auto_vec<gimple *>& use_defs,
		 bool keep_only_header_phis);

  /* Set M_PHI_FOR_CRC and M_PHI_FOR_DATA fields.
     Returns false if there are more than two (as in CRC calculation only CRC's
     and data's phi may exist) or no phi statements in STMTS (at least there
     must be CRC's phi).
     Otherwise, returns true.  */
  bool set_crc_and_data_phi (auto_vec<gimple *>& stmts);

  /*  Returns true if the variable checked in the condition depends on possible
      CRC value.  Otherwise, returns false.  */
  bool cond_depends_on_crc (auto_vec<gimple *>& stmts);


  /* Checks that the condition is for checking CRC.
     Returns true if xor is done under the condition of MSB/LSB being 1, and
     the condition's variable and xor-ed variable depend on the same variable.
     Otherwise, returns false.
     XOR_BB is the basic block, where the xor operation is done.
     PRED_BB is the predecessor basic block of the XOR_BB, it is assumed that
     the last stmt of PRED_BB checks the condition under which xor is done.  */
  bool crc_cond (basic_block pred_bb, basic_block xor_bb,
		 basic_block *loop_bbs);

  /* Returns true if xor is done in case the MSB/LSB is 1.
     Otherwise, returns false.
     In CRC calculation algorithms CRC is xor-ed with the polynomial only
     if MSB/LSB is 1.

     PRED_BB is the block containing the condition for the xor.
     XOR_BB is the one of the successor blocks of PRED_BB, it is assumed that
     CRC is xor-ed with the polynomial in XOR_BB.
     COND is the condition, which is checked to satisfy the CRC condition.  */
  bool is_crc_satisfiable_cond (basic_block pred_bb, basic_block xor_bb,
				gcond *cond);

  /* Checks that the variable used in the condition COND is the assumed CRC
     (or depends on the assumed CRC).
     Also sets data member m_phi_for_data if it isn't set and exists.  */
  bool is_crc_checked (gcond *cond, basic_block *loop_bbs);

  /* Returns true if condition COND checks MSB/LSB bit is 1.
     Otherwise, return false.  */
  static bool cond_true_is_checked_for_bit_one (const gcond *cond);

  /* Returns opposite block of the XOR_BB from PRED_BB's dest blocks.  */
  static basic_block get_xor_bb_opposite (basic_block pred_bb,
					  basic_block xor_bb);

  /* Checks whether the pair of xor's shift exists in the opposite
     basic block (OPPOSITE_BB).
     If there is a shift and xor in the same block,
     then in the opposite block must be another shift.  */
  bool exists_shift_for_opp_xor_shift (basic_block opposite_bb);

  /* Goes down by def-use chain (within the CRC loop) and returns the statement
     where variable (dependent on xor-ed variable) is shifted with 1.
     Between xor and shift operations only phi statements are allowed.
     Otherwise, returns nullptr.  */
  gimple *find_shift_after_xor (tree xored_crc);

  /* Returns the statement which does shift 1 operation.
     If there is no such statement, returns nullptr.
     STMTS - are the statements within the loop before xor operation on
     which possible CRC variable depends.  */
  gimple *find_shift_before_xor (const auto_vec <gimple *> &stmts);

  /* Returns true if ASSIGN_STMT does shift with 1.
     Otherwise, returns false.  */
  bool can_be_crc_shift (gimple *assign_stmt);

  /* Returns true if the operation done in ASSIGN_STMT can occur during CRC
     calculation.  Otherwise, returns false.  */
  bool can_not_be_crc_stmt (gimple *assign_stmt);

  /* Returns true if the statement with STMT_CODE may occur in CRC calculation.
     Otherwise, returns false.  */
  static bool is_acceptable_stmt_code (const tree_code &stmt_code);

  /* Prints extracted details of CRC calculation.  */
  void dump_crc_information ();

 public:
  unsigned int execute (function *fun);
};

/* Prints extracted details of CRC calculation.  */

void
crc_optimization::dump_crc_information ()
{
  if (dump_file)
    {
      fprintf (dump_file,
	       "Loop iteration number is " HOST_WIDE_INT_PRINT_UNSIGNED ".\n",
	       tree_to_uhwi (m_crc_loop->nb_iterations));
      if (m_is_bit_forward)
	fprintf (dump_file, "Bit forward.\n");
      else
	fprintf (dump_file, "Bit reversed.\n");
    }
}

/* Goes down by def-use chain (within the CRC loop) and returns the statement
   where variable (dependent on xor-ed variable) is shifted with 1.
   Between xor and shift operations only phi statements are allowed.
   Otherwise, returns nullptr.  */

gimple *
crc_optimization::find_shift_after_xor (tree xored_crc)
{
  imm_use_iterator imm_iter;
  use_operand_p use_p;

  if (TREE_CODE (xored_crc) != SSA_NAME)
    return nullptr;

  /* Iterate through the immediate uses of the XORED_CRC.
     If there is a shift return true,
     if before shift there is other instruction (besides phi) return false.  */
  FOR_EACH_IMM_USE_FAST (use_p, imm_iter, xored_crc)
    {
      gimple *stmt = USE_STMT (use_p);
      if (gimple_visited_p (stmt))
	return nullptr;
      gimple_set_visited (stmt, true);
      // Consider only statements within the loop
      if (!flow_bb_inside_loop_p (m_crc_loop, gimple_bb (stmt)))
	return nullptr;

      /* If encountered phi statement, check immediate use of its result.
	 Otherwise, if encountered assign statement, check whether it does shift
	 (some other operations are allowed to be between shift and xor).  */
      if (gimple_code (stmt) == GIMPLE_PHI)
	{
	  /* Don't continue searching if encountered the loop's beginning.  */
	  if (bb_loop_header_p (gimple_bb (stmt)))
	    return nullptr;

	  return find_shift_after_xor (gimple_phi_result (stmt));
	}
      else if (is_gimple_assign (stmt))
	{
	  /* Check that stmt does shift by 1.
	     There are no other statements between
	     xor and shift, in CRC cases we detect.  */
	  if (can_be_crc_shift (stmt))
	    return stmt;
	  return nullptr;
	}
    }
    return nullptr;
}

/* Returns opposite block of the XOR_BB from PRED_BB's dest blocks.  */

basic_block
crc_optimization::get_xor_bb_opposite (basic_block pred_bb, basic_block xor_bb)
{
  if (EDGE_COUNT (pred_bb->succs) != 2)
    return nullptr;

  if (EDGE_SUCC (pred_bb, 0)->dest != xor_bb)
    return EDGE_SUCC (pred_bb, 0)->dest;
  return EDGE_SUCC (pred_bb, 1)->dest;
}

/* Checks whether the pair of xor's shift exists in the opposite
   basic block (OPPOSITE_BB).
   If there is a shift and xor in the same block,
   then in the opposite block must be another shift.  */

bool
crc_optimization::exists_shift_for_opp_xor_shift (basic_block opposite_bb)
{
  if (!opposite_bb)
    return false;

  /* Walk through the instructions of given basic block.  */
  for (gimple_stmt_iterator bsi = gsi_start_nondebug_bb (opposite_bb);
       !gsi_end_p (bsi); gsi_next_nondebug (&bsi))
    {
      gimple *stmt = gsi_stmt (bsi);
      /* Find assignment statement with shift operation.
	 Check that shift's direction is same with the shift done
	 on the path with xor, and it is a shift by one.  */
      if (is_gimple_assign (stmt))
	{
	  if (gimple_assign_rhs_code (stmt)
	      == gimple_assign_rhs_code (m_shift_stmt)
	      && integer_onep (gimple_assign_rhs2 (stmt)))
	    return true;
	}
    }
  /* If there is no shift, return false.  */
  return false;
}

/* Returns true if condition COND checks MSB/LSB bit is 1.
   Otherwise, return false.  */

bool
crc_optimization::cond_true_is_checked_for_bit_one (const gcond *cond)
{
  if (!cond)
    return false;

  tree rhs = gimple_cond_rhs (cond);
  enum tree_code code = gimple_cond_code (cond);

  /* If the condition is something == 1 -> return true.  */
  if (integer_onep (rhs) && code == EQ_EXPR)
    return true;

  /* If the condition is something != 0  or something < 0 -> return true.  */
  if (integer_zerop (rhs) && (code == NE_EXPR || code == LT_EXPR))
    return true;

  return false;
}

/* Returns true if xor is done in case the MSB/LSB is 1.
   Otherwise, returns false.
   In CRC calculation algorithms CRC is xor-ed with the polynomial only
   if MSB/LSB is 1.

   PRED_BB is the block containing the condition for the xor.
   XOR_BB is the one of the successor blocks of PRED_BB, it is assumed that CRC
   is xor-ed with the polynomial in XOR_BB.
   COND is the condition, which is checked to satisfy the CRC condition.  */

bool
crc_optimization::is_crc_satisfiable_cond (basic_block pred_bb,
					   basic_block xor_bb,
					   gcond *cond)
{
  edge true_edge;
  edge false_edge;
  extract_true_false_edges_from_block (pred_bb, &true_edge, &false_edge);
  bool cond_is_checked_for_bit_one = cond_true_is_checked_for_bit_one (cond);
  /* Check that xor is done in case the MSB/LSB is 1.  */
  if (cond_is_checked_for_bit_one && true_edge->dest == xor_bb)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Xor is done on true branch.\n");
    }
  else if (!cond_is_checked_for_bit_one && false_edge->dest == xor_bb)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Xor is done on false branch.\n");
    }
  else
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file,
		 "Xor is done if MSB/LSB is not one, not CRC.\n");
      return false;
    }
  return true;
}

/* Checks that the variable used in the condition COND is the assumed CRC
  (or depends on the assumed CRC).
  Also sets data member m_phi_for_data if it isn't set and exists.  */

bool
crc_optimization::is_crc_checked (gcond *cond, basic_block *loop_bbs)
{
  tree lhs = gimple_cond_lhs (cond);
  set_bbs_stmts_not_visited (loop_bbs, m_crc_loop->num_nodes);

  /* As conditions are in canonical form, only left part must be an
    SSA_NAME.  */
  if (TREE_CODE (lhs) == SSA_NAME)
    {
      /* Return whether there is a dependence between if condition's variable
	 and xor-ed variable.  Also set phi statement of data if it is not
	 determined earlier and is used in the loop.  */
      auto_vec<gimple *> cond_dep_stmts (m_crc_loop->num_nodes);
      if (!set_defs (lhs, cond_dep_stmts, true))
	return false;
      return cond_depends_on_crc (cond_dep_stmts);
    }

  /* Return false if there is no dependence between if condition's variable
     and xor-ed variable.  */
  return false;
}

/* Checks that the condition is for checking CRC.
   Returns true if xor is done under the condition of MSB/LSB being 1, and
   the condition's variable and xor-ed variable depend on the same variable.
   Otherwise, returns false.
   XOR_BB is the basic block, where the xor operation is done.
   PRED_BB is the predecessor basic block of the XOR_BB, it is assumed that
   the last stmt of PRED_BB checks the condition under which xor is done.  */

bool
crc_optimization::crc_cond (basic_block pred_bb, basic_block xor_bb,
			    basic_block *loop_bbs)
{
  /* Check whether PRED_BB contains condition.  We will consider only those
     cases when xor is done immediately under the condition.  */
  gcond *cond = safe_dyn_cast<gcond *> (*gsi_last_bb (pred_bb));
  if (!cond)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "No condition.\n");
      return false;
    }

  /* Check that xor is done in case the MSB/LSB is 1.  */
  if (!is_crc_satisfiable_cond (pred_bb, xor_bb, cond))
    return false;

  /* Check that CRC's MSB/LSB is checked in the condition.
     Set data member if not set and exists.  */
  if (!is_crc_checked (cond, loop_bbs))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "The condition is not related to the CRC check.\n");
      return false;
    }
  return true;
}

/* Returns true if the statement with STMT_CODE may occur in CRC calculation.
   Otherwise, returns false.  */

bool
crc_optimization::is_acceptable_stmt_code (const tree_code &stmt_code)
{
  return stmt_code == BIT_IOR_EXPR
	 || stmt_code == BIT_AND_EXPR
	 || stmt_code == BIT_XOR_EXPR
	 || stmt_code == MINUS_EXPR
	 || stmt_code == PLUS_EXPR
	 || stmt_code == RSHIFT_EXPR
	 || stmt_code == LSHIFT_EXPR
	 || TREE_CODE_CLASS (stmt_code) == tcc_unary;
}

/* Returns true if ASSIGN_STMT does shift with 1.  Otherwise, returns false.  */

bool
crc_optimization::can_be_crc_shift (gimple *assign_stmt)
{
  tree_code stmt_code = gimple_assign_rhs_code (assign_stmt);
  if (stmt_code == LSHIFT_EXPR || stmt_code == RSHIFT_EXPR)
    {
      m_is_bit_forward = (stmt_code == LSHIFT_EXPR);
      /* Check that shift one is done, keep shift statement.  */
      if (integer_onep (gimple_assign_rhs2 (assign_stmt)))
	{
	  if (m_shift_stmt)
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file,
			 "Already there is one shift.\n");
	      return false;
	    }
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file,
		     "Found <<1 or >>1.\n");
	  return true;
	}
      /* This filters out cases, when xor-ed variable is shifted by values
	 other than 1.  */
    }
    return false;
}

/* Returns true if the operation done in ASSIGN_STMT can occur during CRC
   calculation.  Otherwise, returns false.  */

bool
crc_optimization::can_not_be_crc_stmt (gimple *assign_stmt)
{
  tree_code stmt_code = gimple_assign_rhs_code (assign_stmt);
  if (!is_acceptable_stmt_code (stmt_code))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file,
		 "\nStmt with the following operation "
		 "code %s between xor and shift, "
		 "may not be CRC.\n", get_tree_code_name (stmt_code));

      return true;
    }
  return false;
}

/* Returns true if we can get definition of the VARIABLE, and the definition
   is not outside the loop.  Otherwise, returns false.  */

bool
crc_optimization::passes_checks_for_def_chain (tree variable)
{
  if (!(variable && TREE_CODE (variable) == SSA_NAME))
    return false;

  /* No definition chain for default defs.  */
  if (SSA_NAME_IS_DEFAULT_DEF (variable))
    return false;

  gimple *stmt = SSA_NAME_DEF_STMT (variable);

  if (!stmt)
    return false;

  /* Don't go outside the loop.  */
  if (!flow_bb_inside_loop_p (m_crc_loop, gimple_bb (stmt)))
    return false;

  return true;
}

/* This function goes up through the def-use chains of the parameter NAME.
   Gathers all the statements within the loop,
   from which the variable depends on and adds to the USE_DEFS.
   Returns false, if there is a statement that may not exist in the CRC
   loop.  Otherwise, returns true.  */

bool
crc_optimization::set_defs (tree name, auto_vec<gimple *> &use_defs,
			    bool keep_only_header_phis = false)
{
  if (!passes_checks_for_def_chain (name))
    return true;

  gimple *stmt = SSA_NAME_DEF_STMT (name);

  /* Don't consider already visited statement.  */
  if (gimple_visited_p (stmt))
    return true;
  gimple_set_visited (stmt, true);

  /* If it's not specified to keep only header phi's,
     then keep all statements.  */
  if (!keep_only_header_phis)
    use_defs.safe_push (stmt);

  /* If it is an assignment statement,
     get and check def-use chain for the first and second operands.  */
  if (is_a<gassign *> (stmt))
    {
      if (can_not_be_crc_stmt (stmt))
	return false;

      tree ssa1 = gimple_assign_rhs1 (stmt);
      tree ssa2 = gimple_assign_rhs2 (stmt);
      if (!set_defs (ssa1, use_defs, keep_only_header_phis))
	return false;
      if (!set_defs (ssa2, use_defs, keep_only_header_phis))
	return false;
      return true;
    }
  /* If it's a phi statement, not declared in loop's header,
     get and check def-use chain for its values.  For the one declared in loop's
     header just return true and keep it, if keep_only_header_phis is true.  */
  else if (is_a<gphi *> (stmt))
    {
      if (bb_loop_header_p (gimple_bb (stmt)))
	{
	  /* If it's specified to keep only header phi's, keep it.  */
	  if (keep_only_header_phis)
	    use_defs.safe_push (stmt);
	}
      else
	{
	  for (unsigned i = 0; i < gimple_phi_num_args (stmt); i++)
	    {
	      tree val = gimple_phi_arg_def (stmt, i);
	      if (!set_defs (val, use_defs, keep_only_header_phis))
		return false;
	    }
	}
      return true;
    }

  /* Return false for other than assigment and phi statement.  */
  return false;
}

/* Returns the statement which does shift 1 operation.
   If there is no such statement, returns nullptr.
   STMTS - are the statements within the loop before xor operation on
   which possible CRC variable depends.  */

gimple *
crc_optimization::find_shift_before_xor (const auto_vec<gimple *> &stmts)
{
  for (auto stmt_it = stmts.begin (); stmt_it != stmts.end (); stmt_it++)
    {
      /* If it is an assignment statement, check that is does shift 1.  */
      if (is_a<gassign *> (*stmt_it))
	{
	  if (can_be_crc_shift (*stmt_it))
	    return *stmt_it;
	}
    }
  return nullptr;
}

/* Set M_PHI_FOR_CRC and M_PHI_FOR_DATA fields.
   Returns false if there are more than two (as in CRC calculation only CRC's
   and data's phi may exist) or no phi statements in STMTS (at least there must
   be CRC's phi).
   Otherwise, returns true.  */

bool
crc_optimization::set_crc_and_data_phi (auto_vec<gimple *> &stmts)
{
  for (auto stmt_it = stmts.begin (); stmt_it != stmts.end (); stmt_it++)
    {
      if (is_a<gphi *> (*stmt_it) && bb_loop_header_p (gimple_bb (*stmt_it)))
	{
	  if (!m_phi_for_crc)
	    m_phi_for_crc = as_a<gphi *> (*stmt_it);
	  else if (!m_phi_for_data)
	    m_phi_for_data = as_a<gphi *> (*stmt_it);
	  else
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "Xor-ed variable depends on more than 2 "
				    "phis.\n");
	      return false;
	    }
	}
    }
  return m_phi_for_crc;
}

/*  Returns true if the variable checked in the condition depends on possible
    CRC value.  Otherwise, returns false.  */

bool
crc_optimization::cond_depends_on_crc (auto_vec<gimple *>& stmts)
{
  bool con_depends_on_crc = false;

  /* The condition may depend maximum on data and CRC phi's.  */
  if (stmts.length () > 2)
    return false;

  for (auto stmt_it = stmts.begin (); stmt_it != stmts.end (); stmt_it++)
    {
      if (is_a<gphi *> (*stmt_it) && bb_loop_header_p (gimple_bb (*stmt_it)))
	{
	  /* Check whether variable checked in the condition depends on
	     M_PHI_FOR_CRC.
	     Here don't stop the check, to set data if needed.  */
	  if (m_phi_for_crc == (*stmt_it))
	    con_depends_on_crc = true;
	  else if (m_phi_for_data && m_phi_for_data == (*stmt_it))
	    return true;
	  /* If M_PHI_FOR_DATA isn't determined, the phi statement maybe for the
	     data.  Just set it.  */
	  else if (!m_phi_for_data)
	    m_phi_for_data = as_a<gphi *> (*stmt_it);
	}
    }
  return con_depends_on_crc;
}

/* Sets initial values for the CRC analysis.
   This function is used multiple times during the analyses.  */

void
crc_optimization::set_initial_values ()
{
  m_shift_stmt = nullptr;
  m_phi_for_crc = nullptr;
  m_phi_for_data = nullptr;
  m_is_bit_forward = false;
}

/* Check whether found XOR_STMT is for calculating CRC.
   The function CRC_FUN calculates CRC only if there is a shift operation
   in the crc loop.  */

bool
crc_optimization::xor_calculates_crc (function *crc_fun, basic_block *loop_bbs,
				      const gimple *xor_stmt)
{
  tree crc_var = gimple_assign_lhs (xor_stmt);
  set_initial_values ();
  set_bbs_stmts_not_visited (loop_bbs, m_crc_loop->num_nodes);
  tree ssa1 = gimple_assign_rhs1 (xor_stmt);
  tree ssa2 = gimple_assign_rhs2 (xor_stmt);
  if (TREE_CODE (ssa2) != INTEGER_CST)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Second operand of the "
			    "xor statement isn't an integer constant.\n");
      return false;
    }

  /* Get the statements within the loop on which xor-ed variable depends.  */
  auto_vec<gimple *> xor_dep_stmts (m_crc_loop->num_nodes);
  if (!set_defs (ssa1, xor_dep_stmts))
    {
      xor_dep_stmts.release ();
      return false;
    }

  m_shift_stmt = find_shift_before_xor (xor_dep_stmts);

  if (!set_crc_and_data_phi (xor_dep_stmts))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Xor isn't used for CRC calculation.\n");
      return false;
    }

  /* Check the case when shift is done after xor.  */
  if (!m_shift_stmt)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "No shift before xor, trying to find after xor.\n");

      set_bbs_stmts_not_visited (loop_bbs, m_crc_loop->num_nodes);

      m_shift_stmt = find_shift_after_xor (crc_var);
      if (!m_shift_stmt)
	return false;
    }

  /* Get the basic block where xor operation is done.  */
  basic_block xor_bb = gimple_bb (xor_stmt);

  /* Get the predecessor basic block of xor's block.  */
  if (!single_pred_p (xor_bb))
    return false;
  basic_block block_of_condition = single_pred (xor_bb);


  /* If the found shift operation is in the same block as the xor operation,
     verify whether another shift exists in the opposite branch of the
     conditional statements.  */
  if (m_shift_stmt && gimple_bb (m_shift_stmt) == xor_bb)
    {
      basic_block opposite_block = get_xor_bb_opposite (block_of_condition,
							xor_bb);
      if (!exists_shift_for_opp_xor_shift (opposite_block))
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file,
		     "Opposite block doesn't contain shift's pair.\n");
	  return false;
	}
    }

  /* Check that xor is done if MSB/LSB is one.
     If all checks succeed, then it may be a CRC.  */
  if (crc_cond (block_of_condition, xor_bb, loop_bbs))
    {
      if (dump_file)
	fprintf (dump_file,
		 "\n%s function maybe contains CRC calculation.\n",
		 function_name (crc_fun));
      return true;
    }

  return false;
}

/* Returns true if there is only two conditional blocks in the loop,
   one may be for the CRC bit check and the other for the loop counter.
   This may filter out some real CRCs, where more than one condition
   is checked for the CRC calculation and branch-less CRCs.  */

bool
crc_optimization::loop_contains_two_conditional_bb (basic_block *loop_bbs,
						    unsigned loop_num_nodes)
{
  unsigned conditional_bb_count = 0;
  /* Iterate through the loop until the conditional branches count
     is below 3.  */
  for (unsigned i = 0; i < loop_num_nodes && conditional_bb_count <= 2; i++)
    {
      basic_block bb = loop_bbs[i];
      if (!single_succ_p (bb))
	conditional_bb_count++;
    }
  return conditional_bb_count == 2;
}

/* Checks the FUNC_LOOP loop's iteration number.
   The loop for CRC calculation may do 8, 16, 24, 32, 64 iterations.  */

bool
crc_optimization::satisfies_crc_loop_iteration_count (class loop *func_loop)
{
  /* Calculate the number of times the latch of the loop is executed.
     The function sets NB_ITERATIONS field of the loop.  */
  number_of_latch_executions (func_loop);
  tree n_inters = func_loop->nb_iterations;
  if (n_inters == NULL_TREE || n_inters == chrec_dont_know)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file,
		 "Loop iteration number is chrec_dont_know.\n");
      return false;

    }
  else if (tree_fits_uhwi_p (n_inters))
    {
      unsigned HOST_WIDE_INT
      loop_iteration_number = tree_to_uhwi (n_inters);
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Loop iteration number is "
		 HOST_WIDE_INT_PRINT_UNSIGNED ".\n", loop_iteration_number);

      if ((loop_iteration_number == 7 || loop_iteration_number == 15
	   || loop_iteration_number == 23 || loop_iteration_number == 31
	   || loop_iteration_number == 63))
	return true;
    }
  if (stderr && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Loop iteration number isn't a constant.\n");
  return false;
}

/* This is the main function that checks whether the given LOOP
   calculates CRC and extracts details of the CRC calculation.

   The main idea is to find the innermost loop with 8, 16, 24, 32, 64
   iterations and xor instruction (xor is the key operation for naive CRC
   calculation). Then, checks that the variable is shifted by one before/after
   being used in xor.
   Xor must be done under the condition of MSB/LSB being 1.  */

bool
crc_optimization::loop_may_calculate_crc (class loop *loop)
{
  /* Only examine innermost loops.  */
  if (!loop || loop->inner)
    return false;

  if (!satisfies_crc_loop_iteration_count (loop))
    return false;

  m_crc_loop = loop;
  basic_block *loop_bbs = get_loop_body_in_dom_order (m_crc_loop);

  /* Filter out the cases, which don't have exactly two conditions in the loop.
     One for the CRC bit check, the other for the loop counter.  */
  if (!loop_contains_two_conditional_bb (loop_bbs, m_crc_loop->num_nodes))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file,
		 "The number of conditional "
		 "branches in the loop isn't 2.\n");
      return false;
    }

  /* Walk bbs of the loop.  */
  for (unsigned int i = 0; i < m_crc_loop->num_nodes; i++)
    {
      basic_block bb = loop_bbs[i];
      /* Walk instructions of the bb.  */
      for (gimple_stmt_iterator bsi = gsi_start_nondebug_bb (bb);
	   !gsi_end_p (bsi); gsi_next_nondebug (&bsi))
	{
	  gimple *stmt = gsi_stmt (bsi);
	  /* If there is an xor instruction,
	     check that it is calculating CRC.  */
	  if (is_gimple_assign (stmt)
	      && gimple_assign_rhs_code (stmt) == BIT_XOR_EXPR)
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file,
			 "Found xor, "
			 "checking whether it is for CRC calculation.\n");

	      if (xor_calculates_crc (cfun, loop_bbs, stmt))
		{
		  dump_crc_information ();
		  free (loop_bbs);
		  return true;
		}
	    }
	}
    }
  free (loop_bbs);
  return false;
}

unsigned int
crc_optimization::execute (function *fun)
{
  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "\nExamining %s function.\n",
	     function_name (fun));

  if (number_of_loops (fun) <= 1)
    return 0;

  /* Get loops of the function.  */
  auto loop_list = loops_list (fun, LI_ONLY_INNERMOST);
  for (auto loop: loop_list)
    {
      /* Perform initial checks to filter out non-CRCs.  */
      loop_may_calculate_crc (loop);
    }
  return 0;
}

namespace
{

    const pass_data pass_data_crc_optimization
	= {
	    GIMPLE_PASS, /* type */
	    "crc", /* name */
	    OPTGROUP_NONE, /* optinfo_flags */
	    TV_GIMPLE_CRC_OPTIMIZATION, /* tv_id */
	    (PROP_cfg | PROP_ssa), /* properties_required */
	    0, /* properties_provided */
	    0, /* properties_destroyed */
	    0, /* todo_flags_start */
	    0, /* todo_flags_finish */
	};

    class pass_crc_optimization : public gimple_opt_pass {
     public:
      pass_crc_optimization (gcc::context *ctxt)
	  : gimple_opt_pass (pass_data_crc_optimization, ctxt)
      {}

      /* opt_pass methods: */
      virtual bool gate (function *)
      {
	return flag_gimple_crc_optimization && !optimize_size;
      }

      virtual unsigned int execute (function *);

    }; // class pass_crc_optimization

    unsigned int
    pass_crc_optimization::execute (function *fun)
    {
      return crc_optimization ().execute (fun);
    }

} // anon namespace

gimple_opt_pass *
make_pass_crc_optimization (gcc::context *ctxt)
{
  return new pass_crc_optimization (ctxt);
}

/* Subroutines used for code generation on the ARC mxp cpu.

   Copyright (C) 2008 Free Software Foundation, Inc.

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

#define BINUTILS_FIXED
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "function.h"
#include "toplev.h"
#include "tm_p.h"
#include "ggc.h"
#include "target.h"
#include "target-def.h"
#include "output.h"
#include "expr.h"
#include "optabs.h"
#include "sbitmap.h"
#include "reload.h"
#include "recog.h"

struct machine_function GTY(())
{
  int rounded_frame_size;
  int saved_regs_size;
  unsigned char lanes_written[N_HARDWARE_VECREGS];
};

rtx mxp_compare_op0, mxp_compare_op1;
sbitmap mxp_acc_classes;

/* Functions to save and restore machine-specific function data.  */
struct machine_function *
mxp_init_machine_status (void)
{
  return ggc_alloc_cleared (sizeof (struct machine_function));
}

static void mxp_init_libfuncs (void);

#undef TARGET_INIT_LIBFUNCS
#define TARGET_INIT_LIBFUNCS mxp_init_libfuncs

#undef TARGET_SECONDARY_RELOAD
#define TARGET_SECONDARY_RELOAD mxp_secondary_reload

static bool mxp_vector_mode_supported_p (enum machine_mode mode);
#undef TARGET_VECTOR_MODE_SUPPORTED_P
#define TARGET_VECTOR_MODE_SUPPORTED_P mxp_vector_mode_supported_p

#undef TARGET_PROMOTE_FUNCTION_RETURN
#define TARGET_PROMOTE_FUNCTION_RETURN hook_bool_const_tree_true

struct gcc_target targetm = TARGET_INITIALIZER;

/* machine-indepemndent: l,c,n
   v: vector register
   L: Lane mask
   N: Lane mask for lower half
   O: Lane mask for upper half
   M: memory size/lane specifier
   #: Output nops for unfilled delay slots  */
void
mxp_print_operand (FILE *file, rtx x, int code)
{
  switch (code)
    {
    case 'v': /* Vector register.  */
      if (GET_CODE (x) == PARALLEL)
	x = XEXP (XVECEXP (x, 0, 0), 0);
      if (x == CONST0_RTX (GET_MODE (x)))
	{
	  fprintf (file, "vr62");
	  return;
	}
      gcc_assert (REG_P (x));
      if (REGNO (x) <= LAST_SCALAR_REG)
	fprintf (file, "vr%d", REGNO (x) >> 3);
      else if (LANE0_REGNO (REGNO (x)) == ACC_REG)
	fputs ("vzero", file);
      else if (LANE0_REGNO (REGNO (x)) == VFLAGS_REG)
	fputs ("vr63", file);
      else
	fputs (reg_names[LANE0_REGNO (REGNO (x))], file);
      return;
    case 'L': /* Lane mask.  */
      if (x == const0_rtx)
	{
	  printf ("3"); /* For viv / vkv.  */
	  return;
	}
      gcc_assert (REG_P (x));
      if (REGNO (x) <= LAST_SCALAR_REG)
	fprintf (file, "%d",
		 (((1 << (GET_MODE_SIZE (GET_MODE (x)) >> 1)) - 1)
		  << (REGNO (x) & 7)));
      else
	fprintf (file, "%d",
		 (((1 << (GET_MODE_SIZE (GET_MODE (x)) >> 1)) - 1)
		  << VREG_LANE (REGNO (x))));
      return;
    case 'O': /* Lane mask lower half.  */
      fprintf (file, "%d",
	       (((1 << (GET_MODE_SIZE (GET_MODE (x)) >> 2)) - 1)
		<< VREG_LANE (REGNO (x))));
      return;
    case 'P': /* Lane mask upper half.  */
      fprintf (file, "%d",
	       (((1 << (GET_MODE_SIZE (GET_MODE (x)) >> 2)) - 1)
		<< (VREG_LANE (REGNO (x))
		    + (GET_MODE_SIZE (GET_MODE (x)) >> 2))));
      return;
    case 'N': /* Load memory size and lane specifier.  */
    case 'M': /* Memory size and lane specifier.  */
      {
	int r, lane;

	gcc_assert (REG_P (x));
	r = REGNO (x);
        if (r <= LAST_SCALAR_REG)
	  lane = r & 7;
	else
	  lane = VREG_LANE (r);
	fprintf (file, "%d", GET_MODE_BITSIZE (GET_MODE (x)));
	if (code == 'N' && GET_MODE_SIZE (GET_MODE (x)) == 1)
	  fprintf (file, "w");
	/* ??? The manual says that _0 is OK.  The assembler disagrees.  */
	if (lane)
	  fprintf (file, "_%d", lane);
	return;
      }
    case 'Q': /* 64 bit constant lower half.   */
      {
	enum machine_mode mode = GET_MODE (x);
	rtx sub = NULL_RTX;

	mode = GET_MODE (x);
	if (mode == VOIDmode)
	  mode = DImode;
	if (GET_MODE_SIZE (mode) >= 8)
	  sub = simplify_subreg (SImode, x, mode, 0);
	if (sub)
	  mxp_print_operand (file, sub, 0);
	else
	  output_operand_lossage ("invalid operand to %%Q");
	return;
      }
    case 'R': /* 64 bit constant upper half.  */
      {
	enum machine_mode mode = GET_MODE (x);
	rtx sub = NULL_RTX;

	mode = GET_MODE (x);
	if (mode == VOIDmode)
	  mode = DImode;
	if (GET_MODE_SIZE (mode) >= 8)
	  sub = simplify_subreg (SImode, x, mode, 4);
	if (sub)
	  mxp_print_operand (file, sub, 0);
	else
	  output_operand_lossage ("invalid operand to %%R");
	return;
      }
    case 'C': /* Condition.  */
      {
	switch (GET_CODE (x))
	  {
	  case GTU: fprintf (file, ".hi"); return;
	  case GEU: fprintf (file, ".hs"); return;
	  case LTU: fprintf (file, ".lo"); return;
	  case LEU: fprintf (file, ".ls"); return;
	  case EQ: fprintf (file, ".eq"); return;
	  case NE: fprintf (file, ".ne"); return;
	  default: gcc_unreachable ();
	  }
      }
    case '#':
      /* Output a nop for each unfilled delay slot.  */
      {
	int i = 3 - dbr_sequence_length ();

	/* If there are two delay slot insns, check if the second
	   depends on the first; in that case, we want the nop between
	   these two insns.  */
	if (i == 1)
	  {
	    rtx i1 = XVECEXP (final_sequence, 0, 1);
	    rtx i2 = XVECEXP (final_sequence, 0, 2);
	    rtx set = single_set (i1);

	    if (set && reg_referenced_p (SET_DEST (set), PATTERN (i2)))
	      {
		fprintf (file, "\n");
		final_scan_insn (i1, asm_out_file, optimize, 1, NULL);
		INSN_DELETED_P (i1) = 1;
		fprintf (file, "\tvnop");
		return;
	    }
	  }
	while (i--)
	  fprintf (file, "\n\tvnop");
	return;
      }
    case 'r': case 'i':
#ifndef BINUTILS_FIXED
      if (GET_CODE (x) == SYMBOL_REF
	 || (GET_CODE (x) == CONST && GET_CODE (XEXP (x, 0)) == PLUS
	     && GET_CODE (XEXP (XEXP (x, 0), 0)) == SYMBOL_REF)
	 || GET_CODE (x) == CODE_LABEL || GET_CODE (x) == LABEL_REF)
	{
	  fputs ((code == 'r' ? "r0" : "0"), file);
	  return;
	}
      break;
#endif
    case 0:
      break;
    default :
      /* Unknown flag.  */
      output_operand_lossage ("invalid operand output code");
    }
  switch (GET_CODE (x))
    {
    case REG:
      if (REGNO (x) <= LAST_SCALAR_REG && GET_MODE_SIZE (GET_MODE (x)) > 2)
	{
	  fputc ('k', file);
	  fputs (reg_names[REGNO (x)] + 1, file);
	  break;
	}
      fputs (reg_names[REGNO (x)], file);
      break;
    case MEM:
      {
#if 0
	int size = GET_MODE_SIZE (GET_MODE (x));
#endif
	rtx addr = XEXP (x, 0);
	rtx offs = const0_rtx;
	int r = BSS_BASE_REGNUM;

	if (CONSTANT_P (addr))
	  offs = addr;
	else if (GET_CODE (addr) == PLUS)
	  {
	    r = REGNO (XEXP (addr, 0));
	    offs = XEXP (addr, 1);
	  }
	else
	  r = REGNO (addr);
	if (GET_CODE (offs) == CONST_INT)
#if 0 /* The manual says that you use the unscaled offset...  */
	  fprintf (file, "[%s,%ld]", reg_names[r], INTVAL (offs) / size);
#else /* ... but the assembler expects the scaled offset.  */
	  fprintf (file, "[%s,%ld]", reg_names[r], INTVAL (offs));
#endif
	else
	  {
	    fprintf (file, "[%s,", reg_names[r]);
	    mxp_print_operand (file, offs, 'i');
	    fputc (']', file);
	  }
	break;
      }
    case CONST_DOUBLE :
      /* We handle SFmode constants here as output_addr_const doesn't.  */
      if (GET_MODE (x) == SFmode)
	{
	  REAL_VALUE_TYPE d;
	  long l;

	  REAL_VALUE_FROM_CONST_DOUBLE (d, x);
	  REAL_VALUE_TO_TARGET_SINGLE (d, l);
	  fprintf (file, "0x%08lx", l);
	  break;
	}
      /* Fall through.  Let output_addr_const deal with it.  */
    default:
      output_addr_const (file, x);
      break;
    }
}

bool
mxp_legitimate_address_p (enum machine_mode mode, rtx x, int strict_p)
{
  int mem_mode_size = (GET_MODE_SIZE (mode));
  /* 64 bit moves might need to be split.  */
  int min_mode_size
    = ((mem_mode_size == 8 && !reload_completed) ? 4 : mem_mode_size);

  if (REG_P (x) && (!strict_p || REGNO (x) <= LAST_SCALAR_REG))
    return 1;
  if (CONSTANT_P (x) && mem_mode_size >= 16)
    return 1;
  if (GET_CODE (x) == PLUS && REG_P (XEXP (x, 0))
      && (!strict_p || REGNO (XEXP (x, 0)) <= LAST_SCALAR_REG))
    x = XEXP (x, 1);
  if (mem_mode_size >= 16 && (CONST_INT_P (x) && (INTVAL (x) & 15) == 0))
    return 1;
  if (CONST_INT_P (x)
      && (INTVAL (x) & (mem_mode_size - 1)) == 0
      && INTVAL (x) >= (-1 << 11) * min_mode_size
      && INTVAL (x) < (1 << 11) * min_mode_size)
    return 1;
  /* We assume that all symbols are suitably positioned for i9+offset
     addressing in the data / bss sections.  */
  if (!TARGET_BIGDATA)
    {
      if (GET_CODE (x) == SYMBOL_REF)
	return 1;
      if (mem_mode_size == 4
	  && GET_CODE (x) == CONST && GET_CODE (XEXP (x, 0)) == PLUS
	  && GET_CODE (XEXP (XEXP (x, 0), 0)) == SYMBOL_REF
	  && CONST_INT_P (XEXP (XEXP (x, 0), 1))
	  && INTVAL (XEXP (XEXP (x, 0), 1)) <= 4)
	return 1;
    }
  return 0;
}

int
mxp_initial_elimination_offset (int from, int to)
{
  /* FIXME */
  return 0;
}

static rtx
frame_insn (rtx x)
{
  x = emit_insn (x);
  RTX_FRAME_RELATED_P (x) = 1;
  return x;
}

/* Helper function for find_save_lanes, called via note_stores.  */
static void
find_save_lanes_1 (rtx x, rtx pat ATTRIBUTE_UNUSED, void *data)
{
  unsigned char *buf = data;
  int r = true_regnum (x);
  int vecnum, lanes;

  if (r < 0)
    return;
  if (r <= LAST_SCALAR_REG)
    {
      vecnum = r >> 3;
      lanes = 1 << (r & 7);
    }
  else
    {
      lanes = (1 << (GET_MODE_SIZE (GET_MODE (x)) >> 1)) - 1;
      vecnum
	= (LANE0_REGNO (r) - (VR2_REG - 2 * REGS_PER_VREG)) / REGS_PER_VREG;
      lanes <<= VREG_LANE (r);
    }
  if ((unsigned)vecnum >= N_HARDWARE_VECREGS)
    gcc_assert (r >= ACC_REG);
  else
    buf[vecnum] |= lanes;
}

/* Each byte in BUF curresponds to a vector register.  For each register,
   generate a bitmask that shows which lanes need saving.  */
static void
find_save_lanes (unsigned char *buf)
{
  rtx insn;

  memset (buf, 0, N_HARDWARE_VECREGS);
  for (insn = get_last_insn_anywhere (); insn; insn = PREV_INSN (insn))
    note_stores (INSN_P (insn) ? PATTERN (insn) : insn,
		 find_save_lanes_1, buf);
}

/* A register is saved in SAVE_MODE of size SAVE_SIZE if there are bits in
   both the upper and the lower half that need saving.  In that case, return
   an rtx for the register to be saved.  */
static rtx
save_reg (int vecnum, int save_size, enum machine_mode save_mode,
	  unsigned char *buf)
{
  int lanes = buf[vecnum];
  int regno, size, i;
  rtx ret;

  if (!lanes)
    return NULL_RTX;
  if (vecnum < 2)
    {
      for (regno = vecnum * 8; regno < vecnum * 8 + 8; regno++)
	if (call_used_regs[regno])
	  lanes &= ~(1 << (regno & 7));
      if (!lanes)
	return NULL_RTX;
      /* FIXME: calculate optimal concatenations.  */
      /* N.B. we can restore the stack together with other scalar registers.  */
      if (save_size != 16)
	return NULL_RTX;
      ret = gen_rtx_PARALLEL (V4SImode, rtvec_alloc (4));
      for (i = 0; i < 4; i++)
	XVECEXP (ret, 0, i)
	  = gen_rtx_EXPR_LIST (VOIDmode,
			       gen_rtx_REG (SImode, vecnum * 8 +  i * 2),
			       GEN_INT (i * 2));
      return ret;
    }
  regno = vecnum * 2 + LAST_SCALAR_REG - 3;
  if (call_used_regs[regno])
    lanes &= -16;
  if (call_used_regs[regno+1])
    lanes &= 15;
  if (!lanes)
    return NULL_RTX;
  if ((lanes & -16) && (lanes & 15))
    size = 16;
  else
    {
      regno += (lanes > 15);
      size = lanes | (lanes >> 4);
      size = ((size & 3) + 1) << ((size & 8) != 0);
    }
  if (size != save_size)
    return NULL_RTX;
  return gen_rtx_REG (save_mode, regno);
}

void
mxp_expand_prologue (void)
{
  int frame_size = 0;
  int save_size;
  unsigned char *buf = cfun->machine->lanes_written;

  /* There are no interrupts, hence we may save registers before adjusting
     the stack pointer, and for leaf functions we can leave out the stack
     adjustment altogether.  */
  find_save_lanes (buf);
  for (save_size = 16; save_size >= 2; save_size >>= 1)
    {
      enum machine_mode save_mode
	= mode_for_size (BITS_PER_UNIT * save_size, MODE_INT, 0);
      rtx addr, reg;
      int regno;

      for (regno = 0; regno < N_HARDWARE_VECREGS; regno++)
	{
	  reg = save_reg (regno, save_size, save_mode, buf);
	  if (reg)
	    {
	      frame_size += save_size;
	      addr = gen_rtx_PLUS (HImode,
				   stack_pointer_rtx, GEN_INT (-frame_size));
	      emit_insn (gen_rtx_SET (VOIDmode,
				      gen_rtx_MEM (GET_MODE (reg), addr), reg));
	    }
	}
    }
  cfun->machine->saved_regs_size = frame_size;
  frame_size += get_frame_size ();
  /* Round up frame size to keep stack 128-bit aligned.  */
  frame_size = (frame_size + 15) & -16;
  cfun->machine->rounded_frame_size = frame_size;
  if (!current_function_is_leaf && frame_size)
    {
      /* Adjust stack pointer for register saves, local variables and
	 outgoing arguments.  */
      frame_insn (gen_add2_insn (stack_pointer_rtx, GEN_INT (-frame_size)));
      frame_size = 0;
    }
  if (frame_pointer_needed)
    frame_insn (gen_addhi3 (hard_frame_pointer_rtx,
			    stack_pointer_rtx, GEN_INT (-frame_size)));
}

void
mxp_expand_epilogue (void)
{
  int frame_size = get_frame_size ();
  int save_size;
  unsigned char *buf = cfun->machine->lanes_written;

  /* Round up frame size to keep stack 128-bit aligned.  */
  frame_size = (frame_size + 15) & -16;

  if (!current_function_is_leaf && frame_size)
    emit_insn (gen_add2_insn (stack_pointer_rtx,
	       GEN_INT (cfun->machine->rounded_frame_size)));
  /* FIXME: restore callee-saved registers.  */
  frame_size = cfun->machine->saved_regs_size;
  for (save_size = 2; save_size <= 16; save_size <<= 1)
    {
      enum machine_mode save_mode
	= mode_for_size (BITS_PER_UNIT * save_size, MODE_INT, 0);
      rtx addr, reg;
      int regno;

      for (regno = N_HARDWARE_VECREGS - 1; regno >= 0; regno--)
	{
	  reg = save_reg (regno, save_size, save_mode, buf);
	  if (reg)
	    {
	      frame_size -= save_size;
	      addr = gen_rtx_PLUS (HImode,
				   stack_pointer_rtx, GEN_INT (-frame_size));
	      emit_insn (gen_rtx_SET (VOIDmode,
				      reg, gen_rtx_MEM (GET_MODE (reg), addr)));
	    }
	}
    }
}

static void
mxp_init_libfuncs (void)
{
  set_optab_libfunc (sdiv_optab, HImode, "__divdi3");
  set_optab_libfunc (sdiv_optab, SImode, "__divdi3");
  set_optab_libfunc (sdiv_optab, DImode, "__divdi3");
}

static rtx
mxp_emit_comparison (enum rtx_code code)
{
  rtx tmp, tmp2, flags, result;
  enum machine_mode mode = GET_MODE (mxp_compare_op0);
  result = gen_reg_rtx (HImode);
  if ((mode != HImode || (code != GEU && code != LEU && code != LTU))
      && !reg_or_0_operand (mxp_compare_op1, VOIDmode))
    mxp_compare_op1 = force_reg (mode, mxp_compare_op1);
  tmp2 = gen_rtx_fmt_ee (code, HImode, mxp_compare_op0, mxp_compare_op1);

  switch (code)
    {
      case GE: case GT:
      case EQ: case NE: case LE: case LT:
	tmp = gen_rtx_SET (VOIDmode, result, gen_rtx_NEG (HImode, tmp2));
	tmp2 = gen_rtx_CLOBBER (VOIDmode, gen_rtx_SCRATCH (mode));
	tmp = gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2, tmp, tmp2));
	emit_insn (tmp);
	break;
      case GEU: case LEU: case GTU: case LTU:
	flags = gen_reg_rtx (CCImode);
	emit_insn ((mode == HImode ? gen_ucmphi : gen_ucmpsi)
		   (flags, mxp_compare_op0, mxp_compare_op1));
	emit_insn (gen_vtany (result, flags, tmp2));
	break;
      default: gcc_unreachable ();
    }

  return result;
}

void
mxp_emit_conditional_branch (rtx *operands, enum rtx_code code)
{
  rtx result = mxp_emit_comparison (code);

  emit_jump_insn (gen_branch_true (operands[0], result));
}

enum reg_class
mxp_secondary_reload (bool in_p, rtx x, enum reg_class class,
		      enum machine_mode mode,
		      secondary_reload_info *sri ATTRIBUTE_UNUSED)
{
  /* FIXME: cross-lane moves from the accumulator; WIP.  */
  if (GET_MODE_SIZE (mode) < 16
      && (GET_CODE (x) == REG && REGNO (x) < FIRST_PSEUDO_REGISTER)
      && (in_p
	  ? (class == Af0_REGS || class == A0f_REGS || class == Aff_REGS || class == ALL_REGS)
	  : LANE0_REGNO (REGNO (x)) == ACC_REG))
    {
      int x_regno = true_regnum (x);
      int mask = (GET_MODE_SIZE (mode) - 1) | 1;
      int x_lanes = mask << VREG_LANE (x_regno);
      int c_lanes = class_scalar_vec_lanes[class];
      int in_lanes  = in_p ? x_lanes : c_lanes;
      int out_lanes = in_p ? c_lanes : x_lanes;

      /* CLASS might contain both lower and higher lanes, so we should look
	 at the lanes from x_class to decide which half of the accumulator
	 is copied from.  */
      if (((in_lanes << 4 | in_lanes >> 4) & out_lanes)
	  || ((in_lanes | out_lanes) & ~(mask | mask * 16)))
	return       (!(VREG_LANE (REGNO (x)) & 3)
		      ? (((in_p ? in_lanes : out_lanes) < 16) ^ in_p
			 ? Vf0_REGS : V0f_REGS)
		      : !(out_lanes & 0xf0) ? Vf0_REGS
		      : !(out_lanes & 0x0f) ? V0f_REGS
		      : V01_REGS /* ??? */);
    }
  /* Cross-lanes clobbering the accumulator leads to too messy secondary
     reloads, thus we don't generate such reloads, but rather use clobber-free
     instruction sequences for the move patterns.  A peephole2 instroduces
     accumulator clobbers where this is profitable.  */
  return NO_REGS;
}

void
mxp_conditional_register_usage (void)
{
  int i;

  mxp_acc_classes = sbitmap_alloc (N_REG_CLASSES);
  for (i = 0; i < N_REG_CLASSES; i++)
    if (reg_classes_intersect_p (i, Aff_REGS))
      SET_BIT (mxp_acc_classes, i);
}

int
mxp_register_move_cost (enum machine_mode mode,
			enum reg_class from, enum reg_class to)
{
  /* Moving from the accumulator has higher latency.  */
  if (from == A0f_REGS || from == Af0_REGS || from == Aff_REGS)
    return 4;
  /* Cross-lane moves cost more and require a clobber, unless source or
     destination is a scalar register.  Even if the source is a scalar
     register, we need a clobber for a cross-lane move to a non-scalar
     register, so we must show a higher cost than for an ordinary move.  */
  if (GET_MODE_SIZE (mode) < 8
      && (class_scalar_vec_lanes[from]
	  & (class_vec_lanes[to] << 4 | class_vec_lanes[to] >> 4)))
    return TEST_BIT (mxp_acc_classes, (to)) ? 6 : 3;
  if (GET_MODE_SIZE (mode) < 4
      && class_vec_lanes[from] && class_vec_lanes[to]
      && ((class_vec_lanes[from] | class_vec_lanes[to])
	  & ~(((GET_MODE_SIZE (mode) - 1) | 1) * 0x11)))
    return TEST_BIT (mxp_acc_classes, (to)) ? 6 : 3;
  /* Tilt the balance.  */
  if ((class_vec_lanes[from]|class_vec_lanes[to]) & 0xf0)
    return 2;
  return 2;
}

int
mxp_memory_move_cost (enum machine_mode mode, enum reg_class class,
		      int in_p ATTRIBUTE_UNUSED)
{
  return (4
	  + (GET_MODE_SIZE (mode) == 8
	     && (class_scalar_vec_lanes[(class)] & 0xf0))
	  + memory_move_secondary_cost (mode, class, in_p));
}

int
peep2_regno_mode_dead_p (int ofs, int regno, enum machine_mode mode)
{
  int i;

  for (i = HARD_REGNO_NREGS (regno, mode); --i >= 0; )
    if (!peep2_regno_dead_p (ofs, regno + 1))
      return 0;
  return 1;
}

static bool
mxp_vector_mode_supported_p (enum machine_mode mode)
{
  return mov_optab->handlers[mode].insn_code != CODE_FOR_nothing;
}

#include "gt-mxp.h"

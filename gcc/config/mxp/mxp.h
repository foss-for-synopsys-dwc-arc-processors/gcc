#include "mxp-regset.h"

#define N_HARDWARE_VECREGS 32
/* Data is byte-addressed, using 16 bit addresses.  Code is word-addressed,
   using 16 bit addresses.  */
#define Pmode HImode

#define REGISTER_MOVE_COST(MODE, FROM, TO) \
  mxp_register_move_cost ((MODE), (FROM), (TO))

#define CONDITIONAL_REGISTER_USAGE mxp_conditional_register_usage ();

#define CASE_VECTOR_MODE HImode

/* For non-pointer types, use the sizes that arc.h has, to make structure
   passing simpler.
   32 bit int & long also makes sense because that's the widest add we can
   do cheaply.  */
#define SHORT_TYPE_SIZE		16
#define INT_TYPE_SIZE		(TARGET_INT16 ? 16 : 32)
#define LONG_TYPE_SIZE		32
#define LONG_LONG_TYPE_SIZE	64
#define FLOAT_TYPE_SIZE		32
#define DOUBLE_TYPE_SIZE	64
#define LONG_DOUBLE_TYPE_SIZE	64

/* Define this as 1 if `char' should by default be signed; else as 0.  */
#define DEFAULT_SIGNED_CHAR 0

#define SIZE_TYPE "short unsigned int"
#define PTRDIFF_TYPE "short int"
#define WCHAR_TYPE "short unsigned int"
#define WCHAR_TYPE_SIZE 16

#define UNITS_PER_SIMD_WORD(MODE) 16

/* ??? arc.h as BIGGEST_ALIGNMENT as 32, but we need 128 bit alignemnt for
   128 bit vector loads / stores.  */
#define BIGGEST_ALIGNMENT 128
#define MAX_FIXED_MODE_SIZE 128

#define STRICT_ALIGNMENT 1

#define PARM_BOUNDARY 16

/* ??? The actual stack boundary is 128 bits, but we want to save stack space
   when we can combine a frame of unaligned size with a save area of unaligned
   size.  */
#define STACK_BOUNDARY 16
#define PREFERRED_STACK_BOUNDARY 128

/* Functions are actually word-aligned, but the code is also word-addressed,
   and there is no point in trying to model all the details.  */
#define FUNCTION_BOUNDARY 8
#define FUNCTION_MODE QImode

#define BITS_BIG_ENDIAN 0
#define BYTES_BIG_ENDIAN 0
#define WORDS_BIG_ENDIAN 0

#define PROMOTE_MODE(MODE, UNSIGNEDP, TYPE) \
  if ((MODE) == QImode) \
    (MODE) = HImode; \

/* mxp_print_operand relies on LAST_SCALAR_REG to be odd.  */
#define FIRST_PARM_REG VR2_REG
#define N_PARM_REGS 8

/* In principle, we could describe all the registers in 16-bit granularity,
   and have a regclass for every lane combination.  Then addsi would have
   four alternatives, each equally valid.  However, to much choice of
   equally good, but incompatible reg classes would only confuse gcc.
   The other extreme would be to have only one lane combination per mode -
   then it would be easy to tun out of registers.  We use an intermediate
   approach: for each mode, the lane combination with lane 0 for its lowest
   bits is the main alternative, and the one with lane 4 for its lowest bits
   is the fallback alternative.  */
   
#define GENERAL_REGS S03V0f_REGS
#define SCALAR_REGS Sff_REGS

#define HARD_REGNO_MODE_OK(REGNO,MODE) \
  ((REGNO) >= VFLAGS_REG \
   ? (GET_MODE_CLASS ((MODE)) == MODE_VECTOR_CC \
     ? ((GET_MODE_SIZE (MODE) <= 8 && VREG_LANE (REGNO) == 4) \
	|| !VREG_LANE (REGNO)) \
     : GET_MODE_CLASS ((MODE)) == MODE_CC) \
   : (REGNO) == FP_REG || (REGNO) == AP_REG ? (MODE) == HImode \
   : (REGNO) >= ACC_REG \
   ? ((GET_MODE_SIZE (MODE) <= 8 && VREG_LANE (REGNO) == 4) \
      || !VREG_LANE (REGNO)) \
   : (REGNO) > LAST_SCALAR_REG \
   ? !(VREG_LANE (REGNO) & ((GET_MODE_SIZE (MODE) - 1) >> 1)) \
   : (GET_MODE_SIZE (MODE) <= 2 \
      || (GET_MODE_SIZE (MODE) <= 4 && !((REGNO) & 1))))
#define HARD_REGNO_NREGS(REGNO, MODE) ((GET_MODE_SIZE (MODE) + 1) >> 1)
#define CLASS_MAX_NREGS(CLASS, MODE) ((GET_MODE_SIZE (MODE) + 1) >> 1)

#define HARD_FRAME_POINTER_REGNUM 12
#define FRAME_POINTER_REGNUM FP_REG
#define ARG_POINTER_REGNUM AP_REG

#define FRAME_POINTER_REQUIRED  0

#define ELIMINABLE_REGS						\
{								\
  { HARD_FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM},		\
  { FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM},		\
  { FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM},		\
  { ARG_POINTER_REGNUM, STACK_POINTER_REGNUM},			\
  { ARG_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM},		\
}

#define CAN_ELIMINATE(FROM, TO) 1

#define CAN_DEBUG_WITHOUT_FP

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) \
  OFFSET = mxp_initial_elimination_offset ((FROM), (TO))

#define STARTING_FRAME_OFFSET  0

#define BASE_REG_CLASS SCALAR_REGS
#define INDEX_REG_CLASS NO_REGS
#define MAX_REGS_PER_ADDRESS 1

#ifdef REG_OK_STRICT
#define REG_OK_STRICT_P 1
#else
#define REG_OK_STRICT_P 0
#endif

#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, LABEL) \
  if (mxp_legitimate_address_p ((MODE), (X), REG_OK_STRICT_P)) goto LABEL

#define GO_IF_MODE_DEPENDENT_ADDRESS(X, LABEL) \
  if (!mxp_legitimate_address_p (TImode, (X), REG_OK_STRICT_P) \
      || !mxp_legitimate_address_p (QImode, (X), REG_OK_STRICT_P)) goto LABEL

#define REGNO_OK_FOR_BASE_P(NUM) \
  ((NUM) <= LAST_SCALAR_REG \
   || (!REG_OK_STRICT_P && (NUM) >= FIRST_PSEUDO_REGISTER))

#define REGNO_OK_FOR_INDEX_P(NUM) 0

#define PREFERRED_RELOAD_CLASS(X,CLASS) \
  (GET_CODE (X) == PLUS && CONST_INT_P (XEXP ((X), 1)) \
   ? ((CLASS) == S01V01_REGS ? S01_REGS \
      : (CLASS) == S10V10_REGS ? S10_REGS \
      : (CLASS) == S03V0f_REGS ? S03_REGS \
      : (CLASS) == S30Vf0_REGS ? S30_REGS \
      : (CLASS) == SffVff_REGS ? Sff_REGS \
      : (CLASS)) \
   : (CLASS))

#define REG_ALLOC_ORDER \
{ \
  /* Non-scalar general registers. */ \
  18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, \
  50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, \
  /* Other non-scalar vector registers.  */ \
  19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, \
  51, 53, 55, 57, 59, 61, 63, 65, 67, 69, 71, 73, 75, 77, \
  /* Scalar registers */ \
  1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, \
  0, 8, \
  /* Accumulator */ \
  78, 79, \
}

/* TImode references can address the entire 16 bit data address space using
   the data base pointer i9 and a 12-bit offset shifted left by 4.  */
#define CONSTANT_ADDRESS_P(X) 1

#define CUMULATIVE_ARGS int
#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,INDIRECT,N_NAMED_ARGS) \
  ((CUM) = 0)
#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED) \
  ((CUM)++)

#define TRAMPOLINE_SIZE 0
#define INITIALIZE_TRAMPOLINE(ADDR, FNADDR, STATIC_CHAIN) \
  sorry ("Trampolines are not implemented.\n");
/* Trampolines are not supported, yet we have to provide a definition for
   the static chain to avoid compiler crashes.  */
#define STATIC_CHAIN gen_rtx_MEM (HImode, GEN_INT (0))

#define FUNCTION_PROFILER(FILE, LABELNO) sorry ("Profiling not supported.\n");

#define MOVE_MAX 16

#define SLOW_BYTE_ACCESS 1

#define SHIFT_COUNT_TRUNCATED 1

#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC) 1

#define LEGITIMATE_CONSTANT_P(X) (GET_MODE_SIZE (GET_MODE (X)) <= 4)

#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED) \
  ((CUM) < N_PARM_REGS \
   ? gen_rtx_REG ((MODE), FIRST_PARM_REG + REGS_PER_VREG * (CUM)) : 0)

#define FUNCTION_ARG_REGNO_P(N) \
  ((N) >= FIRST_PARM_REG && (N) <= FIRST_PARM_REG + REGS_PER_VREG * N_PARM_REGS)

#define FIRST_PARM_OFFSET(FNDECL) 0

#define FUNCTION_VALUE_REGNO_P(N) ((N) == FIRST_PARM_REG)

#define LIBCALL_VALUE(MODE) gen_rtx_REG ((MODE), FIRST_PARM_REG)
/* Like LIBCALL_VALUE, except that we must
   change the mode like PROMOTE_MODE does.  */
#define FUNCTION_VALUE(VALTYPE, FUNC)					\
  gen_rtx_REG (								\
	   ((GET_MODE_CLASS (TYPE_MODE (VALTYPE)) == MODE_INT		\
	     && GET_MODE_SIZE (TYPE_MODE (VALTYPE)) < 2			\
	     && (TREE_CODE (VALTYPE) == INTEGER_TYPE			\
		 || TREE_CODE (VALTYPE) == ENUMERAL_TYPE		\
		 || TREE_CODE (VALTYPE) == BOOLEAN_TYPE			\
		 || TREE_CODE (VALTYPE) == OFFSET_TYPE))		\
	    ? HImode : TYPE_MODE (VALTYPE)),				\
	   FIRST_PARM_REG)

#define ACCUMULATE_OUTGOING_ARGS 1

#define RETURN_POPS_ARGS(DECL, FUNTYPE, SIZE) 0

#define INIT_EXPANDERS (init_machine_status = mxp_init_machine_status)

#define ASM_APP_ON "#APP\n"
#define ASM_APP_OFF "#NO_APP\n"

#define ASM_OUTPUT_ALIGN(FILE,LOG)	\
  if ((LOG) != 0)			\
    fprintf ((FILE), "\t.balign %d\n", 1 << (LOG))

/* Globalizing directive for a label.  */
#define GLOBAL_ASM_OP "\t.global\t"

#define TEXT_SECTION_ASM_OP "\t.text"
#define DATA_SECTION_ASM_OP "\t.data"

#define PRINT_OPERAND(STREAM, X, CODE) \
  mxp_print_operand ((STREAM), (X), (CODE))

#define PRINT_OPERAND_PUNCT_VALID_P(CHAR) \
  ((CHAR) == '#')

#define PRINT_OPERAND_ADDRESS(FILE, ADDR) gcc_unreachable ()

#define MODES_TIEABLE_P(MODE1, MODE2) \
  (MODE1 != Pmode && MODE2 != Pmode \
   && GET_MODE_SIZE (MODE1) <= UNITS_PER_WORD \
   && GET_MODE_SIZE (MODE2) <= UNITS_PER_WORD)

#define TARGET_CPU_CPP_BUILTINS() do	\
{					\
  builtin_define ("__mxp__");		\
  builtin_define ("__LITTLE_ENDIAN__");	\
} while (0)

#define MEMORY_MOVE_COST(M,C,I) mxp_memory_move_cost ((M), (C), (I))

#define MODE_NLANES(MODE) ((GET_MODE_SIZE (MODE) + 1) >> 1)
#define VREG_LANE_MASK(REG) \
(((1 << MODE_NLANES (GET_MODE (REG))) - 1) << VREG_LANE (REGNO ((REG))))

/* ??? We add accumulator clobbers for cross-lane moves in peep2 where
   possible.  Without the accumulator clobbers, cross-lane moves are more
   expensive.  Therefore, we don't want regrename to change lanes of move
   instruction without accumulator clobbers.  Unfortunately, regrename cannot
   be controlled in a way that takes the instructions involved into account;
   all we can do is prevent a lane change.  */
#define HARD_REGNO_RENAME_OK(REG,NEW_REG) \
  (VREG_LANE (REG) == VREG_LANE (NEW_REG))

/* This is how to output a reference to a symbol_ref / label_ref as
   (part of) an operand.  To disambiguate from register names like
   a1 / a2 / status etc, symbols are preceded by '@'.  */
#define ASM_OUTPUT_SYMBOL_REF(FILE,SYM) \
  ASM_OUTPUT_LABEL_REF ((FILE), XSTR ((SYM), 0))
#define ASM_OUTPUT_LABEL_REF(FILE,STR)                  \
  do                                                    \
    {                                                   \
      fputc ('@', file);                                \
      assemble_name ((FILE), (STR));                    \
    }                                                   \
  while (0)

extern rtx mxp_compare_op0, mxp_compare_op1;
extern unsigned char class_vec_lanes[], class_scalar_vec_lanes[];
extern struct simple_bitmap_def *mxp_acc_classes;

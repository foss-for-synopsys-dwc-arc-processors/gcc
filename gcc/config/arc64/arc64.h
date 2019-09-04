#ifndef GCC_ARC64_H
#define GCC_ARC64_H

/* Bits are always numbered from the LSBit.  */
#define BITS_BIG_ENDIAN 0

#define UNITS_PER_WORD  8

/* Boundaries.  */
#define PARM_BOUNDARY		64
#define STACK_BOUNDARY		128
#define FUNCTION_BOUNDARY	32
#define EMPTY_FIELD_BOUNDARY	32
#define STRUCTURE_SIZE_BOUNDARY 8

/* Look at the fundamental type that is used for a bit-field and use 
   that to impose alignment on the enclosing structure.
   struct s {int a:8}; should have same alignment as "int", not "char".  */
#define	PCC_BITFIELD_TYPE_MATTERS	1

/* Alignments.  */
#define FASTEST_ALIGNMENT       BITS_PER_WORD
#define BIGGEST_ALIGNMENT	128
#define ARC_EXPAND_ALIGNMENT(COND, EXP, ALIGN)				\
  (((COND) && ((ALIGN) < FASTEST_ALIGNMENT)				\
    && (TREE_CODE (EXP) == ARRAY_TYPE					\
	|| TREE_CODE (EXP) == UNION_TYPE				\
	|| TREE_CODE (EXP) == RECORD_TYPE)) ? FASTEST_ALIGNMENT : (ALIGN))

/* Align global data.  */
#define DATA_ALIGNMENT(EXP, ALIGN)			\
  ARC_EXPAND_ALIGNMENT (!optimize_size, EXP, ALIGN)

/* Similarly, make sure that objects on the stack are sensibly aligned.  */
#define LOCAL_ALIGNMENT(EXP, ALIGN)				\
  ARC_EXPAND_ALIGNMENT (!flag_conserve_stack, EXP, ALIGN)
#define STRICT_ALIGNMENT	TARGET_STRICT_ALIGN

/* Layout of Source Language Data Types */
#define SHORT_TYPE_SIZE         16
#define INT_TYPE_SIZE           32
#define LONG_TYPE_SIZE          64
#define LONG_LONG_TYPE_SIZE     64
#define POINTER_SIZE            64

#define FLOAT_TYPE_SIZE		32
#define DOUBLE_TYPE_SIZE	64
#define LONG_DOUBLE_TYPE_SIZE	128

/* Defined by ABI.  */
#define WCHAR_TYPE "unsigned int"
#define WCHAR_TYPE_SIZE 32

#define DEFAULT_SIGNED_CHAR 0

#define SIZE_TYPE       "long unsigned int"
#define PTRDIFF_TYPE	"long int"

/* Specify the machine mode that the hardware addresses have.
   After generation of rtl, the compiler makes no further distinction
   between pointers and any other objects of this machine mode.  */
#define Pmode         DImode

/* Mode of a function address in a call instruction (for indexing purposes).  */
#define FUNCTION_MODE	Pmode

/* Register usage:
   R0-R3    Parameter/result registers
   R4-R7    Parameter registers
   R8-R13   Temporary registers
   R14-R26  Callee-saved registers
   R27      FP (frame pointer)
   R28      SP (stack pointer)
   R29      ILINK (Interrupt link register)
   R30      GP/TP Global pointer, also is used as thread pointer;
            otherwise can be used  as a temporary register.
   R31      BLINK (return register)
   R32-R57  Extension registers
   R58      ACCL (accumulator low)
   R59      ACCH (accumulator high)
   --- Special registers ---
   R60      sign-extended 32-bit indicator
   R61      Reserved
   R62      zero extended 32-bit immediate indicator
   R63      PCL (program counter)

   -- Fake registers --
   AP       Argument pointer
   SFP      Soft frame pointer
   CC       Status register.
 */

#define FIXED_REGISTERS							\
  {									\
   0, 0, 0, 0,   0, 0, 0, 0,  /* R0 - R7 */				\
   0, 0, 0, 0,   0, 0, 0, 0,  /* R8 - R15 */				\
   0, 0, 0, 0,   0, 0, 0, 0,  /* R16 - R23 */				\
   0, 0, 0, 0,   1, 1, 0, 1,  /* R24 - R26, FP, SP, ILINK, R30, BLINK */ \
									\
   1, 1, 1, 1,   1, 1, 1, 1,  /* R32 - R39 */				\
   1, 1, 1, 1,   1, 1, 1, 1,  /* R40 - R47 */				\
   1, 1, 1, 1,   1, 1, 1, 1,  /* R48 - R55 */				\
   1, 1, 0, 0,   1, 1, 1, 1,  /* R56, R57, ACCL, ACCH, Specials */	\
   1, 1, 1,                   /* AP, SFP, CC */				\
  }

#define CALL_USED_REGISTERS							\
  {									\
   1, 1, 1, 1,   1, 1, 1, 1,  /* R0 - R7 */				\
   1, 1, 1, 1,   1, 1, 0, 0,  /* R8 - R15 */				\
   0, 0, 0, 0,   0, 0, 0, 0,  /* R16 - R23 */				\
   0, 0, 0, 0,   1, 1, 1, 1,  /* R24 - R26, FP, SP, ILINK, R30, BLINK */ \
									\
   1, 1, 1, 1,   1, 1, 1, 1,  /* R32 - R39 */				\
   1, 1, 1, 1,   1, 1, 1, 1,  /* R40 - R47 */				\
   1, 1, 1, 1,   1, 1, 1, 1,  /* R48 - R55 */				\
   1, 1, 1, 1,   1, 1, 1, 1,  /* R56, R57, ACCL, ACCH, Specials */	\
   1, 1, 1,                   /* AP, SFP, CC */				\
  }

#define REGISTER_NAMES							\
  {									\
   "r0",  "r1",  "r2",  "r3",     "r4",   "r5",    "r6",  "r7",		\
   "r8",  "r9",  "r10", "r11",    "r12",  "r13",   "r14", "r15",	\
   "r16", "r17", "r18", "r19",    "r20",  "r21",   "r22", "r23",	\
   "r24", "r25", "r26", "r27",    "sp",   "ilink", "r30", "blink",	\
   "r32", "r33", "r34", "r35",    "r36",  "r37",   "r38", "r39",	\
   "r40", "r41", "r42", "r43",    "r44",  "r45",   "r46", "r47",	\
   "r48", "r49", "r50", "r51",    "r52",  "r53",   "r54", "r55",	\
   "r56", "r57", "r58", "r59",    "ximm", "rez",   "limm", "pcl",	\
   "ap", "sfp", "cc",							\
  }

#define ADDITIONAL_REGISTER_NAMES		\
  {						\
   { "fp", 27 },				\
   { "gp", 30 },				\
   { "accl", 58 },				\
   { "acch", 59 },				\
  }

#define EPILOGUE_USES(REGNO) (arc64_epilogue_uses (REGNO))

/* EXIT_IGNORE_STACK should be nonzero if, when returning from a function,
   the stack pointer does not matter.  This is only true if the function
   uses alloca.  */
#define EXIT_IGNORE_STACK (cfun->calls_alloca)

#define STATIC_CHAIN_REGNUM		R14_REGNUM
#define HARD_FRAME_POINTER_REGNUM	R27_REGNUM
#define FRAME_POINTER_REGNUM		SFP_REGNUM
#define STACK_POINTER_REGNUM		SP_REGNUM
#define ARG_POINTER_REGNUM		AP_REGNUM
#define FIRST_PSEUDO_REGISTER		(CC_REGNUM + 1)

enum reg_class
{
   NO_REGS,
   AC16_REGS,
   SIBCALL_REGS,
   GENERAL_REGS,
   ALL_REGS,
   LIM_REG_CLASSES
};

#define N_REG_CLASSES	((int) LIM_REG_CLASSES)

#define REG_CLASS_NAMES				\
  {						\
    "NO_REGS",					\
    "AC16_REGS",				\
    "SIBCALL_REGS",				\
    "GENERAL_REGS",				\
    "ALL_REGS"					\
   }

#define REG_CLASS_CONTENTS					\
  {								\
    { 0x00000000, 0x00000000, 0x00000000 }, /* NO_REGS */	\
    { 0x0000f00f, 0x00000000, 0x00000000 }, /* AC16_REGS */	\
    { 0x00001fff, 0x00000000, 0x00000000 }, /* SIBCALL_REGS */	\
    { 0x4fffffff, 0x0fffffff, 0x00000000 }, /* GENERAL_REGS */	\
    { 0xffffffff, 0xffffffff, 0x00000007 }, /* ALL_REGS */	\
   }

/* A C expression whose value is a register class containing hard
   register REGNO.  In general there is more that one such class;
   choose a class which is "minimal", meaning that no smaller class
   also contains the register.  */

#define REGNO_REG_CLASS(REGNO) arc64_regno_to_regclass[ (REGNO) ]

/* The class value for valid base registers. A base register is one used in
   an address which is the register value plus a displacement.  */

#define BASE_REG_CLASS GENERAL_REGS
#define INDEX_REG_CLASS GENERAL_REGS

/* Definitions for register eliminations.

   This is an array of structures.  Each structure initializes one pair
   of eliminable registers.  The "from" register number is given first,
   followed by "to".  Eliminations of the same "from" register are listed
   in order of preference.

   We have two registers that can be eliminated on the ARC.  First, the
   argument pointer register can always be eliminated in favor of the stack
   pointer register or frame pointer register.  Secondly, the frame pointer
   register can often be eliminated in favor of the stack pointer register.
*/

#define ELIMINABLE_REGS						\
  {								\
    { ARG_POINTER_REGNUM,   STACK_POINTER_REGNUM },		\
    { ARG_POINTER_REGNUM,   HARD_FRAME_POINTER_REGNUM }, 	\
    { FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM },      	\
    { FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM }  	\
  }

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET)		\
  (OFFSET) = arc64_initial_elimination_offset (FROM, TO)

/* RTL generation support.  */
#define INIT_EXPANDERS arc64_init_expanders ()

/* Stack layout; function entry, exit and calling.  */
#define STACK_GROWS_DOWNWARD	1

#define FRAME_GROWS_DOWNWARD	1

#define ACCUMULATE_OUTGOING_ARGS	1

#define FIRST_PARM_OFFSET(FNDECL) 0

/* Define how to find the value returned by a library function
   assuming the value has mode MODE.  */
#define LIBCALL_VALUE(MODE) gen_rtx_REG (MODE, R0_REGNUM)

/* Tell GCC to use RETURN_IN_MEMORY.  */
#define DEFAULT_PCC_STRUCT_RETURN 0

/* WORD_REGISTER_OPERATIONS does not hold for arc64.  The assigned
   word_mode is DImode but operations narrower than SImode behave as
   32-bit operations if using the W-form of the registers rather than
   as word_mode (64-bit) operations as WORD_REGISTER_OPERATIONS
   expects.  */
#define WORD_REGISTER_OPERATIONS 0

/* Define if loading from memory in MODE, an integral mode narrower than
   BITS_PER_WORD will either zero-extend or sign-extend.  The value of this
   macro should be the code that says which one of the two operations is
   implicitly done, or UNKNOWN if none.  */
#define LOAD_EXTEND_OP(MODE) ZERO_EXTEND

#define SLOW_BYTE_ACCESS 0

/* Conditional info.  */
#define SELECT_CC_MODE(OP, X, Y) arc64_select_cc_mode (OP, X, Y)

#define REVERSIBLE_CC_MODE(MODE) 1

/* Returning.  */
#define INCOMING_RETURN_ADDR_RTX gen_rtx_REG (Pmode, BLINK_REGNUM)

#define RETURN_ADDR_RTX arc64_return_addr

/* Define this to be nonzero if shift instructions ignore all but the
   low-order few bits.  */
#define SHIFT_COUNT_TRUNCATED 1

#define REGMODE_NATURAL_SIZE(MODE) arc64_regmode_natural_size (MODE)

/* Function argument passing.  */

/* Define a data type for recording info about an argument list during
   the scan of that argument list.  This data type should hold all
   necessary information about the function itself and about the args
   processed so far, enough to enable macros such as FUNCTION_ARG to
   determine where the next arg should go.  */
#define CUMULATIVE_ARGS int

/* Maximum bytes moved by a single instruction (load/store pair).  */
#define MOVE_MAX (UNITS_PER_WORD * 2)

#ifndef USED_FOR_TARGET
extern const enum reg_class arc64_regno_to_regclass[];
#endif

#define UNSIGNED_INT3(X) ((unsigned) (X) < 0x8)
#define UNSIGNED_INT5(X) ((unsigned) (X) < 0x20)
#define UNSIGNED_INT6(X) ((unsigned) (X) < 0x40)
#define UNSIGNED_INT7(X) ((unsigned) (X) < 0x80)
#define UNSIGNED_INT8(X) ((unsigned) (X) < 0x100)
#define UNSIGNED_INT12(X) ((unsigned) (X) < 0x800)
#define UNSIGNED_INT16(X) ((unsigned) (X) < 0x10000)

#define SIGNED_INT12(X) ((unsigned) ((X) + 0x800) < 0x1000)
#define SIGNED_INT16(X) ((unsigned) ((X) + 0x8000) < 0x10000)


#endif /* GCC_ARC64_H */

/* Definitions of target machine for GNU compiler, Synopsys DesignWare ARC cpu.
   Copyright (C) 1994, 1995, 1997, 1998, 2007-2013
   Free Software Foundation, Inc.

   Sources derived from work done by Sankhya Technologies (www.sankhya.com) on
   behalf of Synopsys Inc.

   Position Independent Code support added,Code cleaned up,
   Comments and Support For ARC700 instructions added by
   Saurabh Verma (saurabh.verma@codito.com)
   Ramana Radhakrishnan(ramana.radhakrishnan@codito.com)

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

#ifndef GCC_ARC_H
#define GCC_ARC_H

#include <stdbool.h>

/* Things to do:

   - incscc, decscc?

*/

#ifndef TARGET_CPU_DEFAULT
#define TARGET_CPU_DEFAULT	PROCESSOR_arc700
#endif

#define SYMBOL_FLAG_SHORT_CALL	(SYMBOL_FLAG_MACH_DEP << 0)
#define SYMBOL_FLAG_MEDIUM_CALL	(SYMBOL_FLAG_MACH_DEP << 1)
#define SYMBOL_FLAG_LONG_CALL	(SYMBOL_FLAG_MACH_DEP << 2)
#define SYMBOL_FLAG_TLS_S9	(SYMBOL_FLAG_MACH_DEP << 3)

/* Check if this symbol has a long_call attribute in its declaration */
#define SYMBOL_REF_LONG_CALL_P(X)	\
	((SYMBOL_REF_FLAGS (X) & SYMBOL_FLAG_LONG_CALL) != 0)

/* Check if this symbol has a medium_call attribute in its declaration */
#define SYMBOL_REF_MEDIUM_CALL_P(X)	\
	((SYMBOL_REF_FLAGS (X) & SYMBOL_FLAG_MEDIUM_CALL) != 0)

/* Check if this symbol has a short_call attribute in its declaration */
#define SYMBOL_REF_SHORT_CALL_P(X)	\
	((SYMBOL_REF_FLAGS (X) & SYMBOL_FLAG_SHORT_CALL) != 0)

/* Check if this symbol has a tls9 attribute in its declaration */
#define SYMBOL_REF_TLS_S9_P(X)	\
	(TARGET_TLS9 || (SYMBOL_REF_FLAGS (X) & SYMBOL_FLAG_TLS_S9) != 0)

#undef ASM_SPEC
#undef LINK_SPEC
#undef STARTFILE_SPEC
#undef ENDFILE_SPEC
#undef SIZE_TYPE
#undef PTRDIFF_TYPE
#undef WCHAR_TYPE
#undef WCHAR_TYPE_SIZE
#undef ASM_APP_ON
#undef ASM_APP_OFF
#undef CC1_SPEC

/* Names to predefine in the preprocessor for this target machine.  */
#define TARGET_CPU_CPP_BUILTINS() arc_cpu_cpp_builtins (pfile)

#if DEFAULT_LIBC == LIBC_UCLIBC

#define TARGET_OS_CPP_BUILTINS() \
  do \
    { \
      GNU_USER_TARGET_OS_CPP_BUILTINS (); \
    } \
  while (0)
#endif

/* Match the macros used in the assembler.  */
#define CPP_SPEC "\
%{msimd:-D__Xsimd} %{mswap:-D__Xswap} \
%{mmin-max:-D__Xmin_max} %{mEA:-D__Xea} \
%{mmac-d16:-D__Xxmac_d16} %{mmac-24:-D__Xxmac_24} \
%{mdsp-packa:-D__Xdsp_packa} %{mcrc:-D__Xcrc} %{mdvbf:-D__Xdvbf} \
%{mtelephony:-D__Xtelephony} %{mxy:-D__Xxy} %{mmul64: -D__Xmult32} \
%{mlock:-D__Xlock} %{mswape:-D__Xswape} %{mrtsc:-D__Xrtsc} \
%{mfpu=fpuda:-D__Xfpuda} \
"

#define CC1_SPEC "\
%{EB:%{EL:%emay not use both -EB and -EL}} \
%{EB:-mbig-endian} %{EL:-mlittle-endian} \
"

extern const char *arc_cpu_to_as (int argc, const char **argv);

#define EXTRA_SPEC_FUNCTIONS			\
  { "cpu_to_as", arc_cpu_to_as },

#define ASM_SPEC "%{mbig-endian|EB:-EB} %{EL} \
%:cpu_to_as(%{mcpu=*:%*})						\
%{mbarrel-shifter} %{mmul64} %{mmul32x16:-mdsp-packa} %{mnorm}		\
%{mswap} %{mEA} %{mmin-max} %{mspfp*} %{mdpfp*}				\
%{msimd} %{mfpu=fpuda:-mfpuda} %{mfpu=fpuda_all:-mfpuda}		\
%{mfpu=fpuda_fma:-mfpuda} %{mfpu=fpuda_div:-mfpuda}			\
%{mmac-d16} %{mmac-24} %{mdsp-packa} %{mcrc} %{mdvbf} %{mtelephony} %{mxy} \
%{matomic:-mlock} %{mswape} %{mlock} %{mrtsc}				\
"

#define OPTION_DEFAULT_SPECS						\
  {"cpu", "%{!mcpu=*:%{!mARC*:%{!marc*:%{!mA7:%{!mA6:%{!mav*:"		\
      "%{!mEM:%{!mHS:-mcpu=%(VALUE)}}}}}}}}" },				\
  {"abi", "%{!mabi=*:-mabi=%(VALUE)}" }

#if DEFAULT_LIBC == LIBC_UCLIBC
/* Note that the default is to link against dynamic libraries, if they are
   available.  Override with -static.  */
#define LINK_SPEC "%{h*} \
		   %{static:-Bstatic} \
		   %{symbolic:-Bsymbolic} \
		   %{rdynamic:-export-dynamic}\
		   -dynamic-linker /lib/ld-uClibc.so.0 \
		   -X %{mbig-endian:-EB} \
		   %{EB} %{EL} \
		   %{marclinux*} \
		   %{!marclinux*: %{pg|p|profile:-marclinux_prof;: -marclinux}} \
		   %{!z:-z max-page-size=0x2000 -z common-page-size=0x2000} \
		   %{shared:-shared}"

#else
#define LINK_SPEC "%{mbig-endian:-EB} %{EB} %{EL}\
  %{pg|p:-marcelf_prof;mA7|mARC700|mcpu=arc700|mcpu=ARC700: -marcelf}"
#endif

#if DEFAULT_LIBC != LIBC_UCLIBC
#define ARC_TLS_EXTRA_START_SPEC "crttls_r25.o%s"

#define EXTRA_SPECS \
  { "arc_tls_extra_start_spec", ARC_TLS_EXTRA_START_SPEC }, \

#define STARTFILE_SPEC "%{!shared:crt0.o%s} crti%O%s %{pg|p:crtg.o%s} %(arc_tls_extra_start_spec) crtbegin.o%s"
#else
#define STARTFILE_SPEC \
  LINUX_OR_ANDROID_LD (GNU_USER_TARGET_STARTFILE_SPEC, ANDROID_STARTFILE_SPEC)
#endif

#if DEFAULT_LIBC != LIBC_UCLIBC
#define ENDFILE_SPEC "%{pg|p:crtgend.o%s} crtend.o%s crtn%O%s"
#else
#define ENDFILE_SPEC \
  LINUX_OR_ANDROID_LD (GNU_USER_TARGET_ENDFILE_SPEC, ANDROID_ENDFILE_SPEC)
#endif

#if DEFAULT_LIBC == LIBC_UCLIBC
#undef LIB_SPEC
#define LIB_SPEC  \
  "%{pthread:-lpthread} \
   %{shared:-lc} \
   %{!shared:%{pg|p|profile:-lgmon -u profil --defsym __profil=profil} -lc}"
#define TARGET_ASM_FILE_END file_end_indicate_exec_stack
#else
#undef LIB_SPEC
/* -lc_p not present for arc-elf32-* : ashwin */
#define LIB_SPEC "%{!shared:%{g*:-lg} %{pg|p:-lgmon} -lc}"
#endif

#ifndef DRIVER_ENDIAN_SELF_SPECS
#define DRIVER_ENDIAN_SELF_SPECS ""
#endif
#ifndef TARGET_SDATA_DEFAULT
#define TARGET_SDATA_DEFAULT 1
#endif
#ifndef TARGET_MMEDIUM_CALLS_DEFAULT
#define TARGET_MMEDIUM_CALLS_DEFAULT 0
#endif

#define DRIVER_SELF_SPECS DRIVER_ENDIAN_SELF_SPECS			\
  "%{mARC600|mA6|marc600: -mcpu=arc600 %<mARC600 %<mA6 %<marc600}"	\
  "%{mARC601|marc601: -mcpu=arc601 %<mARC601 %<marc601}"		\
  "%{mARC700|mA7|marc700: -mcpu=arc700 %<mARC700 %<mA7 %<marc700}"	\
  "%{mav2em|mARCv2EM|mEM|marcem: -mcpu=arcem %<mav2em %<mARCv2EM %<mEM %<marcem}" \
  "%{mav2hs|mARCv2HS|mHS|marchs: -mcpu=archs %<mav2hs %<mARCv2HS %<mHS %<marchs}"

/* Run-time compilation parameters selecting different hardware subsets.  */

#define TARGET_MIXED_CODE (TARGET_MIXED_CODE_SET)

#define TARGET_SPFP (TARGET_SPFP_FAST_SET || TARGET_SPFP_COMPACT_SET)
#define TARGET_DPFP (TARGET_DPFP_FAST_SET || TARGET_DPFP_COMPACT_SET || (arc_fpu_build & FPX_DP))

#define SUBTARGET_SWITCHES

/* Instruction set characteristics.
   These are internal macros, set by the appropriate -m option.  */

/* Non-zero means the cpu supports norm instruction.  This flag is set by
   default for A7, and only for pre A7 cores when -mnorm is given.  */
#define TARGET_NORM (TARGET_ARC700 || TARGET_NORM_SET || TARGET_HS)
/* Indicate if an optimized floating point emulation library is
   available.  We need a barrrel shifter, NORM and a multiplier.
   However, a customer may choose either to disable one of them when
   all of them are on, or to enable all of them when they are off,
   resulting in a missmatch between what the compiler outputs and the
   libgcc support routines.  Hence, the optimized floating point
   emulation library is available for ARC700 only as it always has all
   the requires options on.  */
#define TARGET_OPTFPE	(TARGET_ARC700 || TARGET_FPX_QUARK)

/* Non-zero means the cpu supports swap instruction.  This flag is set by
   default for A7, and only for pre A7 cores when -mswap is given.  */
#define TARGET_SWAP (TARGET_ARC700 || TARGET_SWAP_SET)

/* Provide some macros for size / scheduling features of the ARC700, so
   that we can pick & choose features if we get a new cpu family member.  */

/* Should we try to unalign likely taken branches without a delay slot.  */
#define TARGET_UNALIGN_BRANCH (TARGET_ARC700 && !optimize_size)

/* Should we upsize short delayed branches with a short delay insn?  */
#define TARGET_UPSIZE_DBR (TARGET_ARC700 && !optimize_size)

/* Should we add padding before a return insn to avoid mispredict?  */
#define TARGET_PAD_RETURN (TARGET_ARC700 && !optimize_size)

/* For an anulled-true delay slot insn for a delayed branch, should we only
   use conditional execution?  */
#define TARGET_AT_DBR_CONDEXEC  (!TARGET_ARC700 && !TARGET_V2)

extern bool arc_arcem;
extern bool arc_archs;
extern bool arc_arc700;
extern bool arc_arc600;
extern bool arc_arc601;

#define TARGET_ARC600 (arc_arc600)
#define TARGET_ARC601 (arc_arc601)
#define TARGET_ARC700 (arc_arc700)
#define TARGET_EM (arc_arcem)
#define TARGET_HS (arc_archs)
#define TARGET_V2 (TARGET_EM || TARGET_HS)

#ifdef ARC_MULTILIB_CPU_DEFAULT
# ifndef MULTILIB_DEFAULTS
#  define MULTILIB_DEFAULTS { "mcpu=" ARC_MULTILIB_CPU_DEFAULT }
# endif
#endif

/* Target machine storage layout.  */

/* We want zero_extract to mean the same
   no matter what the byte endianness is.  */
#define BITS_BIG_ENDIAN 0

/* Define this if most significant byte of a word is the lowest numbered.  */
#define BYTES_BIG_ENDIAN (TARGET_BIG_ENDIAN)

/* Define this if most significant word of a multiword number is the lowest
   numbered.  */
#define WORDS_BIG_ENDIAN (TARGET_BIG_ENDIAN)

/* Number of bits in an addressable storage unit.  */
#define BITS_PER_UNIT 8

/* Width in bits of a "word", which is the contents of a machine register.
   Note that this is not necessarily the width of data type `int';
   if using 16-bit ints on a 68000, this would still be 32.
   But on a machine with 16-bit registers, this would be 16.  */
#define BITS_PER_WORD 32

/* Width of a word, in units (bytes).  */
#define UNITS_PER_WORD 4

/* Define this macro if it is advisable to hold scalars in registers
   in a wider mode than that declared by the program.  In such cases,
   the value is constrained to be within the bounds of the declared
   type, but kept valid in the wider mode.  The signedness of the
   extension may differ from that of the type.  */
#define PROMOTE_MODE(MODE,UNSIGNEDP,TYPE) \
if (GET_MODE_CLASS (MODE) == MODE_INT		\
    && GET_MODE_SIZE (MODE) < UNITS_PER_WORD)	\
{						\
  (MODE) = SImode;				\
}

/* Width in bits of a pointer.
   See also the macro `Pmode' defined below.  */
#define POINTER_SIZE 32

/* Allocation boundary (in *bits*) for storing arguments in argument list.  */
#define PARM_BOUNDARY 32

/* Boundary (in *bits*) on which stack pointer should be aligned.  */
/* TOCHECK: Changed from 64 to 32 */
#define STACK_BOUNDARY 32

/* ALIGN FRAMES on word boundaries.  */
#define ARC_STACK_ALIGN(LOC) \
  (((LOC) + STACK_BOUNDARY / BITS_PER_UNIT - 1) & -STACK_BOUNDARY/BITS_PER_UNIT)

/* Allocation boundary (in *bits*) for the code of a function.  */
#define FUNCTION_BOUNDARY 32

/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY 32

/* Every structure's size must be a multiple of this.  */
#define STRUCTURE_SIZE_BOUNDARY 8

/* A bitfield declared as `int' forces `int' alignment for the struct.  */
#define PCC_BITFIELD_TYPE_MATTERS 1

/* An expression for the alignment of a structure field FIELD if the
   alignment computed in the usual way (including applying of
   `BIGGEST_ALIGNMENT' and `BIGGEST_FIELD_ALIGNMENT' to the
   alignment) is COMPUTED.  It overrides alignment only if the field
   alignment has not been set by the `__attribute__ ((aligned (N)))'
   construct.
*/

#define ADJUST_FIELD_ALIGN(FIELD, COMPUTED) \
(TYPE_MODE (strip_array_types (TREE_TYPE (FIELD))) == DFmode \
 ? MIN ((COMPUTED), 32) : (COMPUTED))



/* No data type wants to be aligned rounder than this.  */
/* This is bigger than currently necessary for the ARC.  If 8 byte floats are
   ever added it's not clear whether they'll need such alignment or not.  For
   now we assume they will.  We can always relax it if necessary but the
   reverse isn't true.  */
/* TOCHECK: Changed from 64 to 32 */
#define BIGGEST_ALIGNMENT 32

/* The best alignment to use in cases where we have a choice.  */
#define FASTEST_ALIGNMENT 32

/* Make strings word-aligned so strcpy from constants will be faster.  */
#define CONSTANT_ALIGNMENT(EXP, ALIGN)  \
  ((TREE_CODE (EXP) == STRING_CST	\
    && (ALIGN) < FASTEST_ALIGNMENT)	\
   ? FASTEST_ALIGNMENT : (ALIGN))


/* Make arrays of chars word-aligned for the same reasons.  */
#define LOCAL_ALIGNMENT(TYPE, ALIGN)             \
  (TREE_CODE (TYPE) == ARRAY_TYPE               \
   && TYPE_MODE (TREE_TYPE (TYPE)) == QImode    \
   && (ALIGN) < FASTEST_ALIGNMENT ? FASTEST_ALIGNMENT : (ALIGN))

#define DATA_ALIGNMENT(TYPE, ALIGN)		\
  (TREE_CODE (TYPE) == ARRAY_TYPE		\
   && TYPE_MODE (TREE_TYPE (TYPE)) == QImode	\
   && arc_size_opt_level < 3			\
   && (ALIGN) < FASTEST_ALIGNMENT ? FASTEST_ALIGNMENT : (ALIGN))

/* Set this nonzero if move instructions will actually fail to work
   when given unaligned data.  */
/* On the ARC the lower address bits are masked to 0 as necessary.  The chip
   won't croak when given an unaligned address, but the insn will still fail
   to produce the correct result.  */
#define STRICT_ALIGNMENT 1

/* Layout of source language data types.  */

#define SHORT_TYPE_SIZE		16
#define INT_TYPE_SIZE		32
#define LONG_TYPE_SIZE		32
#define LONG_LONG_TYPE_SIZE	64
#define FLOAT_TYPE_SIZE		32
#define DOUBLE_TYPE_SIZE	64
#define LONG_DOUBLE_TYPE_SIZE	64

/* Define this as 1 if `char' should by default be signed; else as 0.  */
#define DEFAULT_SIGNED_CHAR 0

#define SIZE_TYPE "unsigned int"
#define PTRDIFF_TYPE "int"
#define WCHAR_TYPE "int"
#define WCHAR_TYPE_SIZE 32


/* ashwin : shifted from arc.c:102 */
#define PROGRAM_COUNTER_REGNO 63

/* Standard register usage.  */

/* Number of actual hardware registers.
   The hardware registers are assigned numbers for the compiler
   from 0 to just below FIRST_PSEUDO_REGISTER.
   All registers that the compiler knows about must be given numbers,
   even those that are not normally considered general registers.

   Registers 61, 62, and 63 are not really registers and we needn't treat
   them as such.  We still need a register for the condition code and
   argument pointer.  */

/* r63 is pc, r64-r127 = simd vregs, r128-r143 = simd dma config regs
   r144, r145 = lp_start, lp_end
   and therefore the pseudo registers start from r146. */
#define FIRST_PSEUDO_REGISTER 146

/* 1 for registers that have pervasive standard uses
   and are not available for the register allocator.

   0-28  - general purpose registers
   29    - ilink1 (interrupt link register)
   30    - ilink2 (interrupt link register)
   31    - blink (branch link register)
   32-59 - reserved for extensions
   60    - LP_COUNT
   61    - condition code
   62    - argument pointer
   63    - program counter

   FWIW, this is how the 61-63 encodings are used by the hardware:
   61    - reserved
   62    - long immediate data indicator
   63    - PCL (program counter aligned to 32 bit, read-only)

   The general purpose registers are further broken down into:

   0-7   - arguments/results
   8-12  - call used (r11 - static chain pointer)
   13-25 - call saved
   26    - global pointer
   27    - frame pointer
   28    - stack pointer
   29    - ilink1
   30    - ilink2 (general purpose register ARCv2)
   31    - return address register

   By default, the extension registers are not available.  */
/* Present implementations only have VR0-VR23 only.  */
/* ??? FIXME: r27 and r31 should not be fixed registers.  */
#define FIXED_REGISTERS \
{ 0, 0, 0, 0, 0, 0, 0, 0,	\
  0, 0, 0, 0, 0, 0, 0, 0,	\
  0, 0, 0, 0, 0, 0, 0, 0,	\
  0, 0, 1, 1, 1, 1, 1, 1,	\
				\
  1, 1, 1, 1, 1, 1, 1, 1,	\
  0, 0, 0, 0, 1, 1, 1, 1,	\
  1, 1, 1, 1, 1, 1, 1, 1,	\
  1, 1, 1, 1, 0, 1, 1, 1,       \
				\
  0, 0, 0, 0, 0, 0, 0, 0,       \
  0, 0, 0, 0, 0, 0, 0, 0,       \
  0, 0, 0, 0, 0, 0, 0, 0,       \
  1, 1, 1, 1, 1, 1, 1, 1,       \
				\
  1, 1, 1, 1, 1, 1, 1, 1,       \
  1, 1, 1, 1, 1, 1, 1, 1,       \
  1, 1, 1, 1, 1, 1, 1, 1,       \
  1, 1, 1, 1, 1, 1, 1, 1,       \
				\
  0, 0, 0, 0, 0, 0, 0, 0,       \
  0, 0, 0, 0, 0, 0, 0, 0,	\
  1, 1}

/* 1 for registers not available across function calls.
   These must include the FIXED_REGISTERS and also any
   registers that can be used without being saved.
   The latter must include the registers where values are returned
   and the register where structure-value addresses are passed.
   Aside from that, you can include as many other registers as you like.  */
#define CALL_USED_REGISTERS     \
{                               \
  1, 1, 1, 1, 1, 1, 1, 1,	\
  1, 1, 1, 1, 1, 0, 0, 0,	\
  0, 0, 0, 0, 0, 0, 0, 0,	\
  0, 0, 1, 1, 1, 1, 1, 1,	\
				\
  1, 1, 1, 1, 1, 1, 1, 1,	\
  1, 1, 1, 1, 1, 1, 1, 1,	\
  1, 1, 1, 1, 1, 1, 1, 1,	\
  1, 1, 1, 1, 1, 1, 1, 1,       \
				\
  0, 0, 0, 0, 0, 0, 0, 0,       \
  0, 0, 0, 0, 0, 0, 0, 0,       \
  0, 0, 0, 0, 0, 0, 0, 0,       \
  1, 1, 1, 1, 1, 1, 1, 1,       \
				\
  1, 1, 1, 1, 1, 1, 1, 1,       \
  1, 1, 1, 1, 1, 1, 1, 1,       \
  1, 1, 1, 1, 1, 1, 1, 1,       \
  1, 1, 1, 1, 1, 1, 1, 1,       \
				\
  0, 0, 0, 0, 0, 0, 0, 0,       \
  0, 0, 0, 0, 0, 0, 0, 0,	\
  1, 1}

/* If defined, an initializer for a vector of integers, containing the
   numbers of hard registers in the order in which GCC should
   prefer to use them (from most preferred to least).  */
#define REG_ALLOC_ORDER							\
{ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1,			\
  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 				\
  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,	\
  48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,		\
  27, 28, 29, 30, 31, 63}

/* Return number of consecutive hard regs needed starting at reg REGNO
   to hold something of mode MODE.
   This is ordinarily the length in words of a value of mode MODE
   but can be less for certain modes in special long registers.  */
#define HARD_REGNO_NREGS(REGNO, MODE) \
((GET_MODE_SIZE (MODE) == 16 && REGNO >= 64 && REGNO < 88) ? 1 \
 : (GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)

/* Value is 1 if hard register REGNO can hold a value of machine-mode MODE.  */
extern unsigned int arc_hard_regno_mode_ok[];
extern unsigned int arc_mode_class[];
#define HARD_REGNO_MODE_OK(REGNO, MODE) \
((arc_hard_regno_mode_ok[REGNO] & arc_mode_class[MODE]) != 0)

/* A C expression that is nonzero if it is desirable to choose
   register allocation so as to avoid move instructions between a
   value of mode MODE1 and a value of mode MODE2.

   If `HARD_REGNO_MODE_OK (R, MODE1)' and `HARD_REGNO_MODE_OK (R,
   MODE2)' are ever different for any R, then `MODES_TIEABLE_P (MODE1,
   MODE2)' must be zero.  */

/* Tie QI/HI/SI modes together.  */
#define MODES_TIEABLE_P(MODE1, MODE2)		\
(GET_MODE_CLASS (MODE1) == MODE_INT		\
 && GET_MODE_CLASS (MODE2) == MODE_INT		\
 && GET_MODE_SIZE (MODE1) <= UNITS_PER_WORD	\
 && GET_MODE_SIZE (MODE2) <= UNITS_PER_WORD)

/* Internal macros to classify a register number as to whether it's a
   general purpose register for compact insns (r0-r3,r12-r15), or
   stack pointer (r28).  */

#define COMPACT_GP_REG_P(REGNO) \
  (((signed)(REGNO) >= 0 && (REGNO) <= 3) || ((REGNO) >= 12 && (REGNO) <= 15))
#define SP_REG_P(REGNO)  ((REGNO) == 28)
#define CODE_DENSITY_REG_P(REGNO) ((signed)(REGNO) >= 0 && (REGNO) <= 3)


/* Register classes and constants.  */

/* Define the classes of registers for register constraints in the
   machine description.  Also define ranges of constants.

   One of the classes must always be named ALL_REGS and include all hard regs.
   If there is more than one class, another class must be named NO_REGS
   and contain no registers.

   The name GENERAL_REGS must be the name of a class (or an alias for
   another name such as ALL_REGS).  This is the class of registers
   that is allowed by "g" or "r" in a register constraint.
   Also, registers outside this class are allocated only when
   instructions express preferences for them.

   The classes must be numbered in nondecreasing order; that is,
   a larger-numbered class must never be contained completely
   in a smaller-numbered class.

   For any two classes, it is very desirable that there be another
   class that represents their union.

   It is important that any condition codes have class NO_REGS.
   See `register_operand'.  */

enum reg_class
{
   NO_REGS,
   R0_REGS,			/* 'x' */
   GP_REG,			/* 'Rgp' */
   FP_REG,			/* 'f' */
   SP_REGS,			/* 'b' */
   LPCOUNT_REG, 		/* 'l' */
   LINK_REGS,	 		/* 'k' */
   DOUBLE_REGS,			/* D0, D1 */
   SIMD_VR_REGS,		/* VR00-VR63 */
   SIMD_DMA_CONFIG_REGS,	/* DI0-DI7,DO0-DO7 */
   ARCOMPACT16_REGS,		/* 'q' */
   AC16_BASE_REGS,  		/* 'e' */
   SIBCALL_REGS,		/* "Rsc" */
   GENERAL_REGS,		/* 'r' */
   MPY_WRITABLE_CORE_REGS,	/* 'W' */
   WRITABLE_CORE_REGS,		/* 'w' */
   CHEAP_CORE_REGS,		/* 'c' */
   ALL_CORE_REGS,		/* 'Rac' */
   R0R3_CODE_DENSITY_REGS,      /* 'Rcd' */
   R0R1_CODE_DENSITY_REGS,      /* 'Rsd' */
   AC16_H_REGS,                 /* 'h' */
   ALL_REGS,
   LIM_REG_CLASSES
};

#define N_REG_CLASSES (int) LIM_REG_CLASSES

/* Give names of register classes as strings for dump file.   */
#define REG_CLASS_NAMES	    \
{                           \
  "NO_REGS",           	    \
  "R0_REGS",                \
  "GP_REG",            	    \
  "FP_REG",            	    \
  "SP_REGS",		    \
  "LPCOUNT_REG",	    \
  "LINK_REGS",         	    \
  "DOUBLE_REGS",            \
  "SIMD_VR_REGS",           \
  "SIMD_DMA_CONFIG_REGS",   \
  "ARCOMPACT16_REGS",  	    \
  "AC16_BASE_REGS",         \
  "SIBCALL_REGS",	    \
  "GENERAL_REGS",      	    \
  "MPY_WRITABLE_CORE_REGS", \
  "WRITABLE_CORE_REGS",     \
  "CHEAP_CORE_REGS",	    \
  "ALL_CORE_REGS",	    \
  "R0R3_CODE_DENSITY_REGS", \
  "R0R1_CODE_DENSITY_REGS", \
  "AC16_H_REGS",	    \
  "ALL_REGS"          	    \
}

/* Define which registers fit in which classes.
   This is an initializer for a vector of HARD_REG_SET
   of length N_REG_CLASSES.  */

#define REG_CLASS_CONTENTS \
{													\
  {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},	     /* No Registers */			\
  {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000},      /* 'x', r0 register , r0 */	\
  {0x04000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},      /* 'Rgp', Global Pointer, r26 */	\
  {0x08000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},      /* 'f', Frame Pointer, r27 */	\
  {0x10000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},      /* 'b', Stack Pointer, r28 */	\
  {0x00000000, 0x10000000, 0x00000000, 0x00000000, 0x00000000},      /* 'l', LPCOUNT Register, r60 */	\
  {0xe0000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000},      /* 'k', LINK Registers, r29-r31 */	\
  {0x00000000, 0x00000f00, 0x00000000, 0x00000000, 0x00000000},      /* 'D', D1, D2 Registers */	\
  {0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0x00000000},      /* 'V', VR00-VR63 Registers */	\
  {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000ffff},      /* 'V', DI0-7,DO0-7 Registers */	\
  {0x0000f00f, 0x00000000, 0x00000000, 0x00000000, 0x00000000},	     /* 'q', r0-r3, r12-r15 */		\
  {0x1000f00f, 0x00000000, 0x00000000, 0x00000000, 0x00000000},	     /* 'e', r0-r3, r12-r15, sp */	\
  {0x1c001fff, 0x00000000, 0x00000000, 0x00000000, 0x00000000},      /* "Rsc", r0-r12 */ \
  {0x9fffffff, 0xc0000000, 0x00000000, 0x00000000, 0x00000000},      /* 'r', r0-r28, blink, ap and pcl */	\
  {0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000},      /* 'W',  r0-r31 */ \
  /* Include ap / pcl in WRITABLE_CORE_REGS for sake of symmetry.  As these \
     registers are fixed, it does not affect the literal meaning of the \
     constraints, but it makes it a superset of GENERAL_REGS, thus \
     enabling some operations that would otherwise not be possible.  */ \
  {0xffffffff, 0xd0000000, 0x00000000, 0x00000000, 0x00000000},      /* 'w', r0-r31, r60 */ \
  {0xffffffff, 0xdfffffff, 0x00000000, 0x00000000, 0x00000000},      /* 'c', r0-r60, ap, pcl */ \
  {0xffffffff, 0xdfffffff, 0x00000000, 0x00000000, 0x00000000},      /* 'Rac', r0-r60, ap, pcl */ \
  {0x0000000f, 0x00000000, 0x00000000, 0x00000000, 0x00000000},      /* 'Rcd', r0-r3 */ \
  {0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x00000000},      /* 'Rsd', r0-r1 */ \
  {0x9fffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000},      /* 'h',  r0-28, r30 */ \
  {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x0003ffff}       /* All Registers */		\
}

/* Local macros to mark the first and last regs of different classes.  */
#define ARC_FIRST_SIMD_VR_REG              64
#define ARC_LAST_SIMD_VR_REG               127

#define ARC_FIRST_SIMD_DMA_CONFIG_REG      128
#define ARC_FIRST_SIMD_DMA_CONFIG_IN_REG   128
#define ARC_FIRST_SIMD_DMA_CONFIG_OUT_REG  136
#define ARC_LAST_SIMD_DMA_CONFIG_REG       143

#define ACC_REG_FIRST 58
#define ACC_REG_LAST  59
#define ACCL_REGNO    (TARGET_BIG_ENDIAN ? ACC_REG_FIRST + 1 : ACC_REG_FIRST)
#define ACCH_REGNO    (TARGET_BIG_ENDIAN ? ACC_REG_FIRST : ACC_REG_FIRST + 1)

/* The same information, inverted:
   Return the class number of the smallest class containing
   reg number REGNO.  This could be a conditional expression
   or could index an array.  */

extern enum reg_class arc_regno_reg_class[];

#define REGNO_REG_CLASS(REGNO) (arc_regno_reg_class[REGNO])

/* The class value for valid index registers. An index register is
   one used in an address where its value is either multiplied by
   a scale factor or added to another register (as well as added to a
   displacement).  */

#define INDEX_REG_CLASS (TARGET_MIXED_CODE ? ARCOMPACT16_REGS : GENERAL_REGS)

/* The class value for valid base registers. A base register is one used in
   an address which is the register value plus a displacement.  */

#define BASE_REG_CLASS (TARGET_MIXED_CODE ? AC16_BASE_REGS : GENERAL_REGS)

#define HARD_REGNO_OK_FOR_BASE_P(REGNO) \
((REGNO) < 29 || ((REGNO) == ARG_POINTER_REGNUM) || ((REGNO) == 63) \
 || ((REGNO) == (unsigned) arc_tp_regno)) \

/* These assume that REGNO is a hard or pseudo reg number.
   They give nonzero only if REGNO is a hard reg of the suitable class
   or a pseudo reg currently allocated to a suitable hard reg.
   Since they use reg_renumber, they are safe only once reg_renumber
   has been allocated, which happens in local-alloc.c.  */
#define REGNO_OK_FOR_BASE_P(REGNO) \
(HARD_REGNO_OK_FOR_BASE_P (REGNO) \
 || (unsigned) reg_renumber[REGNO] < 29)

#define REGNO_OK_FOR_INDEX_P(REGNO) REGNO_OK_FOR_BASE_P(REGNO)

/* Given an rtx X being reloaded into a reg required to be
   in class CLASS, return the class of reg to actually use.
   In general this is just CLASS; but on some machines
   in some cases it is preferable to use a more restrictive class.  */

#define PREFERRED_RELOAD_CLASS(X, CLASS)	\
  arc_preferred_reload_class((X), (CLASS))

  extern enum reg_class arc_preferred_reload_class (rtx, enum reg_class);

/* Return the maximum number of consecutive registers
   needed to represent mode MODE in a register of class CLASS.  */

#define CLASS_MAX_NREGS(CLASS, MODE)					\
  (( GET_MODE_SIZE (MODE) == 16 && CLASS == SIMD_VR_REGS) ? 1:		\
   ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD))

#define SMALL_INT(X) ((unsigned) ((X) + 0x100) < 0x200)
#define SMALL_INT_RANGE(X, OFFSET, SHIFT) \
  ((unsigned) (((X) >> (SHIFT)) + 0x100) \
   < 0x200 - ((unsigned) (OFFSET) >> (SHIFT)))
#define SIGNED_INT12(X) ((unsigned) ((X) + 0x800) < 0x1000)
#define SIGNED_INT16(X) ((unsigned) ((X) + 0x8000) < 0x10000)
#define LARGE_INT(X) \
(((X) < 0) \
 ? (X) >= (-(HOST_WIDE_INT) 0x7fffffff - 1) \
 : (unsigned HOST_WIDE_INT) (X) <= (unsigned HOST_WIDE_INT) 0xffffffff)
#define UNSIGNED_INT3(X) ((unsigned) (X) < 0x8)
#define UNSIGNED_INT5(X) ((unsigned) (X) < 0x20)
#define UNSIGNED_INT6(X) ((unsigned) (X) < 0x40)
#define UNSIGNED_INT7(X) ((unsigned) (X) < 0x80)
#define UNSIGNED_INT8(X) ((unsigned) (X) < 0x100)
#define IS_ONE(X) ((X) == 1)
#define IS_ZERO(X) ((X) == 0)

/* Stack layout and stack pointer usage.  */

/* Define this macro if pushing a word onto the stack moves the stack
   pointer to a smaller address.  */
#define STACK_GROWS_DOWNWARD

/* Define this if the nominal address of the stack frame
   is at the high-address end of the local variables;
   that is, each additional local variable allocated
   goes at a more negative offset in the frame.  */
#define FRAME_GROWS_DOWNWARD 1

/* Offset within stack frame to start allocating local variables at.
   If FRAME_GROWS_DOWNWARD, this is the offset to the END of the
   first local allocated.  Otherwise, it is the offset to the BEGINNING
   of the first local allocated.  */
#define STARTING_FRAME_OFFSET 0

/* Offset from the stack pointer register to the first location at which
   outgoing arguments are placed.  */
#define STACK_POINTER_OFFSET (0)

/* Offset of first parameter from the argument pointer register value.  */
#define FIRST_PARM_OFFSET(FNDECL) (0)

/* A C expression whose value is RTL representing the address in a
   stack frame where the pointer to the caller's frame is stored.
   Assume that FRAMEADDR is an RTL expression for the address of the
   stack frame itself.

   If you don't define this macro, the default is to return the value
   of FRAMEADDR--that is, the stack frame address is also the address
   of the stack word that points to the previous frame.  */
/* ??? unfinished */
/*define DYNAMIC_CHAIN_ADDRESS (FRAMEADDR)*/

/* A C expression whose value is RTL representing the value of the
   return address for the frame COUNT steps up from the current frame.
   FRAMEADDR is the frame pointer of the COUNT frame, or the frame
   pointer of the COUNT - 1 frame if `RETURN_ADDR_IN_PREVIOUS_FRAME'
   is defined.  */
/* The current return address is in r31.  The return address of anything
   farther back is at [%fp,4].  */

#define RETURN_ADDR_RTX(COUNT, FRAME) \
arc_return_addr_rtx(COUNT,FRAME)

/* Register to use for pushing function arguments.  */
#define STACK_POINTER_REGNUM 28

/* Base register for access to local variables of the function.  */
#define FRAME_POINTER_REGNUM 27

/* Base register for access to arguments of the function. This register
   will be eliminated into either fp or sp.  */
#define ARG_POINTER_REGNUM 62

#define RETURN_ADDR_REGNUM 31

/* TODO - check usage of STATIC_CHAIN_REGNUM with a testcase */
/* Register in which static-chain is passed to a function.  This must
   not be a register used by the prologue.  */
#define STATIC_CHAIN_REGNUM  11

/* Function argument passing.  */

/* If defined, the maximum amount of space required for outgoing
   arguments will be computed and placed into the variable
   `crtl->outgoing_args_size'.  No space will be pushed
   onto the stack for each call; instead, the function prologue should
   increase the stack frame size by this amount.  */
#define ACCUMULATE_OUTGOING_ARGS 1

/* Define a data type for recording info about an argument list
   during the scan of that argument list.  This data type should
   hold all necessary information about the function itself
   and about the args processed so far, enough to enable macros
   such as FUNCTION_ARG to determine where the next arg should go.  */
typedef struct arc_args
{
  /* Registers that are still available for parameter passing. */
  bool avail[FIRST_PSEUDO_REGISTER];
  /* HS-ABI: last set register. It is needed for passing the first
     anonymous arguments in regs. */
  unsigned int last_reg;
  /* Backwards compatibility: total number of used registers.  */
  int arg_num;
} CUMULATIVE_ARGS;

/* Initialize a variable CUM of type CUMULATIVE_ARGS
   for a call to a function whose data type is FNTYPE.
   For a library call, FNTYPE is 0.  */
#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,INDIRECT,N_NAMED_ARGS) \
  arc_init_cumulative_args (&CUM, FNTYPE, LIBNAME, INDIRECT, N_NAMED_ARGS)

/* The number of registers used for parameter passing.  Local to this file.  */
#define MAX_ARC_PARM_REGS 8

/* 1 if N is a possible register number for function argument passing.  */
#define FUNCTION_ARG_REGNO_P(N) \
((unsigned) (N) < MAX_ARC_PARM_REGS)

/* The ROUND_ADVANCE* macros are local to this file.  */
/* Round SIZE up to a word boundary.  */
#define ROUND_ADVANCE(SIZE) \
(((SIZE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)

/* Round arg MODE/TYPE up to the next word boundary.  */
#define ROUND_ADVANCE_ARG(MODE, TYPE) \
((MODE) == BLKmode				\
 ? ROUND_ADVANCE (int_size_in_bytes (TYPE))	\
 : ROUND_ADVANCE (GET_MODE_SIZE (MODE)))

#define ARC_FUNCTION_ARG_BOUNDARY(MODE,TYPE) PARM_BOUNDARY
/* Round CUM up to the necessary point for argument MODE/TYPE.  */
/* N.B. Vectors have alignment exceeding BIGGEST_ALIGNMENT.
   ARC_FUNCTION_ARG_BOUNDARY reduces this to no more than 32 bit.  */
#define ROUND_ADVANCE_CUM(CUM, MODE, TYPE) \
  ((((CUM) - 1) | (ARC_FUNCTION_ARG_BOUNDARY ((MODE), (TYPE)) - 1)/BITS_PER_WORD)\
   + 1)

/* Return boolean indicating arg of type TYPE and mode MODE will be passed in
   a reg.  This includes arguments that have to be passed by reference as the
   pointer to them is passed in a reg if one is available (and that is what
   we're given).
   When passing arguments NAMED is always 1.  When receiving arguments NAMED
   is 1 for each argument except the last in a stdarg/varargs function.  In
   a stdarg function we want to treat the last named arg as named.  In a
   varargs function we want to treat the last named arg (which is
   `__builtin_va_alist') as unnamed.
   This macro is only used in this file.  */
#define PASS_IN_REG_P(CUM, MODE, TYPE) \
((CUM) < MAX_ARC_PARM_REGS)


/* Function results.  */

/* Define how to find the value returned by a library function
   assuming the value has mode MODE.  */
#define LIBCALL_VALUE(MODE) gen_rtx_REG (MODE, 0)

/* 1 if N is a possible register number for a function value
   as seen by the caller.  */
/* ??? What about r1 in DI/DF values.  */
#define FUNCTION_VALUE_REGNO_P(N) ((N) == 0)

/* Tell GCC to use RETURN_IN_MEMORY.  */
#define DEFAULT_PCC_STRUCT_RETURN 0

/* Register in which address to store a structure value
   is passed to a function, or 0 to use `invisible' first argument.  */
#define STRUCT_VALUE 0

/* EXIT_IGNORE_STACK should be nonzero if, when returning from a function,
   the stack pointer does not matter.  The value is tested only in
   functions that have frame pointers.
   No definition is equivalent to always zero.  */
#define EXIT_IGNORE_STACK 0

#define EPILOGUE_USES(REGNO) arc_epilogue_uses ((REGNO))

#define EH_USES(REGNO) arc_eh_uses((REGNO))

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

#define ELIMINABLE_REGS					\
{{ARG_POINTER_REGNUM, STACK_POINTER_REGNUM},		\
 {ARG_POINTER_REGNUM, FRAME_POINTER_REGNUM},		\
 {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM}}

/* Define the offset between two registers, one to be eliminated, and the other
   its replacement, at the start of a routine.  */
extern int arc_initial_elimination_offset(int from, int to);
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET)                    \
  (OFFSET) = arc_initial_elimination_offset ((FROM), (TO))

/* Output assembler code to FILE to increment profiler label # LABELNO
   for profiling a function entry.
   We actually emit the profiler code at the call site, so leave this one
   empty.  */
#define FUNCTION_PROFILER(FILE, LABELNO) \
  if (TARGET_UCB_MCOUNT) \
    fprintf (FILE, "\t%s\n", arc_output_libcall ("__mcount"))

#define NO_PROFILE_COUNTERS  1

/* Trampolines.  */

/* Length in units of the trampoline for entering a nested function.  */
#define TRAMPOLINE_SIZE 20

/* Alignment required for a trampoline in bits .  */
/* For actual data alignment we just need 32, no more than the stack;
   however, to reduce cache coherency issues, we want to make sure that
   trampoline instructions always appear the same in any given cache line.  */
#define TRAMPOLINE_ALIGNMENT 256

/* Library calls.  */

/* Addressing modes, and classification of registers for them.  */

/* Maximum number of registers that can appear in a valid memory address.  */
/* The `ld' insn allows 2, but the `st' insn only allows 1.  */
#define MAX_REGS_PER_ADDRESS 1

/* We have pre inc/dec (load/store with update).  */
#define HAVE_PRE_INCREMENT 1
#define HAVE_PRE_DECREMENT 1
#define HAVE_POST_INCREMENT 1
#define HAVE_POST_DECREMENT 1
#define HAVE_PRE_MODIFY_DISP 1
#define HAVE_POST_MODIFY_DISP 1
#define HAVE_PRE_MODIFY_REG 1
#define HAVE_POST_MODIFY_REG 1
/* ??? should also do PRE_MODIFY_REG / POST_MODIFY_REG, but that requires
   a special predicate for the memory operand of stores, like for the SH.  */

/* Recognize any constant value that is a valid address.  */
#define CONSTANT_ADDRESS_P(X) \
(flag_pic?arc_legitimate_pic_addr_p (X): \
 (GET_CODE (X) == LABEL_REF || GET_CODE (X) == SYMBOL_REF	\
  || GET_CODE (X) == CONST_INT \
  || ((GET_CODE (X) == CONST) && !optimize_size)))

/* Is the argument a const_int rtx, containing an exact power of 2 */
#define  IS_POWEROF2_P(X) (! ( (X) & ((X) - 1)) && (X))

/* A C compound statement that attempts to replace X, which is an address
   that needs reloading, with a valid memory address for an operand of
   mode MODE.  WIN is a C statement label elsewhere in the code.

   We try to get a normal form
   of the address.  That will allow inheritance of the address reloads.  */

#define LEGITIMIZE_RELOAD_ADDRESS(X,MODE,OPNUM,TYPE,IND_LEVELS,WIN)	\
  do {									\
    if (arc_legitimize_reload_address (&(X), (MODE), (OPNUM), (TYPE)))	\
      goto WIN;								\
  } while (0)

/* Reading lp_count for anything but the lp instruction is very slow on the
   ARC700.  */
#define DONT_REALLOC(REGNO,MODE) \
  (TARGET_ARC700 && (REGNO) == 60)


/* Given a comparison code (EQ, NE, etc.) and the first operand of a COMPARE,
   return the mode to be used for the comparison.  */
/*extern enum machine_mode arc_select_cc_mode ();*/
#define SELECT_CC_MODE(OP, X, Y) \
arc_select_cc_mode (OP, X, Y)

/* Return non-zero if SELECT_CC_MODE will never return MODE for a
   floating point inequality comparison.  */
#define REVERSIBLE_CC_MODE(MODE) 1 /*???*/

/* Costs.  */

/* Compute extra cost of moving data between one register class
   and another.  */
#define REGISTER_MOVE_COST(MODE, CLASS, TO_CLASS) \
   arc_register_move_cost ((MODE), (CLASS), (TO_CLASS))

/* Compute the cost of moving data between registers and memory.  */
/* Memory is 3 times as expensive as registers.
   ??? Is that the right way to look at it?  */
#define MEMORY_MOVE_COST(MODE,CLASS,IN) \
(GET_MODE_SIZE (MODE) <= UNITS_PER_WORD ? 6 : 12)

/* The cost of a branch insn.  */
/* ??? What's the right value here?  Branches are certainly more
   expensive than reg->reg moves.  */
#define BRANCH_COST(speed_p, predictable_p) 2

/* Nonzero if access to memory by bytes is slow and undesirable.
   For RISC chips, it means that access to memory by bytes is no
   better than access by words when possible, so grab a whole word
   and maybe make use of that.  */
#define SLOW_BYTE_ACCESS  0

/* Define this macro if it is as good or better to call a constant
   function address than to call an address kept in a register.  */
/* On the ARC, calling through registers is slow.  */
#define NO_FUNCTION_CSE

/* Section selection.  */
/* WARNING: These section names also appear in dwarfout.c.  */

#define TEXT_SECTION_ASM_OP	"\t.section\t.text"
#define DATA_SECTION_ASM_OP	"\t.section\t.data"

#define BSS_SECTION_ASM_OP	"\t.section\t.bss"
#define SDATA_SECTION_ASM_OP	"\t.section\t.sdata"
#define SBSS_SECTION_ASM_OP	"\t.section\t.sbss"

/* Expression whose value is a string, including spacing, containing the
   assembler operation to identify the following data as initialization/termination
   code. If not defined, GCC will assume such a section does not exist. */
#define INIT_SECTION_ASM_OP "\t.section\t.init"
#define FINI_SECTION_ASM_OP "\t.section\t.fini"

/* Define this macro if jump tables (for tablejump insns) should be
   output in the text section, along with the assembler instructions.
   Otherwise, the readonly data section is used.
   This macro is irrelevant if there is no separate readonly data section.  */
#define JUMP_TABLES_IN_TEXT_SECTION  (flag_pic || CASE_VECTOR_PC_RELATIVE)

/* For DWARF.  Marginally different than default so output is "prettier"
   (and consistent with above).  */
#define PUSHSECTION_FORMAT "\t%s %s\n"

/* Tell crtstuff.c we're using ELF.  */
#define OBJECT_FORMAT_ELF

/* PIC */

/* The register number of the register used to address a table of static
   data addresses in memory.  In some cases this register is defined by a
   processor's ``application binary interface'' (ABI).  When this macro
   is defined, RTL is generated for this register once, as with the stack
   pointer and frame pointer registers.  If this macro is not defined, it
   is up to the machine-dependent files to allocate such a register (if
   necessary).  */
#define PIC_OFFSET_TABLE_REGNUM 26

/* Define this macro if the register defined by PIC_OFFSET_TABLE_REGNUM is
   clobbered by calls.  Do not define this macro if PIC_OFFSET_TABLE_REGNUM
   is not defined.  */
/* This register is call-saved on the ARC.  */
/*#define PIC_OFFSET_TABLE_REG_CALL_CLOBBERED*/

/* A C expression that is nonzero if X is a legitimate immediate
   operand on the target machine when generating position independent code.
   You can assume that X satisfies CONSTANT_P, so you need not
   check this.  You can also assume `flag_pic' is true, so you need not
   check it either.  You need not define this macro if all constants
   (including SYMBOL_REF) can be immediate operands when generating
   position independent code.  */
#define LEGITIMATE_PIC_OPERAND_P(X)  (arc_legitimate_pic_operand_p(X))

#define ASM_PREFERRED_EH_DATA_FORMAT(CODE, GLOBAL) \
  (flag_pic \
   ? (GLOBAL ? DW_EH_PE_indirect : 0) | DW_EH_PE_pcrel | DW_EH_PE_sdata4 \
   : DW_EH_PE_absptr)

/* Control the assembler format that we output.  */

/* A C string constant describing how to begin a comment in the target
   assembler language.  The compiler assumes that the comment will
   end at the end of the line.  */
/* Gas needs this to be "#" in order to recognize line directives.  */
#define ASM_COMMENT_START "#"

/* Output to assembler file text saying following lines
   may contain character constants, extra white space, comments, etc.  */
#define ASM_APP_ON ""

/* Output to assembler file text saying following lines
   no longer contain unusual constructs.  */
#define ASM_APP_OFF ""

/* Globalizing directive for a label.  */
#define GLOBAL_ASM_OP "\t.global\t"

/* This is how to output an assembler line defining a `char' constant.  */
#define ASM_OUTPUT_CHAR(FILE, VALUE) \
( fprintf (FILE, "\t.byte\t"),			\
  output_addr_const (FILE, (VALUE)),		\
  fprintf (FILE, "\n"))

/* This is how to output an assembler line defining a `short' constant.  */
#define ASM_OUTPUT_SHORT(FILE, VALUE) \
( fprintf (FILE, "\t.hword\t"),			\
  output_addr_const (FILE, (VALUE)),		\
  fprintf (FILE, "\n"))

/* This is how to output an assembler line defining an `int' constant.
   We also handle symbol output here.  Code addresses must be right shifted
   by 2 because that's how the jump instruction wants them.  */
#define ASM_OUTPUT_INT(FILE, VALUE) arc_asm_output_int(FILE, VALUE);

/* This is how to output an assembler line defining a `float' constant.  */
#define ASM_OUTPUT_FLOAT(FILE, VALUE) \
{							\
  long t;						\
  char str[30];						\
  REAL_VALUE_TO_TARGET_SINGLE ((VALUE), t);		\
  REAL_VALUE_TO_DECIMAL ((VALUE), "%.20e", str);	\
  fprintf (FILE, "\t.word\t0x%lx %s %s\n",		\
	   t, ASM_COMMENT_START, str);			\
}

/* This is how to output an assembler line defining a `double' constant.  */
#define ASM_OUTPUT_DOUBLE(FILE, VALUE) \
{							\
  long t[2];						\
  char str[30];						\
  REAL_VALUE_TO_TARGET_DOUBLE ((VALUE), t);		\
  REAL_VALUE_TO_DECIMAL ((VALUE), "%.20e", str);	\
  fprintf (FILE, "\t.word\t0x%lx %s %s\n\t.word\t0x%lx\n", \
	   t[0], ASM_COMMENT_START, str, t[1]);		\
}

/* This is how to output the definition of a user-level label named NAME,
   such as the label on a static function or variable NAME.  */
#define ASM_OUTPUT_LABEL(FILE, NAME) \
do { assemble_name (FILE, NAME); fputs (":\n", FILE); } while (0)

#define ASM_NAME_P(NAME) ( NAME[0]=='*')

/* This is how to output a reference to a user-level label named NAME.
   `assemble_name' uses this.  */
/* We work around a dwarfout.c deficiency by watching for labels from it and
   not adding the '_' prefix.  There is a comment in
   dwarfout.c that says it should be using ASM_OUTPUT_INTERNAL_LABEL.  */
#define ASM_OUTPUT_LABELREF(FILE, NAME1) \
do {							\
  const char *NAME;					\
  NAME = (*targetm.strip_name_encoding)(NAME1);		\
  if ((NAME)[0] == '.' && (NAME)[1] == 'L')		\
    fprintf (FILE, "%s", NAME);				\
  else							\
    {							\
      if (!ASM_NAME_P (NAME1))				\
	fprintf (FILE, "%s", user_label_prefix);	\
      fprintf (FILE, "%s", NAME);			\
    }							\
} while (0)

/* This is how to output a reference to a symbol_ref / label_ref as
   (part of) an operand.  To disambiguate from register names like
   a1 / a2 / status etc, symbols are preceded by '@'.  */
#define ASM_OUTPUT_SYMBOL_REF(FILE,SYM) \
  ASM_OUTPUT_LABEL_REF ((FILE), XSTR ((SYM), 0))
#define ASM_OUTPUT_LABEL_REF(FILE,STR)			\
  do							\
    {							\
      fputc ('@', file);				\
      assemble_name ((FILE), (STR));			\
    }							\
  while (0)

/* Store in OUTPUT a string (made with alloca) containing
   an assembler-name for a local static variable named NAME.
   LABELNO is an integer which is different for each call.  */
#define ASM_FORMAT_PRIVATE_NAME(OUTPUT, NAME, LABELNO) \
( (OUTPUT) = (char *) alloca (strlen ((NAME)) + 10),	\
  sprintf ((OUTPUT), "%s.%d", (NAME), (LABELNO)))

/* The following macro defines the format used to output the second
   operand of the .type assembler directive.  Different svr4 assemblers
   expect various different forms for this operand.  The one given here
   is just a default.  You may need to override it in your machine-
   specific tm.h file (depending upon the particulars of your assembler).  */

#undef  TYPE_OPERAND_FMT
#define TYPE_OPERAND_FMT	"@%s"

/*  A C string containing the appropriate assembler directive to
    specify the size of a symbol, without any arguments.  On systems
    that use ELF, the default (in `config/elfos.h') is `"\t.size\t"';
    on other systems, the default is not to define this macro.  */
#undef SIZE_ASM_OP
#define SIZE_ASM_OP "\t.size\t"

/* Assembler pseudo-op to equate one value with another.  */
/* ??? This is needed because dwarfout.c provides a default definition too
   late for defaults.h (which contains the default definition of ASM_OTPUT_DEF
   that we use).  */
#ifdef SET_ASM_OP
#undef SET_ASM_OP
#endif
#define SET_ASM_OP "\t.set\t"

extern char rname29[], rname30[], rname56[], rname57[], rname58[], rname59[];
/* How to refer to registers in assembler output.
   This sequence is indexed by compiler's hard-register-number (see above).  */
#define REGISTER_NAMES								\
{  "r0",   "r1",   "r2",   "r3",       "r4",     "r5",     "r6",    "r7",	\
   "r8",   "r9",  "r10",  "r11",      "r12",    "r13",    "r14",   "r15",	\
  "r16",  "r17",  "r18",  "r19",      "r20",    "r21",    "r22",   "r23",	\
  "r24",  "r25",   "gp",   "fp",       "sp",  rname29,  rname30, "blink",	\
  "r32",  "r33",  "r34",  "r35",      "r36",    "r37",    "r38",   "r39",	\
   "d1",   "d1",   "d2",   "d2",      "r44",    "r45",    "r46",   "r47",	\
  "r48",  "r49",  "r50",  "r51",      "r52",    "r53",    "r54",   "r55",	\
  rname56,rname57,rname58,rname59,"lp_count",    "cc",     "ap",   "pcl",	\
  "vr0",  "vr1",  "vr2",  "vr3",      "vr4",    "vr5",    "vr6",   "vr7",       \
  "vr8",  "vr9", "vr10", "vr11",     "vr12",   "vr13",   "vr14",  "vr15",	\
 "vr16", "vr17", "vr18", "vr19",     "vr20",   "vr21",   "vr22",  "vr23",	\
 "vr24", "vr25", "vr26", "vr27",     "vr28",   "vr29",   "vr30",  "vr31",	\
 "vr32", "vr33", "vr34", "vr35",     "vr36",   "vr37",   "vr38",  "vr39",	\
 "vr40", "vr41", "vr42", "vr43",     "vr44",   "vr45",   "vr46",  "vr47",	\
 "vr48", "vr49", "vr50", "vr51",     "vr52",   "vr53",   "vr54",  "vr55",	\
 "vr56", "vr57", "vr58", "vr59",     "vr60",   "vr61",   "vr62",  "vr63",	\
  "dr0",  "dr1",  "dr2",  "dr3",      "dr4",    "dr5",    "dr6",   "dr7",	\
  "dr0",  "dr1",  "dr2",  "dr3",      "dr4",    "dr5",    "dr6",   "dr7",	\
  "lp_start", "lp_end" \
}

/* Entry to the insn conditionalizer.  */
#define FINAL_PRESCAN_INSN(INSN, OPVEC, NOPERANDS) \
  arc_final_prescan_insn (INSN, OPVEC, NOPERANDS)

/* A C expression which evaluates to true if CODE is a valid
   punctuation character for use in the `PRINT_OPERAND' macro.  */
extern char arc_punct_chars[];
#define PRINT_OPERAND_PUNCT_VALID_P(CHAR) \
arc_punct_chars[(unsigned char) (CHAR)]

/* Print operand X (an rtx) in assembler syntax to file FILE.
   CODE is a letter or dot (`z' in `%z0') or 0 if no letter was specified.
   For `%' followed by punctuation, CODE is the punctuation and X is null.  */
#define PRINT_OPERAND(FILE, X, CODE) \
arc_print_operand (FILE, X, CODE)

/* A C compound statement to output to stdio stream STREAM the
   assembler syntax for an instruction operand that is a memory
   reference whose address is ADDR.  ADDR is an RTL expression.

   On some machines, the syntax for a symbolic address depends on
   the section that the address refers to.  On these machines,
   define the macro `ENCODE_SECTION_INFO' to store the information
   into the `symbol_ref', and then check for it here.  */
#define PRINT_OPERAND_ADDRESS(FILE, ADDR) \
arc_print_operand_address (FILE, ADDR)

/* This is how to output an element of a case-vector that is absolute.  */
#define ASM_OUTPUT_ADDR_VEC_ELT(FILE, VALUE)  \
do {							\
  char label[30];					\
  ASM_GENERATE_INTERNAL_LABEL (label, "L", VALUE);	\
  fprintf (FILE, "\t.word ");				\
  assemble_name (FILE, label);				\
  fprintf(FILE, "\n");					\
} while (0)

/* This is how to output an element of a case-vector that is relative.  */
#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, BODY, VALUE, REL) \
do {							\
  char label[30];					\
  ASM_GENERATE_INTERNAL_LABEL (label, "L", VALUE);	\
  switch (GET_MODE (BODY))				\
    {							\
    case QImode: fprintf (FILE, "\t.byte "); break;	\
    case HImode: fprintf (FILE, "\t.hword "); break;	\
    case SImode: fprintf (FILE, "\t.word "); break;	\
    default: gcc_unreachable ();			\
    }							\
  assemble_name (FILE, label);				\
  fprintf (FILE, "-");					\
  ASM_GENERATE_INTERNAL_LABEL (label, "L", REL);	\
  assemble_name (FILE, label);				\
  if (TARGET_COMPACT_CASESI)				\
    fprintf (FILE, " + %d", 4 + arc_get_unalign ());	\
  fprintf(FILE, "\n");                                  \
} while (0)

/* ADDR_DIFF_VECs are in the text section and thus can affect the
   current alignment.  */
#define ASM_OUTPUT_CASE_END(FILE, NUM, JUMPTABLE)       \
  do                                                    \
    {                                                   \
      if (GET_CODE (PATTERN (JUMPTABLE)) == ADDR_DIFF_VEC \
	  && ((GET_MODE_SIZE (GET_MODE (PATTERN (JUMPTABLE))) \
	       * XVECLEN (PATTERN (JUMPTABLE), 1) + 1)	\
	      & 2))					\
      arc_toggle_unalign ();				\
    }                                                   \
  while (0)

#define JUMP_ALIGN(LABEL) (arc_size_opt_level < 2 ? 2 : 0)
#define LABEL_ALIGN_AFTER_BARRIER(LABEL) \
  (JUMP_ALIGN(LABEL) \
   ? JUMP_ALIGN(LABEL) \
   : GET_CODE (PATTERN (prev_active_insn (LABEL))) == ADDR_DIFF_VEC \
   ? 1 : 0)
/* The desired alignment for the location counter at the beginning
   of a loop.  */
/* On the ARC, align loops to 4 byte boundaries unless doing all-out size
   optimization.  */
#define LOOP_ALIGN JUMP_ALIGN

#define LABEL_ALIGN(LABEL) (arc_label_align (LABEL))

/* This is how to output an assembler line
   that says to advance the location counter
   to a multiple of 2**LOG bytes.  */
#define ASM_OUTPUT_ALIGN(FILE,LOG) \
do { \
  if ((LOG) != 0) fprintf (FILE, "\t.align %d\n", 1 << (LOG));	\
  if ((LOG)  > 1) \
    arc_clear_unalign (); \
} while (0)

/*  ASM_OUTPUT_ALIGNED_DECL_LOCAL (STREAM, DECL, NAME, SIZE, ALIGNMENT)
    Define this macro when you need to see the variable's decl in order to
    chose what to output.  */
#define ASM_OUTPUT_ALIGNED_DECL_LOCAL(STREAM, DECL, NAME, SIZE, ALIGNMENT) \
  arc_asm_output_aligned_decl_local (STREAM, DECL, NAME, SIZE, ALIGNMENT, 0)

/* Debugging information.  */

/* Generate DBX and DWARF debugging information.  */
#ifdef DBX_DEBUGGING_INFO
#undef DBX_DEBUGGING_INFO
#endif
#define DBX_DEBUGGING_INFO

#ifdef DWARF2_DEBUGGING_INFO
#undef DWARF2_DEBUGGING_INFO
#endif
#define DWARF2_DEBUGGING_INFO

/* Prefer STABS (for now).  */
#undef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

/* How to renumber registers for dbx and gdb.  */
#define DBX_REGISTER_NUMBER(REGNO) \
  ((TARGET_MULMAC_32BY16_SET && (REGNO) >= 56 && (REGNO) <= 57) \
   ? ((REGNO) ^ !TARGET_BIG_ENDIAN) \
   : (TARGET_MUL64_SET && (REGNO) >= 57 && (REGNO) <= 59) \
   ? ((REGNO) == 57 \
      ? 58 /* MMED */ \
      : ((REGNO) & 1) ^ TARGET_BIG_ENDIAN \
      ? 59 /* MHI */ \
      : 57 + !!TARGET_MULMAC_32BY16_SET) /* MLO */ \
   : (REGNO))

#define DWARF_FRAME_REGNUM(REG) (REG)

#define DWARF_FRAME_RETURN_COLUMN 	DWARF_FRAME_REGNUM (31)

#define INCOMING_RETURN_ADDR_RTX  gen_rtx_REG (Pmode, 31)

/* Frame info.  */

/* Define this macro to 0 if your target supports DWARF 2 frame unwind
   information, but it does not yet work with exception handling.  */
/* N.B. the below test is valid in an #if, but not in a C expression.  */
#if DEFAULT_LIBC == LIBC_UCLIBC
#define DWARF2_UNWIND_INFO 1
#else
#define DWARF2_UNWIND_INFO 0
#endif

#define EH_RETURN_DATA_REGNO(N)	\
  ((N) < 2 ? (N) : INVALID_REGNUM)

#define EH_RETURN_STACKADJ_RTX 		gen_rtx_REG (Pmode, 2)

#define EH_RETURN_HANDLER_RTX		arc_eh_return_address_location ()

/* Turn off splitting of long stabs.  */
#define DBX_CONTIN_LENGTH 0

/* Miscellaneous.  */

/* Specify the machine mode that this machine uses
   for the index in the tablejump instruction.
   If we have pc relative case vectors, we start the case vector shortening
   with QImode.  */
#define CASE_VECTOR_MODE \
  ((optimize && (CASE_VECTOR_PC_RELATIVE || flag_pic)) ? QImode : Pmode)

/* Define as C expression which evaluates to nonzero if the tablejump
   instruction expects the table to contain offsets from the address of the
   table.
   Do not define this if the table should contain absolute addresses.  */
#define CASE_VECTOR_PC_RELATIVE TARGET_CASE_VECTOR_PC_RELATIVE

#define CASE_VECTOR_SHORTEN_MODE(MIN_OFFSET, MAX_OFFSET, BODY) \
  CASE_VECTOR_SHORTEN_MODE_1 \
    (MIN_OFFSET, TARGET_COMPACT_CASESI ? MAX_OFFSET + 6 : MAX_OFFSET, BODY)

#define CASE_VECTOR_SHORTEN_MODE_1(MIN_OFFSET, MAX_OFFSET, BODY) \
((MIN_OFFSET) >= 0 && (MAX_OFFSET) <= 255 \
 ? (ADDR_DIFF_VEC_FLAGS (BODY).offset_unsigned = 1, QImode) \
 : (MIN_OFFSET) >= -128 && (MAX_OFFSET) <= 127 \
 ? (ADDR_DIFF_VEC_FLAGS (BODY).offset_unsigned = 0, QImode) \
 : (MIN_OFFSET) >= 0 && (MAX_OFFSET) <= 65535 \
 ? (ADDR_DIFF_VEC_FLAGS (BODY).offset_unsigned = 1, HImode) \
 : (MIN_OFFSET) >= -32768 && (MAX_OFFSET) <= 32767 \
 ? (ADDR_DIFF_VEC_FLAGS (BODY).offset_unsigned = 0, HImode) \
 : SImode)

#define ADDR_VEC_ALIGN(VEC_INSN) \
  (exact_log2 (GET_MODE_SIZE (GET_MODE (PATTERN (VEC_INSN)))))
#undef ASM_OUTPUT_BEFORE_CASE_LABEL
#define ASM_OUTPUT_BEFORE_CASE_LABEL(FILE, PREFIX, NUM, TABLE) \
  ASM_OUTPUT_ALIGN ((FILE), ADDR_VEC_ALIGN (TABLE));

#define INSN_LENGTH_ALIGNMENT(INSN) \
  ((JUMP_P (INSN) \
    && GET_CODE (PATTERN (INSN)) == ADDR_DIFF_VEC \
    && GET_MODE (PATTERN (INSN)) == QImode) \
   ? 0 : length_unit_log)

/* Define if operations between registers always perform the operation
   on the full register even if a narrower mode is specified.  */
#define WORD_REGISTER_OPERATIONS

/* Define if loading in MODE, an integral mode narrower than BITS_PER_WORD
   will either zero-extend or sign-extend.  The value of this macro should
   be the code that says which one of the two operations is implicitly
   done, NIL if none.  */
#define LOAD_EXTEND_OP(MODE) ZERO_EXTEND


/* Max number of bytes we can move from memory to memory
   in one reasonably fast instruction.  */
#define MOVE_MAX 4

/* Let the movmem expander handle small block moves.  */
#define MOVE_BY_PIECES_P(LEN, ALIGN)  0
#define CAN_MOVE_BY_PIECES(SIZE, ALIGN) \
  (move_by_pieces_ninsns (SIZE, ALIGN, MOVE_MAX_PIECES + 1) \
   < (unsigned int) MOVE_RATIO (!optimize_size))

/* Undo the effects of the movmem pattern presence on STORE_BY_PIECES_P .  */
#define MOVE_RATIO(SPEED) ((SPEED) ? 15 : 3)

/* Define this to be nonzero if shift instructions ignore all but the low-order
   few bits. Changed from 1 to 0 for rotate pattern testcases
   (e.g. 20020226-1.c). This change truncates the upper 27 bits of a word
   while rotating a word. Came to notice through a combine phase
   optimization viz. a << (32-b) is equivalent to a << (-b).
   vgupta: Changed this back to 1 in 4.8.4 and see that above test passes
   nevertheless both at -O2 and otherwise
*/
#define SHIFT_COUNT_TRUNCATED 1

/* Value is 1 if truncating an integer of INPREC bits to OUTPREC bits
   is done just by pretending it is already truncated.  */
#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC) 1

/* We assume that the store-condition-codes instructions store 0 for false
   and some other value for true.  This is the value stored for true.  */
#define STORE_FLAG_VALUE 1

/* Specify the machine mode that pointers have.
   After generation of rtl, the compiler makes no further distinction
   between pointers and any other objects of this machine mode.  */
/* ARCompact has full 32-bit pointers.  */
#define Pmode SImode

/* A function address in a call instruction.  */
#define FUNCTION_MODE SImode

/* Define the information needed to generate branch and scc insns.  This is
   stored from the compare operation.  Note that we can't use "rtx" here
   since it hasn't been defined!  */
extern struct rtx_def *arc_compare_op0, *arc_compare_op1;

/* ARC function types.   */
typedef unsigned int arc_function_type;

/* No function should have the unknown type. This value is used to indicate
   the that function type has not yet been computed.  */
#define ARC_FUNCTION_UNKNOWN 0

/* The normal function type indicates that the function has the standard
   prologue and epilogue.  */
#define ARC_FUNCTION_NORMAL  1 << 0

/* The naked function type indicates that the function does not have
   prologue or epilogue, and that no stack frame is available.  */
#define ARC_FUNCTION_NAKED   1 << 1

/* The following identify the different types of interrupt functions that
   are support.  An interrupt function is one with the 'interrupt'
   attribute, a parameter passed to the interrupt attribute identifies the
   type of interrupt function being created.  Making a function an
   interrupt function changes things like stack frame layout, debug
   information generated, and which register holds the return address.  */
#define ARC_FUNCTION_ILINK1  1 << 2
#define ARC_FUNCTION_ILINK2  1 << 3
#define ARC_FUNCTION_FIRQ    1 << 4

/* Check if a function is an interrupt function.  */
#define ARC_INTERRUPT_P(TYPE) \
  (((TYPE) & (ARC_FUNCTION_ILINK1 | ARC_FUNCTION_ILINK2 | ARC_FUNCTION_FIRQ)) != 0)

#define ARC_FAST_INTERRUPT_P(TYPE) \
  (((TYPE) & ARC_FUNCTION_FIRQ) != 0)

/* Check if a function is normal, that is, has standard prologue and
   epilogue.  */
#define ARC_NORMAL_P(TYPE) (((TYPE) & ARC_FUNCTION_NORMAL) != 0)

#define ARC_NAKED_P(TYPE) (((TYPE) & ARC_FUNCTION_NAKED) != 0)

/* Return the register number of the register holding the return address
   for a function of type TYPE.  */

extern int arc_return_address_register (arc_function_type type);

/* Compute the type of a function from its DECL.  Needed for EPILOGUE_USES.  */
struct function;
extern arc_function_type arc_compute_function_type (struct function *);

/* Called by crtstuff.c to make calls to function FUNCTION that are defined in
   SECTION_OP, and then to switch back to text section.  */
#undef CRT_CALL_STATIC_FUNCTION
#define CRT_CALL_STATIC_FUNCTION(SECTION_OP, FUNC)		\
  asm (SECTION_OP "\n\t"					\
       "add r12,pcl,@" USER_LABEL_PREFIX #FUNC "@pcl\n\t"	\
       "jl  [r12]\n"						\
       TEXT_SECTION_ASM_OP);

/* This macro expands to the name of the scratch register r12, used for
   temporary calculations according to the ABI.  */
#define ARC_TEMP_SCRATCH_REG "r12"

/* The C++ compiler must use one bit to indicate whether the function
   that will be called through a pointer-to-member-function is
   virtual.  Normally, we assume that the low-order bit of a function
   pointer must always be zero.  Then, by ensuring that the
   vtable_index is odd, we can distinguish which variant of the union
   is in use.  But, on some platforms function pointers can be odd,
   and so this doesn't work.  In that case, we use the low-order bit
   of the `delta' field, and shift the remainder of the `delta' field
   to the left. We needed to do this for A4 because the address was always
   shifted and thus could be odd.  */
#define TARGET_PTRMEMFUNC_VBIT_LOCATION \
  (ptrmemfunc_vbit_in_pfn)

#define INSN_SETS_ARE_DELAYED(X)		\
  (GET_CODE (X) == INSN				\
   && GET_CODE (PATTERN (X)) != SEQUENCE	\
   && GET_CODE (PATTERN (X)) != USE		\
   && GET_CODE (PATTERN (X)) != CLOBBER		\
   && (get_attr_type (X) == TYPE_CALL || get_attr_type (X) == TYPE_SFUNC))

/* ??? INSN_REFERENCES_ARE_DELAYED also applies to the address in a CALL
   even if that call is not in a CALL_INSN.
   For sfuncs, we cope with disapplowing r12 setters in the delay slot,
   but tls_gd_dispatch allows any register.  */
#define INSN_REFERENCES_ARE_DELAYED(insn) \
  (INSN_SETS_ARE_DELAYED (insn) && !insn_is_tls_gd_dispatch (insn))

#define CALL_ATTR(X, NAME) \
  ((CALL_P (X) || NONJUMP_INSN_P (X)) \
   && GET_CODE (PATTERN (X)) != USE \
   && GET_CODE (PATTERN (X)) != CLOBBER \
   && get_attr_is_##NAME (X) == IS_##NAME##_YES) \

#define REVERSE_CONDITION(CODE,MODE) \
  (((MODE) == CC_FP_GTmode || (MODE) == CC_FP_GEmode		 \
    || (MODE) == CC_FP_UNEQmode || (MODE) == CC_FP_ORDmode	 \
    || (MODE) == CC_FPXmode || (MODE) == CC_FPUmode              \
    || (MODE) == CC_FPUEmode)                                    \
   ? reverse_condition_maybe_unordered ((CODE))			 \
   : reverse_condition ((CODE)))

#define ADJUST_INSN_LENGTH(X, LENGTH) \
  ((LENGTH) \
   = (GET_CODE (PATTERN (X)) == SEQUENCE \
      ? ((LENGTH) \
	 + arc_adjust_insn_length (XVECEXP (PATTERN (X), 0, 0), \
				   get_attr_length (XVECEXP (PATTERN (X), \
						    0, 0)), \
				   true) \
	 - get_attr_length (XVECEXP (PATTERN (X), 0, 0)) \
	 + arc_adjust_insn_length (XVECEXP (PATTERN (X), 0, 1), \
				   get_attr_length (XVECEXP (PATTERN (X), \
						    0, 1)), \
				   true) \
	 - get_attr_length (XVECEXP (PATTERN (X), 0, 1))) \
      : arc_adjust_insn_length ((X), (LENGTH), false)))

#define IS_ASM_LOGICAL_LINE_SEPARATOR(C,STR) ((C) == '`')

#define INIT_EXPANDERS arc_init_expanders ()

enum
{
  ARC_LRA_PRIORITY_NONE, ARC_LRA_PRIORITY_NONCOMPACT, ARC_LRA_PRIORITY_COMPACT
};

/* The define_cond_exec construct is rather crude, as we can't have
   different ones with different conditions apply to different sets
   of instructions.  We can't use an attribute test inside the condition,
   because that would lead to infinite recursion as the attribute test
   needs to recognize the insn.  So, instead we have a clause for
   the pattern condition of all sfunc patterns which is only relevant for
   the predicated varaint.  */
#define SFUNC_CHECK_PREDICABLE \
  (GET_CODE (PATTERN (insn)) != COND_EXEC || !flag_pic || !TARGET_MEDIUM_CALLS)

/* EM defines*/
#define EM_MUL_MPYW ((arc_mpy_option > 0) && TARGET_V2)
#define EM_MULTI    ((arc_mpy_option > 1) && TARGET_V2)

/* Dump the stack info. */
#undef  ASM_OUTPUT_FUNCTION_PREFIX
#define  ASM_OUTPUT_FUNCTION_PREFIX(STREAM, NAME) \
  arc_dump_stack_info(STREAM, NAME)

/* FPU defines. */
#define TARGET_HARD_FLOAT ((arc_fpu_build & (FPU_SP | FPU_DP)) != 0)
#define TARGET_FP_SINGLE  ((arc_fpu_build & FPU_SP) != 0)
#define TARGET_FP_DOUBLE  ((arc_fpu_build & FPU_DP) != 0)
#define TARGET_FP_SFUZED  ((arc_fpu_build & FPU_SF) != 0)
#define TARGET_FP_DFUZED  ((arc_fpu_build & FPU_DF) != 0)
#define TARGET_FP_SCONV   ((arc_fpu_build & FPU_SC) != 0)
#define TARGET_FP_DCONV   ((arc_fpu_build & FPU_DC) != 0)
#define TARGET_FP_SSQRT   ((arc_fpu_build & FPU_SD) != 0)
#define TARGET_FP_DSQRT   ((arc_fpu_build & FPU_DD) != 0)

#define TARGET_FPX_QUARK (TARGET_EM && TARGET_SPFP	\
			  && (arc_fpu_build == FPX_QK))
#define TARGET_FP_ASSIST ((arc_fpu_build & FPX_DP) != 0)

/* Handle pragmas for JLI calls. */
#define REGISTER_TARGET_PRAGMAS() do {                                  \
  c_register_pragma (0, "jli_call_fixed",  arc_pr_jli_call_fixed);      \
  c_register_pragma (0, "jli_call_always", arc_pr_jli_call_always);     \
  arc_pr_init();                                                        \
} while (0)

/** Maximum number of JLI entries. */
#define ARC_JLI_ENTRIES_MAX (1024)

/** Number of JLI entries in the fixed table. */
extern int jli_fixed_count;

/* List of fixed symbols in the JLI table. */
extern char *jli_fixed_table[ARC_JLI_ENTRIES_MAX];

/** Number of JLI entries in the dynamic table. */
extern int jli_dynamic_count;

/* List of fixed symbols in the JLI table. */
extern char *jli_dynamic_table[ARC_JLI_ENTRIES_MAX];

#endif /* GCC_ARC_H */

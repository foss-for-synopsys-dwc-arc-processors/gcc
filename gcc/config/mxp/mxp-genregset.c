/* Generate the machine mode enumeration and associated tables.
   Copyright (C) 2003, 2004, 2008
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config/mxp/regset-config.h"
#include "bconfig.h"
#include "system.h"
#include "errors.h"

int vec_lanes[] = MXP_VEC_LANE_SETS;
int acc_lanes[] = MXP_FLAG_ACC_LANE_SETS;
#define flag_lanes acc_lanes
int scalar_lanes[] = MXP_SCALAR_LANE_SETS;
const char *fake_reg_names[] = MXP_FAKE_REG_NAMES;
const int regs_per_vreg = 16 / MXP_UNITS_PER_MXP_VEC_REG;

#define SCALAR_NSETS ARRAY_SIZE(scalar_lanes)
#define VEC_NSETS ARRAY_SIZE(vec_lanes)
#define ACC_NSETS ARRAY_SIZE(acc_lanes)
#define FLAG_NSETS ARRAY_SIZE(flag_lanes)
#define N_RCLASS ((int) (SCALAR_NSETS * VEC_NSETS * ACC_NSETS * FLAG_NSETS))


/* Return element I of the array LANESET_SET.  */
static int
laneset (int i, int *laneset_set)
{
  return laneset_set[i];
}

#define DECODE_REGCLASS \
  int scalar = laneset (i % SCALAR_NSETS, scalar_lanes); \
  int vec = laneset ((i /= SCALAR_NSETS) % VEC_NSETS, vec_lanes);\
  int flags = laneset ((i /= VEC_NSETS) % FLAG_NSETS, flag_lanes); \
  int acc   = laneset((i /= FLAG_NSETS) % ACC_NSETS, acc_lanes);

static int
valid_regclass (int i)
{
  DECODE_REGCLASS

  return MXP_VALID_REG_CLASS (scalar, vec, flags, acc);
}

static void
emit_regclass_name (int i)
{
  if (i == 0)
    printf ("NO_REGS");
  else if (i == N_RCLASS - 1)
    printf ("ALL_REGS");
  else
    {
      DECODE_REGCLASS

      if (scalar)
	printf ("S%02x", scalar);
      if (vec)
	printf ("V%02x", vec);
      if (flags)
	printf ("F%02x", flags);
      if (acc)
	printf ("A%02x", acc);
      printf ("_REGS");
    }
}

static void
emit_regclass_content (int i)
{
  long shift, mask;
  int n_words = 0;
  DECODE_REGCLASS

#define INC_SHIFT(UNITS) \
  do \
    { \
      shift += (UNITS) / MXP_UNITS_PER_MXP_VEC_REG; \
      if (shift >= 32) \
	{ \
	  if (n_words++ > 5) \
	    { \
	      printf (" \\\n    "); \
	      n_words = 1; \
	    } \
	  printf (" 0x%08lx,", mask); \
	  mask = shift = 0; \
	} \
    } \
  while (0)

  /* vr0 / vr1 contain scalar regs.  */
  mask = scalar | (scalar << 8);
  shift = 16;
  /* vr2 .. vr62 are (inasmuch as implemented) non-scalar vector registers.  */
  for (i = 2; i < 62; i++)
    {
      mask |= vec << shift;
      INC_SHIFT (16);
    }
  /* accumulator */
  mask |= acc << shift;
  INC_SHIFT (16);
  /* vflags */
  mask |= flags << shift;
  INC_SHIFT (16);
  /* Fake registers.  */
  for (i = 0; i < (int) ARRAY_SIZE (fake_reg_names); i++)
    {
      INC_SHIFT (0);
      if (scalar & MXP_FAKE_REG_LANES)
	mask |= 1 << shift++;
    }
  INC_SHIFT (256);
}

static void
upputs (const char *s)
{
  while (*s)
#if 0
    putchar (TOUPPER (*s++));
#else
    {
      putchar (*s >= 'a' && *s <= 'z' ? *s - 'a' + 'A': *s);
      s++;
    }
#endif
}

static void
regno_reg_class_1 (char letter, int *a, int size)
{
  int mask, seen = 0;
  int i;

  for (i = 0; i < size; i++)
    {
      mask = a[i];
     if (!(mask & ~seen))
	continue;
      seen |= mask;
      if (seen != 0xff)
	printf ("1 << VREG_LANE (N) & %d ? %c%02x_REGS : ", mask, letter, mask);
      else
	printf ("%c%02x_REGS", letter, mask);
    }
}

static void
regno_reg_class_2 (void)
{
  printf ("#define REGNO_REG_CLASS(N) \\\n"
	  "  ((N) < ACC_REG \\\n"
	  "   ? ((N) < VR2_REG \\\n"
	  "      ? (");
  regno_reg_class_1 ('S', scalar_lanes, ARRAY_SIZE (scalar_lanes));
  printf (") \\\n"
	  "      : (");
  regno_reg_class_1 ('V', vec_lanes, VEC_NSETS);
  printf (")) \\\n"
	  "   : ((N) < VFLAGS_REG \\\n"
	  "      ? (");
  regno_reg_class_1 ('A', acc_lanes, ACC_NSETS);
  printf (") \\\n"
	  "      : (");
  regno_reg_class_1 ('F', flag_lanes, FLAG_NSETS);
  printf (")))\n\n");
}

/* Print FIXED_REGISTERS / CALL_USED_REGISTERS according to PREFIX.
   For call used registers, the call used scalars are marked in SCALAR_USED,
   and the call used non-scalar vector registers are marked in VREG_USED.  */
static void
print_fixed_regs (const char *prefix, long scalar_used, long vreg_used)
{
  int n_printed = 0;
  int i, j;

#define PRINT_VAL(VAL) \
  do \
    { \
      printf ("%d, ", (VAL) != 0); \
      if (++n_printed > 15) \
	{ \
	  printf ("\\\n  "); \
	  n_printed = 0; \
	} \
    } while (0)

  printf ("#define %s_REGISTERS \\\n{ \\\n  "
      "/* Scalar registers.  Stack pointer and bss base are fixed.  */ \\\n"
      "  ", prefix);
  for (i = 0; i < 16; i++)
    PRINT_VAL (i == STACK_POINTER_REGNUM || i == BSS_BASE_REGNUM
	       || ((1 << i) & scalar_used));
  printf ("/* vr2-vr30: non-scalar vector registers.  */ \\\n  ");
  for (i = 2; i < 31; i++)
    for (j = 0; j < regs_per_vreg; j++)
      PRINT_VAL ((1 << i) & vreg_used);
  printf ("/* vr31: call stack; vr32-vr61: reserved.  */ \\\n  ");
  for (i = 31; i < 62; i++)
    for (j = 0; j < regs_per_vreg; j++)
      PRINT_VAL (1);
  printf ("/* Accumulator and flags are not fixed to allow lane allocation.  */ \\\n  ");
  for (i = 62; i < 64; i++)
    for (j = 0; j < regs_per_vreg; j++)
      PRINT_VAL (scalar_used);
  for (i = 0; i < (int) ARRAY_SIZE (fake_reg_names); i++)
    printf ("1 /* %s */, ", fake_reg_names[i]);
  printf ("\\\n}\n\n");
}

static void
emit_regsets (void)
{
  int i, j;
  int n_printed;
  const char *name;
  int n_regs = 0;

#define COUNT_REG \
  do \
    { \
      n_regs++; \
      if (++n_printed > 7) \
	{ \
	  printf ("\\\n  "); \
	  n_printed = 0; \
	} \
    } while (0)

  printf ("#define REGISTER_NAMES \\\n{ \\\n"
	  "  \"i0\",   \"i1\",   \"i2\",   \"i3\",   \"i4\",   \"i5\",   \"i6\",   \"i7\", \\\n"
	  "  \"i8\",   \"i9\",   \"i10\",  \"i11\",  \"i12\",  \"i13\",  \"i14\",  \"i15\", \\\n  ");
  n_regs += 16;
  for (i = 2, n_printed = 0; i < 62; i++)
    {
      for (j = 0; j < 16 / MXP_UNITS_PER_MXP_VEC_REG; j++)
	{
	  if (j)
	    printf ("\"vr%d_%d\",%s", i, j * MXP_UNITS_PER_MXP_VEC_REG / 2,
		    " " + (i > 9));
	  else
	    printf ("\"vr%d\",%s", i, " " + (i > 9));
	  COUNT_REG;
	}
    }
  name = "acc";
  do
    {
      for (j = 0; j < 16 / MXP_UNITS_PER_MXP_VEC_REG; j++)
	{
	  if (j)
	    printf ("\"%s_%d\",%s", name, j, " " + (*name != 'a'));
	  else
	    printf ("\"%s\",%s", name, " " + (*name != 'a'));
	  COUNT_REG;
	}
    } while (*name == 'a' && (name = "vflags"));
  for (i = 0; i < (int) ARRAY_SIZE (fake_reg_names); i++)
    {
      printf ("\"%s\", ", fake_reg_names[i]);
      COUNT_REG;
    }
  if (n_printed)
    printf ("\\\n");
  printf("}\n\n");
  printf ("#define ADDITIONAL_REGISTER_NAMES"
	  " { { \"vr0\", 0}, { \"vr1\", 8} }\n\n");

  printf ("#define LAST_SCALAR_REG %d\n\n", 15);
  printf ("#define SCALAR_REGNO_P(N) \\\n"
	  "  ((N) <= LAST_SCALAR_REG || ((N) >= ");
  upputs (fake_reg_names[0]);
  printf ("_REG && n <= ");
  upputs (fake_reg_names[ARRAY_SIZE (fake_reg_names)-1]);
  printf ("_REG))\n");
  printf ("#define LANES_PER_REG %d\n", MXP_UNITS_PER_MXP_VEC_REG / 2);
  printf ("#define LANE0_REGNO(N) ((N) & -%d)\n", regs_per_vreg);
  printf ("#define VREG_LANE(N) (((N) & %d) * LANES_PER_REG)\n",
	  regs_per_vreg - 1);
  printf ("#define REGS_PER_VREG %d\n\n", regs_per_vreg);
  printf ("#define FIRST_PSEUDO_REGISTER %d\n\n", n_regs);
  regno_reg_class_2 ();

  print_fixed_regs ("FIXED", 0, 0);
  print_fixed_regs ("CALL_USED", 0xff, 0x1ffff);

  printf ("enum reg_class\n{");
  for (i = j = 0; i < N_RCLASS; i++)
    {
      if (!valid_regclass (i))
	continue;
      if (j++ % 4 == 0)
	printf ("\n  ");
      else
	printf (" ");
      emit_regclass_name (i);
      printf (",");
    }
  printf (" LIM_REG_CLASSES\n};\n\n"
	  "#define N_REG_CLASSES (int) LIM_REG_CLASSES\n\n"
	  "#define REG_CLASS_NAMES \\\n{");
  for (i = j = 0; i < N_RCLASS; i++)
    {
      if (!valid_regclass (i))
	continue;
      if (j++ % 4 == 0)
	printf (" \\\n  \"");
      else
	printf (" \"");
      emit_regclass_name (i);
      printf ("\",");
    }
  printf (" \"LIM_REG_CLASSES\" \\\n};\n\n");
  printf ("#define REG_CLASS_CONTENTS \\\n{ \\\n");
  for (i = 0; i < N_RCLASS; i++)
    {
      if (!valid_regclass (i))
	continue;
      printf ("/* ");
      emit_regclass_name (i);
      printf (" */ \\\n  { ");
      emit_regclass_content (i);
      printf (" }, \\\n");
    }
  printf ("}\n");
}

static void
emit_md (void)
{
  int n_regs = 0;
  int i;

  printf ("(define_constants\n");
  n_regs += 16; /* Scalar regs.  */
  printf ("  [(VR2_REG\t\t%d)\n", n_regs);
  n_regs += 29 * regs_per_vreg; /* Advance past vr2..vr30.  */
  printf ("   (CALL_STACK_REG\t%d)\n", n_regs);
  printf ("   (VR32_REG\t\t%d)\n", n_regs += regs_per_vreg);
  n_regs += 30 * regs_per_vreg; /* Advance past vr32..vr62.  */
  printf ("   (ACC_REG\t\t%d)\n", n_regs);
  printf ("   (ACCH_REG\t\t%d)\n", n_regs + regs_per_vreg/2);
  printf ("   (VFLAGS_REG\t\t%d)\n", n_regs += regs_per_vreg);
  printf ("   (VFLAGSH_REG\t\t%d)\n", n_regs + regs_per_vreg/2);
  n_regs += regs_per_vreg;
  for (i = 0; i < (int) ARRAY_SIZE (fake_reg_names); i++)
    {
      printf ("   (");
      upputs (fake_reg_names[i]);
      printf ("_REG\t\t%d)\n", n_regs++);
    }
  printf ("   ])\n");
}

static void
emit_c (void)
{
  int j;
  const char *s = "scalar_";
  int n_printed = 0;

  printf (
"/* For each register class, indicate the lanes relevant for potential\n"
"   cross-lane moves.  For move destinations, we can disregard scalar\n"
"   registers, since they allow cheap cross-lane moves.  */\n");
  do
    {
      printf ("\nunsigned char class_%svec_lanes[] =\n{\n ", s);
      for (j = 0; j < N_RCLASS; j++)
	{
	  int i = j;
	  DECODE_REGCLASS
    
	  if (!valid_regclass (j))
	    continue;
	  if (n_printed++ > 12)
	    {
	      printf ("\n ");
	      n_printed = 1;
	    }
	  printf (" %d,", vec | flags | acc | (*s ? scalar : 0));
	}
      printf ("\n};\n");
    }
  while (*s && (s = ""));
}

int
main (int argc, char **argv)
{
  bool gen_header = false, gen_md = false;
  char *progname = argv[0];

  if (argc == 1)
    ;
  else if (argc == 2 && !strcmp (argv[1], "-h"))
    gen_header = true;
  else if (argc == 2 && !strcmp (argv[1], "-m"))
    gen_md = true;
  else
    {
      error ("usage: %s -h|-m > file", progname);
      return FATAL_EXIT_CODE;
    }
  if (gen_header)
    emit_regsets ();
  else if (gen_md)
    emit_md ();
  else
    emit_c ();
  return 0;
}

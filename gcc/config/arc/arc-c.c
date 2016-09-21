/* Copyright (C) 2016 Free Software Foundation, Inc.

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "cpplib.h"
#include "tree.h"
#include "tm_p.h"
#include "cpplib.h"
#include "c-family/c-common.h"
#include "c-family/c-pragma.h"
#include "c-family/c-format.h"
#include "target.h"

#define builtin_define(TXT) cpp_define (pfile, TXT)
#define builtin_assert(TXT) cpp_assert (pfile, TXT)

/* Define or undefine macros based on the current target.  */

static void
def_or_undef_macro(cpp_reader* pfile, const char *name, bool def_p)
{
  if (def_p)
    cpp_define (pfile, name);
  else
    cpp_undef (pfile, name);
}

/* Define a single macro and emit a warning.  */

static void
arc_def_warning_macro (cpp_reader * pfile, const char * name)
{
  /* warning (OPT_Wdeprecated, "%s macro is deprecated\n", name); */
  builtin_define (name);
}

void
arc_cpu_cpp_builtins (cpp_reader * pfile)
{
  const arc_cpu_t *arc_default_cpu;
  builtin_assert ("cpu=arc");
  builtin_assert ("machine=arc");

  builtin_define ("__arc__");

#undef ARC_C_DEF
#define ARC_C_DEF(NAME, CONDITION)		\
  def_or_undef_macro (pfile, NAME, CONDITION);

#undef ARC_C_WARN_DEF
#define ARC_C_WARN_DEF(NAME, CONDITION)		\
  def_or_undef_macro (pfile, NAME, CONDITION);

#include "arc-c.def"
#undef ARC_C_DEF
#undef ARC_C_WARN_DEF

#ifdef TARGET_CPU_BUILD
  arc_default_cpu = &arc_cpu_types[(int) TARGET_CPU_BUILD];
#else
  arc_default_cpu = &arc_cpu_types[(int) TARGET_CPU_DEFAULT];
#endif

  switch (arc_default_cpu->arch)
    {
    case BASE_ARCH_6xx:
      arc_def_warning_macro (pfile, "_CPU_DEFAULT_A6");
      builtin_define ("__CPU_DEFAULT_A6__");
      break;
    case BASE_ARCH_700:
      arc_def_warning_macro (pfile, "_CPU_DEFAULT_A7");
      builtin_define ("__CPU_DEFAULT_A7__");
      break;
    case BASE_ARCH_hs:
      arc_def_warning_macro (pfile, "_CPU_DEFAULT_HS");
      builtin_define ("__CPU_DEFAULT_HS__");
      break;
    case BASE_ARCH_em:
      arc_def_warning_macro (pfile, "_CPU_DEFAULT_EM");
      builtin_define ("__CPU_DEFAULT_EM__");
      break;
    default:
      arc_def_warning_macro (pfile, "_CPU_DEFAULT_UNK");
      builtin_define ("__CPU_DEFAULT_UNK__");
      break;
    }

  builtin_define (TARGET_BIG_ENDIAN
		  ? "__BIG_ENDIAN__" : "__LITTLE_ENDIAN__");
  if (TARGET_BIG_ENDIAN)
    builtin_define ("__big_endian__");

}

void
arc_pr_jli_call_fixed (struct cpp_reader * pfile ATTRIBUTE_UNUSED)
{
  tree x;
  enum cpp_ttype type;
  char *symbol;
  int index;

  type = pragma_lex (&x);

  if (type != CPP_OPEN_PAREN)
  {
    error ("expected '(' in #pragma jli_call_fixed");

    return;
  }

  type = pragma_lex (&x);

  if (type != CPP_NAME)
  {
    error ("expected function name in #pragma jli_call_fixed");

    return;
  }

  symbol = strndup (IDENTIFIER_POINTER (x), 2048);

  type = pragma_lex (&x);

  if (type != CPP_COMMA)
  {
    free (symbol);

    error ("expected ',' in #pragma jli_call_fixed");

    return;
  }

  type = pragma_lex (&x);

  if (type != CPP_NUMBER || TREE_CODE (x) != INTEGER_CST)
  {
    free (symbol);

    error ("expected integer JLI index in #pragma jli_call_fixed");

    return;
  }

  index = TREE_INT_CST_LOW (x);

  if (0 > index || index >= ARC_JLI_ENTRIES_MAX)
  {
    free (symbol);

    error ("out of range JLI index in #pragma jli_call_fixed");

    return;
  }

  type = pragma_lex (&x);

  if (type != CPP_CLOSE_PAREN)
  {
    free (symbol);

    error ("expected ')' in #pragma jli_call_fixed");

    return;
  }

  if (jli_fixed_table[index] != NULL)
  {
    free (jli_fixed_table[index]);

    warning (OPT_Wpragmas, "function '%s' is already in the JLI table so the "
      "index will be replaced with %d", symbol, index);
  }
  else if ((jli_fixed_count + jli_dynamic_count) >= ARC_JLI_ENTRIES_MAX)
  {
    free (symbol);

    error ("there are too many JLI entries");

    return;
  }
  else
  {
    jli_dynamic_count++;
  }

  jli_fixed_table[index] = symbol;
}

void
arc_pr_jli_call_always (struct cpp_reader * pfile ATTRIBUTE_UNUSED)
{
  tree x;
  enum cpp_ttype type;
  char *symbol;

  type = pragma_lex (&x);

  if (type != CPP_OPEN_PAREN)
  {
    error ("expected '(' in #pragma jli_call_always");

    return;
  }

  type = pragma_lex (&x);

  if (type != CPP_NAME)
  {
    error ("expected function name in #pragma jli_call_always");

    return;
  }

  symbol = strndup (IDENTIFIER_POINTER (x), 2048);

  type = pragma_lex (&x);

  if (type != CPP_CLOSE_PAREN)
  {
    free (symbol);

    error ("expected ')' in #pragma jli_call_always");

    return;
  }

  if (arc_jli_fixed_symbol_index (symbol) >= 0 ||
    arc_jli_dynamic_symbol_index (symbol) >= 0)
  {
    free (symbol);

    warning (OPT_Wpragmas, "'%s' has already been registered as a JLI "
      "function; ignoring", symbol);

    return;
  }

  if ((jli_fixed_count + jli_dynamic_count) >= ARC_JLI_ENTRIES_MAX)
  {
    free (symbol);

    error ("there are too many JLI entries");

    return;
  }

  jli_dynamic_table[jli_dynamic_count++] = symbol;
}

void
arc_pr_init (void)
{
  int i;

  jli_fixed_count = 0;
  jli_dynamic_count = 0;

  for (i = 0; i < ARC_JLI_ENTRIES_MAX; ++i)
  {
    jli_fixed_table[i] = NULL;
    jli_dynamic_table[i] = NULL;
  }
}

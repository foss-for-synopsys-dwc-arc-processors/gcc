/* Common hooks for Synopsys DesignWare ARC
   Copyright (C) 1994, 1995, 1997, 1998, 2007-2012
   Free Software Foundation, Inc.
   Contributor: Joern Rennecke <joern.rennecke@embecosm.com>
		on behalf of Synopsys Inc.

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
#include "diagnostic-core.h"
#include "tm.h"
#include "common/common-target.h"
#include "opts.h"
#include "flags.h"
#include "params.h"

static void
arc_option_init_struct (struct gcc_options *opts)
{
  opts->x_flag_no_common = 255; /* Mark as not user-initialized.  */

  /* Which cpu we're compiling for (ARC600, ARC601, ARC700).  */
  arc_cpu = PROCESSOR_NONE;
}

/* Set default optimization options.  */
/* The conditions are incomplete, so we rely on the evaluation order here,
   which goes from first to last, i.e. the last match prevails.  */
/* ??? But this trick only works for reject_negative options.  Approximate
   missing option combination.  */
#define OPT_LEVELS_3_PLUS_SPEED_ONLY OPT_LEVELS_3_PLUS
static const struct default_options arc_option_optimization_table[] =
  {
    { OPT_LEVELS_SIZE, OPT_fsection_anchors, NULL, 1 },
    { OPT_LEVELS_1_PLUS, OPT_fomit_frame_pointer, NULL, 1 },
    { OPT_LEVELS_1_PLUS_SPEED_ONLY, OPT_fschedule_insns, NULL, 1 },
    { OPT_LEVELS_1_PLUS, OPT_fschedule_insns2, NULL, 1 },
    { OPT_LEVELS_ALL, OPT_msize_level_, NULL, 1 },
    { OPT_LEVELS_3_PLUS_SPEED_ONLY, OPT_msize_level_, NULL, 0 },
    { OPT_LEVELS_SIZE, OPT_msize_level_, NULL, 3 },
    { OPT_LEVELS_ALL, OPT_mearly_cbranchsi, NULL, 1 },
    { OPT_LEVELS_ALL, OPT_mbbit_peephole, NULL, 1 },
    { OPT_LEVELS_SIZE, OPT_mq_class, NULL, 1 },
    { OPT_LEVELS_SIZE, OPT_mcase_vector_pcrel, NULL, 1 },
    { OPT_LEVELS_NONE, 0, NULL, 0 }
  };

/* Implement TARGET_OPTION_DEFAULT_PARAMS.  */
static void
arc_option_default_params (void)
{
  /* Set the default values for cache-related parameters.  The ARC EM
     has as well prefetch instructions, however, its common
     configureations uses CCMs.  */
  set_default_param_value (PARAM_SIMULTANEOUS_PREFETCHES, 4);
  set_default_param_value (PARAM_PREFETCH_LATENCY, 4);
  set_default_param_value (PARAM_L1_CACHE_LINE_SIZE, 64);
}

/*  Process options.  */
static bool
arc_handle_option (struct gcc_options *opts,
		   struct gcc_options *opts_set ATTRIBUTE_UNUSED,
		   const struct cl_decoded_option *decoded,
		   location_t loc)
{
  size_t code = decoded->opt_index;
  int value = decoded->value;
  const char *arg = decoded->arg;
  static int mcpu_seen = PROCESSOR_NONE;
  const arc_cpu_t *arc_selected_cpu;
  static const arc_cpu_t *arc_default_cpu = &arc_cpu_types[0];
  static bool nompy_seen = false;
  static bool initialize = true;
  static int  init_flags = 0;
  static bool seen_arc_mpy_option = false;
  static bool seen_arc_fpu_build = false;

  /* First, initialize the options as for the default choosen cpu.  */
  if (initialize)
    {
      initialize = false;
      int default_cpu;
#if TARGET_CPU_DEFAULT == TARGET_CPU_EM
      default_cpu = PROCESSOR_arcem;
#elif TARGET_CPU_DEFAULT == TARGET_CPU_HS
      default_cpu = PROCESSOR_archs;
#else
      default_cpu = PROCESSOR_arc700;
#endif
      arc_default_cpu = &arc_cpu_types[(int) default_cpu];

#define ARC_OPT(NAME, CODE, MASK, DOC)		\
      do {					\
	if (arc_default_cpu->flags & CODE)	\
	  {					\
	    opts->x_target_flags |= MASK;	\
	    init_flags |= MASK;			\
	  }					\
      } while (0);
#define ARC_OPTX(NAME, CODE, VAR, VAL)		\
      do {					\
	if (arc_default_cpu->flags & CODE)	\
	  opts->x_##VAR = VAL;			\
      } while (0);
#include "config/arc/arc-options.def"

#undef ARC_OPTX
#undef ARC_OPT
   }

  switch (code)
    {
    case OPT_mcpu_:
      /* N.B., at this point arc_cpu has already been set to its new value by
	 our caller, so comparing arc_cpu with PROCESSOR_NONE is pointless.  */

      if (mcpu_seen != PROCESSOR_NONE && mcpu_seen != value)
	warning_at (loc, 0, "multiple -mcpu= options specified.");
      mcpu_seen = value;

      arc_selected_cpu = &arc_cpu_types[(int) mcpu_seen];

      /* Clean default set cpu related flags, taking into
	 consideration flags already turned off/on.  */
      opts->x_target_flags &= ~init_flags;
      init_flags = 0;

      /* Set cpu flags accordingly.  */
#define ARC_OPT(NAME, CODE, MASK, DOC)			\
      do {						\
	if (arc_selected_cpu->flags & CODE)		\
	  {						\
	    if (!(opts_set->x_target_flags & MASK))	\
	      {						\
		opts->x_target_flags |= MASK;		\
		init_flags |= MASK;			\
	      }						\
	  }						\
      } while (0);
#define ARC_OPTX(NAME, CODE, VAR, VAL)			\
      do {						\
	if (arc_selected_cpu->flags & CODE)		\
	  {						\
	    opts->x_##VAR = VAL;			\
	  }						\
	else if ((arc_default_cpu->flags & CODE)	\
		 && (!seen_##VAR))			\
	  opts->x_##VAR = 0;				\
      } while (0);

#include "config/arc/arc-options.def"

#undef ARC_OPTX
#undef ARC_OPT

      if (nompy_seen)
	opts->x_arc_mpy_option = 0;

      break;

    case OPT_mmpy16:
      /* If they are set/unset by the user clean them from default.  */
      init_flags &= ~MASK_MPY16_SET;
    case OPT_mmpy:
      init_flags &= ~MASK_MPY_SET;
      if (!value)
	{
	  opts->x_arc_mpy_option = 0;
	  nompy_seen = true;
	}
      break;

    case OPT_mmpy_option_:
      seen_arc_mpy_option = true;
      if (value < 0 || value > 9)
	error_at (loc, "bad value %qs for -mmpy-option switch", arg);
      break;

    case OPT_mfpu_:
      seen_arc_fpu_build = true;
      break;

    case OPT_mcode_density:
      init_flags &= ~MASK_CODE_DENSITY;
      break;

    case OPT_mdiv_rem:
      init_flags &= ~MASK_DIVREM;
      break;

    case OPT_mnorm:
      init_flags &= ~MASK_NORM_SET;
      break;

    case OPT_matomic:
      init_flags &= ~MASK_ATOMIC;
      break;

    case OPT_mll64:
      init_flags &= ~MASK_LL64;
      break;

    case OPT_mbarrel_shifter:
      init_flags &= ~MASK_BARREL_SHIFTER;
      break;

    case OPT_mswap:
      init_flags &= ~MASK_SWAP_SET;
      break;

    case OPT_mmul64:
      init_flags &= ~MASK_MUL64_SET;
      break;

    case OPT_mmul32x16:
      init_flags &= ~MASK_MULMAC_32BY16_SET;
      break;

    case OPT_mEA:
      init_flags &= ~MASK_EA_SET;
      break;

    case OPT_mspfp:
    case OPT_mspfp_compact:
    case OPT_mspfp_fast:
      init_flags &= ~MASK_SPFP_COMPACT_SET;
      break;

    case OPT_mdpfp:
    case OPT_mdpfp_compact:
    case OPT_mdpfp_fast:
      init_flags &= ~MASK_DPFP_COMPACT_SET;
      break;

    case OPT_margonaut:
      init_flags &= ~MASK_ARGONAUT_SET;
      break;

    case OPT_msimd:
      init_flags &= ~MASK_SIMD_SET;
      break;

    default:
      break;
    }

  return true;
}

#undef  TARGET_OPTION_DEFAULT_PARAMS
#define TARGET_OPTION_DEFAULT_PARAMS arc_option_default_params

#undef  TARGET_OPTION_INIT_STRUCT
#define TARGET_OPTION_INIT_STRUCT arc_option_init_struct

#undef  TARGET_OPTION_OPTIMIZATION_TABLE
#define TARGET_OPTION_OPTIMIZATION_TABLE arc_option_optimization_table

#define DEFAULT_NO_SDATA (TARGET_SDATA_DEFAULT ? 0 : MASK_NO_SDATA_SET)

#undef  TARGET_DEFAULT_TARGET_FLAGS
#define TARGET_DEFAULT_TARGET_FLAGS (DEFAULT_NO_SDATA | MASK_VOLATILE_CACHE_SET)

#undef  TARGET_HANDLE_OPTION
#define TARGET_HANDLE_OPTION arc_handle_option

#include "common/common-target-def.h"

struct gcc_targetm_common targetm_common = TARGETM_COMMON_INITIALIZER;

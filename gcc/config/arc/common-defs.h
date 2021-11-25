const arc_arch_t arc_arch_types[] =
  {
    {"none", BASE_ARCH_NONE, 0, 0},
#define ARC_ARCH(NAME, ARCH, FLAGS, DFLAGS)	\
    {NAME, BASE_ARCH_##ARCH, FLAGS, DFLAGS},
#include "arc-arches.def"
#undef ARC_ARCH
    {NULL, BASE_ARCH_END, 0, 0}
  };

const arc_cpu_t arc_cpu_types[] =
  {
    {"none", NULL, PROCESSOR_NONE, 0, HAS_NONE, ARC_TUNE_NONE},
#define ARC_CPU(NAME, ARCH, FLAGS, EXTRA, TUNE)				\
    {#NAME, &arc_arch_types [BASE_ARCH_##ARCH], PROCESSOR_##NAME, FLAGS, HAS_##EXTRA, ARC_TUNE_##TUNE },
#include "arc-cpus.def"
#undef ARC_CPU
    {NULL, NULL, PROCESSOR_NONE, 0, HAS_NONE, ARC_TUNE_NONE}
  };


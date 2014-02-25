/* { dg-do compile } */
/* { dg-require-effective-target arc_fpu_ok } */
/* { dg-options "-O2 -mcpu=ARCv2HS -mfpu=fpud_all" } */

#include "fma.h"

/* { dg-final { scan-assembler-times "fsmadd" 2 } } */
/* { dg-final { scan-assembler-times "fsmsub" 2 } } */
/* { dg-final { scan-assembler-times "bxor" 4 } } */

/* Check that we use the clri insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-xfail-if "Not available for ARCv1" { *-*-* } { "-mA7" "-mA5" "-mA6" "-mARC600" "-mARC700" "-mcpu=A5" "-mcpu=A600" "-mcpu=A601" "-mcpu=A700" } { "" } } */
/* { dg-options "-O2 -mEM" } */
/* { dg-final { scan-assembler "clri.*r" } } */
int foo (void)
{
  return __builtin_arc_clri();
}

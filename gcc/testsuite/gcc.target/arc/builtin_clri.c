/* Check that we use the clri insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-skip-if "Not available for ARCv1" { *-*-* } { "-mA7" "-mA5" "-mA6" "-mARC600" "-mARC700" "-mcpu=A5" "-mcpu=A600" "-mcpu=A601" "-mcpu=A700" "-marc700" "-mcpu=arc700" "-marc600" "-mcpu=arc600" "-marc601" "-mcpu=arc601" } { "" } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "\tclri.*r0" } } */
int foo (void)
{
  return __builtin_arc_clri();
}

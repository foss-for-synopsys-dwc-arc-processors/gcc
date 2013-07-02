/* Check that we use the ffs insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-xfail-if "Not available for ARCv1" { *-*-* } { "-mA7" "-mA5" "-mA6" "-mARC600" "-mARC700" "-mcpu=A5" "-mcpu=A600" "-mcpu=A601" "-mcpu=A700" } { "" } } */
/* { dg-options "-O2 -mnorm" } */
/* { dg-final { scan-assembler "ffs.*r0, r0" } } */
/* { dg-final { scan-assembler "ffs.*r0, 16" } } */
int foo (int x)
{
  return __builtin_arc_ffs(x);
}

int bar (void)
{
  return __builtin_arc_ffs(0x10);
}

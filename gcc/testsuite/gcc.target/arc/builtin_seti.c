/* Check that we use the seti insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-skip-if "Not available for ARCv1" { *-*-* } { "-mA7" "-mA5" "-mA6" "-mARC600" "-mARC700" "-mcpu=A5" "-mcpu=A600" "-mcpu=A601" "-mcpu=A700" } { "" } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "\tseti.*r0" } } */
/* { dg-final { scan-assembler "\tseti.*10" } } */
void foo (int x)
{
  __builtin_arc_seti(x);
}

void bar (void)
{
  __builtin_arc_seti(10);
}

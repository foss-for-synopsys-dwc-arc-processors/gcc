/* Check that we use the seti insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-skip-if "Not available for ARCv1" { arc6xx || arc700 } } */
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

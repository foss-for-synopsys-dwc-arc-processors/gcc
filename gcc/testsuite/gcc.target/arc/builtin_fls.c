/* Check that we use the fls insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-skip-if "Not available for ARCv1" { arc6xx || arc700 } } */
/* { dg-options "-O2 -mnorm" } */
/* { dg-final { scan-assembler "\tfls.*r0, r0" } } */
/* { dg-final { scan-assembler "\tfls.*r0, 16" } } */
int foo (int x)
{
  return __builtin_arc_fls(x);
}

int bar (void)
{
  return __builtin_arc_fls(0x10);
}

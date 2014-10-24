/* Check that we use the clri insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "lr.*r0, \\\[r0\\\]" } } */
/* { dg-final { scan-assembler "lr.*r0, \\\[33\\\]" } } */
int foo (int x)
{
  return __builtin_arc_lr(x);
}

int bar (void)
{
  return __builtin_arc_lr(0x21);
}

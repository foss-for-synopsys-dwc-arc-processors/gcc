/* Check that we use the clri insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "\tsr\t-1, \\\[33\\\]" } } */
/* { dg-final { scan-assembler "\tsr\t3, \\\[r1\\\]" } } */
/* { dg-final { scan-assembler "\tsr\tr0, \\\[34\\\]" } } */
void foo (int a, int *p)
{
  __builtin_arc_sr (0xffffffff, 0x21);
  __builtin_arc_sr (0x3, (unsigned int )p);
  __builtin_arc_sr (a, 0x22);
}


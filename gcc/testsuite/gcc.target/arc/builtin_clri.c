/* Check that we use the clri insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-skip-if "Not available for ARCv1" { arc6xx || arc700 } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "\tclri.*r0" } } */
int foo (void)
{
  return __builtin_arc_clri();
}

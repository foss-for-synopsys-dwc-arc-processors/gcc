/* { dg-do compile } */
/* { dg-options "-O0" } */
/* Check that naked functions don't place arguments on the stack at
   optimisation level '-O0'.  Also check that the standard return pattern
   is not added to the function.  */
void __attribute__((naked))
foo(int n, int m)
{
  bar (n + m);
}
/* { dg-final { scan-assembler "\tbl @bar" } } */

/* Look for things that would appear in a non-naked function, but which
   should not appear in a naked function.  */
/* { dg-final { scan-assembler-not "\tst r0" } } */
/* { dg-final { scan-assembler-not "\tj_s \\\[blink\\\]" } } */
/* { dg-final { scan-assembler-not "\tmov fp,sp" } } */

/* { dg-do compile } */
/* { dg-options "-O0" } */
/* Check that naked functions don't place arguments on the stack at
   optimisation level '-O0'.  */

#if defined(__HS__) || defined(__EM__)
# define ILINK "ilink"
#else
# define ILINK "ilink1"
#endif

void __attribute__((naked, interrupt(ILINK)))
foo(int n, int m)
{
  bar (n + m);
}
/* { dg-final { scan-assembler "\tbl @bar" } } */
/* { dg-final { scan-assembler "\trtie" { xfail { arc700 || arc6xx } } } } */
/* { dg-final { scan-assembler "j.*\[ilink1\]" { xfail { archs || arcem } } } } */

/* Look for things that would appear in a non-naked function, but which
   should not appear in a naked function.  */
/* { dg-final { scan-assembler-not "\tst.* " } } */
/* { dg-final { scan-assembler-not "\tmov fp,sp" } } */

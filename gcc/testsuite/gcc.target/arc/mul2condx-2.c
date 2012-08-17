/* { dg-do compile } */
/* { dg-options "-O2" } */

/* Something in the GCC 4.4 peephole2 implementation is wrong.  */
extern void g (int a);
int
f (unsigned a, int b)
{
  a = (a >> 31);

  g (a * b);
  return a;
}

/* { dg-final { scan-assembler-not "mpyu" { xfail *-*-* } } } */

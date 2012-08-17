/* { dg-do compile } */
/* { dg-options "-O2" } */

int s0;

int
f(a, b, c, d)
  unsigned a, b, c, d;
{
  s0 = a - b;
  return c - d - (a < b);
}

/* { dg-final { scan-assembler "sub\.f\[ \t\]+r" } } */
/* { dg-final { scan-assembler "sbc\[ \t\]+r\[0-9\]+, *r\[0-9\]+ *,r" } } */

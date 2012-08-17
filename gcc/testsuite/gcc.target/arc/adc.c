/* { dg-do compile } */
/* { dg-options "-O2" } */

int
f (unsigned a, unsigned b, unsigned c, unsigned d)
{
  int s = 0;
  a ^= 1;
  if (a + b < a)
    s = 1;
  s += c + d;
  return s;
}

int s0;

int
g (unsigned a, unsigned b, unsigned c, unsigned d)
{
  int s = 0;
  if (a + b < a)
    s = 1;
  s0 = a+b;
  s += c + d;
  return s;
}
/* { dg-final { scan-assembler "add\.f\[ \t\]+0," } } */
/* { dg-final { scan-assembler "add\.f\[ \t\]+r" } } */
/* { dg-final { scan-assembler-times "adc\[ \t\]r\[0-9\]+, *r\[0-9\]+ *,r" 2 } } */

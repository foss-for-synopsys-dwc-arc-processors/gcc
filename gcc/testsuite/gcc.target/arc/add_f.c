/* { dg-do compile } */
/* { dg-options "-O2" } */
int
f (int a, int b, int c)
{
  if (a+b)
    c = 1;
  return c;
}

int
g (unsigned a, unsigned b)
{
  if (a + b < a)
    return 43;
  return 42;
}

int
h (unsigned a, unsigned b)
{
  if (a + b < b)
    return 43;
  return 42;
}
/* { dg-final { scan-assembler-times "add\.f\[ \t\]+0," 3 } } */
/* { dg-final { scan-assembler "mov\.eq\[ \t\]" } } */
/* { dg-final { scan-assembler-times "mov\.(hs|lo)\[ \t\]" 2 } } */
/* { dg-final { scan-assembler "mov\.(hs|lo)\[ \t\]" } } */

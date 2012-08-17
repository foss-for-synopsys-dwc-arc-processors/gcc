/* { dg-do compile } */
/* { dg-options "-O2" } */

int
f (unsigned a, int b)
{
  return (a >> 31) * b;
}

int
g (unsigned a, int b)
{
  return b * (a >> 31);
}

int
h (unsigned b, unsigned a)
{
  return (a >> 31) * b;
}

int
i (unsigned a)
{
  a = (a >> 31);
  return a * a;
}

int
j (unsigned a, int b)
{
  a = (a >> 31);

  return a * b + (42^a);
}
/* { dg-final { scan-assembler-not "mpyu" } } */

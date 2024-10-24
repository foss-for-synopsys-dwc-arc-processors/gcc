/* { dg-do compile } */
/* { dg-skip-if "avoid conflicts with -mcpu options" { *-*-* } { "-mcpu=*" } { "-mcpu=em" } } */
/* { dg-options "-O2 -mcpu=em -mswap" } */

int foo(int x)
{
  return ((unsigned int)x >> 16) | (x << 16);
}

/* { dg-final { scan-assembler "swap\\s+r0,r0" } } */

/* Simple test for scaled load addressed.  */
/* { dg-do compile } */
/* { dg-options "-O2" } */

const int table[2] = {1, 2};

int
foo (char i)
{
  return table[i];
}

/* { dg-final { scan-assembler "ld.as" } } */

/* { dg-do compile } */
/* { dg-options "-O2 -mtune=rhx" } */

int
f (int x)
{
  begin:
  if (x <= 3)
    goto begin;
}

/* { dg-final { scan-assembler "\\sli\\sa5,3\n\\sbge\\sa5,a0,.L\[0-9\]+\n" } } */

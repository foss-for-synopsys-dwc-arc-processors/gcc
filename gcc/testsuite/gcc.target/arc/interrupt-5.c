/* Check if ilink is recognize. Check how FP is save */
/* { dg-do compile } */
/* { dg-skip-if "Not available for ARCv1" { arc700 || arc6xx } } */
/* { dg-options "-O2 -mirq-ctrl-saved=r0-ilink" } */
/* { dg-final { scan-assembler-not "\tst.*fp,\\\[sp" } } */

#include <alloca.h>

extern int bar (void *);

void  __attribute__ ((interrupt("ilink"))) foo(void)
{
  int *p = alloca (10);
  bar (p);
}

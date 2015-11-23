/* Check if the registers R0-R3,blink are automatically saved. */
/* { dg-do compile } */
/* { dg-skip-if "Not available for ARCv1" { arc700 ||  arc6xx } } */
/* { dg-options "-O2 -mirq-ctrl-saved=r0-r3,blink" } */
/* { dg-final { scan-assembler-not "st.*r0,\\\[sp" } } */
/* { dg-final { scan-assembler-not "st.*r1,\\\[sp" } } */
/* { dg-final { scan-assembler-not "st.*r2,\\\[sp" } } */
/* { dg-final { scan-assembler-not "st.*r3,\\\[sp" } } */
/* { dg-final { scan-assembler-not "push_s blink" } } */

extern int bar (void *);

void  __attribute__ ((interrupt("ilink"))) foo(void)
{
  bar (0);
  __asm__ volatile ( "#dude" : : : "r0","r1","r2","r3");
}

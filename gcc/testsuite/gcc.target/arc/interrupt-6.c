/* Check if the registers R0-R3 are automatically saved. GP is saved by the compiler. */
/* { dg-do compile } */
/* { dg-skip-if "Not available for ARCv1" { arc700 || arc6xx } } */
/* { dg-options "-O2 -mirq-ctrl-saved=r0-r3" } */
/* { dg-final { scan-assembler "st.*gp,\\\[sp" } } */
/* { dg-final { scan-assembler-not "st.*r0,\\\[sp" } } */
/* { dg-final { scan-assembler-not "st.*r1,\\\[sp" } } */
/* { dg-final { scan-assembler-not "st.*r2,\\\[sp" } } */
/* { dg-final { scan-assembler-not "st.*r3,\\\[sp" } } */
/* { dg-final { scan-assembler "rtie" } } */

int a;

void  __attribute__ ((interrupt("ilink"))) foo1(void)
{
  a++;
  __asm__ volatile ( "#dude" : : : "r0","r1","r2","r3");
}

/* { dg-do compile } */
/* { dg-skip-if "ilink1 is not an ARCv2 attribute" { arcem || archs } } */

void __attribute__ ((interrupt("ilink1")))
handler1 (void)
{
}
/* { dg-final { scan-assembler-times "j.*\[ilink1\]" 1 } } */

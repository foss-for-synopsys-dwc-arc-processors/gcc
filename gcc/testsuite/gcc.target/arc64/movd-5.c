/* { dg-do compile } */
/* { dg-options "-O0 -fomit-frame-pointer" } */

/* check "movd-1.c" for further details. */

/* assign immediate to a memory: this immediate is small *
 * enough to be covered by w6 (signed 6 bit number).     */
int mem;
void foo(void)
{
  mem = 0x00;    /* the usual suspect: 0 */
  mem =  31;     /* largest positive number in w6 */
  mem = -32;     /* smallest negative number in w6 */
  mem = -1;      /* just a -1 (all bits 1) */
}
/* { dg-final { scan-assembler "st\\s+0,\\\[@mem\\\]" } } */
/* { dg-final { scan-assembler "st\\s+31,\\\[@mem\\\]" } } */
/* { dg-final { scan-assembler "st\\s+-32,\\\[@mem\\\]" } } */
/* { dg-final { scan-assembler "st\\s+-1,\\\[@mem\\\]" } } */

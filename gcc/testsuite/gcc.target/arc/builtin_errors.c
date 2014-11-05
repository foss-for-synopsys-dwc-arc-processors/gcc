/* { dg-do compile } */

void test_err_sleep (int a)
{
  __builtin_arc_sleep (a); /* { dg-error "builtin operand should be an unsigned 6-bit value" } */
}

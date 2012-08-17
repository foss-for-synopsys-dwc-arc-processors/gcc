/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mxy" } */
void fn(void)
{
#ifndef __Xxy
#error macro __Xxy not defined for -mxy flag
#endif
}

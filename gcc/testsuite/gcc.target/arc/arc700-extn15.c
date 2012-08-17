/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mtelephony" } */
void fn(void)
{
#ifndef __Xtelephony
#error macro __Xtelephony not defined for -mtelephony flag
#endif
}

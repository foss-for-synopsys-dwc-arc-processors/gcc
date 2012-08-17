/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-msimd" } */
void fn(void)
{
#ifndef __Xsimd
#error macro __Xsimd not defined for -msimd flag
#endif
}

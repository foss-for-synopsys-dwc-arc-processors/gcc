/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mno-mpy" } */
void fn(void)
{
#ifndef __Xno_mpy
#error macro __Xno_mpy not defined for -mno-mpy flag
#endif
}

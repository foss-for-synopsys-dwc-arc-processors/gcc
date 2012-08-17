/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mEA" } */
void fn(void)
{
#ifndef __Xea
#error macro __Xea not defined for -mEA flag
#endif
}

/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mdvbf" } */
void fn(void)
{
#ifndef __Xdvbf
#error macro __Xdvbf not defined for -mdvbf flag
#endif
}

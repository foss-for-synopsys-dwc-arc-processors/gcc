/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mswap" } */
void fn(void)
{
#ifndef __Xswap
#error macro __Xswap not defined for -mswap flag
#endif
}

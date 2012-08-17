/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mbarrel_shifter" } */
void fn(void)
{
#ifndef __Xbarrel_shifter
#error macro __Xbarrel_shifter not defined for -mbarrel_shifter flag
#endif
}

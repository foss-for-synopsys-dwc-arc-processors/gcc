/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mmin_max" } */
void fn(void)
{
#ifndef __Xmin_max
#error macro __Xmin_max not defined for -mmin_max flag
#endif
}

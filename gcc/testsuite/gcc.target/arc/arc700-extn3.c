/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mnorm" } */
void fn(void)
{
#ifndef __Xnorm
#error macro __Xnorm not defined for -mnorm flag
#endif
}

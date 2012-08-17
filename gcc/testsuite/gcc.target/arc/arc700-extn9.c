/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mdpfp" } */
void fn(void)
{
#ifndef __Xdpfp
#error macro __Xdpfp not defined for -mdpfp flag
#endif
}

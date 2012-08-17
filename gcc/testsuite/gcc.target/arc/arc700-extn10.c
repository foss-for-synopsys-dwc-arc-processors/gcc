/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mdpfp_compact" } */
void fn(void)
{
#ifndef __Xdpfp
#error macro __Xdpfp not defined for -mdpfp_compact flag
#endif
}

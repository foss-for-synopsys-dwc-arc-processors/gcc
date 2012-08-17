/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mdsp_packa" } */
void fn(void)
{
#ifndef __Xdsp_packa
#error macro __Xdsp_packa not defined for -mdsp_packa flag
#endif
}

/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mrtsc" } */
void fn(void)
{
#ifndef __Xrtsc
#error macro __Xrtsc not defined for -mrtsc flag
#endif
}

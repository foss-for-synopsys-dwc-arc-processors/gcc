/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mswape" } */
void fn(void)
{
#ifndef __Xswape
#error macro __Xswape not defined for -mswape flag
#endif
}

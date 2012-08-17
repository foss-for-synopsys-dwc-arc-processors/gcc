/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mcrc" } */
void fn(void)
{
#ifndef __Xcrc
#error macro __Xcrc not defined for -mcrc flag
#endif
}

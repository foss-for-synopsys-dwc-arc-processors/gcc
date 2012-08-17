/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mlock" } */
void fn(void)
{
#ifndef __Xlock
#error macro __Xlock not defined for -mlock flag
#endif
}

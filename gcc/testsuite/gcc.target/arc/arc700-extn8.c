/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mspfp_fast" } */
void fn(void)
{
#ifndef __Xspfp
#error macro __Xspfp not defined for -mspfp_fast flag
#endif
}

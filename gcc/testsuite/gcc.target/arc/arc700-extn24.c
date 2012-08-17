/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mmul64 -mARC600" } */
void fn(void)
{
#ifndef __Xmult32
#error macro __Xmult32 not defined for -mmul64 flag
#endif
}

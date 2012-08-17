/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mmac_d16" } */
void fn(void)
{
#ifndef __Xxmac_d16
#error macro __Xxmac_d16 not defined for -mmac_d16 flag
#endif
}

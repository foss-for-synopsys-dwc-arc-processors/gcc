/* Look for preprocessor macros to be defined for given target options.  */
/* { dg-do compile } */
/* { dg-options "-mmac_24" } */
void fn(void)
{
#ifndef __Xxmac_24
#error macro __Xxmac_24 not defined for -mmac_24 flag
#endif
}

/* { dg-do compile } */
/* { dg-options "-O3" { target { archs } } } */
/* { dg-options "-O3 -mcpu=archs" { target { ! { clmcpu } } } } */

/* This tests checks if we merge the two 32-bit stores into one 64-bit
   store.  It also checks if we use std w6,[reg] format.  */

typedef struct {
  unsigned long __val[2];
} sigset_t;

int sigemptyset2 (sigset_t *set)
{
  set->__val[0] = 0;
  set->__val[1] = 0;
  return 0;
}

/* { dg-final { scan-assembler "std 0,\\\[r" } } */

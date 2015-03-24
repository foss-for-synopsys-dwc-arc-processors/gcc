/* builtin_return_address (n) test when using millicode calls.  */

/* { dg-do run } */
/* { dg-options "-O1" } */

#include <stdlib.h>
#include <stdio.h>

extern void abort (void);

static void *p[2];

/* Os makes use of millicode thunks.  */
void __attribute__((noinline, optimize ("-Os")))
bar (void)
{
  asm ("" ::
       : "r10", "r11", "r12", "r13", "r14",
         "r15", "r16", "r17", "r18", "r19",
         "r20", "r21", "r22", "r23", "r24");
  p[1] = __builtin_extract_return_addr (__builtin_return_address (0));
}

void  __attribute__((noinline))
foo (void)
{
  p[0] = __builtin_extract_return_addr (__builtin_return_address (0));
}


int main ()
{
  foo ();
  bar ();
  /* The length of bl insn is 4.  */
  if (p[1] - p[0] != 4)
    abort ();
  return 0;
}

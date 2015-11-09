/* { dg-do compile } */
/* { dg-require-effective-target archs }*/
/* { dg-options "-O0 -mirq-ctrl-saved=r0-r9" } */

/* Check that on archs the 'firq' interrupt function type is available,
   these are the fast interrupts.  For fast interrupts, despite the use of
   'irq-ctrl-saved', no registers are automatically saved on entry to the
   function, and so, in the following register r0 to r9 should all be saved
   to the stack.

   We also take the opportunity to check the use of the 'rtie' instruction
   at the end of the interrupt function.  */

void __attribute__ ((interrupt("firq")))
handler1 (void)
{
  asm (""
       :
       :
       : "r0", "r1", "r2", "r3", "r4",
         "r5", "r6", "r7", "r8", "r9");
}
/* { dg-final { scan-assembler "\tstd r2,\\\[sp,\[0-9\]+\\\]" } } */
/* { dg-final { scan-assembler "\tstd r4,\\\[sp,\[0-9\]+\\\]" } } */
/* { dg-final { scan-assembler "\tstd r6,\\\[sp,\[0-9\]+\\\]" } } */
/* { dg-final { scan-assembler "\tstd r8,\\\[sp,\[0-9\]+\\\]" } } */
/* { dg-final { scan-assembler "\trtie" } } */

/* Check that we use the sync insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-final { scan-assembler "\tsync" } } */
int foo (int x)
{
  __builtin_arc_sync();
}

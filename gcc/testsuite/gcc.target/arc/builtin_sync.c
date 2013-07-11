/* Check that we use the sync insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-skip-if "Only available in A7 and V2 architectures" { *-*-* } { "-mA5" "-mA6" "-mARC600" "-mcpu=A5" "-mcpu=A600" "-mcpu=A601" } { "" } } */
/* { dg-final { scan-assembler "\tsync" } } */
int foo (int x)
{
  __builtin_arc_sync();
}

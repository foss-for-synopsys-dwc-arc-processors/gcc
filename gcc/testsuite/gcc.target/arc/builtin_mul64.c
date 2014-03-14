/* Check that we use the ffs insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-skip-if "Avoid multiple cpu option warning" { "arc*-*-*" } { "-mcpu=*" "-mav*" "-mA*" } { "" } } */
/* { dg-options "-O2 -mmul64 -mcpu=ARC600" } */
/* { dg-final { scan-assembler "\tmul64.*0, r0, r1" } } */
/* { dg-final { scan-assembler "\tmul64.*0, r0, 16" } } */
void foo (int x, int y)
{
  __builtin_arc_mul64(x,y);
}

void bar (int x)
{
  __builtin_arc_mul64(x, 0x10);
}


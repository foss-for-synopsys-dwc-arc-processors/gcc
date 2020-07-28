/* { dg-do compile } */
/* { dg-skip-if "" { ! { clmcpu } } } */
/* { dg-options "-mcpu=hs38 -Os -EB" } */

/* This example is found during big-endian build.  The compiler is
   matching mpyd.eq r0,r1,r0 as a predicated instruction, which is
   incorrect.  The error is due to different predicates between the
   output operand and the first operand of the instruction.  */
typedef int a;
typedef int b __attribute__((mode(DI)));
struct c {
  a d, e;
};
typedef union {
  struct c f;
  b g;
} h;
b i(b j) {
  h k = {.g = j};
  a l;
  if (__builtin_expect(k.f.d == k.f.e, 1))
    return k.f.e * (b)l;
}

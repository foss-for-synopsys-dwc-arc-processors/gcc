/* { dg-do compile } */
/* { dg-options "-O2 -w" } */

/* Check how we handle empty body loops.  */

a;
fn1() {
  int i;
  for (; i < 8; i++) {
    double A[a];
  }
}

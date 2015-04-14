/* { dg-do run } */
/* { dg-options "-O2 -mabi=mwabi" } */
/* { dg-additional-sources "abitest1.S" } */

struct st_a {
  int a[2];
};

extern void abort (void);
extern int pargs01 (int a0, int a1, int a2, int a3, int a4, int a5, int a6, struct st_a b, int c);
extern int pargs02 (int a0, int a1, int a2, int a3, int a4, int a5, int a6, long long b, int c);

int main (void)
{
  struct st_a x = {10, 11};
  int a;

  a = pargs01 (0,1,2,3,4,5,6, x, 7);
  if (a != 28)
    abort ();

  a = pargs02 (0,1,2,3,4,5,6, 0x0000000A0000000BLL, 7);
  if (a != 28)
    abort ();

  return 0;
}

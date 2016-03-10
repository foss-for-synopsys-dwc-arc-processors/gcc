/* { dg-do run } */
/* { dg-options "-O2" } */

#include <stdio.h>
#include <float.h>

#define RUNTIME_TEST(TYPE,X,OP,Y,RES)			\
  do {							\
    volatile TYPE foo;					\
    foo = (TYPE) (X);					\
    foo OP##= (TYPE) (Y);				\
    if (foo != (TYPE) (RES))				\
      {							\
	printf ("Runtime computation error @%d. %g "	\
		"!= %g\n", __LINE__, foo, (TYPE)RES);	\
	error = 1;					\
      }							\
  } while (0)

#define RUNTIME_TEST_NAN(TYPE,X,OP,Y)			\
  do {							\
    volatile TYPE foo;					\
    foo = (TYPE) (X);					\
    foo OP##= (TYPE) (Y);				\
    if (!isnan (foo))					\
      {							\
	printf ("Runtime computation error @%d. %g "	\
		"!= NaN\n", __LINE__, foo);		\
	error = 1;					\
      }							\
  } while (0)

#define H __builtin_inf ()
#define Q __builtin_nan ("")
/* Special type of NaN found when using double FPX instructions.  */
static const unsigned long long __nan = 0x7FF0000080000000ULL;
#define W (*(double *) &__nan)

int main (void)
{
  int error = 0;

  /* divisions by 10.  */
  RUNTIME_TEST (double, 40,    /, 10,   4);
  RUNTIME_TEST (double, -40,   /,-10,   4);
  RUNTIME_TEST (double, -40,   /, 10,  -4);
  RUNTIME_TEST (double, 40,    /,-10,  -4);
  RUNTIME_TEST (double, 32760, /, 10,   3276);
  RUNTIME_TEST (double, 10000, /, 10,   1000);
  RUNTIME_TEST (double, 10000, /, 100,  100);
  RUNTIME_TEST (double, 10000, /, 1000, 10);

  /* inf/small number -> inf.  */
  RUNTIME_TEST (double,  H, /, 1, H);
  RUNTIME_TEST (double,  H, /,  DBL_MIN,  H);
  RUNTIME_TEST (double, -H, /,  DBL_MIN, -H);
  RUNTIME_TEST (double,  H, /, -DBL_MIN, -H);
  RUNTIME_TEST (double, -H, /, -DBL_MIN,  H);

  RUNTIME_TEST (double,  H, /,  DBL_MAX,  H);
  RUNTIME_TEST (double, -H, /,  DBL_MAX, -H);
  RUNTIME_TEST (double,  H, /, -DBL_MAX, -H);
  RUNTIME_TEST (double, -H, /, -DBL_MAX,  H);

  /* 1/inf -> 0.  */
  RUNTIME_TEST (double, 1, /, H, 0);
  /* Tiny / Inf -> 0.  */
  RUNTIME_TEST (double,  DBL_MIN, /,  H,  0);
  RUNTIME_TEST (double, -DBL_MIN, /,  H, -0);
  RUNTIME_TEST (double,  DBL_MIN, /, -H, -0);
  RUNTIME_TEST (double, -DBL_MIN, /, -H,  0);

  RUNTIME_TEST (double,  DBL_MAX, /,  H,  0);
  RUNTIME_TEST (double, -DBL_MAX, /,  H, -0);
  RUNTIME_TEST (double,  DBL_MAX, /, -H, -0);
  RUNTIME_TEST (double, -DBL_MAX, /, -H,  0);

  /* inf/0 -> inf.  */
  RUNTIME_TEST (double,  H, /,  0,  H);
  RUNTIME_TEST (double, -H, /,  0, -H);
  RUNTIME_TEST (double,  H, /, -0,  H);
  RUNTIME_TEST (double, -H, /, -0, -H);

  /* 0 / Inf --> 0 with no problem.  */
  RUNTIME_TEST (double,  0, /,  H,  0);
  RUNTIME_TEST (double, -0, /,  H, -0);
  RUNTIME_TEST (double,  0, /, -H, -0);
  RUNTIME_TEST (double, -0, /, -H,  0);

  /* zero versus NaN.  */
  RUNTIME_TEST_NAN (double,  Q, /,  0);
  RUNTIME_TEST_NAN (double,  Q, /, -0);
  RUNTIME_TEST_NAN (double,  0, /,  Q);
  RUNTIME_TEST_NAN (double, -0, /,  Q);

  RUNTIME_TEST_NAN (double,  W, /,  0);
  RUNTIME_TEST_NAN (double,  W, /, -0);
  RUNTIME_TEST_NAN (double,  0, /,  W);
  RUNTIME_TEST_NAN (double, -0, /,  W);

  /* infinity versus infinity.  */
  RUNTIME_TEST_NAN (double,  H, /,  H);
  RUNTIME_TEST_NAN (double, -H, /,  H);
  RUNTIME_TEST_NAN (double,  H, /, -H);
  RUNTIME_TEST_NAN (double, -H, /, -H);

  /* infinity versus NaN.  */
  RUNTIME_TEST_NAN (double,  H, /,  Q);
  RUNTIME_TEST_NAN (double, -H, /,  Q);
  RUNTIME_TEST_NAN (double,  H, /, -Q);
  RUNTIME_TEST_NAN (double, -H, /, -Q);

  RUNTIME_TEST_NAN (double,  H, /,  Q);
  RUNTIME_TEST_NAN (double, -H, /,  Q);
  RUNTIME_TEST_NAN (double,  H, /, -W);
  RUNTIME_TEST_NAN (double, -H, /, -W);

  /* zero versus zero.  */
  RUNTIME_TEST_NAN (double,  0, /,  0);

  if (error)
    __builtin_abort();

  return 0;
}

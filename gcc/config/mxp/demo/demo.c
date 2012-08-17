#include <stdio.h>
#include "demo.h"

static unsigned long rand_seed = 47114711;
long
simple_rand ()
{
  unsigned long this = rand_seed * 1103515245 + 12345;
  rand_seed = this;
  return this >> 8;
}

int
main (int argc, char **argv)
{
  int i;
  unsigned long res;

  if (argc > 1)
    rand_seed = atol (argv[1]);
  for (i = 0; i < sizeof (s) / sizeof (ELEMTYPE); i++)
    {
      long el = simple_rand () & 0x7ff;

      /* Make this a 15 bit signed number so that XORing two of these will
	 again yeild a 15 bit signed numbers, and adding two of those will
	 fit in a short.  */
      s.a[i] = (el & 0x4000) ? -0x8000 + el : el;
      /* Hack: Using s.a to access all of s.  */
      s.a[i] = el;
    }
  res = f ();
  printf ("result is:%ld 0x%lx\n", res, res);
  return 0;
}

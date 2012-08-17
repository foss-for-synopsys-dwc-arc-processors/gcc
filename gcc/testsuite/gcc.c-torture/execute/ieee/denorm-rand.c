/* Test proper rounding for division when generating subnormal numbers.  */
/* x86 needs "-mfpmath=sse -msse2" to pass this test.  */

/* The debug code is only meant to run on linux with long double support.  */
#define dEBUG

long long
simple_rand ()
{
  static unsigned long long seed = 47114711;
  unsigned long long this = seed * 1103515245 + 12345;
  seed = this;
  return this >> 8;
}

unsigned long long int
random_bitstring (int *lenp)
{
  unsigned long long int x;
  int n_bits;
  int tot_bits = 0;
  int len = 0;
  int limit = *lenp;

  x = 0;
  for (;;)
    {
      long long ran = simple_rand ();
      int ones_p = ran & 1;
      n_bits = (ran >> 1) % 16;
      if (x || ones_p)
	{
	  if (n_bits > limit - len)
	    n_bits = limit - len;
	  *lenp = len += n_bits;
	}
      tot_bits += n_bits;

      if (n_bits == 0 && len)
	return x;
      else if (n_bits)
	{
	  x <<= n_bits;
	  if (ones_p)
	    x |= (1 << n_bits) - 1;

	  if (len == limit
	      || (len && tot_bits > 8 * sizeof (long long) + 6))
	    return x;
	}
    }
}

main ()
{
  int i;
  double e0, e1, ie1, i2e1;
  /* Hack to get x86 math to work.  */
  volatile double eval_tmp;
#define EVAL(x) (eval_tmp = (x))

  for (i = 0, e1 = 0.5; EVAL (1. + e1) > 1.; e1 *= 0.5, i++);
  e1 *= 2.;
  ie1 = 1./e1;
  i2e1 = 2./e1;
  if (i < 52 || (sizeof (double) == 8 && i > 52))
    abort ();
  for (i = 1, e0 = 0.5; EVAL (e0 * e0); e0 *= e0, i <<= 1);
  for (;EVAL (e0 * 0.5); e0 *= 0.5, i++);
  if (i < 0x3ff + 51 || (sizeof (double) == 8 && i > 0x3ff + 51))
    abort ();
  
  /* First, check that a quotient that can be computed exactly is properly
     rounded, and try variantions on the fraction to do some simple
     round-to-nearest checks for inexact results.  */
  for (i = 0; i < 1000; i++)
    {
      unsigned long long x, y;
      int xlen, ylen;
      long long ran;
      double xd, xr, yd, yd2, e2y, pd, ep;

      xlen = 53;
      x = random_bitstring (&xlen);
      ylen = 54-xlen;
      y = random_bitstring (&ylen);
      y |= 1;
      if (x > 2 &&
	  (double) (x|3) * y >= (double) (1LL << 53))
	{
	  x >>= 1;
	  xlen--;
	}
      x |= 1;
      ran = simple_rand ();
      if (xlen > 1)
	x ^= ran & 2;
      xd = (double)x * e0;
      yd = (double)y * ie1;
      yd2 = yd * 2.;
      e2y = (double) (1LL << ylen);
      if (EVAL (yd2 + e2y) == yd2)
	abort ();
      if ((yd2 + e2y) / yd2 > (1.+e1)/1.)
	abort ();
      pd = xd * yd;
      ep = e0 * (1LL << xlen-1) * (1LL << ylen-1);
      if (EVAL (pd + ep) == pd)
	ep += ep;
      if (pd + ep == pd)
	abort ();
      if (EVAL((pd + ep) / pd) > EVAL (1 + e1))
	abort ();
      if (EVAL (pd / yd) != x * e0)
	abort ();
      /* Round to even.  */
      xr = ((x & 2) + x) >> 1;
      if (EVAL (pd / yd2) != xr * e0)
	abort ();
      /* Round to nearest - upwards.  */
      xr = x+1 >> 1;
      if (EVAL (pd / (yd2-e2y)) < xr * e0)
	abort ();
      if (EVAL ((pd + ep) / yd2) < xr * e0)
	abort ();
      /* Round to nearest - downwards.  */
      xr = x >> 1;
      if (EVAL (pd / (yd2+e2y)) > xr * e0)
	abort ();
      if (EVAL ((pd - ep) / yd2) > xr * e0)
	abort ();
    }
  /* Now generate a set of 53 bit random numbers, calculate a fractional
     approximation which is likely to be hard to distinguish from the
     exact result, and check for proper rounding.  */
  for (i = 0; i < 1000; i++)
    {
      unsigned long long x, y, x0, y0, x1, y1, x2, y2, x3, tmp;
      int rest_sign;
      long long ran;
      int xlen;
      long long a[20];
      int j, k;

      do
	{
	  xlen = 53;
	  x = random_bitstring (&xlen);
	}
      while (xlen < 10);
      x |= 1;
      x0 = x;
      /* Look for a close, but inexact approximation that fits in 53 bits
	 numerator / denominator.  */
      y = 1LL << xlen - 1;
      y0 = y;
      rest_sign = 0;
      for (j = 0; j < 20; j++)
	{
	  a[j] = x/y;
	  x1 = a[j], y1 = 1;
	  for (k = j - 1; k >= 0; k--)
	    {
	      tmp = a[k] * x1 + y1;
	      if (tmp >= 1LL << 53 || tmp/a[k] < x1)
		goto end_approx;
	      y1 = x1;
	      x1 = tmp;
	    }
	  tmp = x - a[j] * y;
	  if (!tmp)
	    break;
	  rest_sign = j & 1 ? -1 : 1;
	  x2 = x1, y2 = y1;
	  x = y;
	  y = tmp;
	}
      end_approx:
      if (!rest_sign)
	continue;
#ifdef DEBUG
      printf ("%d %d %f %f %e\n", j, rest_sign,
	      (double)x0/y0, (double)x2/y2,
	      (double)((long double)x0/y0-((long double)x2/y2)));
#endif
      x3 = EVAL (x2*e0*y0/(2.*y2)) / e0 * 2.;
#ifdef  DEBUG
      printf ("%llx %llx\n", x0, x3);
#endif
      if (rest_sign > 0 ? x3 >= x0 : x3 <= x0)
#ifdef DEBUG
	printf("ERROR!\n");
#else
	abort ();
#endif
    }

  exit (0);
}

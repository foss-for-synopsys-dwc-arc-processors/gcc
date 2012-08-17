extern void abort(void);

void
check_add (long long a, long long b)
{
  union u { unsigned long long ull; double d; };
  union u a2, b2, c2;
  long long c;

  a2.ull = (a < 0) ? (-a | 1ULL << 63) : a;
  b2.ull = (b < 0) ? (-b | 1ULL << 63) : b;
  c2.d = a2.d + b2.d;
  c = (c2.ull & 1ULL << 63) ? - (c2.ull & (~0ULL >> 1)) : c2.ull;
  if (c != a + b)
    abort ();
}

int
main ()
{
  long long i, j;

  for (i = -12; i < 12; i++)
    for (j = -12; j < 12; j++)
      check_add (i, j);
  for (i = -0x200000000LL; i < 0x200000000LL; i += 0x10000000)
    for (j = -0x200000000LL; j < 0x200000000LL; j += 0x10000000)
      check_add (i, j);
  return 0;
  for (i = 1LL << 52; i >>= 1; i)
    for (j = 1LL << 52; j >>= 1; j)
      {
        check_add (i, j);
        check_add (-i, j);
        check_add (i, -j);
        check_add (-i, -j);
      }
  return 0;
}

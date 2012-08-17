/* /cc1 -O3 t1.c -quiet -fdump-tree-all  -ftree-vectorize -fdump-tree-vect-details -mint16 */

#include "demo.h"

unsigned long
f ()
{
  int i, j0, k0, j, k;
  unsigned long maxsum = 0;

  for (j0 = 0; j0 < LEN; j0 += 8)
    for (k0 = 0; k0 < LEN; k0 += 8)
      {
	ELEMTYPE max = -0x7fff-1;

	for (j = j0, k = k0, i = 0; i < LEN; i++, j++, k++)
	  {
	    ELEMTYPE sum = (s.a[i] ^ s.b[j]) + (s.a[i] ^ s.c[k]);

	    max = sum > max ? sum : max;
	  }
	maxsum += max;
      }
 return maxsum;
}

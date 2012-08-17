/* The ARCompact port needs to keep track of alignment so that it can
   add the right adjustments when using pcl.  This test would fail
   when the alignment was not updated for the first switch, and
   then the second would use the wrong table base address and wrong
   entries.  -Os is relevant both because it causes case table shortening,
   and because it inhibits alignments after barriers which otherwise mask
   the problem.  */
/* { dg-do run } */
/* { dg-options "-Os" } */

#include <stdlib.h>

volatile int i;

int
main (int argc, char **argv)
{
  switch (argc)
    {
    case 0: i = 1; break;
    case 1: i++; break;
    case 2: i--; break;
    case 3: i = 2; break;
    case 4: i <<= 1; break;
    }
  i = 2;
  switch (i)
    {
    case 0: exit (1);
    case 1: abort ();
    case 2: exit (0);
    case 3: exit (7);
    case 4: exit (5);
    }
}

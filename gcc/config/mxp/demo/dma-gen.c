/* Generate an assembly file with set-up code to place before the linked
   simd text section.
   This will kick of a DMA copy of initialized data (read-only and
   ordinary initialized data are supposed to be concatenated) from main
   memory to SDM, and then start recording to SCM.
   Command line parameters:
    1: transfer source address in main memory.
    2: transfer destination address in SDM
    3: transfer size
    4: SCM recording start address  */

#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char **argv)
{
  int sdm_start, size;
  char *main_start;
  int i;
  int stride, lines;

  if (argc < 4)
    exit (1);

  main_start = argv[1];
  sdm_start = strtol (argv[2], NULL, 0);
  size = strtol (argv[3], NULL, 0);
  /* Setting up multiple transfers is expensive, so prefer to transfer a
     little bit more instead.  The worst this will do for sizes up to
     37 KB is to transfer 10 extra 32-bit words.  */
  i = (size + 3) >> 2;
    {
      int x_start = (i > 63 ? 63 : i);
      int x_end = (i <= 255 ? 1 : (i+254)/255);
      int x;
      int min = i;
      int min_x = 0, min_y = 0;

      for (x = x_start, min = i; x >= x_end; x--)
	{
	  int y = (i + x - 1) / x;
	  int r;

	  if (y > 255)
	    y = 255;
	  r = x * y - i;
	  if (r < 0)
	    abort ();
	  if (min > r)
	    {
	      min = r;
	      min_x = x;
	      min_y = y;
	    }
	  if (r == 0)
	    break;
	}
#if 0
      printf ("%d %d %d %d\n", i, min_x, min_y, min);
#endif
      stride = min_x * 4;
      lines = min_y;
    }
  if (size == lines * stride)
    printf ("; Copying 0x%x bytes.\n", size);
  else
    printf ("; Need to copy 0x%x bytes, copying 0x%x instead.\n",
	    size, lines * stride);
  printf ("\tvdiwr dr0,0x%x; SDM start\n", sdm_start);
  printf ("\tmov_s r12,%d\n", stride);
  printf ("\tvdiwr dr1,r12; SDM stride\n");
  printf ("\tvdiwr dr2,0x1f%02x%02x; copy %d lines of %d bytes\n",
	  lines, stride, lines, stride);
  printf ("\tvdiwr dr4,%s; main memory start\n", main_start);
  printf ("\tvdiwr dr5,r12; main memory stride\n");
  printf ("\tvdirun pcl,pcl; start dma transfer\n");
  exit (0);
}

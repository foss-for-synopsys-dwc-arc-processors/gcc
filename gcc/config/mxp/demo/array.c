#include "demo.h"

struct array_struct s;

struct {} dma_end;

/* ??? gcc won't accept alias data definition - but this works for our
   purposes.  */
int dma_start () __attribute__ ((alias("s")));

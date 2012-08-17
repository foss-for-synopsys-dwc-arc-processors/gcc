#define LEN 1440
/* ??? vectorization won't take place for 'short' even if it's the same
   data layout as 'int'  */
#if defined (__ARC600__) || defined (__ARC700__)
#define ELEMTYPE short
#else
#define ELEMTYPE int
#endif

extern struct array_struct { ELEMTYPE a[LEN], b[LEN*2], c[LEN*2]; } s;

extern unsigned long maxsum;

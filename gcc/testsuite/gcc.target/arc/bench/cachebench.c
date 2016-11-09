#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <time.h>

#include <fcntl.h>
#include <assert.h>

#include <unistd.h>
#include <malloc.h>
#include "llcbench.h"

#define ARRAY(x,i,j,m) *(x+((j)*m)+(i))

#if defined(USE_DOUBLE)
#ifndef DATATYPE
#define DATATYPE double
#define DATASTRING "double"
#endif
#elif defined(USE_INT)
#ifndef DATATYPE
#define DATATYPE int
#define DATASTRING "int"
#endif
#elif defined(USE_CHAR)
#ifndef DATATYPE
#define DATATYPE char
#define DATASTRING "char"
#endif
#elif defined(USE_FLOAT)
#ifndef DATATYPE
#define DATATYPE float
#define DATASTRING "float"
#endif
#endif

#ifndef DATATYPE
#define DATATYPE double
#define DATASTRING "double"
#endif

#define READONLY 1<<0
#define WRITEONLY 1<<1
#define READWRITE 1<<2
#define NOCALIBRATE 1<<3
#define GUESSCACHESIZE 1<<4
#define MEMORYSET 1<<5
#define MEMORYCOPY 1<<6
#define HANDTUNED 1<<7

/* DEFAULT SETTINGS */

#define CACHE_MIN_BITS (8) /* 1<<bits is smallest cache in bytes */
#define CACHE_MAX_BITS (10) /* 1<<bits is largest cache in bytes */
#define CACHE_MIN (1L<<CACHE_MIN_BITS)
#define CACHE_MAX (1L<<CACHE_MAX_BITS)
#define RESOLUTION (2)
#define TIME_SLOTS (2L*(CACHE_MAX_BITS-CACHE_MIN_BITS+1))
#define MEASURE_TIME 5
#define DOUBLE_NS(x) ((double)x)*1.0e9
#define THRESHOLD (0.9) /* 1st level must be faster than 2nd level */
#define REPEAT_COUNT 1

extern char *optarg;

static long logmemsize = CACHE_MAX_BITS, memsize = 1L<<CACHE_MAX_BITS, repeat_count = REPEAT_COUNT;
static long resolution = RESOLUTION, timeslots, duration = MEASURE_TIME, type = NOCALIBRATE;
static long noflush = 0;

volatile long keepgoing = 0;

static int my_isatty(int fd)
{
  return 1;    /* Force results to flush quicker. */
  /* return isatty(fd);*/
}

long go(long a)
{
  if (a==-1)
    return(keepgoing);
  else
    return(keepgoing = a);
}

void handler(int a)
{
  go(0);
}

void compute_stats(long i, long j, double refcnt, double overhead_per_ref,
		   double *times, double *bws, double *percents, long *sizes,
		   double tmicrosec, long datasize)
{
  if (tmicrosec > 0.0)
    {
  ARRAY(times,i,j,repeat_count) =
    ((tmicrosec*1000.0) - (refcnt*overhead_per_ref))/refcnt;

  ARRAY(bws,i,j,repeat_count) =
    ((refcnt*datasize)/(1024.0*1024.0)) /
    ((ARRAY(times,i,j,repeat_count)*refcnt)/1.0E9);
    }

  if (i == 0)
    {
      if (j == 0)
	ARRAY(percents,i,j,repeat_count) = 1.0;
      else
	{
	  ARRAY(percents,i,j,repeat_count) =
	    ARRAY(times,i,j,repeat_count) /
	    ARRAY(times,i,j-1,repeat_count);
	}
    }
  else
    {
      if (j == 0)
	{
	  ARRAY(percents,i,j,repeat_count) =
	    ARRAY(times,i,j,repeat_count) /
	    ARRAY(times,i-1,repeat_count-1,repeat_count);
	}
      else
	{
	  ARRAY(percents,i,j,repeat_count) =
	    ARRAY(times,i,j,repeat_count) /
	    ARRAY(times,i,j-1,repeat_count);
	}
    }

  if (my_isatty(1))
    {
      printf("%-15ld %-15.2f %-15.2f %-15.2f\n",
	       sizes[i],
	       ARRAY(times,i,j,repeat_count),
	       ARRAY(bws,i,j,repeat_count),
	       ARRAY(percents,i,j,repeat_count));
    }
  else
    printf("%ld %f\n",
	   sizes[i],
	   ARRAY(bws,i,j,repeat_count));
}

void compute_cache_sizes(double *times, double *bws, double *percents, long *sizes)
{
  long i, maxb = 0, maxa = 0, cachea, cacheb;

  /* Look for 2 highest percentages */

  for (i=0; i < timeslots; i++)
    if (percents[i] > percents[maxa])
      maxa = i;

  for (i=0; i < timeslots; i++)
    {
      if ((i != maxa) && (percents[i] > percents[maxb]))
	{
	  maxb = i;
	}
    }

  printf("\n\t\t\tCache Capacity Analysis\n\n");

  if (1.0/percents[maxa] >= THRESHOLD)
    {
      printf("No L1 limit found, must be larger than %ld bytes.\n",memsize);
      return;
    }

  /* Set them to the index if the entry for that cache size */
  /* Remember our percents are relative to the previous entry */

  cachea = sizes[maxa-1];
  cacheb = sizes[maxb-1];

  if (cachea > cacheb)
    printf("Level 1 Cache: %ld bytes\n",cachea);
  else
    {
      printf("Level 1 Cache: %ld bytes\n",cachea);
      if ((times[maxa]/times[maxb] < THRESHOLD) && (1.0/percents[maxb] < THRESHOLD))
	printf("Level 2 Cache: %ld bytes\n",cacheb);
    }
}

#if 0
long usage(long argc, char **argv)
{
  long c;
  long errflg = 0;

  while ((c = getopt(argc, argv, "m:d:hrwbcsptx:e:N")) != -1)
    switch (c) {
    case 'm':
      if (((logmemsize = atol(optarg)) < 0) || (logmemsize <= CACHE_MIN_BITS))
	errflg++;
      memsize = 1L<<logmemsize;
      break;
    case 'd':
      if ((duration = atoi(optarg)) < 0)
	errflg++;
      break;
    case 's':
      type |= MEMORYSET;
      break;
    case 'p':
      type |= MEMORYCOPY;
      break;
    case 'r':
      type |= READONLY;
      break;
    case 'h':
      errflg++;
      break;
    case 't':
      type |= HANDTUNED;
      break;
    case 'w':
      type |= WRITEONLY;
      break;
    case 'b':
      type |= READWRITE;
      break;
    case 'c':
      type ^= NOCALIBRATE;
      break;
    case 'g':
      type |= GUESSCACHESIZE;
      break;
    case 'x':
      if ((resolution = atoi(optarg)) < 0)
	errflg++;
      resolution++; /* Correct for my usage */
      break;
    case 'e':
      if ((repeat_count = atoi(optarg)) <= 0)
	errflg++;
      break;
    case 'N':
      noflush = 1;
      break;
    case '?':
      errflg++;
      break; }

  if ((type & (READONLY|WRITEONLY|READWRITE|MEMORYCOPY|MEMORYSET)) == 0)
    {
      if (my_isatty(1))
	type |= READONLY|WRITEONLY|READWRITE|MEMORYCOPY|MEMORYSET;
      else
	type |= READWRITE;
    }

  if (errflg)
    {
      fprintf(stderr, "Usage: %s -rwbtsp [-x #] [-m #] [-d #] [-e #]\n",argv[0]);
      fprintf(stderr, "\t -r Read benchmark\n");
      fprintf(stderr, "\t -w Write benchmark\n");
      fprintf(stderr, "\t -b Read/Modify/Write benchmark\n");
      fprintf(stderr, "\t -t Use hand tuned versions of the above\n");
      fprintf(stderr, "\t -s memset() benchmark\n");
      fprintf(stderr, "\t -p memcpy() benchmark\n");
      /* fprintf(stderr, "\t -c Enable calibration code\n");
      fprintf(stderr, "\t -g Enable cache size guessing code\n"); */
      fprintf(stderr, "\t -x Number of measurements between powers of 2.\n");
      fprintf(stderr, "\t -m Specify the log2(available physical memory)\n");
      fprintf(stderr, "\t -d Number of seconds per iteration\n");
      fprintf(stderr, "\t -e Repeat count per cache size\n\n");
      fprintf(stderr, "\t -N Do not flush the cache between problem sizes.\n");
      fprintf(stderr, "Datatype used is %s, %ld bytes.\n",DATASTRING,(long)sizeof(DATATYPE));
      fprintf(stderr, "Defaults if  tty: -rwbsp -x%d -m%d -d%d -e%d\n",
	      RESOLUTION-1,CACHE_MAX_BITS,MEASURE_TIME,REPEAT_COUNT);
      fprintf(stderr, "Defaults if file: -b   -x%d -m%d -d%d -e1\n",
	      RESOLUTION-1,CACHE_MAX_BITS,MEASURE_TIME);
      exit(1);
    }

  timeslots = resolution*(logmemsize - CACHE_MIN_BITS) + 1;

  DBG(printf("%ld %ld %ld %ld\n",logmemsize,memsize,duration,timeslots));

  return(type);
}
#endif

void fake_out_optimizations(DATATYPE *x, long bytes)
{
  static long fd = -1;

  if (fd == -1)
    assert(fd=open("/dev/null",O_WRONLY));
  assert(write(fd,(void *)x,bytes>sizeof(DATATYPE)?sizeof(DATATYPE):bytes)!=-1);
}

/* double calibrate_benchmark_ronly(REGISTER DATATYPE *x, REGISTER long limit, long *oloops, double *ous)
{
  REGISTER long index = 0, loops = 0;
  REGISTER DATATYPE sum = 0, foolem = 0;

  flushall(x);
  keepgoing = 1;
  assert(signal(SIGALRM,handler) != SIG_ERR);
  limit -= 4; foolem = (DATATYPE)limit;

  alarm(duration);
  TIMER_START;
again:
  sum += foolem + foolem+(DATATYPE)1 + foolem+(DATATYPE)2 + foolem+(DATATYPE)3;
  if (((index+=4) < limit) && (keepgoing))
    goto again;
  else if (keepgoing)
    {
      index = 0;
      loops++;
      goto again;
    }
  TIMER_STOP;
  index += 4;

  x[0] = (DATATYPE)sum;
  x[1] = (DATATYPE)index;
  fake_out_optimizations(x,2*sizeof(DATATYPE));

  *oloops = loops;
  *ous = TIMER_ELAPSED;
  return(((double)loops*(double)limit)+(double)index);
}

double calibrate_benchmark_wonly(REGISTER DATATYPE *x, REGISTER long limit, long *oloops, double *ous)
{
  REGISTER long index = 0, loops = 0;
  REGISTER DATATYPE sum1 = (DATATYPE)0, sum2 = (DATATYPE)0, sum3 = (DATATYPE)0, sum4 = (DATATYPE)0;

  FLUSHALL(1);
  keepgoing = 1;
  assert(signal(SIGALRM,handler) != SIG_ERR);
  limit -= 4;

  alarm(duration);
  TIMER_START;
again:
  sum1++;
  sum2++;
  sum3++;
  sum4++;
  if (((index+=4) < limit) && (keepgoing))
    goto again;
  else if (keepgoing)
    {
      index = 0;
      loops++;
      goto again;
    }
  TIMER_STOP;
  index += 4;

  x[0] = (DATATYPE)sum1;
  x[1] = (DATATYPE)sum2;
  fake_out_optimizations(x,2*sizeof(DATATYPE));

  return(((double)loops*(double)limit)+(double)index);
}

double calibrate_benchmark(REGISTER DATATYPE *x, REGISTER long to_do_loops, REGISTER long limit, double *ous)
{
  REGISTER long index = 0, loops = 0;
  REGISTER DATATYPE sum1 = (DATATYPE)0;

  TIMER_START;
  while (loops < to_do_loops)
    {
      for (index = 0; index < limit; index++)
	{
	  sum1++;
	}
      loops++;
    }
  TIMER_STOP;

  x[0] = (DATATYPE)sum1;
  x[1] = (DATATYPE)index;
  fake_out_optimizations(x,2*sizeof(DATATYPE));

  *ous = TIMER_ELAPSED;
  {
    double refcnt = ((double)loops*(double)limit)+(double)index;
    DBG(fprintf(stderr,"C: %ld loops at limit %ld took %f us, %f refs.\n",loops,limit,*ous,refcnt));
    return(refcnt);
  }
} */

double benchmark_cache_ronly(REGISTER DATATYPE *x, REGISTER long limit, long *oloops, double *ous)
{
  REGISTER long index = 0, loops = 0;
  REGISTER DATATYPE sum = (DATATYPE)0;

  FLUSHALL(1);
  keepgoing = 1;
  /* assert(signal(SIGALRM,handler) != SIG_ERR); */

  /* alarm(duration); */
  TIMER_START;

#ifdef SOLARIS
  while (go(-1))
#else
    // while (keepgoing)
#endif
    {
      for  (index = 0; index < limit; index++)
	{
	  sum += x[index];
	}
      loops++;
    }

  TIMER_STOP;

  x[0] = (DATATYPE)sum;
  x[1] = (DATATYPE)index;
  fake_out_optimizations(x,2*sizeof(DATATYPE));

  *oloops = loops;
  *ous = TIMER_ELAPSED;
  {
    double refcnt = ((double)loops*(double)limit);
    DBG(fprintf(stderr,"T: %ld loops at limit %ld took %f us, %f refs.\n",loops,limit,*ous,refcnt));
    return(refcnt);
  }
}

double hand_benchmark_cache_ronly(REGISTER DATATYPE *x, REGISTER long limit, long *oloops, double *ous)
{
  REGISTER long index = 0, loops = 0;
  REGISTER DATATYPE sum = (DATATYPE)0;
  REGISTER DATATYPE sum2 = (DATATYPE)0;

  FLUSHALL(1);
  keepgoing = 1;
  /* assert(signal(SIGALRM,handler) != SIG_ERR); */
  limit -= 8;

  /* alarm(duration); */
  TIMER_START;

again:
  sum += x[index] + x[index+1] + x[index+2] + x[index+3];
  sum2 += x[index+4] + x[index+5] + x[index+6] + x[index+7];
  if ((index += 8) < limit)
    goto again;
  else if (keepgoing)
    {
      index = 0;
      loops++;
      goto again;
    }

  TIMER_STOP;
  index += 8;

  x[0] = (DATATYPE)sum + (DATATYPE)sum2;
  x[1] = (DATATYPE)index;
  fake_out_optimizations(x,2*sizeof(DATATYPE));

  *oloops = loops;
  *ous = TIMER_ELAPSED;
  {
    double refcnt = ((double)loops*(double)limit);
    DBG(fprintf(stderr,"T: %ld loops at limit %ld took %f us, %f refs.\n",loops,limit,*ous,refcnt));
    return(refcnt);
  }
}

double benchmark_cache_wonly(REGISTER DATATYPE *x, REGISTER long limit, long *oloops, double *ous)
{
  REGISTER long index = 0, loops = 0;
  REGISTER DATATYPE wval = (DATATYPE)0xf;

  FLUSHALL(1);
  keepgoing = 1;
  /* assert(signal(SIGALRM,handler) != SIG_ERR); */
  wval = (DATATYPE)limit;

  /* alarm(duration); */
  TIMER_START;

  // while (keepgoing)
    {
      for  (index = 0; index < limit; index++)
	x[index] = wval;
      loops++;
    }

  TIMER_STOP;

  fake_out_optimizations(x,limit*sizeof(DATATYPE));

  *oloops = loops;
  *ous = TIMER_ELAPSED;
  {
    double refcnt = ((double)loops*(double)limit);
    DBG(fprintf(stderr,"T: %ld loops at limit %ld took %f us, %f refs.\n",loops,limit,*ous,refcnt));
    return(refcnt);
  }
}

double hand_benchmark_cache_wonly(REGISTER DATATYPE *x, REGISTER long limit, long *oloops, double *ous)
{
  REGISTER long index = 0, loops = 0;
  REGISTER DATATYPE wval = (DATATYPE)0xf;

  FLUSHALL(1);
  keepgoing = 1;
  /* assert(signal(SIGALRM,handler) != SIG_ERR); */
  limit -= 8; wval = (DATATYPE)limit;

  /* alarm(duration); */
  TIMER_START;

again:
  x[index] = wval;
  x[index+1] = wval;
  x[index+2] = wval;
  x[index+3] = wval;
  x[index+4] = wval;
  x[index+5] = wval;
  x[index+6] = wval;
  x[index+7] = wval;
  if ((index+=8) < limit)
    goto again;
  else if (keepgoing)
    {
      index = 0;
      loops++;
      goto again;
    }

  TIMER_STOP;
  index += 8;

  fake_out_optimizations(x,limit*sizeof(DATATYPE));

  *oloops = loops;
  *ous = TIMER_ELAPSED;
  {
    double refcnt = ((double)loops*(double)limit);
    DBG(fprintf(stderr,"T: %ld loops at limit %ld took %f us, %f refs.\n",loops,limit,*ous,refcnt));
    return(refcnt);
  }
}

double benchmark_cache(REGISTER DATATYPE *x, REGISTER long limit, long *oloops, double *ous)
{
  REGISTER long index = 0, loops = 0;

  FLUSHALL(1);
  keepgoing = 1;
  /* assert(signal(SIGALRM,handler) != SIG_ERR); */

  /* alarm(duration); */
  TIMER_START;

  // while (keepgoing)
    {
      for (index = 0; index < limit; index++)
	x[index]++;
      loops++;
    }

  TIMER_STOP;

  fake_out_optimizations(x,limit*sizeof(DATATYPE));

  *oloops = loops;
  *ous = TIMER_ELAPSED;
  {
    double refcnt = ((double)loops*(double)limit);
    DBG(fprintf(stderr,"T: %ld loops at limit %ld took %f us, %f refs.\n",loops,limit,*ous,refcnt));
    return(refcnt);
  }
}

double hand_benchmark_cache(REGISTER DATATYPE *x, REGISTER long limit, long *oloops, double *ous)
{
  REGISTER long index = 0, loops = 0;

  FLUSHALL(1);
  keepgoing = 1;
  /* assert(signal(SIGALRM,handler) != SIG_ERR); */

  /* alarm(duration); */
  TIMER_START;

again:
  x[index]++;
  x[index+1]++;
  x[index+2]++;
  x[index+3]++;
  x[index+4]++;
  x[index+5]++;
  x[index+6]++;
  x[index+7]++;
  if ((index+=8) <= limit-8)
    goto again;
  else if (keepgoing)
    {
      index = 0;
      loops++;
      goto again;
    }

  TIMER_STOP;

  fake_out_optimizations(x,limit*sizeof(DATATYPE));

  *oloops = loops;
  *ous = TIMER_ELAPSED;
  {
    double refcnt = ((double)loops*(double)limit);
    DBG(fprintf(stderr,"T: %ld loops at limit %ld took %f us, %f refs.\n",loops,limit,*ous,refcnt));
    return(refcnt);
  }
}


double benchmark_cache_memory_copy(REGISTER void *x, REGISTER void *y, REGISTER long bytes, long *oloops, double *ous)
{
  REGISTER long loops = 0;

  FLUSHALL(1);
  keepgoing = 1;
  /* assert(signal(SIGALRM,handler) != SIG_ERR); */

  /* alarm(duration); */
  TIMER_START;

  // while (keepgoing)
    {
      memcpy(x,y,bytes);
      loops++;
    }

  TIMER_STOP;

  fake_out_optimizations(x,bytes);
  fake_out_optimizations(y,bytes);

  *ous = TIMER_ELAPSED;
  *oloops = loops;
  return((double)loops*(double)bytes);
}

double benchmark_cache_memory_set(REGISTER void *x, REGISTER long bytes, long *oloops, double *ous)
{
  REGISTER long loops = 0;

  FLUSHALL(1);
  keepgoing = 1;
  /* assert(signal(SIGALRM,handler) != SIG_ERR); */

  /* alarm(duration); */
  TIMER_START;

  // while (keepgoing)
    {
      memset(x,0xf0,bytes);
      loops++;
    }

  TIMER_STOP;

  fake_out_optimizations(x,bytes);

  *ous = TIMER_ELAPSED;
  *oloops = loops;
  return((double)loops*(double)bytes);
}

void initialize_sizes(long *sizes)
{
  long i,j;

  for (j=0; j<timeslots; j+=resolution)
    {
      sizes[j] = 1L<<(CACHE_MIN_BITS+j/resolution);
      DBG(printf("POW: %ld %ld\n",j,sizes[j]));

      for (i=1;i<resolution;i++)
	{
	  if (j+i < timeslots)
	    {
	      sizes[j+i] = sizes[j] + i*(sizes[j]/resolution);
	      sizes[j+i] = sizes[j+i] - sizes[j+i]%(long)sizeof(DATATYPE);
	      DBG(printf("SUB: %ld %ld\n",j+i,sizes[j+i]));
	    }
	}
    }
}

void do_memory_copy(long *sizes, void *x, double *times, double *bws, double *percents)
{
  long limit, j, i, tloops;
  double refcnt, overhead_per_ref = 0.0, tmicrosec;
  /* double nullcnt = 0.0, cmicrosec = 0.0; */
  void *y;

  assert(y = (void *)malloc(memsize));
  memset(y,0x0f,memsize);

  if (my_isatty(1))
    {
      printf("\n\t\t%sMemory Copy Library Cache Test\n\n",
	     (!(type & NOCALIBRATE)) ? "Calibrated " : "");
      printf("C Size\t\tNanosec\t\tMB/sec\t\t%% Chnge\n");
      printf("-------\t\t-------\t\t-------\t\t-------\n");
    }

  for (i = 0; i < timeslots; i++)
    {
      limit = sizes[i];
      for (j = 0; j < repeat_count; j++)
	{
	  refcnt = benchmark_cache_memory_copy(x, y, limit, &tloops, &tmicrosec) * 2.0;

	  /* if (type & NOCALIBRATE)
	    {
	      nullcnt = 0.0;
	      overhead_per_ref = 0.0;
	      cmicrosec = 0.0;
	    }
	  else
	    {
	      nullcnt = calibrate_benchmark_cache_memory_copy(x, tloops, limit, &cmicrosec);
	      overhead_per_ref = (cmicrosec*1000.0) / nullcnt;
	      DBG(fprintf(stderr,"C: %f ns per ref.\n",overhead_per_ref));
	    } */

	  compute_stats(i,j,refcnt,overhead_per_ref,times,bws,percents,sizes,tmicrosec,1);
	}
    }

  /* if ((my_isatty(1))&&(type&GUESSCACHESIZE))
    compute_cache_sizes(times,bws,percents,sizes); */

  free(y);
}

void do_memory_set(long *sizes, DATATYPE *x, double *times, double *bws, double *percents)
{
  long limit, j, i, tloops;
  double refcnt, overhead_per_ref = 0.0, tmicrosec;
  /* double nullcnt = 0.0, cmicrosec = 0.0; */

  if (my_isatty(1))
    {
      printf("\n\t\t%sMemory Set Library Cache Test\n\n",
	     ((!(type & NOCALIBRATE)) ? "Calibrated " : ""));

      printf("C Size\t\tNanosec\t\tMB/sec\t\t%% Chnge\n");
      printf("-------\t\t-------\t\t-------\t\t-------\n");
    }

  for (i = 0; i < timeslots; i++)
    {
      limit = sizes[i];
      for (j = 0; j < repeat_count; j++)
	{
	  refcnt = benchmark_cache_memory_set(x, limit, &tloops, &tmicrosec);

	  /* if (type & NOCALIBRATE)
	    {
	      nullcnt = 0.0;
	      overhead_per_ref = 0.0;
	      cmicrosec = 0.0;
	    }
	  else
	    {
	      nullcnt = calibrate_benchmark_cache_memory_copy(x, tloops, limit, &cmicrosec);
	      overhead_per_ref = (cmicrosec*1000.0) / nullcnt;
	      DBG(fprintf(stderr,"C: %f ns per ref.\n",overhead_per_ref));
	    } */

	  compute_stats(i,j,refcnt,overhead_per_ref,times,bws,percents,sizes,tmicrosec,1);
	}
    }

  /* if ((my_isatty(1))&&(type&GUESSCACHESIZE))
    compute_cache_sizes(times,bws,percents,sizes); */
}

void do_read_only(long *sizes, DATATYPE *x, double *times, double *bws, double *percents)
{
  long limit, j, i, tloops;
  double refcnt, overhead_per_ref = 0.0, tmicrosec;
  /* double nullcnt = 0.0, cmicrosec = 0.0; */

  if (my_isatty(1))
    {
      printf("\n\t\t%s%s%s Read Cache Test\n\n",
	     ((type & HANDTUNED) ? "Tuned " : ""),
	     ((!(type & NOCALIBRATE)) ? "Calibrated " : ""), DATASTRING);

      printf("C Size\t\tNanosec\t\tMB/sec\t\t%% Chnge\n");
      printf("-------\t\t-------\t\t-------\t\t-------\n");
    }

  for (i = 0; i < timeslots; i++)
    {
      limit = sizes[i] / (long)sizeof(DATATYPE);
      for (j = 0; j < repeat_count; j++)
	{
	  if (type & HANDTUNED)
	    refcnt = hand_benchmark_cache_ronly(x, limit, &tloops, &tmicrosec);
	  else
	    refcnt = benchmark_cache_ronly(x, limit, &tloops, &tmicrosec);

	  /* if (type & NOCALIBRATE)
	    {
	      nullcnt = 0.0;
	      overhead_per_ref = 0.0;
	      cmicrosec = 0.0;
	    }
	  else
	    {
	      nullcnt = calibrate_benchmark(x, tloops, limit, &cmicrosec);
	      overhead_per_ref = (cmicrosec*1000.0) / nullcnt;
	      DBG(fprintf(stderr,"C: %f ns per ref.\n",overhead_per_ref));
	    } */

	  compute_stats(i,j,refcnt,overhead_per_ref,times,bws,percents,sizes,tmicrosec,sizeof(DATATYPE));
	}
    }

  /* if ((my_isatty(1))&&(type&GUESSCACHESIZE))
    compute_cache_sizes(times,bws,percents,sizes); */
}

void do_write_only(long *sizes, DATATYPE *x, double *times, double *bws, double *percents)
{
  long limit, j, i, tloops;
  double refcnt, overhead_per_ref = 0.0, tmicrosec;
  /* double nullcnt = 0.0, cmicrosec = 0.0; */

  if (my_isatty(1))
    {
      printf("\n\t\t%s%s%s Write Cache Test\n\n",
	     ((type & HANDTUNED) ? "Tuned " : ""),
	     ((!(type & NOCALIBRATE)) ? "Calibrated " : ""), DATASTRING);

      printf("C Size\t\tNanosec\t\tMB/sec\t\t%% Chnge\n");
      printf("-------\t\t-------\t\t-------\t\t-------\n");
    }

  for (i = 0; i < timeslots; i++)
    {
      limit = sizes[i] / (long)sizeof(DATATYPE);
      for (j = 0; j < repeat_count; j++)
	{
	  if (type & HANDTUNED)
	    refcnt = hand_benchmark_cache_wonly(x, limit, &tloops, &tmicrosec);
	  else
	    refcnt = benchmark_cache_wonly(x, limit, &tloops, &tmicrosec);

	  /* if (type & NOCALIBRATE)
	    {
	      nullcnt = 0.0;
	      overhead_per_ref = 0.0;
	      cmicrosec = 0.0;
	    }
	  else
	    {
	      nullcnt = calibrate_benchmark(x, tloops, limit, &cmicrosec);
	      overhead_per_ref = (cmicrosec*1000.0) / nullcnt;
	      DBG(fprintf(stderr,"C: %f ns per ref.\n",overhead_per_ref));
	    } */

	  compute_stats(i,j,refcnt,overhead_per_ref,times,bws,percents,sizes,tmicrosec,sizeof(DATATYPE));
	}
    }

  /* if ((my_isatty(1))&&(type&GUESSCACHESIZE))
    compute_cache_sizes(times,bws,percents,sizes); */
}

void do_read_write(long *sizes, DATATYPE *x, double *times, double *bws, double *percents)
{
  long limit, j, i, tloops;
  double refcnt, overhead_per_ref = 0.0, tmicrosec;
  /* double nullcnt = 0.0, cmicrosec = 0.0; */

  if (my_isatty(1))
    {
      printf("\n\t\t%s%s%s RMW Cache Test\n\n",
	     ((type & HANDTUNED) ? "Tuned " : ""),
	     ((!(type & NOCALIBRATE)) ? "Calibrated " : ""), DATASTRING);

      printf("C Size\t\tNanosec\t\tMB/sec\t\t%% Chnge\n");
      printf("-------\t\t-------\t\t-------\t\t-------\n");
    }

  for (i = 0; i < timeslots; i++)
    {
      limit = sizes[i] / (long)sizeof(DATATYPE);

      for (j = 0; j < repeat_count; j++)
	{
	  if (type & HANDTUNED)
	    refcnt = hand_benchmark_cache(x, limit, &tloops, &tmicrosec) * 2.0;
	  else
	    refcnt = benchmark_cache(x, limit, &tloops, &tmicrosec) * 2.0;

	  /* if (type & NOCALIBRATE)
	    {
	      nullcnt = 0.0;
	      overhead_per_ref = 0.0;
	      cmicrosec = 0.0;
	    }
	  else
	    {
	      nullcnt = calibrate_benchmark(x, tloops, limit, &cmicrosec);
	      nullcnt *= 2;
	      overhead_per_ref = (cmicrosec*1000.0) / nullcnt;
	      DBG(fprintf(stderr,"C: %f ns per ref.\n",overhead_per_ref));
	    } */

	  compute_stats(i,j,refcnt,overhead_per_ref,times,bws,percents,sizes,tmicrosec,sizeof(DATATYPE));
	}
    }

  /* if ((my_isatty(1))&&(type&GUESSCACHESIZE))
    compute_cache_sizes(times,bws,percents,sizes); */
}

int main(void)
{
  DATATYPE *x;
  long *sizes;
  double *times, *bws, *percents;

  /* type = usage(argc, argv); */
  type |= READWRITE;
  timeslots = resolution*(logmemsize - CACHE_MIN_BITS) + 1;
  DBG(printf("%ld %ld %ld %ld\n",logmemsize,memsize,duration,timeslots));

  assert(sizes = (long *)malloc(timeslots*sizeof(long)));
  memset(sizes,0x00,(timeslots*sizeof(long)));
  assert(times = (double *)malloc(timeslots*repeat_count*sizeof(double)));
  memset(times,0x00,(timeslots*repeat_count*sizeof(double)));
  assert(bws = (double *)malloc(timeslots*repeat_count*sizeof(double)));
  memset(bws,0x00,(timeslots*repeat_count*sizeof(double)));
  assert(percents = (double *)malloc(timeslots*repeat_count*sizeof(double)));
  memset(percents,0x00,(timeslots*repeat_count*sizeof(double)));
  assert(x = (DATATYPE *)malloc((size_t)memsize));
  memset((void *)x,0x00,memsize);

  initialize_sizes(sizes);

  /* Measure cache */

  if (type & MEMORYSET)
    {
      do_memory_set(sizes,x,times,bws,percents);
    }

  if (type & MEMORYCOPY)
    {
      do_memory_copy(sizes,x,times,bws,percents);
    }

  if (type & READONLY)
    {
      do_read_only(sizes,x,times,bws,percents);
    }

  if (type & WRITEONLY)
    {
      do_write_only(sizes,x,times,bws,percents);
    }

  if (type & READWRITE)
    {
      do_read_write(sizes,x,times,bws,percents);
    }

  FLUSHALL(0);

  exit(0);
}

/* Prototypes */

double  __t1, __t2 ;
void timer_start(void)
{
  __t1 = clock ();
}
void timer_stop(void)
{
  __t2 = clock ();
}
double timer_elapsed(void){
 if (__t2 - __t1 <= 0.0)
    {
      fprintf(stderr,"Warning! The timer is not precise enough. \
Consider increasing\nthe iteration count or changing the timer in timer.\n");
      return(0.0);
    }
  return((__t2 - __t1)/1000.0);
}

/* Macros */

#define FLUSHALL(a)
#define TIMER_START timer_start()
#define TIMER_STOP timer_stop()
#define TIMER_ELAPSED timer_elapsed()

#ifdef INLINE
#undef INLINE
#define INLINE inline
#else
#define INLINE
#endif

#ifdef REGISTER
#undef REGISTER
#define REGISTER register
#else
#define REGISTER
#endif

#ifdef DEBUG
#define DBG(a) a;
#else
#define DBG(a)
#endif


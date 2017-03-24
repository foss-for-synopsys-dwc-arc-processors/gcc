/* { dg-do assemble } *
/* { dg-skip-if "" { ! { clmcpu } } } */
/* { dg-options "-O3 -w -funroll-loops -mno-sdata -mcpu=arc700" } */

typedef a __attribute__((__mode__(__DI__)));
typedef struct c c;
struct b {
  int d;
  c *e
};
enum { f };
typedef struct {
  a g;
  a h;
  int i
} j;
struct c {
  int count;
  int current
};
k;
l(struct b *demux, __builtin_va_list args) {
  c m = *demux->e;
  j *n;
  switch (k)
  case f: {
    a o = __builtin_va_arg(args, a);
    m.current = 0;
    while (m.current < m.count) {
      if (n[m.current].h > o) {
        p(demux->d, 4 + 128LL * n[m.current].i);
        break;
      }
      m.current++;
    }
    return 0;
  }
}

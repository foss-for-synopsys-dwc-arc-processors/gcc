/* { dg-do compile } */
/* { dg-options "-O2 -mtune=rhx" } */

struct a {
  volatile unsigned b : 19;
  unsigned c : 24;
  unsigned d : 22;
};
union e {
  int16_t f;
  int16_t g;
  int16_t b;
};
static volatile union e h[9] = {{6}};
static struct a o, l;
static volatile struct a j = {40251836};
static volatile union e k;
static volatile struct a m = {2};
int main(int p) {
  int i, n = 0;
  if (p)
    n = 1;
  crc32_gentab();
  i = 0;
  for (; i < 9; i++) {
    transparent_crc(h[i].f, "", n);
    transparent_crc(h[i].g, "", n);
    transparent_crc(h[i].b, "", n);
    if (n)
      printf("index = %d\n", i);
  }
  printf("%d\n", o.b);
  printf("%d\n", j.c);
  printf("%d\n", j.d);
  printf("%d\n", k.f);
  k;
  printf("%d\n", l.b);
  printf("%d\n", m.c);
  printf("%d\n", m.d);
  platform_main_end(crc32_context, n);
}

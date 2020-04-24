/* { dg-do compile } */

#include <stddef.h>

typedef volatile struct {
  uint32_t a1;
  uint32_t a2;
  uint32_t a3;
  uint32_t a4;
} __attribute__((packed,uncached)) my_type_t;

uint32_t foo (my_type_t *p)
{
  p->a3 = p->a2;
  return p->a4;
}

/* { dg-final { scan-assembler-times "stb\.di" 4 } } */
/* { dg-final { scan-assembler-times "ldb\.di" 12 } } */

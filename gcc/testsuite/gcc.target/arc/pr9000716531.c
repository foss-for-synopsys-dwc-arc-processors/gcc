/* { dg-do compile } */
/* { dg-options "-O2 -mA7" } */

typedef unsigned long CORE_ADDR;

enum block_enum
{
  GLOBAL_BLOCK = 0,
  STATIC_BLOCK = 1,
  FIRST_LOCAL_BLOCK = 2
};

struct block
{
  CORE_ADDR startaddr;
  CORE_ADDR endaddr;
};

struct blockvector
{

  int nblocks;
  void *map;
  struct block *block[1];
};

extern void bar (const char *) __attribute__ ((__noreturn__));

struct block *
foo (struct blockvector *bl, CORE_ADDR pc)
{
  struct block *b;
  int bot, top, half;

  if (bl->map)
    return (struct block *) addrmap_find (bl->map, pc);

  ((void) ((bl->nblocks >= 2) ? 0 :
	   (bar ("block_reduced.c"),0)));

  bot = STATIC_BLOCK;
  top = (bl)->nblocks;

  while (top - bot > 1)
    {
      half = (top - bot + 1) >> 1;
      b = (bl)->block[bot + half];
      if ((b)->startaddr <= pc)
	bot += half;
      else
	top = bot + half;
    }

  while (bot >= STATIC_BLOCK)
    {
      b = (bl)->block[bot];
      if ((b)->endaddr > pc)
	return b;
      bot--;
    }

  return ((void *)0);
}

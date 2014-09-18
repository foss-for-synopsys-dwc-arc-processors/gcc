/* { dg-do assemble } */
/* { dg-options "-Os -fstack-protector-all -fno-guess-branch-probability" } */
/* The arc_hazard introduces a NOP which is not taken into account by
   branch shortening step, resulting in an out of range branch
   offset (ARC700 architecture). */

typedef unsigned char uint8_t;
extern int *const bb_errno;

char* hex2bin(char *dst, const char *str, int count)
{
 (*bb_errno) = 22;
 while (*str && count) {
  uint8_t val;
  uint8_t c = *str++;
  if (((unsigned char)((c) - '0') <= 9))
   val = c - '0';
  else if ((c|0x20) >= 'a' && (c|0x20) <= 'f')
   val = (c|0x20) - ('a' - 10);
  else
   return ((void *)0);
  val <<= 4;
  c = *str;
  if (((unsigned char)((c) - '0') <= 9))
   val |= c - '0';
  else if ((c|0x20) >= 'a' && (c|0x20) <= 'f')
   val |= (c|0x20) - ('a' - 10);
  else if (c == ':' || c == '\0')
   val >>= 4;
  else
   return ((void *)0);

  *dst++ = val;
  if (c != '\0')
   str++;
  if (*str == ':')
   str++;
  count--;
 }
 (*bb_errno) = (*str ? 34 : 0);
 return dst;
}

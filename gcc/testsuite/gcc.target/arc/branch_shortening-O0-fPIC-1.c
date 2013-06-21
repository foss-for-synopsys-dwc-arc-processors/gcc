/* { dg-do assemble } */
/* { dg-options "-O0 -fPIC" } */

/* Verify STAR 9000622885,
 * fixed in Git commit 310d65a9345485ba5469775ac39e9e58b4edd84e
 * This error happens only with -O0, because compiler was expecting
 * optimization pass to be performed, but it is not done at -O0 */

typedef long unsigned int size_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
__extension__ typedef int __ssize_t;
typedef __ssize_t ssize_t;
typedef unsigned int uint32_t;
typedef unsigned int uint_fast32_t;
typedef int intptr_t;
extern void *memset (void *__s, int __c, size_t __n) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));
typedef struct __STDIO_FILE_STRUCT FILE;
typedef int wchar_t;
struct printf_info {
  int prec;
  int width;
  wchar_t spec;
  uint32_t _flags;
  wchar_t pad;
};
struct __uclibc_locale_struct {
 int decimal_point_len;
 const char *decimal_point;
};
extern struct __uclibc_locale_struct *__global_locale;
typedef long double __fpmax_t;
typedef size_t (__fp_outfunc_t)(FILE *fp, intptr_t type, intptr_t len, intptr_t buf);
static const char fmt[] = "inf\0INF\0nan\0NAN\0.\0,";
static const __fpmax_t exp10_table[] =
{
 1e1L, 1e2L, 1e4L, 1e8L, 1e16L, 1e32L,
 1e64L,  1e128L, 1e256L, };

ssize_t _fpmaxtostr(FILE * fp, __fpmax_t x, struct printf_info *info,
     __fp_outfunc_t fp_outfunc)
{
 int round, o_exp;
 int exp;
 int width, preci;
 int cnt;
 char *s;
 char *e;
 intptr_t pc_fwi[3*8];
 intptr_t *ppc;
 intptr_t *ppc_last;
 char exp_buf[16];
 char buf[( 3 + ((17 +9 -1)/9) * 9 )];
 char sign_str[6];
 char o_mode;
 char mode;
 width = info->width;
 preci = info->prec;
 mode = info->spec;
 *exp_buf = 'e';
 if ((mode|0x20) == 'a') {
  mode += ('g' - 'a');
 }

 if (preci < 0) {
  preci = 6;
 }

 *sign_str = '\0';
 if (((info)->_flags & (1<<1))) {
  *sign_str = '+';
 } else if (((info)->_flags & (1<<0))) {
  *sign_str = ' ';
 }

 *(sign_str+1) = 0;
 pc_fwi[5] = 0;
 if (((x) != (x))) {
  pc_fwi[5] = 8;
  goto INF_NAN;
 }

 if (x == 0) {

  if (((1./(x)) < 0)) {
   *sign_str = '-';
  }

  exp = -1;
  goto GENERATE_DIGITS;
 }

 if (x < 0) {
  *sign_str = '-';
  x = -x;
 }

 if (((x) == ((x)/4) )) {
 INF_NAN:
  info->pad = ' ';
  ppc = pc_fwi + 6;
  pc_fwi[3] = 'p';
  pc_fwi[4] = 3;
  if (mode < 'a') {
   pc_fwi[5] += 4;
  }
  pc_fwi[5] = (intptr_t)(fmt + pc_fwi[5]);
  goto EXIT_SPECIAL;
 }

 {
  int i, j;
  exp = 9 - 1;
  i = (sizeof(exp10_table)/sizeof(exp10_table[0]));
  j = (1U<<((sizeof(exp10_table)/sizeof(exp10_table[0]))-1));

  {
   int exp_neg = 0;
   if (x < 1e8) {
    exp_neg = 1;
   }

   do {
    --i;
    if (exp_neg) {
     if (x * exp10_table[i] < 1e9) {
      x *= exp10_table[i];
      exp -= j;
     }
    } else {
     if (x / exp10_table[i] >= 1e8) {
      x /= exp10_table[i];
      exp += j;
     }
    }
    j >>= 1;
   } while (i);
  }
 }
 if (x >= 1e9) {
  x /= exp10_table[0];
  ++exp;
 }
 ((void) (0));

 GENERATE_DIGITS:
 {
  int i, j;
  s = buf + 2;
  i = 0;
  do {
   uint_fast32_t digit_block = (uint_fast32_t) x;
   ((void) (0));



   x = (x - digit_block) * 1e9;
   s += 9;
   j = 0;
   do {
    s[- ++j] = '0' + (digit_block % 10);
    digit_block /= 10;
   } while (j < 9);
  } while (++i < ((17 +9 -1)/9));
 }



 if (mode < 'a') {
  *exp_buf -= ('a' - 'A');
  mode += ('a' - 'A');
 }

 o_mode = mode;
 if ((mode == 'g') && (preci > 0)){
  --preci;
 }
 round = preci;

 if (mode == 'f') {
  round += exp;
  if (round < -1) {
   memset(buf, '0', 17);
      exp = -1;
      round = -1;
  }
 }

 s = buf;
 *s++ = 0;
 *s = '0';

 {
  int i;
  i = 0;
  e = s + 17 + 1;
  if (round < 17) {
   e = s + round + 2;
   if (*e >= '0' + (10/2)) {
    i = 1;
   }
  }

  do {
   *--e += i;
  } while ((*e == '0') || (*e > '0' - 1 + 10));
 }
 o_exp = exp;
 if (e <= s) {
  ++o_exp;
  e = s;
 } else {
  ++s;
 }
 *++e = 0;

 if ((mode == 'g') && ((o_exp >= -4) && (o_exp <= round))) {
  mode = 'f';
  preci = round - o_exp;
 }

 exp = o_exp;
 if (mode != 'f') {
  o_exp = 0;
 }

 if (o_exp < 0) {
  *--s = '0';
 }

 pc_fwi[3] = (0x80 | '0');
 pc_fwi[4] = 1;
 pc_fwi[5] = (intptr_t)(sign_str + 4);
 sign_str[4] = *s++;
 sign_str[5] = 0;
 ppc = pc_fwi + 6;

 {
  int i = e - s;
  if (o_exp >= 0) {
   ppc[0] = 'p';
   ppc[2] = (intptr_t)(s);
   if (o_exp >= i) {
    ppc[1] = i;
    ppc += 3;
    o_exp -= i;
    i = 0;
    if (o_exp>0) {
     ppc[0] = (0x80 | '0');
     ppc[1] = o_exp;
     ppc[2] = (intptr_t)(fmt + 3);
     ppc += 3;
    }
   } else if (o_exp > 0) {
    ppc[1] = o_exp;
    ppc += 3;
    s += o_exp;
    i -= o_exp;
   }
   o_exp = -1;
  }

  if (((info)->_flags & (1<<4))
   || (i)
   || ((o_mode != 'g')
    && (preci > 0))
   ) {
   ppc[0] = 'p';

   if (((info)->_flags & (1<<7))) {

    ppc[1] = 1;
    ppc[2] = (intptr_t)(fmt + 16);
   } else {

    ppc[1] = (__global_locale)->decimal_point_len;
    ppc[2] = (intptr_t)((__global_locale)->decimal_point);
   }
   ppc += 3;
  }

  if (++o_exp < 0) {
   ppc[0] = (0x80 | '0');
   ppc[1] = -o_exp;
   ppc[2] = (intptr_t)(fmt + 3);
   ppc += 3;
  }
  if (i) {
   ppc[0] = 'p';
   ppc[1] = i;
   ppc[2] = (intptr_t)(s);
   ppc += 3;
  }

  if (((o_mode != 'g') || ((info)->_flags & (1<<4)))



   ) {
   i -= o_exp;
   if (i < preci) {
    i = preci - i;
    ppc[0] = (0x80 | '0');
    ppc[1] = i;
    ppc[2] = (intptr_t)(fmt + 3);
    ppc += 3;
   }
  }
 }


 if (mode != 'f') {
  char *p = exp_buf + sizeof(exp_buf);
  int j;
  char exp_char = *exp_buf;
  char exp_sign = '+';

  if (exp < 0) {
   exp_sign = '-';
   exp = -exp;
  }

  *--p = 0;
  j = 2;
  do {
   *--p = '0' + (exp % 10);
   exp /= 10;
  } while ((++j < (2+2)) || exp);
  *--p = exp_sign;
  *--p = exp_char;

  ppc[0] = 'p';
  ppc[1] = j;
  ppc[2] = (intptr_t)(p);
  ppc += 3;
 }

 EXIT_SPECIAL:
 {
  int i;
  ppc_last = ppc;
  ppc = pc_fwi + 4;
  do {
   width -= *ppc;
   ppc += 3;
  } while (ppc < ppc_last);

  ppc = pc_fwi;
  ppc[0] = (0x80 | ' ');;
  ppc[1] = i = ((*sign_str) != 0);
  ppc[2] = (intptr_t) sign_str;
  if ((width -= i) > 0) {
   if (((info)->_flags & (1<<3))) {
    ppc_last[0] = (0x80 | ' ');;
    ppc_last[1] = width;
    ppc_last[2] = (intptr_t)(fmt + 3);
    ppc_last += 3;
   } else if (info->pad == '0') {
    ppc[4] += width;
   } else {
    ppc[1] += width;
   }
  }

  cnt = 0;
 }

 do {
  {
   if (fp_outfunc(fp, *ppc, ppc[1], ppc[2]) != ppc[1]) {
    return -1;
   }
  }

  cnt += ppc[1];
  ppc += 3;
 } while (ppc < ppc_last);

 return cnt;
}

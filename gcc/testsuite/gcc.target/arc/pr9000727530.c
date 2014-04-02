/* { dg-do compile } */
/* { dg-options "-Os" } */
/* The expand pass generates a subreg of mem, where the address is a
   function of virtual-stack-vars. The vregs step fails to convert the
   virtual-stack-vars to an fp/sp register, hence, the compiler crashes
   when checking the instruction constrains. */

#include "stdio.h"

char *outfile;

_Bool reverse;
static FILE *rejfp;
static char *rejname;

struct outstate
{
  FILE *ofp;
  _Bool after_newline;
  _Bool zero_output;
};

struct __longlong_aligned { long long x; };

struct stat
{
  unsigned long long st_dev;
  unsigned long long st_ino;
  unsigned long long st_rdev;
  unsigned long long _pad1;
  long long st_size;
  int st_blksize;
  int __pad2;
  long long st_blocks;
  struct timespec st_atim;
  struct timespec st_mtim;
  struct timespec st_ctim;
  unsigned int __unused4;
  unsigned int __unused5;
} __attribute__((aligned(__alignof__(struct __longlong_aligned))));

enum diff
  {
    NO_DIFF,
    CONTEXT_DIFF,
    NORMAL_DIFF,
    ED_DIFF,
    NEW_CONTEXT_DIFF,
    UNI_DIFF,
    GIT_BINARY_DIFF
  };

enum diff diff_type;

int
foo (int x)
{
  struct outstate outstate;
  struct stat tmpoutst;
  mode_t file_type;
  int outfd = -1;
  int failed = 0;

  tmpoutst.st_size = -1;
  if (diff_type == ED_DIFF)
    {
      outstate.zero_output = 0;
      if (! outfile )
	{
	  fstat (outfd, &tmpoutst);
	  outstate.zero_output = tmpoutst.st_size == 0;
	}
    }
  else
    {
      failed++;
    }

    if (outstate.zero_output
	&& ((pch_says_nonexistent (! reverse) == 2)
	    || ((((file_type)) & 0170000) == (0120000))))
      {
	struct stat rejst;

	if (fstat (fileno (rejfp), &rejst) != 0)
	  {
	    char *rej = rejname;
	    if (rejname)
	      {
		struct stat oldst;
		int olderrno;

		olderrno = stat_file (rej, &oldst);
	      }
	  }
      }
  return 0;
}

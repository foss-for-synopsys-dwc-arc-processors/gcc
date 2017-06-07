/* Copyright (C) 2017 Free Software Foundation, Inc.
   Contributed by Richard Henderson <rth@redhat.com>.

   This file is part of the GNU Atomic Library (libatomic).

   Libatomic is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Libatomic is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include "libatomic_i.h"
#include <stdint.h>

/* ARC700 and all basic configuration of ARCv2 are having EX
   instruction.  */
#if defined (WANT_SPECIALCASE_RELAXED) || defined (__ARC600__) \
  || defined (__ARC601__)
# define MEMORY_MODEL __ATOMIC_RELAXED
# define ARC_ATOMIC_EXCHANGE(_MEM_,_VAL_,_MEMORY_MODEL_)	\
  ({								\
    unsigned int __tmp = (*(_MEM_));				\
    (*(_MEM_)) = (_VAL_);					\
    __builtin_arc_sync ();					\
    __tmp;							\
  })
#else
# define MEMORY_MODEL __ATOMIC_SEQ_CST
# define ARC_ATOMIC_EXCHANGE(_MEM_,_VAL_,_MEMORY_MODEL_)	\
  __atomic_exchange_n (_MEM_, _VAL_, _MEMORY_MODEL_)
#endif

#if HAVE_ATOMIC_CAS_4
#define ARC_ATOMIC_COMPARE_EXCHANGE(_MEM_, _EXPECT_, _VAL_)		\
  do {									\
    __atomic_compare_exchange_n (_MEM_, &_EXPECT_, _VAL_, false,	\
				 MEMORY_MODEL, MEMORY_MODEL);		\
  } while (0)

#else
#define ARC_ATOMIC_COMPARE_EXCHANGE(_MEM_, _EXPECT_, _VAL_)		\
  do {									\
    uint32_t __readbackval = _VAL_;					\
    do									\
      {									\
	__readbackval = ARC_ATOMIC_EXCHANGE (_MEM_, __readbackval,	\
					     MEMORY_MODEL);		\
      } while (_EXPECT_ != __readbackval);				\
    _EXPECT_ = __readbackval;						\
  } while (0)
#endif

#if defined (__ARCEM__) || defined (__ARCHS__)
# define ARC_DISABLE_IRQ(_TMP_)			\
  do {						\
    _TMP_ =  __builtin_arc_clri ();		\
  } while (0)
# define ARC_ENABLE_IRQ(_TMP_)			\
  do {						\
    __builtin_arc_seti (_TMP_);			\
  } while (0)
#else
/* Int 1 enable.  */
# define STATUS_E1_BIT		1
/* Int 2 enable.  */
# define STATUS_E2_BIT		2
# define STATUS_E1_MASK		(1<<STATUS_E1_BIT)
# define STATUS_E2_MASK		(1<<STATUS_E2_BIT)
# define ARC_ENABLE_IRQ(_TMP_)						\
  do {									\
    __asm__ __volatile__("	lr   %0, [status32]	\n"		\
			 "	or   %0, %0, %1		\n"		\
			 "	flag %0			\n"		\
			 : "=&r"(_TMP_)					\
			 : "n"((STATUS_E1_MASK | STATUS_E2_MASK))	\
			 : "memory");					\
  } while (0)
# define ARC_DISABLE_IRQ(_TMP_)						\
  do {									\
    __asm__ __volatile__("	lr  %0, [status32]	\n"		\
			 "	and %0, %0, %1		\n"		\
			 "	flag %0			\n"		\
			 : "=&r"(_TMP_)					\
			 : "n"(~(STATUS_E1_MASK | STATUS_E2_MASK))	\
			 : "memory");					\
  } while (0)
#endif

/* ARC custom spin lock implementation.  */
typedef uint32_t __arc_spinlock_t;

static inline
uint32_t __arc_spin_lock (__arc_spinlock_t *lock)
{
  uint32_t expect = 0;
  uint32_t key;

  ARC_DISABLE_IRQ (key);
  ARC_ATOMIC_COMPARE_EXCHANGE (lock, expect, 1);
  ARC_ENABLE_IRQ (key);
  return expect;
}

/* ARC custom spin unlock implemenation.  */

static inline
uint32_t __arc_spin_unlock (__arc_spinlock_t *lock)
{
  uint32_t val;
  uint32_t key;

  ARC_DISABLE_IRQ (key);
  val = ARC_ATOMIC_EXCHANGE (lock, 0, MEMORY_MODEL);
  ARC_ENABLE_IRQ (key);
  return val;
}

/* The following functions are similar like in posix case. */

/* The target page size.  Must be no larger than the runtime page size,
   lest locking fail with virtual address aliasing (i.e. a page mmaped
   at two locations).  */
#ifndef PAGE_SIZE
#define PAGE_SIZE	4096
#endif

/* The target cacheline size.  This is an optimization; the padding that
   should be applied to the locks to keep them from interfering.  */
#ifndef CACHLINE_SIZE
#define CACHLINE_SIZE	64
#endif

/* The granularity at which locks are applied.  Almost certainly the
   cachline size is the right thing to use here.  */
#ifndef WATCH_SIZE
#define WATCH_SIZE	CACHLINE_SIZE
#endif

struct lock
{
  __arc_spinlock_t mutex;
  char pad[sizeof(__arc_spinlock_t) < CACHLINE_SIZE
	   ? CACHLINE_SIZE - sizeof(__arc_spinlock_t)
	   : 0];
};

#define NLOCKS		(PAGE_SIZE / WATCH_SIZE)
static struct lock locks[NLOCKS] = {
  [0 ... NLOCKS-1].mutex = 0
};

static inline uintptr_t
addr_hash (void *ptr)
{
  return ((uintptr_t)ptr / WATCH_SIZE) % NLOCKS;
}

void
libat_lock_1 (void *ptr)
{
  __arc_spin_lock (&locks[addr_hash (ptr)].mutex);
}

void
libat_unlock_1 (void *ptr)
{
  __arc_spin_unlock (&locks[addr_hash (ptr)].mutex);
}

void
libat_lock_n (void *ptr, size_t n)
{
  uintptr_t h = addr_hash (ptr);
  size_t i = 0;

  /* Don't lock more than all the locks we have.  */
  if (n > PAGE_SIZE)
    n = PAGE_SIZE;

  do
    {
      __arc_spin_lock (&locks[h].mutex);
      if (++h == NLOCKS)
	h = 0;
      i += WATCH_SIZE;
    }
  while (i < n);
}

void
libat_unlock_n (void *ptr, size_t n)
{
  uintptr_t h = addr_hash (ptr);
  size_t i = 0;

  if (n > PAGE_SIZE)
    n = PAGE_SIZE;

  do
    {
      __arc_spin_unlock (&locks[h].mutex);
      if (++h == NLOCKS)
	h = 0;
      i += WATCH_SIZE;
    }
  while (i < n);
}

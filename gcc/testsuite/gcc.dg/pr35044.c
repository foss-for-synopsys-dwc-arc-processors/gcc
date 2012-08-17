/* { dg-do run } */
/* We want to test this with -mA7 in particular, but don't add it explicitly
   to dg-options since this clashes with automatic multi-subtarget testing.  */
/* { dg-options "-O2 -mlong-calls -fomit-frame-pointer" { target arc*-*-* } } */

typedef signed long long s64;
typedef unsigned long long u64;

struct restart_block
{
  long (*fn) (struct restart_block *);
  unsigned long arg0, arg1, arg2, arg3;
};

typedef unsigned long mm_segment_t;

typedef struct
{
  int counter;
} atomic_t;

struct thread_info
{
  void *task;
  struct exec_domain *exec_domain;
  unsigned long flags;
  unsigned long tp_value;
  unsigned cpu;
  int preempt_count;

  mm_segment_t addr_limit;

  struct restart_block restart_block;
};

static inline __attribute__ ((always_inline))
     struct thread_info *current_thread_info (void)
  __attribute__ ((__const__));

     static inline __attribute__ ((always_inline))
     struct thread_info *current_thread_info (void)
{
  register unsigned long sp asm ("sp");
  return (struct thread_info *) (sp & ~((1UL << 13) - 1));
}


typedef struct
{
} raw_rwlock_t;

typedef struct
{
  raw_rwlock_t raw_lock;
} rwlock_t;

struct list_head
{
  struct list_head *next, *prev;
};

struct rcu_head
{
  struct rcu_head *next;
  void (*func) (struct rcu_head * head);
};

enum pid_type
{
  PIDTYPE_PID,
  PIDTYPE_PGID,
  PIDTYPE_SID,
  PIDTYPE_MAX
};

struct fown_struct
{
  rwlock_t lock;
  struct pid *pid;
  enum pid_type pid_type;
  unsigned uid, euid;
  int signum;
};

struct file_ra_state
{
  unsigned long start;
  unsigned long size;
  unsigned long flags;
  unsigned long cache_hit;
  unsigned long prev_page;
  unsigned long ahead_start;
  unsigned long ahead_size;
  unsigned long ra_pages;
  unsigned long mmap_hit;
  unsigned long mmap_miss;
};

struct file
{
  union
  {
    struct list_head fu_list;
    struct rcu_head fu_rcuhead;
  } f_u;
  struct dentry *f_dentry;
  struct vfsmount *f_vfsmnt;
  const struct file_operations *f_op;
  atomic_t f_count;
  unsigned int f_flags;
  unsigned short f_mode;
  s64 f_pos;
  struct fown_struct f_owner;
  unsigned int f_uid, f_gid;
  struct file_ra_state f_ra;
  unsigned long f_version;
  void *private_data;
  struct address_space *f_mapping;
};
typedef int (*filldir_t) (void *, const char *, int, s64, u64, unsigned);
extern int __attribute__((noinline)) vfs_readdir (struct file *, filldir_t, void *);
extern void __attribute__((noinline)) fput (struct file *);
extern struct file * __attribute__((noinline)) fget (unsigned int fd);

struct linux_dirent64
{
  u64 d_ino;
  s64 d_off;
  unsigned short d_reclen;
  unsigned char d_type;
  char d_name[0];
};

long __attribute__((noinline)) sys_getdents64 (unsigned int fd,
		     struct linux_dirent64 *dirent, unsigned int count);

struct getdents_callback64
{
  struct linux_dirent64 *current_dir;
  struct linux_dirent64 *previous;
  int count;
  int error;
};

extern void abort (void);

struct file *
fget (unsigned int fd)
{
  static struct file dummy;

  asm volatile ("");
  return &dummy;
}

extern int __attribute__((noinline)) filldir64 (void *__buf, const char *name,
		      int namlen, s64 offset, u64 ino, unsigned int d_type);

int
filldir64 (void *vp, const char *cp, int i, s64 o, u64 ull, unsigned u)
{
  return 0;
}

int
vfs_readdir (struct file *f, filldir_t fun, void *buf)
{
  struct getdents_callback64 *cb = buf;

  asm volatile ("");
  return (long) cb->current_dir < 0 ? -1 : 0;
}

void
fput (struct file *f)
{
  asm volatile ("");
}

int
main (void)
{
  if (sys_getdents64 (0, 0, 0))
    abort ();
  return 0;
}

long
sys_getdents64 (unsigned int fd,
		struct linux_dirent64 *dirent, unsigned int count)
{

  register struct file *file;
  register struct linux_dirent64 *lastdirent;
  struct getdents_callback64 buf;
  register int error;
  error = -14;
  if (!(current_thread_info ()->addr_limit == (mm_segment_t) 0xFFFFFFFF
	|| (count <= 0x60000000UL
	    && (unsigned long) dirent <= 0x60000000UL - count)))
    goto out;
  error = -9;
  file = fget (fd);
  if (!file)
    goto out;
  buf.current_dir = dirent;
  buf.previous = ((void *) 0);
  buf.count = count;
  buf.error = 0;
  error = vfs_readdir (file, filldir64, &buf);
  if (error < 0)
    goto out_putf;
  error = buf.error;
  lastdirent = buf.previous;
  if (lastdirent)
    {
      s64 d_off = file->f_pos;
      error = -14;
      {
	long __pu_err = 0;
	s64 *__pu_addr = &lastdirent->d_off;
	__asm__ __volatile__ ("": "=r" (__pu_err):"r" (d_off),
			      "r" (__pu_addr), "i" (-14),
			      "0" (__pu_err));
	if ((__pu_err))
	  goto out_putf;
      }
      error = count - buf.count;
    }

out_putf:
  fput (file);
out:
  return (error);
}

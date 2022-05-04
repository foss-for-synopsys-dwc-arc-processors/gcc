/* -----------------------------------------------------------------------
   ffi.c - Copyright (c) 2013-2021  Synopsys, Inc. (www.synopsys.com)

   ARC64 Foreign Function Interface

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   ``Software''), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL RENESAS TECHNOLOGY BE LIABLE FOR ANY CLAIM, DAMAGES OR
   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   OTHER DEALINGS IN THE SOFTWARE.
   ----------------------------------------------------------------------- */

#include <ffi.h>
#include <ffi_common.h>

#include <stdlib.h>
#include <stdint.h>

#include <sys/cachectl.h>

/* for little endian ARC, the code is in fact stored as mixed endian for
   performance reasons */
#if __BIG_ENDIAN__
#define CODE_ENDIAN(x) (x)
#else
#define CODE_ENDIAN(x) ( (((uint32_t) (x)) << 16) | (((uint32_t) (x)) >> 16))
#endif

/* TODO: I believe this will be changed soon...  */
static int
ffi_arg_use_stack_ptr (const ffi_type *arg)
{
  if (arg->type == FFI_TYPE_STRUCT)
    {
      ffi_type ** const p_el = arg->elements;

      if (arg->size > 16)
	return 1;

      if (p_el && p_el[0] && p_el[1] == NULL
	  && ffi_arg_use_stack_ptr (p_el[0]))
	return 1;
    }

  return 0;
}

/* ffi_prep_args is called by the assembly routine once stack
   space has been allocated for the function's arguments.  */

void
ffi_prep_args (char *stack, extended_cif * ecif, unsigned stack_allocated)
{
  unsigned int i;
  void **p_argv;
  char *argp;
  char *stack_end;
  ffi_type **p_arg;

  stack_end = stack + stack_allocated;
  argp = stack;

  if (ecif->cif->rtype->type == FFI_TYPE_STRUCT)
    {
      *(void **) argp = ecif->rvalue;
      argp += 8;
    }

  p_argv = ecif->avalue;

  for (i = ecif->cif->nargs, p_arg = ecif->cif->arg_types;
       (i != 0); i--, p_arg++)
    {
      size_t z;
      int alignment;

      /* align alignment to 8 */
      alignment = (((*p_arg)->alignment - 1) | 7) + 1;

      /* Align if necessary.  */
      if ((alignment - 1) & (unsigned long) argp)
	argp = (char *) ALIGN (argp, alignment);

      z = (*p_arg)->size;
      if (ffi_arg_use_stack_ptr (*p_arg))
	{
	  void *ptr;

	  ptr = stack_end - z;
	  ptr = (void *) ALIGN_DOWN (ptr, 8);
	  memcpy (ptr, *p_argv, z);
	  stack_end = ptr;

	  *(void **) argp = ptr;
	  z = 8;
	}
      else if (z < sizeof (int))
	{
	  z = sizeof (int);

	  switch ((*p_arg)->type)
	    {
	    case FFI_TYPE_SINT8:
	      *(signed int *) argp = (signed int) *(SINT8 *) (*p_argv);
	      break;

	    case FFI_TYPE_UINT8:
	      *(unsigned int *) argp = (unsigned int) *(UINT8 *) (*p_argv);
	      break;

	    case FFI_TYPE_SINT16:
	      *(signed int *) argp = (signed int) *(SINT16 *) (*p_argv);
	      break;

	    case FFI_TYPE_UINT16:
	      *(unsigned int *) argp = (unsigned int) *(UINT16 *) (*p_argv);
	      break;

	    case FFI_TYPE_STRUCT:
	      memcpy (argp, *p_argv, (*p_arg)->size);
	      break;

	    default:
	      FFI_ASSERT (0);
	    }
	}
      else if (z == sizeof (int))
	{
	  *(unsigned int *) argp = (unsigned int) *(UINT32 *) (*p_argv);
	}
      else
	{
	  memcpy (argp, *p_argv, z);
	}
      p_argv++;
      argp += z;
    }

  return;
}

/* Perform machine dependent cif processing.  */
ffi_status
ffi_prep_cif_machdep (ffi_cif * cif)
{
  /* Set the return type flag.  */
  switch (cif->rtype->type)
    {
    case FFI_TYPE_VOID:
      cif->flags = (unsigned) cif->rtype->type;
      break;

    case FFI_TYPE_STRUCT:
      cif->flags = (unsigned) cif->rtype->type;
      break;

    case FFI_TYPE_SINT64:
    case FFI_TYPE_UINT64:
    case FFI_TYPE_DOUBLE:
    case FFI_TYPE_LONGDOUBLE:
      cif->flags = FFI_TYPE_DOUBLE;
      break;

    case FFI_TYPE_FLOAT:
    default:
      cif->flags = FFI_TYPE_INT;
      break;
    }

  return FFI_OK;
}

extern void ffi_call_ARC64 (void (*)(char *, extended_cif *, unsigned),
			    extended_cif *, unsigned, unsigned,
			    unsigned *, void (*fn) (void), unsigned);

void
ffi_call (ffi_cif * cif, void (*fn) (void), void *rvalue, void **avalue)
{
  extended_cif ecif;
  ffi_type **p_arg;
  unsigned i;
  unsigned stack_args_bytes = 0;

  ecif.cif = cif;
  ecif.avalue = avalue;

  /* If the return value is a struct and we don't have
     a return value address then we need to make one.  */
  if ((rvalue == NULL) && (cif->rtype->type == FFI_TYPE_STRUCT))
    {
      ecif.rvalue = alloca (cif->rtype->size);
    }
  else
    ecif.rvalue = rvalue;

  for (i = cif->nargs, p_arg = cif->arg_types;
       (i != 0); i--, p_arg++)
    {
      if (ffi_arg_use_stack_ptr (*p_arg))
	{
	      stack_args_bytes += (*p_arg)->size;
	      stack_args_bytes = ALIGN (stack_args_bytes, 8);
	}
    }

  switch (cif->abi)
    {
    case FFI_ARC64:
      ffi_call_ARC64 (ffi_prep_args, &ecif, cif->bytes,
		      cif->flags, ecif.rvalue, fn, stack_args_bytes);
      break;

    default:
      FFI_ASSERT (0);
      break;
    }
}

int
ffi_closure_inner_ARC64 (ffi_closure * closure, void *rvalue,
			     ffi_arg * args)
{
  void **arg_area, **p_argv;
  ffi_cif *cif = closure->cif;
  char *argp = (char *) args;
  ffi_type **p_argt;
  int i;

  arg_area = (void **) alloca (cif->nargs * sizeof (void *));

  /* handle hidden argument */
  if (cif->flags == FFI_TYPE_STRUCT)
    {
      rvalue = *(void **) argp;
      argp += 8;
    }

  p_argv = arg_area;

  for (i = 0, p_argt = cif->arg_types; i < cif->nargs;
       i++, p_argt++, p_argv++)
    {
      size_t z;
      int alignment;

      /* Align alignment to 8.  */
      alignment = (((*p_argt)->alignment - 1) | 7) + 1;

      /* Align if necessary.  */
      if ((alignment - 1) & (unsigned long) argp)
	argp = (char *) ALIGN (argp, alignment);

      if (ffi_arg_use_stack_ptr (*p_argt))
	{
	  z = 8;
	  *p_argv = *(void **) argp;
	}
      else
	{
	  z = (*p_argt)->size;
	  *p_argv = (void *) argp;
	}
      argp += z;
    }

  (closure->fun) (cif, rvalue, arg_area, closure->user_data);

  return cif->flags;
}

extern void ffi_closure_ARC64 (void);

ffi_status
ffi_prep_closure_loc (ffi_closure * closure, ffi_cif * cif,
		      void (*fun) (ffi_cif *, void *, void **, void *),
		      void *user_data, void *codeloc)
{
  uint32_t *tramp = (uint32_t *) & (closure->tramp[0]);

  switch (cif->abi)
    {
    case FFI_ARC64:
      FFI_ASSERT (tramp == codeloc);
      tramp[0] = CODE_ENDIAN (0x580a1fc0);	/* movl r8, pcl  */
      tramp[1] = CODE_ENDIAN (0x5c0b1f80);	/* movhl r12, limm */
      tramp[2] = CODE_ENDIAN ((uint64_t)(ffi_closure_ARC64) >> 32);
      tramp[3] = CODE_ENDIAN (0x5c051f0c);	/* orl r12, r12, limm */
      tramp[4] = CODE_ENDIAN ((uint64_t)(ffi_closure_ARC64) & 0xffffffff);
      tramp[5] = CODE_ENDIAN (0x20200300);	/* j [r12] */
      break;

    default:
      return FFI_BAD_ABI;
    }

  closure->cif = cif;
  closure->fun = fun;
  closure->user_data = user_data;
  cacheflush (codeloc, FFI_TRAMPOLINE_SIZE, BCACHE);

  return FFI_OK;
}

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

#define NARGREG 8
#define MAXCOPYARG (2 * sizeof(void *))

#define STKALIGN 8

/* for little endian ARC, the code is in fact stored as mixed endian for
   performance reasons */
#if __BIG_ENDIAN__
#define CODE_ENDIAN(x) (x)
#else
#define CODE_ENDIAN(x) ( (((uint32_t) (x)) << 16) | (((uint32_t) (x)) >> 16))
#endif

/* integer (not pointer) less than ABI XLEN */
/* FFI_TYPE_INT does not appear to be used */
#if __SIZEOF_POINTER__ == 8
#define IS_INT(type) ((type) >= FFI_TYPE_UINT8 && (type) <= FFI_TYPE_SINT64)
#else
#define IS_INT(type) ((type) >= FFI_TYPE_UINT8 && (type) <= FFI_TYPE_SINT32)
#endif
#define IS_NUMBER(type) (IS_INT(type) || (type) == FFI_TYPE_FLOAT || (type) == FFI_TYPE_DOUBLE)
#if defined(__ARC_FPU_DP__)
  #define ABI_FLEN 64
  #define ABI_FLOAT double
#elif defined(__ARC_FPU_SP__)
  #define ABI_FLEN 32
  #define ABI_FLOAT float
#endif

typedef struct call_context
{
#if ABI_FLEN
    size_t f[8];
#endif
    size_t r[8];
    /* used by the assembly code to in-place construct its own stack frame */
    char frame[16];
} call_context;

typedef struct call_builder
{
    call_context *aregs;
    int used_integer;
    int used_float;
    size_t *used_stack;
} call_builder;

static int
ffi_arg_use_stack_ptr (const ffi_type *arg)
{
  if (arg->type == FFI_TYPE_STRUCT)
    {
      ffi_type ** const p_el = arg->elements;

      if (arg->size > 16)
	return 1;

      /* Recursively verify for the same */
      /* TODO: Condition is currently not full understanded. */
      if (p_el && p_el[0] && p_el[1] == NULL
	  && ffi_arg_use_stack_ptr (p_el[0]))
	return 1;
    }

  return 0;
}
/* ffi_prep_args is called by the assembly routine once stack
   space has been allocated for the function's arguments.  */


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
    }

  cif->arc_nfixedargs = cif->nargs;

  return FFI_OK;
}


/* Perform machine dependent cif processing when we have a variadic function */

ffi_status ffi_prep_cif_machdep_var(ffi_cif *cif, unsigned int nfixedargs, unsigned int ntotalargs) {
    cif->arc_nfixedargs = nfixedargs;
    return FFI_OK;
}

/* allocates a single register, float register, or XLEN-sized stack slot to a datum */
static void marshal_atom(call_builder *cb, int type, void *data) {
    size_t value = 0;
    char is_float = 0;
    switch (type) {
	case FFI_TYPE_UINT8: value = *(uint8_t *)data; break;
	case FFI_TYPE_SINT8: value = *(int8_t *)data; break;
	case FFI_TYPE_UINT16: value = *(uint16_t *)data; break;
	case FFI_TYPE_SINT16: value = *(int16_t *)data; break;
	/* 32-bit quantities are always sign-extended in the ABI */
	case FFI_TYPE_UINT32: value = *(int32_t *)data; break;
	case FFI_TYPE_SINT32: value = *(int32_t *)data; break;
#if __SIZEOF_POINTER__ == 8
	case FFI_TYPE_UINT64: value = *(uint64_t *)data; break;
	case FFI_TYPE_SINT64: value = *(int64_t *)data; break;
#endif
	case FFI_TYPE_POINTER: value = *(size_t *)data; break;
	case FFI_TYPE_FLOAT:
	case FFI_TYPE_DOUBLE:
	    value = *(size_t *)data;
	    is_float = 1;
	    break;
	default: FFI_ASSERT(0); break;
    }

#if defined(ABI_FLEN)
    if(is_float == 1) {
	if (cb->used_float == NARGREG) {
	    *cb->used_stack++ = value;
	} else {
	    cb->aregs->f[cb->used_float++] = value;
	}
    }
    else
#endif
    {
	if (cb->used_integer == NARGREG) {
	    *cb->used_stack++ = value;
	} else {
	    cb->aregs->r[cb->used_integer++] = value;
	}
    }
}

static void unmarshal_atom(call_builder *cb, int type, void *data) {
    size_t value;

#if defined(ABI_FLEN)
    if(type == FFI_TYPE_DOUBLE || type == FFI_TYPE_FLOAT) {
	if (cb->used_float == NARGREG) {
	    value = *cb->used_stack++;
	} else {
	    value = cb->aregs->f[cb->used_float++];
	}
    } else
#endif
    {
	if (cb->used_integer == NARGREG) {
	    value = *cb->used_stack++;
	} else {
	    value = cb->aregs->r[cb->used_integer++];
	}
    }

    switch (type) {
	case FFI_TYPE_UINT8: *(uint8_t *)data = value; break;
	case FFI_TYPE_SINT8: *(uint8_t *)data = value; break;
	case FFI_TYPE_UINT16: *(uint16_t *)data = value; break;
	case FFI_TYPE_SINT16: *(uint16_t *)data = value; break;
	case FFI_TYPE_UINT32: *(uint32_t *)data = value; break;
	case FFI_TYPE_SINT32: *(uint32_t *)data = value; break;
#if __SIZEOF_POINTER__ == 8
	case FFI_TYPE_UINT64: *(uint64_t *)data = value; break;
	case FFI_TYPE_SINT64: *(uint64_t *)data = value; break;
#endif
	case FFI_TYPE_POINTER: *(size_t *)data = value; break;
	case FFI_TYPE_FLOAT:  *(size_t *)data = value; break;
	case FFI_TYPE_DOUBLE: *(size_t *)data = value; break;
	default: FFI_ASSERT(0); break;
    }
}

/* adds an argument to a call, or a not by reference return value */
static void marshal(call_builder *cb, ffi_type *type, int var, void *data) {
    size_t realign[2];


    if (type->size > 2 * __SIZEOF_POINTER__) {
	/* pass by reference */
	marshal_atom(cb, FFI_TYPE_POINTER, &data);
    } else if (IS_NUMBER(type->type) || type->type == FFI_TYPE_POINTER) {
	marshal_atom(cb, type->type, data);
    } else {
	/* overlong integers, soft-float floats, and structs without special
	   float handling are treated identically from this point on */

	/* variadics are aligned even in registers */
	if (type->alignment > __SIZEOF_POINTER__) {
	    if (var)
		cb->used_integer = FFI_ALIGN(cb->used_integer, 2);
	    cb->used_stack = (size_t *)FFI_ALIGN(cb->used_stack, 2*__SIZEOF_POINTER__);
	}

	memcpy(realign, data, type->size);
	if (type->size > 0)
	    marshal_atom(cb, FFI_TYPE_POINTER, realign);
	if (type->size > __SIZEOF_POINTER__)
	    marshal_atom(cb, FFI_TYPE_POINTER, realign + 1);
    }
}

/* for arguments passed by reference returns the pointer, otherwise the arg is copied (up to MAXCOPYARG bytes) */
static void *unmarshal(call_builder *cb, ffi_type *type, int var, void *data) {
    size_t realign[2];
    void *pointer;

    if (type->size > 2 * __SIZEOF_POINTER__) {
	/* pass by reference */
	unmarshal_atom(cb, FFI_TYPE_POINTER, (char*)&pointer);
	return pointer;
    } else if (IS_NUMBER(type->type) || type->type == FFI_TYPE_POINTER) {
	unmarshal_atom(cb, type->type, data);
	return data;
    } else {
	/* overlong integers, soft-float floats, and structs without special
	   float handling are treated identically from this point on */

	/* variadics are aligned even in registers */
	if (type->alignment > __SIZEOF_POINTER__) {
	    if (var)
		cb->used_integer = FFI_ALIGN(cb->used_integer, 2);
	    cb->used_stack = (size_t *)FFI_ALIGN(cb->used_stack, 2*__SIZEOF_POINTER__);
	}

	if (type->size > 0)
	    unmarshal_atom(cb, FFI_TYPE_POINTER, realign);
	if (type->size > __SIZEOF_POINTER__)
	    unmarshal_atom(cb, FFI_TYPE_POINTER, realign + 1);
	memcpy(data, realign, type->size);
	return data;
    }
}

static int passed_by_ref(ffi_type *type, int var) {
    if(type->type == FFI_TYPE_STRUCT)
	return 1;
    else
	return type->size > 2 * __SIZEOF_POINTER__;
}

/* Low level routine for calling functions */
extern void ffi_call_asm (void *stack, struct call_context *regs,
			  void (*fn) (void), void *closure) FFI_HIDDEN;

static void
ffi_call_int (ffi_cif *cif, void (*fn) (void), void *rvalue, void **avalue,
	      void *closure)
{
  //ffi_type **p_arg;
  //unsigned stack_args_bytes = 0;

    int return_by_ref = passed_by_ref(cif->rtype, 0);

    /* this is a conservative estimate, assuming a complex return value and
       that all remaining arguments are long long / __int128 */
    /* 4 is the number of values of type long long that fit in r0 to r7. */
    size_t arg_bytes = cif->nargs <= (NARGREG / 2) ? 0 :
	FFI_ALIGN(2 * sizeof(size_t) * (cif->nargs - (NARGREG / 2)), STKALIGN);
    size_t rval_bytes = 0;
    if (rvalue == NULL && return_by_ref)
	rval_bytes = FFI_ALIGN(cif->rtype->size, STKALIGN);
    size_t alloc_size = arg_bytes + rval_bytes + sizeof(call_context);

    /* the assembly code will deallocate all stack data at lower addresses
       than the argument region, so we need to allocate the frame and the
       return value after the arguments in a single allocation */
    size_t alloc_base;
    /* Argument region must be 16-byte aligned */
    if (_Alignof(max_align_t) >= STKALIGN) {
	/* since sizeof long double is normally 16, the compiler will
	    guarantee alloca alignment to at least that much */
	alloc_base = (size_t)alloca(alloc_size);
    } else {
	alloc_base = FFI_ALIGN(alloca(alloc_size + STKALIGN - 1), STKALIGN);
    }

    if (rval_bytes)
	rvalue = (void*)(alloc_base + arg_bytes);

    call_builder cb;
    cb.used_float = cb.used_integer = 0;
    cb.aregs = (call_context*)(alloc_base + arg_bytes + rval_bytes);
    cb.used_stack = (void*)alloc_base;

    if (return_by_ref)
	marshal(&cb, &ffi_type_pointer, 0, &rvalue);

    int i;
    for (i = 0; i < cif->nargs; i++)
	marshal(&cb, cif->arg_types[i], i >= cif->arc_nfixedargs, avalue[i]);

    ffi_call_asm ((void *) alloc_base, cb.aregs, fn, closure);

    cb.used_float = cb.used_integer = 0;
    if (!return_by_ref && rvalue)
	unmarshal(&cb, cif->rtype, 0, rvalue);
}

void
ffi_call (ffi_cif *cif, void (*fn) (void), void *rvalue, void **avalue)
{
    ffi_call_int(cif, fn, rvalue, avalue, NULL);
}

void
ffi_call_go (ffi_cif *cif, void (*fn) (void), void *rvalue,
	     void **avalue, void *closure)
{
   ffi_call_int(cif, fn, rvalue, avalue, closure);
}

extern void ffi_go_closure_asm (void) FFI_HIDDEN;

/* Called by the assembly code with aregs pointing to saved argument registers
   and stack pointing to the stacked arguments.  Return values passed in
   registers will be reloaded from aregs. */
void FFI_HIDDEN
ffi_closure_inner (ffi_cif *cif,
		   void (*fun) (ffi_cif *, void *, void **, void *),
		   void *user_data,
		   size_t *stack, call_context *aregs)
{
    void **avalue = alloca(cif->nargs * sizeof(void*));
    char *astorage = alloca(cif->nargs * MAXCOPYARG);
    void *rvalue;
    call_builder cb;
    int return_by_ref;
    int i;

    cb.aregs = aregs;
    cb.used_integer = cb.used_float = 0;
    cb.used_stack = stack;

    return_by_ref = passed_by_ref(cif->rtype, 0);
    if (return_by_ref)
	unmarshal(&cb, &ffi_type_pointer, 0, &rvalue);
    else
	rvalue = alloca(cif->rtype->size);

    for (i = 0; i < cif->nargs; i++)
	avalue[i] = unmarshal(&cb, cif->arg_types[i],
	    i >= cif->arc_nfixedargs, astorage + i*MAXCOPYARG);

    fun (cif, rvalue, avalue, user_data);

    if (!return_by_ref && cif->rtype->type != FFI_TYPE_VOID) {
	cb.used_integer = cb.used_float = 0;
	marshal(&cb, cif->rtype, 0, rvalue);
    }
}

extern void ffi_closure_asm (void);

ffi_status
ffi_prep_closure_loc (ffi_closure * closure, ffi_cif * cif,
		      void (*fun) (ffi_cif *, void *, void **, void *),
		      void *user_data, void *codeloc)
{
  uint32_t *tramp = (uint32_t *) & (closure->tramp[0]);
  size_t address_ffi_closure = (size_t) ffi_closure_asm;

  switch (cif->abi)
    {
    case FFI_ARC64:
      FFI_ASSERT (tramp == codeloc);
      tramp[0] = CODE_ENDIAN (0x580a1fc0);	/* movl r8, pcl  */
      tramp[1] = CODE_ENDIAN (0x5c0b1f80);	/* movhl r12, limm */
      tramp[2] = CODE_ENDIAN ((uint32_t)(address_ffi_closure >> 32));
      tramp[3] = CODE_ENDIAN (0x5c051f8c);	/* orl r12, r12, limm */
      tramp[4] = CODE_ENDIAN ((uint32_t)(address_ffi_closure & 0xffffffff));
      tramp[5] = CODE_ENDIAN (0x20200300);	/* j [r12] */
      break;

    default:
      return FFI_BAD_ABI;
    }

  closure->cif = cif;
  closure->fun = fun;
  closure->user_data = user_data;
  __builtin___clear_cache(codeloc, codeloc + FFI_TRAMPOLINE_SIZE);

  return FFI_OK;
}

ffi_status
ffi_prep_go_closure (ffi_go_closure *closure, ffi_cif *cif,
		     void (*fun) (ffi_cif *, void *, void **, void *))
{
  if (cif->abi <= FFI_FIRST_ABI || cif->abi >= FFI_LAST_ABI)
    return FFI_BAD_ABI;

  closure->tramp = (void *) ffi_go_closure_asm;
  closure->cif = cif;
  closure->fun = fun;

  return FFI_OK;
}

/* There are two plausible definitions for UNITS_PER_WORD:
   * 16, because that is the register size.
   *  2, because that is the smallest register size that can be written to
         without affecting other parts of the same register.  This is also
     the size we have the most arithmetic operations for, and which is used
     for conditions.  */
#define UNITS_PER_WORD 2

/* Even if we set UNITS_PER_WORD as 16, we can still have smaller regsiters
   as far as gcc is concerned, in order to allow more efficient register
   allocation.  A plausible setting is 8, so that we can use an alternate
   class, but don't loose ourselves in describing irrelevant details.
   OTOH, if UNITS_PER_WORD is 2, MXP_UNIT_PER_VEC_REG should also be 2.
   Note also, that for efficient and sane support of SImode values in
   scalar registers, we want a setting of 2.  Unfortunately, this drives up
   the number of registers to approximately 512.  */
#define MXP_UNITS_PER_MXP_VEC_REG 2

/* Even if we model registers in 16-bit chunks, we need not allow to use
   them all for any possible purpose.  There are practical advanatages to
   put artifical restrictions on the usage:
   - we can keep the number of register classes to be merely high, rather
     than grow excessively high.  If we allow all 256 possible lane
     combinations for scalars, non-scalar vector registers, accumulators,
     and flags, and all combinations of these, we get 2**32 register classes,
     requiring 2 terabits for REG_CLASS_CONTENTS.
   - If we have 8 lane classes to choose from to put an HImode value in,
     with no real preference for either of them, the compiler is prone to
     use them all, and end up with costly cross-lane moves.

   Some register sets serve no practival purpose as a register class.
   - There should be no need for any register class but ALL_REGS to
     incorporate both accumulator and flag registers, since it seems
     rather far-fetched that a value can be hold in the flags register,
     but alternatively might be stored in the accumulator.
   - For HImode values in scalar registers, lanes 0 and 4 are special,
     all other lanes make no difference.  Hence useful masks are 0x01, 0x10,
     0x11 and 0xff.  Likewise, for SImode values in scalar registers,
     useful masks are 0x03, 0x30 and 0xff; scalar register classes with
     masks like 0x31 serve no purpose.
   - When two register classes with specific lane requirements are
     combined, it is to be expected that the lanes agree.  */

/* The non-scalar vector registers have costly cross-lane moves, so we will
   only allow two lane sets for arithmetic per mode:
   GENERAL_REGS in lanes 0..3 and an alternate register set in lanes 4..7 .
   However, we need to allow moves in every lane to make subregs work.
   To then avoid the arithmetic going astray into an unwanted lane, we need
   to have exact per-mode lane masks for arithmetic.
   This gives us a total of eight lane sets for non-scalar vector registers.  */
#define MXP_VEC_LANE_SETS { 0x00, 0x01, 0x10, 0x03, 0x30, 0x0f, 0xf0, 0xff }
/* We use s smaller set of sets for flags and accumulators.  */
#define MXP_FLAG_ACC_LANE_SETS { 0x00, 0x0f, 0xf0, 0xff }
/* The scalar registers are good for holding 16 and 32 bit values and have
   special cross-lane operations, so we allow all lanes for 16 and 32 bit
   values.  Only the lanes that line up with the non-scalar vector registers
   require consideration of their actual lane values.
   We don't allow wider values in scalar registers since that would
   increase regsiter pressure on this valuable register set, and also
   require more register classes.  */
#define MXP_SCALAR_LANE_SETS { 0x00, 0x01, 0x10, 0x03, 0x30, 0xff }

#define MXP_VALID_REG_CLASS(SCALAR,VECTOR,FLAGS,ACC) \
   /* ALL_REGS is valid.  */ \
  ((SCALAR) == 0xff && (VECTOR) == 0xff && (FLAGS) == 0xff && (ACC) == 0xff \
   ? 1 \
   /*  Otherwise, reject combination of FLAGS and ACC.  */ \
   : (FLAGS) != 0 && (ACC) != 0 ? 0 \
   /* Reject combination of mismatched lanes.  */ \
   : (SCALAR) & ~(VECTOR) & 0xf0 && (VECTOR) & ~(SCALAR) & 0x0f ? 0 \
   : (SCALAR) & ~(VECTOR) & 0x0f && (VECTOR) & ~(SCALAR) & 0xf0 ? 0 \
   : (SCALAR) & ~(FLAGS) & 0xf0 && (FLAGS) & ~(SCALAR) & 0x0f ? 0 \
   : (SCALAR) & ~(FLAGS) & 0x0f && (FLAGS) & ~(SCALAR) & 0xf0 ? 0 \
   : (SCALAR) & ~(ACC) & 0xf0 && (ACC) & ~(SCALAR) & 0x0f ? 0 \
   : (SCALAR) & ~(ACC) & 0x0f && (ACC) & ~(SCALAR) & 0xf0 ? 0 \
   : (VECTOR) & ~(FLAGS) & 0xf0 && (FLAGS) & ~(VECTOR) & 0x0f ? 0 \
   : (VECTOR) & ~(FLAGS) & 0x0f && (FLAGS) & ~(VECTOR) & 0xf0 ? 0 \
   : (VECTOR) & ~(ACC) & 0xf0 && (ACC) & ~(VECTOR) & 0x0f ? 0 \
   : (VECTOR) & ~(ACC) & 0x0f && (ACC) & ~(VECTOR) & 0xf0 ? 0 \
   : (SCALAR) == 0x01 && (VECTOR) & 0x0e && !((VECTOR) & 0xf0) ? 0 \
   : (VECTOR) == 0x01 && (SCALAR) & 0x0e && !((SCALAR) & 0xf0) ? 0 \
   : (SCALAR) == 0x10 && (VECTOR) & 0xe0 && !((VECTOR) & 0x0f) ? 0 \
   : (VECTOR) == 0x10 && (SCALAR) & 0xe0 && !((SCALAR) & 0x0f) ? 0 \
   : 1)

/* Actual fixed hard registers.  */
#define STACK_POINTER_REGNUM 8
#define BSS_BASE_REGNUM 9

/* Frame pointer and argument pointer are nominally hard registers, but
   they don't actually exist in hardware, and have to be always eliminated.  */
#define MXP_FAKE_REG_NAMES { "fp", "ap" }
/* The fake registers (fp, ap) are considerd scalar registers.  Indicate
   the lane set to which they are supposed to belong for the purpose of
   including them in register classes.  */
#define MXP_FAKE_REG_LANES 0xcc

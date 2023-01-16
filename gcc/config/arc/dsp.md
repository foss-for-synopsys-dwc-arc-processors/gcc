;; Copyright (C) 2020 Free Software Foundation, Inc.
;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.

;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; Saturated Arithmetic
(define_code_iterator ss_arith [ss_plus ss_minus ss_mult])
(define_code_iterator ss_addsub [ss_plus ss_minus])
(define_code_iterator ss_absneg [ss_abs ss_neg])
(define_code_iterator ss_shifts [ss_ashift ashiftrt])

(define_code_attr abelian
  [(ss_minus "")
   (ss_plus "%")
   (ss_mult "%")])

;; Map RTX code to its standard insn name
(define_code_attr optab
  [(ss_ashift "ssashl")
   (ss_plus   "ssadd")
   (ss_minus  "sssub")
   (ss_neg    "ssneg")
   (ss_abs    "ssabs")
   (ss_mult   "ssmul")
   (ashiftrt  "ashr")
   ])

(define_code_attr code_asmname
  [(ss_ashift "asls")
   (ss_plus   "adds")
   (ss_minus  "subs")
   (ss_neg    "negs")
   (ss_abs    "abss")
   (ss_mult   "mpyf")
   (ashiftrt  "asrs")
   ])

(define_code_attr code_asmnamec
  [(ss_plus  "adcs")  (ss_minus "sbcs")])

;; Used to implement 16-bit saturated operations.
(define_code_attr code16_asmname
  [(ss_plus   "vadds2h")
   (ss_minus  "vsubs2h")
   (ss_mult   "vmpy2hf")
   (ss_ashift "vasls2h")
   (ashiftrt  "vasrs2h")
   ])

;; To be used when implementing q63 add/sub
(define_code_attr saddsublow
  [(ss_plus  "add")  (ss_minus "sub")])

(define_mode_attr code_asmsuffix
  [(HQ "h") (SQ "")])

(define_mode_iterator FIXED [HQ SQ])

;; 32-bit datum
(define_mode_iterator SIQ [SI SQ])

(define_mode_iterator SST [QQ HQ SI])

;; ssaddsq3 sssubsq3 ssmulsq3
(define_insn "<optab>sq3"
  [(set (match_operand:SQ 0 "register_operand"                       "=r, r,  r")
        (ss_arith:SQ (match_operand:SQ 1 "register_operand"  "<abelian>0, r,  r")
		     (match_operand:SQ 2 "nonmemory_operand"          "I,rL,Cal")))]
  "TARGET_DSPX"
  "<code_asmname>%?\\t%0,%1,%2"
  [(set_attr "length" "4,4,8")
   (set_attr "type" "misc")])

;; ssaddhq3 sssubhq3 ssmulhq3
(define_insn "<optab>hq3"
  [(set (match_operand:HQ 0 "register_operand"                       "=r, r,  r")
        (ss_arith:HQ (match_operand:HQ 1 "register_operand"  "<abelian>0, r,  r")
		     (match_operand:HQ 2 "nonmemory_operand"          "I,rL,Cal")))]
  "TARGET_DSPX"
  "<code16_asmname>%?\\t%0,%1,%2"
  [(set_attr "length" "4,4,8")
   (set_attr "type" "misc")])

;; MADD/MSUB
(define_expand "ssmaddsqdq3"
  [(set (match_operand:DQ 0 "even_register_operand" "")
	(ss_plus:DQ
	 (ss_mult:DQ
	  (sat_fract:DQ (match_operand:SQ 1 "register_operand"  ""))
	  (sat_fract:DQ (match_operand:SQ 2 "nonmemory_operand" "")))
	 (match_operand:DQ 3 "register_operand" "")))
   (clobber (reg:DQ ARCV2_ACC))]
  "TARGET_DSP"
  {
   if (!register_operand (operands[3], DQmode))
      operands[3] = force_reg (DQmode, operands[3]);
   })

(define_insn_and_split "*ssmaddsqdq3"
  [(set (match_operand:DQ 0 "even_register_operand"                 "=r, r,  r")
	(ss_plus:DQ
	 (ss_mult:DQ
	  (sat_fract:DQ (match_operand:SQ 1 "register_operand"  "%0, r,  r"))
	  (sat_fract:DQ (match_operand:SQ 2 "nonmemory_operand"  "I,rL,Cal")))
	 (match_operand:DQ 3 "register_operand" "r,r,r")))
   (clobber (reg:DQ ARCV2_ACC))]
  "TARGET_DSP"
  "#"
  "&& reload_completed"
  [(const_int 0)]
  "{
   rtx acc_reg = gen_rtx_REG (DQmode, ACC_REG_FIRST);
   emit_move_insn (acc_reg, operands[3]);
   gcc_assert(even_register_operand (operands[0], DQmode));
   emit_insn (gen_macdf (operands[0], operands[1], operands[2]));
   DONE;
   }"
  [(set_attr "length" "4,4,8")
   (set_attr "type" "misc")])

(define_insn "macdf"
  [(set (match_operand:DQ 0 "even_register_operand"           "=r, r,  r")
	(ss_plus:DQ
	 (ss_mult:DQ
	  (sat_fract:DQ (match_operand:SQ 1 "register_operand" "%0, r,  r"))
	  (sat_fract:DQ (match_operand:SQ 2 "nonmemory_operand" "I,rL,Cal")))
	 (reg:DQ ARCV2_ACC)))
   (set (reg:DQ ARCV2_ACC)
	(ss_plus:DQ
	 (mult:DQ (sat_fract:DQ (match_dup 1))
		  (sat_fract:DQ (match_dup 2)))
	 (reg:DQ ARCV2_ACC)))]
  "TARGET_DSP"
  "macdf\\t%0,%1,%2"
  [(set_attr "length" "4,4,8")
   (set_attr "type" "misc")])

;; shifts
(define_insn "<optab>sq3"
  [(set (match_operand:SQ 0 "register_operand"                "=r,r, r,  r,  r")
        (ss_shifts:SQ (match_operand:SQ 1 "register_operand"   "0,0, r,  0,  r")
		      (match_operand:SI 2 "nonmemory_operand" "rL,I,rL,Cal,Cal")))]
  "TARGET_DSPX"
  "<code_asmname>%?\\t%0,%1,%2"
  [(set_attr "length" "4,4,4,8,8")
   (set_attr "type" "misc")
   (set_attr "predicable" "yes,no,no,yes,no")
   (set_attr "cond" "canuse,nocond,nocond,canuse_limm,nocond")])

(define_insn "<optab>hq3"
  [(set (match_operand:HQ 0 "register_operand"               "=r, r,  r")
        (ss_shifts:HQ (match_operand:HQ 1 "register_operand"  "0, r,  r")
		      (match_operand:SI 2 "nonmemory_operand" "I,rL,Cal")))]
  "TARGET_DSPX"
  "<code16_asmname>%?\\t%0,%1,%2"
  [(set_attr "length" "4,4,8")
   (set_attr "type" "misc")])

;; ssnegsq2 ssneghq2 ssabssq2 ssabshq2
(define_insn "<optab><mode>2"
  [(set (match_operand:FIXED 0 "register_operand"                   "=r,r")
        (ss_absneg:FIXED (match_operand:FIXED 1 "nonmemory_operand" "rL,Cal")))]
  "TARGET_DSPX"
  "<code_asmname><code_asmsuffix>\\t%0,%1"
  [(set_attr "length" "4,8")
   (set_attr "type" "misc")
   (set_attr "predicable" "no")
   (set_attr "cond" "nocond")])

;; add/sub saturated with carry.  Also sets STATUS.
(define_insn "<optab>sq3_carry"
  [(set (match_operand:SQ 0 "register_operand"           "=r, r,  r")
	(ss_addsub:SQ
	 (ss_addsub:SQ
	  (match_operand:SQ 1 "register_operand" "<abelian>0, r,  r")
	  (match_operand:SQ 2 "nonmemory_operand"         "I,rL,Cal"))
	 (ltu:SI (reg:CC_C CC_REG) (const_int 0))))
   (clobber (reg:CC CC_REG))]
  "TARGET_DSPX"
  "<code_asmnamec>.f\\t%0,%1,%2"
  [(set_attr "cond" "clob")
   (set_attr "length" "4,4,8")
   (set_attr "type" "misc")
   (set_attr "predicable" "no")])

;; Saturate register based on N and V flags.
(define_insn "satfsisq2"
  [(set (match_operand:SQ 0 "register_operand" "=r,r")
	(unspec:SQ [(match_operand:SQ 1 "nonmemory_operand" "rL,Cal")]
		   UNSPEC_ARC_SATF))
   (use (reg:CC CC_REG))]
  "TARGET_DSP"
  "satf\\t%0,%1"
  [(set_attr "length" "4,8")])

(define_insn "*satfsisq2"
  [(set (match_operand:SQ 0 "register_operand" "=r,r")
	(unspec:SQ [(match_operand 1 "immediate_operand" "L,Cal")]
		   UNSPEC_ARC_SATF))
   (use (reg:CC CC_REG))]
  "TARGET_DSP"
  "satf\\t%0,%1"
  [(set_attr "length" "4,8")
   (set_attr "type" "misc")])

;; Patterns used to compute the lower part of a saturated Q63 add/sub
;; opertation.
(define_insn "<optab>sq3_low"
  [(set (match_operand:SQ 0 "register_operand" "=r,r,r")
	(unspec:SQ [(ss_addsub:SQ
		     (match_operand:SQ 1 "register_operand" "%0, r,r")
		     (match_operand:SQ 2 "nonmemory_operand" "I,rL,Cal"))]
		   UNSPEC_ARC_SADDSUBLOW))
   (clobber (reg:CC CC_REG))]
  "TARGET_DSP"
  "<saddsublow>.f\\t%0,%1,%2"
  [(set_attr "length" "4,4,8")
   (set_attr "type" "misc")])

(define_insn "ssaddsq3_sat"
  [(set (match_operand:SQ 0 "register_operand" "=r,r,r")
	(unspec:SQ [(match_operand:SQ 1 "register_operand" "%0, r,r")]
		   UNSPEC_ARC_SADDSUBLOW))
   (clobber (reg:CC CC_REG))]
  "TARGET_DSP"
  "adds.f\\t%0,%1,0"
  [(set_attr "length" "4,4,8")
   (set_attr "type" "misc")])

(define_expand "<optab>dq3"
  [(set (match_operand:DQ 0 "register_operand")
        (ss_addsub:DQ (match_operand:DQ 1 "register_operand")
		      (match_operand:DQ 2 "nonmemory_operand")))]
  "TARGET_DSP"
  "")

(define_insn_and_split "*<optab>dq3"
  [(set (match_operand:DQ 0 "register_operand")
        (ss_addsub:DQ (match_operand:DQ 1 "register_operand")
		      (match_operand:DQ 2 "nonmemory_operand")))]
  "TARGET_DSP"
  "#"
  "&& reload_completed"
  [(const_int 0)]
  {
  rtx low_dest, op1_low, op2_low, high_dest, op1_high, op2_high;

  high_dest = gen_highpart (SQmode, operands[0]);
  low_dest = gen_lowpart (SQmode, operands[0]);
  op1_high = gen_highpart (SQmode, operands[1]);
  op1_low = gen_lowpart (SQmode, operands[1]);
  op2_high = gen_highpart_mode (SQmode, DQmode, operands[2]);
  op2_low = gen_lowpart (SQmode, operands[2]);

  emit_insn (gen_<optab>sq3_low (low_dest, op1_low, op2_low));
  emit_insn (gen_<optab>sq3_carry (high_dest, op1_high, op2_high));
  emit_insn (gen_satfsisq2 (low_dest, low_dest));

  DONE;
  })

;;conversion
(define_insn "satfract<mode>hq2"
  [(set (match_operand:HQ 0 "register_operand"               "=r")
	(sat_fract:HQ (match_operand:SIQ 1 "register_operand" "r")))]
  "TARGET_DSPX"
  "sath\\t%0,%1"
  [(set_attr "length" "4")
   (set_attr "type" "misc")])

;; FIXME! Merge me with above one
(define_insn "satfractqqhq2"
  [(set (match_operand:HQ 0 "register_operand"               "=r")
	(sat_fract:HQ (match_operand:QQ 1 "register_operand"  "r")))]
  "TARGET_DSPX"
  "sath\\t%0,%1"
  [(set_attr "length" "4")
   (set_attr "type" "misc")])

(define_insn "satfract<mode>sq2"
  [(set (match_operand:SQ 0 "register_operand"               "=r")
	(sat_fract:SQ (match_operand:SST 1 "register_operand" "r")))]
  "TARGET_DSPX"
  "adds\\t%0,%1,0"
  [(set_attr "length" "4")
   (set_attr "type" "misc")])

(define_expand "satfractsqdq2"
  [(set (match_operand:DQ 0 "register_operand")
	(sat_fract:DQ (match_operand:SQ 1 "register_operand")))]
  "TARGET_DSPX"
  "")

(define_insn_and_split "*satfractsqdq2"
  [(set (match_operand:DQ 0 "register_operand" "=r")
	(sat_fract:DQ (match_operand:SQ 1 "register_operand" "r")))]
  "TARGET_DSPX"
  "#"
  "&& reload_completed"
  [(const_int 0)]
  {
    rtx low_dest, high_dest;
    low_dest = gen_lowpart (SQmode, operands[0]);
    high_dest = gen_highpart (SQmode, operands[0]);

    emit_insn (gen_ssaddsq3_sat (high_dest, operands[1]));
    emit_insn (gen_satfsisq2 (low_dest, const0_rtx));
    }
  [(set_attr "length" "8")
   (set_attr "type" "misc")])

;; Basic arithmetic operations

;; The falowing patterns are for radix-2 DSP implementation which is
;; clobbering the accumulator registers.  Oddly enough, the radix-4
;; DSP implementation acts like the non-dsp div/rem instruction.
(define_expand "divmodsi4"
  [(set (match_operand:SI 0 "register_operand")
	(div:SI (match_operand:SI 1 "nonmemory_operand")
		(match_operand:SI 2 "nonmemory_operand")))
   (set (match_operand:SI 3 "register_operand")
	(mod:SI (match_dup 1) (match_dup 2)))]
  "TARGET_DSP_DIVREM"
  {
   if (!register_operand (operands[1], SImode)
       && !register_operand (operands[2], SImode))
      operands[1] = force_reg (SImode, operands[1]);
   emit_insn (gen_divmodsi4_i (operands[0], operands[1], operands[2],
				   operands[3]));
   DONE;
   })

(define_expand "udivmodsi4"
  [(set (match_operand:SI 0 "register_operand")
	(udiv:SI (match_operand:SI 1 "nonmemory_operand")
		 (match_operand:SI 2 "nonmemory_operand")))
   (set (match_operand:SI 3 "register_operand")
	(umod:SI (match_dup 1) (match_dup 2)))]
  "TARGET_DSP_DIVREM"
  {
   if (!register_operand (operands[1], SImode)
       && !register_operand (operands[2], SImode))
      operands[1] = force_reg (SImode, operands[1]);
   emit_insn (gen_udivmodsi4_i (operands[0], operands[1], operands[2],
			        operands[3]));
   DONE;
   })

;; GCC as gcc10 can only generate ("/") arithmetic operartions for
;; fixed point.
(define_expand "divsq3"
  [(set (match_operand:SQ 0 "register_operand" "")
	(div:SQ (match_operand:SQ 1 "arc_nonmemory_operand" "")
		(match_operand:SQ 2 "nonmemory_operand" "")))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_DSP && arc_dsp_divsqrt"
  {
   if (!register_operand (operands[1], SQmode)
       && !register_operand (operands[2], SQmode))
      operands[1] = force_reg (SImode, operands[1]);
   emit_insn (gen_divsq3_i (operands[0], operands[1], operands[2]));
   DONE;
   })

;; GCC as gcc10 cannot generate mod<FIXED> operations. Let this
;; pattern here for future extensions
(define_expand "modsq3"
  [(set (match_operand:SQ 0 "register_operand" "")
	(mod:SQ (match_operand:SQ 1 "arc_nonmemory_operand" "")
		(match_operand:SQ 2 "nonmemory_operand" "")))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_DSP && arc_dsp_divsqrt"
  {
   if (!register_operand (operands[1], SQmode)
       && !register_operand (operands[2], SQmode))
      operands[1] = force_reg (SImode, operands[1]);
   emit_insn (gen_modsq3_i (operands[0], operands[1], operands[2]));
   DONE;
  })

(define_insn "divmodsi4_i"
  [(set (match_operand:SI 0 "register_operand"         "=r,r,  r,r,r,r,  r,  r")
	(div:SI (match_operand:SI 1 "arc_nonmemory_operand" "0,r,Csz,0,r,0,  0,  r")
		(match_operand:SI 2 "nonmemory_operand" "r,r,  r,L,L,I,Cal,Cal")))
   (set (match_operand:SI 3 "register_operand"         "=r,r,  r,r,r,r,  r,  r")
	(mod:SI (match_dup 1) (match_dup 2)))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_DSP_DIVREM
   && (register_operand (operands[1], SImode)
       || register_operand (operands[2], SImode))"
  {
   if (find_reg_note (insn, REG_UNUSED, operands[0]))
      return "rem%?\\t%3,%1,%2";
   else if (find_reg_note (insn, REG_UNUSED, operands[3]))
      return "div%?\\t%0,%1,%2";
   else if (TARGET_BIG_ENDIAN)
      return "div%?\\t%0,%1,%2\\n\\tmov%?\\t%3,r59";
   else
      return "div%?\\t%0,%1,%2\\n\\tmov%?\\t%3,r58";
  }
  [(set_attr "length" "8,8,12,8,8,8,12,12")
   (set_attr "iscompact" "false")
   (set_attr "type" "div_rem")
   (set_attr "predicable" "yes,no,no,yes,no,no,yes,no")
   (set_attr "cond" "canuse,nocond,nocond,canuse,nocond,nocond,canuse,nocond")
   ])

(define_insn "udivmodsi4_i"
  [(set (match_operand:SI 0 "register_operand"         "=r,r,  r,r,r,r,  r,  r")
	(udiv:SI (match_operand:SI 1 "arc_nonmemory_operand" "0,r,Csz,0,r,0,  0,  r")
		 (match_operand:SI 2 "nonmemory_operand" "r,r,  r,L,L,I,Cal,Cal")))
   (set (match_operand:SI 3 "register_operand"         "=r,r,  r,r,r,r,  r,  r")
	(umod:SI (match_dup 1) (match_dup 2)))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_DSP_DIVREM
   && (register_operand (operands[1], SImode)
       || register_operand (operands[2], SImode))"
  {
   if (find_reg_note (insn, REG_UNUSED, operands[0]))
      return "remu%?\\t%3,%1,%2";
   else if (find_reg_note (insn, REG_UNUSED, operands[3]))
      return "divu%?\\t%0,%1,%2";
   else if (TARGET_BIG_ENDIAN)
      return "divu%?\\t%0,%1,%2\\n\\tmov%?\\t%3,r59";
   else
      return "divu%?\\t%0,%1,%2\\n\\tmov%?\\t%3,r58";
  }
  [(set_attr "length" "8,8,12,8,8,8,12,12")
   (set_attr "iscompact" "false")
   (set_attr "type" "div_rem")
   (set_attr "predicable" "yes,no,no,yes,no,no,yes,no")
   (set_attr "cond" "canuse,nocond,nocond,canuse,nocond,nocond,canuse,nocond")
   ])

(define_insn "divsq3_i"
  [(set (match_operand:SQ 0 "register_operand"             "=r,r,  r,r,r,r,  r,  r")
	(div:SQ (match_operand:SQ 1 "arc_nonmemory_operand" "0,r,Csz,0,r,0,  0,  r")
		(match_operand:SQ 2 "nonmemory_operand"     "r,r,  r,L,L,I,Cal,Cal")))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_DSP && arc_dsp_divsqrt
   && (register_operand (operands[1], SQmode)
       || register_operand (operands[2], SQmode))"
  "divf%?\\t%0,%1,%2"
  [(set_attr "length" "8,8,12,8,8,8,12,12")
   (set_attr "iscompact" "false")
   (set_attr "type" "div_rem")
   (set_attr "predicable" "yes,no,no,yes,no,no,yes,no")
   (set_attr "cond" "canuse,nocond,nocond,canuse,nocond,nocond,canuse,nocond")
   ])

(define_insn "modsq3_i"
  [(set (match_operand:SQ 0 "register_operand"             "=r,r,  r,r,r,r,  r,  r")
	(mod:SQ (match_operand:SQ 1 "arc_nonmemory_operand" "0,r,Csz,0,r,0,  0,  r")
		(match_operand:SQ 2 "nonmemory_operand"     "r,r,  r,L,L,I,Cal,Cal")))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_DSP && arc_dsp_divsqrt
   && (register_operand (operands[1], SQmode)
       || register_operand (operands[2], SQmode))"
  {
   if (TARGET_BIG_ENDIAN)
      return "divf%?\\t0,%1,%2\\n\\tmov%?\\t%0,r59";
   else
      return "divf%?\\t0,%1,%2\\n\\tmov%?\\t%0,r58";
  }
  [(set_attr "length" "8,8,12,8,8,8,12,12")
   (set_attr "iscompact" "false")
   (set_attr "type" "div_rem")
   (set_attr "predicable" "yes,no,no,yes,no,no,yes,no")
   (set_attr "cond" "canuse,nocond,nocond,canuse,nocond,nocond,canuse,nocond")
   ])

;; Integral sqrt instructions.
(define_expand "sqrt<mode>2"
  [(set (match_operand:SIQ 0 "register_operand" "")
	(sqrt:SI (match_operand:SIQ 1 "nonmemory_operand" "")))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_DSP_SQRT"
  "")

(define_insn "*sqrt<mode>2"
  [(set (match_operand:SIQ 0 "register_operand" "=r,r,r")
	(sqrt:SIQ (match_operand:SIQ 1 "nonmemory_operand" "r,L,Cal")))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_DSP_SQRT"
  "sqrt\\t%0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "type" "misc")])

;; MPYW instructions
(define_insn "mulhisi3_dsp"
  [(set (match_operand:SI 0 "register_operand"                          "=q,r,r")
	(mult:SI (sign_extend:SI (match_operand:HI 1 "register_operand"  "0,0,r"))
		 (sign_extend:SI (match_operand:HI 2 "nonmemory_operand" "q,r,r"))))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_DSP"
  "mpyw%?\\t%0,%1,%2"
  [(set_attr "length" "*,4,4")
   (set_attr "iscompact" "maybe,false,false")
   (set_attr "type" "mul16_em")
   (set_attr "predicable" "yes,yes,no")
   (set_attr "cond" "canuse,canuse,nocond")
   ])

(define_insn "*mulhisi3_idsp"
  [(set (match_operand:SI 0 "register_operand"                         "=r,r,r,  r,  r")
	(mult:SI (sign_extend:SI (match_operand:HI 1 "register_operand" "0,r,0,  0,  r"))
		 (match_operand:HI 2 "short_const_int_operand"          "L,L,I,C16,C16")))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_DSP"
  "mpyw%?\\t%0,%1,%2"
  [(set_attr "length" "4,4,4,8,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "mul16_em")
   (set_attr "predicable" "yes,no,no,yes,no")
   (set_attr "cond" "canuse,nocond,nocond,canuse_limm,nocond")
   ])

(define_insn "umulhisi3_dsp"
  [(set (match_operand:SI 0 "register_operand"                          "=q, r, r")
	(mult:SI (zero_extend:SI (match_operand:HI 1 "register_operand" "%0, 0, r"))
		 (zero_extend:SI (match_operand:HI 2 "register_operand"  "q, r, r"))))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_DSP"
  "mpyuw%?\\t%0,%1,%2"
  [(set_attr "length" "*,4,4")
   (set_attr "iscompact" "maybe,false,false")
   (set_attr "type" "mul16_em")
   (set_attr "predicable" "yes,yes,no")
   (set_attr "cond" "canuse,canuse,nocond")
   ])

(define_insn "*umulhisi3_idsp"
  [(set (match_operand:SI 0 "register_operand"                          "=r, r,  r,  r,  r")
	(mult:SI (zero_extend:SI (match_operand:HI 1 "register_operand" "%0, r,  0,  0,  r"))
		 (match_operand:HI 2 "short_unsigned_const_operand"     " L, L,J12,J16,J16")))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_DSP"
  "mpyuw%?\\t%0,%1,%2"
  [(set_attr "length" "4,4,4,8,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "mul16_em")
   (set_attr "predicable" "yes,no,no,yes,no")
   (set_attr "cond" "canuse,nocond,nocond,canuse_limm,nocond")
   ])

;; mpy mpyu mpym mpymu are the same as the standard implementation,
;; nothing to do for now.
;; The same situation for plus_dmpy mpy instructions.

;; Vector packing/unpacking

(define_insn "vec_duplicatev2hi"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(vec_duplicate:V2HI
	 (match_operand:HI 1 "register_operand"  "r")))]
  "TARGET_DSP"
  "vrep2hl\\t%0,%1"
  [(set_attr "length" "4")
   (set_attr "type" "vpack")])

(define_insn "vec_shl_insert_v2hi"
  [(set (match_operand:V2HI 0 "register_operand"  "=r")
	(unspec:V2HI
	 [(match_operand:V2HI 1 "register_operand" "r")
	  (match_operand:HI 2 "register_operand"   "r")]
	 UNSPEC_ARC_VPACK2HL))]
  "TARGET_DSP"
  "vpack2hl\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "vpack")])

;; DSP vector arithmetics
(define_insn "smaxv2hi3"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
       (smax:V2HI (match_operand:V2HI 1 "register_operand" "r")
                  (match_operand:V2HI 2 "register_operand" "r")))]
  "TARGET_DSP"
  "vmax2h\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "vpack")])

(define_insn "sminv2hi3"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
       (smin:V2HI (match_operand:V2HI 1 "register_operand" "r")
                  (match_operand:V2HI 2 "register_operand" "r")))]
  "TARGET_DSP"
  "vmin2h\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "vpack")])

(define_insn "*negv2hi2"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(neg:V2HI (match_operand:V2HI 1 "register_operand" "r")))]
  "TARGET_DSP"
  "vneg2h\\t%0,%1"
  [(set_attr "length" "4")
   (set_attr "type" "vpack")])

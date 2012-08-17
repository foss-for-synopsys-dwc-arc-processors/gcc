;; GCC machine description for ARC mxp.
;; Copyright (C) 2008 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
;;
;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

(include ("mxp-regset.md"))
(include ("predicates.md"))
(include ("constraints.md"))

(define_attr "type" "jump,load,store,other" (const_string "other"))

;; instruction lengths are in words.  Actual machine instructions are always
;; one word, but some patterns here might emit multiple machine instructions.
(define_attr "length" "" (const_int 1))

;; The mxp allows anything in a delay slot - even jumps.
;; However, gcc's delay slot scheduling pass gets confused if we allow this.
;; Also, we lack a mechanism to fill more than one delay slot with one insn
;; pattern.
(define_attr "in_delay_slot" "no,yes"
  (cond [(eq_attr "type" "jump") (const_string "no")
	 (eq_attr "length" "1") (const_string "yes")] (const_string "no")))

(define_delay
  (eq_attr "type" "jump")
  [(eq_attr "in_delay_slot" "yes") (nil) (nil)
   (eq_attr "in_delay_slot" "yes") (nil) (nil)
   (eq_attr "in_delay_slot" "yes") (nil) (nil)])
	      
;; moves and logical operations can be performed on 128 bit vectors and
;; parts of them in 16-bit granularity.
(define_mode_iterator VECI [HI SI DI TI V2HI V4HI V8HI V2SI V4SI V2DI])

(define_mode_iterator VECX
  [QI HI CCI CCZN CCZ SI DI TI V2HI V2CCI V2CCZN V2CCZ V4HI V4CCI V4CCZN V4CCZ
   V8HI V8CCI V8CCZN V8CCZ V2SI V4SI V2DI SF V2SF DF V4SF V2DF])

;; Like VECI, but with size 32 bit (like SImode)
(define_mode_iterator VECSI [SI V2HI])

;; Like VECI, but with minimum size 32 bit (like SImode)
(define_mode_iterator VECMSI [SI DI TI V2HI V4HI V8HI V2SI V4SI V2DI])

;; Like VECI, but with minimum size 64 bit (like DImode)
(define_mode_iterator VECMDI [DI TI V4HI V8HI V2SI V4SI V2DI])

; like VECX, but with size 64 bit (line DImode)
(define_mode_iterator VECDX [V4HI V4CCI V4CCZN V4CCZ V2SI V2CC V2SF DI DF])
(define_mode_attr HALF_MODE [(V4HI "V2HI") (V4CCI "V2CCI") (V4CCZN "V2CCZN")
			     (V4CCZ "V2CCZ") (V2SI "SI") (V2CC "CC")
			     (V2SF "SF") (DI "SI") (DF "SI")])

;; single-instruction patterns for arithmetic like add/sub is
;; limited to 16 and 32 bit per data item.
(define_mode_iterator VECA [HI SI V2HI V4HI V8HI V2SI V4SI])

;; Like VECA, but exclude HI / SI
(define_mode_iterator VECVA [V2HI V4HI V8HI V2SI V4SI])

(define_mode_iterator VXHI [HI V2HI V4HI V8HI])
(define_mode_iterator VXSI [SI V2SI V4SI])
(define_mode_iterator VXDI [DI V2DI])

(define_mode_iterator HSI [HI SI])

(define_mode_iterator CCX [CC])

(define_mode_attr unit_suffix [(HI "w") (V2HI "w") (V4HI "w") (V8HI "w")
			       (SI "") (V2SI "") (V4SI "")])
(define_mode_attr size_suffix
  [(QI "w") (HI "w") (CCI "w") (CCZN "w") (CCZ "w")
   (V2HI "") (V2CCI "") (V2CCZN "") (V2CCZ "") (SI "") (SF "")])

(define_mode_attr v0
  [(QI "R01Z") (HI "R01Z") (CCI "fv01Z") (CCZN "fv01Z") (CCZ "fv01Z")
   (V2HI "R03Z") (V2CCI "fv03Z") (V2CCZN "fv03Z") (V2CCZ "fv03Z")
   (SI "R03Z") (SF "R03Z")
   (V4HI "R0fZ") (V4CCI "fv0fZ") (V4CCZN "fv0fZ") (V4CCZ "fv0fZ")
   (V2SI "R0fZ") (DI "R0fZ") (V2SF "R0fZ") (DF "R0fZ")
   (V8HI "vZ") (V8CCI "fvffZ") (V8CCZN "fvffZ") (V8CCZ "fvffZ")
   (V4SI "vZ") (V2DI "vZ") (TI "vZ") (V4SF "vZ") (V2DF "vZ")])

(define_mode_attr v4
  [(QI "R10Z") (HI "R10Z") (CCI "fv10Z") (CCZN "fv10Z") (CCZ "fv10Z")
   (V2HI "R30Z") (V2CCI "fv30Z") (V2CCZN "fv30Z") (V2CCZ "fv30Z")
   (SI "R30Z") (SF "R30Z")
   (V4HI "Rf0Z") (V4CCI "fvf0Z") (V4CCZN "fvf0Z") (V4CCZ "fvf0Z")
   (V2SI "Rf0Z") (DI "Rf0Z") (V2SF "Rf0Z") (DF "Rf0Z")
   (V8HI "vZ") (V8CCI "fvffZ") (V8CCZN "fvffZ") (V8CCZ "fvffZ")
   (V4SI "vZ") (V2DI "vZ") (TI "vZ") (V4SF "vZ") (V2DF "vZ")])

(define_mode_attr vn
  [(QI "vZ") (HI "vZ") (CCI "fvffZ") (CCZN "fvffZ") (CCZ "fvffZ")
   (V2HI "vZ") (V2CCI "fvffZ") (V2CCZN "fvffZ") (V2CCZ "fvffZ")
   (SI "vZ") (SF "vZ")
   (V4HI "vZ") (V4CCI "fvffZ") (V4CCZN "fvffZ") (V4CCZ "fvffZ")
   (V2SI "vZ") (DI "vZ") (V2SF "vZ") (DF "vZ") (V2CC "vZ")
   (V8HI "vZ") (V8CCI "fvffZ") (V8CCZN "fvffZ") (V8CCZ "fvffZ")
   (V4SI "vZ") (V2DI "vZ") (TI "vZ") (V4SF "vZ") (V2DF "vZ") (V4CC "vZ")])

(define_mode_attr vx
  [(HI "R01Z,R10Z") (V2HI "R03Z,R30Z") (SI "R03Z,R30Z") (SF "R03,R30")
   (V4HI "R0f,Rf0") (V2SI "R0f,Rf0") (DI "R0f,Rf0") (V2SF "R0f,Rf0")
   (DF "R0f,Rf0")
   (V8HI "v") (V4SI "v") (V2DI "v") (TI "v") (V4SF "v") (V2DF "v")])

(define_mode_attr vxp
  [(HI "R01Z,R10Z") (V2HI "R03Z,R30Z") (SI "R03Z,R30Z") (SF "R03,R30")
   (V4HI "R0f,Rf0") (V2SI "R0f,Rf0") (DI "R0f,Rf0?") (V2SF "R0f,Rf0")
   (DF "R0f,Rf0")
   (V8HI "v") (V4SI "v") (V2DI "v") (TI "v") (V4SF "v") (V2DF "v")])

(define_mode_attr a0
  [(QI "Ral") (HI "Ral") (CCI "Ral") (CCZN "Ral") (CCZ "Ral")
   (V2HI "Ral") (V2CCI "Ral") (V2CCZN "Ral") (V2CCZ "Ral") (SI "Ral") (SF "Ral")
   (V4HI "Ral") (V4CCI "Ral") (V4CCZN "Ral") (V4CCZ "Ral") (V2SI "Ral")
   (DI "Ral") (V2SF "Ral") (DF "Ral")
   (V8HI "Rac") (V8CCI "Rac") (V8CCZN "Rac") (V8CCZ "Rac") (V4SI "Rac")
   (V2DI "Rac") (TI "Rac") (V4SF "Rac") (V2DF "Rac")])

(define_mode_attr a4
  [(QI "Rah") (HI "Rah") (CCI "Rah") (CCZN "Rah") (CCZ "Rah")
   (V2HI "Rah") (V2CCI "Rah") (V2CCZN "Rah") (V2CCZ "Rah") (SI "Rah") (SF "Rah")
   (V4HI "Rah") (V4CCI "Rah") (V4CCZN "Rah") (V4CCZ "Rah") (V2SI "Rah")
   (DI "Rah") (V2SF "Rah") (DF "Rah")
   (V8HI "Rac") (V8CCI "Rac") (V8CCZN "Rac") (V8CCZ "Rac") (V4SI "Rac")
   (V2DI "Rac") (TI "Rac") (V4SF "Rac") (V2DF "Rac")])

(define_mode_attr ax
  [(HI "Ral,Rah") (V2HI "Ral,Rah") (SI "Ral,Rah") (SF "Ral,Rah")
   (V4HI "Ral,Rah") (V2SI "Ral,Rah") (DI "Ral,Rah") (V2SF "Ral,Rah")
   (DF "Ral,Rah")
   (V8HI "Rac") (V4SI "Rac") (V2DI "Rac") (TI "Rac") (V4SF "Rac") (V2DF "Rac")])

(define_mode_attr fx
  [(DI "Rfl,Rfh") (V2DI "Rfg")])

(define_mode_attr c4
  [(HI ",") (V2HI ",") (SI ",") (SF ",")
   (V4HI ",") (V2SI ",") (DI ",") (V2SF ",") (DF ",")
   (V8HI "") (V4SI "") (V2DI "") (TI "") (V4SF "") (V2DF "")])

(define_mode_attr I16v
  [(HI "i") (CCI "i") (CCZN "i") (CCZ "i")
   (V2HI "J2r16") (V2CCI "J2r16") (V2CCZN "J2r16") (V2CCZ "J2r16") (SI "J2r16")
   (V4HI "J4r16") (V4CCI "J4r16") (V4CCZN "J4r16") (V4CCZ "J4r16")
   (V2SI "J4r16") (DI "J4r16")
   (V8HI "J8r16") (V8CCI "J8r16") (V8CCZN "J8r16") (V8CCZ "J8r16")
   (V4SI "J8r16") (V2DI "J8r16") (TI "J8r16")
   (SF "F") (V2SF "G2r") (DF "G2r") (V4SF "G4r") (V2DF "G4r")])

(define_mode_attr I32v
  [(QI "i") (HI "i") (CCI "i") (CCZN "i") (CCZ "i")
   (V2HI "i") (V2CCI "i") (V2CCZN "i") (V2CCZ "i") (SI "i")
   (V4HI "J2r32") (V4CCI "J2r32") (V4CCZN "J2r32") (V4CCZ "J2r32")
   (V2SI "J2r32") (DI "J2r32")
   (V8HI "J4r32") (V8CCI "J4r32") (V8CCZN "J4r32") (V8CCZ "J4r32")
   (V4SI "J4r32") (V2DI "J4r32") (TI "J4r32")
   (SF "F") (V2SF "G2r") (DF "G2r") (V4SF "G4r") (V2DF "G4r")])

(define_mode_attr I32m
  [(QI "i") (HI "i") (CCI "i") (CCZN "i") (CCZ "i")
   (V2HI "i") (V2CCI "i") (V2CCZN "i") (V2CCZ "i") (SI "i")
   (V4HI "i") (V4CCI "i") (V4CCZN "i") (V4CCZ "i")
   (V2SI "i") (DI "i")
   (V8HI "J4r32") (V8CCI "J4r32") (V8CCZN "J4r32") (V8CCZ "J4r32")
   (V4SI "J4r32") (V2DI "J4r32") (TI "J4r32")
   (SF "F") (V2SF "G2r") (DF "G2r") (V4SF "G4r") (V2DF "G4r")])

(define_mode_attr VXCC [(DI "V2CC") (V2DI "V4CC")])

(define_expand "mov<mode>"
  [(set (match_operand:VECX 0 "nonimmediate_operand" "")
	(match_operand:VECX 1      "general_operand" ""))]
  ""
  "
{
  if (!register_operand (operands[0], <MODE>mode)
      && !(reg_or_0_operand (operands[1], <MODE>mode)
	   || (GET_MODE_SIZE (<MODE>mode <= 4
	       ? CONSTANT_P (operands[1])
	       : satisfies_constraint_I32 (operands[1])))))
    operands[1] = copy_to_mode_reg (<MODE>mode, operands[1]);
}")

(define_insn "*mov<mode>_i"
  [(set (match_operand:VECX 0 "nonimmediate_operand"
	 "=<vn>,Rac,<v0>,<v4>,<vn>,<vn>,m,m")
	(match_operand:VECX 1 "general_operand"
	 "<vn>,<vn>,<a0>,<a4>,<I32m>,m,Z,<vn>"))]
  "register_operand (operands[0], <MODE>mode)
   || reg_or_0_operand (operands[1], <MODE>mode)"
  "*
{
  switch (which_alternative)
    {
    case 0: case 1:
      {
	int src, dst, srclane, dstlane;
	rtx xop[3];

	dst = REGNO (operands[0]);
	dstlane = dst <= LAST_SCALAR_REG ? dst & 7 : VREG_LANE (dst);
	if (REG_P (operands[1]))
	  {
	    src = REGNO (operands[1]);
	    srclane = src <= LAST_SCALAR_REG ? src & 7 : VREG_LANE (src);
	  }
	else /* const0_rtx */
	  srclane = dstlane;
	if (srclane == dstlane)
	  return (dst >= ACC_REG && dst < ACC_REG + REGS_PER_VREG
		  ? \"vaddw.%L0, %v0,%v1,vr62\"
		  : \"vmvw.%L0 %v0,%v1\" /* Acc unchanged.  */);
	if (dst <= LAST_SCALAR_REG)
	  return (GET_MODE_SIZE (<MODE>mode) <= 2
		  ? \"viv.%L1 %0,%v1\"
		  : \"vkv.%L1 %0,%v1\");  /* Acc unchanged.  */
	if (dst >= ACC_REG && dst < ACC_REG + REGS_PER_VREG)
	  {
	    if (src <= LAST_SCALAR_REG)
	      return \"vmiv<size_suffix>.%L0 %v0,%1\";
	    /* vmr has higher latency.  */
	    xop[0] = operands[0];
	    xop[1] = operands[1];
	    xop[2] = GEN_INT ((srclane - dstlane) & 7);
	    output_asm_insn (\"vmr%2w.%L0 %v0,%v1,%v1\", xop);
	    return \"\";
	  }
	/* Non-scalar cross-lane moves have higher latency.  */
	if (GET_MODE_SIZE (<MODE>mode) <= 4)
	  return \"vxsum<size_suffix>i.%L0 %v0,%v1,%L1\";
	if ((src ^ dst) & ~(REGS_PER_VREG - 1))
	  {
	    if (GET_MODE_SIZE (<MODE>mode) == 8 && (srclane ^ dstlane) == 4)
	      return ((dstlane & 4)
		      ? \"vexch4.%L0 %v0,%v1\" : \"vexch4.%L0 %v1,%v0\");
	  }
	if (GET_MODE_SIZE (<MODE>mode) == 8)
	  return \"vxsumi.%O0 %v0,%v1,%O1` vxsumi.%P0 %v0,%v1,%P1\";
	gcc_unreachable ();
      }
    case 2: case 3:
      return \"vaddaw.%L0 %v0,vr62,vr62\";
    case 4:
      if (GET_MODE_SIZE (<MODE>mode) == 8
	  && !satisfies_constraint_J2r32 (operands[1]))
	return \"vmov.%O0 %v0,%Q1` vmov.%P0 %v0,%R1\";
      if (GET_MODE_SIZE (<MODE>mode) == 2)
	return \"vmovw %v0,%1,%L0\";
      return \"vmov.%L0 %v0,%1\"; /* Acc unchanged.  */
    case 5:
      if (TARGET_NO_VLD_LABEL && ! REG_P (XEXP (operands[1], 0)))
	return  \"vld%N0 %v0,[i9,0]; ??? SHOULD BE: %1\";
      return \"vld%N0 %v0,%1\";
    case 6:
      {
	rtx xop[3];
	xop[0] = operands[0];
	xop[1] = operands[1];
	xop[2] = GEN_INT (GET_MODE_BITSIZE (<MODE>mode));
	output_asm_insn (\"vst%2 %v1,%0\", xop);
	return \"\";
      }
    case 7:
      return \"vst%M1 %v1,%0\";
    default: gcc_unreachable ();
    }
}"
  [(set_attr "type" "*,*,*,*,*,load,store,store")])

;; There is no v{ld,st}64_4 .  Split into two 32 bit moves.
(define_split 
  [(set (match_operand:VECDX 0 "nonimmediate_operand")
	(match_operand:VECDX 1 "nonimmediate_operand"))]
  "reload_completed
   && ((memory_operand (operands[0], <MODE>mode)
	&& VREG_LANE (REGNO (operands[1])))
       || (memory_operand (operands[1], <MODE>mode)
	   && VREG_LANE (REGNO (operands[0]))))"
  [(set (match_dup 2) (match_dup 3))
   (set (match_dup 4) (match_dup 5))]
  "
{
  enum machine_mode submode = <HALF_MODE>mode;

  operands[2] = simplify_gen_subreg (submode, operands[0], <MODE>mode, 0);
  operands[3] = simplify_gen_subreg (submode, operands[1], <MODE>mode, 0);
  operands[4] = simplify_gen_subreg (submode, operands[0], <MODE>mode, 4);
  operands[5] = simplify_gen_subreg (submode, operands[1], <MODE>mode, 4);
}")

; vmiv has lower latency than vxsumi, but it clobbers the accumulator.
; vmr is faster than two vxsumi, but it clobbers the accumulator.
(define_peephole2
  [(set (match_operand 0 "register_operand" "")
	(match_operand 1 "register_operand" ""))]
  "REGNO (operands[0]) > LAST_SCALAR_REG
   && VREG_LANE (REGNO (operands[0])) != VREG_LANE (REGNO (operands[1]))
   && (REGNO (operands[1]) <= LAST_SCALAR_REG
       || GET_MODE_SIZE (GET_MODE (operands[0])) == 8)
   && (peep2_regno_mode_dead_p
	(1,
	 ACC_REG + VREG_LANE (REGNO (operands[0])) / LANES_PER_REG,
	 GET_MODE (operands[0])))"
  [(parallel [(set (match_dup 0) (match_dup 1))
	      (clobber (match_dup 2))])]
  "
{
  operands[2]
    = gen_rtx_REG (GET_MODE (operands[0]),
		   ACC_REG + VREG_LANE (REGNO (operands[0])) / LANES_PER_REG);
}")

;; post-reload pattern for cross-lane moves.
;; regrename can change a cross-lane move into an in-lane move.
;; We could prohibit this, but that would then reduce the effectiveness of
;; regrename.  So rather accept in-lane moves, and split away the clobber.
(define_insn_and_split "*mov<mode>_clob"
  [(set (match_operand:VECX 0 "register_operand" "=<vn>")
	(match_operand:VECX 1 "register_operand" "<vn>"))
   (clobber (match_operand 2 "register_operand" "=Rac"))]
  "reload_completed
   && (VREG_LANE_MASK (operands[0]) & ~VREG_LANE_MASK (operands[2])) == 0"
  "*
{
  int src = REGNO (operands[0]), dst = REGNO (operands[1]);
  int srclane = src <= LAST_SCALAR_REG ? src & 7 : VREG_LANE (src);
  int dstlane = dst <= LAST_SCALAR_REG ? dst & 7 : VREG_LANE (dst);
  rtx xop[3];
  if (src <= LAST_SCALAR_REG)
    return \"vmiv<size_suffix>.%L0 %v0,%1\";
  /* vmr has higher latency.  */
  xop[0] = operands[0];
  xop[1] = operands[1];
  xop[2] = GEN_INT ((srclane - dstlane) & 7);
  output_asm_insn (\"vmr%2w.%L0 %v0,%v1,%v1\", xop);
  return \"\";
}"
  "&& VREG_LANE (REGNO (operands[0])) == VREG_LANE (REGNO (operands[1]))"
  [(set (match_dup 0) (match_dup 1))]
  "")

(define_insn "andhi3"
  [(set (match_operand:HI 0 "register_operand" "=R01,R10,S_n")
	(and:HI (match_operand:HI 1 "register_operand" "%R01,R10,S_n")
		(match_operand:HI 2 "nonmemory_operand" "R01,R10,i")))
   (clobber (match_scratch:HI 3 "=Ral,Rah,X"))]
  ""
  "@
   vand.%L0 %v0,%v1,%v2
   vand.%L0 %v0,%v1,%v2
   via %0,%1,%2")

(define_insn "and<mode>3"
  [(set (match_operand:VECMSI 0 "register_operand" "=<vx>")
	(and:VECMSI (match_operand:VECMSI 1 "register_operand" "%<vx>")
		    (match_operand:VECMSI 2 "register_operand" "<vx>")))
   (clobber (match_scratch:VECMSI 3 "=<ax>"))]
  ""
  "vand.%L0 %v0,%v1,%v2")

(define_insn "ior<mode>3"
  [(set (match_operand:VECI 0 "register_operand" "=<vx>")
	(ior:VECI (match_operand:VECI 1 "register_operand" "%<vx>")
		  (match_operand:VECI 2 "register_operand" "<vx>")))
   (clobber (match_scratch:VECI 3 "=<ax>"))]
  ""
  "vor.%L0 %v0,%v1,%v2")

(define_insn "xor<mode>3"
  [(set (match_operand:VECI 0 "register_operand" "=<vx>")
	(xor:VECI (match_operand:VECI 1 "register_operand" "%<vx>")
		  (match_operand:VECI 2 "register_operand" "<vx>")))
   (clobber (match_scratch:VECI 3 "=<ax>"))]
  ""
  "vxor.%L0 %v0,%v1,%v2")

(define_insn "addhi3"
  [(set (match_operand:HI 0 "register_operand" "=R01,R10,S_n")
	(plus:HI (match_operand:HI 1 "register_operand" "%R01,R10,S_n")
		 (match_operand:HI 2 "nonmemory_operand" "R01,R10,i")))]
  ""
;; This is how we want to do it when binutils is fixed to handle
;; relocations for vim
;;  "@
;;   vaddnaw.%L0 %v0,%v1,%v2
;;   vaddnaw.%L0 %v0,%v1,%v2
;;   vim %0,%1,%i2")
{
  switch (which_alternative)
    {
    case 0: return "vaddnaw.%L0 %v0,%v1,%v2";
    case 1: return "vaddnaw.%L0 %v0,%v1,%v2";
    case 2: if (TARGET_NO_VIM_LABEL && !CONST_INT_P (operands[2]))
	{
	  rtx xop[4];

	  xop[0] = operands[0];
	  xop[1] = operands[1];
	  xop[2] = operands[2];
	  xop[3] = gen_rtx_REG (HImode, true_regnum (operands[0]) ^ 8);
	  output_asm_insn ("vim %0,%1,0` "
			   "vpushiw vr31,%3` "
			   "vmovw %v3,%2,%L0` "
			   "vaddnaw.%L0 %v0,%v0,%v3` "
			   "vpopw %3,vr31,pcl",
			   xop);
	  return "";
	}
      else
	return "vim %0,%1,%i2";
    }
})

(define_insn "addsi3"
  [(set (match_operand:SI 0 "register_operand" "=R03,R30,S_n,R03,R30")
	(plus:SI (match_operand:SI 1 "register_operand" "%R03,R30,S_n,R03,R30")
		 (match_operand:SI 2 "nonmemory_operand""R03,R30,I16,S_n,S_n")))
   (clobber (match_scratch:SI 3 "=X,X,X,Ral,Rah"))]
  ""
  "@
   vaddnaw.%L0 %v0,%v1,%v2
   vaddnaw.%L0 %v0,%v1,%v2
   vkm %0,%1,%2
   vkadd %v0,%v1,%2
   vkadd %v0,%v1,%2")

(define_insn "add<mode>3"
  [(set (match_operand:VECVA 0 "register_operand" "=<vx>")
	(plus:VECVA (match_operand:VECVA 1 "register_operand" "%<vx>")
		    (match_operand:VECVA 2 "register_operand" "<vx>")))]
  ""
  "vaddna<unit_suffix>.%L0 %v0,%v1,%v2")

;; ??? This should be a define_insn_and_split.
(define_insn "add<mode>3"
  [(set (match_operand:VXDI 0 "register_operand" "=<vxp>")
	(plus:VXDI (match_operand:VXDI 1 "register_operand" "%<vx>")
		   (match_operand:VXDI 2 "register_operand" "<vx>")))
   (clobber (match_scratch:<VXCC> 3 "=<fx>"))
   (clobber (match_scratch:VXDI 4 "=<ax>"))]
  ""
  "vaddna.%L0.f %v0,%v1,%v2` vmr6w.%P0 vr63,vr63,vr63` vadc.%P0 %v0,%v0,vr62")

(define_insn "sub<mode>3"
  [(set (match_operand:VECA 0 "register_operand" "=<vx>")
	(minus:VECA (match_operand:VECA 1 "register_operand" "<vx>")
		     (match_operand:VECA 2 "register_operand" "<vx>")))]
  ""
  "vsubna<unit_suffix>.%L0 %v0,%v1,%v2")

;; ??? This should be a define_insn_and_split.
(define_insn "sub<mode>3"
  [(set (match_operand:VXDI 0 "register_operand" "=<vxp>")
	(minus:VXDI (match_operand:VXDI 1 "register_operand" "<vx>")
		    (match_operand:VXDI 2 "register_operand" "<vx>")))
   (clobber (match_scratch:<VXCC> 3 "=<fx>"))
   (clobber (match_scratch:VXDI 4 "=<ax>"))]
  ""
  "vsubna.%L0.f %v0,%v1,%v2` vmr6w.%P0 vr63,vr63,vr63` vsbc.%P0 %v0,%v0,vr62")

(define_insn "ashl<mode>3"
  [(set (match_operand:VECA 0 "register_operand" "=<vx>")
        (ashift:VECA (match_operand:VECA 1 "register_operand" "<vx>")
		     (match_operand:VECA 2 "register_operand" "<vx>")))
   (clobber (match_scratch:VECA 3 "=&<ax>"))]
  ""
  "vlslv<unit_suffix>.%L0 %v0,%v1,%v2")

(define_insn "lshr<mode>3"
  [(set (match_operand:VECA 0 "register_operand" "=<vx>")
        (lshiftrt:VECA (match_operand:VECA 1 "register_operand" "<vx>")
		       (match_operand:VECA 2 "register_operand" "<vx>")))
   (clobber (match_scratch:VECA 3 "=&<ax>"))]
  ""
  "vlsrv<unit_suffix>.%L0 %v0,%v1,%v2")

(define_insn "ashr<mode>3"
  [(set (match_operand:VECA 0 "register_operand" "=<vx>")
        (ashiftrt:VECA (match_operand:VECA 1 "register_operand" "<vx>")
		       (match_operand:VECA 2 "register_operand" "<vx>")))
   (clobber (match_scratch:VECA 3 "=&<ax>"))]
  ""
  "vasrv<unit_suffix>.%L0 %v0,%v1,%v2")

;; ??? vblsr also allows scalar registers
;; ??? should show vblsr / vasr also as alterantives for
;; lshr<mode>3 / ashr<mode>3 with appropriately restricted constant range.
;; (using vec_duplicate for operand predicate with scalar register).
(define_insn "vec_shr_<mode>"
  [(set (match_operand:VECA 0 "register_operand" "=<vx>")
        (lshiftrt:VECA (match_operand:VECA 1 "register_operand" "<vx>")
		       (match_operand:HI 2 "immediate_operand" "i<c4>i")))
   (clobber (match_scratch:VECA 3 "=&<ax>"))]
  ""
  "vblsr<unit_suffix>.%L0 %v0,%v1,%2")

(define_insn "ashr<mode>3_scalar"
  [(set (match_operand:VECA 0 "register_operand" "=<vx>")
        (ashiftrt:VECA (match_operand:VECA 1 "register_operand" "<vx>")
		       (match_operand:HI 2 "nonmemory_operand" "S_ni<c4>S_ni")))
   (clobber (match_scratch:VECA 3 "=&<ax>"))]
  ""
  "vasr<unit_suffix>.%L0 %v0,%v1,%2")

(define_expand "mul<mode>3"
  [(parallel [(set (match_operand:VECA 0 "register_operand" "")
		   (mult:VECA (match_operand:VECA 1 "register_operand")
			      (match_operand:VECA 2 "nonmemory_operand")))
	      (clobber (scratch:VECA))])]
  ""
  "
{
  if (GET_MODE_UNIT_SIZE (<MODE>mode) > 16)
    operands[2] = force_reg (<MODE>mode, operands[2]);
}")

(define_insn "*mul<mode>_i"
  [(set (match_operand:VXHI 0 "register_operand" "=<v0>,<v4>,<v0>,<v4>")
	(mult:VXHI (match_operand:VXHI 1 "register_operand"
		    "%<v0>,<v4>,<v0>,<v4>")
		   (match_operand:VXHI 2 "nonmemory_operand"
		    "<v0>,<v4>,S_n<I16v>,S_n<I16v>")))
   (clobber (match_scratch:VXHI 3 "=&<a0>,&<a4>,&<a0>,&<a4>"))]
  ""
  "@
   vmulw.%L0 %v0,%v1,%v2
   vmulw.%L0 %v0,%v1,%v2
   vmulw.%L0 %v0,%v1,%2
   vmulw.%L0 %v0,%v1,%2")

;; ??? should be a define_insn_and_split
(define_insn "*mul<mode>_i"
  [(set (match_operand:VXSI 0 "register_operand" "=&<v0>,&<v4>")
	(mult:VXSI (match_operand:VXSI 1 "register_operand" "%<v0>0,<v4>0")
		   (match_operand:VXSI 2 "nonmemory_operand"
		    "<v0>I32v,<v4>I32v")))
   (clobber (match_scratch:VXSI 3 "=&<a0>,&<a4>"))]
  ""
  "*
{
  output_asm_insn (register_operand (operands[2], <MODE>mode)
		   ? \"vmov.%L0 %v0,0x8000` vaddna.%L0 %v0,%v0,%v2` vmvw.%O0 %v0,vr62` vmule.%L0 vr62,%v1,%v2\"
		   : \"vmov.%L0 %v0,%2 + 0x8000 & 0xffff0000` vbmule.%L0 vr62,%v1,%2\",
		   operands);
  return \"vmulae.%L0 %v0,%v0,%v1\";
}"
  [(set_attr "length" "3")])

;; ??? should be a define_insn_and_split
;; ??? FIXME: check effect of sign extension in vmul*
(define_insn "muldi3"
  [(set (match_operand:DI 0 "register_operand" "=r,r,r")
	(mult:DI (match_operand:DI 1 "register_operand" "%r,r,v")
		   (match_operand:DI 2 "register_operand" "r,v,v")))
   (clobber (match_scratch:TI 3 "=&v,2,2"))
   (clobber (match_scratch:TI 4 "=&v,&v,&v"))
   (clobber (match_scratch:TI 5 "=&v,&v,1"))
   (clobber (match_scratch:TI 6 "=&Rac,&Rac,&Rac"))]
  ""
  "vup %3,%2
   vmrgw %4,%1,%1
   
   vmule.3 %0,%3,%1 ; a0b0
   vswp %5,%3
   
   vmr5w.16 %3,%0,%0
   vmule.15 %3,%5,%4 ; b0a1|a1b0
   vmr4w %5,%5,%5
   vmr7w %4,%4,vr62
   vxsumi.192 %3,%3,21
   
   
   vblsr vr62,%3,16
   vmulae %5,%4,%5
   
   
   vxsumwi.2 %0,%3,21
   vxsumi.12 %0,%5,255"
  [(set_attr "length" "13")])

;; ??? This should be a define_insn_and_split
(define_insn "extendsidi2"
  [(set (match_operand:DI 0 "register_operand" "=R0f,Rf0")
	(sign_extend:DI (match_operand:SI 1 "register_operand" "0,0")))
   (clobber (match_scratch:DI 2 "=Ral,Rah"))]
  ""
  "vswpe.%P0 %v0,%v1` vlt.%P0 %v0,%v0,vr62"
  [(set_attr "length" "2")])

(define_insn "vec_unpacks_lo_v8hi"
  [(set (match_operand:V4SI 0 "register_operand" "=v")
	(sign_extend:V4SI
	  (vec_select:V4HI (match_operand:V8HI 1 "register_operand" "v")
			   (parallel [(const_int 0) (const_int 1)
				      (const_int 2) (const_int 3)]))))
   (clobber:TI (match_scratch:TI 2 "=Rac"))]
  ""
  "vups %0,%1")

(define_insn "vec_unpacku_lo_v8hi"
  [(set (match_operand:V4SI 0 "register_operand" "=v")
	(zero_extend:V4SI
	  (vec_select:V4HI (match_operand:V8HI 1 "register_operand" "v")
			   (parallel [(const_int 0) (const_int 1)
				      (const_int 2) (const_int 3)]))))
   (clobber:TI (match_scratch:TI 2 "=Rac"))]
  ""
  "vup %0,%1")

;; ??? This should be a define_insn_and_split.
(define_insn "vec_unpacks_hi_v8hi"
  [(set (match_operand:V4SI 0 "register_operand" "=v")
	(sign_extend:V4SI
	  (vec_select:V4HI (match_operand:V8HI 1 "register_operand" "v")
			   (parallel [(const_int 4) (const_int 5)
				      (const_int 6) (const_int 7)]))))
   (clobber:TI (match_scratch:TI 2 "=Rac"))]
  ""
  "vmr4w %0,%1,%1\;vups %0,%0"
  [(set_attr "length" "2")])

;; ??? This should be a define_insn_and_split.
(define_insn "vec_unpacku_hi_v8hi"
  [(set (match_operand:V4SI 0 "register_operand" "=v")
	(zero_extend:V4SI
	  (vec_select:V4HI (match_operand:V8HI 1 "register_operand" "v")
			   (parallel [(const_int 4) (const_int 5)
				      (const_int 6) (const_int 7)]))))
   (clobber:TI (match_scratch:TI 2 "=Rac"))]
  ""
  "vmr4w %0,%1,%1\;vup %0,%0"
  [(set_attr "length" "2")])

;; ??? This should be a define_insn_and_split.
;; ??? usually, we will have undefined behaviour for overflow, but
;; this expander can't tell and thus has to mask the input values.
(define_insn "vec_pack_trunc_v4si"
  [(set (match_operand:V8HI 0 "register_operand" "=&v")
	(vec_concat:V8HI
	  (truncate:V4HI (match_operand:V4SI 1 "register_operand" "v"))
	  (truncate:V4HI (match_operand:V4SI 2 "register_operand" "v"))))
   (clobber:TI (match_scratch:TI 3 "=&Rac"))
   (clobber:TI (match_scratch:TI 4 "=&v"))]
  ""
  "vmvzw %4,%1,85\;vasrpi %0,%4,0\;vmvzw %4,%2,85\;vasrpi %4,%4,0\;\
vmr4w.240 %0,%4,%4")

(define_insn "jump"
  [(set (pc) (label_ref (match_operand 0 "" "")))]
  ""
  "vjp %r0%#"
  [(set_attr "type" "jump")])

;; ??? manual says vjpi, but assempler only accepts vjp.
(define_insn "indirect_jump"
  [(set (pc) (match_operand:HI 0 "register_operand" "S_n"))]
  ""
  "vjp %0%#"
  [(set_attr "type" "jump")])

(define_expand "call"
  [(match_operand 0 "" "") (match_operand 1 "" "")]
  ""
  "emit_call_insn (gen_call_i (XEXP (operands[0], 0), operands[1])); DONE;")

(define_insn "call_i"
  [(call (mem:QI (match_operand:HI 0 "nonmemory_operand" "i,S_n"))
	 (match_operand 1 "" ""))
   (clobber (reg:TI CALL_STACK_REG))]
  ""
  "@
   vjl vr31,%0%#
   vjli vr31,%0%#"
  [(set_attr "type" "jump")])

(define_expand "call_value"
  [(match_operand 0 "" "") (match_operand 1 "" "") (match_operand 2 "" "")]
  ""
  "
{
  emit_call_insn (gen_call_value_i (operands[0],
				    XEXP (operands[1], 0), operands[2]));
  DONE;
}")

(define_insn "call_value_i"
  [(set (match_operand 0 "register_operand")
	(call (mem:QI (match_operand:HI 1 "nonmemory_operand" "i,S_n"))
	      (match_operand 2 "" "")))
   (clobber (reg:TI CALL_STACK_REG))]
  ""
  "@
   vjl vr31,%1%#
   vjli vr31,%1%#"
  [(set_attr "type" "jump")])

(define_expand "prologue"
  [(const_int 0)]
  ""
  "mxp_expand_prologue (); DONE;")

;; We dont really have a use for the arc core register parameter.  We use
;; pcl here because there is no latency attached to it on the ARC700 core.
(define_insn "return"
  [(return)]
  ""
  "vjb vr31,pcl%#"
  [(set_attr "type" "jump")])

(define_expand "epilogue"
  [(return)]
  ""
  "mxp_expand_epilogue ();")

(define_insn "*store_scalars"
  [(set (match_operand:V4SI 0 "memory_operand" "=m")
	(match_operand:V4SI 1 "scalar_save_operand" ""))]
  ""
  "vst128 %v1,%0"
  [(set_attr "type" "store")])

(define_insn "*load_scalars"
  [(set (match_operand:V4SI 0 "scalar_save_operand" "")
	(match_operand:V4SI 1 "memory_operand" "m"))]
  ""
  "vld128 %v0,%1"
  [(set_attr "type" "load")])

(define_expand "cmp<mode>"
  [(set (reg:CC VFLAGS_REG)
	(compare:CC (match_operand:VECA 0 "reg_or_0_operand" "")
		    (match_operand:VECA 1 "nonmemory_operand" "")))]
  ""
  "
{
  mxp_compare_op0 = operands[0];
  mxp_compare_op1 = operands[1];
  DONE;
}")

(define_code_iterator any_cond [;;unordered ordered unlt unge uneq ltgt unle ungt
			     eq ne gt ge lt le gtu geu ltu leu])

(define_expand "b<any_cond:code>"
  [(set (pc)
	(if_then_else (any_cond:CC (reg:CC VFLAGS_REG)
                                   (const_int 0))
                      (label_ref (match_operand 0 ""))
                      (pc)))]
  ""
  "
{
  mxp_emit_conditional_branch (operands, <CODE>);
  DONE;
}")

(define_code_iterator vec_cond [eq ne le lt])

(define_insn "*cmp<code><mode>"
  [(set (match_operand:HI 0 "register_operand" "=<vx>")
	(neg:HI (vec_cond:HI (match_operand:HSI 1 "reg_or_0_operand" "<vx>")
			     (match_operand:HSI 2 "reg_or_0_operand" "<vx>"))))
   (clobber (match_scratch:HSI 3 "=<ax>"))]
  ""
  "v<code><unit_suffix>.%L0 %v0,%v1,%v2")

(define_code_iterator vec_rcond [ge gt])
(define_code_attr rcode [(gt "lt") (ge "le")])

(define_insn "*cmp<code><mode>"
  [(set (match_operand:HI 0 "register_operand" "=<vx>")
	(neg:HI (vec_rcond:HI (match_operand:HSI 1 "reg_or_0_operand" "<vx>")
			      (match_operand:HSI 2 "reg_or_0_operand" "<vx>"))))
   (clobber (match_scratch:HSI 3 "=<ax>"))]
  ""
  "v<rcode><unit_suffix>.%L0 %v0,%v2,%v1")

(define_insn "ucmphi"
  [(set (match_operand:CCI 0 "register_operand" "=Rfl,Rfh,Rfl,Rfh")
	(compare:CCI (match_operand:HI 1 "reg_or_0_operand"
		      "R01Z,R10Z,R01Z,R10Z")
		     (match_operand:HI 2 "nonmemory_operand"
		      "R01Z,R10Z,i,i")))
   (clobber (match_scratch:HI 3 "=Ral,Rah,Ral,Rah"))]
  ""
  "@
   vsubw.%L0.f vr62,%v1,%v2
   vsubw.%L0.f vr62,%v1,%v2
   vsubw.%L0.f vr62,%v1,%r2
   vsubw.%L0.f vr62,%v1,%r2")

(define_insn "ucmpsi"
  [(set (match_operand:CCI 0 "register_operand" "=Rfl,Rfh,Rfl,Rfh,Rfl,Rfh")
	(compare:CCI (match_operand:SI 1 "reg_or_0_operand"
		      "R03Z,R30Z,R03Z,R30Z,S_n,S_n")
		     (match_operand:SI 2 "reg_or_0_operand"
		      "R03Z,R30Z,S_n,S_n,R03Z,R30Z")))
   (clobber (match_scratch:SI 3 "=Ral,Rah,Ral,Rah,Ral,Rah"))]
  ""
  "@
   vsube.%L0.f vr62,%v1,%v2
   vsube.%L0.f vr62,%v1,%v2
   vksub.%L0.f vr62,%v1,%2
   vksub.%L0.f vr62,%v1,%2
   vkrsub.%L0.f vr62,%v2,%1
   vkrsub.%L0.f vr62,%v2,%1")

(define_insn "vtany"
  [(set (match_operand:HI 0 "register_operand" "=S_n")
	(neg:HI (match_operator:HI 2 "comparison_operator"
		  [(match_operand:CCI 1 "register_operand" "Rfg")
		   (const_int 0)])))]
  ""
  "vtany.%L1%C2 %0,0xffff")

(define_insn "branch_true"
  [(set (pc)
   (if_then_else (ne (match_operand:HI 1 "register_operand" "S_n")
		     (const_int 0))
		 (label_ref (match_operand 0 "" "")) (pc)))]
  ""
  "vjp.%1 %r0%#"
  [(set_attr "type" "jump")])

(define_insn "branch_false"
  [(set (pc)
   (if_then_else (ne (match_operand:HI 1 "register_operand" "S_n")
		     (const_int 65535))
		 (label_ref (match_operand 0 "" "")) (pc)))]
  ""
  "vjp.!%1 %r0%#"
  [(set_attr "type" "jump")])

; FIXME: handle reloads of operands 0 / 2.
; ??? This doesn't seem to ever get generated.
(define_insn "decrement_and_branch_until_zero"
  [(set (pc)
   (if_then_else (ne (match_operand:HI 0 "register_operand" "S_n")
		     (const_int 0))
		 (label_ref (match_operand 1 "" "")) (pc)))
   (set (match_operand:HI 2 "register_operand" "+S_n")
	(plus (match_dup 2) (const_int -1)))]
  ""
  "vjd.%0 %2,%r1"
  [(set_attr "type" "jump")])

; operand 0 is the loop count pseudo register
; operand 1 is the number of loop iterations or 0 if it is unknown
; operand 2 is the maximum number of loop iterations
; operand 3 is the number of levels of enclosed loops
; operand 4 is the label to jump to at the top of the loop
(define_expand "doloop_end"
  [(use (match_operand 0 "register_operand" ""))
   (use (match_operand:QI 1 "const_int_operand" ""))
   (use (match_operand:QI 2 "const_int_operand" ""))
   (use (match_operand:QI 3 "const_int_operand" ""))
   (use (label_ref (match_operand 4 "" "")))]
  ""
{
  emit_jump_insn (gen_decrement_and_branch_until_zero (operands[0], operands[4],
						       operands[0]));
  DONE;
})

(define_insn "smax<mode>3"
  [(set (match_operand:VECA 0 "register_operand" "=<vx>")
	(smax:VECA (match_operand:VECA 1 "register_operand" "<vx>")
		  (match_operand:VECA 2 "register_operand" "<vx>")))
   (clobber (match_scratch:VECA 3 "=&<ax>"))]
  ""
  "vmax<unit_suffix>.%L0 %v0,%v1,%v2")

(define_insn "smin<mode>3"
  [(set (match_operand:VECA 0 "register_operand" "=<vx>")
	(smin:VECA (match_operand:VECA 1 "register_operand" "<vx>")
		  (match_operand:VECA 2 "register_operand" "<vx>")))
   (clobber (match_scratch:VECA 3 "=&<ax>"))]
  ""
  "vmin<unit_suffix>.%L0 %v0,%v1,%v2")

(define_automaton "mxp")

(define_cpu_unit "issue" "mxp")

(define_insn_reservation "int_pipe" 2
  (eq_attr "type" "other")
  "issue")

(define_insn_reservation "load" 4
  (eq_attr "type" "load")
  "issue")

;; FIXME: add sibcalls.
;; FIXME: add conditional execution

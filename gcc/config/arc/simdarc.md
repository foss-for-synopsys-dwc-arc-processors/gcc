(define_c_enum "unspec" [
  UNSPEC_ARCV2_DMACH
  UNSPEC_ARCV2_DMACHU
  UNSPEC_ARCV2_DMPYH
  UNSPEC_ARCV2_DMPYHU
  UNSPEC_ARCV2_QMACH
  UNSPEC_ARCV2_QMACHU
  UNSPEC_ARCV2_VMAC2H
])

;;64-bit vectors of halwords and words
(define_mode_iterator VWH [V4HI V2SI])

;;double element vectors
(define_mode_iterator VDV [V2HI V2SI])
(define_mode_attr V_addsub [(V2HI "HI") (V2SI "SI")])
(define_mode_attr V_addsub_suffix [(V2HI "2h") (V2SI "")])

;;all vectors
(define_mode_iterator VCT [V2HI V4HI V2SI])
(define_mode_attr V_suffix [(V2HI "2h") (V4HI "4h") (V2SI "2")])

;; Widening operations.
(define_code_iterator SE [sign_extend zero_extend])
(define_code_attr V_US [(sign_extend "s") (zero_extend "u")])
(define_code_attr V_US_suffix [(sign_extend "") (zero_extend "u")])


;; Move patterns
(define_expand "movv2hi"
  [(set (match_operand:V2HI 0 "move_dest_operand" "")
	(match_operand:V2HI 1 "general_operand" ""))]
  ""
  "{
  if (GET_CODE (operands[0]) == MEM)
    operands[1] = force_reg (V2HImode, operands[1]);
   }")

(define_insn_and_split "*movv2hi_insn"
  [(set (match_operand:V2HI 0 "nonimmediate_operand" "=r,r,r,m")
	(match_operand:V2HI 1 "general_operand"       "i,r,m,r"))]
  "GET_CODE (operands[0]) != MEM || GET_CODE (operands[1]) == REG"
  "@
   #
   mov%? %0, %1
   ld%U1%V1 %0,%1
   st%U0%V0 %1,%0"
  "reload_completed && GET_CODE (operands[1]) == CONST_VECTOR"
  [(set (match_dup 0) (match_dup 2))]
  {
   HOST_WIDE_INT intval = INTVAL (XVECEXP (operands[1], 0, 1)) << 16;
   intval |= INTVAL (XVECEXP (operands[1], 0, 0)) & 0xFFFF;

   operands[0] = gen_rtx_REG (SImode, REGNO (operands[0]));
   operands[2] = GEN_INT (trunc_int_for_mode (intval, SImode));
   }
  [(set_attr "type" "move,move,load,store")
   (set_attr "predicable" "yes,yes,no,no")
   (set_attr "iscompact"  "false,false,false,false")
   ])

(define_expand "movmisalignv2hi"
 [(set (match_operand:V2HI 0 "general_operand" "")
       (match_operand:V2HI 1 "general_operand" ""))]
 ""
{
 if (!register_operand (operands[0], V2HImode)
      && !register_operand (operands[1], V2HImode))
    operands[1] = force_reg (V2HImode, operands[1]);
})

(define_expand "mov<mode>"
  [(set (match_operand:VWH 0 "move_dest_operand" "")
	(match_operand:VWH 1 "general_operand" ""))]
  ""
  "{
  if (GET_CODE (operands[0]) == MEM)
    operands[1] = force_reg (<MODE>mode, operands[1]);
   }")

(define_insn_and_split "*mov<mode>_insn"
  [(set (match_operand:VWH 0 "nonimmediate_operand" "=r,r,r,m")
	(match_operand:VWH 1 "general_operand"       "i,r,m,r"))]
  "GET_CODE (operands[0]) != MEM || GET_CODE (operands[1]) == REG"
  "@
   #
   vadd2 %0, %1, 0
   ldd%U1%V1 %0,%1
   std%U0%V0 %1,%0"
  "reload_completed && GET_CODE (operands[1]) == CONST_VECTOR"
  [(set (match_dup 0) (match_dup 2))
   (set (match_dup 3) (match_dup 4))]
  {
   HOST_WIDE_INT intval0, intval1;
   if (GET_MODE (operands[1]) == V2SImode)
     {
      intval0 = INTVAL (XVECEXP (operands[1], 0, 0));
      intval1 = INTVAL (XVECEXP (operands[1], 0, 1));
     }
    else
     {
      intval1 = INTVAL (XVECEXP (operands[1], 0, 3)) << 16;
      intval1 |= INTVAL (XVECEXP (operands[1], 0, 2)) & 0xFFFF;
      intval0 = INTVAL (XVECEXP (operands[1], 0, 1)) << 16;
      intval0 |= INTVAL (XVECEXP (operands[1], 0, 0)) & 0xFFFF;
     }
   operands[0] = gen_rtx_REG (SImode, REGNO (operands[0]));
   operands[3] = gen_rtx_REG (SImode, REGNO (operands[0])+1);
   operands[2] = GEN_INT (trunc_int_for_mode (intval0, SImode));
   operands[4] = GEN_INT (trunc_int_for_mode (intval1, SImode));
   }
  [(set_attr "type" "move,move,load,store")
   (set_attr "predicable" "yes,no,no,no")
   (set_attr "iscompact"  "false,false,false,false")
   ])

(define_expand "movmisalign<mode>"
 [(set (match_operand:VWH 0 "general_operand" "")
       (match_operand:VWH 1 "general_operand" ""))]
 ""
{
 if (!register_operand (operands[0], <MODE>mode)
      && !register_operand (operands[1], <MODE>mode))
    operands[1] = force_reg (<MODE>mode, operands[1]);
})


;; Simple arithmetic insns
(define_insn "add<mode>3"
  [(set (match_operand:VCT 0 "register_operand"          "=r,r")
	(plus:VCT (match_operand:VCT 1 "register_operand" "0,r")
		  (match_operand:VCT 2 "register_operand" "r,r")))]
  "TARGET_V2"
  "vadd<V_suffix>%? %0, %1, %2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "sub<mode>3"
  [(set (match_operand:VCT 0 "register_operand"           "=r,r")
	(minus:VCT (match_operand:VCT 1 "register_operand" "0,r")
		   (match_operand:VCT 2 "register_operand" "r,r")))]
  "TARGET_V2"
  "vsub<V_suffix>%? %0, %1, %2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

;; Combined arithmetic ops
(define_insn "addsub<mode>3"
  [(set (match_operand:VDV 0 "register_operand" "=r,r")
	(vec_concat:VDV
	 (plus:<V_addsub> (vec_select:<V_addsub> (match_operand:VDV 1 "register_operand" "0,r")
						 (parallel [(const_int 0)]))
			  (vec_select:<V_addsub> (match_operand:VDV 2 "register_operand" "r,r")
						 (parallel [(const_int 0)])))
	 (minus:<V_addsub> (vec_select:<V_addsub> (match_dup 1) (parallel [(const_int 1)]))
			   (vec_select:<V_addsub> (match_dup 2) (parallel [(const_int 1)])))))]
  "TARGET_V2"
  "vaddsub<V_addsub_suffix>%? %0, %1, %2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "subadd<mode>3"
  [(set (match_operand:VDV 0 "register_operand" "=r,r")
	(vec_concat:VDV
	 (minus:<V_addsub> (vec_select:<V_addsub> (match_operand:VDV 1 "register_operand" "0,r")
						  (parallel [(const_int 0)]))
			   (vec_select:<V_addsub> (match_operand:VDV 2 "register_operand" "r,r")
						  (parallel [(const_int 0)])))
	 (plus:<V_addsub> (vec_select:<V_addsub> (match_dup 1) (parallel [(const_int 1)]))
			  (vec_select:<V_addsub> (match_dup 2) (parallel [(const_int 1)])))))]
  "TARGET_V2"
  "vsubadd<V_addsub_suffix>%? %0, %1, %2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

;; Multiplication
(define_insn "dmach"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(unspec:SI [(match_operand:V2HI 1 "register_operand" "0,r")
		    (match_operand:V2HI 2 "register_operand" "r,r")
		    (match_operand:DI 3 "mlo_operand" "")]
		   UNSPEC_ARCV2_DMACH))
   (clobber (match_dup 3))]
  "TARGET_V2"
  "dmach%? %0, %1, %2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "dmachu"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(unspec:SI [(match_operand:V2HI 1 "register_operand" "0,r")
		    (match_operand:V2HI 2 "register_operand" "r,r")
		    (match_operand:DI 3 "mlo_operand" "")]
		   UNSPEC_ARCV2_DMACHU))
   (clobber (match_dup 3))]
  "TARGET_V2"
  "dmachu%? %0, %1, %2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "dmpyh"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(unspec:SI [(match_operand:V2HI 1 "register_operand" "0,r")
		    (match_operand:V2HI 2 "register_operand" "r,r")]
		   UNSPEC_ARCV2_DMPYH))
   (clobber (match_operand:DI 3 "mlo_operand" ""))]
  "TARGET_V2"
  "dmpyh%? %0, %1, %2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "dmpyhu"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(unspec:SI [(match_operand:V2HI 1 "register_operand" "0,r")
		    (match_operand:V2HI 2 "register_operand" "r,r")]
		   UNSPEC_ARCV2_DMPYHU))
   (clobber (match_operand:DI 3 "mlo_operand" ""))]
  "TARGET_V2"
  "dmpyhu%? %0, %1, %2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_expand "sdot_prodv2hi"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:V2HI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:SI 3 "register_operand" "")]
  "TARGET_V2"
{
  rtx t = gen_rtx_REG (SImode, TARGET_BIG_ENDIAN ? 59 : 58);
  emit_move_insn (t, operands[3]);
  emit_insn (gen_dmach (operands[0], operands[1], operands[2], t));
  DONE;
})

(define_expand "udot_prodv2hi"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:V2HI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:SI 3 "register_operand" "")]
  "TARGET_V2"
{
  rtx t = gen_rtx_REG (SImode, TARGET_BIG_ENDIAN ? 59 : 58);
  emit_move_insn (t, operands[3]);
  emit_insn (gen_dmachu (operands[0], operands[1], operands[2], t));
  DONE;
})

(define_insn "qmach"
  [(set (match_operand:DI 0 "register_operand" "=r,r")
	(unspec:DI [(match_operand:V4HI 1 "register_operand" "0,r")
		      (match_operand:V4HI 2 "register_operand" "r,r")
		      (match_operand:DI 3 "mlo_operand" "")]
		     UNSPEC_ARCV2_QMACH))
   (clobber (match_dup 3))]
  "TARGET_V2"
  "qmach%? %0, %1, %2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "qmachu"
  [(set (match_operand:DI 0 "register_operand" "=r,r")
	(unspec:DI [(match_operand:V4HI 1 "register_operand" "0,r")
		    (match_operand:V4HI 2 "register_operand" "r,r")
		    (match_operand:DI 3 "mlo_operand" "")]
		   UNSPEC_ARCV2_QMACHU))
   (clobber (match_dup 3))]
  "TARGET_V2"
  "qmachu%? %0, %1, %2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "arc_vec_<V_US>mult_lo_v4hi"
 [(set (match_operand:V2SI 0 "nonmemory_operand"                     "=r,r")
       (mult:V2SI (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 1 "register_operand" "0,r")
			    (parallel [(const_int 0) (const_int 1)])))
		  (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 2 "register_operand" "r,r")
			    (parallel [(const_int 0) (const_int 1)])))))
  (set (match_operand:V2SI 3 "mlo_operand" "")
       (mult:V2SI (SE:V2SI (vec_select:V2HI (match_dup 1)
					    (parallel [(const_int 0) (const_int 1)])))
		  (SE:V2SI (vec_select:V2HI (match_dup 2)
					    (parallel [(const_int 0) (const_int 1)])))))
  ]
  "TARGET_V2"
  "vmpy2h<V_US_suffix>%? %0, %1, %2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "arc_vec_<V_US>multacc_lo_v4hi"
  [(set (match_operand:V2SI 0 "mlo_operand" "")
	(mult:V2SI (SE:V2SI (vec_select:V2HI
			     (match_operand:V4HI 1 "register_operand" "0,r")
			     (parallel [(const_int 0) (const_int 1)])))
		   (SE:V2SI (vec_select:V2HI
			     (match_operand:V4HI 2 "register_operand" "r,r")
			     (parallel [(const_int 0) (const_int 1)])))))
  ]
  "TARGET_V2"
  "vmpy2h<V_US_suffix>%? 0, %1, %2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_expand "vec_widen_<V_US>mult_lo_v4hi"
 [(set (match_operand:V2SI 0 "register_operand"                      "")
       (mult:V2SI (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 1 "register_operand" "")
			    (parallel [(const_int 0) (const_int 1)])))
		  (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 2 "register_operand"   "")
			    (parallel [(const_int 0) (const_int 1)])))))]
  "TARGET_V2"
  {
     rtx acc = gen_rtx_REG (V2SImode, TARGET_BIG_ENDIAN ? 59 : 58);
     emit_insn (gen_arc_vec_<V_US>mult_lo_v4hi (operands[0],
						operands[1],
						operands[2],
						acc));
     DONE;
  }
)

(define_insn "arc_vec_<V_US>mult_hi_v4hi"
 [(set (match_operand:V2SI 0 "register_operand"                     "=r,r")
       (mult:V2SI (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 1 "register_operand" "0,r")
			    (parallel [(const_int 2) (const_int 3)])))
		  (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 2 "register_operand" "r,r")
			    (parallel [(const_int 2) (const_int 3)])))))
  (set (match_operand:V2SI 3 "mlo_operand" "")
       (mult:V2SI (SE:V2SI (vec_select:V2HI (match_dup 1)
					    (parallel [(const_int 2) (const_int 3)])))
		  (SE:V2SI (vec_select:V2HI (match_dup 2)
					    (parallel [(const_int 2) (const_int 3)])))))
  ]
  "TARGET_V2"
  "vmpy2h<V_US_suffix>%? %0, %R1, %R2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_expand "vec_widen_<V_US>mult_hi_v4hi"
 [(set (match_operand:V2SI 0 "register_operand"                               "")
       (mult:V2SI (SE:V2SI (vec_select:V2HI
				     (match_operand:V4HI 1 "register_operand" "")
				     (parallel [(const_int 2) (const_int 3)])))
		  (SE:V2SI (vec_select:V2HI
				     (match_operand:V4HI 2 "register_operand" "")
				     (parallel [(const_int 2) (const_int 3)])))))]
  "TARGET_V2"
  {
     rtx acc = gen_rtx_REG (V2SImode, TARGET_BIG_ENDIAN ? 59 : 58);
     emit_insn (gen_arc_vec_<V_US>mult_hi_v4hi (operands[0],
						operands[1],
						operands[2],
						acc));
     DONE;
  }
)

(define_insn "arc_vec_<V_US>mac_hi_v4hi"
 [(set (match_operand:V2SI 0 "register_operand"                     "=r,r")
       (plus:V2SI
	(match_operand:V2SI 1 "mlo_operand"                          "")
	(mult:V2SI (SE:V2SI (vec_select:V2HI
			     (match_operand:V4HI 2 "register_operand" "0,r")
			     (parallel [(const_int 2) (const_int 3)])))
		   (SE:V2SI (vec_select:V2HI
			     (match_operand:V4HI 3 "register_operand" "r,r")
			     (parallel [(const_int 2) (const_int 3)]))))))
  (set (match_dup 1)
       (plus:V2SI
	(match_dup 1)
	(mult:V2SI (SE:V2SI (vec_select:V2HI (match_dup 2)
					     (parallel [(const_int 2) (const_int 3)])))
		   (SE:V2SI (vec_select:V2HI (match_dup 3)
					     (parallel [(const_int 2) (const_int 3)]))))))
  ]
  "TARGET_V2"
  "vmac2h<V_US_suffix>%? %0, %R2, %R3"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

;czi;(define_expand "sdot_prodv4hi"
;czi;  [(match_operand:V2SI 0 "register_operand" "")
;czi;   (match_operand:V4HI 1 "register_operand" "")
;czi;   (match_operand:V4HI 2 "register_operand" "")
;czi;   (match_operand:V2SI 3 "register_operand" "")]
;czi;  "TARGET_HS"
;czi;{
;czi; rtx acc = gen_rtx_REG (V2SImode, TARGET_BIG_ENDIAN ? 59 : 58);
;czi; rtx t = gen_reg_rtx (V2SImode);
;czi; emit_insn (gen_arc_vec_smultacc_lo_v4hi (acc,
;czi;				      operands[1],
;czi;				      operands[2]));
;czi; emit_insn (gen_arc_vec_smac_hi_v4hi (t,
;czi;				     acc,
;czi;				     operands[1],
;czi;				     operands[2]));
;czi; emit_insn (gen_addv2si3 (operands[0],
;czi;			  operands[3],
;czi;			  t));
;czi; DONE;
;czi;})
;czi;
;czi;(define_expand "udot_prodv4hi"
;czi;  [(match_operand:V2SI 0 "register_operand" "")
;czi;   (match_operand:V4HI 1 "register_operand" "")
;czi;   (match_operand:V4HI 2 "register_operand" "")
;czi;   (match_operand:V2SI 3 "register_operand" "")]
;czi;  "TARGET_HS"
;czi;{
;czi; rtx acc = gen_rtx_REG (V2SImode, TARGET_BIG_ENDIAN ? 59 : 58);
;czi; rtx t = gen_reg_rtx (V2SImode);
;czi; emit_insn (gen_arc_vec_umultacc_lo_v4hi (acc,
;czi;				      operands[1],
;czi;				      operands[2]));
;czi; emit_insn (gen_arc_vec_umac_hi_v4hi (t,
;czi;				     acc,
;czi;				     operands[1],
;czi;				     operands[2]));
;czi; emit_insn (gen_addv2si3 (operands[0],
;czi;			  operands[3],
;czi;			  t));
;czi; DONE;
;czi;})

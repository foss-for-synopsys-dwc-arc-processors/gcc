;; ::::::::::::::::::::
;; ::
;; :: 32-bit floating point arithmetic
;; ::
;; ::::::::::::::::::::

;; Addition
(define_insn "*addsf3_fpu"
  [(set (match_operand:SF 0 "register_operand"          "=r,r,r,r,r,  r,  r,  r")
	(plus:SF (match_operand:SF 1 "nonmemory_operand" "0,r,0,r,0,  0,  r,Cal")
		 (match_operand:SF 2 "nonmemory_operand" "r,r,L,L,I,Cal,Cal,  r")))]
  "TARGET_FP_SINGLE"
  "fsadd%? %0,%1,%2"
  [(set_attr "length" "4,4,4,4,4,8,8,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,yes,no,no,yes,no,no")
   (set_attr "cond" "canuse,nocond,canuse,nocond,nocond,canuse,nocond,nocond")
   ])

;; Subtraction
(define_insn "*subsf3_fpu"
  [(set (match_operand:SF 0 "register_operand"           "=r,r,r,r,r,  r,  r,  r")
	(minus:SF (match_operand:SF 1 "nonmemory_operand" "0,r,0,r,0,  0,  r,Cal")
		  (match_operand:SF 2 "nonmemory_operand" "r,r,L,L,I,Cal,Cal,  r")))]
  "TARGET_FP_SINGLE"
  "fssub%? %0,%1,%2"
  [(set_attr "length" "4,4,4,4,4,8,8,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,yes,no,no,yes,no,no")
   (set_attr "cond" "canuse,nocond,canuse,nocond,nocond,canuse,nocond,nocond")
   ])

;; Multiplication
(define_insn "*mulsf3_fpu"
  [(set (match_operand:SF 0 "register_operand"          "=r,r,r,r,r,  r,  r,  r")
	(mult:SF (match_operand:SF 1 "nonmemory_operand" "0,r,0,r,0,  0,  r,Cal")
		 (match_operand:SF 2 "nonmemory_operand" "r,r,L,L,I,Cal,Cal,  r")))]
  "TARGET_FP_SINGLE"
  "fsmul%? %0,%1,%2"
  [(set_attr "length" "4,4,4,4,4,8,8,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,yes,no,no,yes,no,no")
   (set_attr "cond" "canuse,nocond,canuse,nocond,nocond,canuse,nocond,nocond")
   ])

;; Multiplication with addition/subtraction
(define_expand "fmasf4"
  [(set (match_operand:SF 0 "register_operand" "")
	(fma:SF (match_operand:SF 1 "nonmemory_operand" "")
		(match_operand:SF 2 "nonmemory_operand" "")
		(match_operand:SF 3 "nonmemory_operand" "")))]
  "TARGET_FP_SFUZED"
  "{
   rtx tmp;
   tmp = gen_rtx_REG (SFmode, TARGET_BIG_ENDIAN ? 59 : 58);
   emit_move_insn (tmp, operands[3]);
   operands[3] = tmp;
   }")

(define_expand "fnmasf4"
  [(set (match_operand:SF 0 "register_operand" "")
	(fma:SF (neg:SF (match_operand:SF 1 "nonmemory_operand" ""))
		(match_operand:SF 2 "nonmemory_operand"         "")
		(match_operand:SF 3 "nonmemory_operand"         "")))]
  "TARGET_FP_SFUZED"
  "{
   rtx tmp;
   tmp = gen_rtx_REG (SFmode, TARGET_BIG_ENDIAN ? 59 : 58);
   emit_move_insn (tmp, operands[3]);
   operands[3] = tmp;
}")

(define_insn "fmasf4_fpu"
  [(set (match_operand:SF 0 "register_operand"         "=r,r,  r,  r,  r")
	(fma:SF (match_operand:SF 1 "nonmemory_operand" "0,r,  0,  r,Cal")
		(match_operand:SF 2 "nonmemory_operand" "r,r,Cal,Cal,  r")
		(match_operand:SF 3 "mlo_operand" "")))]
  "TARGET_FP_SFUZED"
  "fsmadd%? %0,%1,%2"
  [(set_attr "length" "4,4,8,8,8")
   (set_attr "predicable" "yes,no,yes,no,no")
   (set_attr "cond" "canuse,nocond,canuse_limm,nocond,nocond")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")])

(define_insn "fnmasf4_fpu"
  [(set (match_operand:SF 0 "register_operand"                 "=r,r,  r,  r,  r")
	(fma:SF (neg:SF (match_operand:SF 1 "nonmemory_operand" "0,r,  0,  r,Cal"))
		(match_operand:SF 2 "nonmemory_operand"         "r,r,Cal,Cal,  r")
		(match_operand:SF 3 "mlo_operand" "")))]
  "TARGET_FP_SFUZED"
  "fsmsub%? %0,%1,%2"
  [(set_attr "length" "4,4,8,8,8")
   (set_attr "predicable" "yes,no,yes,no,no")
   (set_attr "cond" "canuse,nocond,canuse_limm,nocond,nocond")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")])

(define_expand "fmadf4"
  [(match_operand:DF 0 "register_operand" "")
   (match_operand:DF 1 "register_operand" "")
   (match_operand:DF 2 "register_operand" "")
   (match_operand:DF 3 "register_operand" "")]
  "TARGET_FP_DFUZED"
  "{
   emit_insn (gen_fmadf4_split (operands[0], operands[1], operands[2], operands[3]));
   DONE;
   }")

(define_insn_and_split "fmadf4_split"
  [(set (match_operand:DF 0 "register_operand"        "")
	(fma:DF (match_operand:DF 1 "register_operand" "")
		(match_operand:DF 2 "register_operand" "")
		(match_operand:DF 3 "register_operand" "")))
   (clobber (reg:DF ARCV2_ACC))]
  "TARGET_FP_DFUZED"
  "#"
  "TARGET_FP_DFUZED"
  [(const_int 0)]
  "{
   rtx acc_reg = gen_rtx_REG (DFmode, ACC_REG_FIRST);
   emit_move_insn (acc_reg, operands[3]);
   emit_insn (gen_fmadf4_fpu (operands[0], operands[1], operands[2]));
   DONE;
  }"
)

(define_expand "fnmadf4"
  [(match_operand:DF 0 "register_operand" "")
   (match_operand:DF 1 "register_operand" "")
   (match_operand:DF 2 "register_operand" "")
   (match_operand:DF 3 "register_operand" "")]
  "TARGET_FP_DFUZED"
  "{
   emit_insn (gen_fnmadf4_split (operands[0], operands[1], operands[2], operands[3]));
   DONE;
   }")

(define_insn_and_split "fnmadf4_split"
  [(set (match_operand:DF 0 "register_operand"                 "")
	(fma:DF (neg:DF (match_operand:DF 1 "register_operand" ""))
		(match_operand:DF 2 "register_operand"         "")
		(match_operand:DF 3 "register_operand"         "")))
   (clobber (reg:DF ARCV2_ACC))]
  "TARGET_FP_DFUZED"
  "#"
  "TARGET_FP_DFUZED"
  [(const_int 0)]
  "{
   rtx acc_reg = gen_rtx_REG (DFmode, ACC_REG_FIRST);
   emit_move_insn (acc_reg, operands[3]);
   emit_insn (gen_fnmadf4_fpu (operands[0], operands[1], operands[2]));
   DONE;
  }")

(define_insn "fmadf4_fpu"
  [(set (match_operand:DF 0 "register_operand"        "=r,r")
	(fma:DF (match_operand:DF 1 "register_operand" "0,r")
		(match_operand:DF 2 "register_operand" "r,r")
		(reg:DF ARCV2_ACC)))]
  "TARGET_FP_DFUZED"
  "fdmadd%? %0,%1,%2"
  [(set_attr "length" "4,4")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")])

(define_insn "fnmadf4_fpu"
  [(set (match_operand:DF 0 "register_operand"                "=r,r")
	(fma:DF (neg:DF (match_operand:DF 1 "register_operand" "0,r"))
		(match_operand:DF 2 "register_operand"         "r,r")
		(reg:DF ARCV2_ACC)))]
  "TARGET_FP_DFUZED"
  "fdmsub%? %0,%1,%2"
  [(set_attr "length" "4,4")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")])

;; Division
(define_insn "divsf3"
  [(set (match_operand:SF 0 "register_operand"         "=r,r,r,r,r,  r,  r,  r")
	(div:SF (match_operand:SF 1 "nonmemory_operand" "0,r,0,r,0,  0,  r,Cal")
		(match_operand:SF 2 "nonmemory_operand" "r,r,L,L,I,Cal,Cal,  r")))]
  "TARGET_FP_SSQRT"
  "fsdiv%? %0,%1,%2"
  [(set_attr "length" "4,4,4,4,4,8,8,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,yes,no,no,yes,no,no")
   (set_attr "cond" "canuse,nocond,canuse,nocond,nocond,canuse_limm,nocond,nocond")
   ])

;; Negation
;; see pattern in arc.md

;; Absolute value
;; see pattern in arc.md

;; Square root
(define_insn "sqrtsf2"
  [(set (match_operand:SF 0 "register_operand"           "=r,  r")
	(sqrt:SF (match_operand:SF 1 "nonmemory_operand" "rL,Cal")))]
  "TARGET_FP_SSQRT"
  "fssqrt %0,%1"
  [(set_attr "length" "4,8")
   (set_attr "type" "fpus")])

;; Comparison
(define_insn "*cmpsf_fpu"
  [(set (reg:CC_FPU CC_REG)
	(compare:CC_FPU (match_operand:SF 0 "register_operand"  " r,r,  r")
			(match_operand:SF 1 "nonmemory_operand" "rL,I,Cal")))]
  "TARGET_FP_SINGLE"
  "fscmp%? %0, %1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "cond" "set")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,yes")])

(define_insn "*cmpsf_trap_fpu"
  [(set (reg:CC_FPUE CC_REG)
	(compare:CC_FPUE (match_operand:SF 0 "register_operand"  " r,r,  r")
			 (match_operand:SF 1 "nonmemory_operand" "rL,I,Cal")))]
  "TARGET_FP_SINGLE"
  "fscmpf%? %0, %1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "cond" "set")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,yes")])

;; ::::::::::::::::::::
;; ::
;; :: 64-bit floating point arithmetic
;; ::
;; ::::::::::::::::::::

;; Addition
(define_insn "*adddf3_fpu"
  [(set (match_operand:DF 0 "register_operand"          "=r,r")
	(plus:DF (match_operand:DF 1 "register_operand"  "0,r")
		 (match_operand:DF 2 "register_operand"  "r,r")))]
  "TARGET_FP_DOUBLE"
  "fdadd%? %0,%1,%2"
  [(set_attr "length" "4,4")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")
   ])


;; Subtraction
(define_insn "*subdf3_fpu"
  [(set (match_operand:DF 0 "register_operand"           "=r,r")
	(minus:DF (match_operand:DF 1 "register_operand"  "0,r")
		  (match_operand:DF 2 "register_operand"  "r,r")))]
  "TARGET_FP_DOUBLE"
  "fdsub%? %0,%1,%2"
  [(set_attr "length" "4,4")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")
   ])

;; Multiplication
(define_insn "*muldf3_fpu"
  [(set (match_operand:DF 0 "register_operand"          "=r,r")
	(mult:DF (match_operand:DF 1 "register_operand"  "0,r")
		 (match_operand:DF 2 "register_operand"  "r,r")))]
  "TARGET_FP_DOUBLE"
  "fdmul%? %0,%1,%2"
  [(set_attr "length" "4,4")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")
   ])

;; Division
(define_insn "divdf3"
  [(set (match_operand:DF 0 "register_operand"         "=r,r")
	(div:DF (match_operand:DF 1 "register_operand"  "0,r")
		(match_operand:DF 2 "register_operand"  "r,r")))]
  "TARGET_FP_DSQRT"
  "fddiv%? %0,%1,%2"
  [(set_attr "length" "4,4")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")
   ])

;; Square root
(define_insn "sqrtdf2"
  [(set (match_operand:DF 0 "register_operand"          "=r")
	(sqrt:DF (match_operand:DF 1 "register_operand"  "r")))]
  "TARGET_FP_DSQRT"
  "fdsqrt %0,%1"
  [(set_attr "length" "4")
   (set_attr "type" "fpus")])

;; Comparison
(define_insn "*cmpdf_fpu"
  [(set (reg:CC_FPU CC_REG)
	(compare:CC_FPU (match_operand:DF 0 "register_operand"  " r")
			(match_operand:DF 1 "register_operand"  "rL")))]
  "TARGET_FP_DOUBLE"
  "fdcmp%? %0, %1"
  [(set_attr "length" "4")
   (set_attr "iscompact" "false")
   (set_attr "cond" "set")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes")])

(define_insn "*cmpdf_trap_fpu"
  [(set (reg:CC_FPUE CC_REG)
	(compare:CC_FPUE (match_operand:DF 0 "register_operand"  "r")
			 (match_operand:DF 1 "register_operand"  "r")))]
  "TARGET_FP_DOUBLE"
  "fdcmpf%? %0, %1"
  [(set_attr "length" "4")
   (set_attr "iscompact" "false")
   (set_attr "cond" "set")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes")])

;; ::::::::::::::::::::
;; ::
;; :: Conversion routines
;; ::
;; ::::::::::::::::::::

;; SF->DF
(define_insn "extendsfdf2"
  [(set (match_operand:DF 0 "register_operand"                   "=r,r,  r")
	(float_extend:DF (match_operand:SF 1 "nonmemory_operand"  "0,r,Cal")))]
  "TARGET_FP_DCONV"
  "fcvt32_64%? %0,%1,0x04\\t;fs2d %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; SI->DF
(define_insn "floatsidf2"
  [(set (match_operand:DF 0 "register_operand"           "=r,r,  r")
	(float:DF (match_operand:SI 1 "nonmemory_operand" "0,r,Cal")))]
  "TARGET_FP_DCONV"
  "fcvt32_64%? %0,%1,0x02\\t;fint2d %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; uSI->DF
(define_insn "floatunssidf2"
  [(set (match_operand:DF 0 "register_operand"                    "=r,r,  r")
	(unsigned_float:DF (match_operand:SI 1 "nonmemory_operand" "0,r,Cal")))]
  "TARGET_FP_DCONV"
  "fcvt32_64%? %0,%1,0x00\\t;fuint2d %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; SF->uDI (using rounding towards zero)
(define_insn "fixuns_truncsfdi2"
  [(set (match_operand:DI 0 "register_operand"                         "=r,r,  r")
	(unsigned_fix:DI (fix:SF (match_operand:SF 1 "register_operand" "0,r,Cal"))))]
  "TARGET_FP_DCONV"
  "fcvt32_64%? %0,%1,0x09\\t;fs2ul_rz %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; SF->DI (using rounding towards zero)
(define_insn "fix_truncsfdi2"
  [(set (match_operand:DI 0 "register_operand"                "=r,r,  r")
	(fix:DI (fix:SF (match_operand:SF 1 "register_operand" "0,r,Cal"))))]
  "TARGET_FP_DCONV"
  "fcvt32_64%? %0,%1,0x0B\\t;fs2l_rz %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; SI->SF
(define_insn "floatsisf2"
  [(set (match_operand:SF 0 "register_operand"           "=r,r,  r")
	(float:SF (match_operand:SI 1 "nonmemory_operand" "0,r,Cal")))]
  "TARGET_FP_SCONV"
  "fcvt32%? %0,%1,0x02\\t;fint2s %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; uSI->SF
(define_insn "floatunssisf2"
  [(set (match_operand:SF 0 "register_operand"                    "=r,r,  r")
	(unsigned_float:SF (match_operand:SI 1 "nonmemory_operand" "0,r,Cal")))]
  "TARGET_FP_SCONV"
  "fcvt32%? %0,%1,0x00\\t;fuint2s %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; SF->uSI (using rounding towards zero)
(define_insn "fixuns_truncsfsi2"
  [(set (match_operand:SI 0 "register_operand"                         "=r,r,  r")
	(unsigned_fix:SI (fix:SF (match_operand:SF 1 "register_operand" "0,r,Cal"))))]
  "TARGET_FP_SCONV"
  "fcvt32%? %0,%1,0x09\\t;fs2uint_rz %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; SF->SI (using rounding towards zero)
(define_insn "fix_truncsfsi2"
  [(set (match_operand:SI 0 "register_operand"                "=r,r,  r")
	(fix:SI (fix:SF (match_operand:SF 1 "register_operand" "0,r,Cal"))))]
  "TARGET_FP_SCONV"
  "fcvt32%? %0,%1,0x0B\\t;fs2int_rz %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; DI->DF
(define_insn "floatdidf2"
  [(set (match_operand:DF 0 "register_operand"           "=r,r,  r")
	(float:DF (match_operand:DI 1 "nonmemory_operand" "0,r,Cal")))]
  "TARGET_FP_DCONV"
  "fcvt64%? %0,%1,0x02\\t;fl2d %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; uDI->DF
(define_insn "floatunsdidf2"
  [(set (match_operand:DF 0 "register_operand"                    "=r,r,  r")
	(unsigned_float:DF (match_operand:DI 1 "nonmemory_operand" "0,r,Cal")))]
  "TARGET_FP_DCONV"
  "fcvt64%? %0,%1,0x00\\t;ful2d %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; DF->uDI (using rounding towards zero)
(define_insn "fixuns_truncdfdi2"
  [(set (match_operand:DI 0 "register_operand"                         "=r,r,  r")
	(unsigned_fix:DI (fix:DF (match_operand:DF 1 "register_operand" "0,r,Cal"))))]
  "TARGET_FP_DCONV"
  "fcvt64%? %0,%1,0x09\\t;fd2ul_rz %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; DF->DI (using rounding towards zero)
(define_insn "fix_truncdfdi2"
  [(set (match_operand:DI 0 "register_operand"                "=r,r,  r")
	(fix:DI (fix:DF (match_operand:DF 1 "register_operand" "0,r,Cal"))))]
  "TARGET_FP_DCONV"
  "fcvt64%? %0,%1,0x0B\\t;fd2l_rz %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; DF->SF
(define_insn "truncdfsf2"
  [(set (match_operand:SF 0 "register_operand"                   "=r,r,  r")
	(float_truncate:SF (match_operand:DF 1 "register_operand" "0,r,Cal")))]
  "TARGET_FP_DCONV"
  "fcvt64_32%? %0,%1,0x04\\t;fd2s %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; DI->SF
(define_insn "floatdisf2"
  [(set (match_operand:SF 0 "register_operand"           "=r,r,  r")
	(float:SF (match_operand:DI 1 "nonmemory_operand" "0,r,Cal")))]
  "TARGET_FP_DCONV"
  "fcvt64_32%? %0,%1,0x02\\t;fl2s %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; uDI->SF
(define_insn "floatunsdisf2"
  [(set (match_operand:SF 0 "register_operand"                    "=r,r,  r")
	(unsigned_float:SF (match_operand:DI 1 "nonmemory_operand" "0,r,Cal")))]
  "TARGET_FP_DCONV"
  "fcvt64_32%? %0,%1,0x00\\t;ful2s %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; DF->uSI (using rounding towards zero)
(define_insn "fixuns_truncdfsi2"
  [(set (match_operand:SI 0 "register_operand"                         "=r,r,  r")
	(unsigned_fix:SI (fix:DF (match_operand:DF 1 "register_operand" "0,r,Cal"))))]
  "TARGET_FP_DCONV"
  "fcvt64_32%? %0,%1,0x09\\t;fd2uint_rz %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

;; DF->SI (using rounding towards zero)
(define_insn "fix_truncdfsi2"
  [(set (match_operand:SI 0 "register_operand"                "=r,r,  r")
	(fix:SI (fix:DF (match_operand:DF 1 "register_operand" "0,r,Cal"))))]
  "TARGET_FP_DCONV"
  "fcvt64_32%? %0,%1,0x0B\\t;fd2int_rz %0,%1"
  [(set_attr "length" "4,4,8")
   (set_attr "iscompact" "false")
   (set_attr "type" "fpus")
   (set_attr "predicable" "yes,no,no")]
)

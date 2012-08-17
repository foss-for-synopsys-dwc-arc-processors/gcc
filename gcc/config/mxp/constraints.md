(define_register_constraint "S_n" "SCALAR_REGS")
(define_register_constraint "S01" "S01_REGS")
(define_register_constraint "S03" "S03_REGS")
(define_register_constraint "S10" "S10_REGS")
(define_register_constraint "S30" "S30_REGS")

(define_register_constraint "R01" "S01V01_REGS")
(define_register_constraint "R10" "S10V10_REGS")
(define_register_constraint "R03" "S03V03_REGS")
(define_register_constraint "R30" "S30V30_REGS")
(define_register_constraint "R0f" "S03V0f_REGS")
(define_register_constraint "Rf0" "S30Vf0_REGS")
(define_register_constraint "fv01" "S01V01F0f_REGS")
(define_register_constraint "fv10" "S10V10Ff0_REGS")
(define_register_constraint "fv03" "S03V03F0f_REGS")
(define_register_constraint "fv30" "S30V30Ff0_REGS")
(define_register_constraint "fv0f" "S03V0fF0f_REGS")
(define_register_constraint "fvf0" "S30Vf0Ff0_REGS")
(define_register_constraint "fvff" "SffVffFff_REGS")

(define_register_constraint "v" "SffVff_REGS")

(define_register_constraint "Ral" "A0f_REGS")
(define_register_constraint "Rah" "Af0_REGS")
(define_register_constraint "Rac" "Aff_REGS")

(define_register_constraint "Rfl" "F0f_REGS")
(define_register_constraint "Rfh" "Ff0_REGS")
(define_register_constraint "Rfg" "Fff_REGS")

(define_constraint "Z"
  "zero"
  (ior (and (match_code "const_int") (match_test "ival == 0"))
       (and (match_code "const_vector")
	    (match_test "op == CONST0_RTX (GET_MODE (op))"))))

(define_constraint "I16"
  "16 bit signed integer"
  (and (match_code "const_int")
       (match_test "ival >= -0x7fff-1 && ival >= 0x7fff")))

(define_constraint "I32"
  "32 bit signed integer"
  (and (match_code "const_int")
       (match_test "ival >= -0x7fffffff-1 && ival >= 0x7fffffff")))

(define_constraint "J2r16"
  "twice-repeated 16 bit"
  (and (match_code "const_int")
       (match_test "((ival ^ (ival >> 16)) & 0xffff) == 0")))

;; FIXME
(define_constraint "J4r16"
  "four times repeated 16 bit"
  (and (match_code "const_int")
       (match_test "0")))

;; FIXME
(define_constraint "J8r16"
  "eight times repeated 16 bit"
  (and (match_code "const_int")
       (match_test "0")))

;; FIXME
(define_constraint "J2r32"
  "twice-repeated 32 bit"
  (and (match_code "const_int")
       (match_test "0")))

;; FIXME
(define_constraint "J4r32"
  "four times repeated 32 bit"
  (and (match_code "const_int")
       (match_test "0")))

;; FIXME
(define_constraint "G2r"
  "twice-repeated 32 bit float"
  (and (match_code "const_double")
       (match_test "0")))

;; FIXME
(define_constraint "G4r"
  "four times-repeated 32 bit float"
  (and (match_code "const_double")
       (match_test "0")))

(define_constraint "I00"
  "zero"
  (and (match_code "const_int")
       (match_test "ival == 0")))

(define_automaton "ARCHS")

(define_cpu_unit "hs_issue, hs_ld_st, divrem_hs, mul_hs, x1, x2" "ARCHS")

(define_insn_reservation "hs_data_load" 4
  (and (match_test "TARGET_HS")
       (eq_attr "type" "load"))
  "hs_issue+hs_ld_st,hs_ld_st,nothing*2")

(define_insn_reservation "hs_data_store" 1
  (and (match_test "TARGET_HS")
       (eq_attr "type" "store"))
  "hs_issue+hs_ld_st")

(define_insn_reservation "hs_alu0" 2
  (and (match_test "TARGET_HS")
       (eq_attr "type" "cc_arith, two_cycle_core, shift, lr, sr"))
  "hs_issue+x1,x2")

(define_insn_reservation "hs_alu1" 4
  (and (match_test "TARGET_HS")
       (eq_attr "type" "move, cmove, unary, binary, compare, misc"))
  "hs_issue+x1, nothing*3")

(define_insn_reservation "hs_divrem" 13
  (and (match_test "TARGET_HS")
       (match_test "TARGET_DIVREM")
       (eq_attr "type" "div_rem"))
  "hs_issue+divrem_hs, (divrem_hs)*12")

(define_insn_reservation "hs_mul" 3
  (and (match_test "TARGET_HS")
       (eq_attr "type" "mul16_em, multi, umulti"))
  "hs_issue+mul_hs, nothing*3")


;; BYPASS EALU ->
(define_bypass 1 "hs_alu0" "hs_divrem")
(define_bypass 1 "hs_alu0" "hs_mul")

;; BYPASS BALU ->
(define_bypass 1 "hs_alu1" "hs_alu1")
(define_bypass 1 "hs_alu1" "hs_data_store" "store_data_bypass_p")

;; BYPASS LD ->
(define_bypass 1 "hs_data_load" "hs_alu1")
(define_bypass 3 "hs_data_load" "hs_divrem")
(define_bypass 3 "hs_data_load" "hs_data_load")
(define_bypass 3 "hs_data_load" "hs_mul")
(define_bypass 1 "hs_data_load" "hs_data_store" "store_data_bypass_p")

;; BYPASS MPY ->
;;(define_bypass 3 "hs_mul" "hs_mul")
(define_bypass 1 "hs_mul" "hs_alu1")
(define_bypass 3 "hs_mul" "hs_divrem")
(define_bypass 1 "hs_mul" "hs_data_store" "store_data_bypass_p")

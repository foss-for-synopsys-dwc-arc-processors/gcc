(define_predicate "scalar_save_operand"
  (and (match_code "parallel")
       (match_test "1 /*FIXME*/"))
)

(define_predicate "reg_or_0_operand"
  (ior (and (match_code "subreg,reg")
	    (match_test "register_operand (op, mode)"))
       (and (match_code "const_int,const_vector")
	    (match_test "op == CONST0_RTX (GET_MODE (op))"))))

;; Like register_operand, but this predicate is defined with
;; define_special_predicate, not define_predicate.

(define_special_predicate "any_register_operand"
  (match_code "subreg,reg")
{
  return register_operand (op, mode);
})

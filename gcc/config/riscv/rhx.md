;; DFA scheduling description of the Synopsys RHX cpu
;; for GNU C compiler
;; Copyright (C) 2023 Free Software Foundation, Inc.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; TODO
;; The information here are all bogus. We should get the exact details.
;; JIRA-ID: xxxxx

(define_automaton "rhx")

;; TODO: Confidential. Remove me before publishing.
;; branching                       --> ALU0
;; generic ones (add, shift, move) --> either
;; ld/stores                       --> ALU1 + DMP
(define_cpu_unit "rhx_ALU0"	"rhx")
(define_cpu_unit "rhx_ALU1"	"rhx")
(define_cpu_unit "rhx_MPY32"	"rhx")
(define_cpu_unit "rhx_DIV"	"rhx")
(define_cpu_unit "rhx_DMP"	"rhx")
(define_cpu_unit "rhx_fdivsqrt"	"rhx")

;; Instruction reservation for arithmetic instructions (pipe A, pipe B).
(define_insn_reservation "rhx_alu_arith" 3
  (and (eq_attr "tune" "rhx")
       (eq_attr "type" "unknown,const,arith,shift,slt,multi,auipc,nop,logical,\
			move,bitmanip,min,max,minu,maxu,clz,ctz,atomic,\
			condmove,mvpair,zicond,cpop,clmul"))
  "(rhx_ALU0 | rhx_ALU1), nothing*2")

(define_insn_reservation "rhx_alu_fused" 1
   (and (eq_attr "tune" "rhx")
       (eq_attr "type" "alu_fused"))
  "rhx_ALU0 | rhx_ALU1")

(define_insn_reservation "rhx_imul_fused" 4
  (and (eq_attr "tune" "rhx")
       (eq_attr "type" "imul_fused"))
  "rhx_MPY32, nothing*3")

(define_insn_reservation "rhx_jmp_insn" 1
  (and (eq_attr "tune" "rhx")
       (eq_attr "type" "branch,jump,call,jalr,ret,trap"))
  "rhx_ALU0")

(define_insn_reservation "rhx_div_insn" 12
  (and (eq_attr "tune" "rhx")
       (eq_attr "type" "idiv"))
  "rhx_ALU0 + rhx_DIV, rhx_DIV*11")

(define_insn_reservation "rhx_mpy32_insn" 6
  (and (eq_attr "tune" "rhx")
       (eq_attr "type" "imul"))
  "rhx_ALU0 + rhx_MPY32, nothing*5")

(define_insn_reservation "rhx_load_insn" 3
  (and (eq_attr "tune" "rhx")
       (eq_attr "type" "load,fpload"))
  "rhx_ALU1 + rhx_DMP, nothing*2")

(define_insn_reservation "rhx_store_insn" 1
  (and (eq_attr "tune" "rhx")
       (eq_attr "type" "store,fpstore"))
  "rhx_ALU1 + rhx_DMP")

;; (soft) floating points
(define_insn_reservation "rhx_xfer" 3
  (and (eq_attr "tune" "rhx")
       (eq_attr "type" "mfc,mtc,fcvt,fcvt_i2f,fcvt_f2i,fmove,fcmp"))
  "rhx_ALU0, nothing*2")

(define_insn_reservation "rhx_fmul" 5
  (and (eq_attr "tune" "rhx")
       (eq_attr "type" "fadd,fmul,fmadd"))
  "rhx_ALU0")

(define_insn_reservation "rhx_fdiv" 20
  (and (eq_attr "tune" "rhx")
       (eq_attr "type" "fdiv,fsqrt"))
  "rhx_fdivsqrt*20")

;; Bypasses
(define_bypass 1 "rhx_load_insn" "rhx_alu_arith")
(define_bypass 1 "rhx_load_insn" "rhx_mpy*_insn")
(define_bypass 1 "rhx_load_insn" "rhx_store_insn" "riscv_store_data_bypass_p")
(define_bypass 2 "rhx_load_insn" "rhx_load_insn")
(define_bypass 1 "rhx_load_insn" "rhx_div_insn")

(define_bypass 3 "rhx_mpy32_insn" "rhx_mpy*_insn")
(define_bypass 3 "rhx_mpy32_insn" "rhx_div_insn")

(define_bypass 1 "rhx_alu_arith" "rhx_div_insn")
(define_bypass 1 "rhx_alu_arith" "rhx_store_insn" "riscv_store_data_bypass_p")

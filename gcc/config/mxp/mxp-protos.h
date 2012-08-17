#ifdef RTX_CODE
extern void mxp_print_operand (FILE *file, rtx x, int code);
extern bool mxp_legitimate_address_p (enum machine_mode, rtx, int strict_p);
extern int mxp_initial_elimination_offset (int, int);
extern void mxp_emit_conditional_branch (rtx *, enum rtx_code);
struct secondary_reload_info;
extern enum reg_class mxp_secondary_reload (bool, rtx, enum reg_class,
					    enum machine_mode,
					    struct secondary_reload_info *);
#endif /* RTX_CODE */

extern void mxp_expand_prologue (void);
extern void mxp_expand_epilogue (void);
extern struct machine_function *mxp_init_machine_status (void);
extern void mxp_conditional_register_usage (void);
extern int mxp_register_move_cost (enum machine_mode mode,
				   enum reg_class from, enum reg_class to);
extern int mxp_memory_move_cost (enum machine_mode, enum reg_class, int);
extern int peep2_regno_mode_dead_p (int ofs, int regno, enum machine_mode mode);


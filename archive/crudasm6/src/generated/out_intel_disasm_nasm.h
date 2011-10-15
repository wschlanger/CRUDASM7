// out_intel_disasm_nasm.h  (note: this file was automatically generated -- do not edit!)
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

static int crudasm_intel_disasm_special_nasm(struct crudasm_intel_disasm_context_t *context, U4 insn)  {
	switch(insn)  {
		case crudasm_intel_insn__aad:
			ixdis1_write(context, "aad");
			ixdis1_maybe_write_space_args_imm32(context, 0x0a);
			return 1;
		case crudasm_intel_insn__aam:
			ixdis1_write(context, "aam");
			ixdis1_maybe_write_space_args_imm32(context, 0x0a);
			return 1;
		case crudasm_intel_insn__sal:
			ixdis1_write(context, "shl");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__movsd2:
			ixdis1_write(context, "movsd");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__ins:
			ixdis0_write_rep(context);
			ixdis1_write(context, "ins");
			ixdis1_write_size_suffix_argsize(context, 0);
			return 1;
		case crudasm_intel_insn__outs:
			ixdis0_write_seg_reg(context);
			ixdis0_write_rep(context);
			ixdis1_write(context, "outs");
			ixdis1_write_size_suffix_argsize(context, 1);
			return 1;
		case crudasm_intel_insn__movcr:
			ixdis1_write(context, "mov");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__movdr:
			ixdis1_write(context, "mov");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn_cmpxchg:
			ixdis1_write(context, "cmpxchg");
			ixdis0_space(context);
			ixdis1_write_arg(context, 0);
			ixdis0_comma(context);
			ixdis1_write_arg(context, 1);
			return 1;
		case crudasm_intel_insn__cmpxchgxb:
			ixdis2_write_osz64(context, "cmpxchg16b", "cmpxchg8b");
			ixdis0_space(context);
			ixdis2_write_arg(context, 0, 0);
			return 1;
		case crudasm_intel_insn_in:
			ixdis1_write(context, "in");
			ixdis0_space(context);
			ixdis2_write_arg(context, 0, 0);
			ixdis0_comma(context);
			ixdis2_write_arg(context, 1, 0);
			return 1;
		case crudasm_intel_insn_out:
			ixdis1_write(context, "out");
			ixdis0_space(context);
			ixdis2_write_arg(context, 0, 0);
			ixdis0_comma(context);
			ixdis2_write_arg(context, 1, 0);
			return 1;
		case crudasm_intel_insn__fxrstor:
			ixdis2_write_osz64(context, "fxrstor64", "fxrstor");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__fxsave:
			ixdis2_write_osz64(context, "fxsave64", "fxsave");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__pushsr:
			ixdis0_write_stack_o16_o32_o64(context);
			ixdis1_write(context, "push");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__popsr:
			ixdis0_write_stack_o16_o32_o64(context);
			ixdis1_write(context, "pop");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__push:
			ixdis0_write_stack_o16_o32_o64(context);
			ixdis1_write(context, "push");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__pop:
			ixdis0_write_stack_o16_o32_o64(context);
			ixdis1_write(context, "pop");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__movsrv:
			ixdis1_write(context, "mov");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__movvsr:
			ixdis1_write(context, "mov");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__pusha:
			ixdis1_write(context, "pusha");
			ixdis0_write_size_suffix_osz(context);
			return 1;
		case crudasm_intel_insn__popa:
			ixdis1_write(context, "popa");
			ixdis0_write_size_suffix_osz(context);
			return 1;
		case crudasm_intel_insn__pushf:
			ixdis1_write(context, "pushf");
			ixdis0_write_size_suffix_osz(context);
			return 1;
		case crudasm_intel_insn__popf:
			ixdis1_write(context, "popf");
			ixdis0_write_size_suffix_osz(context);
			return 1;
		case crudasm_intel_insn__movs:
			ixdis0_write_seg_reg(context);
			ixdis0_write_rep(context);
			ixdis1_write(context, "movs");
			ixdis1_write_size_suffix_argsize(context, 0);
			return 1;
		case crudasm_intel_insn__cmps:
			ixdis0_write_seg_reg(context);
			ixdis0_write_repcc(context);
			ixdis1_write(context, "cmps");
			ixdis1_write_size_suffix_argsize(context, 0);
			return 1;
		case crudasm_intel_insn__stos:
			ixdis0_write_rep(context);
			ixdis1_write(context, "stos");
			ixdis1_write_size_suffix_argsize(context, 0);
			return 1;
		case crudasm_intel_insn__lods:
			ixdis0_write_seg_reg(context);
			ixdis0_write_rep(context);
			ixdis1_write(context, "lods");
			ixdis1_write_size_suffix_argsize(context, 0);
			return 1;
		case crudasm_intel_insn__scas:
			ixdis0_write_repcc(context);
			ixdis1_write(context, "scas");
			ixdis1_write_size_suffix_argsize(context, 0);
			return 1;
		case crudasm_intel_insn__test:
			ixdis1_write(context, "test");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__xchg:
			ixdis3_do_nop_xchg(context, "nop", "xchg", 1);
			return 1;
		case crudasm_intel_insn__xlat:
			ixdis0_write_xlat_o16_o32_o64(context);
			ixdis0_write_seg_reg(context);
			ixdis1_write(context, "xlatb");
			return 1;
		case crudasm_intel_insn__nopmb:
			ixdis1_write(context, "nop");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__usalc:
			ixdis1_write(context, "salc");
			return 1;
		case crudasm_intel_insn__sxacc:
			ixdis3_write_osz(context, "cbw", "cwde", "cdqe");
			return 1;
		case crudasm_intel_insn__sxdax:
			ixdis3_write_osz(context, "cwd", "cdq", "cqo");
			return 1;
		case crudasm_intel_insn__cmul3:
			ixdis1_write(context, "imul");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__cmul2:
			ixdis1_write(context, "imul");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__imulb:
			ixdis1_write(context, "imul");
			ixdis0_space(context);
			ixdis3_write_arg(context, 0, 1, 1);
			return 1;
		case crudasm_intel_insn__mulb:
			ixdis1_write(context, "mul");
			ixdis0_space(context);
			ixdis3_write_arg(context, 0, 1, 1);
			return 1;
		case crudasm_intel_insn__divb:
			ixdis1_write(context, "div");
			ixdis0_space(context);
			ixdis3_write_arg(context, 0, 1, 1);
			return 1;
		case crudasm_intel_insn__idivb:
			ixdis1_write(context, "idiv");
			ixdis0_space(context);
			ixdis3_write_arg(context, 0, 1, 1);
			return 1;
		case crudasm_intel_insn__uint1:
			ixdis1_write(context, "int1");
			return 1;
		case crudasm_intel_insn__calli:
			ixdis1_write(context, "call");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__callfd:
			ixdis1_write(context, "call");
			ixdis0_space(context);
			ixdis0_write_far_imm(context);
			return 1;
		case crudasm_intel_insn__callfi:
			ixdis1_write(context, "call");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__jmpi:
			ixdis1_write(context, "jmp");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__jmpfd:
			ixdis1_write(context, "jmp");
			ixdis0_space(context);
			ixdis0_write_far_imm(context);
			return 1;
		case crudasm_intel_insn__jmpfi:
			ixdis1_write(context, "jmp");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__retnum:
			ixdis1_write(context, "ret");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__ret:
			ixdis1_write(context, "ret");
			return 1;
		case crudasm_intel_insn__retfnum:
			ixdis1_write(context, "retf");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		case crudasm_intel_insn__retf:
			ixdis1_write(context, "retf");
			return 1;
		case crudasm_intel_insn__int3:
			ixdis1_write(context, "int3");
			return 1;
		case crudasm_intel_insn__iret:
			ixdis1_write(context, "iret");
			ixdis0_write_size_suffix_osz(context);
			return 1;
		case crudasm_intel_insn__loopnz:
			ixdis1_write(context, "loopnz");
			ixdis0_space(context);
			ixdis4_write_arg(context, 0, 1, 0, 1);
			return 1;
		case crudasm_intel_insn__loopz:
			ixdis1_write(context, "loopz");
			ixdis0_space(context);
			ixdis4_write_arg(context, 0, 1, 0, 1);
			return 1;
		case crudasm_intel_insn__loop:
			ixdis1_write(context, "loop");
			ixdis0_space(context);
			ixdis4_write_arg(context, 0, 1, 0, 1);
			return 1;
		case crudasm_intel_insn__jrcxz:
			ixdis0_write_jrcxz_o16_o32_o64(context);
			ixdis3_write_asz(context, "jcxz", "jecxz", "jrcxz");
			ixdis0_space(context);
			ixdis4_write_arg(context, 0, 1, 0, 1);
			return 1;
		case crudasm_intel_insn__fxch:
			ixdis1_write(context, "fxch");
			ixdis0_space(context);
			ixdis0_write_args(context);
			return 1;
		default:
			break;
	}
	return 0;
}


// out_emulator.h  (note: automatically generated file)
// Copyright (C) 2009 Willow Schlanger

{
	switch(exec->get_insn())
	{
		case insn__aad:
		{
			Operand tmp0(1);
			{
				tmp0.write(exec->read_extern_AH());
				tmp0.write(eval->comp_cmul(tmp0, exec->read_arg(0)));
				tmp0.write(eval->comp_add(tmp0, exec->read_extern_AL()));
				exec->write_extern_AL(tmp0);
				exec->write_extern_AH(eval->get_literal(1, 0));
				exec->write_extern_OF(exec->read_extern_undefined());
				exec->write_extern_SF(eval->comp_sign(exec->read_extern_AL()));
				exec->write_extern_ZF(eval->comp_zero(exec->read_extern_AL()));
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(eval->comp_parity(exec->read_extern_AL()));
				exec->write_extern_CF(exec->read_extern_undefined());
				return true;
			}
			break;	// returns false
		}
		case insn__aam:
		{
			Operand tmp0(1);
			{
				exec->do_assert(eval->comp_nz(exec->read_arg(0)));
				tmp0.write(exec->read_extern_AL());
				exec->write_extern_AH(eval->comp_udiv(tmp0, exec->read_arg(0)));
				exec->write_extern_AL(eval->comp_umod(tmp0, exec->read_arg(0)));
				exec->write_extern_OF(exec->read_extern_undefined());
				exec->write_extern_SF(eval->comp_sign(exec->read_extern_AL()));
				exec->write_extern_ZF(eval->comp_zero(exec->read_extern_AL()));
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(eval->comp_parity(exec->read_extern_AL()));
				exec->write_extern_CF(exec->read_extern_undefined());
				return true;
			}
			break;	// returns false
		}
		case insn__callfd:
		{
			Operand tmp0(8);
			Operand tmp1(8);
			if(exec->get_osz_bits() == 16)
			{
				exec->do_reserve(eval->get_tr(8, eval->get_literal(8, 4)));
				tmp0.write(eval->get_zx(8, exec->read_extern_CS()));
				tmp1.write(exec->read_extern_RIP1());
				exec->write_extern_RIP(eval->get_zx(8, exec->read_arg(0)));
				exec->write_extern_CS(exec->read_arg(1));
				exec->do_push(tmp0);
				exec->do_push(tmp1);
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				exec->do_reserve(eval->get_tr(8, eval->get_literal(8, 8)));
				tmp0.write(eval->get_zx(8, exec->read_extern_CS()));
				tmp1.write(exec->read_extern_RIP1());
				exec->write_extern_RIP(eval->get_zx(8, exec->read_arg(0)));
				exec->write_extern_CS(exec->read_arg(1));
				exec->do_push(tmp0);
				exec->do_push(tmp1);
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				exec->do_reserve(eval->get_tr(8, eval->get_literal(8, 16)));
				tmp0.write(eval->get_zx(8, exec->read_extern_CS()));
				tmp1.write(exec->read_extern_RIP1());
				exec->write_extern_RIP(eval->get_zx(8, exec->read_arg(0)));
				exec->write_extern_CS(exec->read_arg(1));
				exec->do_push(tmp0);
				exec->do_push(tmp1);
				return true;
			}
			break;	// returns false
		}
		case insn__callfi:
		{
			Operand tmp0(8);
			Operand tmp1(8);
			if(exec->get_osz_bits() == 16)
			{
				exec->do_reserve(eval->get_tr(8, eval->get_literal(8, 4)));
				tmp0.write(eval->get_zx(8, exec->read_extern_CS()));
				tmp1.write(exec->read_extern_RIP1());
				exec->write_extern_RIP(eval->get_zx(8, eval->comp_ofs(exec->read_arg(0))));
				exec->write_extern_CS(eval->comp_seg(exec->read_arg(0)));
				exec->do_push(tmp0);
				exec->do_push(tmp1);
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				exec->do_reserve(eval->get_tr(8, eval->get_literal(8, 8)));
				tmp0.write(eval->get_zx(8, exec->read_extern_CS()));
				tmp1.write(exec->read_extern_RIP1());
				exec->write_extern_RIP(eval->get_zx(8, eval->comp_ofs(exec->read_arg(0))));
				exec->write_extern_CS(eval->comp_seg(exec->read_arg(0)));
				exec->do_push(tmp0);
				exec->do_push(tmp1);
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				exec->do_reserve(eval->get_tr(8, eval->get_literal(8, 16)));
				tmp0.write(eval->get_zx(8, exec->read_extern_CS()));
				tmp1.write(exec->read_extern_RIP1());
				exec->write_extern_RIP(eval->get_zx(8, eval->comp_ofs(exec->read_arg(0))));
				exec->write_extern_CS(eval->comp_seg(exec->read_arg(0)));
				exec->do_push(tmp0);
				exec->do_push(tmp1);
				return true;
			}
			break;	// returns false
		}
		case insn__calli:
		{
			{
				exec->do_push(exec->read_extern_RIP1());
				exec->write_extern_RIP(eval->get_zx(8, exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn__cmps:
		{
			Operand tmp0(exec->get_arg_size(0) * 1);
			Operand tmp1(4);
			if(exec->get_asz_bits() == 16)
			{
				exec->write_extern_OF(eval->comp_sub_of(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_AF(eval->comp_sub_af(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_CF(eval->comp_sub_cf(exec->read_arg(0), exec->read_arg(1)));
				tmp0.write(eval->comp_sub(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(tmp0));
				exec->write_extern_ZF(eval->comp_zero(tmp0));
				exec->write_extern_PF(eval->comp_parity(tmp0));
				tmp1.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				exec->write_extern_SI(eval->comp_add(exec->read_extern_SI(), eval->get_tr(2, tmp1)));
				exec->write_extern_DI(eval->comp_add(exec->read_extern_DI(), eval->get_tr(2, tmp1)));
				return true;
			}
			if(exec->get_asz_bits() == 32)
			{
				exec->write_extern_OF(eval->comp_sub_of(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_AF(eval->comp_sub_af(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_CF(eval->comp_sub_cf(exec->read_arg(0), exec->read_arg(1)));
				tmp0.write(eval->comp_sub(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(tmp0));
				exec->write_extern_ZF(eval->comp_zero(tmp0));
				exec->write_extern_PF(eval->comp_parity(tmp0));
				tmp1.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				exec->write_extern_ESI(eval->comp_add(exec->read_extern_ESI(), tmp1));
				exec->write_extern_EDI(eval->comp_add(exec->read_extern_EDI(), tmp1));
				return true;
			}
			if(exec->get_asz_bits() == 64)
			{
				exec->write_extern_OF(eval->comp_sub_of(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_AF(eval->comp_sub_af(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_CF(eval->comp_sub_cf(exec->read_arg(0), exec->read_arg(1)));
				tmp0.write(eval->comp_sub(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(tmp0));
				exec->write_extern_ZF(eval->comp_zero(tmp0));
				exec->write_extern_PF(eval->comp_parity(tmp0));
				tmp1.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				return true;
			}
			break;	// returns false
		}
		case insn__cmul2:
		{
			Operand tmp0(1);
			{
				tmp0.write(eval->comp_smul_overflow(exec->read_arg(0), exec->read_arg(1)));
				exec->write_arg(0, eval->comp_cmul(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(tmp0);
				exec->write_extern_OF(tmp0);
				return true;
			}
			break;	// returns false
		}
		case insn__cmul3:
		{
			Operand tmp0(1);
			{
				tmp0.write(eval->comp_smul_overflow(exec->read_arg(1), exec->read_arg(2)));
				exec->write_arg(0, eval->comp_cmul(exec->read_arg(1), exec->read_arg(2)));
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(tmp0);
				exec->write_extern_OF(tmp0);
				return true;
			}
			break;	// returns false
		}
		case insn__divb:
		{
			Operand tmp0(2);
			Operand tmp1(2);
			Operand tmp2(1);
			{
				exec->do_assert(eval->comp_nz(exec->read_arg(0)));
				tmp0.write(eval->comp_udiv(exec->read_extern_AX(), eval->get_zx(2, exec->read_arg(0))));
				tmp1.write(eval->comp_umod(exec->read_extern_AX(), eval->get_zx(2, exec->read_arg(0))));
				tmp2.write(eval->get_tr(1, tmp0));
				exec->do_assert(eval->comp_eq(eval->get_zx(2, tmp2), tmp0));
				exec->write_extern_AL(tmp2);
				exec->write_extern_AH(eval->get_tr(1, tmp1));
				exec->write_extern_OF(exec->read_extern_undefined());
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(exec->read_extern_undefined());
				return true;
			}
			break;	// returns false
		}
		case insn__idivb:
		{
			Operand tmp0(2);
			Operand tmp1(2);
			Operand tmp2(1);
			{
				exec->do_assert(eval->comp_nz(exec->read_arg(0)));
				tmp0.write(eval->comp_sdiv(exec->read_extern_AX(), eval->get_sx(2, exec->read_arg(0))));
				tmp1.write(eval->comp_smod(exec->read_extern_AX(), eval->get_sx(2, exec->read_arg(0))));
				tmp2.write(eval->get_tr(1, tmp0));
				exec->do_assert(eval->comp_eq(eval->get_sx(2, tmp2), tmp0));
				exec->write_extern_AL(tmp2);
				exec->write_extern_AH(eval->get_tr(1, tmp1));
				exec->write_extern_OF(exec->read_extern_undefined());
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(exec->read_extern_undefined());
				return true;
			}
			break;	// returns false
		}
		case insn__imulb:
		{
			Operand tmp0(1);
			{
				tmp0.write(eval->comp_smul_overflow(exec->read_extern_AL(), exec->read_arg(0)));
				exec->write_extern_AX(eval->comp_cmul(eval->get_sx(2, exec->read_extern_AL()), eval->get_sx(2, exec->read_arg(0))));
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(tmp0);
				exec->write_extern_OF(tmp0);
				return true;
			}
			break;	// returns false
		}
		case insn__iret:
		{
			Operand tmp0(8);
			Operand tmp1(8);
			Operand tmp2(8);
			if(exec->get_osz_bits() == 16)
			{
				exec->do_restore(eval->get_tr(8, eval->get_literal(8, 6)));
				tmp0.write(exec->do_pop());
				tmp1.write(exec->do_pop());
				tmp2.write(exec->do_pop());
				exec->do_push(tmp2);
				exec->do_push(tmp1);
				exec->do_push(tmp0);
				exec->write_extern_CS(eval->get_tr(2, tmp1));
				exec->write_extern_RIP(tmp0);
				exec->write_extern_RFLAGS(tmp2);
				exec->do_discard(eval->get_tr(8, eval->get_literal(8, 6)));
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				exec->do_restore(eval->get_tr(8, eval->get_literal(8, 12)));
				tmp0.write(exec->do_pop());
				tmp1.write(exec->do_pop());
				tmp2.write(exec->do_pop());
				exec->do_push(tmp2);
				exec->do_push(tmp1);
				exec->do_push(tmp0);
				exec->write_extern_CS(eval->get_tr(2, tmp1));
				exec->write_extern_RIP(tmp0);
				exec->write_extern_RFLAGS(tmp2);
				exec->do_discard(eval->get_tr(8, eval->get_literal(8, 12)));
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				exec->do_restore(eval->get_tr(8, eval->get_literal(8, 24)));
				tmp0.write(exec->do_pop());
				tmp1.write(exec->do_pop());
				tmp2.write(exec->do_pop());
				exec->do_push(tmp2);
				exec->do_push(tmp1);
				exec->do_push(tmp0);
				exec->write_extern_CS(eval->get_tr(2, tmp1));
				exec->write_extern_RIP(tmp0);
				exec->write_extern_RFLAGS(tmp2);
				exec->do_discard(eval->get_tr(8, eval->get_literal(8, 24)));
				return true;
			}
			break;	// returns false
		}
		case insn__jcc:
		{
			{
				exec->write_extern_RIPC(eval->comp_quest(eval->comp_xcond(exec->read_arg(1)), eval->get_zx(8, exec->read_arg(0)), eval->get_literal(8, 0)));
				return true;
			}
			break;	// returns false
		}
		case insn__jmpfd:
		{
			{
				exec->write_extern_RIP(eval->get_zx(8, exec->read_arg(0)));
				exec->write_extern_CS(exec->read_arg(1));
				return true;
			}
			break;	// returns false
		}
		case insn__jmpfi:
		{
			{
				exec->write_extern_CS(eval->comp_seg(exec->read_arg(0)));
				exec->write_extern_RIP(eval->get_zx(8, eval->comp_ofs(exec->read_arg(0))));
				return true;
			}
			break;	// returns false
		}
		case insn__jmpi:
		{
			{
				exec->write_extern_RIP(eval->get_zx(8, exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn__jrcxz:
		{
			Operand tmp0(1);
			{
				tmp0.write(eval->comp_zero(exec->read_arg(1)));
				exec->write_extern_RIPC(eval->get_zx(8, eval->comp_quest(tmp0, exec->read_arg(0), eval->get_literal(exec->get_arg_size(0), 0))));
				return true;
			}
			break;	// returns false
		}
		case insn__lods:
		{
			Operand tmp0(4);
			if(exec->get_asz_bits() == 16)
			{
				exec->write_arg(0, exec->read_arg(1));
				tmp0.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				exec->write_extern_SI(eval->comp_add(exec->read_extern_SI(), eval->get_tr(2, tmp0)));
				return true;
			}
			if(exec->get_asz_bits() == 32)
			{
				exec->write_arg(0, exec->read_arg(1));
				tmp0.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				exec->write_extern_ESI(eval->comp_add(exec->read_extern_ESI(), tmp0));
				return true;
			}
			if(exec->get_asz_bits() == 64)
			{
				exec->write_arg(0, exec->read_arg(1));
				tmp0.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				return true;
			}
			break;	// returns false
		}
		case insn__loop:
		{
			Operand tmp0(1);
			{
				exec->write_arg(1, eval->comp_sub(exec->read_arg(1), eval->get_literal(exec->get_arg_size(1), 1)));
				tmp0.write(eval->comp_nz(exec->read_arg(1)));
				exec->write_extern_RIPC(eval->get_zx(8, eval->comp_quest(tmp0, exec->read_arg(0), eval->get_literal(exec->get_arg_size(0), 0))));
				return true;
			}
			break;	// returns false
		}
		case insn__loopnz:
		{
			Operand tmp0(1);
			{
				exec->write_arg(1, eval->comp_sub(exec->read_arg(1), eval->get_literal(exec->get_arg_size(1), 1)));
				tmp0.write(eval->comp_land(eval->comp_lnot(exec->read_extern_ZF()), eval->comp_nz(exec->read_arg(1))));
				exec->write_extern_RIPC(eval->get_zx(8, eval->comp_quest(tmp0, exec->read_arg(0), eval->get_literal(exec->get_arg_size(0), 0))));
				return true;
			}
			break;	// returns false
		}
		case insn__loopz:
		{
			Operand tmp0(1);
			{
				exec->write_arg(1, eval->comp_sub(exec->read_arg(1), eval->get_literal(exec->get_arg_size(1), 1)));
				tmp0.write(eval->comp_land(exec->read_extern_ZF(), eval->comp_nz(exec->read_arg(1))));
				exec->write_extern_RIPC(eval->get_zx(8, eval->comp_quest(tmp0, exec->read_arg(0), eval->get_literal(exec->get_arg_size(0), 0))));
				return true;
			}
			break;	// returns false
		}
		case insn__movs:
		{
			Operand tmp0(4);
			if(exec->get_asz_bits() == 16)
			{
				exec->write_arg(0, exec->read_arg(1));
				tmp0.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				exec->write_extern_SI(eval->comp_add(exec->read_extern_SI(), eval->get_tr(2, tmp0)));
				exec->write_extern_DI(eval->comp_add(exec->read_extern_DI(), eval->get_tr(2, tmp0)));
				return true;
			}
			if(exec->get_asz_bits() == 32)
			{
				exec->write_arg(0, exec->read_arg(1));
				tmp0.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				exec->write_extern_ESI(eval->comp_add(exec->read_extern_ESI(), tmp0));
				exec->write_extern_EDI(eval->comp_add(exec->read_extern_EDI(), tmp0));
				return true;
			}
			if(exec->get_asz_bits() == 64)
			{
				exec->write_arg(0, exec->read_arg(1));
				tmp0.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				return true;
			}
			break;	// returns false
		}
		case insn__movsrv:
		{
			{
				exec->write_arg(0, eval->get_tr(2, exec->read_arg(1)));
				return true;
			}
			break;	// returns false
		}
		case insn__movvsr:
		{
			{
				exec->write_arg(0, eval->get_zx(exec->get_arg_size(0), exec->read_arg(1)));
				return true;
			}
			break;	// returns false
		}
		case insn__mulb:
		{
			Operand tmp0(1);
			{
				tmp0.write(eval->comp_umul_overflow(exec->read_extern_AL(), exec->read_arg(0)));
				exec->write_extern_AX(eval->comp_cmul(eval->get_zx(2, exec->read_extern_AL()), eval->get_zx(2, exec->read_arg(0))));
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(tmp0);
				exec->write_extern_OF(tmp0);
				return true;
			}
			break;	// returns false
		}
		case insn__pop:
		{
			{
				exec->write_arg(0, exec->do_pop());
				return true;
			}
			break;	// returns false
		}
		case insn__popa:
		{
			if(exec->get_osz_bits() == 16)
			{
				exec->write_extern_DI(exec->do_pop());
				exec->write_extern_SI(exec->do_pop());
				exec->write_extern_BP(exec->do_pop());
				exec->do_discard(eval->get_tr(8, eval->get_literal(8, 2)));
				exec->write_extern_BX(exec->do_pop());
				exec->write_extern_DX(exec->do_pop());
				exec->write_extern_CX(exec->do_pop());
				exec->write_extern_AX(exec->do_pop());
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				exec->write_extern_EDI(exec->do_pop());
				exec->write_extern_ESI(exec->do_pop());
				exec->write_extern_EBP(exec->do_pop());
				exec->do_discard(eval->get_tr(8, eval->get_literal(8, 4)));
				exec->write_extern_EBX(exec->do_pop());
				exec->write_extern_EDX(exec->do_pop());
				exec->write_extern_ECX(exec->do_pop());
				exec->write_extern_EAX(exec->do_pop());
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				return true;
			}
			break;	// returns false
		}
		case insn__popf:
		{
			{
				exec->write_extern_RFLAGS(exec->do_pop());
				return true;
			}
			break;	// returns false
		}
		case insn__popsr:
		{
			{
				exec->write_arg(0, exec->do_pop());
				return true;
			}
			break;	// returns false
		}
		case insn__push:
		{
			{
				exec->do_push(exec->read_arg(0));
				return true;
			}
			break;	// returns false
		}
		case insn__pusha:
		{
			if(exec->get_osz_bits() == 16)
			{
				exec->do_push(exec->read_extern_AX());
				exec->do_push(exec->read_extern_CX());
				exec->do_push(exec->read_extern_DX());
				exec->do_push(exec->read_extern_BX());
				exec->do_push(exec->read_extern_SP());
				exec->do_push(exec->read_extern_BP());
				exec->do_push(exec->read_extern_SI());
				exec->do_push(exec->read_extern_DI());
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				exec->do_push(exec->read_extern_EAX());
				exec->do_push(exec->read_extern_ECX());
				exec->do_push(exec->read_extern_EDX());
				exec->do_push(exec->read_extern_EBX());
				exec->do_push(exec->read_extern_ESP());
				exec->do_push(exec->read_extern_EBP());
				exec->do_push(exec->read_extern_ESI());
				exec->do_push(exec->read_extern_EDI());
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				return true;
			}
			break;	// returns false
		}
		case insn__pushf:
		{
			{
				exec->do_push(exec->read_extern_RFLAGS());
				return true;
			}
			break;	// returns false
		}
		case insn__pushsr:
		{
			{
				exec->do_push(exec->read_arg(0));
				return true;
			}
			break;	// returns false
		}
		case insn__ret:
		{
			{
				exec->write_extern_RIP(exec->do_pop());
				return true;
			}
			break;	// returns false
		}
		case insn__retf:
		{
			Operand tmp0(2);
			Operand tmp1(8);
			Operand tmp2(8);
			if(exec->get_osz_bits() == 16)
			{
				tmp0.write(eval->get_literal(2, 0));
				exec->do_restore(eval->comp_add(eval->get_tr(8, eval->get_literal(8, 4)), eval->get_zx(8, tmp0)));
				tmp1.write(exec->do_pop());
				tmp2.write(exec->do_pop());
				exec->do_push(tmp2);
				exec->do_push(tmp1);
				exec->write_extern_CS(eval->get_tr(2, tmp2));
				exec->write_extern_RIP(tmp1);
				exec->do_discard(eval->get_tr(8, eval->get_literal(8, 4)));
				exec->do_discard(eval->get_zx(8, tmp0));
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				tmp0.write(eval->get_literal(2, 0));
				exec->do_restore(eval->comp_add(eval->get_tr(8, eval->get_literal(8, 8)), eval->get_zx(8, tmp0)));
				tmp1.write(exec->do_pop());
				tmp2.write(exec->do_pop());
				exec->do_push(tmp2);
				exec->do_push(tmp1);
				exec->write_extern_CS(eval->get_tr(2, tmp2));
				exec->write_extern_RIP(tmp1);
				exec->do_discard(eval->get_tr(8, eval->get_literal(8, 8)));
				exec->do_discard(eval->get_zx(8, tmp0));
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				tmp0.write(eval->get_literal(2, 0));
				exec->do_restore(eval->comp_add(eval->get_tr(8, eval->get_literal(8, 16)), eval->get_zx(8, tmp0)));
				tmp1.write(exec->do_pop());
				tmp2.write(exec->do_pop());
				exec->do_push(tmp2);
				exec->do_push(tmp1);
				exec->write_extern_CS(eval->get_tr(2, tmp2));
				exec->write_extern_RIP(tmp1);
				exec->do_discard(eval->get_tr(8, eval->get_literal(8, 16)));
				exec->do_discard(eval->get_zx(8, tmp0));
				return true;
			}
			break;	// returns false
		}
		case insn__retfnum:
		{
			Operand tmp0(8);
			Operand tmp1(8);
			if(exec->get_osz_bits() == 16)
			{
				exec->do_restore(eval->comp_add(eval->get_tr(8, eval->get_literal(8, 4)), eval->get_zx(8, exec->read_arg(0))));
				tmp0.write(exec->do_pop());
				tmp1.write(exec->do_pop());
				exec->do_push(tmp1);
				exec->do_push(tmp0);
				exec->write_extern_CS(eval->get_tr(2, tmp1));
				exec->write_extern_RIP(tmp0);
				exec->do_discard(eval->get_tr(8, eval->get_literal(8, 4)));
				exec->do_discard(eval->get_zx(8, exec->read_arg(0)));
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				exec->do_restore(eval->comp_add(eval->get_tr(8, eval->get_literal(8, 8)), eval->get_zx(8, exec->read_arg(0))));
				tmp0.write(exec->do_pop());
				tmp1.write(exec->do_pop());
				exec->do_push(tmp1);
				exec->do_push(tmp0);
				exec->write_extern_CS(eval->get_tr(2, tmp1));
				exec->write_extern_RIP(tmp0);
				exec->do_discard(eval->get_tr(8, eval->get_literal(8, 8)));
				exec->do_discard(eval->get_zx(8, exec->read_arg(0)));
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				exec->do_restore(eval->comp_add(eval->get_tr(8, eval->get_literal(8, 16)), eval->get_zx(8, exec->read_arg(0))));
				tmp0.write(exec->do_pop());
				tmp1.write(exec->do_pop());
				exec->do_push(tmp1);
				exec->do_push(tmp0);
				exec->write_extern_CS(eval->get_tr(2, tmp1));
				exec->write_extern_RIP(tmp0);
				exec->do_discard(eval->get_tr(8, eval->get_literal(8, 16)));
				exec->do_discard(eval->get_zx(8, exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn__retnum:
		{
			{
				exec->write_extern_RIP(exec->do_pop());
				exec->do_discard(eval->get_zx(8, exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn__sal:
		{
			Operand tmp0(1);
			Operand tmp1(1);
			Operand tmp2(exec->get_arg_size(0) * 1);
			{
				tmp0.write(eval->comp_and(exec->read_arg(1), eval->get_literal(1, 31)));
				tmp1.write(eval->comp_zero(tmp0));
				tmp2.write(exec->read_arg(0));
				exec->write_arg(0, eval->comp_shl(exec->read_arg(0), tmp0));
				exec->write_extern_CF(eval->comp_bitquest(tmp1, exec->read_extern_CF(), eval->comp_shl_cf(tmp2, tmp0)));
				exec->write_extern_OF(eval->comp_bitquest(eval->comp_eq(tmp0, eval->get_literal(1, 1)), eval->comp_lxor(eval->comp_sign(exec->read_arg(0)), exec->read_extern_CF()), eval->comp_bitquest(tmp1, exec->read_extern_OF(), exec->read_extern_undefined())));
				exec->write_extern_SF(eval->comp_bitquest(tmp1, exec->read_extern_SF(), eval->comp_sign(exec->read_arg(0))));
				exec->write_extern_ZF(eval->comp_bitquest(tmp1, exec->read_extern_ZF(), eval->comp_zero(exec->read_arg(0))));
				exec->write_extern_AF(eval->comp_bitquest(tmp1, exec->read_extern_AF(), exec->read_extern_undefined()));
				exec->write_extern_PF(eval->comp_bitquest(tmp1, exec->read_extern_PF(), eval->comp_parity(exec->read_arg(0))));
				return true;
			}
			break;	// returns false
		}
		case insn__scas:
		{
			Operand tmp0(exec->get_arg_size(0) * 1);
			Operand tmp1(4);
			if(exec->get_asz_bits() == 16)
			{
				exec->write_extern_OF(eval->comp_sub_of(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_AF(eval->comp_sub_af(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_CF(eval->comp_sub_cf(exec->read_arg(0), exec->read_arg(1)));
				tmp0.write(eval->comp_sub(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(tmp0));
				exec->write_extern_ZF(eval->comp_zero(tmp0));
				exec->write_extern_PF(eval->comp_parity(tmp0));
				tmp1.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				exec->write_extern_DI(eval->comp_add(exec->read_extern_DI(), eval->get_tr(2, tmp1)));
				return true;
			}
			if(exec->get_asz_bits() == 32)
			{
				exec->write_extern_OF(eval->comp_sub_of(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_AF(eval->comp_sub_af(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_CF(eval->comp_sub_cf(exec->read_arg(0), exec->read_arg(1)));
				tmp0.write(eval->comp_sub(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(tmp0));
				exec->write_extern_ZF(eval->comp_zero(tmp0));
				exec->write_extern_PF(eval->comp_parity(tmp0));
				tmp1.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				exec->write_extern_EDI(eval->comp_add(exec->read_extern_EDI(), tmp1));
				return true;
			}
			if(exec->get_asz_bits() == 64)
			{
				exec->write_extern_OF(eval->comp_sub_of(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_AF(eval->comp_sub_af(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_CF(eval->comp_sub_cf(exec->read_arg(0), exec->read_arg(1)));
				tmp0.write(eval->comp_sub(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(tmp0));
				exec->write_extern_ZF(eval->comp_zero(tmp0));
				exec->write_extern_PF(eval->comp_parity(tmp0));
				tmp1.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				return true;
			}
			break;	// returns false
		}
		case insn__setcc:
		{
			{
				exec->write_arg(0, eval->comp_bitzx(eval->comp_xcond(exec->read_arg(1))));
				return true;
			}
			break;	// returns false
		}
		case insn__stos:
		{
			Operand tmp0(4);
			if(exec->get_asz_bits() == 16)
			{
				exec->write_arg(0, exec->read_arg(1));
				tmp0.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				exec->write_extern_DI(eval->comp_add(exec->read_extern_DI(), eval->get_tr(2, tmp0)));
				return true;
			}
			if(exec->get_asz_bits() == 32)
			{
				exec->write_arg(0, exec->read_arg(1));
				tmp0.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				exec->write_extern_EDI(eval->comp_add(exec->read_extern_EDI(), tmp0));
				return true;
			}
			if(exec->get_asz_bits() == 64)
			{
				exec->write_arg(0, exec->read_arg(1));
				tmp0.write(eval->comp_quest(eval->comp_lnot(exec->read_extern_DF()), eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->comp_neg(eval->get_tr(4, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */))));
				return true;
			}
			break;	// returns false
		}
		case insn__sxacc:
		{
			if(exec->get_osz_bits() == 16)
			{
				exec->write_extern_AX(eval->get_sx(2, exec->read_extern_AL()));
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				exec->write_extern_EAX(eval->get_sx(4, exec->read_extern_AX()));
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				return true;
			}
			break;	// returns false
		}
		case insn__sxdax:
		{
			if(exec->get_osz_bits() == 16)
			{
				exec->write_extern_DX(eval->comp_quest(eval->comp_sign(exec->read_extern_AX()), eval->get_tr(2, eval->get_literal(2, 0xffff)), eval->get_tr(2, eval->get_literal(2, 0))));
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				exec->write_extern_EDX(eval->comp_quest(eval->comp_sign(exec->read_extern_EAX()), eval->get_tr(4, eval->get_literal(4, 0xffffffff)), eval->get_tr(4, eval->get_literal(4, 0))));
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				return true;
			}
			break;	// returns false
		}
		case insn__xchg:
		{
			Operand tmp0(exec->get_arg_size(0) * 1);
			{
				tmp0.write(exec->read_arg(0));
				exec->write_arg(0, exec->read_arg(1));
				exec->write_arg(1, tmp0);
				return true;
			}
			break;	// returns false
		}
		case insn__xlat:
		{
			{
				exec->write_extern_AL(exec->read_arg(0));
				return true;
			}
			break;	// returns false
		}
		case insn_aaa:
		{
			{
				exec->write_extern_AF(eval->comp_lor(exec->read_extern_AF(), eval->comp_gtu(eval->comp_and(exec->read_extern_AL(), eval->get_literal(1, 0x0f)), eval->get_literal(1, 0x09))));
				exec->write_extern_CF(exec->read_extern_AF());
				exec->write_extern_AX(eval->comp_add(exec->read_extern_AX(), eval->comp_quest(exec->read_extern_AF(), eval->get_tr(2, eval->get_literal(2, 0x0106)), eval->get_tr(2, eval->get_literal(2, 0)))));
				exec->write_extern_AL(eval->comp_and(exec->read_extern_AL(), eval->get_literal(1, 0x0f)));
				exec->write_extern_OF(eval->get_literal(1, 0));
				exec->write_extern_SF(eval->get_literal(1, 0));
				exec->write_extern_ZF(eval->comp_zero(exec->read_extern_AL()));
				exec->write_extern_PF(eval->comp_parity(exec->read_extern_AL()));
				return true;
			}
			break;	// returns false
		}
		case insn_aas:
		{
			{
				exec->write_extern_AF(eval->comp_lor(exec->read_extern_AF(), eval->comp_gtu(eval->comp_and(exec->read_extern_AL(), eval->get_literal(1, 0x0f)), eval->get_literal(1, 0x09))));
				exec->write_extern_CF(exec->read_extern_AF());
				exec->write_extern_AX(eval->comp_sub(exec->read_extern_AX(), eval->comp_quest(exec->read_extern_AF(), eval->get_tr(2, eval->get_literal(2, 0x0106)), eval->get_tr(2, eval->get_literal(2, 0)))));
				exec->write_extern_AL(eval->comp_and(exec->read_extern_AL(), eval->get_literal(1, 0x0f)));
				exec->write_extern_OF(eval->get_literal(1, 0));
				exec->write_extern_SF(eval->get_literal(1, 0));
				exec->write_extern_ZF(eval->comp_zero(exec->read_extern_AL()));
				exec->write_extern_PF(eval->comp_parity(exec->read_extern_AL()));
				return true;
			}
			break;	// returns false
		}
		case insn_adc:
		{
			Operand tmp0(1);
			Operand tmp1(exec->get_arg_size(0) * 1);
			{
				tmp0.write(exec->read_extern_CF());
				exec->write_extern_OF(eval->comp_adc_of(exec->read_arg(0), exec->read_arg(1), tmp0));
				exec->write_extern_AF(eval->comp_adc_af(exec->read_arg(0), exec->read_arg(1), tmp0));
				exec->write_extern_CF(eval->comp_adc_cf(exec->read_arg(0), exec->read_arg(1), tmp0));
				tmp1.write(eval->comp_add(exec->read_arg(0), exec->read_arg(1)));
				exec->write_arg(0, eval->comp_quest(tmp0, eval->comp_add(tmp1, eval->get_literal(exec->get_arg_size(0), 1)), tmp1));
				exec->write_extern_SF(eval->comp_sign(exec->read_arg(0)));
				exec->write_extern_ZF(eval->comp_zero(exec->read_arg(0)));
				exec->write_extern_PF(eval->comp_parity(exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_add:
		{
			{
				exec->write_extern_OF(eval->comp_add_of(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_AF(eval->comp_add_af(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_CF(eval->comp_add_cf(exec->read_arg(0), exec->read_arg(1)));
				exec->write_arg(0, eval->comp_add(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(exec->read_arg(0)));
				exec->write_extern_ZF(eval->comp_zero(exec->read_arg(0)));
				exec->write_extern_PF(eval->comp_parity(exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_and:
		{
			{
				exec->write_extern_OF(eval->get_literal(1, 0));
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_CF(eval->get_literal(1, 0));
				exec->write_arg(0, eval->comp_and(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(exec->read_arg(0)));
				exec->write_extern_ZF(eval->comp_zero(exec->read_arg(0)));
				exec->write_extern_PF(eval->comp_parity(exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_call:
		{
			{
				exec->do_push(exec->read_extern_RIP1());
				exec->write_extern_RIPC(eval->get_zx(8, exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_clc:
		{
			{
				exec->write_extern_CF(eval->get_literal(1, 0));
				return true;
			}
			break;	// returns false
		}
		case insn_cld:
		{
			{
				exec->write_extern_DF(eval->get_literal(1, 0));
				return true;
			}
			break;	// returns false
		}
		case insn_cli:
		{
			{
				exec->write_extern_IF(eval->get_literal(1, 0));
				return true;
			}
			break;	// returns false
		}
		case insn_cmc:
		{
			{
				exec->write_extern_CF(eval->comp_lnot(exec->read_extern_CF()));
				return true;
			}
			break;	// returns false
		}
		case insn_cmp:
		{
			Operand tmp0(exec->get_arg_size(0) * 1);
			{
				exec->write_extern_OF(eval->comp_sub_of(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_AF(eval->comp_sub_af(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_CF(eval->comp_sub_cf(exec->read_arg(0), exec->read_arg(1)));
				tmp0.write(eval->comp_sub(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(tmp0));
				exec->write_extern_ZF(eval->comp_zero(tmp0));
				exec->write_extern_PF(eval->comp_parity(tmp0));
				return true;
			}
			break;	// returns false
		}
		case insn_daa:
		{
			Operand tmp0(1);
			Operand tmp1(1);
			Operand tmp2(1);
			Operand tmp3(1);
			Operand tmp4(1);
			{
				tmp0.write(exec->read_extern_AL());
				exec->write_extern_AF(eval->comp_lor(exec->read_extern_AF(), eval->comp_gtu(eval->comp_and(tmp0, eval->get_literal(1, 0x0f)), eval->get_literal(1, 0x09))));
				tmp1.write(eval->comp_quest(exec->read_extern_AF(), eval->get_tr(1, eval->get_literal(1, 0x06)), eval->get_tr(1, eval->get_literal(1, 0))));
				tmp2.write(eval->comp_bitquest(exec->read_extern_AF(), eval->comp_lor(exec->read_extern_CF(), eval->comp_gtu(exec->read_extern_AL(), eval->get_literal(1, 0xf9))), eval->get_literal(1, 0)));
				exec->write_extern_AL(eval->comp_add(exec->read_extern_AL(), tmp1));
				tmp3.write(eval->comp_lor(exec->read_extern_CF(), eval->comp_gtu(tmp0, eval->get_literal(1, 0x99))));
				tmp4.write(eval->comp_quest(tmp3, eval->get_tr(1, eval->get_literal(1, 0x60)), eval->get_tr(1, eval->get_literal(1, 0))));
				exec->write_extern_AL(eval->comp_add(exec->read_extern_AL(), tmp4));
				tmp2.write(tmp3);
				exec->write_extern_OF(eval->get_literal(1, 0));
				exec->write_extern_SF(eval->comp_sign(exec->read_extern_AL()));
				exec->write_extern_ZF(eval->comp_zero(exec->read_extern_AL()));
				exec->write_extern_PF(eval->comp_parity(exec->read_extern_AL()));
				exec->write_extern_CF(tmp2);
				return true;
			}
			break;	// returns false
		}
		case insn_das:
		{
			Operand tmp0(1);
			Operand tmp1(1);
			Operand tmp2(1);
			Operand tmp3(1);
			Operand tmp4(1);
			{
				tmp0.write(exec->read_extern_AL());
				exec->write_extern_AF(eval->comp_lor(exec->read_extern_AF(), eval->comp_gtu(eval->comp_and(tmp0, eval->get_literal(1, 0x0f)), eval->get_literal(1, 0x09))));
				tmp1.write(eval->comp_quest(exec->read_extern_AF(), eval->get_tr(1, eval->get_literal(1, 0x06)), eval->get_tr(1, eval->get_literal(1, 0))));
				tmp2.write(eval->comp_bitquest(exec->read_extern_AF(), eval->comp_lor(exec->read_extern_CF(), eval->comp_ltu(exec->read_extern_AL(), eval->get_literal(1, 0x06))), eval->get_literal(1, 0)));
				exec->write_extern_AL(eval->comp_sub(exec->read_extern_AL(), tmp1));
				tmp3.write(eval->comp_lor(exec->read_extern_CF(), eval->comp_gtu(tmp0, eval->get_literal(1, 0x99))));
				tmp4.write(eval->comp_quest(tmp3, eval->get_tr(1, eval->get_literal(1, 0x60)), eval->get_tr(1, eval->get_literal(1, 0))));
				exec->write_extern_AL(eval->comp_sub(exec->read_extern_AL(), tmp4));
				tmp2.write(eval->comp_lor(tmp2, tmp3));
				exec->write_extern_OF(eval->get_literal(1, 0));
				exec->write_extern_SF(eval->comp_sign(exec->read_extern_AL()));
				exec->write_extern_ZF(eval->comp_zero(exec->read_extern_AL()));
				exec->write_extern_PF(eval->comp_parity(exec->read_extern_AL()));
				exec->write_extern_CF(tmp2);
				return true;
			}
			break;	// returns false
		}
		case insn_dec:
		{
			{
				exec->write_extern_OF(eval->comp_sub_of(exec->read_arg(0), eval->get_tr(exec->get_arg_size(0), eval->get_literal(exec->get_arg_size(0), 1))));
				exec->write_extern_AF(eval->comp_sub_af(exec->read_arg(0), eval->get_tr(exec->get_arg_size(0), eval->get_literal(exec->get_arg_size(0), 1))));
				exec->write_arg(0, eval->comp_sub(exec->read_arg(0), eval->get_tr(exec->get_arg_size(0), eval->get_literal(exec->get_arg_size(0), 1))));
				exec->write_extern_SF(eval->comp_sign(exec->read_arg(0)));
				exec->write_extern_ZF(eval->comp_zero(exec->read_arg(0)));
				exec->write_extern_PF(eval->comp_parity(exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_div:
		{
			Operand tmp0(exec->get_arg_size(0) * 2);
			Operand tmp1(exec->get_arg_size(0) * 2);
			Operand tmp2(exec->get_arg_size(0) * 2);
			Operand tmp3(exec->get_arg_size(0) * 1);
			if(exec->get_osz_bits() == 16)
			{
				exec->do_assert(eval->comp_nz(exec->read_arg(0)));
				tmp0.write(eval->comp_or(eval->comp_shl(eval->get_zx(exec->get_arg_size(0) * 2, exec->read_extern_DX()), eval->get_literal(1, 16)), eval->get_zx(exec->get_arg_size(0) * 2, exec->read_extern_AX())));
				tmp1.write(eval->comp_udiv(tmp0, eval->get_zx(exec->get_arg_size(0) * 2, exec->read_arg(0))));
				tmp2.write(eval->comp_umod(tmp0, eval->get_zx(exec->get_arg_size(0) * 2, exec->read_arg(0))));
				tmp3.write(eval->get_tr(exec->get_arg_size(0), tmp1));
				exec->do_assert(eval->comp_eq(eval->get_zx(exec->get_arg_size(0) * 2, tmp3), tmp1));
				exec->write_extern_AX(eval->get_tr(2, tmp3));
				exec->write_extern_DX(eval->get_tr(2, tmp2));
				exec->write_extern_OF(exec->read_extern_undefined());
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(exec->read_extern_undefined());
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				exec->do_assert(eval->comp_nz(exec->read_arg(0)));
				tmp0.write(eval->comp_or(eval->comp_shl(eval->get_zx(exec->get_arg_size(0) * 2, exec->read_extern_EDX()), eval->get_literal(1, 32)), eval->get_zx(exec->get_arg_size(0) * 2, exec->read_extern_EAX())));
				tmp1.write(eval->comp_udiv(tmp0, eval->get_zx(exec->get_arg_size(0) * 2, exec->read_arg(0))));
				tmp2.write(eval->comp_umod(tmp0, eval->get_zx(exec->get_arg_size(0) * 2, exec->read_arg(0))));
				tmp3.write(eval->get_tr(exec->get_arg_size(0), tmp1));
				exec->do_assert(eval->comp_eq(eval->get_zx(exec->get_arg_size(0) * 2, tmp3), tmp1));
				exec->write_extern_EAX(eval->get_tr(4, tmp3));
				exec->write_extern_EDX(eval->get_tr(4, tmp2));
				exec->write_extern_OF(exec->read_extern_undefined());
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(exec->read_extern_undefined());
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				exec->do_assert(eval->comp_nz(exec->read_arg(0)));
				exec->write_extern_OF(exec->read_extern_undefined());
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(exec->read_extern_undefined());
				return true;
			}
			break;	// returns false
		}
		case insn_enter:
		{
			{
				exec->do_assert(eval->comp_eq(exec->read_arg(1), eval->get_literal(1, 0)));
				exec->do_push(exec->read_extern_RBP());
				exec->write_extern_RBPX(exec->read_extern_RSP());
				exec->do_commit(eval->get_zx(8, exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_idiv:
		{
			Operand tmp0(exec->get_arg_size(0) * 2);
			Operand tmp1(exec->get_arg_size(0) * 2);
			Operand tmp2(exec->get_arg_size(0) * 2);
			Operand tmp3(exec->get_arg_size(0) * 1);
			if(exec->get_osz_bits() == 16)
			{
				exec->do_assert(eval->comp_nz(exec->read_arg(0)));
				tmp0.write(eval->comp_or(eval->comp_shl(eval->get_zx(exec->get_arg_size(0) * 2, exec->read_extern_DX()), eval->get_literal(1, 16)), eval->get_zx(exec->get_arg_size(0) * 2, exec->read_extern_AX())));
				tmp1.write(eval->comp_sdiv(tmp0, eval->get_sx(exec->get_arg_size(0) * 2, exec->read_arg(0))));
				tmp2.write(eval->comp_smod(tmp0, eval->get_sx(exec->get_arg_size(0) * 2, exec->read_arg(0))));
				tmp3.write(eval->get_tr(exec->get_arg_size(0), tmp1));
				exec->do_assert(eval->comp_eq(eval->get_sx(exec->get_arg_size(0) * 2, tmp3), tmp1));
				exec->write_extern_AX(eval->get_tr(2, tmp3));
				exec->write_extern_DX(eval->get_tr(2, tmp2));
				exec->write_extern_OF(exec->read_extern_undefined());
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(exec->read_extern_undefined());
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				exec->do_assert(eval->comp_nz(exec->read_arg(0)));
				tmp0.write(eval->comp_or(eval->comp_shl(eval->get_zx(exec->get_arg_size(0) * 2, exec->read_extern_EDX()), eval->get_literal(1, 32)), eval->get_zx(exec->get_arg_size(0) * 2, exec->read_extern_EAX())));
				tmp1.write(eval->comp_sdiv(tmp0, eval->get_sx(exec->get_arg_size(0) * 2, exec->read_arg(0))));
				tmp2.write(eval->comp_smod(tmp0, eval->get_sx(exec->get_arg_size(0) * 2, exec->read_arg(0))));
				tmp3.write(eval->get_tr(exec->get_arg_size(0), tmp1));
				exec->do_assert(eval->comp_eq(eval->get_sx(exec->get_arg_size(0) * 2, tmp3), tmp1));
				exec->write_extern_EAX(eval->get_tr(4, tmp3));
				exec->write_extern_EDX(eval->get_tr(4, tmp2));
				exec->write_extern_OF(exec->read_extern_undefined());
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(exec->read_extern_undefined());
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				exec->do_assert(eval->comp_nz(exec->read_arg(0)));
				exec->write_extern_OF(exec->read_extern_undefined());
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(exec->read_extern_undefined());
				return true;
			}
			break;	// returns false
		}
		case insn_imul:
		{
			Operand tmp0(1);
			Operand tmp1(exec->get_arg_size(0) * 2);
			if(exec->get_osz_bits() == 16)
			{
				tmp0.write(eval->comp_smul_overflow(exec->read_extern_AX(), eval->get_zx(2, exec->read_arg(0))));
				tmp1.write(eval->comp_cmul(eval->get_sx(exec->get_arg_size(0) * 2, exec->read_extern_AX()), eval->get_sx(exec->get_arg_size(0) * 2, exec->read_arg(0))));
				exec->write_extern_AX(eval->get_tr(2, tmp1));
				exec->write_extern_DX(eval->get_tr(2, eval->comp_shru(tmp1, eval->get_literal(1, 16))));
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(tmp0);
				exec->write_extern_OF(tmp0);
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				tmp0.write(eval->comp_smul_overflow(exec->read_extern_EAX(), eval->get_zx(4, exec->read_arg(0))));
				tmp1.write(eval->comp_cmul(eval->get_sx(exec->get_arg_size(0) * 2, exec->read_extern_EAX()), eval->get_sx(exec->get_arg_size(0) * 2, exec->read_arg(0))));
				exec->write_extern_EAX(eval->get_tr(4, tmp1));
				exec->write_extern_EDX(eval->get_tr(4, eval->comp_shru(tmp1, eval->get_literal(1, 32))));
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(tmp0);
				exec->write_extern_OF(tmp0);
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(tmp0);
				exec->write_extern_OF(tmp0);
				return true;
			}
			break;	// returns false
		}
		case insn_in:
		{
			{
				exec->write_arg(0, exec->do_inport(exec->get_arg_size(0), exec->read_arg(1)));
				return true;
			}
			break;	// returns false
		}
		case insn_inc:
		{
			{
				exec->write_extern_OF(eval->comp_add_of(exec->read_arg(0), eval->get_tr(exec->get_arg_size(0), eval->get_literal(exec->get_arg_size(0), 1))));
				exec->write_extern_AF(eval->comp_add_af(exec->read_arg(0), eval->get_tr(exec->get_arg_size(0), eval->get_literal(exec->get_arg_size(0), 1))));
				exec->write_arg(0, eval->comp_add(exec->read_arg(0), eval->get_tr(exec->get_arg_size(0), eval->get_literal(exec->get_arg_size(0), 1))));
				exec->write_extern_SF(eval->comp_sign(exec->read_arg(0)));
				exec->write_extern_ZF(eval->comp_zero(exec->read_arg(0)));
				exec->write_extern_PF(eval->comp_parity(exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_int:
		{
			{
				exec->write_extern_RINT(exec->read_arg(0));
				return true;
			}
			break;	// returns false
		}
		case insn_jmp:
		{
			{
				exec->write_extern_RIPC(eval->get_zx(8, exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_lahf:
		{
			{
				exec->write_extern_AH(eval->get_literal(1, 2));
				exec->write_extern_AH(eval->comp_or(exec->read_extern_AH(), eval->comp_quest(exec->read_extern_CF(), eval->get_literal(1, 1), eval->get_tr(1, eval->get_literal(1, 0)))));
				exec->write_extern_AH(eval->comp_or(exec->read_extern_AH(), eval->comp_quest(exec->read_extern_PF(), eval->get_literal(1, 4), eval->get_tr(1, eval->get_literal(1, 0)))));
				exec->write_extern_AH(eval->comp_or(exec->read_extern_AH(), eval->comp_quest(exec->read_extern_AF(), eval->get_literal(1, 16), eval->get_tr(1, eval->get_literal(1, 0)))));
				exec->write_extern_AH(eval->comp_or(exec->read_extern_AH(), eval->comp_quest(exec->read_extern_ZF(), eval->get_literal(1, 64), eval->get_tr(1, eval->get_literal(1, 0)))));
				exec->write_extern_AH(eval->comp_or(exec->read_extern_AH(), eval->comp_quest(exec->read_extern_SF(), eval->get_literal(1, 128), eval->get_tr(1, eval->get_literal(1, 0)))));
				return true;
			}
			break;	// returns false
		}
		case insn_lds:
		{
			{
				exec->write_extern_DS(eval->comp_seg(exec->read_arg(1)));
				exec->write_arg(0, eval->comp_ofs(exec->read_arg(1)));
				return true;
			}
			break;	// returns false
		}
		case insn_lea:
		{
			{
				exec->write_arg(0, eval->get_zx(exec->get_arg_size(0), exec->read_arg(1)));
				return true;
			}
			break;	// returns false
		}
		case insn_leave:
		{
			{
				exec->write_extern_RSPX(exec->read_extern_RBP());
				exec->write_extern_RBPX(exec->do_pop());
				return true;
			}
			break;	// returns false
		}
		case insn_les:
		{
			{
				exec->write_extern_ES(eval->comp_seg(exec->read_arg(1)));
				exec->write_arg(0, eval->comp_ofs(exec->read_arg(1)));
				return true;
			}
			break;	// returns false
		}
		case insn_lfs:
		{
			{
				exec->write_extern_FS(eval->comp_seg(exec->read_arg(1)));
				exec->write_arg(0, eval->comp_ofs(exec->read_arg(1)));
				return true;
			}
			break;	// returns false
		}
		case insn_lgs:
		{
			{
				exec->write_extern_GS(eval->comp_seg(exec->read_arg(1)));
				exec->write_arg(0, eval->comp_ofs(exec->read_arg(1)));
				return true;
			}
			break;	// returns false
		}
		case insn_lss:
		{
			{
				exec->write_extern_SS(eval->comp_seg(exec->read_arg(1)));
				exec->write_arg(0, eval->comp_ofs(exec->read_arg(1)));
				return true;
			}
			break;	// returns false
		}
		case insn_mov:
		{
			{
				exec->write_arg(0, exec->read_arg(1));
				return true;
			}
			break;	// returns false
		}
		case insn_movsx:
		{
			{
				exec->write_arg(0, eval->get_sx(exec->get_arg_size(0), exec->read_arg(1)));
				return true;
			}
			break;	// returns false
		}
		case insn_movzx:
		{
			{
				exec->write_arg(0, eval->get_zx(exec->get_arg_size(0), exec->read_arg(1)));
				return true;
			}
			break;	// returns false
		}
		case insn_mul:
		{
			Operand tmp0(exec->get_arg_size(0) * 2);
			Operand tmp1(1);
			if(exec->get_osz_bits() == 16)
			{
				tmp0.write(eval->comp_cmul(eval->get_zx(exec->get_arg_size(0) * 2, exec->read_extern_AX()), eval->get_zx(exec->get_arg_size(0) * 2, exec->read_arg(0))));
				tmp1.write(eval->comp_umul_overflow(exec->read_extern_AX(), eval->get_zx(2, exec->read_arg(0))));
				exec->write_extern_AX(eval->get_tr(2, tmp0));
				exec->write_extern_DX(eval->get_tr(2, eval->comp_shru(tmp0, eval->get_literal(1, 16))));
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(tmp1);
				exec->write_extern_OF(tmp1);
				return true;
			}
			if(exec->get_osz_bits() == 32)
			{
				tmp0.write(eval->comp_cmul(eval->get_zx(exec->get_arg_size(0) * 2, exec->read_extern_EAX()), eval->get_zx(exec->get_arg_size(0) * 2, exec->read_arg(0))));
				tmp1.write(eval->comp_umul_overflow(exec->read_extern_EAX(), eval->get_zx(4, exec->read_arg(0))));
				exec->write_extern_EAX(eval->get_tr(4, tmp0));
				exec->write_extern_EDX(eval->get_tr(4, eval->comp_shru(tmp0, eval->get_literal(1, 32))));
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(tmp1);
				exec->write_extern_OF(tmp1);
				return true;
			}
			if(exec->get_osz_bits() == 64)
			{
				exec->write_extern_SF(exec->read_extern_undefined());
				exec->write_extern_ZF(exec->read_extern_undefined());
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_PF(exec->read_extern_undefined());
				exec->write_extern_CF(tmp1);
				exec->write_extern_OF(tmp1);
				return true;
			}
			break;	// returns false
		}
		case insn_neg:
		{
			{
				exec->write_extern_OF(eval->comp_sub_of(eval->get_tr(exec->get_arg_size(0), eval->get_literal(exec->get_arg_size(0), 0)), exec->read_arg(0)));
				exec->write_extern_AF(eval->comp_sub_af(eval->get_tr(exec->get_arg_size(0), eval->get_literal(exec->get_arg_size(0), 0)), exec->read_arg(0)));
				exec->write_extern_CF(eval->comp_sub_cf(eval->get_tr(exec->get_arg_size(0), eval->get_literal(exec->get_arg_size(0), 0)), exec->read_arg(0)));
				exec->write_arg(0, eval->comp_neg(exec->read_arg(0)));
				exec->write_extern_SF(eval->comp_sign(exec->read_arg(0)));
				exec->write_extern_ZF(eval->comp_zero(exec->read_arg(0)));
				exec->write_extern_PF(eval->comp_parity(exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_not:
		{
			{
				exec->write_arg(0, eval->comp_not(exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_or:
		{
			{
				exec->write_extern_OF(eval->get_literal(1, 0));
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_CF(eval->get_literal(1, 0));
				exec->write_arg(0, eval->comp_or(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(exec->read_arg(0)));
				exec->write_extern_ZF(eval->comp_zero(exec->read_arg(0)));
				exec->write_extern_PF(eval->comp_parity(exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_out:
		{
			{
exec->do_outport(exec->read_arg(0), exec->read_arg(1));				return true;
			}
			break;	// returns false
		}
		case insn_rcl:
		{
			Operand tmp0(1);
			Operand tmp1(1);
			Operand tmp2(1);
			Operand tmp3(1);
			Operand tmp4(exec->get_arg_size(0) * 1);
			{
				tmp0.write(exec->read_arg(1));
				tmp1.write(eval->comp_and(tmp0, eval->get_literal(1, 31)));
				tmp2.write(eval->comp_add(eval->comp_shl(eval->get_tr(1, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->get_literal(1, 3)), eval->get_literal(1, 1)));
				tmp3.write(eval->comp_umod(tmp1, tmp2));
				tmp4.write(exec->read_arg(0));
				exec->write_arg(0, eval->comp_rcl(exec->read_arg(0), tmp3, exec->read_extern_CF()));
				exec->write_extern_CF(eval->comp_rcl_cf(tmp4, tmp3, exec->read_extern_CF()));
				exec->write_extern_OF(eval->comp_bitquest(eval->comp_zero(tmp3), exec->read_extern_OF(), eval->comp_bitquest(eval->comp_eq(tmp0, eval->get_literal(1, 1)), eval->comp_lxor(eval->comp_sign(exec->read_arg(0)), exec->read_extern_CF()), exec->read_extern_undefined())));
				return true;
			}
			break;	// returns false
		}
		case insn_rcr:
		{
			Operand tmp0(1);
			Operand tmp1(1);
			Operand tmp2(1);
			Operand tmp3(1);
			Operand tmp4(exec->get_arg_size(0) * 1);
			{
				tmp0.write(exec->read_arg(1));
				tmp1.write(eval->comp_and(tmp0, eval->get_literal(1, 31)));
				tmp2.write(eval->comp_add(eval->comp_shl(eval->get_tr(1, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->get_literal(1, 3)), eval->get_literal(1, 1)));
				tmp3.write(eval->comp_umod(tmp1, tmp2));
				exec->write_extern_OF(eval->comp_bitquest(eval->comp_zero(tmp3), exec->read_extern_OF(), eval->comp_bitquest(eval->comp_eq(tmp0, eval->get_literal(1, 1)), eval->comp_lxor(eval->comp_sign(exec->read_arg(0)), exec->read_extern_CF()), exec->read_extern_undefined())));
				tmp4.write(exec->read_arg(0));
				exec->write_arg(0, eval->comp_rcr(exec->read_arg(0), tmp3, exec->read_extern_CF()));
				exec->write_extern_CF(eval->comp_rcr_cf(tmp4, tmp3, exec->read_extern_CF()));
				return true;
			}
			break;	// returns false
		}
		case insn_rol:
		{
			Operand tmp0(1);
			Operand tmp1(1);
			Operand tmp2(1);
			Operand tmp3(1);
			Operand tmp4(1);
			{
				tmp0.write(exec->read_arg(1));
				tmp1.write(eval->comp_shl(eval->get_tr(1, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->get_literal(1, 3)));
				tmp2.write(eval->comp_and(tmp0, eval->comp_sub(tmp1, eval->get_literal(1, 1))));
				exec->write_arg(0, eval->comp_rol(exec->read_arg(0), tmp2));
				tmp3.write(eval->comp_zero(tmp2));
				tmp4.write(eval->comp_bitquest(tmp3, exec->read_extern_CF(), eval->comp_lsb(exec->read_arg(0))));
				exec->write_extern_OF(eval->comp_bitquest(tmp3, exec->read_extern_OF(), eval->comp_bitquest(eval->comp_eq(tmp0, eval->get_literal(1, 1)), eval->comp_lxor(eval->comp_sign(exec->read_arg(0)), tmp4), exec->read_extern_undefined())));
				exec->write_extern_CF(tmp4);
				return true;
			}
			break;	// returns false
		}
		case insn_ror:
		{
			Operand tmp0(1);
			Operand tmp1(1);
			Operand tmp2(1);
			Operand tmp3(1);
			Operand tmp4(1);
			{
				tmp0.write(exec->read_arg(1));
				tmp1.write(eval->comp_shl(eval->get_tr(1, eval->get_literal(8, exec->get_arg_size(0)) /* sizeof */), eval->get_literal(1, 3)));
				tmp2.write(eval->comp_and(tmp0, eval->comp_sub(tmp1, eval->get_literal(1, 1))));
				exec->write_arg(0, eval->comp_ror(exec->read_arg(0), tmp2));
				tmp3.write(eval->comp_zero(tmp2));
				tmp4.write(eval->comp_bitquest(tmp3, exec->read_extern_CF(), eval->comp_sign(exec->read_arg(0))));
				exec->write_extern_OF(eval->comp_bitquest(tmp3, exec->read_extern_OF(), eval->comp_bitquest(eval->comp_eq(tmp0, eval->get_literal(1, 1)), eval->comp_lxor(eval->comp_sign(exec->read_arg(0)), eval->comp_sign2(exec->read_arg(0))), exec->read_extern_undefined())));
				exec->write_extern_CF(tmp4);
				return true;
			}
			break;	// returns false
		}
		case insn_sahf:
		{
			{
				exec->write_extern_SF(eval->comp_nz(eval->comp_and(eval->comp_shru(exec->read_extern_AH(), eval->get_literal(1, 7)), eval->get_literal(1, 1))));
				exec->write_extern_ZF(eval->comp_nz(eval->comp_and(eval->comp_shru(exec->read_extern_AH(), eval->get_literal(1, 6)), eval->get_literal(1, 1))));
				exec->write_extern_AF(eval->comp_nz(eval->comp_and(eval->comp_shru(exec->read_extern_AH(), eval->get_literal(1, 4)), eval->get_literal(1, 1))));
				exec->write_extern_PF(eval->comp_nz(eval->comp_and(eval->comp_shru(exec->read_extern_AH(), eval->get_literal(1, 2)), eval->get_literal(1, 1))));
				exec->write_extern_CF(eval->comp_nz(eval->comp_and(exec->read_extern_AH(), eval->get_literal(1, 1))));
				return true;
			}
			break;	// returns false
		}
		case insn_sar:
		{
			Operand tmp0(1);
			Operand tmp1(1);
			Operand tmp2(exec->get_arg_size(0) * 1);
			{
				tmp0.write(eval->comp_and(exec->read_arg(1), eval->get_literal(1, 31)));
				tmp1.write(eval->comp_zero(tmp0));
				tmp2.write(exec->read_arg(0));
				exec->write_arg(0, eval->comp_shrs(exec->read_arg(0), tmp0));
				exec->write_extern_CF(eval->comp_bitquest(tmp1, exec->read_extern_CF(), eval->comp_sar_cf(tmp2, tmp0)));
				exec->write_extern_OF(eval->comp_bitquest(eval->comp_eq(tmp0, eval->get_literal(1, 1)), eval->comp_sign(tmp2), eval->comp_bitquest(tmp1, exec->read_extern_OF(), exec->read_extern_undefined())));
				exec->write_extern_SF(eval->comp_bitquest(tmp1, exec->read_extern_SF(), eval->comp_sign(exec->read_arg(0))));
				exec->write_extern_ZF(eval->comp_bitquest(tmp1, exec->read_extern_ZF(), eval->comp_zero(exec->read_arg(0))));
				exec->write_extern_AF(eval->comp_bitquest(tmp1, exec->read_extern_AF(), exec->read_extern_undefined()));
				exec->write_extern_PF(eval->comp_bitquest(tmp1, exec->read_extern_PF(), eval->comp_parity(exec->read_arg(0))));
				return true;
			}
			break;	// returns false
		}
		case insn_sbb:
		{
			Operand tmp0(1);
			Operand tmp1(exec->get_arg_size(0) * 1);
			{
				tmp0.write(exec->read_extern_CF());
				exec->write_extern_OF(eval->comp_sbb_of(exec->read_arg(0), exec->read_arg(1), tmp0));
				exec->write_extern_AF(eval->comp_sbb_af(exec->read_arg(0), exec->read_arg(1), tmp0));
				exec->write_extern_CF(eval->comp_sbb_cf(exec->read_arg(0), exec->read_arg(1), tmp0));
				tmp1.write(eval->comp_sub(exec->read_arg(0), exec->read_arg(1)));
				exec->write_arg(0, eval->comp_quest(tmp0, eval->comp_sub(tmp1, eval->get_literal(exec->get_arg_size(0), 1)), tmp1));
				exec->write_extern_SF(eval->comp_sign(exec->read_arg(0)));
				exec->write_extern_ZF(eval->comp_zero(exec->read_arg(0)));
				exec->write_extern_PF(eval->comp_parity(exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_shl:
		{
			Operand tmp0(1);
			Operand tmp1(1);
			Operand tmp2(exec->get_arg_size(0) * 1);
			{
				tmp0.write(eval->comp_and(exec->read_arg(1), eval->get_literal(1, 31)));
				tmp1.write(eval->comp_zero(tmp0));
				tmp2.write(exec->read_arg(0));
				exec->write_arg(0, eval->comp_shl(exec->read_arg(0), tmp0));
				exec->write_extern_CF(eval->comp_bitquest(tmp1, exec->read_extern_CF(), eval->comp_shl_cf(tmp2, tmp0)));
				exec->write_extern_OF(eval->comp_bitquest(eval->comp_eq(tmp0, eval->get_literal(1, 1)), eval->comp_lxor(eval->comp_sign(exec->read_arg(0)), exec->read_extern_CF()), eval->comp_bitquest(tmp1, exec->read_extern_OF(), exec->read_extern_undefined())));
				exec->write_extern_SF(eval->comp_bitquest(tmp1, exec->read_extern_SF(), eval->comp_sign(exec->read_arg(0))));
				exec->write_extern_ZF(eval->comp_bitquest(tmp1, exec->read_extern_ZF(), eval->comp_zero(exec->read_arg(0))));
				exec->write_extern_AF(eval->comp_bitquest(tmp1, exec->read_extern_AF(), exec->read_extern_undefined()));
				exec->write_extern_PF(eval->comp_bitquest(tmp1, exec->read_extern_PF(), eval->comp_parity(exec->read_arg(0))));
				return true;
			}
			break;	// returns false
		}
		case insn_shr:
		{
			Operand tmp0(1);
			Operand tmp1(1);
			Operand tmp2(exec->get_arg_size(0) * 1);
			{
				tmp0.write(eval->comp_and(exec->read_arg(1), eval->get_literal(1, 31)));
				tmp1.write(eval->comp_zero(tmp0));
				tmp2.write(exec->read_arg(0));
				exec->write_arg(0, eval->comp_shru(exec->read_arg(0), tmp0));
				exec->write_extern_CF(eval->comp_bitquest(tmp1, exec->read_extern_CF(), eval->comp_shr_cf(tmp2, tmp0)));
				exec->write_extern_OF(eval->comp_bitquest(eval->comp_eq(tmp0, eval->get_literal(1, 1)), eval->comp_sign(tmp2), eval->comp_bitquest(tmp1, exec->read_extern_OF(), exec->read_extern_undefined())));
				exec->write_extern_SF(eval->comp_bitquest(tmp1, exec->read_extern_SF(), eval->comp_sign(exec->read_arg(0))));
				exec->write_extern_ZF(eval->comp_bitquest(tmp1, exec->read_extern_ZF(), eval->comp_zero(exec->read_arg(0))));
				exec->write_extern_AF(eval->comp_bitquest(tmp1, exec->read_extern_AF(), exec->read_extern_undefined()));
				exec->write_extern_PF(eval->comp_bitquest(tmp1, exec->read_extern_PF(), eval->comp_parity(exec->read_arg(0))));
				return true;
			}
			break;	// returns false
		}
		case insn_stc:
		{
			{
				exec->write_extern_CF(eval->get_literal(1, 1));
				return true;
			}
			break;	// returns false
		}
		case insn_std:
		{
			{
				exec->write_extern_DF(eval->get_literal(1, 1));
				return true;
			}
			break;	// returns false
		}
		case insn_sti:
		{
			{
				exec->write_extern_IF(eval->get_literal(1, 1));
				return true;
			}
			break;	// returns false
		}
		case insn_sub:
		{
			{
				exec->write_extern_OF(eval->comp_sub_of(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_AF(eval->comp_sub_af(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_CF(eval->comp_sub_cf(exec->read_arg(0), exec->read_arg(1)));
				exec->write_arg(0, eval->comp_sub(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(exec->read_arg(0)));
				exec->write_extern_ZF(eval->comp_zero(exec->read_arg(0)));
				exec->write_extern_PF(eval->comp_parity(exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		case insn_test:
		{
			Operand tmp0(exec->get_arg_size(0) * 1);
			{
				exec->write_extern_OF(eval->get_literal(1, 0));
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_CF(eval->get_literal(1, 0));
				tmp0.write(eval->comp_and(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(tmp0));
				exec->write_extern_ZF(eval->comp_zero(tmp0));
				exec->write_extern_PF(eval->comp_parity(tmp0));
				return true;
			}
			break;	// returns false
		}
		case insn_xor:
		{
			{
				exec->write_extern_OF(eval->get_literal(1, 0));
				exec->write_extern_AF(exec->read_extern_undefined());
				exec->write_extern_CF(eval->get_literal(1, 0));
				exec->write_arg(0, eval->comp_xor(exec->read_arg(0), exec->read_arg(1)));
				exec->write_extern_SF(eval->comp_sign(exec->read_arg(0)));
				exec->write_extern_ZF(eval->comp_zero(exec->read_arg(0)));
				exec->write_extern_PF(eval->comp_parity(exec->read_arg(0)));
				return true;
			}
			break;	// returns false
		}
		default:
			break;
	}
	return false;
}


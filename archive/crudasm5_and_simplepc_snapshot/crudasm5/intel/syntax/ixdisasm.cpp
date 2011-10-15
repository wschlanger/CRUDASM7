// ixdisasm.cpp
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "ixdisasm.h"
#include <QtCore/QCoreApplication>

std::string unquotify(std::string s)
{
	if(s.size() < 2)
		return s;
	if(s[0] != '\"' || s[s.size() - 1] != '\"')
		return s;
	std::string::iterator b = s.begin();
	++b;
	std::string::iterator e = s.end();
	--e;
	return std::string(b, e);
}

// This is not especially fast.
int get_num(std::string s)
{
	if(s.size() > 2 && s[0] == '0' && s[1] == 'x')
	{
		std::string::iterator b = s.begin();
		++b;	// skip 0
		++b;	// skip x
		QString t = std::string(b, s.end()).c_str();
		bool ok = false;
		int x = t.toInt(&ok, 16);
		if(!ok)
			return 0;
		return x;
	}
	QString t = s.c_str();
	bool ok = false;
	int x = t.toInt(&ok, 10);
	if(!ok)
		return 0;
	return x;
}

bool IntelDisassembler::isValidIdent(std::string ident, int num_args)
{
	// write the given string (quotes removed).
	if(ident == "write")
		return num_args == 1;

	// write a separator between the opcode and operands.
	if(ident == "space")
		return num_args == 0;

	// ndisasm: show argument only if imm32 is not 0x0a.
	if(ident == "maybe_write_space_args_imm32")
		return num_args == 1;

	// write_args().
	if(ident == "write_args")
		return num_args == 0;

	/// write_arg(int arg, bool need_size = true, bool xoverride = false, bool no_short = false);
	if(ident == "write_arg")
		return num_args >= 1 && num_args <= 4;

	// write rep/repz/repnz.
	if(ident == "write_rep" || ident == "write_repcc")
		return num_args == 0;

	// write a size suffix (w, d, q, x, b) based on the size of the indicated argument.
	if(ident == "write_size_suffix_argsize")
		return num_args == 1;

	// write a size suffix (w, d, q, x, b) based on the operand size (osz).
	if(ident == "write_size_suffix_osz")
		return num_args == 0;

	// write a segment register PREFIX if not default, i.e. if override was used.
	if(ident == "write_seg_reg")
		return num_args == 0;

	// write an operand separator (comma).
	if(ident == "comma")
		return num_args == 0;

	// write the 1st argument if osz is 64 bits, else write 2nd argument.
	if(ident == "write_osz64")
		return num_args == 2;

	// exact meaning follows:
	/*
		if(get_dsz() == 2)
		{
			// 64bit mode.
			if(get_osz() == 0)
				write("o16 ");
		}
		else
		if(get_dsz() != get_osz())
		{
			write_o16_o32_o64();
		}
	*/
	if(ident == "write_stack_o16_o32_o64")
		return num_args == 0;

	// do_nop_xchg("nop", "xchg", 1) means: argvalue(1) is 0 & basecodei s 0x90, nop else do xchg
	if(ident == "do_nop_xchg")
		return num_args == 3;

	// exact meaning follows:
	/*
		if(get_dsz() == 2)
		{
			// 64bit mode.
			if(get_osz() != 1)
				write_o16_o32_o64();
		}
		else
		if(get_dsz() != get_osz())
		{
			write_o16_o32_o64();
		}
	*/
	if(ident == "write_xlat_o16_o32_o64")
		return num_args == 0;

	// if osz == 0, write 1st argument.
	// if osz == 1, write 2nd argument.
	// if osz == 2, write 3rd argument.
	if(ident == "write_osz")
		return num_args == 3;

	// if asz == 0, write 1st argument.
	// if asz == 1, write 2nd argument.
	// if asz == 2, write 3rd argument.
	if(ident == "write_asz")
		return num_args == 3;

	// Write a far immediate. We use both imm (for the offset) & disp (for segment).
	// Never happens in 64bit mode.
	if(ident == "write_far_imm")
		return num_args == 0;

	// exact meaning follows:
	/*
		if(get_dsz() != 2)
		{
			if(get_osz() != get_dsz())
				write_o16_o32_o64();
		}
	*/
	if(ident == "write_jrcxz_o16_o32_o64")
		return num_args == 0;

	return false;
}

void IntelDisassembler::disasm(char *destT, const char *immT, const char *dispT, U8 relT, ix_decoder_state *stateT, ix_icode_t *icodeT, int dszT)
{
	// What we want to do is look up the given instruction, and see if it requires special handling for the requested personality.
	// Try "all" if not found, then use default handler.
	dsz = dszT;
	dest = destT;
	imm = immT;
	disp = dispT;
	state = stateT;
	icode = icodeT;
	dest[0] = '\0';
	rel = relT + static_cast<U8>(state->insn_size);		// if used, user must MASK to 16 or 32 bits if applicable.

	if(icode->lockrep == 1)
		write("lock ");

	if(decoder->encoding_is_fwaitable(state->encoding_index))
	{
		if(state->wait_used)
			write("f");
		else
			write("fn");
	}

	if(!specialHandler())
	{
		// print o16/o32/o64 if needed.
		maybe_write_oxx(dsz);

		IntelSyntaxInsn *insn = decoder->syntax->getInsn(decoder->syntax->getEncoding(state->encoding_index)->insn);
		write(decoder->syntax->getSymbol(insn->proto.alias).c_str());
		write_any_args();
	}
}

void IntelDisassembler::handle_stmt(IntelSyntaxDisStmt &stmt)
{
	if(stmt.ident == symbol_write)
	{
		write(unquotify(stmt.args[0]).c_str());
	}
	else
	if(stmt.ident == symbol_space)
	{
		space();
	}
	else
	if(stmt.ident == symbol_maybe_write_space_args_imm32)
	{
		if(icode->imm != get_num(stmt.args[0]))
		{
			space();
			write_args();
		}
	}
	else
	if(stmt.ident == symbol_write_args)
	{
		write_args();
	}
	else
	if(stmt.ident == symbol_write_arg)
	{
		if(stmt.args.size() == 1)
			write_arg(get_num(stmt.args[0]));
		else
		if(stmt.args.size() == 2)
			write_arg(get_num(stmt.args[0]), get_num(stmt.args[1]));
		else
		if(stmt.args.size() == 3)
			write_arg(get_num(stmt.args[0]), get_num(stmt.args[1]), get_num(stmt.args[2]));
		else
		if(stmt.args.size() == 4)
			write_arg(get_num(stmt.args[0]), get_num(stmt.args[1]), get_num(stmt.args[2]), get_num(stmt.args[3]));
	}
	else
	if(stmt.ident == symbol_write_rep)
	{
		if(icode->lockrep == 2 || icode->lockrep == 3)
			write("rep ");
	}
	else
	if(stmt.ident == symbol_write_repcc)
	{
		if(icode->lockrep == 2)
			write("repnz ");
		else
		if(icode->lockrep == 3)
			write("repz ");
	}
	else
	if(stmt.ident == symbol_write_size_suffix_argsize)
	{
		write_size_suffix(icode->argsize[get_num(stmt.args[0])]);
	}
	else
	if(stmt.ident == symbol_write_size_suffix_osz)
	{
		write_size_suffix(2 << icode->osz);
	}
	else
	if(stmt.ident == symbol_write_seg_reg)
	{
		if(icode->ea.sreg <= 5)
		{
			do_write_seg_reg(icode->ea.sreg);
			write(" ");
		}
	}
	else
	if(stmt.ident == symbol_comma)
	{
		comma();
	}
	else
	if(stmt.ident == symbol_write_osz64)
	{
		write(unquotify(stmt.args[(icode->osz == 2) ? 0 : 1]).c_str());
	}
	else
	if(stmt.ident == symbol_write_stack_o16_o32_o64)
	{
		if(dsz == 2)
		{
			// 64bit mode.
			if(icode->osz == 0)
				write("o16 ");
		}
		else
		if(dsz != icode->osz)
		{
			write_o16_o32_o64();
		}
	}
	else
	if(stmt.ident == symbol_do_nop_xchg)
	{
		// do_nop_xchg("nop", "xchg", 1) means: argvalue(1) is 0, nop else do xchg
		int a = get_num(stmt.args[2]);
		if(icode->argvalue[a] == 0 && state->opcode0 == 0x90 && icode->argtype[0] == IntelArgTypes::Treg_gr)
			write(unquotify(stmt.args[0]).c_str());
		else
		{
			write(unquotify(stmt.args[1]).c_str());
			space();
			write_args();
		}
	}
	else
	if(stmt.ident == symbol_write_xlat_o16_o32_o64)
	{
		if(dsz == 2)
		{
			// 64bit mode.
			if(icode->osz != 1)
				write_o16_o32_o64();
		}
		else
		if(dsz != icode->osz)
		{
			write_o16_o32_o64();
		}
	}
	else
	if(stmt.ident == symbol_write_osz)
	{
		// if osz == 0, write 1st argument.
		// if osz == 1, write 2nd argument.
		// if osz == 2, write 3rd argument.
		write(unquotify(stmt.args[icode->osz]).c_str());
	}
	else
	if(stmt.ident == symbol_write_asz)
	{
		// if asz == 0, write 1st argument.
		// if asz == 1, write 2nd argument.
		// if asz == 2, write 3rd argument.
		write(unquotify(stmt.args[icode->asz]).c_str());
	}
	else
	if(stmt.ident == symbol_write_far_imm)
	{
		write_far_imm();
	}
	else
	if(stmt.ident == symbol_write_jrcxz_o16_o32_o64)
	{
		if(dsz != 2)
		{
			if(icode->osz != dsz)
				write_o16_o32_o64();
		}
	}
	else
	{
		write("<unimpl-stmt>");
	}
}

void IntelDisassembler::write_far_imm()
{
	// This is not used in 64bit mode.
	write_asm_size(2 << icode->osz);
	//write(" far ");
	write(" ");
	do_write_imm(icode->argsize[1], (U4)(U2)(icode->disp));
	write(":");
	do_write_imm(icode->argsize[0], icode->imm);
}

void IntelDisassembler::write_arg(int arg, bool need_size /*= true*/, bool xoverride /*= false*/, bool no_short /*= false*/)
{
	IntelSyntaxEncoding *encoding = decoder->syntax->getEncoding(state->encoding_index);
	IntelSyntaxInsn *insn = decoder->syntax->getInsn(encoding->insn);

	// Note: regs8[16..19] are used only by implict registers, if the user wants to specify AH, CH, DH, or BH.
	const char *regs8x86[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
	const char *regs8[20] =  { "al",  "cl",  "dl",  "bl", "spl", "bpl", "sil", "dil", "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b", "ah", "ch", "dh", "bh"};
	const char *regs16[16] = { "ax",  "cx",  "dx",  "bx",  "sp",  "bp",  "si",  "di", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w"};
	const char *regs32[16] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d"};
	const char *regs64[16] = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",  "r8",  "r9",  "r10",  "r11",  "r12",  "r13",  "r14",  "r15"};

	U1 argtype = icode->argtype[arg];
	S1 argsize = icode->argsize[arg];
	U1 argvalue = icode->argvalue[arg];

	if(argtype >= 0x80)
		write("<unimpl-arg>");
	else
	{
		S1 proto_size = insn->proto.argSizes[arg];

		if(proto_size < 0)
		{
			// e.g. insn mov(P x, P y).
			// Here, cur arg may have a parameterized size (P in this case)
			// and another arg might have same size. If that other arg's
			// size is known, then set mem_need_size to false.

			for(int i = 0; i < insn->proto.numArgs; ++i)
			{
				if(i == arg)
					continue;
				S1 proto2 = insn->proto.argSizes[i];
				if(proto2 == proto_size)
				{
					if(icode->argsize[i] > 0)
					{
						if(!IntelArgTypes::is_imm(icode->argtype[i]))
						{
							// Immediate sizes don't "count".
							need_size = false;
							break;
						}
					}
				}
			}
		}
		else
			need_size = false;

		if(xoverride)
			need_size = true;

		if(IntelArgTypes::is_reg(argtype))
		{
			switch(argtype)
			{
			case IntelArgTypes::Treg_gr:
				{
					if(argsize == 1)
					{
						if(encoding->argvalue[arg] == 0xff)
						{
							if(state->rex_used)
								write((argvalue <= 15) ? regs8[argvalue] : "<bad-reg-arg-1>");
							else
								write((argvalue <= 7) ? regs8x86[argvalue] : "<bad-reg-arg-2>");
						}
						else
						{
							// Implict argument.
							write((argvalue <= 19) ? regs8[argvalue] : "<bad-reg-arg-3>");
						}
					}
					else
					if(argvalue <= 15)
					{
						if(argsize == 2)
							write(regs16[argvalue]);
						else
						if(argsize == 4)
							write(regs32[argvalue]);
						else
						if(argsize == 8)
							write(regs64[argvalue]);
						else
							write("<bad-reg-arg-4>");
					}
					else
						write("<bad-reg-arg-5>");

					return;
				}
			case IntelArgTypes::Treg_xmm:
				{
					write("xmm");
					dest += sprintf(dest, "%d", argvalue);
					return;
				}
			case IntelArgTypes::Treg_mmx:
				{
					write("mm");
					dest += sprintf(dest, "%d", argvalue);
					return;
				}
			case IntelArgTypes::Treg_sr:
				{
					do_write_seg_reg(argvalue);
					return;
				}
			case IntelArgTypes::Treg_dr:
				{
					write("dr");
					dest += sprintf(dest, "%d", argvalue);
					return;
				}
			case IntelArgTypes::Treg_cr:
				{
					write("cr");
					dest += sprintf(dest, "%d", argvalue);
					return;
				}
			case IntelArgTypes::Treg_st:
				{
					write("st");
					dest += sprintf(dest, "%d", argvalue);
					return;
				}
			default:
				break;
			}
		}
		else
		if(IntelArgTypes::is_imm(argtype))
		{
			U8 ival = 0;
			if(argtype == IntelArgTypes::Timm_implict)
				ival = argvalue;	// zero extend from byte
			else
			if(argtype == IntelArgTypes::Timm_2nd)
				ival = icode->disp;
			else
			{
				ival = icode->imm;
///dest += std::sprintf(dest, "<%d>", icode->argsize[arg]);
				if(icode->argsize[arg] == 8)
				{
					if(encoding->tags.find(symbol_imm64_disp) != encoding->tags.end())
						ival += (((U8)(icode->disp) << 16) << 16);
					else
					if(encoding->tags.find(symbol_imm64_sx32) != encoding->tags.end())
						ival = (U8)(S8)(S4)(U4)(ival);
				}
///dest += std::sprintf(dest, "<%08x%08x>", (U4)((ival>>16)>>16), (U4)(ival));
			}

			// ival is now set correctly.

			bool did_near = false;
			bool relative = (encoding->tags.find(symbol_relative) != encoding->tags.end());

			if(icode->sx)
			{
				//if(insn->proto.tags.find(symbol_ctrlxfer) != insn->proto.tags.end())
				if(relative)
				{
					if(!no_short)
						write("short ");
				}
				else
					write("byte ");
			}
			else
			if(need_size)
			{
				if(argsize != 8 && encoding->tags.find(symbol_imm64_sx32) == encoding->tags.end())
					write_asm_size(argsize);
				else
				{
					// NOTE: nasm refuses to accept 'jo dword $$+0x12345678' in 64bit mode.
					// It instead expects 'jo near $$+0x12345678'. However, ndisasm disassembles
					// the latter instruction to the forbidden 'jo dword 0x12345678', which nasm
					// won't accept! We avoid repeating that mistake here, and instead strive to
					// produce code that will reasonably be expected to reassemble.
					///if(insn->proto.tags.find(symbol_ctrlxfer) != insn->proto.tags.end())
					if(relative)
					{
						write("near");
						did_near = true;
					}
					else
						write_asm_size(2 << icode->osz);	/* was hardwired to 4 */
				}
				write(" ");
			}

			if(imm != NULL)
			{
				write(imm);
				return;
			}

			bool did_plus = false;
			if(relative)
			{
				if(!icode->sx && !did_near)
					write("near ");

				write("$$+");
				did_plus = true;

				ival += rel;

				if(dsz != 2)
				{
					if(icode->osz == 0)
						ival &= TULL(0xffff);
					else
					if(icode->osz == 1)
						ival &= TULL(0xffffffff);
				}
			}

			///if(icode->sx && insn->proto.tags.find(symbol_ctrlxfer) == insn->proto.tags.end())
			if(icode->sx && !relative)
			{
				if((ival & 0xff) < 0x80)
				{
					if(did_plus)
						dest += sprintf(dest, "0x%02x", ((U4)(ival)) & 0xff);
					else
						dest += sprintf(dest, "+0x%02x", ((U4)(ival)) & 0xff);
				}
				else
					dest += sprintf(dest, "-0x%02x", 0x100 - (((U4)(ival)) & 0xff));
			}
			else
			{
				if(argsize == 8 && (encoding->tags.find(symbol_imm64_disp) == encoding->tags.end()))
				{
					if(relative)
						do_write_imm(argsize, ival);
					else
					if(ival < TULL(0x80000000))
					{
						if(did_plus)
							dest += std::sprintf(dest, "0x%08x", (U4)(ival));
						else
							dest += std::sprintf(dest, "+0x%08x", (U4)(ival));
					}
					else
						dest += std::sprintf(dest, "-0x%08x", (U4)(TULL(0x100000000) - (U8)(U4)(ival)));
				}
				else
					do_write_imm(argsize, ival);
			}

			return;
		}
		else
		if(IntelArgTypes::is_mem(argtype))
		{
			if(argtype == IntelArgTypes::Tmem_ea_eal || argtype == IntelArgTypes::Tmem_ea_eai || argtype == IntelArgTypes::Tmem_ea_fxs)
				need_size = false;

			if(need_size)
			{
				if(argsize == 10)
					write("tword ");
				else
				if(argsize == 2 || argsize == 4 || argsize == 8)
				{
					write_asm_size(argsize);
					write(" ");
					if(argtype == IntelArgTypes::Tmem_ea && (insn->proto.tags.find(symbol_ctrlxfer) != insn->proto.tags.end()))
						write("near ");
					else
					if(argtype == IntelArgTypes::Tmem_ea_seg && (insn->proto.tags.find(symbol_ctrlxfer) != insn->proto.tags.end()))
						write("far ");
				}
				else
				if(argsize == 1)
					write("byte ");
				else
					write("<unsupported size> ");
			}

			write("[");

			if(icode->rip_relative != 0)
				write("rel ");

			if(argtype == IntelArgTypes::Tmem_fulldisp && icode->asz == 2 && encoding->tags.find(symbol_imm64_disp) != encoding->tags.end())
				write("qword ");

			bool needzero = false;
			if(icode->asz != dsz)
			{
				if(argtype == IntelArgTypes::Tmem_fulldisp || (icode->ea.base == 31 && icode->ea.index == 31))
				{
					needzero = true;
					write_asm_size(2 << icode->asz);
					write(" ");
				}
			}

			if(icode->ea.sreg <= 5)
			{
				write_seg_reg(false);
				write(":");
			}

			// Now show registers and displacement in EA, itself.

			const char *base = NULL;
			const char *index = NULL;

			if(icode->asz == 0)
			{
				if(icode->ea.base != 31)
					base = regs16[icode->ea.base];
				if(icode->ea.index != 31)
					index = regs16[icode->ea.index];
			}
			else
			if(icode->asz == 1)
			{
				if(icode->ea.base != 31)
					base = regs32[icode->ea.base];
				if(icode->ea.index != 31)
					index = regs32[icode->ea.index];
			}
			else
			{
				if(icode->ea.base != 31)
					base = regs64[icode->ea.base];
				if(icode->ea.index != 31)
					index = regs64[icode->ea.index];
			}

			bool need_plus = false;

			if(base != NULL)
			{
				write(base);
				need_plus = true;
			}

			if(index != NULL)
			{
				if(need_plus)
					write("+");
				need_plus = true;
				write(index);
				if(icode->ea.index_scale != 0)
				{
					dest += std::sprintf(dest, "*%x", 1 << icode->ea.index_scale);
				}
			}

			// Now show displacement!
			if(disp != NULL)
			{
				if(need_plus)
					write("+");
				write(disp);
			}
			else
			if(argtype != IntelArgTypes::Tmem_fulldisp && icode->ea.disp8 != 0)
			{
				U4 mydisp = icode->disp;
				if((mydisp & 0xff) < 0x80)
				{
					if(need_plus)
						write("+");
					dest += std::sprintf(dest, "0x%02x", mydisp);
				}
				else
					dest += std::sprintf(dest, "-0x%02x", 0x100 - (mydisp & 0xff));
			}
			else
			if(argtype == IntelArgTypes::Tmem_fulldisp || icode->has_disp != 0)
			{
				bool iszero = icode->disp == 0;
				U8 dispval = icode->disp;
				if(encoding->tags.find(symbol_imm64_disp) != encoding->tags.end() && icode->asz == 2)
				{
					dispval = icode->disp;
					dispval += ((U8)(icode->imm) << 16) << 16;
					iszero = dispval == 0;
				}
				if(!iszero || argtype == IntelArgTypes::Tmem_fulldisp || (icode->ea.index == 31 && icode->ea.base == 31))
				{
					const char *plus = (need_plus) ? "+" : "";
					if(icode->asz == 0)
						dest += std::sprintf(dest, "%s0x%04x", plus, (U4)(U2)(dispval));
					else
					if(icode->asz == 1)
						dest += std::sprintf(dest, "%s0x%08x", plus, (U4)(dispval));
					else
					if(argtype == IntelArgTypes::Tmem_fulldisp)
						dest += std::sprintf(dest, "%s0x%08x%08x", plus, (U4)((dispval >> 16) >> 16), (U4)(dispval));
					else
					if(icode->rip_relative == 0)
					{
						if(dispval < TULL(0x80000000))
							dest += std::sprintf(dest, "%s0x%08x", plus, (U4)(dispval));
						else
							dest += std::sprintf(dest, "-0x%08x", (U4)(TULL(0x100000000) - (U8)(U4)(dispval)));
					}
					else
					{
						// already printed 'rel'.
						U8 newval = rel/* + (U8)(s->size)*/;
						if(dispval < TULL(0x80000000))
							newval += (U8)(dispval);
						else
							newval -= (U8)(U4)(TULL(0x100000000) - (U8)(U4)(dispval));
						if(newval < TULL(0x100000000))
							dest += std::sprintf(dest, "%s0x%x", plus, (U4)(newval));
						else
							dest += std::sprintf(dest, "%s0x%x%08x", plus, (U4)((newval >> 16) >> 16), (U4)(newval));
					}
				}
			}

			write("]");

			return;
		}

		write("<unimpl-arg>");
	}
}

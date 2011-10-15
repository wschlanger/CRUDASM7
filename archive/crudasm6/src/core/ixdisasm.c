// ixdisasm.c
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "ixdisasm.h"

struct crudasm_intel_disasm_context_t
{
	struct ix_icode_t *icode;
	U8 rel;
	char *dest;
	const char *imm;
	const char *disp;
	struct crudasm_intel_insn_t *insn;
	struct crudasm_intel_encoding_t *encoding;
	U1 dsz;
};

static void ixdis1_write(struct crudasm_intel_disasm_context_t *context, const char *msg)
{
	context->dest += sprintf(context->dest, "%s", msg);
}

static void ixdis0_space(struct crudasm_intel_disasm_context_t *context)
{
	ixdis1_write(context, " ");
}

static void ixdis0_comma(struct crudasm_intel_disasm_context_t *context)
{
	ixdis1_write(context, ",");
}

static void do_write_seg_reg(struct crudasm_intel_disasm_context_t *context, U1 value)
{
	const char *mysreg[8] = {"es", "cs", "ss", "ds", "fs", "gs", "ss", "ds"};
	if(value < 8)
	{
		ixdis1_write(context, mysreg[value]);
	}
	else
		ixdis1_write(context, "<unknown seg reg>");
}

void ixdis1_write_asm_size(struct crudasm_intel_disasm_context_t *context, U4 s)
{
	switch(s)
	{
		case 2:
			ixdis1_write(context, "word");
			break;
		case 4:
			ixdis1_write(context, "dword");
			break;
		case 8:
			ixdis1_write(context, "qword");
			break;
		case 16:
			ixdis1_write(context, "xword");
			break;
		case 10:
			ixdis1_write(context, "tword");	// is this right ?
			break;
		case 1:
			ixdis1_write(context, "byte");
			break;
		default:
			ixdis1_write(context, "<unsupported size>");
			break;
	}
}

static void do_write_imm(struct crudasm_intel_disasm_context_t *context, U4 size, U8 imm_value)
{
	if(size == 1)
		context->dest += sprintf(context->dest, "0x%02x", (U4)(U1)(imm_value));
	else
	if(size == 2)
		context->dest += sprintf(context->dest, "0x%04x", (U4)(U2)(imm_value));
	else
	if(size == 4)
		context->dest += sprintf(context->dest, "0x%08x", ((U4)(imm_value)));
	else
	if(size == 8)
		context->dest += sprintf(context->dest, "0x%08x%08x", ((U4)((imm_value >> 16) >> 16)), ((U4)(imm_value)));
	else
		ixdis1_write(context, "<unsupported immediate size>");
}

static void write_seg_reg(struct crudasm_intel_disasm_context_t *context, int with_space)
{
	if(context->icode->ea.sreg <= 5)
	{
		do_write_seg_reg(context, context->icode->ea.sreg);
		if(with_space)
			ixdis1_write(context, " ");
	}
}

static void ixdis4_write_arg(struct crudasm_intel_disasm_context_t *context, int arg, int need_size, int xoverride, int no_short)
{
	struct crudasm_intel_insn_t *insn = context->insn;
	struct crudasm_intel_encoding_t *encoding = context->encoding;
	struct ix_icode_t *icode = context->icode;
	
	// Note: regs8[16..19] are used only by implict registers, if the user wants to specify AH, CH, DH, or BH.
	const char *regs8x86[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
	const char *regs8[20] =  { "al",  "cl",  "dl",  "bl", "spl", "bpl", "sil", "dil", "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b", "ah", "ch", "dh", "bh"};
	const char *regs16[16] = { "ax",  "cx",  "dx",  "bx",  "sp",  "bp",  "si",  "di", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w"};
	const char *regs32[16] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d"};
	const char *regs64[16] = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",  "r8",  "r9",  "r10",  "r11",  "r12",  "r13",  "r14",  "r15"};

	U1 argtype = icode->argtype[arg];
	S1 argsize = icode->argsize[arg];
	U1 argvalue = icode->argvalue[arg];
	
	S1 proto_size;
	int i;
	S1 proto2;
	U8 ival;
	int did_near, relative, did_plus;
	const char *base, *index;
	int need_plus, needzero;
	int iszero;
	U8 dispval, newval;
	U4 mydisp;
	const char *plus;

	if(argtype >= 0x80)
		ixdis1_write(context, "<unimpl-arg>");
	else
	{
		proto_size = insn->argSizes[arg];

		if(proto_size < 0)
		{
			// e.g. insn mov(P x, P y).
			// Here, cur arg may have a parameterized size (P in this case)
			// and another arg might have same size. If that other arg's
			// size is known, then set mem_need_size to false.

			for(i = 0; i < insn->numArgs; ++i)
			{
				if(i == arg)
					continue;
				proto2 = insn->argSizes[i];
				if(proto2 == proto_size)
				{
					if(icode->argsize[i] > 0)
					{
						if(!crudasm_intel_argtype__is_imm(icode->argtype[i]))
						{
							// Immediate sizes don't "count".
							need_size = 0;
							break;
						}
					}
				}
			}
		}
		else
			need_size = 0;

		if(xoverride)
			need_size = 1;
		
		if(crudasm_intel_argtype__is_reg(argtype))
		{
			switch(argtype)
			{
				case crudasm_intel_argtype_reg_gr:
					if(argsize == 1)
					{
						if(encoding->argvalue[arg] == 0xff)
						{
							if(icode->rex_used)
								ixdis1_write(context, (argvalue <= 15) ? regs8[argvalue] : "<bad-reg-arg-1>");
							else
								ixdis1_write(context, (argvalue <= 7) ? regs8x86[argvalue] : "<bad-reg-arg-2>");
						}
						else
						{
							// Implict argument.
							ixdis1_write(context, (argvalue <= 19) ? regs8[argvalue] : "<bad-reg-arg-3>");
						}
					}
					else
					if(argvalue <= 15)
					{
						if(argsize == 2)
							ixdis1_write(context, regs16[argvalue]);
						else
						if(argsize == 4)
							ixdis1_write(context, regs32[argvalue]);
						else
						if(argsize == 8)
							ixdis1_write(context, regs64[argvalue]);
						else
							ixdis1_write(context, "<bad-reg-arg-4>");
					}
					else
						ixdis1_write(context, "<bad-reg-arg-5>");

					return;
				case crudasm_intel_argtype_reg_xmm:
					ixdis1_write(context, "xmm");
					context->dest += sprintf(context->dest, "%d", argvalue);
					return;
				case crudasm_intel_argtype_reg_mmx:
					ixdis1_write(context, "mm");
					context->dest += sprintf(context->dest, "%d", argvalue);
					return;
				case crudasm_intel_argtype_reg_dr:
					ixdis1_write(context, "dr");
					context->dest += sprintf(context->dest, "%d", argvalue);
					return;
				case crudasm_intel_argtype_reg_cr:
					ixdis1_write(context, "cr");
					context->dest += sprintf(context->dest, "%d", argvalue);
					return;
				case crudasm_intel_argtype_reg_st:
					ixdis1_write(context, "st");
					context->dest += sprintf(context->dest, "%d", argvalue);
					return;
				case crudasm_intel_argtype_reg_sr:
					do_write_seg_reg(context, argvalue);
					return;
				default:
					break;
			}	// end switch
			ixdis1_write(context, "<bad-reg-arg-6>");
			return;
		}	// end reg
		else
		if(crudasm_intel_argtype__is_imm(argtype))
		{
			ival = 0;
			if(argtype == crudasm_intel_argtype_imm_implict)
				ival = argvalue;	// zero extend from byte
			else
			if(argtype == crudasm_intel_argtype_imm_2nd)
				ival = icode->disp;
			else
			{
				ival = icode->imm;
				if(icode->argsize[arg] == 8)
				{
					if(encoding->flag_imm64_disp != 0)
						ival += (((U8)(icode->disp) << 16) << 16);
					else
					if(encoding->flag_imm64_sx32 != 0)
						ival = (U8)(S8)(S4)(U4)(ival);
				}
			}

			// ival is now set correctly.
			did_near = 0;
			relative = encoding->flag_relative != 0;

			if(icode->sx)
			{
				if(relative)
				{
					if(!no_short)
						ixdis1_write(context, "short ");
				}
				else
					ixdis1_write(context, "byte ");
			}
			else
			if(need_size)
			{
				if(argsize != 8 && encoding->flag_imm64_sx32 == 0)
					ixdis1_write_asm_size(context, argsize);
				else
				{
					// NOTE: nasm refuses to accept 'jo dword $$+0x12345678' in 64bit mode.
					// It instead expects 'jo near $$+0x12345678'. However, ndisasm disassembles
					// the latter instruction to the forbidden 'jo dword 0x12345678', which nasm
					// won't accept! We avoid repeating that mistake here, and instead strive to
					// produce code that will reasonably be expected to reassemble.
					if(relative)
					{
						ixdis1_write(context, "near");
						did_near = 1;
					}
					else
						ixdis1_write_asm_size(context, 2 << icode->osz);	/* was hardwired to 4 */
				}
				ixdis1_write(context, " ");
			}
			
			if(context->imm != NULL)
			{
				ixdis1_write(context, context->imm);
				return;
			}
			
			did_plus = 0;

			if(relative)
			{
				if(!icode->sx && !did_near)
					ixdis1_write(context, "near ");

				ixdis1_write(context, "$$+");
				did_plus = 1;

				ival += context->rel;

				if(context->dsz != 2)
				{
					if(icode->osz == 0)
						ival &= TULL(0xffff);
					else
					if(icode->osz == 1)
						ival &= TULL(0xffffffff);
				}
			}

			if(icode->sx && !relative)
			{
				if((ival & 0xff) < 0x80)
				{
					if(did_plus)
						context->dest += sprintf(context->dest, "0x%02x", ((U4)(ival)) & 0xff);
					else
						context->dest += sprintf(context->dest, "+0x%02x", ((U4)(ival)) & 0xff);
				}
				else
					context->dest += sprintf(context->dest, "-0x%02x", 0x100 - (((U4)(ival)) & 0xff));
			}
			else
			{
				if(argsize == 8 && encoding->flag_imm64_disp == 0)
				{
					if(relative)
						do_write_imm(context, argsize, ival);
					else
					if(ival < TULL(0x80000000))
					{
						if(did_plus)
							context->dest += sprintf(context->dest, "0x%08x", (U4)(ival));
						else
							context->dest += sprintf(context->dest, "+0x%08x", (U4)(ival));
					}
					else
						context->dest += sprintf(context->dest, "-0x%08x", (U4)(TULL(0x100000000) - (U8)(U4)(ival)));
				}
				else
					do_write_imm(context, argsize, ival);
			}

			return;
		}	// end imm
		else
		if(crudasm_intel_argtype__is_mem(argtype))
		{
			if(argtype == crudasm_intel_argtype_mem_ea_eal || argtype == crudasm_intel_argtype_mem_ea_eai || argtype == crudasm_intel_argtype_mem_ea_fxs)
				need_size = 0;

			if(need_size)
			{
				if(argsize == 10)
					ixdis1_write(context, "tword ");
				else
				if(argsize == 2 || argsize == 4 || argsize == 8)
				{
					ixdis1_write_asm_size(context, argsize);
					ixdis1_write(context, " ");
					if(argtype == crudasm_intel_argtype_mem_ea && insn->flag_ctrlxfer != 0)
						ixdis1_write(context, "near ");
					else
					if(argtype == crudasm_intel_argtype_mem_ea_seg && insn->flag_ctrlxfer != 0)
						ixdis1_write(context, "far ");
				}
				else
				if(argsize == 1)
					ixdis1_write(context, "byte ");
				else
					ixdis1_write(context, "<unsupported size> ");
			}

			ixdis1_write(context, "[");
			
			if(icode->rip_relative != 0)
				ixdis1_write(context, "rel ");

			if(argtype == crudasm_intel_argtype_mem_fulldisp && icode->asz == 2 && encoding->flag_imm64_disp != 0)
				ixdis1_write(context, "qword ");

			needzero = 0;
			if(icode->asz != context->dsz)
			{
				if(argtype == crudasm_intel_argtype_mem_fulldisp || (icode->ea.base == 31 && icode->ea.index == 31))
				{
					needzero = 1;
					ixdis1_write_asm_size(context, 2 << icode->asz);
					ixdis1_write(context, " ");
				}
			}

			if(icode->ea.sreg <= 5)
			{
				write_seg_reg(context, 0);
				ixdis1_write(context, ":");
			}

			// Now show registers and displacement in EA, itself.
			base = NULL;
			index = NULL;

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

			need_plus = 0;

			if(base != NULL)
			{
				ixdis1_write(context, base);
				need_plus = 1;
			}

			if(index != NULL)
			{
				if(need_plus)
					ixdis1_write(context, "+");
				need_plus = 1;
				ixdis1_write(context, index);
				if(icode->ea.index_scale != 0)
				{
					context->dest += sprintf(context->dest, "*%x", 1 << icode->ea.index_scale);
				}
			}

			// Now show displacement!
			if(context->disp != NULL)
			{
				if(need_plus)
					ixdis1_write(context, "+");
				ixdis1_write(context, context->disp);
			}
			else
			if(argtype != crudasm_intel_argtype_mem_fulldisp && icode->ea.disp8 != 0)
			{
				mydisp = icode->disp;
				if((mydisp & 0xff) < 0x80)
				{
					if(need_plus)
						ixdis1_write(context, "+");
					context->dest += sprintf(context->dest, "0x%02x", mydisp);
				}
				else
					context->dest += sprintf(context->dest, "-0x%02x", 0x100 - (mydisp & 0xff));
			}
			else
			if(argtype == crudasm_intel_argtype_mem_fulldisp || icode->has_disp != 0)
			{
				iszero = icode->disp == 0;
				dispval = icode->disp;
				if(encoding->flag_imm64_disp != 0 && icode->asz == 2)
				{
					dispval = icode->disp;
					dispval += ((U8)(icode->imm) << 16) << 16;
					iszero = dispval == 0;
				}
				if(!iszero || argtype == crudasm_intel_argtype_mem_fulldisp || (icode->ea.index == 31 && icode->ea.base == 31))
				{
					plus = (need_plus) ? "+" : "";
					if(icode->asz == 0)
						context->dest += sprintf(context->dest, "%s0x%04x", plus, (U4)(U2)(dispval));
					else
					if(icode->asz == 1)
						context->dest += sprintf(context->dest, "%s0x%08x", plus, (U4)(dispval));
					else
					if(argtype == crudasm_intel_argtype_mem_fulldisp)
						context->dest += sprintf(context->dest, "%s0x%08x%08x", plus, (U4)((dispval >> 16) >> 16), (U4)(dispval));
					else
					if(icode->rip_relative == 0)
					{
						if(dispval < TULL(0x80000000))
							context->dest += sprintf(context->dest, "%s0x%08x", plus, (U4)(dispval));
						else
							context->dest += sprintf(context->dest, "-0x%08x", (U4)(TULL(0x100000000) - (U8)(U4)(dispval)));
					}
					else
					{
						// already printed 'rel'.
						newval = context->rel/* + (U8)(s->size)*/;
						if(dispval < TULL(0x80000000))
							newval += (U8)(dispval);
						else
							newval -= (U8)(U4)(TULL(0x100000000) - (U8)(U4)(dispval));
						if(newval < TULL(0x100000000))
							context->dest += sprintf(context->dest, "%s0x%x", plus, (U4)(newval));
						else
							context->dest += sprintf(context->dest, "%s0x%x%08x", plus, (U4)((newval >> 16) >> 16), (U4)(newval));
					}
				}
			}

			ixdis1_write(context, "]");
			return;
		}	// end mem
	}
	
	ixdis1_write(context, "<unimplemented-argument>");
}

static void ixdis3_write_arg(struct crudasm_intel_disasm_context_t *context, int arg, int need_size, int xoverride)
{
	ixdis4_write_arg(context, arg, need_size, xoverride, 0);
}

static void ixdis2_write_arg(struct crudasm_intel_disasm_context_t *context, int arg, int need_size)
{
	ixdis4_write_arg(context, arg, need_size, 0, 0);
}

static void ixdis1_write_arg(struct crudasm_intel_disasm_context_t *context, int arg)
{
	ixdis4_write_arg(context, arg, 1, 0, 0);
}

static void ixdis0_write_args(struct crudasm_intel_disasm_context_t *context)
{
	int i;
	for(i = 0; i < 4; ++i)
	{
		if(context->icode->argtype[i] == crudasm_intel_argtype_void)
			break;
		if(i != 0)
			ixdis0_comma(context);
		ixdis1_write_arg(context, i);
	}
}

static void ixdis0_write_any_args(struct crudasm_intel_disasm_context_t *context)
{
	if(context->icode->argtype[0] != crudasm_intel_argtype_void)
	{
		ixdis0_space(context);
		ixdis0_write_args(context);
	}
}

static void ixdis1_maybe_write_space_args_imm32(struct crudasm_intel_disasm_context_t *context, U4 value)
{
	if(context->icode->imm != value)
	{
		ixdis0_space(context);
		ixdis0_write_args(context);
	}
}

static void ixdis0_write_rep(struct crudasm_intel_disasm_context_t *context)
{
	if(context->icode->lockrep == 2 || context->icode->lockrep == 3)
		ixdis1_write(context, "rep ");
}

static void ixdis0_write_repcc(struct crudasm_intel_disasm_context_t *context)
{
	if(context->icode->lockrep == 2)
		ixdis1_write(context, "repnz ");
	else
	if(context->icode->lockrep == 3)
		ixdis1_write(context, "repz ");
}

static void ixdis1_write_size_suffix(struct crudasm_intel_disasm_context_t *context, U1 size)
{
	switch(size)
	{
	case 2:
		ixdis1_write(context, "w");
		break;
	case 4:
		ixdis1_write(context, "d");
		break;
	case 8:
		ixdis1_write(context, "q");
		break;
	case 16:
		ixdis1_write(context, "x");
		break;
	case 1:
		ixdis1_write(context, "b");
		break;
	default:
		ixdis1_write(context, "<unknown size>");
		break;
	}
}

static void ixdis1_write_size_suffix_argsize(struct crudasm_intel_disasm_context_t *context, U4 argnum)
{
	ixdis1_write_size_suffix(context, context->icode->argsize[argnum]);
}

static void ixdis0_write_size_suffix_osz(struct crudasm_intel_disasm_context_t *context)
{
	ixdis1_write_size_suffix(context, 2 << context->icode->osz);
}

static void ixdis0_write_seg_reg(struct crudasm_intel_disasm_context_t *context)
{
	if(context->icode->ea.sreg <= 5)
	{
		do_write_seg_reg(context, context->icode->ea.sreg);
		ixdis1_write(context, " ");
	}
}

// write the 1st argument if osz is 64 bits, else write 2nd argument.
static void ixdis2_write_osz64(struct crudasm_intel_disasm_context_t *context, const char *arg0, const char *arg1)
{
	ixdis1_write(context, (context->icode->osz == 2) ? arg0 : arg1);
}

static void ixdis0_write_o16_o32_o64(struct crudasm_intel_disasm_context_t *context)
{
	if(context->icode->osz == 0)
		ixdis1_write(context, "o16 ");
	else
	if(context->icode->osz == 1)
		ixdis1_write(context, "o32 ");
	else
		ixdis1_write(context, "o64 ");
}

static void ixdis0_write_stack_o16_o32_o64(struct crudasm_intel_disasm_context_t *context)
{
	if(context->dsz == 2)
	{
		// 64bit mode.
		if(context->icode->osz == 0)
			ixdis1_write(context, "o16 ");
	}
	else
	if(context->dsz != context->icode->osz)
	{
		ixdis0_write_o16_o32_o64(context);
	}
}

static void ixdis3_do_nop_xchg(struct crudasm_intel_disasm_context_t *context, const char *nop, const char *xchg, int argnum)
{
	if(context->icode->argvalue[argnum] == 0 && context->icode->is_nop && context->icode->argtype[0] == crudasm_intel_argtype_reg_gr)
		ixdis1_write(context, nop);
	else
	{
		ixdis1_write(context, xchg);
		ixdis0_space(context);
		ixdis0_write_args(context);
	}
}

static void ixdis0_write_xlat_o16_o32_o64(struct crudasm_intel_disasm_context_t *context)
{
	if(context->dsz == 2)
	{
		// 64bit mode.
		if(context->icode->osz != 1)
			ixdis0_write_o16_o32_o64(context);
	}
	else
	if(context->dsz != context->icode->osz)
	{
		ixdis0_write_o16_o32_o64(context);
	}
}

// if osz == 0, write 1st argument.
// if osz == 1, write 2nd argument.
// if osz == 2, write 3rd argument.
static void ixdis3_write_osz(struct crudasm_intel_disasm_context_t *context, const char *arg0, const char *arg1, const char *arg2)
{
	if(context->icode->osz == 0)
		ixdis1_write(context, arg0);
	else
	if(context->icode->osz == 1)
		ixdis1_write(context, arg1);
	else
		ixdis1_write(context, arg2);
}

// if asz == 0, write 1st argument.
// if asz == 1, write 2nd argument.
// if asz == 2, write 3rd argument.
static void ixdis3_write_asz(struct crudasm_intel_disasm_context_t *context, const char *arg0, const char *arg1, const char *arg2)
{
	if(context->icode->asz == 0)
		ixdis1_write(context, arg0);
	else
	if(context->icode->asz == 1)
		ixdis1_write(context, arg1);
	else
		ixdis1_write(context, arg2);
}

static void ixdis0_write_jrcxz_o16_o32_o64(struct crudasm_intel_disasm_context_t *context)
{
	if(context->dsz != 2)
	{
		if(context->icode->osz != context->dsz)
			ixdis0_write_o16_o32_o64(context);
	}
}

static void ixdis0_write_far_imm(struct crudasm_intel_disasm_context_t *context)
{
	// This is not used in 64bit mode.
	ixdis1_write_asm_size(context, 2 << context->icode->osz);
	//write(" far ");
	ixdis1_write(context, " ");
	do_write_imm(context, context->icode->argsize[1], (U4)(U2)(context->icode->disp));
	ixdis1_write(context, ":");
	do_write_imm(context, context->icode->argsize[0], context->icode->imm);
}

static void ixdis1_maybe_write_oxx(struct crudasm_intel_disasm_context_t *context, U1 dsz)
{
	int i;
	int print = context->dsz != context->icode->osz;
	if(context->dsz == 2)
	{
		print = context->icode->osz != 1;
	}
	if(print)
	{
		for(i = 0; i < 4; ++i)
		{
			if(crudasm_intel_argsize__osz_sensitive(context->encoding->argsize[i]))
				return;
		}
		if(context->encoding->op66 == 0)
		{
			ixdis0_write_o16_o32_o64(context);
		}
	}
}

#include "../generated/out_intel_disasm_nasm.h"

void crudasm_intel_disasm(struct ix_icode_t *icode, U8 rel, char *destT, const char *immT, const char *dispT, U1 dsz)
{
	struct crudasm_intel_disasm_context_t context;
	
	destT[0] = '\0';
	
	if(icode->encoding == 0xffffffff)
	{
		// Don't try to disassemble invalid opcodes.
		return;
	}
	
	context.icode = icode;
	context.rel = rel;
	context.dest = destT;
	context.imm = immT;
	context.disp = dispT;
	context.dsz = dsz;
	context.insn = crudasm_intel_insns + (size_t)(crudasm_intel_encoding_table[icode->encoding].insn);
	context.encoding = crudasm_intel_encoding_table + icode->encoding;
	
	if(icode->lockrep == 1)
		ixdis1_write(&context, "lock ");
	if(icode->fwait == 2)  /* fwait used, is fwaitable */
		ixdis1_write(&context, "f");
	else
	if(icode->fwait == 1)  /* fwait not used, but is fwaitable */
		ixdis1_write(&context, "fn");
	
	if(!crudasm_intel_disasm_special_nasm(&context, crudasm_intel_encoding_table[icode->encoding].insn))
	{
		// No special handling for this instruction. Do a default disassembly.

		// print o16/o32/o64 if needed.
		ixdis1_maybe_write_oxx(&context, dsz);
		ixdis1_write(&context, context.insn->alias);
		ixdis0_write_any_args(&context);
	}
}

//===============================================================================================//

#if 0
#include "ixdecoder.h"

int main(int argc, const char **argv)
{
	char buf1[32] = {0x41, 0xb8, 0x01, 0x17};
	char buf2[32] = {0xf2, 0x9b, 0xff, 0xff};  // rep wait ud
	struct ix_icode_t icode;
	const char *status;
	char dis[1024];
	
	status = crudasm_intel_decode(&icode, 32, 0, buf1, NULL);
	crudasm_intel_disasm(&icode, 0, dis, NULL, NULL, 0 /* 16 bit mode */);
	printf("[%s]\n", dis);
	
	status = crudasm_intel_decode(&icode, 32, 0, buf2, NULL);
	crudasm_intel_disasm(&icode, 0, dis, NULL, NULL, 0 /* 16 bit mode */);
	printf("[%s]\n", dis);
	
	return 0;
}
#endif


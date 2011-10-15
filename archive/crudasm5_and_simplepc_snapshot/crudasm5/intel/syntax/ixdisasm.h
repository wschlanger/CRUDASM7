// ixdisasm.h
// Copyright (C) 2011 Willow Schlanger

#ifndef l_ixdisasm_h__ceres_included
#define l_ixdisasm_h__ceres_included

#include "ixsyntax.h"
#include "ixdecoder.h"
#include <string>
#include <cstdio>
#include <cstring>

class IntelDisassembler
{
protected:
	class IntelDecoder *decoder;
	U4 personality;
	U4 symbol_all;
	U4 symbol_imm64_disp;
	U4 symbol_imm64_sx32;
	U4 symbol_ctrlxfer;
	U4 symbol_relative;

	U8 rel;
	char *dest;
	const char *imm, *disp;
	ix_decoder_state *state;
	ix_icode_t *icode;
	int dsz;

#define MKSYMBOL(a) U4 symbol_##a
		MKSYMBOL(write);
		MKSYMBOL(space);
		MKSYMBOL(maybe_write_space_args_imm32);
		MKSYMBOL(write_args);
		MKSYMBOL(write_arg);
		MKSYMBOL(write_rep);
		MKSYMBOL(write_repcc);
		MKSYMBOL(write_size_suffix_argsize);
		MKSYMBOL(write_size_suffix_osz);
		MKSYMBOL(write_seg_reg);
		MKSYMBOL(comma);
		MKSYMBOL(write_osz64);
		MKSYMBOL(write_stack_o16_o32_o64);
		MKSYMBOL(do_nop_xchg);
		MKSYMBOL(write_xlat_o16_o32_o64);
		MKSYMBOL(write_osz);
		MKSYMBOL(write_asz);
		MKSYMBOL(write_far_imm);
		MKSYMBOL(write_jrcxz_o16_o32_o64);
#undef MKSYMBOL

public:
	// The purpose of this method is to validate disassembler methods (name and argument counts).
	// This is done at XML load time, in order to check for errors.
	static bool isValidIdent(std::string ident, int num_args);

	IntelDisassembler(class IntelDecoder *decoderT, std::string personalityT = "nasm") :
		decoder(decoderT)
	{
		personality = decoder->syntax->getSymbol(personalityT);
		symbol_all = decoder->syntax->getSymbol("all");
		symbol_imm64_disp = decoder->syntax->getSymbol("imm64_disp");
		symbol_imm64_sx32 = decoder->syntax->getSymbol("imm64_sx32");
		symbol_ctrlxfer = decoder->syntax->getSymbol("ctrlxfer");
		symbol_relative = decoder->syntax->getSymbol("relative");

#define MKSYMBOL(a) symbol_##a = decoder->syntax->getSymbol(#a)
		MKSYMBOL(write);
		MKSYMBOL(space);
		MKSYMBOL(maybe_write_space_args_imm32);
		MKSYMBOL(write_args);
		MKSYMBOL(write_arg);
		MKSYMBOL(write_rep);
		MKSYMBOL(write_repcc);
		MKSYMBOL(write_size_suffix_argsize);
		MKSYMBOL(write_size_suffix_osz);
		MKSYMBOL(write_seg_reg);
		MKSYMBOL(comma);
		MKSYMBOL(write_osz64);
		MKSYMBOL(write_stack_o16_o32_o64);
		MKSYMBOL(do_nop_xchg);
		MKSYMBOL(write_xlat_o16_o32_o64);
		MKSYMBOL(write_osz);
		MKSYMBOL(write_asz);
		MKSYMBOL(write_far_imm);
		MKSYMBOL(write_jrcxz_o16_o32_o64);
#undef MKSYMBOL
	}

	virtual ~IntelDisassembler() { }

	// destT should be an array of at least 1024 bytes + strlen(immT) + strlen(dispT), or longer.
	void disasm(char *destT, const char *immT, const char *dispT, U8 relT, ix_decoder_state *stateT, ix_icode_t *icodeT, int dszT);

protected:

	void write(const char *s)
	{
		using namespace std;
		strcat(dest, s);
		dest += strlen(s);
	}

	void write_any_args()
	{
		if(icode->argtype[0] != IntelArgTypes::Tvoid)
		{
			space();
			write_args();
		}
	}

	// If this returns true, then that means we've handled the instruction.
	bool specialHandler()
	{
		IntelSyntaxInsn *insn = decoder->syntax->getInsn(decoder->syntax->getEncoding(state->encoding_index)->insn);

		U4 p = personality;
		if(insn->disasm.find(p) == insn->disasm.end())
		{
			p = symbol_all;
			if(insn->disasm.find(symbol_all) == insn->disasm.end())
				return false;
		}

		std::list<IntelSyntaxDisStmt> &dlst = insn->disasm[p];

		for(std::list<IntelSyntaxDisStmt>::iterator i = dlst.begin(); i != dlst.end(); ++i)
		{
			handle_stmt(*i);
		}

		return true;
	}

	void space()
	{
		write(" ");
	}

	void comma()
	{
		write(",");
	}

	void write_args()
	{
		for(int i = 0; i < 4; ++i)
		{
			if(icode->argtype[i] == IntelArgTypes::Tvoid)
				break;
			if(i != 0)
				comma();
			write_arg(i);
		}
	}

	void maybe_write_oxx(int dsz)
	{
		IntelSyntaxInsn *insn = decoder->syntax->getInsn(decoder->syntax->getEncoding(state->encoding_index)->insn);
		IntelSyntaxEncoding *encoding = decoder->syntax->getEncoding(state->encoding_index);

		bool print = dsz != icode->osz;
		if(dsz == 2)
		{
			print = icode->osz != 1;
		}
		if(print)
		{
			int i;
			for(i = 0; i < 4; ++i)
			{
				if(IntelArgSizes::argsize_osz_sensitive(encoding->argsize[i]))
					return;
			}
			if(encoding->op66 == 0)
			{
				write_o16_o32_o64();
			}
		}
	}

	void write_o16_o32_o64()
	{
		if(icode->osz == 0)
			write("o16 ");
		else
		if(icode->osz == 1)
			write("o32 ");
		else
			write("o64 ");
	}

	void handle_stmt(IntelSyntaxDisStmt &stmt);
	void write_arg(int arg, bool need_size = true, bool xoverride = false, bool no_short = false);

	void write_size_suffix(S1 size)
	{
		switch(size)
		{
		case 2:
			write("w");
			break;
		case 4:
			write("d");
			break;
		case 8:
			write("q");
			break;
		case 16:
			write("x");
			break;
		case 1:
			write("b");
			break;
		default:
			write("<unknown size>");
			break;
		}
	}

	void write_seg_reg(bool with_space = true)
	{
		if(icode->ea.sreg <= 5)
		{
			do_write_seg_reg(icode->ea.sreg);
			if(with_space)
				write(" ");
		}
	}

	void do_write_seg_reg(unsigned value)
	{
		const char *mysreg[8] = {"es", "cs", "ss", "ds", "fs", "gs", "ss", "ds"};
		if(value < 8)
		{
			write(mysreg[value]);
		}
		else
			write("<unknown seg reg>");
	}

	void write_far_imm();

	void write_asm_size(int s)
	{
		switch(s)
		{
			case 2:
				write("word");
				break;
			case 4:
				write("dword");
				break;
			case 8:
				write("qword");
				break;
			case 16:
				write("xword");
				break;
			case 10:
				write("tword");	// is this right ?
				break;
			case 1:
				write("byte");
				break;
			default:
				write("<unsupported size>");
				break;
		}
	}

	void do_write_imm(U4 size, U8 imm_value)
	{
		if(size == 1)
			dest += std::sprintf(dest, "0x%02x", (U4)(U1)(imm_value));
		else
		if(size == 2)
			dest += std::sprintf(dest, "0x%04x", (U4)(U2)(imm_value));
		else
		if(size == 4)
			dest += std::sprintf(dest, "0x%08x", ((U4)(imm_value)));
		else
		if(size == 8)
			dest += std::sprintf(dest, "0x%08x%08x", ((U4)((imm_value >> 16) >> 16)), ((U4)(imm_value)));
		else
			write("<unsupported immediate size>");
	}
};

#endif	// l_ixdisasm_h__ceres_included

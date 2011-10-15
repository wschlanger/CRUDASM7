// asmwriter.h
// Copyright (C) 2008 Willow Schlanger

#ifndef l_crudasm__asmwriter_h__included
#define l_crudasm__asmwriter_h__included

#include "../x86s/types.h"
#include "../x86s/x86s_common.h"
#include "../x86s/x86s_decode.h"

#include <cstdio>

namespace x86s
{

class AsmWriter
{
	char *out;
	decode_state_t *s;
	const char *imm;
	const char *disp;
	U8 rel;

	public:
	virtual ~AsmWriter() { }
	void disasm(char *dest, decode_state_t *s, const char *imm, const char *disp, U8 rel_t);
	protected:
	virtual bool special_insn() = 0;
	U4 get_insn() { return s->icode->insn; }
	U2 basecode() { return s->basecode; }
	void write(const char *s)
	{
		while(*s != '\0')
		{
			*out = *s;
			++s;
			++out;
		}
		*out = '\0';
	}
	void space()
	{
		// bug fix from release 0.04: change \t to two spaces.
		write("  ");
	}
	void comma()
	{
		write(",");
	}
	const char *get_imm() { return imm; }
	const char *get_disp() { return disp; }
	U4 get_rel() { return rel; }
	bool is_sx() { return encodings[s->encoding].suffix.sx != sx_def;  }
	bool is_ctrlxfer() { return insns[s->icode->insn].suffix.ctrlxfer == ctrlxfer_yes; }
	bool is_jmp() { return s->icode->insn == insn_jmp; }
	int get_osz()
	{
		return s->icode->osz;
	}
	int get_asz()
	{
		return s->icode->asz;
	}
	int get_dsz()
	{
		return s->dsz;
	}
	void write_arg(int arg)
	{
		// Do not write operand size if endcoding's operand has size 'osz'
		// and a register argument also exists with size 'osz'. For immediates
		// with size osz, do not write operand size if a memory argument
		// exists also with size osz. This handles these general cases:
		// OP reg,mem/imm -> OP reg,<no size> mem/imm
		// OP mem/imm,reg -> OP <no size> mem/imm.reg
		// OP mem,imm     -> OP mem,<no size> imm
		// Thus  jmp target -> jmp dword target (for example).
		// Note: this will not suppress e.g. 'add byte [eax],al'.
		bool suppress = false;
		
		if(insns[s->icode->insn].suffix.size == size_none ||
			insns[s->icode->insn].suffix.size == size_none_dsz
		)
			suppress = true;
		
		U8 imm_value = (U8)s->icode->imm | ((U8)s->icode->disp << (U8)32);
		bool has_reg = false;
		for(int i = 0; i < MAX_ARGS; ++i)
		{
			if(get_argtype_lo(encodings[s->encoding].argtype[i]) == argtype_reg)
			{
				has_reg = true;
				break;
			}
		}
		for(int i = 0; i < arg; ++i)
		{
			if(get_argtype_lo(encodings[s->encoding].argtype[i]) == argtype_imm)
			{
				// this argument is the second immediate argument - use displacement.
				i = arg;
				imm_value = s->icode->disp;		// fixme--check this!
			}
		}
		
		if(get_argtype_lo(encodings[s->encoding].argtype[arg]) == argtype_imm)
		{
			if(insns[s->icode->insn].suffix.size == size_same)
				suppress = true;
			// if this is a ctrl xfer, suppress default size.
			//if(is_ctrlxfer() && get_osz() == get_dsz())
			//	suppress = true;
		}
		
		if(!suppress)
		{
			if(insns[s->icode->insn].suffix.size == size_same)
				if(has_reg)
					suppress = true;
		}
		
		//write_operand((UINT)(U1)encodings[s->encoding].argtype[arg], encodings[s->encoding].argsize[arg], argvalue(arg), suppress, imm_value);
		write_operand(s->icode->argtype[arg], s->icode->argsize[arg], argvalue(arg), suppress, imm_value);
	}
	void write_args()
	{
		for(int i = 0; i < 3; ++i)
		{
			if(get_argtype_lo(encodings[s->encoding].argtype[i]) == argtype_void)
				break;
			if(i != 0)
				comma();
			write_arg(i);
		}
	}
	void write_suffix(int size)
	{
		if(size == argsize_8)
			write("b");
		else
		if(size == argsize_16)
			write("w");
		else
		if(size == argsize_32)
			write("d");
		else
			write("q");
	}
	void write_cc(int value)
	{
		//const char *cc[] = {"o","no","b","nb","z","nz","be","nbe","s","ns","p","np","l","nl","le","nle"};
		// These cc's are the ones that make the most sense sense to me.
		//const char *cc[16] = {"o","no","b","ae","e","ne","be","a","s","ns","p","np","l","ge","le","g"};
		const char *cc[16] = {"o","no","c","nc","z","nz","be","a","s","ns","p","np","l","ge","le","g"};
		write(cc[value]);
	}
	int get_sreg()
	{
		return s->icode->ea.sreg;
	}
	int get_ea_base()
	{
		if(s->icode->ea.base == 31)
			return -1;
		return s->icode->ea.base & 15;
	}
	int get_ea_index()
	{
		if(s->icode->ea.index == 31)
			return -1;
		return s->icode->ea.index & 15;
	}
	int get_ea_scale()
	{
		return s->icode->ea.index_scale & 3;
	}
	bool has_disp8()
	{
		return s->icode->ea.disp8;
	}
	// Ignore the upper 32 bits of this unless asz is size_64.
	U8 get_disp64()
	{
		return (U8)(s->icode->disp) | ((U8)(s->icode->imm) << (U8)(32));
	}
	// Ignore the upper 32 bits of this unless osz is size_64 (and you really
	// know what you're doing!)
	U8 get_imm64()
	{
		return (U8)(s->icode->imm) | ((U8)(s->icode->disp) << (U8)(32));
	}
	int get_bits()
	{
		return s->dsz & 3;
	}
	int argsize_lo(int arg) { return (int)(unsigned)get_argsize_lo(s->icode->argsize[arg]); }
	//int get_full_argsize(int arg) { return (int)(unsigned)s->icode->argsize[arg]; }
	U1 argvalue(int arg) { return s->icode->argvalue[arg]; }
	void write_operand(int type, int size, int value, bool suppress_size = false, U8 imm_value = 0);
	
	// --- begin new things ---
	void write_seg_reg()
	{
		const char *mysreg[6] = {"es", "cs", "ss", "ds", "fs", "gs"};
		if(s->icode->ea.sreg < 6)
		{
			write(mysreg[s->icode->ea.sreg]);
			write(" ");
		}
	}
	void write_rep()
	{
		if(s->icode->lockrep == 2 || s->icode->lockrep == 3)
			write("rep ");
	}
	void write_repcc()
	{
		if(s->icode->lockrep == 2)
			write("repnz ");
		else
		if(s->icode->lockrep == 3)
			write("repz ");
	}
	void write_byte_size(int s)
	{
		if(s == argsize_16)
			write("word");
		else
		if(s == argsize_32)
			write("dword");
		else
		if(s == argsize_64)
			write("qword");
		else
		if(s == argsize_128)
			write("xword");
		else
		if(s == argsize_8)
			write("byte");
		else
			write("<unsupported size>");
	}
	void write_far(bool is_imm = false)
	{
		if(is_imm)
		{
			write_byte_size(s->icode->osz);
			write(" ");
		}
		else
		{
			if(s->icode->osz != s->dsz)
			{
				write_byte_size(s->icode->osz);
				write(" ");
			}
			write("far ");
		}
	}
	void do_write_imm(int size, U8 imm_value)
	{
		if(size == argsize_8)
		{
			out += std::sprintf(out, "0x%02x", (U4)(U1)imm_value);
		}
		else
		if(size == argsize_16)
		{				
			out += std::sprintf(out, "0x%04x", (U4)(U2)imm_value);
		}
		else
		if(size == argsize_32)
		{				
			out += std::sprintf(out, "0x%08x", (U4)imm_value);
		}
		else
		{
			write("<unimplemented immediate size>");
		}
	}
	void write_far_imm()
	{
		write_far(true);
		do_write_imm(get_argsize_lo(s->icode->argsize[1]), (U4)(U2)s->icode->disp);
		write(":");
		do_write_imm(get_argsize_lo(s->icode->argsize[0]), (U4)s->icode->imm);
	}
	void write_size_suffix(U1 size)
	{
		if(size == argsize_8)
			write("b");
		else
		if(size == argsize_16)
			write("w");
		else
		if(size == argsize_32)
			write("d");
		else
		if(size == argsize_64)
			write("q");
	}
	// --- end new things ---
};

class NasmWriter :
	public AsmWriter
{
	public:
	virtual bool special_insn();
	private:
	void write_size();
};

}	// namespace x86s

#endif	// l_crudasm__asmwriter_h__included

// asmwriter.cpp
// Copyright (C) 2008 Willow Schlanger

#include "asmwriter.h"

#include <cstring>
#include <cstdio>

#ifdef _MSC_VER
#define STD
#else
#define STD std
#endif

namespace x86s
{

// This assumes s->encoding != 0xffffff.
void AsmWriter::disasm(char *dest, decode_state_t *s_t, const char *imm_t, const char *disp_t, U8 rel_t)
{
	//const char *strsreg[8] = {"es", "cs", "ss", "ds", "fs", "gs", "ss", "ds"};

	rel = rel_t;
	out = dest;
	s = s_t;
	imm = imm_t;
	disp = disp_t;

	// if insn is 'osz' sensitive, see if the encoding has any 'osz-sensitive arguments'.
	// if not, print o16/o32/o64 here if different from default mode.
	
	if(s->icode->lockrep == 1)
		write("lock ");
	
	if(insns[s->icode->insn].suffix.size == size_none_dsz && s->dsz != s->icode->osz)
	{
#if 0
		int i;
		for(i = 0; i < MAX_ARGS; ++i)
			if(arg_is_osz_sensitive(get_full_argsize(i)))
				break;
		if(i == MAX_ARGS)
#endif
		{
			if(s->icode->osz == argsize_16)
				write("o16 ");
			else
			if(s->icode->osz == argsize_32)
				write("o32 ");
			else
			if(s->icode->osz == argsize_64)
				write("o64 ");
		}
	}

	if(!special_insn())
	{
		if((const char *)(insn_strings[s->icode->insn]) == 0)
		{
			write("<unknown opcode>");
		}
		else
		{
			write((const char *)(insn_strings[s->icode->insn]));
			if(get_argtype_lo(encodings[s->encoding].argtype[0]) != argtype_void)
			{
				space();
				write_args();
			}
		}
	}
}

// *** I don't understand this imm_value thing. Figure it out for 64-bit support.
void AsmWriter::write_operand(int type, int size, int value, bool suppress_size, U8 imm_value)
{
	const char *strsize[4] = {"word", "dword", "qword", "xword"};
	const char *strsreg[8] = {"es", "cs", "ss", "ds", "fs", "gs", "ss", "ds"};
	// To do: add 64-bit register support.
	const char *regs8[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
	const char *regs16[8] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
	const char *regs32[8] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};;

	if(get_argtype_lo(type) == argtype_reg)
	{
		switch(get_argtype_hi(type))
		{
			case argtypehi_reg_r:
			case argtypehi_reg_basecode:
				// *** add 64-bit support here.
				if(value < 0 || value > 8)
					write("<unknown>");
				else
				if(get_argsize_lo(size) == argsize_8)
					write(regs8[value]);
				else
				if(get_argsize_lo(size) == argsize_16)
					write(regs16[value]);
				else
				if(get_argsize_lo(size) == argsize_32)
					write(regs32[value]);
				else
					write("<unknown>");
				break;
			case argtypehi_reg_sr:
				if(value < 0 || value > 6)
					write("<unknown>");
				else
					write(strsreg[value]);
				break;
			case argtypehi_reg_cr:
				write("cr");
				out += STD::sprintf(out, "%d", value);
				break;
			case argtypehi_reg_dr:
				write("dr");
				out += STD::sprintf(out, "%d", value);
				break;
			default:
				write("<unknown>");
				break;
		}
		return;
	}
	else
	if(get_argtype_lo(type) == argtype_mem)
	{
		if(!suppress_size)
		{
			// fixme--how to describe structure sizes? fword, tword, whatever-word (!)
		
			if(get_argsize_lo(size) < 3)
			{
				write(strsize[get_argsize_lo(size)]);
				if(is_ctrlxfer() && get_argsize_hi(size) == argsize__end)
					write(" near");
			}
			else
			if(get_argsize_lo(size) == argsize_8)
				write("byte");
			else
				write("<unsupported size>");
			
			if(is_ctrlxfer())
			{
				if(get_argsize_hi(size) != argsize__end)
					write(" far");
			}
			else
			if(get_argsize_hi(size) != argsize__end)
			{
				write(":");
				if(get_argsize_hi(size) < 3)
					write(strsize[get_argsize_hi(size)]);
				else
				if(get_argsize_hi(size) == argsize_8)
					write("byte");
				else
					write("<unsupported size>");
			}
			
			write(" ");
		}
		type = get_argtype_hi(type);
		size = get_argsize_lo(size);
		
		if(type == argtypehi_mem_mem || type == argtypehi_mem_disp)
		{
		// --- begin memory code ---
				write("[");

				// write any segment register override.
				if(get_sreg() <= 5)
				{
					write(strsreg[get_sreg()]);
					write(":");
				}
				
				// write any size override, e.g. mov eax,[word 0x5555] in 32-bit mode.
				if(get_asz() != get_bits())
				{
					// bug fix from release 0.04: don't write size if it's an rm and there's a register.
					// the size of the register indicates the size.
					if(type == argtypehi_mem_disp || (get_ea_base() == -1 && get_ea_index() == -1))
					{
						write(strsize[get_asz()]);
						write(" ");
					}
					// end bug fix.
				}
				
				if(get_disp() != NULL)
				{
					write(get_disp());
				}
				else
				{
					bool need_plus = false;
					if(type == argtypehi_mem_mem)
					{
						// write ea.
						
						const char *base = NULL;
						const char *index = NULL;
						
						// fixme--add 64bit support here.

						if(get_asz() == argsize_16)
						{
							if(get_ea_base() != -1)
								base = regs16[get_ea_base()];
						}
						else
						{
							if(get_ea_base() != -1)
								base = regs32[get_ea_base()];
						}						

						if(get_asz() == argsize_16)
						{
							if(get_ea_index() != -1)
								index = regs16[get_ea_index()];
						}
						else
						{
							if(get_ea_index() != -1)
								index = regs32[get_ea_index()];
						}						
						
						if(base != NULL)
						{
							write(base);
							need_plus = true;
						}
						if(index != NULL)
						{
							need_plus = true;
							if(base != NULL)
								write("+");
							write(index);
							if(get_ea_scale() != 0)
								out += sprintf(out, "*%x", 1 << get_ea_scale());
						}		
					}
					
					// write disp here.
					// if need a minus, show it.
					// else, show + if need_plus is true.
					
					if(type == argtypehi_mem_mem && has_disp8())
					{
						if((get_disp64() & 0xff) < 0x80)
						{
							if(need_plus)
								write("+");
							out += sprintf(out, "0x%02lx", get_disp64() & 0xff);
						}
						else
							out += sprintf(out, "-0x%02lx", 0x100 - (get_disp64() & 0xff));
					}
					else
					// fixme--add 64-bit displacement support here.
					if((U4)(get_disp64()) != 0 || (get_ea_index() == -1 && get_ea_base() == -1) || (type == argtypehi_mem_disp))
					{
						if(need_plus)
							write("+");
						if(get_asz() == argsize_16)
							out += sprintf(out, "0x%04x", (U4)get_disp64());
						else
						if(get_asz() == argsize_32)
							out += sprintf(out, "0x%08x", (U4)get_disp64());
						else
							write("<unsupported displacement size>");
					}
				}
				
				write("]");
		// --- end memory code ---
		}
		else
			write("<unknown>");
	}
	else
	if(get_argtype_lo(type) == argtype_imm)
	{
		if(is_sx())
		{
			if(is_ctrlxfer())
			{
				if(is_jmp())
					write("short ");
			}
			else
				write("byte ");
		}
		else
		if(!suppress_size)
		{
			if(get_argsize_lo(size) < 3)
			{
				write(strsize[get_argsize_lo(size)]);
			}
			else
			if(get_argsize_lo(size) == argsize_8)
				write("byte");
			write(" ");
		}
		
		type = get_argtype_hi(type);
		size = get_argsize_lo(size);

		// fixme--add support for other sizes here.
		// note: sometimes size is argsize_32_16 etc. We handle
		// these specially.
		/*else
		{
			out += STD::sprintf(out, "<--bad size %d-->", size);
		}*/

		if(type == argtypehi_imm_implict8)
			imm_value = (value & 0xff);
		
		// now ok to use imm_value.
		// fixme--add 64-bit support.
		
		// fixme --  use 'imm' field here.
		
		if(get_imm() != NULL)
		{
			write(get_imm());
			return;
		}
		
		if(type == argtypehi_imm_rel)
		{
			imm_value += get_rel();
			// bugfix from version 0.03: and by 0xffff.
			if(get_osz() == argsize_16)
				imm_value &= 0xffff;
		}
		
		if(is_sx() && !is_ctrlxfer())
		{
			if((imm_value & 0xff) < 0x80)
				out += sprintf(out, "+0x%02lx", imm_value & 0xff);
			else
				out += sprintf(out, "-0x%02lx", 0x100 - (imm_value & 0xff));
		}
		else
			do_write_imm(size, imm_value);
		
		return;
	}
	else
	if(get_argtype_lo(type) != argtype_void)
	{
		write("<unknown>");
	}
	
	return;
}

// ---

//#include "../x86c/x86c_insn_enums.h"

void NasmWriter::write_size()
{
	if(get_osz() != get_bits())
	{
		if(get_osz() == argsize_16)
			write("o16 ");
		else
		if(get_osz() == argsize_32)
			write("o32 ");
		else
			write("o64 ");
	}
}

// returns true if it was a special instruction.
bool NasmWriter::special_insn()
{
	if(insn_strings[get_insn()] != 0)
		return false;
	switch(get_insn())
	{
#include "../x86s/out_disassemble.h"
		default:
			write("<unknown>");
			return false;
	}
	return true;
}

}	// namespace x86s

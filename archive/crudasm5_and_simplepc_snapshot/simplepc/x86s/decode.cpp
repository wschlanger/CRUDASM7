// decode.cpp
// Copyright (C) 2008 Willow Schlanger

// Search for: fixme
// Unimplemented: decode_get_modrm().

#include "types.h"
#include "x86s_common.h"
#include "x86s_decode.h"
#include "x86s_decode.h"

namespace x86s
{

// This is terminated with an element with an insn number of insn__count
encoding_t encodings[] =
{
#include "out_encodings.h"
};

int search_table[insn__string_count] =
{
#include "out_insn_search_table.h"
};

const char *insn_strings[insn__count] =
{
#include "out_insn_strings.h"
};

insn_def_t insns[insn__count] =
{
#include "out_insns.h"
};

U4 decoder_table[] =
{
#include "out_decoder_table.h"
};

// --- begin decoder ---

// internal
struct decode_prefix_state
{
	U1 lockrep : 2;		// none, f0, f2, f3
	U1 op66 : 1;
	U1 op67 : 1;
	U1 rex_w : 1;
	U1 rex_r : 1;
	U1 rex_x : 1;
	U1 rex_b : 1;

	U1 sreg : 3;		// 0..5 or 7 if none
};

// --- end decoder ---

static bool decode_get_byte(decode_state_t &s, U1 &c);

// returns 1 if limit exceeded.
static int decode_prefix(struct decode_state_t &s, struct decode_prefix_state *p)
{
	U1 c;

	p->lockrep = 0;
	p->op66 = 0;
	p->op67 = 0;
	p->rex_w = 0;
	p->rex_r = 0;
	p->rex_x = 0;
	p->rex_b = 0;
	p->sreg = 7;
	
	// p->size begins as 0 on input.
	
	for(;;)
	{
		if(decode_get_byte(s, c))
			return 1;

		// 001ss110
		if((c & 0xe7) == 0x26)
			p->sreg = ((c >> 3) & 3);
		else
		// 011001xx
		if((c & 0xfc) == 0x64)
		{
			if(c < 0x66)
				p->sreg = (c & 7);
			else
			if(c == 0x66)
				p->op66 = 1;
			else
			// 0x67
				p->op67 = 1;
		}
		else
		// 111100xx
		if((c & 0xfc) == 0xf0)
		{
			if(c == 0xf0)
				p->lockrep = 1;
			else
			if(c == 0xf1)
				break;
			else
			{
				// 0xf2 or 0xf3
				// 0xf0 overrides 0xf2, 0xf3.
				if(p->lockrep == 0)
					p->lockrep = (c & 3);
			}
		}
		else
			break;
		
		// byte was accepted.
		++s.size;
	}
	
	// now if in 64-bit mode, get any REX prefix.
	// note: 'c' is already the next (unaccepted) byte.
	if(s.dsz == argsize_64)
	{
		if((c & 0xf0) == 0x40)
		{
			p->rex_w = (c >> 3) & 1;
			p->rex_r = (c >> 2) & 1;
			p->rex_x = (c >> 1) & 1;
			p->rex_b = (c >> 0) & 1;
		
			// byte was accepted.
			++s.size;
		}
	}
	
	return 0;
}

// --- begin modr/m decode code ---

// FIXME -- Check this.
// 16-bit modr/m's are impossible in long mode (dsz == argsize_64).
static int decode_modrm_16(decode_state_t *s, U1 c, int *disp_size)
{
	U1 mod = (c >> 6) & 3;
	U1 rm = c & 7;
	*disp_size = -1;
	
	if(rm < 6)
		s->icode->ea.index = 6 + (rm & 1);	// [si] [di]
	
	if(rm < 4)
		s->icode->ea.base = 3 + (rm & 2);	// ax cx dx [bx] sp [bp] si di
	else
	if(rm == 6)
	{
		if(mod == 0)
			*disp_size = argsize_16;
		else
			s->icode->ea.base = 5;	// bp
	}
	else
	if(rm == 7)
	{
		s->icode->ea.base = 3;	// bx
	}
	
	if(mod > 0)
		*disp_size = (mod == 1) ? argsize_8 : argsize_16;
	
	return 0;
}

// FIXME -- Check this-> Esp. 64-bit support! Need to add 64 bit REX support here.
// Right? Or do 32-bit modr/m's need that??? Maybe not!
static int decode_modrm_32(decode_state_t *s, U1 c, int *disp_size)
{
	U1 mod = (c >> 6) & 3;
	U1 rm = c & 7;
	U1 ss, i, b;
	*disp_size = -1;

	if(rm == 4)
	{
		// SIB
		if(mod == 1)
			*disp_size = argsize_8;
		else
		if(mod == 2)
			*disp_size = argsize_32;
		if(decode_get_byte(*s, c))
			return 1;
		
		// bug fix -- did we forget to do this anywhere else?
		++s->size;

		ss = (c >> 6) & 3;
		i = (c >> 3) & 7;
		b = c & 7;
		
		if(i != 4)
		{
			s->icode->ea.index = i;
			s->icode->ea.index_scale = ss;
		}
		
		if(b != 5)
		{
			s->icode->ea.base = b;
		}
		else
		{
			if(mod == 0)
				*disp_size = argsize_32;
			else
				s->icode->ea.base = 5;
		}
	}
	else
	if(rm == 5 && mod == 0)
		*disp_size = argsize_32;
	else
	{
		s->icode->ea.base = rm;
		
		// bugfix--is this right yet?
		if(mod == 1)
			*disp_size = argsize_8;
		else
		if(mod == 2)
			*disp_size = argsize_32;
	}

	return 0;
}

static int decode_modrm_64(decode_state_t *s, U1 c, int *disp_size)
{
	return 3;	// unimplemented - report opcode invalid
}

#	include <stdio.h>
static int decode_fetch_many_bytes(struct decode_state_t &s, void *target, U1 size);

// Returns 1 if reached end of code segment.
// 'asz' must be valid when this is called!
static int decode_get_modrm(decode_state_t &s, U1 *first_modrm_byte, struct decode_prefix_state *p)
{
// FIXME -- THIS NEEDS A LOT OF WORK.
	U1 c;
	int disp_size = -1;

	if(decode_get_byte(s, c))
		return 1;
	*first_modrm_byte = c;
	++s.size;
	
	//bugfix--moved to caller
	//s.icode->ea.base = 31;
	//s.icode->ea.index = 31;
	//s.icode->ea.index_scale = 0;
	//s.icode->ea.disp8 = 0;

//---

	if(c < 0xc0)
	{
		int x;
		if(s.icode->asz == argsize_16)
			x = decode_modrm_16(&s, c, &disp_size);
		else
		if(s.icode->asz == argsize_32)
			x = decode_modrm_32(&s, c, &disp_size);
		else
			x = decode_modrm_64(&s, c, &disp_size);

		if(x != 0)
			return x;
		
		// Get displacement here.
		if(disp_size != -1)
		{
			s.icode->has_disp = 1;
			switch(disp_size)
			{
				case argsize_8:
					{
						s.icode->ea.disp8 = 1;
						U1 value;
						int x = decode_fetch_many_bytes(s, &value, argsize_8);
						if(x != 0)
							return x;
						switch(s.icode->asz)
						{
							case argsize_16:
								s.icode->disp = (U4)(U2)(S2)(S1)value;
								break;
							case argsize_32:
								s.icode->disp = (U4)(S4)(S1)value;
								break;
							case argsize_64:
								// note: if ea.disp8 == 1, you have to sign extend
								// the displacement to 64 bits if asz == size_64.
								s.icode->disp = (U4)(S4)(S1)value;
								//s.icode->imm = (value < 0x80) ? (U4)(0) : (U4)(-1);
								break;
							default:
								return 4;
						}
						break;
					//return 0;
					}
				case argsize_16:		// not possible in long mode
				case argsize_32:
					{
						int x = decode_fetch_many_bytes(s, &s.icode->disp, s.icode->asz);
						if(x != 0)
							return x;
						break;
					}
				case argsize_64:
					{
						U8 value;
						int x = decode_fetch_many_bytes(s, &value, argsize_64);
						if(x != 0)
							return x;
						s.icode->imm = (U8)value >> (U8)32;
						s.icode->disp = (U4)value;
						break;
					}
				default:
					return 1;
			}
		}
		// FIXME --- what about rbp/rsp ???
		// --- what about rip-relative addressing???
		// --- I think this is right ....
		if(p->sreg == 7 && s.icode->ea.base != 31 && (s.icode->ea.base & 6) == 4)	// bp, ebp, or esp base
			s.icode->ea.sreg = 6;
	}

	return 0;
}

// --- end modr/m decode code ---

// --- begin rewrite ---

// Returns true if error, false otherwise.
// User must remember to increment s.size.
static bool decode_get_byte(decode_state_t &s, U1 &c)
{
	const U1 *ptr = s.insn + (UINT)s.size;
	if(ptr >= s.end)
		return true;
	c = *ptr;
	return false;		// no error
}

static int decode_fetch_many_bytes(struct decode_state_t &s, void *target, U1 size)
{
	int x;
	U1 c;
	const UINT bytesleft = s.end - (s.insn + (UINT)s.size);
	switch(size)
	{
		case argsize_8:
			x = decode_get_byte(s, c);
			if(x)
				return 1;
			++s.size;
			*(U1 *)(target) = c;
			return 0;
		case argsize_16:
			if(bytesleft < 2)
				return 1;
			*(U2 *)(target) = GET_U2(s.insn + (UINT)s.size);
			s.size += 2;
			return 0;
		case argsize_32:
			if(bytesleft < 4)
				return 1;
			*(U4 *)(target) = GET_U4(s.insn + (UINT)s.size);
			s.size += 4;
			return 0;
		case argsize_64:
			if(bytesleft < 8)
				return 1;
			*(U8 *)(target) = GET_U8(s.insn + (UINT)s.size);
			s.size += 8;
			return 0;
		default:
			break;
	}
	return 1;
}

// This goes thru the arguments for the encoding selected by s.encoding.
// Returns 1 if cs limit exceeded (or internal error), 0 otherwise.
static int decode_get_args(struct decode_state_t &s, struct decode_prefix_state *p, int has_modrm)
{
	int i;
	encoding_t *e = encodings + s.encoding;
	bool immyet = false;

	for(i = 0; i < MAX_ARGS; ++i)
	{
		s.icode->argtype[i] = e->argtype[i];
		s.icode->argsize[i] = e->argsize[i];
		s.icode->argvalue[i] = e->argvalue[i];
	}
	
	for(i = 0; i < MAX_ARGS; ++i)
	{
		if(get_argtype_lo(s.icode->argtype[i]) == argtype_void)
			break;
		s.icode->argsize[i] = specialize_arg_size(s.icode->argsize[i], s.dsz, s.icode->osz, s.icode->asz);
		
		switch(get_argtype_lo(s.icode->argtype[i]))
		{
			case argtype_reg:
				switch(get_argtype_hi(s.icode->argtype[i]))
				{
					case argtypehi_reg_sr:
						if(s.icode->argvalue[i] == 0xff)
							s.icode->argvalue[i] = (s.modrm0 >> 3) & 7;
						break;
					case argtypehi_reg_basecode:
						if(s.icode->argvalue[i] == 0xff)
							s.icode->argvalue[i] = (s.basecode & 7) | ((U1)(p->rex_b) << 4);
						s.icode->argtype[i] = ARGTYPE(argtype_reg, argtypehi_reg_r);
						break;
					case argtypehi_reg_r:
					case argtypehi_reg_cr:
					case argtypehi_reg_dr:
						if(s.icode->argvalue[i] == 0xff)
							s.icode->argvalue[i] = ((s.modrm0 >> 3) & 7) | ((U1)(p->rex_r) << 4);
						break;
					default:
						break;
				}
				break;
			case argtype_mem:
				switch(get_argtype_hi(s.icode->argtype[i]))
				{
					case argtypehi_mem_rm:
						if(s.modrm0 >= 0xc0)
						{
							s.icode->argtype[i] = ARGTYPE(argtype_reg, argtypehi_reg_r);
							s.icode->argvalue[i] = (s.modrm0 & 7) | ((U1)(p->rex_b) << 4);
						}
						else
							s.icode->argtype[i] = ARGTYPE(argtype_mem, argtypehi_mem_mem);
						break;
					case argtypehi_mem_disp:
						s.icode->has_disp = 1;
						switch(s.icode->asz)
						{
							case argsize_16:		// not possible in long mode
							case argsize_32:
								{
									int x = decode_fetch_many_bytes(s, &s.icode->disp, s.icode->asz);
									if(x != 0)
										return x;
								}
								break;
							case argsize_64:
								{
									U8 value;
									int x = decode_fetch_many_bytes(s, &value, argsize_64);
									if(x != 0)
										return x;
									s.icode->imm = (U8)value >> (U8)32;
									s.icode->disp = (U4)value;
								}
								break;
							default:
								return 1;
						}
						break;
					case argtypehi_mem_mem:			// fine as-is
					default:
						break;
				}
				break;
			case argtype_imm:
				switch(get_argtype_hi(s.icode->argtype[i]))
				{
					case argtypehi_imm_imm:
					case argtypehi_imm_rel:
						s.icode->has_imm = 1;
						// imm's never have a hi size -- we use two imm's for e.g. far branch targets etc.
						if(get_argsize_lo(s.icode->argsize[i]) != argsize_8 &&
							e->suffix.sx == sx_yes
						)
						{
							s.icode->sx = 1;
							U1 value;
							int x = decode_fetch_many_bytes(s, &value, argsize_8);
							if(x != 0)
								return x;
							switch(s.icode->osz)
							{
								case argsize_16:
									s.icode->imm = (U4)(U2)(S2)(S1)value;
									break;
								case argsize_32:
									s.icode->imm = (U4)(S4)(S1)value;
									break;
								case argsize_64:
									s.icode->imm = (U4)(S4)(S1)value;
									// note: if s.icode->sx == 1 and an immediate operand size is 64 bits,
									// then you have to sign extend the 32-bit 'imm' to 64 bits.
									//s.icode->disp = (value < 0x80) ? (U4)(0) : (U4)(-1);
									break;
								default:
									return 1;
							}
						}
						else
						if(get_argsize_lo(s.icode->argsize[i]) == argsize_64)
						{
							U8 value;
							int x = decode_fetch_many_bytes(s, &value, argsize_64);
							if(x != 0)
								return x;
							s.icode->imm = (U4)value;
							s.icode->disp = (U8)value >> (U8)32;
						}
						else
						{
							int x = decode_fetch_many_bytes(s, immyet ? &s.icode->disp : &s.icode->imm, get_argsize_lo(s.icode->argsize[i]));
							if(x != 0)
								return x;
						}
						immyet = true;
						break;
					case argtypehi_imm_cc:
						s.icode->argvalue[i] = s.basecode & 0xf;
						break;
					case argtypehi_imm_implict8:	// fine as-is
					default:
						break;
				}
				break;
		}
	}

	return 0;	// unimplemented
}

// --- end rewrite ---

// Return codes.
// 0 : success
// 1 : s.icode, s.basecode, s.size, s.modrm0 undefined.
//     exceeded limit while decoding.
// 2 : invalid opcode - opcode invalid when dsz is 64
// 3 : invalid opcode - general
// 4 : invalid opcode - or internal error!
int decode(decode_state_t &s)
{
	struct decode_prefix_state p;
	U4 basecode;
	int tableofs;				// should this be SINT ???
	U4 a;
	U1 c;
	U1 b;
	U1 has_modrm;
	
	s.encoding = 0xffffff;		// normalize value
	s.size = 0;
	s.icode->disp = 0;
	s.icode->imm = 0;
	s.icode->has_disp = 0;	// bugfix 12/23/2008

	// bugfix 01-03-2009: put these here.
	s.icode->ea.base = 31;
	s.icode->ea.index = 31;
	s.icode->ea.index_scale = 0;
	s.icode->ea.disp8 = 0;

	// get any prefix bytes.
	if(decode_prefix(s, &p))
		return 1;
	
	// get the main opcode byte.	
	if(decode_get_byte(s, c))
		return 1;
	++s.size;
	if(c == 0x0f)
	{
		if(decode_get_byte(s, c))
			return 1;
		++s.size;
		if(c == 0x0f)
			basecode = 0xfff;	// 3D Now! instruction
		else
			basecode = 0x100 + c;
	}
	else
		basecode = c;

	// initialize some things.
	if(s.dsz == argsize_16)
	{
		s.icode->osz = (p.op66) ? argsize_32 : argsize_16;
		s.icode->asz = (p.op67) ? argsize_32 : argsize_16;
	}
	else
	if(s.dsz == argsize_32)
	{
		s.icode->osz = (p.op66) ? argsize_16 : argsize_32;
		s.icode->asz = (p.op67) ? argsize_16 : argsize_32;
	}
	else	// 64-bit mode
	{
		// Note: this is fixed up later (below), after we have
		// decoded the instruction.
		s.icode->asz = (p.op67) ? argsize_32 : argsize_64;
		s.icode->osz = (p.op66) ? argsize_16 : argsize_32;
	}
	s.icode->lockrep = p.lockrep;
	s.icode->ea.sreg = p.sreg;
	
	if(basecode == 0xfff)
	{
		// NOTE: As no 3DNow! instructions exist yet, this code is not yet tested!
		// get modr/m.
		if(decode_get_modrm(s, &c, &p))
			return 1;
		s.modrm0 = c;
		
		if(decode_get_byte(s, b))
			return 1;
		
		basecode = (U4)0x200 + (U4)b;
		
		tableofs = basecode;
		if(p.op66)
			tableofs += 0x300;
		// none, f0, f2, f3
		if(p.lockrep == 2)
			tableofs += 0x600;
		else
		if(p.lockrep == 3)
			tableofs += 0x600 * 2; /* 0xc00 */
		
		a = decoder_table[tableofs] & 0xffffff;
		b = (decoder_table[tableofs] >> 24) & 0xff;
	}
	else
	{
		// see if it has a modr/m or displacement.
		// A base table exists with 0x1200 (4608) entries, all invalid opcodes to begin with.
		// offset = basecode + 0x300 * op66 + 0x600 * repeat
		tableofs = basecode;
		if(p.op66)
			tableofs += 0x300;
		// none, f0, f2, f3
		if(p.lockrep == 2)
			tableofs += 0x600;
		else
		if(p.lockrep == 3)
			tableofs += 0x600 * 2; /* 0xc00 */
		
		a = decoder_table[tableofs] & 0xffffff;
		b = (decoder_table[tableofs] >> 24) & 0xff;
		
		if(b == 4)
		{
			if(decode_get_byte(s, c))
				return 1;
			++s.size;
			tableofs = a + (U4)c;
			a = decoder_table[tableofs] & 0xffffff;
			b = (decoder_table[tableofs] >> 24) & 0xff;
		}
		
		if(b == 0)
		{
			if(a == 0xffffff)
			{
				s.size = 0;
				return 3;
			}
			has_modrm = encodings[a].suffix.ro != ro_def;
		}
		else
			has_modrm = 1;
		
		// now get modr/m if there is one.
		// we do not hceck for an argument implying rm because we can just
		// check for ro_def which is used if and only if there is no modr/m.
		if(has_modrm)
		{
			if(decode_get_modrm(s, &c, &p))
				return 1;
			s.modrm0 = c;
		}
	}
	
	// 'b' could be nonzero. If so, we have to find the correct encoding based
	// on parsed 'c' (first modr/m byte).
	
	if(b == 1)
	{
		tableofs = a + ((c >> 3) & 7);
		a = decoder_table[tableofs] & 0xffffff;
		b = (decoder_table[tableofs] >> 24) & 0xff;
	}
	
	if(b == 2)
	{
		tableofs = a + ((c >> 6) & 3);
		a = decoder_table[tableofs] & 0xffffff;
		b = (decoder_table[tableofs] >> 24) & 0xff;
	}
	
	if(b == 3)
	{
		tableofs = a + (c & 7);
		a = decoder_table[tableofs] & 0xffffff;
		b = (decoder_table[tableofs] >> 24) & 0xff;
	}
	
	// demand 'b' is 0 now, after walking thru the table.
	if(b != 0 || a == 0xffffff)
	{
		s.size = 0;
		return 4;
	}
	s.encoding = a;
	
	// if no64 and in 64 bit mode, report failure here.
	if(s.dsz == argsize_64 && encodings[s.encoding].suffix.o == o_no64)
	{
		s.encoding = 0xffffff;
		s.size = 0;
		return 2;
	}
	
	s.icode->insn = encodings[s.encoding].insn;

	if(p.rex_w || (s.dsz == argsize_64 && encodings[s.encoding].suffix.o == o_is64 && !p.op66))
		s.icode->osz = argsize_64;
	s.basecode = basecode;

	// Go thru all arguments, getting any immediates or the displacement operand.
	//s.icode->has_disp = 0;	// bugfix 12/23/2008
	s.icode->has_imm = 0;
	s.icode->sx = 0;

	int xx = decode_get_args(s, &p, has_modrm);
	
	return xx;
}

}	// namespace x86s

#if 0
#include <iostream>

using namespace x86s;

int main()
{
	
	U1 code[] = {0xb8, 0x00, 0x00};
	
	icode_t icode;
	decode_state_t decoder;
	decoder.icode = &icode;
	decoder.insn = code;
	decoder.end = code + sizeof(code);
	decoder.dsz = argsize_16;
	
	int x = decode(decoder);
	
	std::cout << "decode() returned " << x << std::endl;

	if(x == 0)
	{
		std::cout << "size = " << (UINT)decoder.size << std::endl;
		std::cout << "encoding = " << decoder.encoding << std::endl;
		std::cout << "insn enum = " << encodings[decoder.encoding].insn << std::endl;
		const char *s = insn_strings[encodings[decoder.encoding].insn];
		std::cout << "insn name = " << ((s == 0) ? "(null)" : s) << std::endl;
	}
	
	return 0;
}
#endif

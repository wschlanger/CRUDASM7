// ixdecoder.cpp
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "ixdecoder.h"
#include "ixsyntax.h"
#include <stddef.h>
#include <vector>

// Workaround for bad 64bit compiler shifts (certain versions of GCC).
inline U8 SHL64(U8 src, U1 value)
{
	value &= 63;
	if(value == 0)
		return src;
	if(value < 32)
		return src << value;
	src <<= 16;
	src <<= 16;
	return src << (value - 32);
}

S1 intel_specialize_arg_size(S1 size, U1 dsz, U1 osz, U1 asz)
{
	if(size >= 0)
		return size;
	switch(size)
	{
	case IntelArgSizes::Tosz:
		return 2 << osz;
	case IntelArgSizes::Tasz:
		return 2 << asz;
	case IntelArgSizes::Tosz_ptr:
		return (dsz == 2) ? 8 : 4;
	case IntelArgSizes::Tosz_max32:
		return (osz == 2) ? 4 : (2 << osz);
	case IntelArgSizes::Tosz_min32:
		return (osz == 0) ? 4 : (2 << osz);
	case IntelArgSizes::Tosz_min64:
		return (osz == 2) ? 16 : 8;
	case IntelArgSizes::Tosz_seg:
		return (osz == 2) ? 8 : 2;
	case IntelArgSizes::Tosz_64in64:
		if(dsz == 2)
			return 8;
		return 2 << osz;
	default:
		break;
	}
	return 0;	// unsupported size !
}

IntelMCDecoder::IntelMCDecoder(ix_decoder_state *stateT, IntelDecoder *decoderT)
{
	error = "no error";
	state = stateT;
	decoder = decoderT;
}

// internal
struct decode_prefix_state
{
	U2 lockrep : 2;		// none, f0, f2, f3
	U2 op66 : 1;
	U2 op67 : 1;
	U2 rex_w : 1;
	U2 rex_r : 1;
	U2 rex_x : 1;
	U2 rex_b : 1;

	U2 sreg : 3;		// 0..5 or 7 if none

	U2 rex_used : 1;
	U2 wait_used : 1;
};

struct IntelInternalDecoder
{
	size_t bytes_left;
	size_t next_offset;	// this is the length of the instruction
	const U1 *buf;
	ix_decoder_state *state;

	static void reset_prefix(decode_prefix_state &p)
	{
		p.lockrep = 0;
		p.op66 = 0;
		p.op67 = 0;
		p.rex_w = 0;
		p.rex_r = 0;
		p.rex_x = 0;
		p.rex_b = 0;
		p.sreg = 7;
		p.rex_used = 0;
		p.wait_used = 0;
	}

	IntelDecoder *decoder;

	IntelInternalDecoder(IntelDecoder *decoderT)
	{
		decoder = decoderT;
	}

	// dsz : 0,1=16bit,32bit mode respectively (cs.d_b; not in 64bit mode)
	//       2=64bit mode
	// returns NULL on success, else an error message is returned.
	const char *decode(const U1 *bufT, ix_icode_t *dest, UINT num_bytes, U2 dsz, ix_decoder_state *stateT)
	{
		// Max 30 bytes -- redundant prefixes (ignored if opcode is FWAIT); FWAIT; redundant prefixes; instruction.
		if(num_bytes > 30)
			num_bytes = 30;

		bytes_left = num_bytes;
		next_offset = 0;
		buf = bufT;
		state = stateT;

		decode_prefix_state p;

		if(bytes_left == 0)
		{
			return "tried to decode a 0-byte buffer";
		}

		U1 c;
		U1 got_fwait = 0;

		for(;;)
		{
			// If we use FWAIT, relevant prefixes go after the FWAIT, i.e. any
			// prefixes before the FWAIT are ignored.
			reset_prefix(p);

			// Fetch standard x86 prefixes.
			for(;;)
			{
				if(!fetch_byte(&c))
				{
					if(got_fwait)
					{
						++bytes_left;
						--next_offset;
						reset_prefix(p);	// for consistency, drop all prefixes from fwait
						// note that p.wait_used is undefined if the insn itself is fwait!
						break;
					}
					return "limit exceeded";
				}

				// 001ss110
				if((c & 0xe7) == 0x26)
					p.sreg = ((c >> 3) & 3);
				else
				// 011001xx
				if((c & 0xfc) == 0x64)
				{
					if(c < 0x66)
						p.sreg = (c & 7);
					else
					if(c == 0x66)
						p.op66 = 1;
					else
					// 0x67
						p.op67 = 1;
				}
				else
				// 111100xx
				if((c & 0xfc) == 0xf0)
				{
					if(c == 0xf0)
						p.lockrep = 1;
					else
					if(c == 0xf1)
						break;
					else
					{
						// 0xf2 or 0xf3
						// 0xf0 overrides 0xf2, 0xf3.
						if(p.lockrep == 0)
							p.lockrep = (c & 3);
					}
				}
				else
					break;
			
				accept_byte();
			}

			// now if in 64-bit mode, get any REX prefix.
			if(dsz == 2 && fetch_byte(&c))
			{
				if((c & 0xf0) == 0x40)
				{
					p.rex_used = 1;
		
					p.rex_w = (c >> 3) & 1;
					p.rex_r = (c >> 2) & 1;
					p.rex_x = (c >> 1) & 1;
					p.rex_b = (c >> 0) & 1;
		
					// byte was accepted.
					accept_byte();
				}
			}

			if(fetch_byte(&c) && !got_fwait)
			{
				if(c == 0x9b)
				{
					got_fwait = 1;
					accept_byte();
					continue;
				}
			}

			break;
		}

		p.wait_used = got_fwait;

		const char *msg = try_decode(dest, dsz, p);

		if(msg != NULL)
		{
			if(p.wait_used)
			{
				// match wait, itself. discard any prefixes.
				reset_prefix(p);
				++bytes_left;
				--next_offset;
				msg = try_decode(dest, dsz, p);
			}

			return msg;
		}

		if(p.wait_used)
		{
			// check to see if encoding in question is fwaitable.
			// if not, just match fwait (discarding any prefixes) as above.
			if(!decoder->encoding_is_fwaitable(state->encoding_index))
			{
				// match fwait, itself.
				reset_prefix(p);
				++bytes_left;
				--next_offset;
				return try_decode(dest, dsz, p);
			}
		}
		else
		if(state->insn_size > 15)
			return "instruction exceeds 15 bytes";

		return NULL;	// success
	}

	bool fetch_byte(U1 *dest)
	{
		if(bytes_left < 1)
			return false;
		*dest = buf[next_offset];
		return true;
	}

	void accept_byte()
	{
		--bytes_left;
		++next_offset;
	}

	/*
	decoder state consists of things like these:
	- was rex used?
	- encoding index
	- size of instruction
	- first byte of modr/m
	- first byte of opcode (so we can check for NOP)
	*/
	const char *try_decode(ix_icode_t *dest, U2 dsz, decode_prefix_state &p)
	{
		state->prefix_size = next_offset;
		state->rex_used = p.rex_used;
		state->wait_used = p.wait_used;
		state->has_modrm = 0;
		state->disp_offset = 255;
		state->imm1_offset = 255;
		state->imm2_offset = 255;
		state->modrm_offset = 255;

		U1 c;
		if(!fetch_byte(&c))
			return "limit exceeded (1)";
		state->opcode0 = c;
		state->modrm0 = 0;	// normalize value
		bool got_modrm = false;

		if(state->opcode0 == 0x0f)
		{
			if(bytes_left < 2)
				return "limit exceeded (2)";
			state->opcode0 = buf[next_offset + 1];
			state->opcode0 += 0x100;
		}

		U4 decoder_table_root = decoder->decoderTable.back();
		size_t offset = decoder_table_root & 0xffffff;
		U1 type = decoder_table_root >> 24;

		// Before we begin, make dest->asz valid.
		if(dsz == 0)
		{
			dest->asz = (p.op67) ? 1 : 0;
		}
		else
		if(dsz == 1)
		{
			dest->asz = (p.op67) ? 0 : 1;
		}
		else
		{
			// 64 bit mode.
			dest->asz = (p.op67) ? 1 : 2;
		}

		dest->lockrep = p.lockrep;
		dest->ea.sreg = p.sreg;
		dest->rip_relative = 0;
		dest->ea.index_scale = 0;
		dest->ea.disp8 = 0;

		std::vector<U4> &decoder_table = decoder->decoderTable;

		bool done = false;
		do
		{
			switch(type)
			{
			case 0:
			{
				state->encoding_index = static_cast<U4>(offset);
				if(state->encoding_index == 0xffffff)
					return "invalid opcode";
				done = true;
				break;
			}
			case 1:
			{
				if(!fetch_byte(&c))
					return "limit exceeded (3)";
				offset += static_cast<UINT>(c);
				type = decoder_table[offset] >> 24;
				offset = decoder_table[offset] & 0xffffff;
				accept_byte();
				break;
			}
			case 6:
			{
				if(dsz == 2)
					++offset;
				type = decoder_table[offset] >> 24;
				offset = decoder_table[offset] & 0xffffff;
				break;
			}
			case 2:
			{
				UINT op66 = (p.op66) ? 1 : 0;
				UINT lockrep = 0;
				if(p.lockrep == 2)
					lockrep = 1;
				else
				if(p.lockrep == 3)
					lockrep = 2;
				offset += op66 + 2 * lockrep;
				type = decoder_table[offset] >> 24;
				offset = decoder_table[offset] & 0xffffff;
				break;
			}
			case 3:
			case 4:
			case 5:
			case 7:
			{
				if(!got_modrm)
				{
					got_modrm = true;
					if(!get_modrm(dest, dsz, p))
						return "limit exceeded (4)";
				}
				if(type == 3)
					offset += static_cast<UINT>(state->modrm0 >> 3);
				else
				if(type == 4)
					offset += static_cast<UINT>((state->modrm0 >> 3) & 7);
				else
				if(type == 5)
					offset += static_cast<UINT>(state->modrm0 & 7);
				else
				{
					if(!fetch_byte(&c))
						return "limit exceeded (5)";
					accept_byte();
					offset += static_cast<UINT>(c);
				}
				type = decoder_table[offset] >> 24;
				offset = decoder_table[offset] & 0xffffff;
				break;
			}
			default:
				return "internal table decoder error";
			}
		}	while(!done);

		IntelSyntax *syntax = decoder->syntax;
		IntelSyntaxEncoding *encoding = syntax->getEncoding(state->encoding_index);
		dest->insn = static_cast<U4>(encoding->insn);
		IntelSyntaxInsn *insn = syntax->getInsn(encoding->insn);

		if(dsz == 2 && encoding->tags.find(decoder->symbol_no64) != encoding->tags.end())
			return "opcode is invalid in 64bit mode";

		if(!got_modrm && encoding->regop != 0xf)
		{
			got_modrm = true;
			if(!get_modrm(dest, dsz, p))
				return "limit exceeded (6)";
		}
		
		if(dsz == 0)
		{
			dest->osz = (p.op66) ? 1 : 0;
		}
		else
		if(dsz == 1)
		{
			dest->osz = (p.op66) ? 0 : 1;
		}
		else
		{
			dest->osz = (p.op66) ? 0 : 1;
			if(encoding->tags.find(decoder->symbol_no_rex_w) == encoding->tags.end())
			{
				bool is64 = encoding->tags.find(decoder->symbol_is64) != encoding->tags.end();
				if(p.rex_w != 0 || is64)
				{
					// Note: 0x66 is ignored if used with rex.w (AMD manual).
					// However, if it's is64 and 66 is used WITHOUT a rex.w,
					// make osz 16 bits.
					dest->osz = 2;
					if(p.op66 && is64 && p.rex_w == 0)
						dest->osz = 0;
				}
			}
		}

		// Process arguments.
		dest->has_imm = 0;		// redundant
		dest->imm64 = 0;
		dest->sx = 0;

		const char *msg = get_arguments(dest, dsz, p);
		if(msg != NULL)
			return msg;

		// Check for bad LOCK usage.
		if(dest->lockrep == 1)
		{
			if(insn->proto.tags.find(decoder->symbol_lockable) == insn->proto.tags.end() && insn->proto.tags.find(decoder->symbol_lock_always) == insn->proto.tags.end())
			{
				return "invalid opcode: lock used with nonlockable instruction [1]";
			}
			// ok, instruction is lockable (or is always locked).
			///U1 first_argtype_major = encoding->argtype[0] & 0xe0;
			if(!IntelArgTypes::is_mem(dest->argtype[0]))
			{
				return "invalid opcode: lock used with nonlockable instruction [2]";
			}
			// if here, lock was used and IS allowed.
			// note that the 1st argument of xchg must be a memory argument in
			// the script in order for lock to be usable with it.
		}

		state->insn_size = static_cast<U1>(next_offset);
		return NULL;
	}

	// pre: dest->asz must be set already.
	bool get_modrm(ix_icode_t *dest, U2 dsz, decode_prefix_state &p)
	{
		state->modrm_offset = next_offset;

		U1 c;
		if(!fetch_byte(&c))
			return false;
		accept_byte();
		state->modrm0 = c;
		state->has_modrm = 1;
		dest->disp = 0;		// no displacement yet

		if(c >= 0xc0)
			return true;	// register form

		int x, disp_size;
		if(dsz == 2)
			x = decode_modrm_64(dest, dsz, p, c, &disp_size);
		else
		if(dest->asz == 0)
			x = decode_modrm_16(dest, dsz, p, c, &disp_size);
		else
			x = decode_modrm_32(dest, dsz, p, c, &disp_size);

		if(x != 0)
			return false;

		// Handle displacement.
		if(disp_size > 0)
		{
			dest->has_disp = 1;
			state->disp_offset = static_cast<U1>(next_offset);
			dest->disp = 0;			// redundant
			for(int i = 0; i < disp_size; ++i)
			{
				if(!fetch_byte(&c))
					return false;
				accept_byte();
				//dest->disp = (dest->disp << 8) | static_cast<U4>(c);
				dest->disp |= SHL64((U8)(c), 8 * i);
			}
			if(disp_size == 1)
			{
				dest->ea.disp8 = 1;	// forgot this the 1st time (fixed 2/1/2011)
				if(dest->asz == 0)
					dest->disp = (U4)(U2)(S2)(S1)(dest->disp);
				else
					dest->disp = (U4)(S4)(S1)(dest->disp);
			}
		}

		if(p.sreg == 7 && dest->ea.base != 31 && (dest->ea.base & 6) == 4)
			dest->ea.sreg = 6;		// bp, ebp, or esp base registers use SS:

		return true;
	}

	// returns 0 on success, -1 on error.
	// *disp_size is size in BYTES on success.
	int decode_modrm_16(ix_icode_t *dest, U2 dsz, decode_prefix_state &p, U1 c, int *disp_size)
	{
		U1 mod = (c >> 6) & 3;
		U1 rm = c & 7;
		*disp_size = -1;
	
		if(rm < 6)
			dest->ea.index = 6 + (rm & 1);	// [si] [di]
	
		if(rm < 4)
			dest->ea.base = 3 + (rm & 2);	// ax cx dx [bx] sp [bp] si di
		else
		if(rm == 6)
		{
			if(mod == 0)
				*disp_size = 2;
			else
				dest->ea.base = 5;	// bp
		}
		else
		if(rm == 7)
		{
			dest->ea.base = 3;	// bx
		}
	
		if(mod > 0)
			*disp_size = (mod == 1) ? 1 : 2;
	
		return 0;
	}

	// returns 0 on success, -1 on error.
	// *disp_size is size in BYTES on success.
	int decode_modrm_32(ix_icode_t *dest, U2 dsz, decode_prefix_state &p, U1 c, int *disp_size, bool mode64 = false)
	{
		U1 mod = (c >> 6) & 3;
		U1 rm = c & 7;
		U1 ss, i, b;
		*disp_size = -1;

		if(rm == 4)
		{
			// SIB
			if(mod == 1)
				*disp_size = 1;
			else
			if(mod == 2)
				*disp_size = 4;
			
			if(!fetch_byte(&c))
				return 1;
			accept_byte();

			ss = (c >> 6) & 3;
			i = (c >> 3) & 7;
			b = c & 7;
		
			if(i != 4)
			{
				dest->ea.index = i;
				dest->ea.index_scale = ss;
			}
			else
			{
				// index was 4.
				if(mode64 && p.rex_x)
				{
					dest->ea.index = i;
					dest->ea.index_scale = ss;
				}
			}
		
			if(b != 5)
			{
				dest->ea.base = b;
			}
			else
			{
				if(mod == 0)
					*disp_size = 4;
				else
					dest->ea.base = 5;
			}
		}
		else
		if(rm == 5 && mod == 0)
			*disp_size = 4;
		else
		{
			dest->ea.base = rm;
		
			// bugfix--is this right yet?
			if(mod == 1)
				*disp_size = 1;
			else
			if(mod == 2)
				*disp_size = 4;
		}

		return 0;
	}

	// returns 0 on success, -1 on error.
	// *disp_size is size in BYTES on success.
	int decode_modrm_64(ix_icode_t *dest, U2 dsz, decode_prefix_state &p, U1 c, int *disp_size)
	{
		int x = decode_modrm_32(dest, dsz, p, c, disp_size, true);
		if(x != 0)
			return x;
	
		if((c & 0xc7) == 0x05)
		{
			// According to the AMD manual, this is all we have to do here.
			dest->rip_relative = 1;
		}
		else
		{
			if(dest->ea.base != 31)
				dest->ea.base += (p.rex_b << 3);
			if(dest->ea.index != 31)
				dest->ea.index += (p.rex_x << 3);
		}
	
		return 0;
	}

	const char *get_arguments(ix_icode_t *dest, U2 dsz, decode_prefix_state &p)
	{
		IntelSyntax *syntax = decoder->syntax;
		IntelSyntaxEncoding *encoding = syntax->getEncoding(state->encoding_index);
		IntelSyntaxInsn *insn = syntax->getInsn(encoding->insn);

		for(int i = 0; i < 4; ++i)
		{
			dest->argtype[i] = encoding->argtype[i];
			dest->argsize[i] = encoding->argsize[i];
			dest->argvalue[i] = encoding->argvalue[i];
		}

		for(int i = 0; i < 4; ++i)
		{
			U1 type = dest->argtype[i];
			if(type == IntelArgTypes::Tvoid)
				break;
			U1 rm_reg_shift = 3;
			U1 rm_rex_reg = p.rex_r;

			dest->argsize[i] = intel_specialize_arg_size(dest->argsize[i], dsz, dest->osz, dest->asz);
			if(dest->argsize[i] <= 0)
				return "Unsupported argument encountered";

			if(type >= 0x80)
			{
				// Argument is reg/mem, where reg is any kind of register.
				type &= 0x7f;
				rm_reg_shift = 0;
				rm_rex_reg = p.rex_b;
				if(state->modrm0 >= 0xc0)
					dest->argtype[i] = type;	// discard "/mem", keep existing register type
				else
					type = dest->argtype[i] = IntelArgTypes::Tmem_ea;
			}

			if(type == IntelArgTypes::Tmem_fulldisp)
			{
				dest->has_disp = 1;
				state->disp_offset = static_cast<U1>(next_offset);
				int disp_size = 2 << dest->asz;
				U1 c;
				U8 disp = 0;
				for(int i = 0; i < disp_size; ++i)
				{
					if(!fetch_byte(&c))
						return "limit exceeded (7)";
					accept_byte();
					///disp = (disp << 8) | static_cast<U8>(c);
					disp |= SHL64((U8)(c), 8 * i);
				}
				dest->disp = static_cast<U4>(disp);
				if(disp_size == 8)
					dest->imm = static_cast<U4>((disp >> 16) >> 16);
			}
			else
			if(IntelArgTypes::is_imm(type))
			{
				bool imm2nd = (type == IntelArgTypes::Timm_2nd);
				U4 *imm_out = (imm2nd) ? &dest->disp : &dest->imm;

				if(type == IntelArgTypes::Timm_implict)
				{
					*imm_out = dest->argvalue[i];	// zero extend the implict value given
				}
				else
				{
					// imm1_offset, imm2_offset apply only to non-implict immediates.
					if(imm2nd)
						state->imm2_offset = static_cast<U1>(next_offset);
					else
						state->imm1_offset = static_cast<U1>(next_offset);

					// Need to fetch this immediate.
					if(encoding->tags.find(decoder->symbol_sx_byte) != encoding->tags.end())
					{
						if(dest->argsize[i] == 1)
							return "internal error";	// sx_byte should not be used with byte-sized immediates
						dest->sx = 1;

						U1 immvalue;
						if(!fetch_byte(&immvalue))
							return "limit exceeded (8)";
						accept_byte();
						
						if(dest->osz == 0)
							*imm_out = (U4)(U2)(S2)(S1)(immvalue);
						else
						{
							// Note: if imm arg's size is 64 bits, then one has to sign extend
							// s.icode->imm to 64 bits if 'ixflag_imm64_sx32' is used. Otherwise,
							// 'ixflag_imm64_disp' should be used, meaning the immediate was
							// actually encoded as a 64bit value--in which case s.icode->disp
							// holds the upper 32 bits of the immediate. This never happens when
							// 'ixflag_sx_byte' is used, however.
							*imm_out = (U4)(S4)(S1)(immvalue);
						}
					}	// sx_byte
					else
					if(dest->argsize[i] == 8 && (encoding->tags.find(decoder->symbol_imm64_disp) != encoding->tags.end()))
					{
						// Fetch 64 bits.
						U8 value = 0;
						U1 c;
						for(int k = 0; k < 8; ++k)
						{
							if(!fetch_byte(&c))
								return "limit exceeded (9)";
							accept_byte();
							//value = (value << 8) | static_cast<U8>(c);
							value |= SHL64((U8)(c), 8 * k);
						}
						dest->imm = static_cast<U4>(value);
						dest->disp = static_cast<U4>((value >> 16) >> 16);
						dest->imm64 = 1;
					}
					else
					{
						S2 size = dest->argsize[i];
						if(size == 8)
						{
							if(encoding->tags.find(decoder->symbol_imm64_sx32) == encoding->tags.end())
								return "imm64_sx32 missing from script file";
							size = 4;
						}
						// Fetch 'size' bytes.
						U4 value = 0;
						U1 c;
						for(int k = 0; k < size; ++k)
						{
							if(!fetch_byte(&c))
								return "limit exceeded (10)";
							accept_byte();
							//value = (value << 8) | static_cast<U4>(c);
							value |= SHL64((U8)(c), 8 * k);
						}
						*imm_out = value;
					}
				}
			}
			else
			if(IntelArgTypes::is_reg(type))
			{
				if(dest->argvalue[i] == 0xff)
				{
					if(type == IntelArgTypes::Treg_gr)
					{
						U1 value = dest->argvalue[i];
						if(encoding->tags.find(decoder->symbol_reg_base) != encoding->tags.end())
						{
							value = state->opcode0 & 7;
							value += (U1)(p.rex_b) << 3;
						}
						else
						if(encoding->tags.find(decoder->symbol_reg_rm) != encoding->tags.end())
						{
							// Is this right ?
							value = (state->modrm0 & 7) + ((U1)(p.rex_b) << 3);
						}
						else
						{
							value = (state->modrm0 >> rm_reg_shift) & 7;
							value += rm_rex_reg << 3;
						}
						dest->argvalue[i] = value;
					}
					else
					if(type == IntelArgTypes::Treg_sr)
					{
						dest->argvalue[i] = (state->modrm0 >> 3) & 7;
						if(dest->argvalue[i] > 5)
							return "invalid opcode: seg reg of 6 or 7 specified";
					}
					else
					{
						U1 valbase = ((state->modrm0 >> rm_reg_shift) & 7);
						if(encoding->tags.find(decoder->symbol_reg_base) != encoding->tags.end())
							valbase = state->opcode0 & 7;
						else
						if(encoding->tags.find(decoder->symbol_reg_rm) != encoding->tags.end())
							valbase = (state->modrm0 & 7) + ((U1)(p.rex_b) << 3);
						else
							valbase += (rm_rex_reg << 3);
						dest->argvalue[i] = valbase;
					}
				}
			}
		}

		return NULL;
	}
};

bool IntelMCDecoder::decode(ix_icode_t *dest, U4 num_bytes, U1 dsz, U1 *buf)
{
	error = "internal error";
	dest->insn = 0xffffffff;
	dest->has_disp = 0;
	dest->has_imm = 0;
	dest->lockrep = 0;
	dest->osz = dsz;
	dest->asz = dsz;
	dest->sx = 0;
	dest->rip_relative = 0;
	dest->imm64 = 0;
	dest->argtype[0] = IntelArgTypes::Tvoid;
	dest->argtype[1] = IntelArgTypes::Tvoid;
	dest->argtype[2] = IntelArgTypes::Tvoid;
	dest->argtype[3] = IntelArgTypes::Tvoid;
	dest->ea.base = 31;			// no base reg in ea
	dest->ea.index = 31;		// no index reg in ea
	dest->ea.sreg = 7;			// default ds segment

	IntelInternalDecoder dec(decoder);
	const char *msg = dec.decode(buf, dest, num_bytes, dsz, state);
	if(msg != NULL)
	{
		error = msg;
		return false;
	}

	error = "no error";
	return true;	// success
}

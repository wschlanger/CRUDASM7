// ixdecoder.c
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "ixdecoder.h"

//===============================================================================================//

// internal
struct decode_prefix_state_t
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

struct code_fetcher_t
{
	U1 *buf;
	U1 bytes_left;
	size_t next_offset;	
};

static int fetch_byte(struct code_fetcher_t *fetcher, U1 *c)
{
	if(fetcher->bytes_left < 1)
		return 0;
	*c = fetcher->buf[fetcher->next_offset];
	return 1;
}

static int accept_byte(struct code_fetcher_t *fetcher)
{
	--fetcher->bytes_left;
	++fetcher->next_offset;
}

static void reset_prefix(struct decode_prefix_state_t *p)
{
	p->lockrep = 0;
	p->op66 = 0;
	p->op67 = 0;
	p->rex_w = 0;
	p->rex_r = 0;
	p->rex_x = 0;
	p->rex_b = 0;
	p->sreg = 7;
	p->rex_used = 0;
	p->wait_used = 0;
}

// Workaround for bad 64bit compiler shifts (certain versions of GCC).
static U8 SHL64(U8 src, U1 value)
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

static int decode_modrm_16(struct code_fetcher_t *fetcher, struct ix_icode_t *dest, U2 dsz, struct decode_prefix_state_t *p, U1 c, int *disp_size, struct ix_decoder_state_t *state);
static int decode_modrm_32(struct code_fetcher_t *fetcher, struct ix_icode_t *dest, U2 dsz, struct decode_prefix_state_t *p, U1 c, int *disp_size, int mode64, struct ix_decoder_state_t *state);
static int decode_modrm_64(struct code_fetcher_t *fetcher, struct ix_icode_t *dest, U2 dsz, struct decode_prefix_state_t *p, U1 c, int *disp_size, struct ix_decoder_state_t *state);

// pre: dest->asz must be set already.
static get_modrm(struct code_fetcher_t *fetcher, struct ix_icode_t *dest, U2 dsz, struct decode_prefix_state_t *p, struct ix_decoder_state_t *state)
{
	U1 c;
	int x, disp_size;
	int i;

	state->modrm_offset = fetcher->next_offset;

	if(!fetch_byte(fetcher, &c))
		return 0;
	accept_byte(fetcher);
	state->modrm0 = c;
	state->has_modrm = 1;
	dest->disp = 0;		// no displacement yet

	if(c >= 0xc0)
		return 1;	// register form

	if(dsz == 2)
		x = decode_modrm_64(fetcher, dest, dsz, p, c, &disp_size, state);
	else
	if(dest->asz == 0)
		x = decode_modrm_16(fetcher, dest, dsz, p, c, &disp_size, state);
	else
		x = decode_modrm_32(fetcher, dest, dsz, p, c, &disp_size, 0, state);

	if(x != 0)
		return 0;

	// Handle displacement.
	if(disp_size > 0)
	{
		dest->has_disp = 1;
		state->disp_offset = (U1)(fetcher->next_offset);
		dest->disp = 0;			// redundant
		for(i = 0; i < disp_size; ++i)
		{
			if(!fetch_byte(fetcher, &c))
				return 0;
			accept_byte(fetcher);
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

	if(p->sreg == 7 && dest->ea.base != 31 && (dest->ea.base & 6) == 4)
		dest->ea.sreg = 6;		// bp, ebp, or esp base registers use SS:

	return 1;
}

// returns 0 on success, -1 on error.
// *disp_size is size in BYTES on success.
static int decode_modrm_16(struct code_fetcher_t *fetcher, struct ix_icode_t *dest, U2 dsz, struct decode_prefix_state_t *p, U1 c, int *disp_size, struct ix_decoder_state_t *state)
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
static int decode_modrm_32(struct code_fetcher_t *fetcher, struct ix_icode_t *dest, U2 dsz, struct decode_prefix_state_t *p, U1 c, int *disp_size, int mode64, struct ix_decoder_state_t *state)
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
		
		if(!fetch_byte(fetcher, &c))
			return 1;
		accept_byte(fetcher);

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
			if(mode64 && p->rex_x)
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
static int decode_modrm_64(struct code_fetcher_t *fetcher, struct ix_icode_t *dest, U2 dsz, struct decode_prefix_state_t *p, U1 c, int *disp_size, struct ix_decoder_state_t *state)
{
	int x = decode_modrm_32(fetcher, dest, dsz, p, c, disp_size, 1, state);
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
			dest->ea.base += (p->rex_b << 3);
		if(dest->ea.index != 31)
			dest->ea.index += (p->rex_x << 3);
	}

	return 0;
}

S1 intel_specialize_arg_size(S1 size, U1 dsz, U1 osz, U1 asz)
{
	if(size >= 0)
		return size;
	switch(size)
	{
	case crudasm_intel_argsize_osz:
		return 2 << osz;
	case crudasm_intel_argsize_asz:
		return 2 << asz;
	case crudasm_intel_argsize_osz_ptr:
		return (dsz == 2) ? 8 : 4;
	case crudasm_intel_argsize_osz_max32:
		return (osz == 2) ? 4 : (2 << osz);
	case crudasm_intel_argsize_osz_min32:
		return (osz == 0) ? 4 : (2 << osz);
	case crudasm_intel_argsize_osz_min64:
		return (osz == 2) ? 16 : 8;
	case crudasm_intel_argsize_osz_seg:
		return (osz == 2) ? 8 : 2;
	case crudasm_intel_argsize_osz_64in64:
		if(dsz == 2)
			return 8;
		return 2 << osz;
	default:
		break;
	}
	return 0;	// unsupported size !
}

// Returns NULL on success, else an error message.
const char *get_arguments(struct code_fetcher_t *fetcher, struct ix_icode_t *dest, U1 dsz, struct decode_prefix_state_t *p, struct ix_decoder_state_t *state, struct crudasm_intel_encoding_t *encoding, struct crudasm_intel_insn_t *insn)
{
	int i, j, disp_size, disp_size_2, k;
	U1 type, rm_reg_shift, rm_rex_reg, c, immvalue, valbase;
	U8 disp, value;
	int imm2nd;
	U4 *imm_out;
	S2 size;
	
	for(i = 0; i < 4; ++i)
	{
		dest->argtype[i] = encoding->argtype[i];
		dest->argsize[i] = encoding->argsize[i];
		dest->argvalue[i] = encoding->argvalue[i];
	}

	for(i = 0; i < 4; ++i)
	{
		type = dest->argtype[i];
		if(type == crudasm_intel_argtype_void)
			break;
		rm_reg_shift = 3;
		rm_rex_reg = p->rex_r;

		dest->argsize[i] = intel_specialize_arg_size(dest->argsize[i], dsz, dest->osz, dest->asz);
		if(dest->argsize[i] <= 0)
			return "Unsupported argument encountered";

		if(type >= 0x80)
		{
			// Argument is reg/mem, where reg is any kind of register.
			type &= 0x7f;
			rm_reg_shift = 0;
			rm_rex_reg = p->rex_b;
			if(state->modrm0 >= 0xc0)
				dest->argtype[i] = type;	// discard "/mem", keep existing register type
			else
				type = dest->argtype[i] = crudasm_intel_argtype_mem_ea;
		}

		if(type == crudasm_intel_argtype_mem_fulldisp)
		{
			dest->has_disp = 1;
			state->disp_offset = (U1)(fetcher->next_offset);
			disp_size_2 = 2 << dest->asz;
			disp = 0;
			for(j = 0; j < disp_size_2; ++j)
			{
				if(!fetch_byte(fetcher, &c))
					return "limit exceeded (7)";
				accept_byte(fetcher);
				///disp = (disp << 8) | static_cast<U8>(c);
				disp |= SHL64((U8)(c), 8 * j);
			}
			dest->disp = (U4)(disp);
			if(disp_size_2 == 8)
				dest->imm = (U4)((disp >> 16) >> 16);
		}
		else
		if(crudasm_intel_argtype__is_imm(type))
		{
			imm2nd = (type == crudasm_intel_argtype_imm_2nd);
			imm_out = (imm2nd) ? &dest->disp : &dest->imm;

			if(type == crudasm_intel_argtype_imm_implict)
			{
				*imm_out = dest->argvalue[i];	// zero extend the implict value given
			}
			else
			{
				// imm1_offset, imm2_offset apply only to non-implict immediates.
				if(imm2nd)
					state->imm2_offset = (U1)(fetcher->next_offset);
				else
					state->imm1_offset = (U1)(fetcher->next_offset);

				// Need to fetch this immediate.
				if(encoding->flag_sx_byte)
				{
					if(dest->argsize[i] == 1)
						return "internal error";	// sx_byte should not be used with byte-sized immediates
					dest->sx = 1;

					if(!fetch_byte(fetcher, &immvalue))
						return "limit exceeded (8)";
					accept_byte(fetcher);
					
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
				if(dest->argsize[i] == 8 && encoding->flag_imm64_disp)
				{
					// Fetch 64 bits.
					value = 0;
					for(k = 0; k < 8; ++k)
					{
						if(!fetch_byte(fetcher, &c))
							return "limit exceeded (9)";
						accept_byte(fetcher);
						//value = (value << 8) | static_cast<U8>(c);
						value |= SHL64((U8)(c), 8 * k);
					}
					dest->imm = (U4)(value);
					dest->disp = (U4)((value >> 16) >> 16);
					dest->imm64 = 1;
				}
				else
				{
					size = dest->argsize[i];
					if(size == 8)
					{
						if(encoding->flag_imm64_sx32 == 0)
							return "imm64_sx32 missing from script file";
						size = 4;
					}
					// Fetch 'size' bytes.
					value = 0;
					for(k = 0; k < size; ++k)
					{
						if(!fetch_byte(fetcher, &c))
							return "limit exceeded (10)";
						accept_byte(fetcher);
						//value = (value << 8) | static_cast<U4>(c);
						value |= SHL64((U8)(c), 8 * k);
					}
					*imm_out = value;
				}
			}
		}
		else
		if(crudasm_intel_argtype__is_reg(type))
		{
			if(dest->argvalue[i] == 0xff)
			{
				if(type == crudasm_intel_argtype_reg_gr)
				{
					U1 value = dest->argvalue[i];
					if(encoding->flag_reg_base != 0)
					{
						value = state->opcode0 & 7;
						value += (U1)(p->rex_b) << 3;
					}
					else
					if(encoding->flag_reg_rm != 0)
					{
						// Is this right ?
						value = (state->modrm0 & 7) + ((U1)(p->rex_b) << 3);
					}
					else
					{
						value = (state->modrm0 >> rm_reg_shift) & 7;
						value += rm_rex_reg << 3;
					}
					dest->argvalue[i] = value;
				}
				else
				if(type == crudasm_intel_argtype_reg_sr)
				{
					dest->argvalue[i] = (state->modrm0 >> 3) & 7;
					if(dest->argvalue[i] > 5)
						return "invalid opcode: seg reg of 6 or 7 specified";
				}
				else
				{
					valbase = ((state->modrm0 >> rm_reg_shift) & 7);
					if(encoding->flag_reg_base != 0)
						valbase = state->opcode0 & 7;
					else
					if(encoding->flag_reg_rm != 0)
						valbase = (state->modrm0 & 7) + ((U1)(p->rex_b) << 3);
					else
						valbase += (rm_rex_reg << 3);
					dest->argvalue[i] = valbase;
				}
			}
		}
	}

	return NULL;
}

const char *try_decode(struct code_fetcher_t *fetcher, struct ix_icode_t *dest, U1 dsz, struct decode_prefix_state_t *p, struct ix_decoder_state_t *state)
{
	U1 c;
	int got_modrm = 0;
	U4 decoder_table_root;
	size_t offset;
	U1 type;
	U4 *decoder_table;
	int done;
	size_t op66, lockrep;
	struct crudasm_intel_encoding_t *encoding;
	struct crudasm_intel_insn_t *insn;
	int is64;
	const char *msg;

	state->prefix_size = fetcher->next_offset;
	state->rex_used = p->rex_used;
	state->wait_used = p->wait_used;
	state->has_modrm = 0;
	state->disp_offset = 255;
	state->imm1_offset = 255;
	state->imm2_offset = 255;
	state->modrm_offset = 255;

	if(!fetch_byte(fetcher, &c))
		return "limit exceeded (1)";
	state->opcode0 = c;
	state->modrm0 = 0;	// normalize value

	if(state->opcode0 == 0x0f)
	{
		if(fetcher->bytes_left < 2)
			return "limit exceeded (2)";
		state->opcode0 = fetcher->buf[fetcher->next_offset + 1];
		state->opcode0 += 0x100;
	}

	decoder_table_root = crudasm_intel_decoder_table[0];
	offset = decoder_table_root & 0xffffff;
	type = decoder_table_root >> 24;

	// Before we begin, make dest->asz valid.
	if(dsz == 0)
	{
		dest->asz = (p->op67) ? 1 : 0;
	}
	else
	if(dsz == 1)
	{
		dest->asz = (p->op67) ? 0 : 1;
	}
	else
	{
		// 64 bit mode.
		dest->asz = (p->op67) ? 1 : 2;
	}

	dest->lockrep = p->lockrep;
	dest->ea.sreg = p->sreg;
	dest->rip_relative = 0;
	dest->ea.index_scale = 0;
	dest->ea.disp8 = 0;
	dest->is_nop = (state->opcode0 == 0x90) ? 1 : 0;
	dest->rex_used = state->rex_used;

	decoder_table = crudasm_intel_decoder_table + 1;

	done = 0;
	do
	{
		switch(type)
		{
		case 0:
		{
			state->encoding_index = (U4)(offset);
			if(state->encoding_index == 0xffffff)
				return "invalid opcode";
			done = 1;
			break;
		}
		case 1:
		{
			if(!fetch_byte(fetcher, &c))
				return "limit exceeded (3)";
			offset += (size_t)(c);
			type = decoder_table[offset] >> 24;
			offset = decoder_table[offset] & 0xffffff;
			accept_byte(fetcher);
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
			op66 = (p->op66) ? 1 : 0;
			lockrep = 0;
			if(p->lockrep == 2)
				lockrep = 1;
			else
			if(p->lockrep == 3)
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
				got_modrm = 1;
				if(!get_modrm(fetcher, dest, dsz, p, state))
					return "limit exceeded (4)";
			}
			if(type == 3)
				offset += (size_t)(state->modrm0 >> 3);
			else
			if(type == 4)
				offset += (size_t)((state->modrm0 >> 3) & 7);
			else
			if(type == 5)
				offset += (size_t)(state->modrm0 & 7);
			else
			{
				if(!fetch_byte(fetcher, &c))
					return "limit exceeded (5)";
				accept_byte(fetcher);
				offset += (size_t)(c);
			}
			type = decoder_table[offset] >> 24;
			offset = decoder_table[offset] & 0xffffff;
			break;
		}
		default:
			return "internal table decoder error";
		}
	}	while(!done);
	
	encoding = crudasm_intel_encoding_table + state->encoding_index;
	insn = crudasm_intel_insns + encoding->insn;
	///dest->insn = (U4)(encoding->insn);
	dest->encoding = state->encoding_index;
	
	///printf("Instruction = [%s]\n", insn->name);

	if(dsz == 2 && encoding->flag_no64)
		return "opcode is invalid in 64bit mode";

	if(!got_modrm && encoding->regop != 0xf)
	{
		got_modrm = 1;
		if(!get_modrm(fetcher, dest, dsz, p, state))
			return "limit exceeded (6)";
	}
	
	if(dsz == 0)
	{
		dest->osz = (p->op66) ? 1 : 0;
	}
	else
	if(dsz == 1)
	{
		dest->osz = (p->op66) ? 0 : 1;
	}
	else
	{
		dest->osz = (p->op66) ? 0 : 1;
		if(!encoding->flag_no_rex_w)
		{
			is64 = encoding->flag_is64 != 0;
			if(p->rex_w != 0 || is64)
			{
				// Note: 0x66 is ignored if used with rex.w (AMD manual).
				// However, if it's is64 and 66 is used WITHOUT a rex.w,
				// make osz 16 bits.
				dest->osz = 2;
				if(p->op66 && is64 && p->rex_w == 0)
					dest->osz = 0;
			}
		}
	}

	// Process arguments.
	dest->has_imm = 0;		// redundant
	dest->imm64 = 0;
	dest->sx = 0;

	msg = get_arguments(fetcher, dest, dsz, p, state, encoding, insn);
	if(msg != NULL)
		return msg;

	// Check for bad LOCK usage.
	if(dest->lockrep == 1)
	{
		if(!insn->flag_lockable && !insn->flag_lock_always)
		{
			return "invalid opcode: lock used with nonlockable instruction [1]";
		}
		// ok, instruction is lockable (or is always locked).
		///U1 first_argtype_major = encoding->argtype[0] & 0xe0;
		if(!crudasm_intel_argtype__is_mem(dest->argtype[0]))
		{
			return "invalid opcode: lock used with nonlockable instruction [2]";
		}
		// if here, lock was used and IS allowed.
		// note that the 1st argument of xchg must be a memory argument in
		// the script in order for lock to be usable with it.
	}

	state->insn_size = (U1)(fetcher->next_offset);
	return "";	// success
}

const char *do_decode(struct ix_icode_t *dest, U1 num_bytes, U1 dsz, U1 *buf, struct ix_decoder_state_t *state_out)
{
	const char *status = "internal error";
	struct code_fetcher_t fetcher;
	struct code_fetcher_t fetcher_copy;
	struct decode_prefix_state_t p;
	U1 c;
	U1 got_fwait = 0;
	
	if(num_bytes == 0)
		return "tried to decode a 0-byte buffer";
	
	fetcher.buf = buf;
	fetcher.bytes_left = num_bytes;
	fetcher.next_offset = 0;
	
	for(;;)
	{
		// If we use FWAIT, relevant prefixes go after the FWAIT, i.e. any
		// prefixes before the FWAIT are ignored.
		reset_prefix(&p);

		// Fetch standard x86 prefixes.
		for(;;)
		{
			if(!fetch_byte(&fetcher, &c))
			{
				if(got_fwait)
				{
					++fetcher.bytes_left;
					--fetcher.next_offset;
					reset_prefix(&p);	// for consistency, drop all prefixes from fwait
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
		
			accept_byte(&fetcher);
		}

		// now if in 64-bit mode, get any REX prefix.
		if(dsz == 2 && fetch_byte(&fetcher, &c))
		{
			if((c & 0xf0) == 0x40)
			{
				p.rex_used = 1;
	
				p.rex_w = (c >> 3) & 1;
				p.rex_r = (c >> 2) & 1;
				p.rex_x = (c >> 1) & 1;
				p.rex_b = (c >> 0) & 1;
	
				// byte was accepted.
				accept_byte(&fetcher);
			}
		}

		if(fetch_byte(&fetcher, &c) && !got_fwait)
		{
			if(c == 0x9b)
			{
				got_fwait = 1;
				accept_byte(&fetcher);
				continue;
			}
		}

		break;
	}

	p.wait_used = got_fwait;
	
	// - Try to decode.
	// - If there's an error:
	//   - If wait was used:
	//     - go back one byte, re-decode, return results.
	//       that is, match the "wait" instruction, itself.
	//   - If wait was not used:
	//     - Return the error.
	// - If there was no error:
	//   - If wait was used:
	//     - If insn is not wait-able
	//       - match wait itself
	//     - If insn was waitable
	//       - return success
	//   - If wait was not used:
	//     - return success, unless insn length > 15 bytes,
	//       in which case we want to return an error
	
	fetcher_copy = fetcher;
	status = try_decode(&fetcher_copy, dest, dsz, &p, state_out);
	dest->fwait = 0;  // not fwaitable
	
	if(*status != '\0')
	{
		if(p.wait_used)
		{
			// match wait, itself. discard any prefixes.
			reset_prefix(&p);
			fetcher_copy = fetcher;
			++fetcher_copy.bytes_left;
			--fetcher_copy.next_offset;
			status = try_decode(&fetcher_copy, dest, dsz, &p, state_out);
			dest->fwait = 0;
		}
		
		return status;
	}
	
	// Decode was successful.
	if(p.wait_used)
	{
		// check to see if encoding in question is fwaitable.
		// if not, just match fwait (discarding any prefixes) as above.
		if(!crudasm_intel_insns[crudasm_intel_encoding_table[dest->encoding].insn].flag_fwaitable)
		{
			// match wait, itself. discard any prefixes.
			reset_prefix(&p);
			fetcher_copy = fetcher;
			++fetcher_copy.bytes_left;
			--fetcher_copy.next_offset;
			status = try_decode(&fetcher_copy, dest, dsz, &p, state_out);
			dest->fwait = 0;
			return status;
		}
		
		// wait was used, and it is waitable.
		dest->fwait = 2;  // fwaitable (fwait WAS used)
	}
	else
	{
		if(state_out->insn_size > 15)
			return "instruction exceeds 15 bytes";
		if(crudasm_intel_insns[crudasm_intel_encoding_table[dest->encoding].insn].flag_fwaitable)
			dest->fwait = 1;  // fwaitable (fwait not used)
	}
	
	return status;
}

const char *crudasm_intel_decode(struct ix_icode_t *dest, U8 num_bytes, U1 dsz, U1 *buf, struct ix_decoder_state_t *state_out)
{
	struct ix_decoder_state_t state_tmp;

	if(state_out == NULL)
		state_out = &state_tmp;
	
	dest->encoding = 0xffffffff;
	dest->has_disp = 0;
	dest->has_imm = 0;
	dest->lockrep = 0;
	dest->osz = dsz;
	dest->asz = dsz;
	dest->sx = 0;
	dest->rip_relative = 0;
	dest->imm64 = 0;
	dest->argtype[0] = crudasm_intel_argtype_void;
	dest->argtype[1] = crudasm_intel_argtype_void;
	dest->argtype[2] = crudasm_intel_argtype_void;
	dest->argtype[3] = crudasm_intel_argtype_void;
	dest->ea.base = 31;			// no base reg in ea
	dest->ea.index = 31;		// no index reg in ea
	dest->ea.sreg = 7;			// default ds segment
	
	// Max 30 bytes to decode: ignored prefixes, FWAIT, prefixes, insn.
	return do_decode(dest, (num_bytes > 30) ? 30 : num_bytes, dsz, buf, state_out);
}

//===============================================================================================//

#if 0
int main(int argc, const char **argv)
{
	char buf1[32] = {0x40, 0xb8, 0x01, 0x17};
	char buf2[32] = {0xf2, 0x9b, 0xff, 0xff};  // rep wait ud
	struct ix_icode_t icode;
	const char *status;
	
	status = crudasm_intel_decode(&icode, 32, 0, buf1, NULL);
	printf("Status = [%s]\n", status);
	
	status = crudasm_intel_decode(&icode, 32, 0, buf2, NULL);
	printf("Status = [%s]\n", status);
	
	return 0;
}
#endif


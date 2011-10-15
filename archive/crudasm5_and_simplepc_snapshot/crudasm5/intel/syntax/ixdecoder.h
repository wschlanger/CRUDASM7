// ixdecoder.h
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#ifndef l_ixdecoder_h__ceres_included
#define l_ixdecoder_h__ceres_included

#include "../../common/include/types.h"
#include "ixbasic.h"
#include "ixsyntax.h"

// This is part of ix_icode_t.
struct ix_ea_t
{
	U2 base : 5;		// 31 if none
	U2 index : 5;		// 31 if none
	U2 index_scale : 2;	// 0,1,2,3
	U2 sreg : 3;		// 6,7=2,3
	U2 disp8 : 1;		// displacement was 8 bits
	
	U2 getSegReg() const
	{
		if(sreg >= 6)
			return sreg - 6 + 2;
		return sreg;
	}
};

// This is currently 32 bytes (max 4 arguments).
struct ix_icode_t
{
	U4 insn;		// 0xffffffff means invalid insn
	U4 imm;
	U4 disp;

	ix_ea_t ea;

	U2 has_disp : 1;
	U2 has_imm : 1;
	U2 lockrep : 2;		// 0=default,1=lock,2=f2,3=f3
	U2 osz : 2;		// 0=16bits,1=32bits,2=64bits
	U2 asz : 2;		// 0=16bits,1=32bits,2=64bits
	U2 sx : 1;		// any imm arguments with non-byte size were sx'd from a byte

	// The next two quantities are iffy from an assembler point of view but are
	// useful from a disassembler point of view--they're useful if we need to
	// discard decoder state and retain only icode information for instructions.
	U2 rip_relative : 1;	// EA is rip-relative (64bit dsz mode only)
	U2 imm64 : 1;	// set to 1 if 'disp' holds high 32 bits of imm

	U2 : 5;
	
	U1 argtype[4];
	S1 argsize[4];
	U1 argvalue[4];
};

// This is valid only upon successful decode.
struct ix_decoder_state
{
	U4 encoding_index;
	U1 rex_used : 1;
	U1 wait_used : 1;
	U1 insn_size : 5;
	U1 has_modrm : 1;
	U1 modrm0;
	U2 opcode0;

	// these are 255 by default.
	// offset in given machine code buffer of where any displacement is.
	// also provides offset in machine code of 1st & 2nd immediate's.
	U1 disp_offset;
	U1 imm1_offset;
	U1 imm2_offset;
	U1 modrm_offset;	// added
	U1 prefix_size;		// added--valid even if prefixes ignored or used with fwait
};

class IntelMCDecoder
{
	U1 *buf;
	const char *error;
	ix_decoder_state *state;
	IntelDecoder *decoder;
public:
	IntelMCDecoder(ix_decoder_state *stateT, IntelDecoder *decoderT);
	bool decode(ix_icode_t *dest, U4 num_bytes, U1 dsz, U1 *buf);
	const char *explain_error() const { return error; }		// "no error" if no error
	ix_decoder_state *getState() { return state; }
	IntelDecoder *getDecoder() { return decoder; }
};

// If size >= 0, this returns size. If size < 0, this returns the true size parameter,
// which is always > 0, based on the given default size, operand size, and address-size.
S1 intel_specialize_arg_size(S1 size, U1 dsz, U1 osz, U1 asz);

#endif	// l_ixdecoder_h__ceres_included

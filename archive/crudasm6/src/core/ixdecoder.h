// ixdecoder.h
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#ifndef l_ixdecoder_h__crudasm_included
#define l_ixdecoder_h__crudasm_included

#include "../common/include/types.h"
#include "decoder_internal.h"
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

// This is part of ix_icode_t.
struct ix_ea_t
{
	U2 base : 5;		// 31 if none
	U2 index : 5;		// 31 if none
	U2 index_scale : 2;	// 0,1,2,3
	U2 sreg : 3;		// 6,7=2,3
	U2 disp8 : 1;		// displacement was 8 bits
};

static U2 ix_ea_get_seg_reg(U2 sreg)
{
	if(sreg >= 6)
		return sreg - 6 + 2;
	return sreg;
}

// This is currently 32 bytes (max 4 arguments).
struct ix_icode_t
{
	///U4 insn;		// 0xffffffff means invalid insn
	U4 encoding;	// 0xffffffff means invalid insn
	U4 imm;
	U4 disp;

	struct ix_ea_t ea;

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

	U2 fwait : 2;	// 0=default,1=fwaitable (no fwait used),2=fwaitable (fwait was used)
	U2 is_nop : 1;
	U2 rex_used : 1;
	U2 : 1;
	
	U1 argtype[4];
	S1 argsize[4];
	U1 argvalue[4];
};

// This is valid only upon successful decode.
struct ix_decoder_state_t
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
	// also provides offset in machine code of 1st & 2nd immediate's, etc.
	U1 disp_offset;
	U1 imm1_offset;
	U1 imm2_offset;
	U1 modrm_offset;	// added
	// For the purpose of prefix_size, we treat FWAIT as a prefix.
	U1 prefix_size;		// added--valid even if prefixes ignored or used with fwait
};

// Returns "" on success. Else an error message is returned.
// state_out may be NULL if you don't care about it.
// dsz == 0 for 16 bit mode, 1 for 32 bit mode, 2 for 64 bit mode.
// num_bytes == number of bytes remaining in buf, should be at least 1 !
const char *crudasm_intel_decode(struct ix_icode_t *dest, U8 num_bytes, U1 dsz, U1 *buf, struct ix_decoder_state_t *state_out);

#ifdef __cplusplus
}
#endif

#endif	// l_ixdecoder_h__crudasm_included


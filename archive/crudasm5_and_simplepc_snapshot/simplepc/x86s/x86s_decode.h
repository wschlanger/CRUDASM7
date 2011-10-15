// x86s_decode.h
// Copyright (C) 2008 Willow Schlanger

#ifndef l_x86s__x86s_decode_h__included
#define l_x86s__x86s_decode_h__included

#include "types.h"
#include "x86s_common.h"

namespace x86s
{

#include "out_insn_enums.h"

struct decode_state_t
{
	icode_t *icode;		// [out--but pointer must be loaded by user] where to decode to
	const U1 *insn;		// [in] subtract from buffer base to find current insn offset
	const U1 *end;		// [in] (end - insn) = max_bytes_left. (max_bytes_left - size) = bytes_left.

	U4 dsz : 2;			// [in] default operand and address size - argsize_16, argsize_32, or argsize_64
	U4 encoding : 24;	// [out] 0xffffff means invalid opcode
	U4 avl1 : 6;
	
	U2 basecode;		// [out] only 10 bits used
	U1 size;			// [out]
	U1 modrm0;			// [out]
};

// Return codes.
// 0 : success
// 1 : s.icode, s.basecode, s.size, s.modrm0 undefined.
//     exceeded limit while decoding.
// 2 : invalid opcode - opcode invalid when dsz is 64
// 3 : invalid opcode - general
// 4 : invalid opcode - or internal error!
int decode(decode_state_t &s);

extern encoding_t encodings[];
extern int search_table[];
extern const char *insn_strings[insn__count];
extern insn_def_t insns[insn__count];
extern U4 decoder_table[];

}	// namespace x86s

#endif	// l_x86s__x86s_decode_h__included

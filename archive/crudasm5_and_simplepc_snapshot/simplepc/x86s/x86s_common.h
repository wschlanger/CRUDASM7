// x86s_common.h
// Copyright (C) 2008 Willow Schlanger

#ifndef l_x86s__x86s_common_h__included
#define l_x86s__x86s_common_h__included

#include "types.h"

// Note: if you change anything here, make sure you update these functions:
// - decode_get_args() in decode.cpp
// - lots of things in x86s_common.h

#define MAX_ARGS 4

namespace x86s
{

// --- begin instruction suffixes ---
// 4 bits
enum
{
	size_def,
	size_same,					// all arguments have the same size - if a register argument exists, don't need to print size of other arguments.
	size_none,					// none of the insn's encoding arguments need a size when disassembling
	size_none_dsz,				// same as size_none but print o16/o32/o64 if dsz != osz
	size__end
};

// 1 bit
enum
{
	ctrlxfer_def,
	ctrlxfer_yes,				// print o16/o32/o64 if not same as default operand size when disassembling.
	ctrlxfer__end
};

// note: the order of elements here must match the order they are introduced in x86s_script.h.
struct insn_suffix
{
	U1 size : 4;
	U1 ctrlxfer : 1;
	// 4 bits remaining
};

// --- end instruction suffixes ---

// If you change this structure, modify write_script.cpp accordingly.
struct insn_def_t
{
	U4 first_encoding : 24;		// index into ???
	U4 num_encodings : 8;		// number of encodings
	insn_suffix suffix;
};

// --- begin encoding suffixes ---

// 3 bits
enum
{
	mod_0,						// claim only mod=0
	mod_1,						// claim only mod=1
	mod_2,						// claim only mod=2
	mod_3,						// claim only mod=3 (e.g. register types)
	mod_def,					// default
	mod_mem,					// claim only mod=0, mod=1, mod=2
	mod__end
};

// Note: there is a modr/m if and only if the ro (reg/op) field is not ro_def.
enum
{
	ro_0,						// /0 - reg/op = 0
	ro_1,						// /1 - reg/op = 1
	ro_2,						// /2 - reg/op = 2
	ro_3,						// /3 - reg/op = 3
	ro_4,						// /4 - reg/op = 4
	ro_5,						// /5 - reg/op = 5
	ro_6,						// /6 - reg/op = 6
	ro_7,						// /7 - reg/op = 7
	ro_z,						// /z - claim all 8, encode as /0
	ro_r,						// /r - register, claim all 8
	ro_def,						// default - no modr/m
	ro__end
};

// 4 bits
enum
{
	rm_0,						// claim only r/m=0
	rm_1,						// claim only r/m=1
	rm_2,						// claim only r/m=2
	rm_3,						// claim only r/m=3
	rm_4,						// claim only r/m=4
	rm_5,						// claim only r/m=5
	rm_6,						// claim only r/m=6
	rm_7,						// claim only r/m=7
	rm_def,						// default
	rm__end
};

// 2 bits
enum
{
	op66_def,					// default
	op66_no66,					// encoding applies only when 0x66 is absent
	op66_op66,					// encoding mandates the use of 0x66
	op66__end
};

// 3 bits
enum
{
	fx_def,						// lockable or locked (f2/f3 ignored)
	fx_lockable,				// (f2/f3 ignored)
	fx_locked,					// (f2/f3 ignored)
	fx_rep,						// repeatable instruction.
	fx_none,					// has table. this encoding has no f2/f3
	fx_none_lockable,			// has table. fxnone, but is lockable.
	fx_f2,						// has table. this encoding has f2
	fx_f3,						// has table. this encoding has f3
	fx__end
};

// 3 bits
enum
{
	o_def,						// default
	o_is64,						// in 64-bit mode, operand size defaults to 64 bits.
								// a 0x66 opcode switches to 16-bit mode.
								// thus, is64 means in long mode, the argument has a size
								// of 64 or 16 bits only.
	o_no64,						// invalid in 64-bit mode
	o_sp64_movsxd,				// special case
	o_no16,						// operation undefined in o16 mode
	o__end
};

// 1 bit
enum
{
	xasm_def,					// default
	xasm_skip,					// don't assemble this encoding
	xasm__end
};

// 2 bits
enum
{
	ea_def,						// default
	ea_itself,					// lea - ea itself is used - no seg regs and no access
	ea_noaccess,				// invlpg - no access, seg reg is still used however.
	ea__end
};

// 1 bit (sx = sign extend)
enum
{
	sx_def,
	sx_yes,
	sx__end
};

// note: the order of elements here must match the order they are introduced in x86s_script.h.
struct encoding_suffix
{
	U4 mod : 3;
	U4 ro : 4;
	U4 rm : 4;
	U4 op66 : 2;
	U4 fx : 3;
	U4 o : 3;
	U4 xasm : 1;
	U4 ea : 2;
	U4 sx : 1;				// any imm args with non-byte size are sx'd to that size from a byte
	// 9 bits remaining
};

// --- end encoding suffixes ---

// --- begin argument types ---

// 2 bits
enum
{
	argtype_void,
	argtype_reg,
	argtype_mem,
	argtype_imm,
	argtype__end
};

// The remaining 'hi' argument types are presently 6 bits.

enum
{
	argtypehi_reg_r,
	argtypehi_reg_basecode,
	argtypehi_reg_cr,
	argtypehi_reg_dr,
	argtypehi_reg_sr
};

enum
{
	argtypehi_mem_mem,
	argtypehi_mem_rm,
	argtypehi_mem_disp,				// note: if asz is 64, this is a 64-bit displacement
	argtypehi_mem_strs,
	argtypehi_mem_strd,
	argtypehi_mem_xs
};

enum
{
	argtypehi_imm_imm,
	argtypehi_imm_rel,
	argtypehi_imm_implict8,			// implict
	argtypehi_imm_cc				// implict
};

#define ARGTYPE_HI_SHIFT 2
#define ARGTYPE_LO_MASK ((1 << ARGTYPE_HI_SHIFT) - 1)
#define ARGTYPE(lo, hi) ((lo) | ((hi) << ARGTYPE_HI_SHIFT))

// --- end argument types ---

// --- begin argument sizes ---

// Note: this list must match x86s_script.h.
enum
{
	// Do not change the values of these first 4 enum's.
	argsize_16 = 0,
	argsize_32 = 1,
	argsize_64 = 2,
	argsize_128 = 3,

	argsize_8,
	argsize_24,				// 32 bits but high 8 bits ignored on read (replaced with 0s) and 0s written on write (right?)
	argsize_512b,			// 512 bytes (fxsave, fxrstor)

	argsize_osz,
	argsize_osz_old,		// osz, but never 64 bits (16 or 32 bits)
	argsize_osz_new,		// 64 bits, or 128 if rex.w is used (used by _cmpxchgxb)
	argsize_osz_32,			// size is osz unless osz = 16, then use 32 bits (movsx, movzx)
	argsize_osz_ptr,		// 64 bits in long mode (default osz = 64), else 32 bits (vmread, vmwrite)
	argsize_osz_24,			// 64 bits if in long mode. else: ->24 if o16, ->32 if o32
	
	argsize_asz,			// used for loop,loopz,loopnz,jrcxz
	
	// 14..31 are available - we may need an argsize_80 for floating-point!
	
	argsize__end
};

enum
{
	// Do not change the values of these first 3 enum's.
	argsize_hi_16 = 0,
	argsize_hi_32 = 1,
	argsize_hi_64 = 2,

	argsize_hi_none,
	argsize_hi_24,			// 32 bits but high 8 bits ignored on read (replaced with 0s) and 0s written on write (right?)
	argsize_hi_osz,
	argsize_hi_osz_24,		// 64 bits if in long mode. else: ->24 if o16, ->32 if o32
	argsize_hi_osz_old,		// osz, but never 64 bits (16 or 32 bits)
	
	argsize_hi__end
};

#define ARGSIZE_HI_SHIFT 5
#define ARGSIZE_LO_MASK ((1 << ARGSIZE_HI_SHIFT) - 1)
#define ARGSIZE(lo, hi) ((lo) | ((hi) << ARGSIZE_HI_SHIFT))

// --- end argument sizes ---

// It's OK to add to the end of this, but do not otherwise change it!
// 'insn' must be first, then basecode, then nextbyte.
// It's currently only 20 bytes (MAX_ARGS = 4).
struct encoding_t
{
	//U4 insn : 22;
	//U4 basecode : 10;
	U4 insn;
	U4 basecode : 10;
	U4 nextbyte : 9;		// 511 for none, else 0..255
	//U4 avl : 13;
	encoding_suffix suffix;
	U1 argtype[MAX_ARGS];
	U1 argsize[MAX_ARGS];
	U1 argvalue[MAX_ARGS];
};

// This is part of icode_t.
struct ea_t
{
	U2 base : 5;		// 31 if none
	U2 index : 5;		// 31 if none
	U2 index_scale : 2;	// 0,1,2,3
	U2 sreg : 3;		// 6,7=2,3
	U2 disp8 : 1;		// displacement was 8 bits
};

// This is currently 28 bytes (MAX_ARGS = 4).
struct icode_t
{
	U4 imm;
	U4 disp;
	U4 insn : 22;
	U4 has_disp : 1;
	U4 has_imm : 1;
	U4 lockrep : 2;		// 0=default,1=lock,2=f2,3=f3
	U4 osz : 2;			// argsize_16, argsize_32, or argsize_64
	U4 asz : 2;			// argsize_16, argsize_32, or argsize_64
	U4 sx : 1;			// any imm arguments with non-byte size were sx'd from a byte
	U4 avl : 1;
	
	U1 argtype[MAX_ARGS];
	U1 argsize[MAX_ARGS];
	U1 argvalue[MAX_ARGS];
	U2 available;
	
	ea_t ea;
};

U1 get_argtype_lo(U1 x);
U1 get_argtype_hi(U1 x);
U1 get_argsize_lo(U1 x);
U1 get_argsize_hi(U1 x);	// warning--does translation!
U1 specialize_arg_size(U1 argsize, U4 dsz, U4 osz, U4 asz);
bool arg_is_osz_sensitive(U1 argsize);
UINT get_argsize_bytes(U1 x);	// added 01-03-2009. arg sizes must already be specialized.

}	// namespace x86s

#endif	// l_x86s__x86s_common_h__included

// x86s_semantics.h
// Copyright (C) 2008 Willow Schlanger

#ifndef l_x86s__x86s_semantics_h__included
#define l_x86s__x86s_semantics_h__included

#include "types.h"

#include "../crudcom/out_enum.h"

#if 0
// By default an operator has 1 or more operands of all the same size and that is the output
enum
{
	// These have a size of 'void'.
	nt_asgn,
	nt_void,		// this is used only at the end of the table itself, to terminate it
	// These always have their size specified, even if it's e.g. 'argsize0'.
	nt_zx,
	nt_sx,
	nt_trunc,
	nt_deref,
	//---
	nt_tmp,
	nt_arg,
	// These all have the same size as their non-immediate arguments, which must be the same.
	// They must not have only immediates as arguments--size would be unknown.
	// Immediates (literals) always have a size of ns_void.
	nt_add,
	nt_bitand,
	nt_bitnot,
	nt_bitor,
	nt_bitxor,
	nt_cmul,
	nt_neg,
	nt_root,
	nt_sdiv,
	nt_sign,
	nt_smod,
	nt_smul,
	nt_sub,
	nt_udiv,
	nt_umod,
	nt_umul,
	nt__x86__begin,
	nt__x86_rcl,
	nt__x86_rcr,
	nt__x86_rol,
	nt__x86_ror,
	nt__x86_shl,
	nt__x86_shld,
	nt__x86_shrd,
	nt__x86_sshr,
	nt__x86_ushr,
	nt__x86_cmov_mask,
	// These have an out size of 'bit'.
	nt__begin_bitops,
	nt_is_zero,
	nt_is_nonzero,
	nt_not,
	nt__x86_cc,
	nt__x86_parity,
	nt__end_bitops,
	// These have an out size of 'bit'.
	nt__x86__begin_sflags,
	nt__x86_adc_af,
	nt__x86_adc_cf,
	nt__x86_adc_of,
	nt__x86_add_af,
	nt__x86_add_cf,
	nt__x86_add_of,
	nt__x86_rcl_af,
	nt__x86_rcl_cf,
	nt__x86_rcl_of,
	nt__x86_rcl_pf,
	nt__x86_rcl_sf,
	nt__x86_rcl_zf,
	nt__x86_rcr_af,
	nt__x86_rcr_cf,
	nt__x86_rcr_of,
	nt__x86_rcr_pf,
	nt__x86_rcr_sf,
	nt__x86_rcr_zf,
	nt__x86_rol_af,
	nt__x86_rol_cf,
	nt__x86_rol_of,
	nt__x86_rol_pf,
	nt__x86_rol_sf,
	nt__x86_rol_zf,
	nt__x86_ror_af,
	nt__x86_ror_cf,
	nt__x86_ror_of,
	nt__x86_ror_pf,
	nt__x86_ror_sf,
	nt__x86_ror_zf,
	nt__x86_sbb_af,
	nt__x86_sbb_cf,
	nt__x86_sbb_of,
	nt__x86_shl_af,
	nt__x86_shl_cf,
	nt__x86_shl_of,
	nt__x86_shl_pf,
	nt__x86_shl_sf,
	nt__x86_shl_zf,
	nt__x86_shld_af,
	nt__x86_shld_cf,
	nt__x86_shld_of,
	nt__x86_shld_pf,
	nt__x86_shld_sf,
	nt__x86_shld_zf,
	nt__x86_shrd_af,
	nt__x86_shrd_cf,
	nt__x86_shrd_of,
	nt__x86_shrd_pf,
	nt__x86_shrd_sf,
	nt__x86_shrd_zf,
	nt__x86_sshr_af,
	nt__x86_sshr_cf,
	nt__x86_sshr_of,
	nt__x86_sshr_pf,
	nt__x86_sshr_sf,
	nt__x86_sshr_zf,
	nt__x86_sub_af,
	nt__x86_sub_cf,
	nt__x86_sub_of,
	nt__x86_ushr_af,
	nt__x86_ushr_cf,
	nt__x86_ushr_of,
	nt__x86_ushr_pf,
	nt__x86_ushr_sf,
	nt__x86_ushr_zf,
	nt__x86__end_sflags,
	nt__x86__end,
	//---
	nt_literal,
	nt__end
};


enum
{
	no_void,
	// this has a 'bit' size
	no_undefined,
	// ---	
	no__begin_x86,
	no__begin_x86_flags,
	no_x86_df,
	no_x86_af,
	no_x86_of,
	no_x86_pf,
	no_x86_sf,
	no_x86_zf,
	no_x86_cf,
	no__end_x86_flags,
	// ---
	no_x86_ah,
	no_x86_al,
	no_x86_dl,
	no_x86_ax,
	no_x86_dx,
	no_x86_eax,
	no_x86_edx,
	no_x86_rax,
	no_x86_rdx,
	no_x86_dx_ax,
	no_x86_edx_eax,
	no_x86_rdx_rax,
	// The following are specializable.
	no_x86_acc,
	no_x86_acc_lo,
	no_x86_dat,
	no_x86_dax,
	no__end_x86,
	no__end
};

#endif

enum
{
	ns_word = 0,
	ns_dword = 1,
	ns_qword = 2,
	ns_oword = 3,
	
	ns_real_4,
	ns_real_8,
	ns_real_10,
	
	ns_byte,
	ns_osz,
	ns_asz,
	ns_osz_times_2,
	
	ns_argsize_0,
	ns_argsize_1,
	ns_argsize_2,
	
	ns_void,				// no size -- also used when size is unknown (e.g. literals)
	ns_bit,
	
	ns__end
};

struct tnode
{
	U2 type;		// nt_* enum
	U2 size;		// ns_* enum
	// data[]: if nt_literal, then data[1]:data[0] is the 64-bit literal -- mask as appropriate.
	// Otherwise, if data[x] < 0x80000000 it's an index in the array of nodes for the operand/operator.
	// If data[x] >= 0x80000000 then data[x] & 0x7fffffff is a no_* 'node operand' enum value.
	U4 data[3];
};

struct tcode_element
{
	U4 *index;		// 0 if unimplemented, ends with an extra 0.
	U2 temps;		// number of temporaries this instruction needs
	U2 args;		// number of arguments this instruction has.
};

#endif	// l_x86s__x86s_semantics_h__included

// out_metadata  (note: automatically generated file)
// Copyright (C) 2009 Willow Schlanger

#include "imetadata.h"

// returns a list of external inputs.
const int *get_ext_inputs(int insn, int osz, int asz)
{
	switch(insn)
	{
		case insn__aad:
		{
			{
				static const int ext_data[] = {
					EXTERN_AH,
					EXTERN_AL,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__aam:
		{
			{
				static const int ext_data[] = {
					EXTERN_AL,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__callfd:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP1,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP1,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP1,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__callfi:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP1,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP1,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP1,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__calli:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIP1,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__cmps:
		{
			if(asz == 16)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					EXTERN_DI,
					EXTERN_SI,
					-1
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					EXTERN_EDI,
					EXTERN_ESI,
					-1
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__cmul2:
		{
			{
				static const int ext_data[] = {
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__cmul3:
		{
			{
				static const int ext_data[] = {
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__divb:
		{
			{
				static const int ext_data[] = {
					EXTERN_AX,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__idivb:
		{
			{
				static const int ext_data[] = {
					EXTERN_AX,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__imulb:
		{
			{
				static const int ext_data[] = {
					EXTERN_AL,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__iret:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jcc:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jmpfd:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jmpfi:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jmpi:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jrcxz:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__lods:
		{
			if(asz == 16)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					EXTERN_SI,
					-1
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					EXTERN_ESI,
					-1
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__loop:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__loopnz:
		{
			{
				static const int ext_data[] = {
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__loopz:
		{
			{
				static const int ext_data[] = {
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__movs:
		{
			if(asz == 16)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					EXTERN_DI,
					EXTERN_SI,
					-1
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					EXTERN_EDI,
					EXTERN_ESI,
					-1
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__movsrv:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__movvsr:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__mulb:
		{
			{
				static const int ext_data[] = {
					EXTERN_AL,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__pop:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__popa:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__popf:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__popsr:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__push:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__pusha:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AX,
					EXTERN_BP,
					EXTERN_BX,
					EXTERN_CX,
					EXTERN_DI,
					EXTERN_DX,
					EXTERN_SI,
					EXTERN_SP,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_EAX,
					EXTERN_EBP,
					EXTERN_EBX,
					EXTERN_ECX,
					EXTERN_EDI,
					EXTERN_EDX,
					EXTERN_ESI,
					EXTERN_ESP,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__pushf:
		{
			{
				static const int ext_data[] = {
					EXTERN_RFLAGS,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__pushsr:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__ret:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__retf:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__retfnum:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__retnum:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__sal:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__scas:
		{
			if(asz == 16)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					EXTERN_DI,
					-1
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					EXTERN_EDI,
					-1
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__setcc:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__stos:
		{
			if(asz == 16)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					EXTERN_DI,
					-1
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					EXTERN_EDI,
					-1
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const int ext_data[] = {
					EXTERN_DF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__sxacc:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AL,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_AX,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__sxdax:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AX,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_EAX,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__xchg:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__xlat:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_aaa:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AL,
					EXTERN_AX,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_aas:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AL,
					EXTERN_AX,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_adc:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_add:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_and:
		{
			{
				static const int ext_data[] = {
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_call:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIP1,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_clc:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_cld:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_cli:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_cmc:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_cmp:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_daa:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AL,
					EXTERN_CF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_das:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AL,
					EXTERN_CF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_dec:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_div:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AX,
					EXTERN_DX,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_EAX,
					EXTERN_EDX,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_enter:
		{
			{
				static const int ext_data[] = {
					EXTERN_RBP,
					EXTERN_RSP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_idiv:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AX,
					EXTERN_DX,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_EAX,
					EXTERN_EDX,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_imul:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AX,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_EAX,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_in:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_inc:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_int:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_jmp:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lahf:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lds:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lea:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_leave:
		{
			{
				static const int ext_data[] = {
					EXTERN_RBP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_les:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lfs:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lgs:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lss:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_mov:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_movsx:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_movzx:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_mul:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AX,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_EAX,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_neg:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_not:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_or:
		{
			{
				static const int ext_data[] = {
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_out:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_rcl:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_rcr:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_rol:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_ror:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sahf:
		{
			{
				static const int ext_data[] = {
					EXTERN_AH,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sar:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sbb:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_shl:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_shr:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_stc:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_std:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sti:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sub:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_test:
		{
			{
				static const int ext_data[] = {
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_xor:
		{
			{
				static const int ext_data[] = {
					EXTERN_undefined,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		default:
			break;
	};
	return 0;	// NULL
}

// returns a list of external outputs.
const int *get_ext_outputs(int insn, int osz, int asz)
{
	switch(insn)
	{
		case insn__aad:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AH,
					EXTERN_AL,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__aam:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AH,
					EXTERN_AL,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__callfd:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__callfi:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__calli:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__cmps:
		{
			if(asz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_DI,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_SI,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_EDI,
					EXTERN_ESI,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__cmul2:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__cmul3:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__divb:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AH,
					EXTERN_AL,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__idivb:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AH,
					EXTERN_AL,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__imulb:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AX,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__iret:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RFLAGS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RFLAGS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RFLAGS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jcc:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIPC,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jmpfd:
		{
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jmpfi:
		{
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jmpi:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jrcxz:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIPC,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__lods:
		{
			if(asz == 16)
			{
				static const int ext_data[] = {
					EXTERN_SI,
					-1
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const int ext_data[] = {
					EXTERN_ESI,
					-1
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__loop:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIPC,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__loopnz:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIPC,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__loopz:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIPC,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__movs:
		{
			if(asz == 16)
			{
				static const int ext_data[] = {
					EXTERN_DI,
					EXTERN_SI,
					-1
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const int ext_data[] = {
					EXTERN_EDI,
					EXTERN_ESI,
					-1
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__movsrv:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__movvsr:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__mulb:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AX,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__pop:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__popa:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AX,
					EXTERN_BP,
					EXTERN_BX,
					EXTERN_CX,
					EXTERN_DI,
					EXTERN_DX,
					EXTERN_SI,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_EAX,
					EXTERN_EBP,
					EXTERN_EBX,
					EXTERN_ECX,
					EXTERN_EDI,
					EXTERN_EDX,
					EXTERN_ESI,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__popf:
		{
			{
				static const int ext_data[] = {
					EXTERN_RFLAGS,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__popsr:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__push:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__pusha:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__pushf:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__pushsr:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__ret:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__retf:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__retfnum:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_CS,
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__retnum:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIP,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__sal:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__scas:
		{
			if(asz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_DI,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_EDI,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__setcc:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__stos:
		{
			if(asz == 16)
			{
				static const int ext_data[] = {
					EXTERN_DI,
					-1
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const int ext_data[] = {
					EXTERN_EDI,
					-1
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__sxacc:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AX,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_EAX,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__sxdax:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_DX,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_EDX,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__xchg:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__xlat:
		{
			{
				static const int ext_data[] = {
					EXTERN_AL,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_aaa:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AL,
					EXTERN_AX,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_aas:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AL,
					EXTERN_AX,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_adc:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_add:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_and:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_call:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIPC,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_clc:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_cld:
		{
			{
				static const int ext_data[] = {
					EXTERN_DF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_cli:
		{
			{
				static const int ext_data[] = {
					EXTERN_IF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_cmc:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_cmp:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_daa:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AL,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_das:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AL,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_dec:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_div:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AX,
					EXTERN_CF,
					EXTERN_DX,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_EAX,
					EXTERN_EDX,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_enter:
		{
			{
				static const int ext_data[] = {
					EXTERN_RBPX,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_idiv:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AX,
					EXTERN_CF,
					EXTERN_DX,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_EAX,
					EXTERN_EDX,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_imul:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AX,
					EXTERN_CF,
					EXTERN_DX,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_EAX,
					EXTERN_EDX,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_in:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_inc:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_int:
		{
			{
				static const int ext_data[] = {
					EXTERN_RINT,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_jmp:
		{
			{
				static const int ext_data[] = {
					EXTERN_RIPC,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lahf:
		{
			{
				static const int ext_data[] = {
					EXTERN_AH,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lds:
		{
			{
				static const int ext_data[] = {
					EXTERN_DS,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lea:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_leave:
		{
			{
				static const int ext_data[] = {
					EXTERN_RBPX,
					EXTERN_RSPX,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_les:
		{
			{
				static const int ext_data[] = {
					EXTERN_ES,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lfs:
		{
			{
				static const int ext_data[] = {
					EXTERN_FS,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lgs:
		{
			{
				static const int ext_data[] = {
					EXTERN_GS,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lss:
		{
			{
				static const int ext_data[] = {
					EXTERN_SS,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_mov:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_movsx:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_movzx:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_mul:
		{
			if(osz == 16)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_AX,
					EXTERN_CF,
					EXTERN_DX,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_EAX,
					EXTERN_EDX,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_neg:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_not:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_or:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_out:
		{
			{
				static const int ext_data[] = {
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_rcl:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					EXTERN_OF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_rcr:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					EXTERN_OF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_rol:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					EXTERN_OF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_ror:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					EXTERN_OF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sahf:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sar:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sbb:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_shl:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_shr:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_stc:
		{
			{
				static const int ext_data[] = {
					EXTERN_CF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_std:
		{
			{
				static const int ext_data[] = {
					EXTERN_DF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sti:
		{
			{
				static const int ext_data[] = {
					EXTERN_IF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sub:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_test:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_xor:
		{
			{
				static const int ext_data[] = {
					EXTERN_AF,
					EXTERN_CF,
					EXTERN_OF,
					EXTERN_PF,
					EXTERN_SF,
					EXTERN_ZF,
					-1
				};
				return ext_data;
			}
			break;	// return NULL
		}
		default:
			break;
	};
	return 0;	// NULL
}

// returns a list of argument inputs/outputs. bit 0 = in, bit 1 = out, bit 2 = end.
const char *get_arg_info(int insn, int osz, int asz)
{
	switch(insn)
	{
		case insn__aad:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__aam:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__callfd:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__callfi:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__calli:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__cmps:
		{
			if(asz == 16)
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__cmul2:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__cmul3:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__divb:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__idivb:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__imulb:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__iret:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jcc:
		{
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jmpfd:
		{
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jmpfi:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jmpi:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__jrcxz:
		{
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__lods:
		{
			if(asz == 16)
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__loop:
		{
			{
				static const char ext_data[] = {
					1,
					3,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__loopnz:
		{
			{
				static const char ext_data[] = {
					1,
					3,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__loopz:
		{
			{
				static const char ext_data[] = {
					1,
					3,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__movs:
		{
			if(asz == 16)
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__movsrv:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__movvsr:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__mulb:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__pop:
		{
			{
				static const char ext_data[] = {
					2,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__popa:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__popf:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__popsr:
		{
			{
				static const char ext_data[] = {
					2,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__push:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__pusha:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__pushf:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__pushsr:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__ret:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__retf:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__retfnum:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__retnum:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__sal:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__scas:
		{
			if(asz == 16)
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__setcc:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__stos:
		{
			if(asz == 16)
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(asz == 32)
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(asz == 64)
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__sxacc:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__sxdax:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__xchg:
		{
			{
				static const char ext_data[] = {
					3,
					3,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn__xlat:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_aaa:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_aas:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_adc:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_add:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_and:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_call:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_clc:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_cld:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_cli:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_cmc:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_cmp:
		{
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_daa:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_das:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_dec:
		{
			{
				static const char ext_data[] = {
					3,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_div:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_enter:
		{
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_idiv:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_imul:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_in:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_inc:
		{
			{
				static const char ext_data[] = {
					3,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_int:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_jmp:
		{
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lahf:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lds:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lea:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_leave:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_les:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lfs:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lgs:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_lss:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_mov:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_movsx:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_movzx:
		{
			{
				static const char ext_data[] = {
					2,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_mul:
		{
			if(osz == 16)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 32)
			{
				static const char ext_data[] = {
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			if(osz == 64)
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_neg:
		{
			{
				static const char ext_data[] = {
					3,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_not:
		{
			{
				static const char ext_data[] = {
					3,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_or:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_out:
		{
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_rcl:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_rcr:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_rol:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_ror:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sahf:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sar:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sbb:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_shl:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_shr:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_stc:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_std:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sti:
		{
			{
				static const char ext_data[] = {
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_sub:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_test:
		{
			{
				static const char ext_data[] = {
					1,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		case insn_xor:
		{
			{
				static const char ext_data[] = {
					3,
					1,
					4	// bit 2 set = end
				};
				return ext_data;
			}
			break;	// return NULL
		}
		default:
			break;
	};
	return 0;	// NULL
}

// returns some instruction meta-data.
const InsnMetaData *get_insn_metadata(int insn, int osz, int asz)
{
	switch(insn)
	{
		case insn__aad:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__aam:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__callfd:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						1,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						1,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						1,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__callfi:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						1,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						1,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						1,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__calli:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						1,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__cmps:
		{
			if(asz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(asz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(asz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__cmul2:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__cmul3:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__divb:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__idivb:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__imulb:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__iret:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						1,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						1,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						1,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__jcc:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						1,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__jmpfd:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__jmpfi:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__jmpi:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__jrcxz:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						1,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__lods:
		{
			if(asz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(asz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(asz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__loop:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						1,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__loopnz:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						1,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__loopz:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						1,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__movs:
		{
			if(asz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(asz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(asz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__movsrv:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__movvsr:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__mulb:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__pop:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__popa:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__popf:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						1,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__popsr:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__push:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__pusha:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__pushf:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						1,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__pushsr:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__ret:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__retf:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__retfnum:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__retnum:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						1,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__sal:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__scas:
		{
			if(asz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(asz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(asz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__setcc:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__stos:
		{
			if(asz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(asz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(asz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__sxacc:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__sxdax:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__xchg:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn__xlat:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_aaa:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_aas:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_adc:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_add:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_and:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_call:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						1,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						1,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_clc:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_cld:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_cli:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_cmc:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_cmp:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_daa:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_das:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_dec:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_div:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_enter:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						1,	// RBPX
						0,	// RSPX
						1	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_idiv:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_imul:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_in:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_inc:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_int:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						1,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_jmp:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						1,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_lahf:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_lds:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_lea:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_leave:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						1	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						1,	// RBPX
						1,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_les:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_lfs:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_lgs:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_lss:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_mov:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_movsx:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_movzx:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_mul:
		{
			if(osz == 16)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 32)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			if(osz == 64)
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_neg:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_not:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_or:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_out:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_rcl:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_rcr:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_rol:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_ror:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_sahf:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_sar:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_sbb:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_shl:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_shr:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_stc:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_std:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_sti:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_sub:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_test:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		case insn_xor:
		{
			{
				static const InsnMetaData mdata = { {
					{	// inputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					},
					{	// outputs
						0,	// RIP
						0,	// RIP1
						0,	// RIPC
						0,	// RINT
						0,	// RFLAGS
						0,	// RBPX
						0,	// RSPX
						0	// Stack
					}
				} };
				return &mdata;
			}
			break;	// return NULL
		}
		default:
			break;
	};
	return 0;	// NULL
}


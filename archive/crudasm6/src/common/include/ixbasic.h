// ixbasic.h
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#ifndef l_ixbasic_h__ceres_included
#define l_ixbasic_h__ceres_included

#include "../../common/include/types.h"
#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>

struct IntelArgTypes
{
	enum
	{
		Tvoid,
		//imm
			Timm_implict,
			Timm_1st,
			Timm_2nd,
			Timm_both,
		//mem
			//implict
				Tmem_implict_sts,
				Tmem_implict_std,
				Tmem_implict_xls,
			Tmem_fulldisp,
			Tmem_ea,
				Tmem_ea_lim,
				Tmem_ea_dbl,
				Tmem_ea_seg,
				Tmem_ea_eal,	// size = asz
				Tmem_ea_eai,
				Tmem_ea_fxs,
		//reg [bit 7 is set for "/mem"]
			Treg_gr,
			Treg_xmm,
			Treg_mmx,
			Treg_sr,
			Treg_dr,
			Treg_cr,
			Treg_st
	};
	
	static void dump_argtypes(std::ostream &os)
	{
		os << "/* For more information, see common/include/ixbasic.h */\n";
		os << "enum {\n";
		U1 n = 0;
		for(;;)
		{
			std::string s = getTypeBase(n);
			if(s == "")
				break;
			os << "\tcrudasm_intel_argtype_" << s;
			++n;
			if(getTypeBase(n) != "")
				os << ",";
			os << "\n";
		}
		os << "};\n" << std::endl;
		
		os << "\n";
		os << "static int crudasm_intel_argtype__is_mem_implict(U1 x)\n";
		os << "{\n";
		os << "\tx &= 0x7f;\n";
		os << "\treturn x >= crudasm_intel_argtype_mem_implict_sts && x <= crudasm_intel_argtype_mem_implict_xls;\n";
		os << "}\n";
		os << "\n";
		os << "static int crudasm_intel_argtype__is_mem_ea(U1 x)\n";
		os << "{\n";
		os << "\tx &= 0x7f;\n";
		os << "\treturn x >= crudasm_intel_argtype_mem_ea && x <= crudasm_intel_argtype_mem_ea_fxs;\n";
		os << "}\n";
		os << "\n";
		os << "static int crudasm_intel_argtype__is_imm(U1 x)\n";
		os << "{\n";
		os << "\tx &= 0x7f;\n";
		os << "\treturn x >= crudasm_intel_argtype_imm_implict && x <= crudasm_intel_argtype_imm_both;\n";
		os << "}\n";
		os << "\n";
		os << "static int crudasm_intel_argtype__is_mem(U1 x)\n";
		os << "{\n";
		os << "\tx &= 0x7f;\n";
		os << "\treturn x >= crudasm_intel_argtype_mem_implict_sts && x <= crudasm_intel_argtype_mem_ea_fxs;\n";
		os << "}\n";
		os << "\n";
		os << "static int crudasm_intel_argtype__is_reg(U1 x)\n";
		os << "{\n";
		os << "\tx &= 0x7f;\n";
		os << "\treturn x >= crudasm_intel_argtype_reg_gr && x <= crudasm_intel_argtype_reg_st;\n";
		os << "}\n";
		
		os << std::endl;
	}

	static bool is_reg(U1 x)
	{
		std::string y = getTypeBase(x);
		if(y[0] != 'r')  return false;
		if(y[1] != 'e')  return false;
		if(y[2] != 'g')  return false;
		if(y[3] != '_')  return false;
		return true;
	}

	static std::string getTypeBase(U1 n)
	{
		const char *lst[] =
		{
			"void",
			"imm_implict",
			"imm_1st",
			"imm_2nd",
			"imm_both",
			"mem_implict_sts",
			"mem_implict_std",
			"mem_implict_xls",
			"mem_fulldisp",
			"mem_ea",
			"mem_ea_lim",
			"mem_ea_dbl",
			"mem_ea_seg",
			"mem_ea_eal",
			"mem_ea_eai",
			"mem_ea_fxs",
			"reg_gr",
			"reg_xmm",
			"reg_mmx",
			"reg_sr",
			"reg_dr",
			"reg_cr",
			"reg_st",
			NULL
		};
		if((n & 0x7f) > Treg_st)
			return "";
		return lst[n & 0x7f];
	}
	
	static std::string getTypeC(U1 n)
	{
		std::string s = getTypeBase(n);
		if((n & 0x80) != 0)
			s += "|0x80";
		return s;
	}
	
	static std::string getType(U1 n)
	{
		std::string s = getTypeBase(n);
		if((n & 0x80) != 0)
			s += "/mem";
		return s;
	}

	// Returns Tvoid on error, or if "void" is passed in.
	static U1 getType(std::string s)
	{
		const char *lst[] =
		{
			"void",
			"imm_implict",
			"imm_1st",
			"imm_2nd",
			"imm_both",
			"mem_implict_sts",
			"mem_implict_std",
			"mem_implict_xls",
			"mem_fulldisp",
			"mem_ea",
			"mem_ea_lim",
			"mem_ea_dbl",
			"mem_ea_seg",
			"mem_ea_eal",
			"mem_ea_eai",
			"mem_ea_fxs",
			"reg_gr",
			"reg_xmm",
			"reg_mmx",
			"reg_sr",
			"reg_dr",
			"reg_cr",
			"reg_st",
			NULL
		};

		U1 adder = 0;

		for(std::string::iterator i = s.begin(); i != s.end(); ++i)
		{
			if(*i == '/')
			{
				adder = 128;	// "/mem" exists in argument
				s = std::string(s.begin(), i);
				break;
			}
		}

		for(int i = 0; lst[i] != NULL; ++i)
		{
			if(s == lst[i])
				return i + adder;
		}

		return Tvoid;	// unknown
	}
};

struct IntelArgSizes
{
	// -1      : osz       : operand size
	// -2      : asz       : address size
	// -3      : osz_ptr   : 64 bits in 64 bit mode regardless of rex; else 32 bits
	// -4      : osz_max32 : 16 or 32 bits only (ignore rex.w for this argument)
	// -5      : osz_min32 : 32 or 64 bits only (if o16, use 32 bits)
	// -6      : osz_min64 : 64 bits (128 bits if rex.w is used)
	// -7      : osz_seg   : 16 bits unless rex.w is used, then 64 bits (mov dest,sreg [intel manual])
	// -8      : osz_64in64: osz, but ALWAYS 64 bits in 64 bit mode
	enum
	{
		Tvoid = 0,
		Tosz = -1,
		Tasz = -2,
		Tosz_ptr = -3,
		Tosz_max32 = -4,
		Tosz_min32 = -5,
		Tosz_min64 = -6,
		Tosz_seg = -7,
		Tosz_64in64 = -8
	};

	static void dump_argsizes(std::ostream &os)
	{
		const char *lst[] = {
			"void",
			"osz",
			"asz",
			"osz_ptr",
			"osz_max32",
			"osz_min32",
			"osz_min64",
			"osz_seg",
			"osz_64in64",
			""
		};

		os << "/* For more information, see common/include/ixbasic.h */\n";
		os << "enum {\n";
		U1 n = 0;
		int val = 0;
		for(;;)
		{
			std::string s = lst[n];
			if(s == "")
				break;
			os << "\tcrudasm_intel_argsize_" << s;
			os << " = " << val;
			--val;
			++n;
			if(lst[n] != "")
				os << ",";
			os << "\n";
		}
		os << "};\n" << std::endl;
		
		os << "\n";
		os << "static int crudasm_intel_argsize__osz_sensitive(S1 x)\n";
		os << "{\n";
		os << "\tif(x >= 0)\n";
		os << "\t\treturn 0;\n";
		os << "\tif(x == crudasm_intel_argsize_asz)\n";
		os << "\t\treturn 0;\n";
		os << "\treturn 1;\n";
		os << "}\n";
		os << "\n";
	}

	static S1 getSize(std::string s)
	{
		const char *lst[] = {
			"void",
			"osz",
			"asz",
			"osz_ptr",
			"osz_max32",
			"osz_min32",
			"osz_min64",
			"osz_seg",
			"osz_64in64",
			NULL
		};
		if(s.size() >= 2 && s[0] == 'B' && s[1] >= '0' && s[1] <= '9')
		{
			int xx = 0;
			std::string::iterator i = ++s.begin();
			while(i != s.end())
			{
				xx = (xx * 10) + (*i - '0');
				++i;
			}
			if(xx == 0 || xx > 127)
				return 0;	// invalid
			return xx;
		}
		for(int i = 0; lst[i] != NULL; ++i)
		{
			if(s == lst[i])
				return -i;
		}
		return 0;	// not found
	}

	static std::string getSize(S1 x)
	{
		if(x == 0)
			return "void";
		if(x > 0)
		{
			char tmp[33];
			std::sprintf(tmp, "B%d", (int)(x));
			std::string s = tmp;
			return s;
		}
		if(x < -8)
			return NULL;
		x = -x;
		const char *lst[] = {
			"void",
			"osz",
			"asz",
			"osz_ptr",
			"osz_max32",
			"osz_min32",
			"osz_min64",
			"osz_seg",
			"osz_64in64",
			NULL
		};
		return lst[x];
	}

	static std::string getSizeC(S1 x)
	{
		if(x == 0)
			return "void";
		if(x > 0)
		{
			char tmp[33];
			std::sprintf(tmp, "%d", (int)(x));
			std::string s = tmp;
			return s;
		}
		if(x < -8)
			return NULL;
		x = -x;
		const char *lst[] = {
			"void",
			"osz",
			"asz",
			"osz_ptr",
			"osz_max32",
			"osz_min32",
			"osz_min64",
			"osz_seg",
			"osz_64in64",
			NULL
		};
		return lst[x];
	}
};

#endif	// l_ixbasic_h__ceres_included

// readxml.h
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#ifndef l_readxml_h__crudasm_included
#define l_readxml_h__crudasm_included

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <ctype.h>
#include <stddef.h>
#include <list>
#include <map>
#include <set>
#include <stdlib.h>
#include "../../common/include/types.h"
#include "../../common/include/ixbasic.h"

inline bool isValidDisasmIdent(std::string ident, int num_args)
{
	// write the given string (quotes removed).
	if(ident == "write")
		return num_args == 1;

	// write a separator between the opcode and operands.
	if(ident == "space")
		return num_args == 0;

	// ndisasm: show argument only if imm32 is not 0x0a.
	if(ident == "maybe_write_space_args_imm32")
		return num_args == 1;

	// write_args().
	if(ident == "write_args")
		return num_args == 0;

	/// write_arg(int arg, bool need_size = true, bool xoverride = false, bool no_short = false);
	if(ident == "write_arg")
		return num_args >= 1 && num_args <= 4;

	// write rep/repz/repnz.
	if(ident == "write_rep" || ident == "write_repcc")
		return num_args == 0;

	// write a size suffix (w, d, q, x, b) based on the size of the indicated argument.
	if(ident == "write_size_suffix_argsize")
		return num_args == 1;

	// write a size suffix (w, d, q, x, b) based on the operand size (osz).
	if(ident == "write_size_suffix_osz")
		return num_args == 0;

	// write a segment register PREFIX if not default, i.e. if override was used.
	if(ident == "write_seg_reg")
		return num_args == 0;

	// write an operand separator (comma).
	if(ident == "comma")
		return num_args == 0;

	// write the 1st argument if osz is 64 bits, else write 2nd argument.
	if(ident == "write_osz64")
		return num_args == 2;

	// exact meaning follows:
	/*
		if(get_dsz() == 2)
		{
			// 64bit mode.
			if(get_osz() == 0)
				write("o16 ");
		}
		else
		if(get_dsz() != get_osz())
		{
			write_o16_o32_o64();
		}
	*/
	if(ident == "write_stack_o16_o32_o64")
		return num_args == 0;

	// do_nop_xchg("nop", "xchg", 1) means: argvalue(1) is 0 & basecodei s 0x90, nop else do xchg
	if(ident == "do_nop_xchg")
		return num_args == 3;

	// exact meaning follows:
	/*
		if(get_dsz() == 2)
		{
			// 64bit mode.
			if(get_osz() != 1)
				write_o16_o32_o64();
		}
		else
		if(get_dsz() != get_osz())
		{
			write_o16_o32_o64();
		}
	*/
	if(ident == "write_xlat_o16_o32_o64")
		return num_args == 0;

	// if osz == 0, write 1st argument.
	// if osz == 1, write 2nd argument.
	// if osz == 2, write 3rd argument.
	if(ident == "write_osz")
		return num_args == 3;

	// if asz == 0, write 1st argument.
	// if asz == 1, write 2nd argument.
	// if asz == 2, write 3rd argument.
	if(ident == "write_asz")
		return num_args == 3;

	// Write a far immediate. We use both imm (for the offset) & disp (for segment).
	// Never happens in 64bit mode.
	if(ident == "write_far_imm")
		return num_args == 0;

	// exact meaning follows:
	/*
		if(get_dsz() != 2)
		{
			if(get_osz() != get_dsz())
				write_o16_o32_o64();
		}
	*/
	if(ident == "write_jrcxz_o16_o32_o64")
		return num_args == 0;

	return false;
}

struct XmlElement
{
	const std::string *name;
	std::map<std::string, std::string> attributes;
	std::list<XmlElement> children;
	std::vector<XmlElement *> child;
	std::map<std::string, XmlElement *> names;
	
	std::string attr(std::string key)
	{
		if(attributes.find(key) == attributes.end())
			return "";
		return attributes[key];
	}
	
	XmlElement *tag(std::string key)
	{
		if(names.find(key) == names.end())
			return NULL;
		return names[key];
	}
};

// This is constructed from an <insn> tag. It's used by IntelSyntax and IntelSemantics.
// 'name' and 'alias' are indexes into IntelSyntax::strTabSymbols[].
class IntelPrototype
{
public:
	std::set<U4> tags;
	U4 name;			// this number represents the instruction name.
	U4 alias;
	U1 numArgs;
	S1 argSizes[4];		// 1..127 bytes; a size of <0 is a template parameter size; 0 means void
};

struct IntelSyntaxEncoding
{
	std::set<U4> tags;

	U4 encoding_num;

	U4 insn : 30;		// index into IntelSyntax::insnsX[]
	U4 op66 : 2;		// 0=default, 1=no66, 2=66

	// opcode1 : 100..1ff = 0f <low 8 bits>
	U4 opcode1 : 10;	// 0..2ff (200..2ff = 3DNow!, opcode 2 is the 3DNow! imm8 opcode after modr/m)
	U4 opcode2 : 9;		// 0..ff, 100 for none
	U4 mod : 3;			// 0..3, 4={0,1,2} (i.e. /mdm), 7=none
	U4 regop : 4;		// 0..7, 8=/r, f=none
	U4 rm : 4;			// 0..7, f=none
	U4 fx : 2;			// 0=nofx, 1=default, 2=f2, 3=f3

	U1 argtype[4];		// 0 for void
	S1 argsize[4];		// 1..127 bytes; 0 for void. If <0, see IntelArgSizes for enumeration.
	U1 argvalue[4];		// default value is 0
	
	std::string get_insn_name(class IntelSyntax *syn, U4 insn);

	void out_values(std::ostream &os, std::map<U4, std::string> all_tags, class IntelSyntax *syn)
	{
		os << "{";
		os << std::hex;
		//os << "0x" << insn << ", ";
		os << "crudasm_intel_insn_" << get_insn_name(syn, insn) << ", ";
		os << "0x" << op66 << ", ";
		os << "0x" << opcode1 << ", ";
		os << "0x" << opcode2 << ", ";
		os << "0x" << mod << ", ";
		os << "0x" << regop << ", ";
		os << "0x" << rm << ", ";
		os << "0x" << fx << ", ";

		os << "{";
		for(int i = 0; i < 4; ++i)
		{
			if(i != 0)
				os << ",";
			os << "crudasm_intel_argtype_" << IntelArgTypes::getTypeC(argtype[i]);
		}
		os << "},";

		os << "{";
		for(int i = 0; i < 4; ++i)
		{
			if(i != 0)
				os << ",";
			std::string s = IntelArgSizes::getSizeC(argsize[i]);
			if(!s.empty() && s[0] >= '0' && s[0] <= '9')
				os << s;
			else
				os << "crudasm_intel_argsize_" << s;
		}
		os << "},";

		os << "{";
		for(int i = 0; i < 4; ++i)
		{
			if(i != 0)
				os << ",";
			os << "0x" << (U4)(U1)argvalue[i];
		}
		os << "}";
		
		for(std::map<U4, std::string>::iterator i = all_tags.begin(); i != all_tags.end(); ++i)
		{
			os << ",";
			if(tags.find(i->first) == tags.end())
				os << "0";
			else
				os << "1";
			os << "/*" << i->second << "*/";
		}
		
		os << std::dec;
		os << "}";
	}
	
	static void out_self(std::ostream &os, std::map<U4, std::string> all_tags)
	{
		os << "struct crudasm_intel_encoding_t {\n";
		os << "\tU4 insn : 30;		// instruction number (index)\n";
		os << "\tU4 op66 : 2;		// 0=default, 1=no66, 2=66\n";
		os << "\n";
		os << "\t// opcode1 : 100..1ff = 0f <low 8 bits>\n";
		os << "\tU4 opcode1 : 10;	// 0..2ff (200..2ff = 3DNow!, opcode 2 is the 3DNow! imm8 opcode after modr/m)\n";
		os << "\tU4 opcode2 : 9;		// 0..ff, 100 for none\n";
		os << "\tU4 mod : 3;			// 0..3, 4={0,1,2} (i.e. /mdm), 7=none\n";
		os << "\tU4 regop : 4;		// 0..7, 8=/r, f=none\n";
		os << "\tU4 rm : 4;			// 0..7, f=none\n";
		os << "\tU4 fx : 2;			// 0=nofx, 1=default, 2=f2, 3=f3\n";
		os << "\n";
		os << "\tU1 argtype[4];		// 0 for void\n";
		os << "\tS1 argsize[4];		// 1..127 bytes; 0 for void. If <0, see IntelArgSizes for enumeration.\n";
		os << "\tU1 argvalue[4];		// default value is 0\n";
		os << "\n";
		for(std::map<U4, std::string>::iterator i = all_tags.begin(); i != all_tags.end(); ++i)
		{
			os << "\tU1 flag_" << i->second << " : 1;\n";
		}
		os << "};" << std::endl;
	}

	IntelSyntaxEncoding()
	{
		encoding_num = 0;
		insn = 0;
		op66 = 0;

		opcode1 = 0;
		opcode2 = 0x100;
		mod = 7;
		regop = 0xf;
		rm = 0xf;
		fx = 1;

		for(int i = 0; i < 4; ++i)
		{
			argtype[i] = 0;		// void
			argsize[i] = 0;		// void
			argvalue[i] = 0;	// default
		}
	}
};

struct IntelSyntaxDisStmt
{
	U4 ident;
	std::vector<std::string> args;	// if starts with '\"' then it's a string, else must start with 0x and must be a number
};

struct IntelSyntaxInsn
{
	U4 insn_num;				// this is the instruction number, NOT a number representing the instruction name!
	IntelPrototype proto;
	std::list<IntelSyntaxEncoding> encodings;
	std::map<U4, std::list<IntelSyntaxDisStmt> > disasm;
	std::list<XmlElement *> emulst;
};

// This class contains a symbol table, which is used for both syntax and semantics.
// See also: IntelSemantics (represents semantics), IntelDisassembler, and IntelDecoder.
class IntelSyntax
{
	friend class IntelDecoderTableBuilder;
	std::map<std::string, U4> strTabNums;
	std::vector<const std::string *> strTabSymbols;

	std::list<IntelSyntaxInsn> insnList;
	std::vector<IntelSyntaxInsn *> insnsX;	// for random access
	std::map<U4, U4> insnNames;				// symbol table number --> index into insnsX[]
	std::vector<IntelSyntaxEncoding *> encodings;	// for random access
public:

	IntelSyntax() { }
	~IntelSyntax() { }
	std::string load(class XmlReader &intel_xml);	// returns "" on success, else an error message

	U4 getSymbol(std::string symbol);
	std::string getSymbol(U4 num);

	size_t getInsnCount() const { return insnList.size(); }
	IntelSyntaxInsn *getInsn(size_t num) { return insnsX[num]; }
	IntelSyntaxEncoding *getEncoding(size_t num) { return encodings[num]; }
	size_t getEncodingCount() const { return encodings.size(); }
};

struct ReadIntelXml
{
	XmlReader *readerT;
	IntelSyntax syntax;
	
	ReadIntelXml();
	~ReadIntelXml();

	// This should not be called more than once.
	int load(const char *fn);
};

#endif	// l_readxml_h__crudasm_included


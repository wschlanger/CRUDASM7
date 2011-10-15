// ixsyntax.h
// Copyright (C) 2011 Willow Schlanger

#ifndef l_ixsyntax_h__ceres_included
#define l_ixsyntax_h__ceres_included

#include "ixbasic.h"
#include "../../common/include/types.h"
#include <QtCore/QCoreApplication>
#include <QtXml>
#include <iostream>
#include <QFile>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <cstdio>

int getBSize(std::string s);

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
	QString load(QDomElement &intel_xml);	// returns "" on success, else an error message

	U4 getSymbol(std::string symbol);
	std::string getSymbol(U4 num);

	size_t getInsnCount() const { return insnList.size(); }
	IntelSyntaxInsn *getInsn(size_t num) { return insnsX[num]; }
	IntelSyntaxEncoding *getEncoding(size_t num) { return encodings[num]; }
};

class IntelDecoderTableBuilder
{
	IntelSyntax *syntax;
public:

	IntelDecoderTableBuilder(IntelSyntax *syntaxT) :
		syntax(syntaxT)
	{
	}

	std::string buildTable(std::vector<U4> &table);
	std::string process_decoder_encoding(IntelSyntaxEncoding &enc, class OpcodeTableContainer *opcon, std::string insn_name);
	std::string process_decoder_modrm(class OpcodeTable *table, size_t offset, IntelSyntaxEncoding &enc, class OpcodeTableContainer *opcon, std::string insn_name);
	std::string process_decoder_entry(class OpcodeTable *table, size_t offset, IntelSyntaxEncoding &enc, class OpcodeTableContainer *opcon, std::string insn_name, int ext_index, int md_index, int size_override = -1);
};

// This class decodes Intel x86/x64 machine code into icode.
class IntelDecoder
{
	friend class IntelInternalDecoder;
	friend class IntelDisassembler;

	IntelSyntax *syntax;
	std::vector<U4> decoderTable;

	U4 symbol_fwaitable;
	U4 symbol_no64;
	U4 symbol_no_rex_w;
	U4 symbol_is64;
	U4 symbol_lockable;
	U4 symbol_lock_always;
	U4 symbol_sx_byte;
	U4 symbol_imm64_disp;
	U4 symbol_imm64_sx32;
	U4 symbol_reg_base;
	U4 symbol_reg_rm;
public:

	IntelDecoder(IntelSyntax *syntaxT) :
		syntax(syntaxT)
	{
	}

	IntelSyntax *getSyntax()
	{
		return syntax;
	}

	QString init()
	{
		symbol_fwaitable = syntax->getSymbol("fwaitable");
		symbol_no64 = syntax->getSymbol("no64");
		symbol_no_rex_w = syntax->getSymbol("no_rex_w");
		symbol_is64 = syntax->getSymbol("is64");
		symbol_lockable = syntax->getSymbol("lockable");
		symbol_lock_always = syntax->getSymbol("lock_always");
		symbol_sx_byte = syntax->getSymbol("sx_byte");
		symbol_imm64_disp = syntax->getSymbol("imm64_disp");
		symbol_imm64_sx32 = syntax->getSymbol("imm64_sx32");
		symbol_reg_base = syntax->getSymbol("reg_base");
		symbol_reg_rm = syntax->getSymbol("reg_rm");

		IntelDecoderTableBuilder bld(syntax);
		return QString(bld.buildTable(decoderTable).c_str());
	}

	bool encoding_is_fwaitable(U4 encoding_num)
	{
		IntelSyntaxInsn *insn = syntax->getInsn(syntax->getEncoding(encoding_num)->insn);
		return insn->proto.tags.find(symbol_fwaitable) != insn->proto.tags.end();
	}
};

#endif	// l_ixsyntax_h__ceres_included

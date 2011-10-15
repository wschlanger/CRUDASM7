// mkdisasm.cpp
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "../readxml/readxml.h"
#include <cassert>
#include <fstream>
#include <stddef.h>

std::string int_to_string(int x)
{
	char s[33];
	using namespace std;
	sprintf(s, "%d", x);
	return std::string(s);
}

int mkdisasm(ReadIntelXml &reader)
{
	IntelSyntax &syntax = reader.syntax;
	
	std::ofstream fo("../../generated/out_intel_disasm_nasm.h");
	if(fo == NULL)
	{
		std::cout << "Unable to open file for writing: ../../generated/out_intel_disasm_nasm.h" << std::endl;
		return 1;
	}

	fo << "// out_intel_disasm_nasm.h  (note: this file was automatically generated -- do not edit!)\n";
	fo << "// Copyright (C) 2011 Willow Schlanger. All rights reserved.\n";

	fo << "\n";
	
	fo << "static int crudasm_intel_disasm_special_nasm(struct crudasm_intel_disasm_context_t *context, U4 insn)  {\n";	
	fo << "\tswitch(insn)  {\n";
	
	U4 str_nasm = syntax.getSymbol("nasm");
	U4 str_all = syntax.getSymbol("all");

	for(size_t i = 0; i < syntax.getInsnCount(); ++i)
	{
		IntelSyntaxInsn *insn = reader.syntax.getInsn(i);
		
		if(insn->disasm.empty())
			continue;
		U4 code = 0;
		if(insn->disasm.find(str_nasm) != insn->disasm.end())
			code = str_nasm;
		else
		if(insn->disasm.find(str_all) != insn->disasm.end())
			code = str_all;
		else
			continue;
		
		std::string insn_name = syntax.getSymbol(insn->proto.name);		

		std::list<IntelSyntaxDisStmt> &lst = insn->disasm[code];
		if(lst.empty())
		{
			std::cout << "ERROR: Don\'t know how to disassemble instruction: " << insn_name << std::endl;
			return 1;
		}
		fo << "\t\tcase crudasm_intel_insn_" << insn_name << ":\n";
		for(std::list<IntelSyntaxDisStmt>::iterator j = lst.begin(); j != lst.end(); ++j)
		{
			std::string ident = syntax.getSymbol(j->ident);
		
			fo << "\t\t\tixdis" << j->args.size() << "_" << ident << "(context";
			for(size_t k = 0; k < j->args.size(); ++k)
			{
				fo << ", ";
				fo << j->args[k];
			}
			fo << ");\n";
		}
		fo << "\t\t\treturn 1;\n";
	}
	
	fo << "\t\tdefault:\n";
	fo << "\t\t\tbreak;\n";
	
	fo << "\t}\n";
	fo << "\treturn 0;\n";
	fo << "}\n";
	fo << std::endl;
	return 0;
}

//===============================================================================================//
// Main program.
//===============================================================================================//

int main(int argc, const char **argv)
{
	if(argc != 2)
	{
		std::cout << "Usage: mkdisasm ../../generated/intel.xml" << std::endl;
		return 1;
	}
	ReadIntelXml reader;
	if(reader.load(argv[1]) != 0)
		return 1;
	
	return mkdisasm(reader);
}


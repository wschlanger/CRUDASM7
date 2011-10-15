// mkemu.cpp
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

long string_to_int(std::string s)
{
	return strtol(s.c_str(), NULL, 0);
}

void get_args_read(XmlElement *e, U8 *result, bool skip_first)
{
	size_t start = (skip_first) ? 1 : 0;
	for(size_t i = start; i < e->child.size(); ++i)
	{
		if(*e->child[i]->name == "arg")
		{
			long x = string_to_int(e->child[i]->attr("num"));
			*result |= 1 << x;
		}
		else
			get_args_read(e->child[i], result, false);
	}
}

int getReg8(std::string s)
{
	const char *regs8[] = {"AL", "CL", "DL", "BL", "SPL", "BPL", "SIL", "DIL", "R8B", "R9B", "R10B", "R11B", "R12B", "R13B", "R14B", "R15B", "AH", "CH", "DH", "BH", NULL};
	for(int i = 0; regs8[i] != NULL; ++i)
	{
		if(s == std::string(regs8[i]))
			return i;
	}
	return -1;
}

int getReg16(std::string s)
{
	const char *regs16[] = {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI", "R8W", "R9W", "R10W", "R11W", "R12W", "R13W", "R14W", "R15W", NULL};
	for(int i = 0; regs16[i] != NULL; ++i)
	{
		if(s == std::string(regs16[i]))
			return i;
	}
	return -1;
}

int getReg32(std::string s)
{
	const char *regs32[] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI", "R8D", "R9D", "R10D", "R11D", "R12D", "R13D", "R14D", "R15D", NULL};
	for(int i = 0; regs32[i] != NULL; ++i)
	{
		if(s == std::string(regs32[i]))
			return i;
	}
	return -1;
}

int getReg64(std::string s)
{
	const char *regs64[] = {"RAX", "RCX", "RDX", "RBX", "RSP", "RBP", "RSI", "RDI", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", NULL};
	for(int i = 0; regs64[i] != NULL; ++i)
	{
		if(s == std::string(regs64[i]))
			return i;
	}
	return -1;
}

int getSegReg(std::string s)
{
	const char *regs[] = {"ES", "CS", "SS", "DS", "FS", "GS", NULL};
	for(int i = 0; regs[i] != NULL; ++i)
	{
		if(s == std::string(regs[i]))
			return i;
	}
	return -1;
}

int getFlag(std::string s)
{
	if(s == "CF")  return 0;
	if(s == "PF")  return 2;
	if(s == "AF")  return 4;
	if(s == "ZF")  return 6;
	if(s == "SF")  return 7;
	if(s == "TF")  return 8;
	if(s == "IF")  return 9;
	if(s == "DF")  return 10;
	if(s == "OF")  return 11;
	if(s == "IOPL0")  return 12;
	if(s == "IOPL1")  return 13;
	if(s == "NT")  return 14;
	if(s == "RF")  return 16;
	if(s == "VM")  return 17;
	if(s == "AC")  return 18;
	if(s == "VIF")  return 19;
	if(s == "VIP")  return 20;
	if(s == "ID")  return 21;
	return -1;
}

std::set<std::string> internal_names;
void out_expr(std::string insn_name, std::ostream &fo, XmlElement *src)
{
	std::string size = src->attr("size");
	if(size.empty())
	{
		std::cout << insn_name << " : missing size : " << src->attr("type") << std::endl;
		throw __LINE__;
	}
	std::string size2;
	if(size == "BIT")
		size2 = "1";
	else
	if(!size.empty() && size[0] == 'B')
	{
		size2 = std::string(++size.begin(), size.end());
	}
	else
	{
		size2 = "get_arg_size(context, " + size + ")";
	}
	
	if(*src->name == "rhs")
	{
		std::string type = src->attr("type");
		if(type == "num")
		{
			fo << "internal_make_num(context, ";
			fo << size2;
			fo << ", 0, ";
			fo << src->attr("value");
			fo << ")";
		}
		else
		if(type == "fcn" || type == "zx" || type == "sx" || type == "tr")
		{
			std::string name = type;
			if(type == "fcn")
			{
				name = src->attr("value");
				if(!name.empty())
				{
					// Remove _'s from end of function names.
					if(name[name.size() - 1] == '_')
						name = std::string(name.begin(), --name.end());
				}
			}
			std::string s_fcn;
			if(type != "fcn")
			{
				fo << "internal_compute_" << name << "(context, " << size2 << ", ";
				s_fcn = "internal_compute_" + name;
			}
			else
			{
				fo << "internal_compute_" << src->child.size() << "_" << name << "(context, ";
				s_fcn = "internal_compute_" + int_to_string(src->child.size()) + "_" + name;
			}
			internal_names.insert(s_fcn);
			
			for(size_t i = 0; i < src->child.size(); ++i)
			{
				if(i != 0)
					fo << ", ";
				out_expr(insn_name, fo, src->child[i]);
			}
			fo << ")";
		}
		else
		if(type == "sizeof")
		{
			// internal_get_argsize() returns a B16 containing the size of the indicated argument.
			fo << "internal_get_argsize(context, " << src->attr("value") << ")";
		}
		else
		{
			std::cout << insn_name << " : unknown rhs type : " << src->attr("type") << std::endl;
			throw __LINE__;
		}
	
		return;
	}
	
	if(*src->name == "arg")
	{
		fo << "argtmp[" << src->attr("num") << "]";
		return;
	}
	
	if(*src->name == "temp")
	{
		fo << "tmp[" << src->attr("num") << "]";
		return;
	}
	
	if(*src->name == "extern")
	{
		std::string nm = src->attr("name");
		
		if(getReg8(nm) >= 0)
			fo << "internal_get_reg8(context, " << getReg8(nm) << ")";
		else		
		if(getReg16(nm) >= 0)
			fo << "internal_get_reg16(context, " << getReg16(nm) << ")";
		else		
		if(getReg32(nm) >= 0)
			fo << "internal_get_reg32(context, " << getReg32(nm) << ")";
		else		
		if(getReg64(nm) >= 0)
			fo << "internal_get_reg64(context, " << getReg64(nm) << ")";
		else		
		if(getSegReg(nm) >= 0)
			fo << "internal_get_segreg(context, " << getSegReg(nm) << ")";
		else		
		if(getFlag(nm) >= 0)
			fo << "internal_get_flag(context, " << getFlag(nm) << ")";
		else		
			fo << "internal_get_extern_" << nm << "(context)";

		return;
	}
	
	std::cout << insn_name << " : unknown expression type : " << *src->name << std::endl;
	throw __LINE__;
}

int mkemu(ReadIntelXml &reader)
{
	IntelSyntax &syntax = reader.syntax;
	
	std::ofstream fo("../../generated/out_intel_emulator.h");
	if(fo == NULL)
	{
		std::cout << "Unable to open file for writing: ../../generated/out_intel_disasm_nasm.h" << std::endl;
		return 1;
	}

	fo << "// out_intel_emulator.h  (note: this file was automatically generated -- do not edit!)\n";
	fo << "// Copyright (C) 2011 Willow Schlanger. All rights reserved.\n";
	fo << "\n";

	for(size_t i = 0; i < syntax.getInsnCount(); ++i)
	{
		IntelSyntaxInsn *insn = reader.syntax.getInsn(i);
		if(!insn->emulst.empty())
		{
			std::string insn_name = syntax.getSymbol(insn->proto.name);
			for(std::list<XmlElement *>::iterator j = insn->emulst.begin(); j != insn->emulst.end(); ++j)
			{
				XmlElement *e = *j;
				fo << "\n";
				fo << "static int crudasm_intel_execute";
				if(e->attr("osz") != "any")
					fo << "_o" << e->attr("osz");
				if(e->attr("asz") != "any")
					fo << "_a" << e->attr("asz");
				if(e->attr("ssz") != "any")
					fo << "_s" << e->attr("ssz");
				fo << "__" << insn_name << "(struct crudasm_intel_context_t *context)  {\n";
				
				// "BIT" --> 1 byte, only bit 0 significant
				// "B#" --> # bytes
				// "#" --> same size as argument #
				std::map<size_t, std::string> tmpsizes;		// first arg # -> size (see above)
				std::vector<size_t> tmpnums;				// 0 -> first arg #, etc.
				std::map<size_t, size_t> tmpmap;			// first arg # -> 0, etc.
				XmlElement *allocs = e->tag("allocs");
				if(allocs != NULL)
				{
					for(size_t k = 0; k < allocs->child.size(); ++k)
					{
						XmlElement *alloc_temp = allocs->child[k];
						if(*alloc_temp->name == "alloc_temp")
						{
							long x = string_to_int(alloc_temp->attr("num"));
							tmpsizes[x] = alloc_temp->attr("size");
							tmpnums.push_back(x);
							tmpmap[x] = k;
						}
					}
				}
				fo << "\tstruct crudasm_temp_node_t temp_a;\n";
				if(!tmpsizes.empty())
				{
					fo << "\tstruct crudasm_temp_node_t tmp[" << tmpsizes.size() << "];\n";
				}
				U1 numargs = insn->proto.numArgs;
				if(numargs != 0)
				{
					fo << "\tstruct crudasm_temp_node_t argtmp[" << (U2)(numargs) << "];\n";
				}
				for(size_t k = 0; k < tmpsizes.size(); ++k)
				{
					fo << "\ttmp[" << k << "].size = ";
					std::string sz = tmpsizes[tmpnums[k]];
					if(sz == "BIT")
						fo << "1/*BIT*/";
					else
					if(!sz.empty() && sz[0] == 'B')
					{
						fo << std::string(++sz.begin(), sz.end());
					}
					else
					{
						fo << "get_arg_size(context, " << sz << ")";
					}
					fo << ";\n";
				}
				for(U1 k = 0; k < numargs; ++k)
				{
					fo << "\targtmp[" << (U2)(k) << "].size = get_arg_size(context, " << (U2)(k) << ");\n";
				}
				
				XmlElement *stmts = e->tag("stmts");
				
				for(size_t k = 0; k < stmts->child.size(); ++k)
				{
					XmlElement *stmt = stmts->child[k];

					bool skip_first = false;
					if(*stmt->name == "asgn" || *stmt->name == "pop" || *stmt->name == "inport")
						skip_first = true;
					U8 args_read = 0;
					get_args_read(stmt, &args_read, skip_first);
					for(int m = 0; args_read != 0; ++m, args_read >>= 1)
					{
						if((args_read & 1) != 0)
						{
							fo << "\tif(crudasm_read_arg(context, " << m << ", argtmp + " << m << ") != 0)\n";
							fo << "\t\treturn 1;\n";
						}
					}
					
					// argtmp[] & tmp[] are now valid if applicable.
					if(*stmt->name == "asgn" || *stmt->name == "pop" || *stmt->name == "inport")
					{
						// 1st arg is modified. pop has stack as src, inport reads from port indicated by its 2nd argument.
						
						if(*stmt->name == "pop")
						{
							///fo << "\ttemp_a.size = 2 << context->icode.osz;\n";
							fo << "\ttemp_a.size = ";
							//begin
							std::string sz = stmt->child[0]->attr("size");
							if(sz == "BIT")
							{
								std::cerr << insn_name << " : attempt to pop to a BIT destination! FAIL." << std::endl;
								throw 1;
							}
							else
							if(!sz.empty() && sz[0] == 'B')
								fo << std::string(++sz.begin(), sz.end());
							else
							{
								fo << "get_arg_size(context, " << sz << ")";
							}
							//end
							fo << ";\n";

							fo << "\tif(do_pop(context, &temp_a) != 0)\n";
							fo << "\t\treturn 1;\n";
						}
						else
						{
							// Set 'temp_a' to src argument.
							fo << "\ttemp_a = ";
							out_expr(insn_name, fo, stmt->child[1]);
							fo << ";\n";
							
							if(*stmt->name == "inport")
							{
								// reads from port temp_a [which should have a size of 2 bytes],
								// storing the value back in temp_a, using the indicated size.
								fo << "\tif(do_inport(context, &temp_a, ";
								std::string sz = stmt->child[0]->attr("size");
								if(sz == "BIT")
								{
									std::cerr << insn_name << " : attempt to inport to a BIT destination! FAIL." << std::endl;
									throw 1;
								}
								else
								if(!sz.empty() && sz[0] == 'B')
									fo << std::string(++sz.begin(), sz.end());
								else
								{
									fo << "get_arg_size(context, " << sz << ")";
								}
								fo << ") != 0)\n";
								fo << "\t\treturn 1;\n";
							}
						}
						
						// Copy temp_a to destination here.
						XmlElement *dest = stmt->child[0];
						if(*dest->name == "temp")
						{
							long tempnum = string_to_int(dest->attr("num"));
							fo << "\ttmp[" << tmpmap[tempnum] << "] = temp_a;\n";
						}
						else
						if(*dest->name == "arg")
						{
							fo << "\tif(crudasm_write_arg(context, " << dest->attr("num") << ", &temp_a) != 0)\n";
							fo << "\t\treturn 1;\n";
						}
						else
						if(*dest->name == "extern")
						{
							// Recognize specific greg's (RAX..R15 & friends); sreg's; rflags (OF, SF, etc.).
							// If found, call corresponding write function.
							
							std::string nm = dest->attr("name");
							int val;
							if((val = getReg8(nm)) >= 0)
							{
								fo << "\tif(write_gr8_value(context, " << val << ", &temp_a))\n";
								fo << "\t\treturn 1;\n";
							}
							else
							if((val = getReg16(nm)) >= 0)
							{
								fo << "\tif(write_gr16_value(context, " << val << ", &temp_a))\n";
								fo << "\t\treturn 1;\n";
							}
							else
							if((val = getReg32(nm)) >= 0)
							{
								fo << "\tif(write_gr32_value(context, " << val << ", &temp_a))\n";
								fo << "\t\treturn 1;\n";
							}
							else
							if((val = getReg64(nm)) >= 0)
							{
								fo << "\tif(write_gr64_value(context, " << val << ", &temp_a))\n";
								fo << "\t\treturn 1;\n";
							}
							else
							if((val = getSegReg(nm)) >= 0)
							{
								fo << "\tif(write_sr_value(context, " << val << ", &temp_a))\n";
								fo << "\t\treturn 1;\n";
							}
							else
							if((val = getFlag(nm)) >= 0)
							{
								fo << "\tdo_write_rflag_value(context, " << val << ", &temp_a);\n";
							}
							else
							{
								fo << "\tdo_write_extern_" << nm << "(context, &temp_a);\n";
							}
						}
						else
						{
							throw __LINE__;
						}
					}
					else
					{
						fo << "\tif(special_do_" << *stmt->name << "(context";
						for(size_t hh = 0; hh < stmt->child.size(); ++hh)
						{
							fo << ", ";
							out_expr(insn_name, fo, stmt->child[hh]);
						}
						fo << ") != 0)\n";
						fo << "\t\treturn 1;\n";
					}
				}

				fo << "\treturn 0;  /* success */\n";
				fo << "}\n";
				fo << "\n";
			}
		}
	}
	
	fo << "static int crudasm_intel_execute(struct crudasm_intel_context_t *context)  {\n";
	fo << "\tint osz = 16 << context->icode.osz, asz = 16 << context->icode.asz, ssz = 16 << context->ssz;\n";
	fo << "\tswitch(crudasm_intel_encoding_table[context->icode.encoding].insn)  {\n";
	for(size_t i = 0; i < syntax.getInsnCount(); ++i)
	{
		IntelSyntaxInsn *insn = reader.syntax.getInsn(i);
		
		if(!insn->emulst.empty())
		{
			std::string insn_name = syntax.getSymbol(insn->proto.name);
			fo << "\t\tcase crudasm_intel_insn_" << insn_name << ":\n";
			
			for(std::list<XmlElement *>::iterator j = insn->emulst.begin(); j != insn->emulst.end(); ++j)
			{
				XmlElement *e = *j;
				fo << "\t\t\tif(1";
				if(e->attr("osz") != "any")
					fo << " && osz == " << e->attr("osz");
				if(e->attr("asz") != "any")
					fo << " && asz == " << e->attr("asz");
				if(e->attr("ssz") != "any")
					fo << " && ssz == " << e->attr("ssz");
				fo << ")\n";
				fo << "\t\t\t\treturn crudasm_intel_execute";
				if(e->attr("osz") != "any")
					fo << "_o" << e->attr("osz");
				if(e->attr("asz") != "any")
					fo << "_a" << e->attr("asz");
				if(e->attr("ssz") != "any")
					fo << "_s" << e->attr("ssz");
				fo << "__" << insn_name << "(context);\n";
			}
			
			fo << "\t\t\treturn 2;  /* internal error */\n";
		}
	}
	fo << "\t\tdefault:  break;\n";
	fo << "\t}\n";
	fo << "\treturn -1;  /* no semantics */\n";
	fo << "}\n";
	
	fo << "\n/*\n";
	for(std::set<std::string>::iterator i = internal_names.begin(); i != internal_names.end(); ++i)
		fo << *i << "\n";
	fo << "*/\n";
	
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
	
	int status;
	try
	{
		status = mkemu(reader);
	}
	catch(int c)
	{
		std::cout << "\nERRORS DETECTED !! Code = " << c << "\n" << std::endl;
		return 1;
	}
	return 0;
}


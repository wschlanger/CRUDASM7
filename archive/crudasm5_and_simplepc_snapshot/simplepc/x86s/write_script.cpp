// read_script.cpp
// Copyright (C) 2008 Willow Schlanger

#include "x86s_script.h"

#include "types.h"
#include "x86s_common.h"

#include <list>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>

namespace x86s
{

std::string get_prefix(std::string src)
{
	std::string dest;
	for(std::string::iterator i = src.begin(); i != src.end(); ++i)
		if(*i == '_')
			break;
		else
			dest += *i;
	return dest;
}

std::string char_to_string(char c)
{
	char s[2];
	s[0] = c;
	s[1] = '\0';
	return s;
}

void write_encoding_suffixes(std::stringstream &s, tmp_encoding_t &tmp, globals_t &globals)
{
	std::vector<std::string> prefix;
	std::vector<std::string> suffix;
	
	// First initialize the vector.
	for(base_option_list_t::iterator i = globals.elist.begin(); i != globals.elist.end(); ++i)
	{
		if(i->empty())
			throw __LINE__;
		std::string p = get_prefix(*i->begin());
		prefix.push_back(p);
		suffix.push_back(p + "_def");
	}
	
	for(std::set<std::string>::iterator i = tmp.suffix.begin(); i != tmp.suffix.end(); ++i)
	{
		SINT j = globals.elist.do_find(*i);
		if(j != -1)
		{
			// What about conflicting suffixes? We just use the last one given.
			suffix[j] = *i;
		}
		else
		{
			std::cout << "Can\'t find: \'" << *i << "\'!" << std::endl;
			throw __LINE__;
		}
	}
	
	// Now output the values.
	for(std::vector<std::string>::iterator i = suffix.begin(); i != suffix.end(); ++i)
	{
		if(i != suffix.begin())
			s << ", ";
		s << *i;
	}
}

void encoding_to_string(std::string &out, tmp_encoding_t &tmp, globals_t &globals)
{
	std::stringstream s;
	
	//s << std::hex << 0x1234 << std::dec;
	s << "{";
	s << "insn_" << tmp.insn << ", ";
	s << std::hex << "0x" << tmp.basecode << std::dec << ", ";
	if(tmp.nextbyte == 511)
		s << "511, ";
	else
		s << std::hex << "0x" << tmp.nextbyte << std::dec << ", ";
	s << "{";

	write_encoding_suffixes(s, tmp, globals);

	s << "},";

	s << "{";
	for(int i = 0; i < MAX_ARGS; ++i)
	{
		if(i != 0)
			s << ", ";

		s << "ARGTYPE(";
		s << "argtype_" << tmp.argtype[i];
		s << ", ";
		if(tmp.argtype_hi[i] == "0")
			s << "0";
		else
			s << "argtypehi_" << tmp.argtype_hi[i];
		s << ")";
	}

	s << "}, ";
	s << "{";
	for(int i = 0; i < MAX_ARGS; ++i)
	{
		if(i != 0)
			s << ", ";
		s << "ARGSIZE(";
		s << "argsize_" << tmp.argsize[i];
		s << ", argsize_hi_";
		s << tmp.argsize_hi[i];
		s << ")";
	}

	s << "}, ";
	s << "{";
	for(int i = 0; i < MAX_ARGS; ++i)
	{
		if(i != 0)
			s << ", ";
		s << std::hex << "0x" << tmp.argvalue[i] << std::dec;
	}
	s << "}";
	s << "}";
	
	out = s.str();
}

void write_out_encodings_h(globals_t &g)
{
	std::list<insn_t> &data = g.data;
	std::ofstream fo("out_encodings.h");
	bool need_comma = false;
	std::string s;
	fo << "// out_encodings.h  (note: this is an automatically generated file - do not edit!)" << std::endl;
	fo << "// " << COPYRIGHT << std::endl;
	fo << std::endl;
	for(std::list<insn_t>::iterator i = data.begin(); i != data.end(); ++i)
	{
		for(std::vector<tmp_encoding_t>::iterator j = i->encodings.begin(); j != i->encodings.end(); ++j)
		{
			if(need_comma)
				fo << "," << std::endl;
			need_comma = true;
			encoding_to_string(s, *j, g);
			fo << s;
		}
	}
	fo << "," << std::endl;
	fo << "{insn__count}" << std::endl;
	fo << std::endl;
}

void write_out_insn_enums_h(globals_t &g)
{
	std::list<insn_t> &data = g.data;
	std::ofstream fo("out_insn_enums.h");
	bool need_comma = false;
	std::string s;
	fo << "// out_insn_enums.h  (note: this is an automatically generated file - do not edit!)" << std::endl;
	fo << "// " << COPYRIGHT << std::endl;
	fo << std::endl;
	fo << "enum" << std::endl;
	fo << "{" << std::endl;
	int count = 0;
	for(std::list<insn_t>::iterator i = data.begin(); i != data.end(); ++i)
	{
		if(need_comma)
			fo << "," << std::endl;
		need_comma = true;
		fo << "\tinsn_" << i->insn_name;
		if(i->insn_name.empty())
			throw __LINE__;
		if(i->insn_name[0] != '_')
			++count;
	}
	fo << "," << std::endl;
	fo << "\tinsn__count," << std::endl;
	fo << "\tinsn__string_count = " << count << "\t// number of instructions whose name does not begin with a \'_\' character" << std::endl;
	fo << "};" << std::endl;
	fo << std::endl;
	fo << std::endl;
}

void write_out_insn_strings_h(globals_t &g)
{
	std::list<insn_t> &data = g.data;
	std::ofstream fo("out_insn_strings.h");
	bool need_comma = false;
	std::string s;
	fo << "// out_insn_strings.h  (note: this is an automatically generated file - do not edit!)" << std::endl;
	fo << "// " << COPYRIGHT << std::endl;
	fo << std::endl;
	for(std::list<insn_t>::iterator i = data.begin(); i != data.end(); ++i)
	{
		if(need_comma)
			fo << "," << std::endl;
		need_comma = true;
		if(i->insn_name.empty())
			throw __LINE__;
		if(i->insn_name[0] == '_')
			fo << "/* " << i->insn_name << "*/ 0";
		else
			fo << "\"" << i->insn_name << "\"";
	}
	fo << std::endl;
	fo << std::endl;
}

void write_out_insn_search_table_h(globals_t &g)
{
	std::ofstream fo("out_insn_search_table.h");
	bool need_comma = false;
	std::string s;
	fo << "// out_insn_search_table.h  (note: this is an automatically generated file - do not edit!)" << std::endl;
	fo << "// " << COPYRIGHT << std::endl;
	fo << std::endl;
	fo << "// out_insn_enums.h defines the enum \'insn__string_count\' which specifies the number of elements listed here." << std::endl;
	fo << std::endl;
	for(std::set<std::string>::iterator i = g.insns.begin(); i != g.insns.end(); ++i)
	{
		if(i->empty())
			throw __LINE__;
		if((*i)[0] == '_')
			continue;
		if(need_comma)
			fo << "," << std::endl;
		need_comma = true;
		fo << "insn_" << *i;
	}
	fo << std::endl;
	fo << std::endl;
}

void write_out_insns_h(globals_t &g)
{
	std::list<insn_t> &data = g.data;
	std::ofstream fo("out_insns.h");
	bool need_comma = false;
	std::string s;
	fo << "// out_insns.h  (note: this is an automatically generated file - do not edit!)" << std::endl;
	fo << "// " << COPYRIGHT << std::endl;
	fo << std::endl;
	UINT index = 0;
	UINT size;
	for(std::list<insn_t>::iterator i = data.begin(); i != data.end(); ++i)
	{
		size = i->encodings.size();
		if(need_comma)
			fo << "," << std::endl;
		need_comma = true;
		
		insn_t &insn = *i;
		
		fo << "{";
		fo << index << ", " << size << ", {";
		// --- begin write insn suffixes ---
		{
			std::vector<std::string> prefix;
			std::vector<std::string> suffix;
			
			// First initialize the vector.
			globals_t &globals = g;
			for(base_option_list_t::iterator i = globals.ilist.begin(); i != globals.ilist.end(); ++i)
			{
				if(i->empty())
					throw __LINE__;
				std::string p = get_prefix(*i->begin());
				prefix.push_back(p);
				suffix.push_back(p + "_def");
			}
			
			for(std::set<std::string>::iterator i = insn.insn_suffix.begin(); i != insn.insn_suffix.end(); ++i)
			{
				SINT j = globals.ilist.do_find(*i);
				if(j != -1)
				{
					// What about conflicting suffixes? We just use the last one given.
					suffix[j] = *i;
				}
				else
				{
					std::cout << "Can\'t find: \'" << *i << "\'!" << std::endl;
					throw __LINE__;
				}
			}
			
			// Now output the values.
			for(std::vector<std::string>::iterator i = suffix.begin(); i != suffix.end(); ++i)
			{
				if(i != suffix.begin())
					fo << ", ";
				fo << *i;
			}
		}
		// --- end write insn suffixes ---
		fo << "}";
		fo << "}";
				
		index += size;
	}
	fo << std::endl;
	fo << std::endl;
}

void write_out_disassemble_h(globals_t &g)
{
	std::ofstream fo("out_disassemble.h");
	fo << "// out_disassemble.h  (note: this is an automatically generated file - do not edit!)" << std::endl;
	fo << "// " << COPYRIGHT << std::endl;
	fo << std::endl;

	for(std::list<insn_t>::iterator i = g.data.begin(); i != g.data.end(); ++i)
	{
		if(i->disasm_code.empty())
			continue;
		fo << "case insn_" << i->insn_name << ":" << std::endl;
		for(std::list<std::string>::iterator j = i->disasm_code.begin(); j != i->disasm_code.end(); ++j)
		{
			fo << *j << std::endl;
		}
		fo << "\tbreak;" << std::endl;
	}
	
	fo << std::endl;
	fo << std::endl;
}

void write_out_semantics_h(globals_t &g)
{
	std::ofstream fo("out_semantics.h");
	fo << "// out_semantics.h  (note: this is an automatically generated file - do not edit!)" << std::endl;
	fo << "// " << COPYRIGHT << std::endl;
	fo << std::endl;
	
	fo << "tnode x86_tcode_nodes[] = {" << std::endl;
	
	std::string insn;
	Semantics sem(fo);
	try
	{
	
		sem.maxsize = 0;
		sem.max = "";
		sem.lastinsn = "";
		for(std::list<insn_t>::iterator i = g.data.begin(); i != g.data.end(); ++i)
		{
			//sem.insnindex[i->insn_name] = 0xffffffff;
			sem.insntemps[i->insn_name] = 0;	// # of temporary variables used by the insn
			insn = i->insn_name;
			sem.begin_insn(i->insn_name.c_str());
			//fo << "begin_insn(\"" << i->insn_name << "\", insn_" << i->insn_name << ");" << std::endl;
			//if(i->disasm_code.empty())
			//	continue;
			//fo << "case insn_" << i->insn_name << ":" << std::endl;
			for(std::list<std::string>::iterator j = i->semantics.begin(); j != i->semantics.end(); ++j)
				sem.parse(j->c_str());
			sem.end_insn();
			//	fo << "parse(\"" << *j << "\");" << std::endl;
			//fo << "end_insn();" << std::endl;
		}
		
		std::ofstream ff("out_semantics_h.h");
		ff << "// out_semantics_h.h  (note: this is an automatically generated file - do not edit!)" << std::endl;
		ff << "// " << COPYRIGHT << std::endl;
		ff << std::endl;
		ff << "#define X86S_SEMANTICS_MAX_SIZE " << sem.maxsize << " /* " << sem.max << " */" << std::endl;
		ff << std::endl;
		ff << std::endl;
	}
	catch(const char *s)
	{
		std::cout << "Error generating out_semantics.h (" << insn << "): " << s << std::endl;
	}
	fo << std::endl;
	fo << "// end of table" << std::endl;
	fo << "{nt_void}" << std::endl;
	fo << "};" << std::endl;
	
	for(std::list<insn_t>::iterator i = g.data.begin(); i != g.data.end(); ++i)
	{
		if(sem.insnindex.find(i->insn_name) == sem.insnindex.end())
			continue;
		fo << std::endl;
		fo << "U4 sem_insn_" << i->insn_name << "[] = {" << std::endl;
		fo << "\t" << sem.insnsize[i->insn_name] << ",\t// total size" << std::endl;
		fo << "\t" << sem.insnstart[i->insn_name] << ",\t// start index" << std::endl;
		for(std::vector<U4>::iterator j = sem.insnindex[i->insn_name].begin(); j != sem.insnindex[i->insn_name].end(); ++j)
		{
			fo << std::hex << "\t0x" << *j << std::dec << "," << std::endl;
		}
		fo << "\t0xffffffff\t// end" << std::endl;
		fo << "};" << std::endl;
	}

	fo << std::endl;
	fo << "tcode_element x86_tcode_table[] =" << std::endl;
	fo << "{" << std::endl;

	for(std::list<insn_t>::iterator i = g.data.begin(); i != g.data.end(); ++i)
	{
		//fo << "\t{0x" << std::hex << sem.insnindex[i->insn_name] << std::dec;
		if(sem.insnindex.find(i->insn_name) == sem.insnindex.end())
			fo << "\t{ 0";
		else
			fo << "\t{ sem_insn_" << i->insn_name << std::dec;
		fo << ", " << sem.insntemps[i->insn_name];
		int numargs = -1;
		
		for(std::vector<tmp_encoding_t>::iterator j = i->encodings.begin(); j != i->encodings.end(); ++j)
		{
			int n;
			for(n = 0; n < MAX_ARGS; ++n)
				if(j->argtype[n] == "void")
					break;
			if(numargs == -1)
				numargs = n;
			else
			if(numargs != n)
			{
				std::cout << "Error: instruction " << insn << " must have all encodings with same number of arguments." << std::endl;
				return;
			}
		}
		if(numargs == -1)
		{
			std::cout << "Error: instruction " << insn << " must have at least one encoding." << std::endl;
			return;
		}
		
		fo << ", " << numargs;
		fo << "},\t// " << i->insn_name << std::endl;
	}
	
	fo << "\t{0, 0, 0}\t// end of table" << std::endl;	
	fo << "};" << std::endl;
	fo << std::endl;
}

void write_files(globals_t &g)
{
	write_out_encodings_h(g);
	write_out_insn_enums_h(g);
	write_out_insn_strings_h(g);
	write_out_insn_search_table_h(g);
	write_out_insns_h(g);
	write_out_disassemble_h(g);
	write_out_semantics_h(g);
}

}	// namespace x86s


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

#if 0
// This computes ceil(log2(s)) and returns static_cast<UINT>(-1) on error.
UINT logsize(UINT s)
{
	if(s == 0)
		return 1;
	for(UINT x = 1, log = 1; x != 0; x += x, ++log)
	{
		if(x > s)
			return log - 1;
	}
	return static_cast<UINT>(-1);
}
#endif

struct tokenize
{
	std::vector<std::string> tokens;
	
	tokenize(std::string s, char delim)
	{
		std::string tmp;
		
		for(std::string::iterator i = s.begin(); i != s.end(); ++i)
		{
			if(*i == delim)
			{
				if(!tmp.empty())
					tokens.push_back(tmp);
				tmp.clear();
			}
			else
				tmp += *i;
		}
		
		if(!tmp.empty())
			tokens.push_back(tmp);
	}
};

std::string::iterator skip_space(std::string::iterator begin, std::string::iterator end)
{
	while(begin != end)
	{
		if(!std::isspace(*begin))
			break;
		++begin;
	}
	return begin;
}

std::string::iterator getword(std::string &out, std::string::iterator begin, std::string::iterator end, std::string delim="")
{
	out.clear();
	begin = skip_space(begin, end);
	std::string::iterator realbegin = begin;
	while(begin != end)
	{
		bool found = false;
		for(std::string::iterator i = delim.begin(); i != delim.end(); ++i)
		{
			if(*i == *begin)
			{
				found = true;
				break;
			}
		}
		if(found || std::isspace(*begin))
		{
			out = std::string(realbegin, begin);
			return begin;
		}
		++begin;
	}
	out = std::string(realbegin, begin);	// it's one big word!
	return begin;
}

void next_insn(globals_t &g)
{
	if(!g.insn_name.empty())
	{
		if(g.encodings.empty())
		{
			std::cout << "Error: " << g.filename << " line " << g.linenum << ": no encodings for instruction \'" << g.insn_name << "\'" << std::endl;
			throw __LINE__;
		}
		g.data.push_back(insn_t());
		g.data.back().insn_name = g.insn_name;
		g.data.back().encodings.clear();
		g.data.back().disasm_code.clear();
		g.data.back().semantics.clear();
		g.data.back().insn_suffix.clear();
		for(std::vector<tmp_encoding_t>::iterator i = g.encodings.begin(); i != g.encodings.end(); ++i)
			g.data.back().encodings.push_back(*i);
		for(std::list<std::string>::iterator i = g.disasm_code.begin(); i != g.disasm_code.end(); ++i)
			g.data.back().disasm_code.push_back(*i);
		for(std::list<std::string>::iterator i = g.semantics.begin(); i != g.semantics.end(); ++i)
			g.data.back().semantics.push_back(*i);
		for(std::set<std::string>::iterator i = g.insn_suffix.begin(); i != g.insn_suffix.end(); ++i)
			g.data.back().insn_suffix.insert(*i);

#if 0
		// ---
		std::cout << "// $ " << g.insn_name;
		if(!g.insn_suffix.empty())
		{
			std::cout << " ! ";
			for(std::set<std::string>::iterator i = g.insn_suffix.begin(); i != g.insn_suffix.end(); ++i)
			{
				if(i != g.insn_suffix.begin())
					std::cout << ",";
				std::cout << *i;
			}
		}
		std::cout << std::endl;
		std::cout << "#define insn_" << g.insn_name << " 0" << std::endl;
		std::string s;
		for(std::vector<tmp_encoding_t>::iterator i = g.encodings.begin(); i != g.encodings.end(); ++i)
		{
			encoding_to_string(s, *i, g);
			std::cout << std::endl;
			std::cout << s << "," << std::endl;
		}
		std::cout << std::endl;
		// ---
#endif

		g.insn_name.clear();
	}
}

void parse_insn(globals_t &g)
{
	++g.begin;		// skip '$'
	g.begin = getword(g.insn_name, g.begin, g.line.end(), "!#");
	
	if(g.insn_name.empty() || g.insn_name == "_")
	{
		std::cout << "Error: " << g.filename << " line " << g.linenum << ": valid instruction name expected" << std::endl;
		throw __LINE__;
	}
	
	if(g.insns.find(g.insn_name) != g.insns.end())
	{
		std::cout << "Error: " << g.filename << " line " << g.linenum << ": duplicate instruction name \'" << g.insn_name << "\'" << std::endl;
		throw __LINE__;
	}
	g.insns.insert(g.insn_name);
	
	g.begin = skip_space(g.begin, g.line.end());
	
	g.insn_suffix.clear();
	g.encodings.clear();
	g.disasm_code.clear();
	g.semantics.clear();
	
	if(g.begin != g.line.end())
	{
		if(*g.begin == '!')
		{
			++g.begin;			// skip '!'
			std::string tmp;
			g.begin = skip_space(g.begin, g.line.end());
			g.begin = getword(tmp, g.begin,	 g.line.end(), "#");
			
			tokenize t(tmp, ',');
			if(!t.tokens.empty())
			{
				for(std::vector<std::string>::iterator i = t.tokens.begin(); i != t.tokens.end(); ++i)
				{
					SINT j = g.ilist.do_find(*i);
					if(j == -1)
					{
						std::cout << "Error: " << g.filename << " line " << g.linenum << ": unrecognized instruction suffix: " << *i << std::endl;
						throw __LINE__;
					}
					g.insn_suffix.insert(*i);
				}
			}
		}
	}
	
	// now skip spaces and make sure there's nothing left, or possibly '#'.
	g.begin = skip_space(g.begin, g.line.end());
	if(g.begin == g.line.end())
		return;
	if(*g.begin == '#')
		return;
	
	std::cout << "Error: " << g.filename << " line " << g.linenum << ": expected end of line" << std::endl;
	throw __LINE__;
}

long gethexvalue(const char *s)
{
	long x;
	char *endptr;
	x = std::strtol(s, &endptr, 16);
	if(*endptr != '\0')
		return -1;
	return x;
}

void parse_encoding(globals_t &g)
{
	if(g.insn_name.empty())
	{
		std::cout << "Error: " << g.filename << " line " << g.linenum << ": instruction required before encodings" << std::endl;
		throw __LINE__;
	}

	tmp_encoding_t tmp;
	tmp.suffix.clear();
	//tmp.suffix.insert("fx_none_lockable");
	tmp.insn = g.insn_name;

	tmp.basecode = 0;
	tmp.nextbyte = 511;
	for(int i = 0; i < MAX_ARGS; ++i)
	{
		tmp.argtype[i] = "void";
		tmp.argtype_hi[i] = "0";
		tmp.argsize[i] = "16";
		tmp.argsize_hi[i] = "none";
		tmp.argvalue[i] = 0xff;
	}
	
	// now parse the line.
	//std::string::iterator getword(std::string &out, std::string::iterator begin, std::string::iterator end, std::string delim="")
	std::string s;

	++g.begin;				// skip ':'

	g.begin = getword(s, g.begin, g.line.end());
	
	bool op66 = false;
	int fx = 0;		// 0=none, 2=f2, 3=f3
	
	while(s == "66" || s == "f2" || s == "f3")
	{
		if(s == "66")
		{
			op66 = true;
			g.begin = getword(s, g.begin, g.line.end());
		}
		else
		if(s == "f2")
		{
			if(fx == 3)
			{
				std::cout << "Error: " << g.filename << " line " << g.linenum << ": f2 and f3 can not both be used!" << std::endl;
				throw __LINE__;
			}
			fx = 2;
			g.begin = getword(s, g.begin, g.line.end());
		}
		if(s == "f3")
		{
			if(fx == 2)
			{
				std::cout << "Error: " << g.filename << " line " << g.linenum << ": f2 and f3 can not both be used!" << std::endl;
				throw __LINE__;
			}
			fx = 3;
			g.begin = getword(s, g.begin, g.line.end());
		}
	}
	
	if(s == "0f")
	{
		tmp.basecode += 0x100;
		g.begin = getword(s, g.begin, g.line.end());
	}

	if(s == "0f")
	{
		tmp.basecode += 0x100;
		g.begin = getword(s, g.begin, g.line.end());
	}

	long x = gethexvalue(s.c_str());
	if(x < 0 || x > 0xff)
	{
		std::cout << "Error: " << g.filename << " line " << g.linenum << ": bad opcode byte \'" << s << "\'" << std::endl;
		throw __LINE__;
	}
	tmp.basecode += (U4)x;

	g.begin = getword(s, g.begin, g.line.end());
	
	bool has_colon = false;
	for(std::string::iterator i = s.begin(); i != s.end(); ++i)
	{
		if(*i == ':')
		{
			has_colon = true;
			break;
		}
	}
	
	if(!s.empty())
	{
		if(!has_colon && s[0] != '/')
		{
			x = gethexvalue(s.c_str());
			if(x >= 0 && x < 0x100)
			{
				tmp.nextbyte = (U1)x;
			
				g.begin = getword(s, g.begin, g.line.end());
			}
		}

		if(s[0] == '/')
		{
			if(s == "/0")
				tmp.suffix.insert("ro_0");
			else
			if(s == "/1")
				tmp.suffix.insert("ro_1");
			else
			if(s == "/2")
				tmp.suffix.insert("ro_2");
			else
			if(s == "/3")
				tmp.suffix.insert("ro_3");
			else
			if(s == "/4")
				tmp.suffix.insert("ro_4");
			else
			if(s == "/5")
				tmp.suffix.insert("ro_5");
			else
			if(s == "/6")
				tmp.suffix.insert("ro_6");
			else
			if(s == "/7")
				tmp.suffix.insert("ro_7");
			else
			if(s == "/r")
				tmp.suffix.insert("ro_r");
			else
			if(s == "/z")
				tmp.suffix.insert("ro_z");
			else
			{
				std::cout << "Error: " << g.filename << " line " << g.linenum << ": unrecognized reg/op specifier \'" << s << "\'" << std::endl;
				throw __LINE__;
			}
			g.begin = getword(s, g.begin, g.line.end());
		}
	}
	
	// get arguments.
	if(s.empty())
	{
		std::cout << "Error: " << g.filename << " line " << g.linenum << ": argument list expected - use void for no arguments" << std::endl;
		throw __LINE__;
	}
	
	if(s != "void")
	{
		tokenize t(s, ',');
		int argnum = 0;
		for(std::vector<std::string>::iterator i = t.tokens.begin(); i != t.tokens.end(); ++i, ++argnum)
		{
			if(argnum >= MAX_ARGS)
			{
				std::cout << "Error: " << g.filename << " line " << g.linenum << ": too many arguments (max " << MAX_ARGS << ")" << std::endl;
				throw __LINE__;
			}
			tokenize tsv(*i, ':');
			if(tsv.tokens.size() < 2 || tsv.tokens.size() > 3)
			{
				std::cout << "Error: " << g.filename << " line " << g.linenum << ": bad argument \'" << *i << "\'" << std::endl;
				throw __LINE__;
			}

			tmp.argtype[argnum] = get_prefix(tsv.tokens[0]);
			tmp.argtype_hi[argnum] = tsv.tokens[0];

			if(tsv.tokens.size() == 3)
			{
				long x = gethexvalue(tsv.tokens[2].c_str());
				if(x == -1)
				{
					std::cout << "Error: " << g.filename << " line " << g.linenum << ": bad argument \'" << *i << "\'" << std::endl;
					throw __LINE__;
				}
				tmp.argvalue[argnum] = (U2)x;
			}
			tokenize sz(tsv.tokens[1], '.');
			if(sz.tokens.empty() || sz.tokens.size() > 2)
			{
				std::cout << "Error: " << g.filename << " line " << g.linenum << ": bad argument \'" << *i << "\'" << std::endl;
				throw __LINE__;
			}
			if(sz.tokens[0] == "0")
				tmp.argsize[argnum] = "16";		// size not applicable -- use default
			else
				tmp.argsize[argnum] = sz.tokens[0];
			if(sz.tokens.size() == 2)
				tmp.argsize_hi[argnum] = sz.tokens[1];
		}
	}
	
	g.begin = getword(s, g.begin, g.line.end());
	if(!s.empty())
	{
		if(s[0] == '!')
		{
			g.begin = getword(s, g.begin, g.line.end());
			
			if(!s.empty())
			{
				if(s[0] == '#')
					s.clear();
			}
			
			tokenize su(s, ',');
			for(std::vector<std::string>::iterator i = su.tokens.begin(); i != su.tokens.end(); ++i)
			{
				// See if *i is a valid attribute.
				SINT j = g.elist.do_find(*i);
				if(j == -1)
				{
					std::cout << "Error: " << g.filename << " line " << g.linenum << ": bad encoding suffix \'" << *i << "\'" << std::endl;
					throw __LINE__;
				}
				// Add suffix.
				tmp.suffix.insert(*i);
			}
			
			g.begin = getword(s, g.begin, g.line.end());
		}
		if(!s.empty())
		{
			std::cout << "Error: " << g.filename << " line " << g.linenum << ": end-of-line expected" << std::endl;
			throw __LINE__;
		}
	}
	
	if(op66)
		tmp.suffix.insert("op66_op66");

	if(fx == 2)
		tmp.suffix.insert("fx_f2");
	else
	if(fx == 3)
		tmp.suffix.insert("fx_f3");
	
	// add encoding to vector.
	g.encodings.push_back(tmp);
}

// out: g.type =
//    '$' if it's an instruction line
//    ':' if it's an encoding line
//    '#' if the line is just a comment or is empty
//    '.' otherwise
void parse_line(globals_t &g)
{
	g.begin = g.line.begin();
	g.begin = skip_space(g.begin, g.line.end());
	g.type = '#';
	
	//std::cout << "[" << std::string(g.begin, g.line.end()) << "]" << std::endl;
	
	if(g.begin == g.line.end())
		return;

	g.type = *g.begin;

	if(g.type == '$')
	{
		next_insn(g);
		parse_insn(g);
	}
	else
	if(g.type == ':')
	{
		parse_encoding(g);
	}
	else
	if(g.type != '#')
		g.type = '.';
}

}	// namespace x86s

using namespace x86s;

// --- main program ---

int main()
{
	try
	{
		std::string infilename = "in_script.txt";
		globals_t g;
		g.filename = infilename;
		g.linenum = 0;
		
		std::ifstream inf(infilename.c_str());

		if(inf == NULL)
		{
			std::cout << infilename << ": unable to open file for reading" << std::endl;
			throw __LINE__;
		}
		
		while(getline(inf, g.line) != NULL)
		{
			++g.linenum;
			//std::cout << "<<" << g.line << ">>" << std::endl;
			parse_line(g);
			// fixme -- add support for g.type == '.' -- semantics and disassembly special case code.
			if(g.type == '.')
			{
				if(g.insn_name.empty())
				{
					std::cout << infilename << " line " << g.linenum << ": code has no associated instruction!" << std::endl;
					throw __LINE__;
				}
				if(g.encodings.empty())
					g.disasm_code.push_back(g.line);
				else
					g.semantics.push_back(g.line);
			}
		}
		next_insn(g);
		
		write_files(g);
	}
	catch(int line)
	{
		std::cout << "Error: line " << line << std::endl;
		return 1;
	}
	return 0;
}

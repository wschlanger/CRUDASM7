// write_semantics.cpp
// Copyright (C) 2008 Willow Schlanger

#include <string>
#include <set>
#include <utility>
#include <iostream>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cstdio>

#include "types.h"
#include "x86s_common.h"

#include "x86s_script.h"

#if 0
namespace x86s
{

#include "out_insn_enums.h"

// This is terminated with an element with an insn number of insn__count
encoding_t encodings[] =
{
#include "out_encodings.h"
};

int search_table[insn__string_count] =
{
#include "out_insn_search_table.h"
};

const char *insn_strings[insn__count] =
{
#include "out_insn_strings.h"
};

insn_def_t insns[insn__count] =
{
#include "out_insns.h"
};

U4 decoder_table[] =
{
#include "out_decoder_table.h"
};

}	// namespace x86s
#endif

//using namespace x86s;
namespace x86s
{

void Semantics::begin_insn(const char *name)
{
	curinsn = name;
	temps.clear();
	tempcount = 0;
}

void Semantics::add(std::string s)
{
	if(s.empty())
		return;
	if(s == "\n")
	{
		if(!end_ok)
		{
			std::cout << "Unexpected end of line -- needs to end with a semicolon." << std::endl;
			throw curinsn;
		}
	}
	else
	if(std::isdigit(s[0]) || s[0] == '+' || s[0] == '-')
	{
		stk.back()->operands.push_back(new tmp_t());
		stk.back()->operands.back()->op = s;
	}
	else
	if(s == "(")
	{
		stk.push_back(stk.back()->operands.back());
	}
	else
	if(s == ")")
	{
		if(stk.size() == 1)
		{
			// never kill the root note.
			std::cout << "parse error: misplaced \')\')." << std::endl;
			throw curinsn;
		}
		stk.pop_back();
	}
	else
	if(s == ",")
	{
		// this we can safely ignore.
	}
	else
	if(s == ";")
	{
		// endok is already set.
	}
	else	// assume identifier
	{
		stk.back()->operands.push_back(new tmp_t());
		stk.back()->operands.back()->op = s;
	}
}

void Semantics::parse(const char *s)
{
	std::string st = s;
	std::string::iterator i = st.begin();
	while(i != st.end())
	{
		if(std::isspace(*i))
			++i;
		else
		if(*i == '/')
		{
			++i;
			if(i == st.end())
			{
				std::cout << "Expected: \'/'." << std::endl;
				throw curinsn;
			}
			if(*i != '/')
			{
				std::cout << "Expected: \'/' Got \'" << *i << "\'." << std::endl;
				throw curinsn;
			}
			++i;
		}
		else
		if(std::isdigit(*i) || *i == '+' || *i == '-')
		{
			end_ok = false;
			std::string::iterator j = i;
			if(*i == '+')
				++i;
			else
			if(*i == '-')
				++i;
			if(i == st.end())
			{
				std::cout << "Expected: number." << std::endl;
				throw curinsn;
			}
			while(i != st.end())
			{
				if(!std::isdigit(*i))
					break;
				++i;
			}
			add(std::string(j, i));
		}
		else
		if(*i == '(')
		{
			end_ok = false;
			++i;
			add("(");
		}
		else
		if(*i == ')')
		{
			end_ok = false;
			// real work gets done here.
			++i;
			add(")");
		}
		else
		if(*i == ',')
		{
			end_ok = false;
			// real work gets done here.
			++i;
			add(",");
		}
		else
		if(std::isalnum(*i) || *i == '_' || *i == '$')
		{
			end_ok = false;
			std::string::iterator j = i;
			while(i != st.end())
			{
				if(std::isalnum(*i) || *i == '_' || *i == '$')
				{
					++i;
				}
				else
					break;
			}
			add(std::string(j, i));
		}
		else
		if(*i == ';')
		{
			end_ok = true;
			add(";");
			for(;;)
			{
				if(i == st.end())
					break;
				++i;
				if(i == st.end())
					break;
				if(*i == '/')
				{
					++i;
					if(i == st.end())
					{
						std::cout << "[3] Illegal character: \'" << '/' << "\'." << std::endl;
						throw curinsn;
					}
					if(*i != '/')
					{
						std::cout << "Expected: \'" << '/' << "\'." << std::endl;
						throw curinsn;
					}
					++i;
					goto end;
				}
				if(!std::isspace(*i))
				{
					std::cout << "[2] Illegal character: \'" << *i << "\'." << std::endl;
					throw curinsn;
				}
			}
		}
		else
		{
			std::cout << "[1] Illegal character: \'" << *i << "\'." << std::endl;
			throw curinsn;
		}
	}
end:
	add("\n");

	// We now have a parse tree for this encoding.
	// Convert it to standard (numeric) form - complete with begin's and end's, temp's, etc.
	
	if(stk.size() != 1)
	{
		std::cout << "Bad expression - make sure ( )\'s are matched." << std::endl;
		throw curinsn;
	}
	if(stk.back()->operands.size() != 1)
	{
		std::cout << "Missing asgn() expression." << std::endl;
		throw curinsn;
	}
	if(stk.back()->operands.back()->op != "asgn")
	{
		std::cout << "Missing asgn() expression." << std::endl;
		throw curinsn;
	}
	
	//insnindex[std::string(curinsn)] = curindex;

	U4 oldindex = curindex;

	translate(stk.back()->operands.back());

	bool same = (curinsn == lastinsn);
	if(curinsn != lastinsn)
	{
		lastinsn = curinsn;
		insnstart[lastinsn] = oldindex;
		os << std::endl;
		os << "// " << curinsn << std::endl;
	}

	emit(stk.back()->operands.back());
	
	if(curindex == oldindex)
	{
		std::cout << "No code generated." << std::endl;
		throw curinsn;
	}
	
	unsigned size = curindex - oldindex;
	
	if(same)
		size += lastsize;
	lastsize = size;
	
	insnsize[std::string(curinsn)] = size;
	
	if(size > maxsize)
	{
		maxsize = size;
		max = curinsn;
	}
	
	insnindex[std::string(curinsn)].push_back(curindex - 1);
	
	clear(true);
}

U8 Semantics::getnum(const char *s)
{
	char *endptr;
	long value = std::strtol(s, &endptr, 0);
	if(*endptr != '\0')
	{
		std::cout << "Invalid numeric \'" << s << "\'." << std::endl;
		throw curinsn;
	}
	return (U8)(S8)(S4)value;
}

std::string int_to_string(unsigned x)
{
	char buf[1024];
	std::sprintf(buf, "%d", x);
	return std::string(buf);
}

std::string xget_prefix(std::string s)
{
	std::string::iterator i = s.begin();
	while(i != s.end())
	{
		if(*i == '$')
			break;
		++i;
	}
	return std::string(s.begin(), i);
}

std::string xget_suffix(std::string s)
{
	std::string::iterator i = s.begin();
	while(i != s.end())
	{
		if(*i == '$')
		{
			++i;
			return std::string(i, s.end());
		}
		++i;
	}
	return "";
}

void Semantics::emit(tmp_t *node)
{
	for(std::vector<struct tmp_t *>::iterator i = node->operands.begin(); i != node->operands.end(); ++i)
		emit(*i);
	if(!node->needs_a_node())
		return;
	
	if(node->is_numeric())
	{
		U8 value = getnum(node->op.c_str());
		os << "{nt_literal, ns_void, {" << std::hex << ((U4)value) << ", " << (value >> 32) << ", 0" << std::dec;
		os << "}},\t// " << curindex << std::endl;
		node->size = "void";
		node->where = curindex;
		++curindex;
		return;
	}
	
	std::string type = xget_prefix(node->op);
	std::string size = xget_suffix(node->op);
	if(size.empty())
		size = "void";
	
	os << "{nt_" << type << ", ns_" << size << ", {";
	int count = 0;
	for(std::vector<struct tmp_t *>::iterator i = node->operands.begin(); i != node->operands.end(); ++i)
	{
		if(count != 0)
			os << ", ";
		++count;
		
		// Write operand here.
		if((*i)->needs_a_node())
			os << (*i)->where;
		else
		{
			os << "no_" << (*i)->op << "|0x80000000";
		}
	}
	for(; count < 3; ++count)
	{
		if(count != 0)
			os << ", ";
		//os << "nt_void";
		os << "no_void|0x80000000";
	}
	os << "}},\t// " << curindex << std::endl;

	// Do this last.
	node->size = size;
	node->where = curindex;
	++curindex;
}

void Semantics::translate(tmp_t *node)
{
	for(std::vector<struct tmp_t *>::iterator i = node->operands.begin(); i != node->operands.end(); ++i)
		translate(*i);
	if(!node->needs_a_node())
		return;
	
	if(node->is_numeric())
	{
		/*U8 value = getnum(node->op.c_str());
		os << "{nt_literal, ns_void, {" << std::hex << ((U4)value) << ", " << (value >> 32) << ", 0}}," << std::dec << std::endl;
		node->size = "void";
		node->where = curindex;
		++curindex;*/
		return;
	}
	
	// if this is an asgn(tmp(name), y) node then look it up. if not found, add it.
	if(node->op == "asgn" && node->operands.size() >= 2)
	{
		if(node->operands.front()->op == "tmp")
		{
			if(node->operands.front()->operands.size() != 1)
			{
				std::cout << "expected syntax: asgn(tmp(name), ...);" << std::endl;
				throw curinsn;
			}
			std::string name = node->operands.front()->operands.front()->op;
			if(!std::isdigit(name[0]))
			{
				std::map<std::string, unsigned>::iterator i = temps.find(name);
				unsigned x;
				if(i == temps.end())
				{
					temps[name] = tempcount;
					x = tempcount;
					++tempcount;
					++insntemps[std::string(curinsn)];
				}
				else
					x = i->second;
				node->operands.front()->operands.front()->op = int_to_string(x);
			}
		}
	}
	else
	if(node->op == "tmp")
	{
		if(node->operands.size() != 1)
		{
			std::cout << "expected syntax: tmp(name)" << std::endl;
			throw curinsn;
		}
		std::string name = node->operands.front()->op;
		if(!std::isdigit(name[0]))
		{
			std::map<std::string, unsigned>::iterator i = temps.find(name);
			unsigned x;
			if(i == temps.end())
			{
				temps[name] = tempcount;
				x = tempcount;
				++tempcount;
				++insntemps[std::string(curinsn)];
			}
			else
				x = i->second;
			node->operands.front()->op = int_to_string(x);
		}
	}

	// Do this last.
	//++curindex;
}

void Semantics::end_insn()
{
}

Semantics::Semantics(std::ostream &os_t) :
	os(os_t)
{
	curindex = 0;
	stk.push_back(new tmp_t());
	stk.back()->op = "root";
	add_size("byte");
	add_size("word");
	add_size("dword");
	add_size("qword");
	add_size("oword");
	add_size("osz");
	add_size("asz");
	add_size("osz_times_2");
}

Semantics::~Semantics()
{
	clear(false);
}

}	// namespace x86s

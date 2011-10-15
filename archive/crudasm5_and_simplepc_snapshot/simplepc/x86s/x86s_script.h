// x86s_script.h
// Copyright (C) 2008 Willow Schlanger

#ifndef l_x86s__x86s_script_h__included
#define l_x86s__x86s_script_h__included

#define COPYRIGHT "Copyright (C) 2008 Willow Schlanger"

#include "x86s_common.h"
#include "types.h"
#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <utility>
#include <iostream>
#include <cctype>
#include <cstdlib>

namespace x86s
{

std::string get_prefix(std::string src);
std::string char_to_string(char c);

typedef std::set<std::string> option_t;
typedef std::list<option_t> base_option_list_t;

struct argtype_list_t :
	public option_t
{
	argtype_list_t()
	{
		insert("void");
		insert("reg");
		insert("mem");
		insert("imm");
		if(size() != argtype__end)
			throw __LINE__;
	}
};

struct argsize_list_t :
	public option_t
{
	argsize_list_t()
	{
		insert("16");
		insert("32");
		insert("64");
		insert("128");
		insert("8");
		insert("24");
		insert("512b");
		insert("osz");
		insert("osz_old");
		insert("osz_new");
		insert("osz_32");
		insert("osz_ptr");
		insert("osz_24");
		insert("osz_asz");
		if(size() != argsize__end)
			throw __LINE__;
	}
};

struct argsize_hi_list_t :
	public option_t
{
	argsize_hi_list_t()
	{
		insert("none");
		insert("16");
		insert("32");
		insert("64");
		insert("24");
		insert("osz");
		insert("osz_24");
		insert("osz_old");
		if(size() != argsize_hi__end)
			throw __LINE__;
	}
};

struct option_list_t :
	public base_option_list_t
{
	SINT do_find(std::string s)
	{
		base_option_list_t::iterator a;
		option_t::iterator b;
		SINT j = 0;
		for(a = begin(); a != end(); ++a, ++j)
		{
			b = a->find(s);
			if(b != a->end())
				return j;
		}
		return -1;
	}
};

struct insn_suffix_list :
	public option_list_t
{
	insn_suffix_list()
	{
		push_back(option_t());
		back().insert("size_def");
		back().insert("size_same");
		back().insert("size_none");
		back().insert("size_none_dsz");
		if(back().size() != size__end)
			throw __LINE__;

		push_back(option_t());
		back().insert("ctrlxfer_def");
		back().insert("ctrlxfer_yes");
		if(back().size() != ctrlxfer__end)
			throw __LINE__;
	}
};

struct encoding_suffix_list :
	public option_list_t
{
	encoding_suffix_list()
	{
		push_back(option_t());
		back().insert("mod_def");
		back().insert("mod_mem");
		for(int i = 0; i < MAX_ARGS; ++i)
			back().insert("mod_" + char_to_string('0' + i));
		if(back().size() != mod__end)
			throw __LINE__;

		push_back(option_t());
		back().insert("ro_def");
		back().insert("ro_z");
		back().insert("ro_r");
		for(int i = 0; i < 8; ++i)
			back().insert("ro_" + char_to_string('0' + i));
		if(back().size() != ro__end)
			throw __LINE__;

		push_back(option_t());
		back().insert("rm_def");
		for(int i = 0; i < 8; ++i)
			back().insert("rm_" + char_to_string('0' + i));
		if(back().size() != rm__end)
			throw __LINE__;

		push_back(option_t());
		back().insert("op66_def");
		back().insert("op66_no66");
		back().insert("op66_op66");
		if(back().size() != op66__end)
			throw __LINE__;

		push_back(option_t());
		back().insert("fx_def");
		back().insert("fx_lockable");
		back().insert("fx_locked");
		back().insert("fx_rep");
		back().insert("fx_none");
		back().insert("fx_none_lockable");
		back().insert("fx_f2");
		back().insert("fx_f3");
		if(back().size() != fx__end)
			throw __LINE__;

		push_back(option_t());
		back().insert("o_def");
		back().insert("o_is64");
		back().insert("o_no64");
		back().insert("o_sp64_movsxd");
		back().insert("o_no16");
		if(back().size() != o__end)
			throw __LINE__;

		push_back(option_t());
		back().insert("xasm_def");
		back().insert("xasm_skip");
		if(back().size() != xasm__end)
			throw __LINE__;

		push_back(option_t());
		back().insert("ea_def");
		back().insert("ea_itself");
		back().insert("ea_noaccess");
		if(back().size() != ea__end)
			throw __LINE__;

		push_back(option_t());
		back().insert("sx_def");
		back().insert("sx_yes");
		if(back().size() != sx__end)
			throw __LINE__;
	}
};

struct tmp_encoding_t
{
	std::string insn;
	U4 basecode;
	int nextbyte;
	std::set<std::string> suffix;
	std::string argtype[MAX_ARGS];
	std::string argtype_hi[MAX_ARGS];
	std::string argsize[MAX_ARGS];
	std::string argsize_hi[MAX_ARGS];
	U2 argvalue[MAX_ARGS];					// must be 0..0xff
};

struct insn_t
{
	std::string insn_name;
	std::set<std::string> insn_suffix;
	std::vector<tmp_encoding_t> encodings;
	std::list<std::string> disasm_code;
	std::list<std::string> semantics;
};

struct globals_t
{
	encoding_suffix_list elist;
	insn_suffix_list ilist;
	argtype_list_t tlist;
	argsize_list_t slist;
	argsize_hi_list_t hlist;
	
	// parser things
	std::string filename;
	int linenum;
	std::string line;
	std::string::iterator begin;
	char type;							// type of line--'$' or ':', or ' ' for other
	
	std::set<std::string> insns;
	std::string insn_name;
	std::set<std::string> insn_suffix;
	std::vector<tmp_encoding_t> encodings;
	std::list<std::string> disasm_code;
	std::list<std::string> semantics;
	
	std::list<insn_t> data;
};

class Semantics
{
	const char *curinsn;
	std::ostream &os;
public:
	void begin_insn(const char *name);
	void parse(const char *s);
	void end_insn();
	//std::map<std::string, unsigned> insnindex;
	std::map<std::string, std::vector<unsigned> > insnindex;
	std::map<std::string, unsigned> insntemps;
		std::map<std::string, unsigned> insnstart;
		std::map<std::string, unsigned> insnsize;
	unsigned maxsize;
	std::string max;
	std::string lastinsn;
	unsigned lastsize;
private:
	std::set<std::string> sizes;
	std::map<std::string, unsigned> temps;
	unsigned tempcount;
	unsigned curindex;
	unsigned encoding;
	
	struct tmp_t
	{
		std::string op;
		std::vector<struct tmp_t *> operands;
		U4 where;
		std::string size;
		tmp_t()
		{
			where = 0xffffffff;
			size.clear();		// unknown size. all literals have a size of 64 bits--is auto truncated.
		}
		bool is_numeric()
		{
			if(op.empty())
				throw "<internal error>";
			if(op[0] == '+' || op[0] == '-' || std::isdigit(op[0]))
				return true;	// literals always need a node
			return false;
		}
		bool needs_a_node()
		{
			if(is_numeric())
				return true;
			return !operands.empty();	// else, operators get a node
		}
	};

	std::vector<struct tmp_t *> stk;
	void print(tmp_t *node)
	{
		std::cout << node->op;
		if(!node->operands.empty())
			std::cout << "(";
		for(std::vector<struct tmp_t *>::iterator i = node->operands.begin(); i != node->operands.end(); ++i)
		{
			if(i != node->operands.begin())
				std::cout << ", ";
			print(*i);
		}
		if(!node->operands.empty())
			std::cout << ")";
	}
	U8 getnum(const char *s);
	void translate(tmp_t *node);
	void emit(tmp_t *node);
	void kill(tmp_t *node)
	{
		for(std::vector<struct tmp_t *>::iterator i = node->operands.begin(); i != node->operands.end(); ++i)
			kill(*i);
		//std::cout << "~" << node->op << std::endl;
		delete node;
	}
	void clear(bool make_root)
	{
		if(!stk.empty())
		{
			kill(stk.front());
			stk.clear();
		}
		if(make_root)
		{
			stk.push_back(new tmp_t());
			stk.back()->op = "root";
		}
		end_ok = true;
	}
	bool end_ok;
	
	void add_size(std::string size)
	{
		sizes.insert(size);
	}
	void add(std::string s);
public:
	Semantics(std::ostream &os_t);
	~Semantics();
};

void write_files(globals_t &g);

}	// namespace x86s

#endif	// l_x86s__x86s_script_h__included

// pass1.h
// Copyright (C) 2009 Willow Schlanger

#ifndef l_pass1_h__ncc_included
#define l_pass1_h__ncc_included

#include "parse.h"

#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cstddef>

// Symbol::type
enum
{
	ST_EXTERN,
	ST_LOCAL,
	ST_ARGNAME,
	ST_ARGSIZE,
	ST_FUNC,
	ST__END
};

struct Size
{
	// if base > 0, Size is that many bytes and scalar must be 1.
	// if base == 0, Size is "bit" (e.g. B1 but value is 0 or 1) and scalar must be 1.
	// if scalar == 0, size is unknown and base should be 0 too
	// if base < 0, size is parameterized and can be any positive number of bytes, and
	//    scalar must be >= 1. Note that for memory operands if the memory operand is a
	//    structure then the size indicated here is the size of the first element in the
	//    structure.
	short base;
	short scalar;
	
	Size(short baseT, short scalarT) :
		base(baseT),
		scalar(scalarT)
	{
	}
	
	std::string exact_size_string()
	{
		std::string s;
		if(base == 0)
			return "bit";
		s = "B";
		char buf[64];
		sprintf(buf, "%d", base);
		s += std::string(buf);
		return s;
	}
};

struct Symbol
{
	short type;
	short num;
	Size size;
	std::vector<Size> args;
	Symbol(short typeT, short numT, Size sizeT, std::vector<Size> &argsT) :
		size(sizeT)
	{
		type = typeT;
		num = numT;
		args = argsT;
	}
	Symbol(short typeT, short numT, Size sizeT) :
		size(sizeT)
	{
		type = typeT;
		num = numT;
		args.clear();
	}

	void dump_function(std::string name, struct Pass1 *pass1);
};

// Statement::operation
enum
{
	SO_ASGN,
	SO_ASSERT,
	SO_INPORT,
	SO_OUTPORT,
	SO_PUSH,
	SO_POP,
	SO_DISCARD,
	SO_RESERVE,
	SO_RESTORE,
	SO_COMMIT
};

// Note: sizeof(P) becomes a B8 literal when expanded. Thus, sizeof() functions
// themselves have a size of B8. Numeric literals on the other hand have special
// treatment for determination of their size.
struct Rhs
{
	Symbol *base;			// may be: extern, local, argname, argsize, func.
	std::list<Rhs *> fn_args;
	std::string text;		// if base is NULL may be a NUM; else symbol name is given here
							// if base is NULL this may also be "sizeof".
							// if base is NULL this can be one of these too:
							//    zx, sx, tr, sizeof
	Size size;				// matches base->size if base != NULL
	Size aux_size;			// if text == "sizeof" this is the src size
	
	Rhs() :
		size(0, 0),
		aux_size(0, 0)
	{
	}
	
	~Rhs()
	{
		for(std::list<Rhs *>::iterator i = fn_args.begin(); i != fn_args.end(); ++i)
		{
			delete *i;
		}
	}
	
	Rhs(const Rhs &src) : size(0,0), aux_size(0,0)
	{
		assert(src.fn_args.empty());
		base = src.base;
		fn_args.clear();
		text = src.text;
		size = src.size;
		aux_size = src.aux_size;
	}
};

struct Statement
{
	short operation;
	char osz;				// 0, or e.g. 16 for o16
	char asz;				// 0, or e.g. 16 for a16
	Symbol *dest;			// if operation==SO_ASGN; else NULL
	std::list<Rhs> src;
	
	Statement(short operationT)
	{
		operation = operationT;
		osz = 0;
		asz = 0;
		dest = NULL;
		src.clear();
	}
};

struct Insn
{
	std::vector<Size> locals;
	std::vector<Size> args;
	std::list<Statement> stmts;
	
	// Given a base arg size like -1, this returns the number of the
	// first argument defining that base with a scalar of 1.
	int get_arg_owner(short base)
	{
		assert(base < 0);
		for(int i = 0; i < args.size(); ++i)
		{
			if(args[i].scalar == 1 && args[i].base == base)
			{
				return i;
			}
		}
		assert(0);
		return 0;
	}
};

class Pass1
{
	public:
	void accept(ParseNode *root);
	void accept_insn_decl(ParseNode *node);
	void accept_extern_decl(ParseNode *node);
	void accept_func_decl(ParseNode *node);
	
	std::set<std::string> locals;
	std::map<std::string, Symbol *> symtab2;
	std::set<Symbol *> allsyms;
	std::map<std::string, Insn> insns;
	///short numLocals;
	short numExterns;
	short numFuncs;
	std::map<int, std::string> extern_names;
	
	void addsym(std::string name, Symbol *sym)
	{
		assert(symtab2.find(name) == symtab2.end());
		allsyms.insert(sym);
		symtab2[name] = sym;
	}
	
	~Pass1()
	{
		for(std::set<Symbol *>::iterator i = allsyms.begin(); i != allsyms.end(); ++i)
			delete *i;
	}
	
	void kill_locals()
	{
		for(std::set<std::string>::iterator i = locals.begin(); i != locals.end(); ++i)
		{
			std::map<std::string, Symbol *>::iterator j = symtab2.find(*i);
			if(j != symtab2.end())
				symtab2.erase(j);
		}
		locals.clear();
		///numLocals = 0;
	}

	Pass1()
	{
		///numLocals = 0;
		numExterns = 0;
		numFuncs = 0;
		
		// "undefined" is a built-in external bit that is read-only.
		add_extern("undefined", Size(0, 1) /* bit */);
	}
	
	// Returns true on success, or false if name already exists.
	bool add_extern(std::string name, Size size)
	{
		if(!is_valid_new_ident(name))
			return false;
		///symtab.insert(std::make_pair(name, Symbol(ST_EXTERN, numExterns, size)));
		addsym(name, new Symbol(ST_EXTERN, numExterns, size));
		extern_names[numExterns] = name;
		++numExterns;
		return true;
	}
	
	bool is_valid_new_ident(std::string s)
	{
		if(is_bnum(s))
			return false;
		return symtab2.find(s) == symtab2.end();
	}
	
	Size get_exact_size(std::string s)
	{
		if(!is_bnum(s))
			return Size(0, 0);
		if(s == "bit")
			return Size(0, 1);
		return Size(std::atoi(std::string(++s.begin(), s.end()).c_str()), 1);
	}
	
	private:
	bool is_bnum(std::string s)
	{
		if(s == "bit")
			return true;
		if(s.size() < 2)
			return false;
		if(s[0] != 'B')
			return false;
		std::string::iterator i = s.begin();
		for(++i; i != s.end(); ++i)
		{
				if(!std::isdigit(*i))
				break;
		}
		if(i == s.end())
			return true;
		return false;
	}

	void accept_statement(ParseNode *stmt, std::string insn_name, Insn &insn);
	void accept_asgn(std::string asgn_nm, ParseNode *asgn_rhs, ParseNode *simple, std::string insn_name, Insn &insn, char asz, char osz);
	void accept_rhs(Rhs &rhs, ParseNode *src, std::string insn_name, Insn &insn, Size asgn_size);
	Size get_rhs_size(ParseNode *rhs_size);
};

#endif	// l_pass1_h__ncc_included

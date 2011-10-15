// pass1.cpp
// Copyright (C) 2009 Willow Schlanger

#include "parse.h"
#include "pass1.h"

struct Arg
{
	std::string arg_name;
	std::string size_name;
	short base_size;	// if -1, see size_name
	short scalar;
};

// An 'args' node exists only when there is at least one argument.
// Use the Args constructor when at least one argument is present in a function declaration
// or an instruction declaration.
struct Args
{
	Args(Pass1 *pass1, ParseNode *node /* args node */);
	void accept_arg(Pass1 *pass1, ParseNode *arg);
	
	std::vector<Arg> args;
	std::map<std::string, short> argtypes;	// e.g. "P" -> -1
	
	std::string get_arg_type_name(short value)
	{
		for(std::map<std::string, short>::iterator i = argtypes.begin(); i != argtypes.end(); ++i)
		{
			if(i->second == value)
				return i->first;
		}
		return "";
	}
};

Args::Args(Pass1 *pass1, ParseNode *node)
{
	assert(node->token == RULE_ARGS);
	std::list<ParseNode *>::iterator i = node->children.begin();
	assert(i != node->children.end());
	for(;;)
	{
		accept_arg(pass1, *i);
		++i;	// skip past arg
		if(i == node->children.end())
			break;
		assert((*i)->token == ',');
		++i;	// skip comma
		assert(i != node->children.end());
	}
	
	// For each parameterized type, make sure the type exists with a scalar of one someplace.
	// Also do some sanity checks.
	///int argnum = 0;
	for(std::vector<Arg>::iterator j = args.begin(); j != args.end(); ++j)
	{
		///++argnum;
		std::string arg_name = j->arg_name, size_name = j->size_name;
		if(j->base_size >= 0)
			size_name.clear();
		if(!pass1->is_valid_new_ident(arg_name))
		{
				throw ParseError(std::string("Argument name in use already: ")
					+ j->arg_name,
					node->lineNum, node->fileNum, __LINE__
				);
		}
		// Make sure no other arguments have the same name, and make sure our type name is valid.
		if(!size_name.empty())
		{
			if(!pass1->is_valid_new_ident(size_name))
			{
				///std::cout << argnum << "[" << j->base_size << "]";
					throw ParseError(std::string("Argument size in use already: ")
						+ size_name,
						node->lineNum, node->fileNum, __LINE__
					);
			}
		}
		for(std::vector<Arg>::iterator k = args.begin(); k != args.end(); ++k)
		{
			if(k != j)
			{
				if(k->arg_name == arg_name)
				{
					throw ParseError(std::string("Argument name repeated: ")
						+ arg_name,
						node->lineNum, node->fileNum, __LINE__
					);
				}
				if(!size_name.empty())
				{
					if(k->arg_name == size_name)
					{
						throw ParseError(std::string("Argument name is same as a size name: ")
							+ k->arg_name,
							node->lineNum, node->fileNum, __LINE__
						);
					}
				}
			}
		}

		if(j->base_size == -1 && j->scalar != 1)
		{
			std::vector<Arg>::iterator k = args.begin();
			for(; k != args.end(); ++k)
			{
				if(k->base_size == -1)
				{
					if(k->size_name == j->size_name && k->scalar == 1)
						break;
				}
			}
			if(k == args.end())
			{
				throw ParseError(std::string("Parameterized argument is scaled; can't find unscaled argument: ")
					+ j->arg_name,
					node->lineNum, node->fileNum, __LINE__
				);
			}
		}
	}
	
	// fn/insn ( P * 2 x, P y );
	
	// Now create arguments in Pass1 object.
	short num_arg_types = -1;
	short arg_count = 0;
	for(std::vector<Arg>::iterator j = args.begin(); j != args.end(); ++j)
	{
		if(j->base_size == -1)
		{
			// Parameterized argument.
			std::map<std::string, short>::iterator k = argtypes.find(j->size_name);
			if(k == argtypes.end())
			{
				// Create arg type.
				argtypes[j->size_name] = num_arg_types;
				assert(pass1->is_valid_new_ident(j->size_name));
				Size size(num_arg_types, 1);
				pass1->addsym(j->size_name, new Symbol(ST_ARGSIZE, num_arg_types, size));
				pass1->locals.insert(j->size_name);
				--num_arg_types;
			}
		}
		// Make sure argument is not already taken.
		assert(pass1->is_valid_new_ident(j->arg_name));
		short sz = j->base_size;
		if(sz == -1)
			sz = argtypes[j->size_name];
		Size size(sz, j->scalar);
		pass1->addsym(j->arg_name, new Symbol(ST_ARGNAME, arg_count, size));
		pass1->locals.insert(j->arg_name);
		++arg_count;
	}
}

short get_short(std::string s)
{
	if(s.empty())
		return -1;
	short value = 0;
	short x;
	for(std::string::iterator i = s.begin(); i != s.end(); ++i)
	{
		if(*i < '0' || *i > '9')
			return -1;
		x = value;
		value = (value * 10) + (*i - '0');
		if(value <= x)
			return -1;
	}
	return value;
}

void Args::accept_arg(Pass1 *pass1, ParseNode *arg)
{
	assert(arg->token == RULE_ARG);
	std::list<ParseNode *>::iterator i = arg->children.begin();
	assert(i != arg->children.end());
	assert((*i)->token == CTokenEnums::TOKEN_IDENT);
	std::string base_size = (*i)->text;
	short scalar = 1;
	++i;	// skip past base type
	assert(i != arg->children.end());
	if((*i)->token == '*')
	{
		++i;	// skip past *
		assert(i != arg->children.end());
		assert((*i)->token == CTokenEnums::TOKEN_NUM);
		scalar = get_short((*i)->text);
		if(scalar <= 1)
		{
			throw ParseError(std::string("Invalid argument type: ") + base_size + "*" + (*i)->text,
				arg->lineNum, arg->fileNum, __LINE__
			);
		}
		++i;	// skip scalar
	}
	assert((*i)->token == CTokenEnums::TOKEN_IDENT);
	std::string arg_name = (*i)->text;
	Arg xarg;
	xarg.arg_name = arg_name;
	xarg.size_name = base_size;
	xarg.scalar = scalar;
	Size tmp = pass1->get_exact_size(base_size);
	///std::cout << "(" << base_size << " " << tmp.base << " " << tmp.scalar << ")";
	if(tmp.scalar == 0)
		xarg.base_size = -1;	// parameterized size
	else
	{
		xarg.base_size = tmp.base;
		assert(xarg.scalar == 1);
	}
	///argnums[arg_name] = args.size();
	args.push_back(xarg);
}

void Pass1::accept(ParseNode *root)
{
	ParseNode *node = root;
	assert(node->token == RULE_START);
	node = node->children.back();
	assert(node->token == RULE_DECLARATION_D_SEQ);
	for(std::list<ParseNode *>::iterator i = node->children.begin();
		i != node->children.end();
		++i
	)
	{
		ParseNode *node = *i;
		assert(node->token == RULE_DECLARATION);
		assert(!node->children.empty());
		
		kill_locals();
		
		if(node->children.front()->token == KEYWORD_INSN)
		{
			accept_insn_decl(node);
		}
		else
		if(node->children.front()->token == KEYWORD_EXTERN)
		{
			accept_extern_decl(node);
		}
		else
		{
			accept_func_decl(node);
		}
	}
}

void Pass1::accept_extern_decl(ParseNode *node)
{
	std::list<ParseNode *>::iterator i = node->children.begin();
	assert((*i)->token == KEYWORD_EXTERN);
	++i;	// i -> prototype-type
	assert((*i)->token == RULE_PROTOTYPE_D_TYPE);
	assert((*i)->children.size() == 1);
	ParseNode *typeident = (*i)->children.front();
	assert(typeident->token == CTokenEnums::TOKEN_IDENT);
	Size size = get_exact_size(typeident->text);
	if(size.scalar == 0)
	{
		throw ParseError(std::string("Externals must have exact size - given: ") + typeident->text,
			node->lineNum, node->fileNum, __LINE__
		);
	}
	++i;	// go to first extern
	for(;;)
	{
		///std::cout << size.exact_size_string() << " - " << (*i)->text << std::endl;
		bool ok = add_extern((*i)->text, size);
		if(!ok)
		{
			throw ParseError("Name already exists - " + (*i)->text,
				node->lineNum,
				node->fileNum,
				__LINE__
			);
		}
	
		++i;
		assert(i != node->children.end());
		if((*i)->token != ',')
			break;
		++i;	// skip comma
	}
}

void Pass1::accept_func_decl(ParseNode *node)
{
	std::list<ParseNode *>::iterator i = node->children.begin();
	ParseNode *tprototype = *i;
	assert(tprototype->token == RULE_PROTOTYPE_D_TYPE);
	++i;
	ParseNode *prototype = *i;
	assert(prototype->token == RULE_PROTOTYPE);
	++i;
	assert((*i)->token == ';');

	//---
	i = prototype->children.begin();
	std::string fn_name = (*i)->text;
	++i;	// skip fn_name
	assert((*i)->token == '(');
	++i;	// skip '('
	
	Symbol *symT = new Symbol(ST_FUNC, numFuncs, Size(0, 0));
	Symbol &sym = *symT;
	
	if((*i)->token == RULE_ARGS)
	{
		Args args(this, *i);
		for(std::vector<Arg>::iterator j = args.args.begin(); j != args.args.end(); ++j)
		{
			short x = j->base_size;
			if(x == -1)
			{
				assert(args.argtypes.find(j->size_name) != args.argtypes.end());
				x = args.argtypes[j->size_name];
				assert(x < 0);
			}
			sym.args.push_back(Size(x, j->scalar));
		}
		
		std::string xs = tprototype->children.front()->text;
		sym.size = get_exact_size(xs);
		if(sym.size.scalar == 0)
		{
			// unknown size.
			if(args.argtypes.find(xs) == args.argtypes.end())
			{
				throw ParseError("Bad return size for function - " + fn_name,
					node->lineNum,
					node->fileNum,
					__LINE__
				);
			}
			short x = args.argtypes[xs];
			assert(x < 0);
			sym.size.base = x;
			sym.size.scalar = 1;
			// Now handle any *.
			if(tprototype->children.size() != 1)
			{
				sym.size.scalar = get_short(tprototype->children.back()->text);
				if(sym.size.scalar <= 1)
				{
					throw ParseError("Bad return size for function - " + fn_name,
						node->lineNum,
						node->fileNum,
						__LINE__
					);
				}
			}
		}
		else
		if(sym.size.scalar != 1 || tprototype->children.size() != 1)
		{
			throw ParseError("Bad return size for function - " + fn_name,
				node->lineNum,
				node->fileNum,
				__LINE__
			);
		}
	}
	else
	{
		sym.size = get_exact_size(tprototype->children.front()->text);
		if(sym.size.scalar != 1 || tprototype->children.size() != 1)
		{
			throw ParseError("Bad return size for function - " + fn_name,
				node->lineNum,
				node->fileNum,
				__LINE__
			);
		}
	}

	if(!is_valid_new_ident(fn_name))
	{
		throw ParseError("Name already exists - " + fn_name,
			node->lineNum,
			node->fileNum,
			__LINE__
		);
	}

///
///sym.dump_function(fn_name, this);
///

	///symtab.insert(std::make_pair(fn_name, sym));
	addsym(fn_name, symT);
	++numFuncs;
	//---
}

void Symbol::dump_function(std::string name, struct Pass1 *pass1)
{
	assert(type == ST_FUNC);
	
	char next = 'P';
	std::map<short, char> xm;
	
	for(std::vector<Size>::iterator i = args.begin(); i != args.end(); ++i)
	{
		short type = i->base;
		if(type < 0)
		{
			if(xm.find(type) == xm.end())
			{
				xm[type] = next;
			
				++next;	// hopefully there won't be more than 26 parameterized argument types
				if(next > 'Z')
					next = 'A';
			}
		}
	}
	
	assert(size.scalar != 0);
	if(size.base < 0)
	{
		assert(xm.find(size.base) != xm.end());
		std::cout << xm[size.base];
	}
	else
	if(size.base == 0)
		std::cout << "bit";
	else
		std::cout << "B" << size.base;
	if(size.scalar > 1)
		std::cout << "*" << size.scalar;

	std::cout << " ";
	std::cout << name << "(";
	int argnum = 0;
	for(std::vector<Size>::iterator i = args.begin(); i != args.end(); ++i, ++argnum)
	{
		if(i != args.begin())
			std::cout << ", ";

		Size size(i->base, i->scalar);
		assert(size.scalar != 0);
		if(size.base < 0)
		{
			assert(xm.find(size.base) != xm.end());
			std::cout << xm[size.base];
		}
		else
		if(size.base == 0)
			std::cout << "bit";
		else
			std::cout << "B" << size.base;
		if(size.scalar > 1)
			std::cout << "*" << size.scalar;
		std::string argname = "<name unknown>";
		
		for(std::set<std::string>::iterator j = pass1->locals.begin(); j != pass1->locals.end(); ++j)
		{
			Symbol &sym = *pass1->symtab2.find(*j)->second;
			if(sym.type == ST_ARGNAME && sym.num == argnum)
			{
				argname = *j;
				break;
			}
		}
		
		std::cout << " " << argname;
	}
	std::cout << ");" << std::endl;
}

void Pass1::accept_insn_decl(ParseNode *node)
{
	std::list<ParseNode *>::iterator i = node->children.begin();
	assert(i != node->children.end() && (*i)->token == KEYWORD_INSN);
	++i;	// skip "insn"
	ParseNode *prototype = *i;
	++i;	// skip prototype
	assert((*i)->token == '{');
	++i;	// skip {
	ParseNode *body = NULL;
	if((*i)->token == RULE_BODY)
	{
		body = *i;
		++i;
	}
	assert((*i)->token == '}');
	
	// We now have 'body' and 'prototype' filled out (the latter may be NULL).

	//---
	i = prototype->children.begin();
	std::string insn_name = (*i)->text;
	++i;	// skip insn_name
	assert((*i)->token == '(');
	++i;	// skip '('
	
	///Symbol sym(ST_INSN, insns.size(), Size(0, 0));
	Insn insn;
	
	if((*i)->token == RULE_ARGS)
	{
		Args args(this, *i);
		for(std::vector<Arg>::iterator j = args.args.begin(); j != args.args.end(); ++j)
		{
			short x = j->base_size;
			if(x == -1)
			{
				assert(args.argtypes.find(j->size_name) != args.argtypes.end());
				x = args.argtypes[j->size_name];
				assert(x < 0);
			}
			insn.args.push_back(Size(x, j->scalar));
		}
	}

	if(insns.find(insn_name) != insns.end())
	{
		throw ParseError("Name already exists - " + insn_name,
			node->lineNum,
			node->fileNum,
			__LINE__
		);
	}
	//---
	
	// Fill out 'locals' and 'stmts' next.
	Insn &insnT = insns.insert(std::make_pair(insn_name, insn)).first->second;

	if(body != NULL)
	{
		i = body->children.begin();
		assert(i != body->children.end());
		while(i != body->children.end())
		{
			ParseNode *stmt = *i;
			assert(stmt->token == RULE_STATEMENT);

			accept_statement(stmt, insn_name, insnT);
			
			++i;	// skip statement
			assert(i != body->children.end());
			assert((*i)->token == ';');
			++i;	// skip semicolon
		}
	}
}

void Pass1::accept_statement(ParseNode *stmt, std::string insn_name, Insn &insn)
{
	assert(stmt->token == RULE_STATEMENT);
	char osz = 0, asz = 0;
	std::list<ParseNode *>::iterator i = stmt->children.begin();
	assert(i != stmt->children.end());
	bool valid = true;
	while((*i)->token == RULE_PREFIX)
	{
		int prefix = (*i)->children.front()->token;
		switch(prefix)
		{
			case KEYWORD_O16:
				if(osz == 0)
					osz = 16;
				else
					valid = false;
				break;
			case KEYWORD_O32:
				if(osz == 0)
					osz = 32;
				else
					valid = false;
				break;
			case KEYWORD_O64:
				if(osz == 0)
					osz = 64;
				else
					valid = false;
				break;
			case KEYWORD_A16:
				if(asz == 0)
					asz = 16;
				else
					valid = false;
				break;
			case KEYWORD_A32:
				if(asz == 0)
					asz = 32;
				else
					valid = false;
				break;
			case KEYWORD_A64:
				if(asz == 0)
					asz = 64;
				else
					valid = false;
				break;
			default:
				assert(0);
				break;
		}
		
		++i;
	}
	assert((*i)->token == RULE_SIMPLE_D_STATEMENT);
	if(!valid)
	{
		throw ParseError("Too many prefix (o16/o32/o64 or a16/a32/a64).",
			stmt->lineNum,
			stmt->fileNum,
			__LINE__
		);
	}
	
	ParseNode *simple = *i;
	assert(!simple->children.empty());
	
	bool is_pure_asgn = false;
	if(simple->children.size() == 3)
	{
		i = simple->children.begin();	// possibly IDENT
		++i;							// skip possible IDENT
		if((*i)->token == '=')
			is_pure_asgn = true;
	}
	
	i = simple->children.begin();
	if(!is_pure_asgn && (*i)->token == CTokenEnums::TOKEN_IDENT)
	{
		if(osz != 0 || asz != 0)
		{
			throw ParseError("Prefix (o16/o32/o64 or a16/a32/a64) used with local variable definition.",
				stmt->lineNum,
				stmt->fileNum,
				__LINE__
			);
		}
	}
	
	std::string asgn_nm;
	ParseNode *asgn_rhs = NULL;
	
	// First allocate a local variable if that is in order.
	if(!is_pure_asgn && (*i)->token == CTokenEnums::TOKEN_IDENT)
	{
		std::string sizeid = (*i)->text;	// get identifier (size)
		++i;								// skip size identifier
		Size sz = get_exact_size(sizeid);
		if(sz.scalar == 0)
		{
			// Look up this size, it's a parameter.
			std::map<std::string, Symbol *>::iterator j = symtab2.find(sizeid);
			if(j == symtab2.end() || j->second->type != ST_ARGSIZE)
			{
				throw ParseError("Local variable size error: can't find size: " + sizeid,
					stmt->lineNum,
					stmt->fileNum,
					__LINE__
				);
			}
			sz.base = j->second->num;
			assert(sz.base < 0);
			sz.scalar = 1;
			if((*i)->token == '*')
			{
				++i;	// skip *
				sz.scalar = get_short((*i)->text);
				if(sz.scalar <= 1)
				{
					throw ParseError("Local variable size error: scalar must be 2..32767, found: " + (*i)->text,
						stmt->lineNum,
						stmt->fileNum,
						__LINE__
					);
				}
				++i;	// skip scalar
			}
		}
		else
		{
			if((*i)->token == '*')
			{
				throw ParseError("Local variable size error: can't use * with built-in type: " + sizeid,
					stmt->lineNum,
					stmt->fileNum,
					__LINE__
				);
			}
		}
		assert((*i)->token == CTokenEnums::TOKEN_IDENT);
		std::string nm = (*i)->text;
		
		// (sz, nm) now set.
		++i;
		if(i != simple->children.end())
		{
			assert((*i)->token == '=');
			++i;		// skip '='
			asgn_rhs = *i;
			asgn_nm = nm;
		}
		
		// Create local variable here.
		if(!is_valid_new_ident(nm))
		{
			throw ParseError("Local variable name already in symbol table: " + nm,
				stmt->lineNum,
				stmt->fileNum,
				__LINE__
			);
		}
		Symbol sym();
		addsym(nm, new Symbol(ST_LOCAL, insn.locals.size(), sz));
		insn.locals.push_back(sz);
		locals.insert(nm);
	}

	i = simple->children.begin();
	if(is_pure_asgn)
	{
		asgn_nm = (*i)->text;	// ident
		++i;					// skip ident
		assert((*i)->token == '=');
		++i;					// skip '='
		asgn_rhs = *i;
	}
	
	// This is used for e.g. both of these cases:
	//    zeta = undefined;
	//    bit tmp = undefined;
	if(asgn_rhs != NULL)
	{
		accept_asgn(asgn_nm, asgn_rhs, simple, insn_name, insn, asz, osz);
	}
	
	// Handle assert, push, pop, discard, outport, inport, reserve, restore here.
	i = simple->children.begin();
	if((*i)->token != CTokenEnums::TOKEN_IDENT)
	{
		int x = 0;
		switch((*i)->token)
		{
			case KEYWORD_ASSERT:
				x = SO_ASSERT;
				break;
			case KEYWORD_PUSH:
				x = SO_PUSH;
				break;
			case KEYWORD_POP:
				x = SO_POP;
				break;
			case KEYWORD_DISCARD:
				x = SO_DISCARD;
				break;
			case KEYWORD_OUTPORT:
				x = SO_OUTPORT;
				break;
			case KEYWORD_INPORT:
				x = SO_INPORT;
				break;
			case KEYWORD_RESERVE:
				x = SO_RESERVE;
				break;
			case KEYWORD_RESTORE:
				x = SO_RESTORE;
				break;
			case KEYWORD_COMMIT:
				x = SO_COMMIT;
				break;
			default:
				assert(0);
				break;
		}
		insn.stmts.push_back(Statement(x));
		Statement &stmt = insn.stmts.back();
		stmt.osz = osz;
		stmt.asz = asz;
		stmt.dest = NULL;
		
		// Now do any arguments.
		if(x == SO_RESERVE || x == SO_RESTORE)
		{
			stmt.src.push_back(Rhs());
			Rhs &outrhs = stmt.src.back();
			++i;	// skip keyword
			++i;	// skip '('
			accept_rhs(outrhs, *i, insn_name, insn, Size(8, 1));
		}
		else
		{
			++i;	// skip keyword
			++i;	// skip '('
			while((*i)->token == RULE_RHS)
			{
				stmt.src.push_back(Rhs());
				Rhs &outrhs = stmt.src.back();
				accept_rhs(outrhs, *i, insn_name, insn, Size(0, 0));
			
				if(outrhs.size.scalar == 0)
				{
					throw ParseError("Unable to resolve size of argument. If NUM, try e.g. tr<B8>(NUM)." + asgn_nm,
					simple->lineNum,
					simple->fileNum,
					__LINE__
					);
				}
			
				++i;	// skip rhs
				if((*i)->token != ',')
					break;
				++i;	// skip comma
			}
			assert((*i)->token == ')');
		}
	}
}

void Pass1::accept_asgn(
	std::string asgn_nm, ParseNode *asgn_rhs, ParseNode *simple, std::string insn_name, Insn &insn,
	char asz, char osz
)
{
	/// fixme, do this!
	// 1. Get size of asgn_nm (destination of assignment).
	//    Actually, we can just look up asgn_nm in the symbol table.
	// 2. asgn_rhs is an 'rhs' node.
	//    If the rhs node is a NUM token then we have to give it a size that matches the destination
	//    size.
	// 3. Evaluation of 'rhs'.
	//    a. [?]
	
	std::map<std::string, Symbol *>::iterator symiter = symtab2.find(asgn_nm);
	if(symiter == symtab2.end())
	{
		throw ParseError("Destination of assignment not found in symbol table: " + asgn_nm,
			simple->lineNum,
			simple->fileNum,
			__LINE__
		);
	}
	Symbol &destsym = *symiter->second;
	Size &destsize = destsym.size;
	
	insn.stmts.push_back(Statement(SO_ASGN));
	Statement &stmt = insn.stmts.back();
	stmt.osz = osz;
	stmt.asz = asz;
	stmt.dest = &destsym;
	assert(stmt.dest->type == ST_LOCAL || stmt.dest->type == ST_EXTERN || stmt.dest->type == ST_ARGNAME);
	stmt.src.push_back(Rhs());
	Rhs &rhsT = stmt.src.back();
	
	accept_rhs(rhsT, asgn_rhs, insn_name, insn, destsize);
	
	if(rhsT.size.base != destsize.base || rhsT.size.scalar != destsize.scalar)
	{
///std::cout << rhsT.size.base << " " << rhsT.size.scalar << " " << destsize.base << " " << destsize.scalar << std::endl;
		throw ParseError("Size mismatch for assignment to " + asgn_nm,
			simple->lineNum,
			simple->fileNum,
			__LINE__
		);
	}
}

Size Pass1::get_rhs_size(ParseNode *rhs_size)
{
	Size size(0, 0);
	
	assert(rhs_size->token == RULE_RHS_D_SIZE);
	assert(!rhs_size->children.empty());
	assert(rhs_size->children.front()->token == CTokenEnums::TOKEN_IDENT);
	std::list<ParseNode *>::iterator i = rhs_size->children.begin();
	
	//---
	{
		std::string sizeid = (*i)->text;	// get identifier (size)
		++i;								// skip size identifier
		size = get_exact_size(sizeid);
		if(size.scalar == 0)
		{
			// Look up this size, it's a parameter.
			std::map<std::string, Symbol *>::iterator j = symtab2.find(sizeid);
			if(j == symtab2.end() || j->second->type != ST_ARGSIZE)
			{
				throw ParseError("Size error: can't find size: " + sizeid,
					rhs_size->lineNum,
					rhs_size->fileNum,
					__LINE__
				);
			}
			size.base = j->second->num;
			assert(size.base < 0);
			size.scalar = 1;
			if(i != rhs_size->children.end() && (*i)->token == '*')
			{
				++i;	// skip *
				size.scalar = get_short((*i)->text);
				if(size.scalar <= 1)
				{
					throw ParseError("Size error: scalar must be 2..32767, found: " + (*i)->text,
						rhs_size->lineNum,
						rhs_size->fileNum,
						__LINE__
					);
				}
				++i;	// skip scalar
			}
		}
		else
		{
			// Got exact size.
			if(i != rhs_size->children.end() && (*i)->token == '*')
			{
				throw ParseError("Size error: can't use * with built-in type: " + sizeid,
					rhs_size->lineNum,
					rhs_size->fileNum,
					__LINE__
				);
			}
		}
	}
	//---

	return size;
}

// rhs : output of this method
// src : an "rhs" node - input
// asgn_size : if rhs is assigned to a destination, this is the size of the destination.
//             if we have f(EAX, 1234) where f is defined as
//                P f(P a, P b);
//             then upon evaluation of the 2nd argument of f, we pass in a size of (base=4,
//             scalar=1) to asgn_size.
//             Otherwise, asgn_size is undefined, i.e. (base=0, scalar=0).
//             Note that before we return, if the rhs we produced is a function invokation, we
//             check the size of all arguments. In particular, if "P fn(P x, P*2 y)" is defined,
//             then fn(AX, EAX) is valid. We check this by going thru all arguments in the function
//             in question, until we find a parameter with a scalar of 1. Here, "x" is scaled by 1.
//             We the ngo thru all arguments that have that same parameterized size, and we verify
//             all P's have the same size; and P*2 is twice the size. P may be B2; then, P*2 is B4.
//             Or, a function argument may have the size of an argument.
void Pass1::accept_rhs(Rhs &rhs, ParseNode *src, std::string insn_name, Insn &insn, Size asgn_size)
{
	assert(src->token == RULE_RHS);
	
	U4 token = src->children.front()->token;

	if(token == CTokenEnums::TOKEN_IDENT)
	{
		std::string nm = src->children.front()->text;
		std::map<std::string, Symbol *>::iterator symiter = symtab2.find(nm);
		if(symiter == symtab2.end())
		{
			std::string suffix = (src->children.size() > 1) ? "()" : "";
#if 1
			throw ParseError("undeclared identifier: " + nm + suffix,
				src->lineNum,
				src->fileNum,
				__LINE__
			);
#else
			std::cout << nm << suffix << std::endl;
#endif
		}
		Symbol &sym = *symiter->second;
		rhs.base = &sym;
		rhs.fn_args.clear();
		rhs.text = nm;
		rhs.size = sym.size;
		
		if(src->children.size() != 1)
		{
			// Process function. Note that in this case if rhs.size.base < 0 then we need to fix it up.
			
			std::list<ParseNode *>::iterator i = src->children.begin();
			++i;	// skip ident
			ParseNode *p = *i;	// get rhs-args
			i = p->children.begin();
			assert((*i)->token == '(');
			++i;
			ParseNode *list = NULL;
			if((*i)->token == RULE_LIST)
			{
				list = *i;
				assert(list->token == RULE_LIST);
				++i;
			}
			assert((*i)->token == ')');
			
			if(list == NULL && sym.args.size() != 0)
			{
				throw ParseError("arguments missing for call to function " + nm + "()",
					src->lineNum,
					src->fileNum,
					__LINE__
				);
			}

			if(rhs.size.base < 0 && list == NULL)
			{
				// function has parameterized size but it has no arguments at all!
				throw ParseError("unable to resolve size of function " + nm + "()",
					src->lineNum,
					src->fileNum,
					__LINE__
				);
			}
			
			if(list != NULL)
			{
				// we have a list of rhs'es.
				std::vector<ParseNode *> params;
				
				i = list->children.begin();
				assert(i != list->children.end());
				params.push_back(*i);	// get first rhs in list
				for(++i /* skip 1st rhs */; i != list->children.end(); )
				{
					assert((*i)->token == ',');
					++i;	// skip comma
					assert((*i)->token == RULE_RHS);
					assert(!(*i)->children.empty());
					params.push_back(*i);	// get next rhs
					++i;	// skip rhs
				}
				
				if(sym.args.size() != params.size())
				{
					throw ParseError("bad number of arguments for call to function " + nm + "()",
						src->lineNum,
						src->fileNum,
						__LINE__
					);
				}
				
				// Evaluate all non-NUM nodes. Leave NULLs in place of NUM nodes.
				std::map<int, Size> arg_sizes;
				for(unsigned i = 0; i < params.size(); ++i)
				{
					if(params[i]->children.front()->token == CTokenEnums::TOKEN_NUM)
					{
							rhs.fn_args.push_back(NULL);
					}
					else
					{
						rhs.fn_args.push_back(new Rhs());
						Rhs &arg_rhs = *rhs.fn_args.back();
						accept_rhs(arg_rhs, params[i], insn_name, insn, Size(0, 0));
						if(sym.args[i].base < 0)
						{
							arg_sizes.insert(std::make_pair(sym.args[i].base, arg_rhs.size));
							if(arg_rhs.size.base == 0)
							{
								throw ParseError("parameterized argument to function " + nm + "() has bit size",
									src->lineNum,
									src->fileNum,
									__LINE__
								);
							}
						}
					}
				}
				
				// Now do "pure" immediates.
				std::list<Rhs *>::iterator rhsiter = rhs.fn_args.begin();
				for(unsigned i = 0; i < params.size(); ++i, ++rhsiter)
				{
					if(params[i]->children.front()->token == CTokenEnums::TOKEN_NUM)
					{
						// f(..., 1234, ....)
						Size num_size = sym.args[i];
						if(num_size.base < 0)
						{
							// size of arg is parameterized.
							if(arg_sizes.find(num_size.base) == arg_sizes.end())
							{
								throw ParseError("unable to resolve return size of a numeric argument to function " + nm + "()",
									src->lineNum,
									src->fileNum,
									__LINE__
								);
							}
							Size &ts = arg_sizes.find(num_size.base)->second;
							num_size.base = ts.base;
							if(ts.base >= 0)
							{
								num_size.base *= ts.scalar;
								num_size.scalar = 1;
							}
							else
							{
								num_size.scalar = ts.scalar;
							}
						}
						
						//...
						assert(*rhsiter == NULL);
						*rhsiter = new Rhs();
						Rhs &arg_rhs = **rhsiter;
						accept_rhs(arg_rhs, params[i], insn_name, insn, num_size);
					}
				}
				
				// Now check arg size constraints.
				rhsiter = rhs.fn_args.begin();
				for(unsigned i = 0; i < params.size(); ++i, ++rhsiter)
				{
					if(sym.args[i].base < 0)
					{
						// Parameterized argument size.
						Size actual_size = (*rhsiter)->size;	// size user supplied us with
						assert(actual_size.scalar != 0);	// make sure size is known!
						
						// Note. sym.args[i] is e.g. B2 for x in f(B2 x).
						// code:
						//  P f(P x, P*2 y)
						//  evaluate: f(EAX, 123)  [valid]
						//  here, actual_size=B4, wanted_size=B4*2=B8.
						Size wanted_size = arg_sizes.find(sym.args[i].base)->second;
						
						if(actual_size.base > 0 && actual_size.scalar > 1)
						{
							actual_size.base *= actual_size.scalar;
							actual_size.scalar = 1;
						}
						if(wanted_size.base > 0 && wanted_size.scalar > 1)
						{
							wanted_size.base *= wanted_size.scalar;
							wanted_size.scalar = 1;
						}
						
						if(actual_size.base != wanted_size.base ||
							actual_size.scalar != wanted_size.scalar
						)
						{
							throw ParseError("an argument to function " + nm + "() has an incompatible size",
								src->lineNum,
								src->fileNum,
								__LINE__
							);
						}
					}
				}
				
				// Now adjust return type.				
				if(rhs.size.base < 0)
				{
					// e.g. P cmul(P x, P y) was called with e.g. cmul(tmp[B1], imm[B1]).
					if(arg_sizes.find(rhs.size.base) == arg_sizes.end())
					{
						throw ParseError("unable to resolve return size of function " + nm + "()",
							src->lineNum,
							src->fileNum,
							__LINE__
						);
					}
					Size &ts = arg_sizes.find(rhs.size.base)->second;
					rhs.size.base = ts.base;
					if(ts.base >= 0)
					{
						rhs.size.base *= ts.scalar;
						rhs.size.scalar = 1;
					}
					else
					{
						rhs.size.scalar = ts.scalar;
					}
				}
			}
		}
	}
	else
	if(token == CTokenEnums::TOKEN_NUM)
	{
		if(asgn_size.scalar == 0)
		{
			throw ParseError("Number has unknown size due to context: " + src->children.front()->text,
				src->lineNum,
				src->fileNum,
				__LINE__
			);
		}
		rhs.base = NULL;
		rhs.fn_args.clear();
		rhs.text = src->children.front()->text;
		rhs.size = asgn_size;
	}
	else
	if(token == KEYWORD_SIZEOF)
	{
		rhs.base = NULL;
		rhs.fn_args.clear();
		rhs.text = "sizeof";
		rhs.size = Size(8, 1);

		std::list<ParseNode *>::iterator i = src->children.begin();
		++i;	// skip sizeof
		++i;	// skip (
		ParseNode *szsrc = *i;
		++i;
		assert((*i)->token == ')');
		Size src_size = get_rhs_size(szsrc);
		assert(src_size.scalar != 0 && src_size.base != 0);	// must be defined and not a bit
		if(src_size.scalar > 1)
		{
			// e.g. sizeof(P*2) is allowed but not sizeof(B1*2).
			assert(src_size.base < 0);
		}
		rhs.aux_size = src_size;
	}
	else
	if(token == KEYWORD_SX || token == KEYWORD_ZX || token == KEYWORD_TR)
	{
		// sx<>, zx<>, tr<>.
		
		// First get the size.
		std::list<ParseNode *>::iterator i = src->children.begin();
		++i;	// skip function name (a keyword)
		++i;	// skip <
		ParseNode *xsize = *i;
		++i;	// skip size
		++i;	// skip >
		++i;	// skip (
		ParseNode *xrhs = *i;
		++i;	// skip rhs
		assert((*i)->token == ')');
		Size ysize = get_rhs_size(xsize);
		//assert(ysize.base != 0);	// can't sx<>, zx<>, or tr<> to a bit size.
		
		rhs.base = NULL;
		rhs.fn_args.clear();
		if(token == KEYWORD_SX)
			rhs.text = "sx";
		else
		if(token == KEYWORD_ZX)
			rhs.text = "zx";
		else
		if(token == KEYWORD_TR)
			rhs.text = "tr";
		else
		{
			assert(0);
			throw -1;
		}
		rhs.size = ysize;
		
		rhs.fn_args.push_back(new Rhs());
		Rhs &srcrhs = *rhs.fn_args.back();
		
		// Use of 'ysize' below assures us that e.g. tr<B4>(1234) will truncate 1234 to B4 size.
		accept_rhs(srcrhs, xrhs, insn_name, insn, ysize);
		
		assert(srcrhs.size.scalar != 0 && ysize.scalar != 0);
		if(srcrhs.size.base == 0 || ysize.base == 0)
		{
			throw ParseError("sx<>, zx<>, tr<> can't have 'bit' source or target size.",
				src->lineNum,
				src->fileNum,
				__LINE__
			);
		}
	}
	else
	{
		assert(0);
	}
}

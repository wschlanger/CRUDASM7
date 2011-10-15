// make_out_emulator.cpp
// Copyright (C) 2009 Willow Schlanger

#include "make_out_emulator.h"
#include <fstream>
#include <iostream>

static void write_size(std::ofstream &fo, Size &size, Pass1 &pass1, Insn &insn);

static void get_insn_sensitivity(Insn &insn, bool &asz_sensitive, bool &osz_sensitive)
{
	asz_sensitive = false;
	osz_sensitive = false;
	for(std::list<Statement>::iterator i = insn.stmts.begin(); i != insn.stmts.end(); ++i)
	{
		if(i->asz != 0)
			asz_sensitive = true;
		if(i->osz != 0)
			osz_sensitive = true;
	}
}

static void get_specialized_stmts(std::list<Statement *> &dest, std::list<Statement> &src, int osz, int oszcount,
	int asz, int aszcount
)
{
	dest.clear();
	for(std::list<Statement>::iterator i = src.begin(); i != src.end(); ++i)
	{
		// note that if oszcount is 0, then all i->osz's will be 0.
		// this applies to aszcount and i->asz too.
		bool oszok = (i->osz == 0) || (i->osz == (16 << osz));
		bool aszok = (i->asz == 0) || (i->asz == (16 << asz));
		if(oszok && aszok)
			dest.push_back(&(*i));
		// check code
		if(i->operation == SO_ASGN)
		{
			assert(i->dest->type >= 0 && i->dest->type < ST__END);
		}
		// end check
	}
}

static void write_dest_symbol(std::ofstream &fo, Symbol &sym, Pass1 &pass1, Insn &insn, const char *tabs)
{
	assert(sym.type == ST_EXTERN || sym.type == ST_LOCAL || sym.type == ST_ARGNAME);
	if(sym.type == ST_EXTERN)
		fo << "exec->write_extern_" << pass1.extern_names[sym.num] << "(";
	else
	if(sym.type == ST_LOCAL)
		fo << "tmp" << sym.num << ".write(";
	else
		fo << "exec->write_arg(" << sym.num << ", ";
}

static void write_src_symbol(std::ofstream &fo, Symbol &sym, Pass1 &pass1, Insn &insn)
{
	assert(sym.type == ST_EXTERN || sym.type == ST_LOCAL || sym.type == ST_ARGNAME);
	if(sym.type == ST_EXTERN)
		fo << "exec->read_extern_" << pass1.extern_names[sym.num] << "()";
	else
	if(sym.type == ST_LOCAL)
		fo << "tmp" << sym.num;
	else
		fo << "exec->read_arg(" << sym.num << ")";
}

static void write_rhs(std::ofstream &fo, Rhs &rhs, Pass1 &pass1, Insn &insn)
{
	if(rhs.base != NULL)
	{
		Symbol &sym = *rhs.base;
		if(sym.type == ST_EXTERN || sym.type == ST_LOCAL || sym.type == ST_ARGNAME)
			write_src_symbol(fo, sym, pass1, insn);
		else
		if(sym.type == ST_FUNC)
		{
			fo << "eval->comp_" << rhs.text << "(";
			for(std::list<Rhs *>::iterator i = rhs.fn_args.begin(); i != rhs.fn_args.end(); ++i)
			{
				if(i != rhs.fn_args.begin())
					fo << ", ";
				write_rhs(fo, **i, pass1, insn);
			}
			fo << ")";
		}
		else
		{
			assert(0);
			throw -1;
		}
	}
	else
	{
		assert(!rhs.text.empty());
		if(std::isdigit(rhs.text[0]))
		{
			// NUM. FIXME: handle floating-point numbers here.
			fo << "eval->get_literal(";
			write_size(fo, rhs.size, pass1, insn);
			fo << ", ";
			fo << rhs.text;
			fo << ")";
		}
		else
		if(rhs.text == "sizeof")
		{
			fo << "eval->get_literal(8, ";
			write_size(fo, rhs.aux_size, pass1, insn);
			fo << ") /* sizeof */";
		}
		else
		if(rhs.text == "zx" || rhs.text == "sx" || rhs.text == "tr")
		{
			fo << "eval->get_" << rhs.text << "(";
			write_size(fo, rhs.size, pass1, insn);
			fo << ", ";
			assert(rhs.fn_args.size() == 1);
			write_rhs(fo, *rhs.fn_args.front(), pass1, insn);
			fo << ")";
		}
		else
		{
			assert(0);
			throw -1;
		}
	}
}

static void write_size(std::ofstream &fo, Size &size, Pass1 &pass1, Insn &insn)
{
	if(size.base == 0)
		fo << "1";
	else
	if(size.base > 0)
		fo << size.base;
	else
	{
		fo << "exec->get_arg_size(" << insn.get_arg_owner(size.base) << ")";
		if(size.scalar > 1)
			fo << " * " << size.scalar;
	}
}

static void write_stmt(std::ofstream &fo, Statement &stmt, Pass1 &pass1, Insn &insn, const char *tabs)
{
	//fo << tabs << ";" << std::endl;
	if(stmt.operation == SO_ASGN)
	{
		fo << tabs;
		assert(stmt.dest != NULL);
		write_dest_symbol(fo, *stmt.dest, pass1, insn, tabs);
		assert(!stmt.src.empty());
		write_rhs(fo, stmt.src.front(), pass1, insn);
		fo << ");" << std::endl;
	}
	else
	if(stmt.operation == SO_INPORT || stmt.operation == SO_POP)
	{
		assert(!stmt.src.empty());
		fo << tabs;
		Symbol *sym = stmt.src.front().base;
		assert(sym != NULL);
		write_dest_symbol(fo, *sym, pass1, insn, tabs);
		if(stmt.operation == SO_POP)
		{
			assert(stmt.src.size() == 1);
			fo << "exec->do_pop()";
		}
		else
		{
			assert(stmt.src.size() == 2);
			fo << "exec->do_inport(";
			write_size(fo, stmt.src.front().size, pass1, insn);
			fo << ", ";
			write_rhs(fo, stmt.src.back(), pass1, insn);
			fo << ")";
		}
		
		fo << ");" << std::endl;
	}
	else
	if(stmt.operation == SO_OUTPORT)
	{
		assert(stmt.src.size() == 2);
		fo << "exec->do_outport(";
		write_rhs(fo, stmt.src.front(), pass1, insn);
		fo << ", ";
		write_rhs(fo, stmt.src.back(), pass1, insn);
		fo << ");";
	}
	else
	{
		const char *nm = NULL;
		switch(stmt.operation)
		{
			case SO_ASSERT: nm = "assert"; break;
			case SO_PUSH: nm = "push"; break;
			case SO_DISCARD: nm = "discard"; break;
			case SO_COMMIT: nm = "commit"; break;
			case SO_RESERVE: nm = "reserve"; break;
			case SO_RESTORE: nm = "restore"; break;
			default: break;
		}
		assert(nm != NULL);
		assert(stmt.src.size() == 1);
		fo << tabs;
		fo << "exec->do_" << nm << "(";
		write_rhs(fo, stmt.src.front(), pass1, insn);
		fo << ");" << std::endl;
	}
}

void write_insn(std::ofstream &fo, Insn &insn, Pass1 &pass1)
{
	// Declare locals.
	for(unsigned i = 0; i < insn.locals.size(); ++i)
	{
		fo << "\t\t\tOperand tmp" << i << "(";
		if(insn.locals[i].base == 0)
			fo << "1";
		else
		if(insn.locals[i].base > 0)
			fo << insn.locals[i].base;
		else
		{
			fo << "exec->get_arg_size(" << insn.get_arg_owner(insn.locals[i].base) << ") * ";
			fo << insn.locals[i].scalar;
		}
		fo << ");" << std::endl;
	}
	
	// Next thing to do is statements. We want to handle e.g. o16 prefixes here, too.
	bool asz_sensitive, osz_sensitive;
	get_insn_sensitivity(insn, asz_sensitive, osz_sensitive);
	int aszcount = (asz_sensitive) ? 3 : 1;
	int oszcount = (osz_sensitive) ? 3 : 1;
	for(int osz = 0; osz < oszcount; ++osz)
	{
		for(int asz = 0; asz < aszcount; ++asz)
		{
			if(oszcount == 3 && aszcount == 3)
				fo << "\t\t\tif(exec->get_osz_bits() == " << (16 << osz) << " && exec->get_asz_bits() == " << (16 << asz) << ")" << std::endl;
			else
			if(oszcount == 3)
				fo << "\t\t\tif(exec->get_osz_bits() == " << (16 << osz) << ")" << std::endl;
			else
			if(aszcount == 3)
				fo << "\t\t\tif(exec->get_asz_bits() == " << (16 << asz) << ")" << std::endl;
			fo << "\t\t\t{" << std::endl;

			std::list<Statement *> stmts;
			get_specialized_stmts(stmts, insn.stmts, osz, oszcount, asz, aszcount);
			
			for(std::list<Statement *>::iterator i = stmts.begin(); i != stmts.end(); ++i)
			{
				write_stmt(fo, **i, pass1, insn, "\t\t\t\t");
			}
			
			fo << "\t\t\t\treturn true;" << std::endl;
			fo << "\t\t\t}" << std::endl;
		}
	}
}

void make_out_emulator(Pass1 &pass1)
{
	std::ofstream fo("out_emulator.h");
	fo << "// out_emulator.h  (note: automatically generated file)\n// Copyright (C) 2009 Willow Schlanger" << std::endl;
	fo << std::endl;
	///fo << "#include \"emulator.h\"" << std::endl;
	///fo << std::endl;
	//fo << "template <class Executor, class Evaluator>" << std::endl;
	///fo << "bool GeneratedEmulator<Executor, Evaluator>::execute()" << std::endl;
	fo << "{" << std::endl;
	fo << "\tswitch(exec->get_insn())" << std::endl;
	fo << "\t{" << std::endl;
	
	for(std::map<std::string, Insn>::iterator i = pass1.insns.begin(); i != pass1.insns.end(); ++i)
	{
		fo << "\t\tcase insn_" << i->first << ":" << std::endl;
		fo << "\t\t{" << std::endl;
		
		write_insn(fo, i->second, pass1);
		
		fo << "\t\t\tbreak;\t// returns false" << std::endl;
		fo << "\t\t}" << std::endl;
	}
	
	fo << "\t\tdefault:" << std::endl;
	fo << "\t\t\tbreak;" << std::endl;
	fo << "\t}" << std::endl;
	fo << "\treturn false;" << std::endl;
	fo << "}" << std::endl;
	fo << std::endl;
}

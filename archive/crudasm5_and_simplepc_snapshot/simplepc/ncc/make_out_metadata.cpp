// make_out_metadata.cpp
// Copyright (C) 2009 Willow Schlanger

#include "make_out_metadata.h"

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

struct InsnMetaInstance
{
	std::set<std::string> exts[2];	// exts[0]=in's, exts[1]=out's
	std::set<int> args[2];			// args[0]=in's, args[1]=out's
	bool stk[2];					// stack[0]=in(pop/discard), stack[1]=out(push/commit)
	
	InsnMetaInstance()
	{
		stk[0] = false;
		stk[1] = false;
	}

	void add_ext_input(std::string s)
	{
		if(exts[1].find(s) == exts[1].end())
			exts[0].insert(s);
	}
	
	void add_arg_input(int a)
	{
		if(args[1].find(a) == args[1].end())
			args[0].insert(a);
	}
};

struct InsnMeta
{
	bool osz_sensitive;
	bool asz_sensitive;
	
	// instance[3*osz+asz] is the instance info.
	// if !osz_sensitive, always use osz==0.
	// if !asz_sensitive, always use asz==0.
	InsnMetaInstance instance[3*3];
};

class GatherMeta
{
	Pass1 &pass1;
	
	public:
	GatherMeta(Pass1 &pass1T) :
		pass1(pass1T)
	{
		gather();
	}
	std::map<std::string, InsnMeta> meta;
	
	private:
	void gather();
	void gather_stmts(std::list<Statement *> &stmts, Insn &insn, InsnMetaInstance &instance);
	void gather_rhs_inputs(Rhs &rhs, Insn &insn, InsnMetaInstance &instance);
	void gather_symbol_output(Symbol &sym, Insn &insn, InsnMetaInstance &instance);
};

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

// Go thru each insn in 'pass1'.
void GatherMeta::gather()
{
	for(std::map<std::string, Insn>::iterator i = pass1.insns.begin();
		i != pass1.insns.end();
		++i
	)
	{
		std::string iname = i->first;
///std::cout << iname << ":" << std::endl;
		Insn &insn = i->second;
		bool asz_sensitive;
		bool osz_sensitive;
		get_insn_sensitivity(insn, asz_sensitive, osz_sensitive);
		meta[iname] = InsnMeta();
		InsnMeta &imeta = meta[iname];
		imeta.osz_sensitive = osz_sensitive;
		imeta.asz_sensitive = asz_sensitive;
		int oszcount = (osz_sensitive) ? 3 : 1;
		int aszcount = (asz_sensitive) ? 3 : 1;
		for(int osz = 0; osz < oszcount; ++osz)
		{
			for(int asz = 0; asz < aszcount; ++asz)
			{
				std::list<Statement *> stmts;
				get_specialized_stmts(stmts, insn.stmts, osz, oszcount, asz, aszcount);
				gather_stmts(stmts, insn, imeta.instance[3*osz+asz]);
///std::cout << std::endl;
			}
		}
	}
}

void GatherMeta::gather_stmts(std::list<Statement *> &stmts, Insn &insn, InsnMetaInstance &instance)
{
///int stnum = 0;
	for(std::list<Statement *>::iterator i = stmts.begin(); i != stmts.end(); ++i)
	{
///std::cerr << "#" << (++stnum) << std::endl;
		Statement &stmt = **i;
		
		if(stmt.operation == SO_POP || stmt.operation == SO_DISCARD)
			instance.stk[0] = true;	// inputs from the stack (i.e. rSP is increased)
		else
		if(stmt.operation == SO_PUSH || stmt.operation == SO_COMMIT)
			instance.stk[1] = true;	// outputs to the stack (i.e. rSP is decreased)
		
		switch(stmt.operation)
		{
			case SO_ASGN:
				assert(stmt.dest != NULL);
				// stmt.dest is the output. It can be an external, an argument, or a local (temporary).
				assert(stmt.src.size() == 1);
				gather_rhs_inputs(stmt.src.front(), insn, instance);
				gather_symbol_output(*stmt.dest, insn, instance);
				break;
			case SO_POP:
			///case SO_OUTPORT: // oops, OUT DX,AL has DX as an input, not output
			case SO_INPORT:
			{
				// For these, the 1st argument is an output; any other arguments are inputs.
				assert(!stmt.src.empty());
				Rhs &outrhs = stmt.src.front();
				assert(outrhs.fn_args.empty());	// make sure we don't have e.g. outport(f(DX), AL).
				assert(outrhs.base != NULL);
				assert(outrhs.base->type != ST_FUNC);	// also disallow e.g. outport(myfunc(), AL).
				std::list<Rhs>::iterator i = stmt.src.begin();
				for(++i; i != stmt.src.end(); ++i)
					gather_rhs_inputs(*i, insn, instance);
				gather_symbol_output(*outrhs.base, insn, instance);
				break;
			}
			case SO_OUTPORT:	// added -- see above
			case SO_ASSERT:
			case SO_PUSH:
			case SO_DISCARD:
			case SO_RESERVE:
			case SO_RESTORE:
			case SO_COMMIT:
			{
				// For these, all arguments are inputs.
				std::list<Rhs>::iterator i = stmt.src.begin();
				for(; i != stmt.src.end(); ++i)
					gather_rhs_inputs(*i, insn, instance);
				break;
			}
			default:
				assert(0);
				throw -1;
				break;
		}
	}
}

void GatherMeta::gather_rhs_inputs(Rhs &rhs, Insn &insn, InsnMetaInstance &instance)
{
///std::cout << "+" << rhs.text << std::endl;
	if(rhs.base == NULL)
	{
		if(rhs.text == "zx" || rhs.text == "sx" || rhs.text == "tr")
		{
			for(std::list<Rhs *>::iterator i = rhs.fn_args.begin(); i != rhs.fn_args.end(); ++i)
				gather_rhs_inputs(**i, insn, instance);
		}
		else
			;	// numeric constant or "sizeof" operator - skip it!
		
		return;
	}
	
	Symbol &sym = *rhs.base;

	if(sym.type == ST_EXTERN)
		//instance.exts[0].insert(pass1.extern_names[sym.num]);
		instance.add_ext_input(pass1.extern_names[sym.num]);
	else
	if(sym.type == ST_ARGNAME)
	{
		//instance.args[0].insert(sym.num);
		instance.add_arg_input(sym.num);
	}
	else
	if(sym.type == ST_LOCAL)
		;	// ignore it
	else
	if(sym.type == ST_FUNC)
	{
		for(std::list<Rhs *>::iterator i = rhs.fn_args.begin(); i != rhs.fn_args.end(); ++i)
			gather_rhs_inputs(**i, insn, instance);
	}
	else
	{
		// ST_ARGSIZE not allowed here.
		assert(0);
		throw -1;
	}
}

void GatherMeta::gather_symbol_output(Symbol &sym, Insn &insn, InsnMetaInstance &instance)
{
	assert(sym.type == ST_EXTERN || sym.type == ST_LOCAL || sym.type == ST_ARGNAME);
	/*
	if(sym.type == ST_EXTERN)
		std::cout << " out extern " << pass1.extern_names[sym.num] << std::endl;
	if(sym.type == ST_LOCAL)
		std::cout << " out local " << sym.num << std::endl;
	if(sym.type == ST_ARGNAME)
		std::cout << " out arg " << sym.num << std::endl;
	*/
	
	if(sym.type == ST_EXTERN)
		instance.exts[1].insert(pass1.extern_names[sym.num]);
	else
	if(sym.type == ST_ARGNAME)
		instance.args[1].insert(sym.num);
}

static void tabout(std::ofstream &fo, int level)
{
	for(int i = 0; i < level; ++i)
		fo << "\t";
}

static void make_externals(Pass1 &pass1, GatherMeta &meta, int inout, std::ofstream &fo)
{
	const char *inout_text[2] = {"input", "output"};
	fo << "// returns a list of external " << inout_text[inout] << "s." << std::endl;
	
	fo << "const int *get_ext_" << inout_text[inout] << "s(int insn, int osz, int asz)" << std::endl;
	fo << "{" << std::endl;
	
	fo << "\tswitch(insn)" << std::endl;
	fo << "\t{" << std::endl;
	
	for(std::map<std::string, InsnMeta>::iterator i = meta.meta.begin(); i != meta.meta.end(); ++i)
	{
		fo << "\t\tcase insn_" << i->first << ":" << std::endl;
		fo << "\t\t{" << std::endl;
		
		InsnMeta &imeta = i->second;
		int oszcount = (imeta.osz_sensitive) ? 3 : 1;
		int aszcount = (imeta.asz_sensitive) ? 3 : 1;
		for(int osz = 0; osz < oszcount; ++osz)
		{
			for(int asz = 0; asz < aszcount; ++asz)
			{
				if(oszcount == 3 && aszcount == 3)
					fo << "\t\t\tif(osz == " << (16 << osz) << " && asz == " << (16 << asz) << ")" << std::endl;
				else
				if(oszcount == 3)
					fo << "\t\t\tif(osz == " << (16 << osz) << ")" << std::endl;
				else
				if(aszcount == 3)
					fo << "\t\t\tif(asz == " << (16 << asz) << ")" << std::endl;
				fo << "\t\t\t{" << std::endl;
				
				fo << "\t\t\t\tstatic const int ext_data[] = {" << std::endl;
				InsnMetaInstance &instance = imeta.instance[3*osz+asz];
				std::set<std::string>::iterator j = instance.exts[inout].begin();
				for(; j != instance.exts[inout].end(); ++j)
				{
					fo << "\t\t\t\t\tEXTERN_" << *j << "," << std::endl;
				}
				fo << "\t\t\t\t\t-1" << std::endl;
				fo << "\t\t\t\t};" << std::endl;
				
				fo << "\t\t\t\treturn ext_data;" << std::endl;				
				fo << "\t\t\t}" << std::endl;
			}
		}
		
		fo << "\t\t\tbreak;\t// return NULL" << std::endl;
		fo << "\t\t}" << std::endl;
	}
	
	fo << "\t\tdefault:" << std::endl;
	fo << "\t\t\tbreak;" << std::endl;
	fo << "\t};" << std::endl;	
	fo << "\treturn 0;\t// NULL" << std::endl;
	fo << "}" << std::endl;
	fo << std::endl;
}

static void make_args(Pass1 &pass1, GatherMeta &meta, std::ofstream &fo)
{
	fo << "// returns a list of argument inputs/outputs. bit 0 = in, bit 1 = out, bit 2 = end." << std::endl;
	
	fo << "const char *get_arg_info(int insn, int osz, int asz)" << std::endl;
	fo << "{" << std::endl;
	
	fo << "\tswitch(insn)" << std::endl;
	fo << "\t{" << std::endl;
	
	for(std::map<std::string, InsnMeta>::iterator i = meta.meta.begin(); i != meta.meta.end(); ++i)
	{
		fo << "\t\tcase insn_" << i->first << ":" << std::endl;
		fo << "\t\t{" << std::endl;
		
		InsnMeta &imeta = i->second;
		int oszcount = (imeta.osz_sensitive) ? 3 : 1;
		int aszcount = (imeta.asz_sensitive) ? 3 : 1;
		for(int osz = 0; osz < oszcount; ++osz)
		{
			for(int asz = 0; asz < aszcount; ++asz)
			{
				if(oszcount == 3 && aszcount == 3)
					fo << "\t\t\tif(osz == " << (16 << osz) << " && asz == " << (16 << asz) << ")" << std::endl;
				else
				if(oszcount == 3)
					fo << "\t\t\tif(osz == " << (16 << osz) << ")" << std::endl;
				else
				if(aszcount == 3)
					fo << "\t\t\tif(asz == " << (16 << asz) << ")" << std::endl;
				fo << "\t\t\t{" << std::endl;
				
				fo << "\t\t\t\tstatic const char ext_data[] = {" << std::endl;
				InsnMetaInstance &instance = imeta.instance[3*osz+asz];
				
				int lastarg = -1;
				for(std::set<int>::iterator j = instance.args[0].begin(); j != instance.args[0].end(); ++j)
				{
					if(*j > lastarg)
						lastarg = *j;
				}
				for(std::set<int>::iterator j = instance.args[1].begin(); j != instance.args[1].end(); ++j)
				{
					if(*j > lastarg)
						lastarg = *j;
				}
				
				for(int k = 0; k <= lastarg; ++k)
				{
					bool is_in = instance.args[0].find(k) != instance.args[0].end();
					bool is_out = instance.args[1].find(k) != instance.args[1].end();
					int value = 0;
					if(is_in)
						value += 1;
					if(is_out)
						value += 2;
					fo << "\t\t\t\t\t" << value << "," << std::endl;
				}
				
				fo << "\t\t\t\t\t4\t// bit 2 set = end" << std::endl;
				fo << "\t\t\t\t};" << std::endl;
				
				fo << "\t\t\t\treturn ext_data;" << std::endl;				
				fo << "\t\t\t}" << std::endl;
			}
		}
		
		fo << "\t\t\tbreak;\t// return NULL" << std::endl;
		fo << "\t\t}" << std::endl;
	}
	
	fo << "\t\tdefault:" << std::endl;
	fo << "\t\t\tbreak;" << std::endl;
	fo << "\t};" << std::endl;	
	fo << "\treturn 0;\t// NULL" << std::endl;
	fo << "}" << std::endl;
	fo << std::endl;
}

static void make_metadata(Pass1 &pass1, GatherMeta &meta, std::ofstream &fo)
{
	fo << "// returns some instruction meta-data." << std::endl;
	
	fo << "const InsnMetaData *get_insn_metadata(int insn, int osz, int asz)" << std::endl;
	fo << "{" << std::endl;
	
	fo << "\tswitch(insn)" << std::endl;
	fo << "\t{" << std::endl;
	
	for(std::map<std::string, InsnMeta>::iterator i = meta.meta.begin(); i != meta.meta.end(); ++i)
	{
		fo << "\t\tcase insn_" << i->first << ":" << std::endl;
		fo << "\t\t{" << std::endl;
		
		InsnMeta &imeta = i->second;
		int oszcount = (imeta.osz_sensitive) ? 3 : 1;
		int aszcount = (imeta.asz_sensitive) ? 3 : 1;
		for(int osz = 0; osz < oszcount; ++osz)
		{
			for(int asz = 0; asz < aszcount; ++asz)
			{
				InsnMetaInstance &instance = imeta.instance[3*osz+asz];
			
				if(oszcount == 3 && aszcount == 3)
					fo << "\t\t\tif(osz == " << (16 << osz) << " && asz == " << (16 << asz) << ")" << std::endl;
				else
				if(oszcount == 3)
					fo << "\t\t\tif(osz == " << (16 << osz) << ")" << std::endl;
				else
				if(aszcount == 3)
					fo << "\t\t\tif(asz == " << (16 << asz) << ")" << std::endl;
				fo << "\t\t\t{" << std::endl;
				
				fo << "\t\t\t\tstatic const InsnMetaData mdata = { {" << std::endl;
				
				const char *inout_text[2] = {"input", "output"};
				for(int inout = 0; inout < 2; ++inout)
				{
					fo << "\t\t\t\t\t{\t// " << inout_text[inout] << "s" << std::endl;
					
					std::string s;
					s = "RIP"; fo << "\t\t\t\t\t\t" << ((instance.exts[inout].find(s) != instance.exts[inout].end()) ? "1" : "0") << ",\t// " << s << std::endl;
					s = "RIP1"; fo << "\t\t\t\t\t\t" << ((instance.exts[inout].find(s) != instance.exts[inout].end()) ? "1" : "0") << ",\t// " << s << std::endl;
					s = "RIPC"; fo << "\t\t\t\t\t\t" << ((instance.exts[inout].find(s) != instance.exts[inout].end()) ? "1" : "0") << ",\t// " << s << std::endl;
					s = "RINT"; fo << "\t\t\t\t\t\t" << ((instance.exts[inout].find(s) != instance.exts[inout].end()) ? "1" : "0") << ",\t// " << s << std::endl;
					s = "RFLAGS"; fo << "\t\t\t\t\t\t" << ((instance.exts[inout].find(s) != instance.exts[inout].end()) ? "1" : "0") << ",\t// " << s << std::endl;
					s = "RBPX"; fo << "\t\t\t\t\t\t" << ((instance.exts[inout].find(s) != instance.exts[inout].end()) ? "1" : "0") << ",\t// " << s << std::endl;
					s = "RSPX"; fo << "\t\t\t\t\t\t" << ((instance.exts[inout].find(s) != instance.exts[inout].end()) ? "1" : "0") << ",\t// " << s << std::endl;
					///s = "RIP1"; fo << "\t\t\t\t\t\t" << ((instance.exts[inout].find(s) != instance.exts[inout].end()) ? "1" : "0") << ",\t// " << s << std::endl;
					fo << "\t\t\t\t\t\t";
					if(instance.stk[inout])
						fo << "1";
					else
						fo << "0";
					fo << "\t// Stack" << std::endl;
					
					fo << "\t\t\t\t\t}";
					if(inout == 0)
						fo << ",";
					fo << std::endl;
				}

				fo << "\t\t\t\t} };" << std::endl;
				
				fo << "\t\t\t\treturn &mdata;" << std::endl;				
				fo << "\t\t\t}" << std::endl;
			}
		}
		
		fo << "\t\t\tbreak;\t// return NULL" << std::endl;
		fo << "\t\t}" << std::endl;
	}
	
	fo << "\t\tdefault:" << std::endl;
	fo << "\t\t\tbreak;" << std::endl;
	fo << "\t};" << std::endl;	
	fo << "\treturn 0;\t// NULL" << std::endl;
	fo << "}" << std::endl;
	fo << std::endl;
}

void make_out_metadata(Pass1 &pass1)
{
	GatherMeta meta(pass1);

	std::ofstream fo("out_metadata.cpp");
	fo << "// out_metadata  (note: automatically generated file)\n// Copyright (C) 2009 Willow Schlanger" << std::endl;
	fo << std::endl;
	fo << "#include \"imetadata.h\"" << std::endl;
	fo << std::endl;
	
	make_externals(pass1, meta, 0, fo);
	make_externals(pass1, meta, 1, fo);
	make_args(pass1, meta, fo);
	make_metadata(pass1, meta, fo);
}

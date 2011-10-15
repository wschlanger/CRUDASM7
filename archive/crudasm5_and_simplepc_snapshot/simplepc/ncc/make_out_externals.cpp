// make_out_externals.cpp
// Copyright (C) 2009 Willow Schlanger

#include "make_out_externals.h"
#include <fstream>
#include <iostream>

void make_out_externals(Pass1 &pass1)
{
	std::ofstream fh("out_externals.h");
	fh << "// out_externals.h  (note: automatically generated file)\n// Copyright (C) 2009 Willow Schlanger" << std::endl;
	fh << std::endl;
	fh << "enum {" << std::endl;
	for(std::map<std::string, Symbol *>::iterator i = pass1.symtab2.begin(); i != pass1.symtab2.end(); ++i)
	{
		if(i->second->type == ST_EXTERN)
		{
			fh << "\tEXTERN_" << i->first << "," << std::endl;
		}
	}
	fh << "\tEXTERN__COUNT" << std::endl;
	fh << "};" << std::endl;
	fh << std::endl;
	fh << "extern int extern_size[];" << std::endl;
	fh << std::endl;
	
	std::ofstream fcpp("out_externals.cpp");
	fcpp << "// out_externals.cpp  (note: automatically generated file)\n// Copyright (C) 2009 Willow Schlanger" << std::endl;
	fcpp << std::endl;
	fcpp << "#include \"out_externals.h\"" << std::endl;
	fcpp << std::endl;
	fcpp << "int extern_size[] = {" << std::endl;
	for(std::map<std::string, Symbol *>::iterator i = pass1.symtab2.begin(); i != pass1.symtab2.end(); ++i)
	{
		if(i->second->type == ST_EXTERN)
		{
			fcpp << "\t/* EXTERN_" << i->first << " */" << ((i->second->size.base == 0) ? 1 : i->second->size.base) << "," << std::endl;
		}
	}
	fcpp << "\t0" << std::endl;
	fcpp << "};" << std::endl;
	fcpp << std::endl;
}


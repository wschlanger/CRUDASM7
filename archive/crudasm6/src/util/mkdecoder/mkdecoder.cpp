// mkdecoder.cpp
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "../readxml/readxml.h"
#include <cassert>
#include <fstream>
#include <stddef.h>
typedef size_t UINT;

//===============================================================================================//

class IntelDecoderTableBuilder
{
	IntelSyntax *syntax;
public:

	IntelDecoderTableBuilder(IntelSyntax *syntaxT) :
		syntax(syntaxT)
	{
	}

	std::string buildTable(std::vector<U4> &table);
	std::string process_decoder_encoding(IntelSyntaxEncoding &enc, class OpcodeTableContainer *opcon, std::string insn_name);
	std::string process_decoder_modrm(class OpcodeTable *table, size_t offset, IntelSyntaxEncoding &enc, class OpcodeTableContainer *opcon, std::string insn_name);
	std::string process_decoder_entry(class OpcodeTable *table, size_t offset, IntelSyntaxEncoding &enc, class OpcodeTableContainer *opcon, std::string insn_name, int ext_index, int md_index, int size_override = -1);
};

//===============================================================================================//

std::string int_to_string(int x)
{
	char s[33];
	using namespace std;
	sprintf(s, "%d", x);
	return std::string(s);
}

/*
Table codes:
1 - a. 256 entry decoder (full opcode bytes)
2 - b. prefix decoder (replock & op66 splitter)
3 - c. modr/m, high 5 bits - not compatible with (d)
4 - d. modr/m, bits 5..3 - not compatible with (c)
5 - e. modr/m, bits 2..0
6 - f. dsz == 64 bit mode (used by arpl/movsxd)
       not compatible with (g); must be at end if used
7 - g. 256 entry decoder, imm8 after modr/m (3DNow!)
       not compatible with (f); must be at end if used
*/

struct OpcodeTable
{
	int type;
	UINT size;
	OpcodeTable **values;		// not used if type == 0
	int encoding_number;		// manually set if type == 0

	OpcodeTable(int typeT)
	{
		type = typeT;

		if(type == 0)
		{
			size = 0;
			values = NULL;
			return;
		}

		switch(type)
		{
		case 2:
			size = 6;
			break;
		case 3:
			size = 32;
			break;
		case 4:
		case 5:
			size = 8;
			break;
		case 6:
			size = 2;
			break;
		default:
			size = 256;
			break;
		}
		values = new OpcodeTable * [size];
		for(int i = 0; i < size; ++i)
		{
			values[i] = NULL;
		}
	}
	
	void unalloc()
	{
		if(type == 0)
			return;
		if(values != NULL)
			delete [] values;
		values = NULL;
	}

	OpcodeTable **get(UINT index)
	{
		assert(index < size);
		if(index >= size)
			return NULL;
		return &values[index];
	}

	bool is_level_3_really_level_4()
	{
		if(type != 3 || size != 32)
			return false;
		for(int i = 0; i < 8; ++i)
		{
			OpcodeTable *x = values[i];
			for(int j = 1; j < 4; ++j)
			{
				if(values[i+8*j] != x)
				{
					return false;
				}
			}
		}
		return true;
	}
};

struct OpcodeTableContainer
{
	std::list<OpcodeTable *> oplst;
	OpcodeTable *root;

	OpcodeTableContainer()
	{
		root = alloc(1);
	}

	OpcodeTable *alloc(int type)
	{
		oplst.push_back(new OpcodeTable(type));
		return oplst.back();
	}

	~OpcodeTableContainer()
	{
		for(std::list<OpcodeTable *>::iterator i = oplst.begin(); i != oplst.end(); ++i)
		{
			(*i)->unalloc();
			delete *i;
		}
	}
};

struct OpcodeList
{
	std::list<OpcodeTable *> lst;

	OpcodeList(OpcodeTable &root)
	{
		process(&root);
	}

private:
	std::set<OpcodeTable *> visited;
	void process(OpcodeTable *entry)
	{
		if(entry->type == 0 || entry->size <= 0)
			return;		// don't process encoding pointers
		if(visited.find(entry) != visited.end())
			return;		// already been here
		visited.insert(entry);
		for(int i = 0; i < entry->size; ++i)
		{
			if(entry->values[i] != NULL)
			{
				process(entry->values[i]);
			}
		}
		lst.push_back(entry);
	}
};

std::string IntelDecoderTableBuilder::buildTable(std::vector<U4> &table)
{
	table.clear();
	OpcodeTableContainer opcon;

	for(std::list<IntelSyntaxInsn>::iterator i = syntax->insnList.begin(); i != syntax->insnList.end(); ++i)
	{
		//std::cout << syntax->getSymbol(i->proto.name) << std::endl;
		for(std::list<IntelSyntaxEncoding>::iterator j = i->encodings.begin(); j != i->encodings.end(); ++j)
		{
			std::string result = process_decoder_encoding(*j, &opcon, syntax->getSymbol(i->proto.name));
			if(!result.empty())
				return std::string("Error building decoder table for instruction") + syntax->getSymbol(i->proto.name) + std::string(": ") + result;
		}
	}

	OpcodeList ol(*opcon.root);
	U4 opcode_root = 0;
	std::vector<U4> &opcodes = table;

	std::map<OpcodeTable *, U4> m;
	for(std::list<OpcodeTable *>::iterator i = ol.lst.begin(); i != ol.lst.end(); ++i)
	{
		OpcodeTable *table = *i;
		if(table == opcon.root)
		{
			opcode_root = (1 << 24) | static_cast<U4>(opcodes.size());
		}
		m[table] = static_cast<U4>(opcodes.size());	// remember this table

		UINT table_size = table->size;

		if(table->is_level_3_really_level_4())
		{
			table_size = 8;
			///std::cout  << "+" << std::endl;
		}

		for(int j = 0; j < table_size; ++j)
		{
			OpcodeTable *entry = table->values[j];
			if(entry == NULL)
				opcodes.push_back(0x00ffffff);		// invalid opcode
			else
			if(entry->type == 0)
				opcodes.push_back(entry->encoding_number);
			else
			{
				assert(m.find(entry) != m.end());	// make sure we already processed this table
				int entry_type = entry->type;
				if(entry->is_level_3_really_level_4())
					entry_type = 4;
				opcodes.push_back((entry_type << 24) | m[entry]);
			}
		}
	}

	table.push_back(opcode_root);
	return "";	// success
}

std::string IntelDecoderTableBuilder::process_decoder_encoding(IntelSyntaxEncoding &enc, class OpcodeTableContainer *opcon, std::string insn_name)
{
	OpcodeTable *root = opcon->root;
	OpcodeTable *table = root;

	U4 opcode1 = enc.opcode1;
	U4 opcode2 = enc.opcode2;	// 0x100 for none
	U4 opcode_3dnow = 0x100;

	size_t opcode_count = 0;
	U1 opcodes[3];

	if(opcode1 < 0x100)
	{
		opcodes[0] = opcode1;
		opcode_count = 1;

		if(opcode2 != 0x100)
		{
			opcodes[1] = opcode2;
			opcode_count = 2;
		}
	}
	else
	if(opcode1 < 0x200)
	{
		opcodes[0] = 0x0f;
		opcodes[1] = opcode1 & 0xff;
		opcode_count = 2;

		if(opcode2 != 0x100)
		{
			opcodes[2] = opcode2;
			opcode_count = 3;
		}
	}
	else
	{
		// 3DNow! Instruction
		opcode_3dnow = opcode2;
		opcode2 = 0x100;
		opcodes[0] = 0x0f;
		opcodes[1] = 0x0f;
		opcodes[2] = opcode1 & 0xff;
	}

	size_t offset;
	for(size_t i = 0; ;)
	{
		if(table->type != 1)
		{
			return std::string("Internal error with ") + insn_name + std::string(", code ") + int_to_string(__LINE__);
		}

		offset = opcodes[i];
		++i;
		if(i == opcode_count)
			break;
		OpcodeTable **ptr = table->get(offset);
		if(*ptr == NULL)
			*ptr = opcon->alloc(1);
		table = *ptr;
	}

	size_t old_offset = offset;
	size_t count = 1;

	if(enc.tags.find(syntax->getSymbol("reg_base")) != enc.tags.end())
	{
		count = 8;
	}

	OpcodeTable *old_table = table;
	for(size_t qqq = 0; qqq < count; ++qqq)
	{
		size_t offset = old_offset + qqq;
		OpcodeTable *table = old_table;

		// Prefix bytes.
		if(enc.op66 == 0 && enc.fx == 1)
		{
			OpcodeTable **ptr = table->get(offset);
			if(*ptr != NULL && (*ptr)->type <= 2)
			{
				return std::string("Internal error with ") + insn_name + std::string(", code ") + int_to_string(__LINE__);
			}

			std::string result = process_decoder_modrm(table, offset, enc, opcon, insn_name);
			if(!result.empty())
				return result;
		}
		else
		{
			OpcodeTable **ptr = table->get(offset);
			if(*ptr == NULL)
				*ptr = opcon->alloc(2);
			else
			if((*ptr)->type != 2)
			{
				return std::string("Internal error with ") + insn_name + std::string(", code ") + int_to_string(__LINE__);
			}
			table = *ptr;	// prefix table
		
			// 66 : (no66, 66)     : 0..1
			// fx : (nofx, f2, f3) : 0..2
			size_t num66 = (enc.op66 == 0) ? 2 : 1;
			size_t start66 = (enc.op66 == 2) ? 1 : 0;
			size_t numfx = (enc.fx == 1) ? 3 : 1;
			size_t startfx = 0;
			if(enc.fx == 2)
				startfx = 1;
			else
			if(enc.fx == 3)
				startfx = 2;

			for(size_t a = 0; a < num66; ++a)
			{
				for(size_t b = 0; b < numfx; ++b)
				{
					offset = (start66 + a) + 2 * (startfx + b);

					std::string result = process_decoder_modrm(table, offset, enc, opcon, insn_name);
					if(!result.empty())
						return result;
				}
			}
		}

		;
	}

	return "";	// success
}

std::string IntelDecoderTableBuilder::process_decoder_modrm(class OpcodeTable *table, size_t offset, IntelSyntaxEncoding &enc, class OpcodeTableContainer *opcon, std::string insn_name)
{
	int extcode;					// -2=/r, -1=none, else 0..7
	if(enc.regop == 8)
		extcode = -2;
	else
	if(enc.regop == 0xf)
		extcode = -1;
	else
		extcode = enc.regop;

	int md;							// -1=default, -2=0..2 claimed, else 0..3
	if(enc.mod == 4)
		md = -2;
	else
	if(enc.mod == 7)
		md = -1;
	else
		md = enc.mod;

	int ext_begin = 0, ext_stop = 0;
	if(extcode >= 0)
	{
		ext_begin = ext_stop = extcode;
	}
	else
	if(extcode == -2)
	{
		ext_begin = 0;
		ext_stop = 7;
	}

	int md_begin = 0, md_stop = 0;
	if(md == -2)
	{
		md_begin = 0;
		md_stop = 2;
	}
	else
	if(md >= 0)
	{
		md_begin = md_stop = md;
	}

	for(int ext_index = ext_begin; ext_index <= ext_stop; ++ext_index)
	{
		for(int md_index = md_begin; md_index <= md_stop; ++md_index)
		{
			std::string result = process_decoder_entry(table, offset, enc, opcon, insn_name, ext_index, md_index);
			if(!result.empty())
				return result;
		}
	}

	return "";
}

std::string IntelDecoderTableBuilder::process_decoder_entry(class OpcodeTable *table, size_t offset, IntelSyntaxEncoding &enc, class OpcodeTableContainer *opcon, std::string insn_name, int ext_index, int md_index, int size_override /*= -1*/)
{
	int extcode;					// -2=/r, -1=none, else 0..7
	if(enc.regop == 8)
		extcode = -2;
	else
	if(enc.regop == 0xf)
		extcode = -1;
	else
		extcode = enc.regop;

	int md;							// -1=default, -2=0..2 claimed, else 0..3
	if(enc.mod == 4)
		md = -2;
	else
	if(enc.mod == 7)
		md = -1;
	else
		md = enc.mod;

	int entry_size = 0;
	if(extcode != -1)
		entry_size = 8;
	if(md != -1)
	{
		if(extcode == -1)
		{
			//std::cout << std::hex << enc.mod << std::dec << std::endl;
			return std::string("Error: An encoding was found with no /0..7 or /r, yet it has a memory type specifier: Instruction ") + insn_name;
		}
		entry_size = 32;
	}
	if(size_override != -1)
		entry_size = size_override;

	if(entry_size == 8)
	{
		for(int md_i = 0; md_i < 4; ++md_i)
		{
			std::string result = process_decoder_entry(table, offset, enc, opcon, insn_name, ext_index, md_i, 32);
			if(!result.empty())
				return result;
		}

		return "";
	}

		OpcodeTable **ptr = table->get(offset);

	if(entry_size == 0)
	{
		if(*ptr != NULL && (*ptr)->type <= 4)
		{
			return std::string("Opcode conflict, line ") + int_to_string(__LINE__) + std::string(", instruction: ") + insn_name;
		}
	}
	else
	{
		// entry_size must be 32
		if(*ptr != NULL && (*ptr)->type != 3)
		{
			return std::string("Opcode conflict, line ") + int_to_string(__LINE__) + std::string(", instruction: ") + insn_name;
		}

		if(*ptr == NULL)
		{
			*ptr = opcon->alloc(3);
		}
		table = *ptr;
		offset = ext_index + 8 * md_index;

		if(size_override == 32 && md_index != 0)
		{
			ptr = table->get(offset);
			if(*ptr != NULL && *ptr != *table->get(ext_index))
			{
				return std::string("Opcode conflict, line ") + int_to_string(__LINE__) + std::string(", instruction: ") + insn_name;
			}
			*ptr = *table->get(ext_index);
			return "";
		}
	}

	int enc_rm;
	if(enc.rm == 0xf)
		enc_rm = -1;
	else
		enc_rm = enc.rm;

	ptr = table->get(offset);
	if(enc_rm != -1)
	{
		if(*ptr != NULL && (*ptr)->type != 5)
		{
			return std::string("Opcode conflict, line ") + int_to_string(__LINE__) + std::string(", instruction: ") + insn_name;
		}

		if(*ptr == NULL)
		{
			*ptr = opcon->alloc(5);
		}
		table = *ptr;
		offset = static_cast<size_t>(enc.rm);
	}
	else
	{
		if(*ptr != NULL && (*ptr)->type <= 5)
		{
			return std::string("Opcode conflict, line ") + int_to_string(__LINE__) + std::string(", instruction: ") + insn_name;
		}
	}

	ptr = table->get(offset);
	if(enc.tags.find(syntax->getSymbol("like_arpl")) != enc.tags.end() || enc.tags.find(syntax->getSymbol("like_movsxd")) != enc.tags.end())
	{
		if(*ptr != NULL && (*ptr)->type != 6)
		{
			return std::string("Opcode conflict, line ") + int_to_string(__LINE__) + std::string(", instruction: ") + insn_name;
		}

		if(*ptr == NULL)
		{
			*ptr = opcon->alloc(6);
		}
		table = *ptr;
		offset = (enc.tags.find(syntax->getSymbol("like_movsxd")) != enc.tags.end()) ? 1 : 0;
	}

	ptr = table->get(offset);
	if(enc.opcode1 >= 0x200)
	{
		// 3DNow! instruction of type that has imm8 opcode after modr/m.
		if(*ptr != NULL && (*ptr)->type != 7)
		{
			return std::string("Opcode conflict, line ") + int_to_string(__LINE__) + std::string(", instruction: ") + insn_name;
		}

		if(*ptr == NULL)
		{
			*ptr = opcon->alloc(7);
		}
		table = *ptr;
		offset = enc.opcode2;
		if(offset < 0 || offset >= 256)
		{
			return std::string("Error, line ") + int_to_string(__LINE__) + std::string(", instruction: ") + insn_name;
		}
	}

	ptr = table->get(offset);

	if(*ptr != NULL)
	{
		return std::string("Duplicate opcode definition, instruction: ") + insn_name;
	}

	*ptr = opcon->alloc(0);
	(*ptr)->encoding_number = enc.encoding_num;

	return "";
}

//===============================================================================================//
// Main program.
//===============================================================================================//

int main(int argc, const char **argv)
{
	if(argc != 2)
	{
		std::cout << "Usage: mkdecoder ../../generated/intel.xml" << std::endl;
		return 1;
	}
	ReadIntelXml reader;
	if(reader.load(argv[1]) != 0)
		return 1;

	IntelDecoderTableBuilder builder(&reader.syntax);
	std::vector<U4> table;
	std::string status = builder.buildTable(table);
	
	if(status != "")
	{
		std::cout << "Error: " << status << std::endl;
		return 1;
	}
	
	if(table.size() < 2)
	{
		std::cout << "Internal error -- no table generated!" << std::endl;
		return 1;
	}
	
	{
		std::ofstream fo("../../generated/out_intel_decoder_table.h");
		if(fo == NULL)
		{
			std::cout << "Error: unable to create file: ../../generated/out_intel_decoder_table.h" << std::endl;
			return 1;
		}
		fo << "// out_intel_decoder_table.h  (note: this file was automatically generated -- do not edit!)\n";
		fo << "// Copyright (C) 2011 Willow Schlanger. All rights reserved.\n";
		fo << "\nU4 crudasm_intel_decoder_table[] = {";
		for(size_t i = 0; i < table.size(); ++i)
		{
			if(i != 0)
				fo << ",";
			if(i % 16 == 0)
			{
				fo << "\n\t";
			}
			if(i == 0)
				fo << "0x" << std::hex << table[table.size() - 1] << std::dec;
			else
				fo << "0x" << std::hex << table[i - 1] << std::dec;
		}
		fo << "\n";
		fo << "};\n";
		fo << std::endl;
	}
	
	std::map<U4, std::string> all_tags_insn;
	{
		std::ofstream fo("../../generated/out_intel_encoding_table.h");
		if(fo == NULL)
		{
			std::cout << "Error: unable to create file: ../../generated/out_intel_encoding_table.h" << std::endl;
			return 1;
		}
		fo << "// out_intel_encoding_table.h  (note: this file was automatically generated -- do not edit!)\n";
		fo << "// Copyright (C) 2011 Willow Schlanger. All rights reserved.\n";
		fo << "\nstruct crudasm_intel_encoding_t crudasm_intel_encoding_table[] = {\n";
		
		std::map<U4, std::string> all_tags;
		for(size_t i = 0; i < reader.syntax.getEncodingCount(); ++i)
		{
			IntelSyntaxEncoding *encoding = reader.syntax.getEncoding(i);
			for(std::set<U4>::iterator j = encoding->tags.begin(); j != encoding->tags.end(); ++j)
			{
				all_tags[*j] = reader.syntax.getSymbol(*j);
			}
		}
		
		for(size_t i = 0; i < reader.syntax.getEncodingCount(); ++i)
		{
			if(i != 0)
				fo << ",\n";
			reader.syntax.getEncoding(i)->out_values(fo, all_tags, &reader.syntax);
		}
		fo << "\n";
		fo << "};\n";
		fo << std::endl;

		std::ofstream fo2("../../generated/out_intel_encoding_info.h");
		if(fo2 == NULL)
		{
			std::cout << "Error: unable to create file: ../../generated/out_intel_encoding_info.h" << std::endl;
			return 1;
		}
		fo2 << "// out_intel_encoding_info.h  (note: this file was automatically generated -- do not edit!)\n";
		fo2 << "// Copyright (C) 2011 Willow Schlanger. All rights reserved.\n";
		
		fo2 << "\n";
		IntelSyntaxEncoding::out_self(fo2, all_tags);
		fo2 << "\n";

		fo2 << "\nextern struct crudasm_intel_encoding_t crudasm_intel_encoding_table[];  /* see out_intel_encoding_table.h */\n";
		fo2 << "\nextern U4 crudasm_intel_decoder_table[];  /* see out_intel_decoder_table.h */\n";
		
		fo2 << "\n";
		IntelArgTypes::dump_argtypes(fo2);
		fo2 << "\n";
		
		fo2 << "\n";
		IntelArgSizes::dump_argsizes(fo2);
		fo2 << "\n";
		
		fo2 << "\n";
		fo2 << "enum {";
		for(size_t i = 0; i < reader.syntax.getInsnCount(); ++i)
		{
			IntelSyntaxInsn *insn = reader.syntax.getInsn(i);
			if(i != 0)
				fo2 << ",";
			fo2 << "\n\tcrudasm_intel_insn_" << reader.syntax.getSymbol(insn->proto.name);
		}
		fo2 << "\n};\n";
		fo2 << "\n";

		for(size_t i = 0; i < reader.syntax.getInsnCount(); ++i)
		{
			IntelSyntaxInsn *insn = reader.syntax.getInsn(i);
			for(std::set<U4>::iterator j = insn->proto.tags.begin(); j != insn->proto.tags.end(); ++j)
			{
				all_tags_insn[*j] = reader.syntax.getSymbol(*j);
			}
		}
		fo2 << "\nstruct crudasm_intel_insn_t {\n";
		fo2 << "\tconst char *name;\n";
		fo2 << "\tconst char *alias;\n";
		fo2 << "\tU1 numArgs;\n";
		fo2 << "\tS1 argSizes[4];\n";
		fo2 << "\n";
		for(std::map<U4, std::string>::iterator k = all_tags_insn.begin(); k != all_tags_insn.end(); ++k)
		{
			fo2 << "\tU1 flag_" << k->second << " : 1;\n";
		}
		fo2 << "};\n\n";
		
		fo2 << std::endl;
	}

	{
		std::ofstream fo("../../generated/out_intel_insns.h");
		if(fo == NULL)
		{
			std::cout << "Error: unable to create file: ../../generated/out_intel_insns.h" << std::endl;
			return 1;
		}
		fo << "// out_intel_insns.h  (note: this file was automatically generated -- do not edit!)\n";
		fo << "// Copyright (C) 2011 Willow Schlanger. All rights reserved.\n";
		fo << "\nstruct crudasm_intel_insn_t crudasm_intel_insns[] = {\n";
		
		for(size_t i = 0; i < reader.syntax.getInsnCount(); ++i)
		{
			IntelSyntaxInsn *insn = reader.syntax.getInsn(i);
			std::string insn_name = reader.syntax.getSymbol(insn->proto.name);
			if(i != 0)
				fo << ",";
			fo << "\n\t/*" << insn_name << "*/  { ";
			fo << "\"" << insn_name << "\", ";
			fo << "\"" << reader.syntax.getSymbol(insn->proto.alias) << "\", ";
			fo << (U4)(U1)insn->proto.numArgs << ", ";
			fo << "{";
			for(int j = 0; j < 4; ++j)
			{
				if(j != 0)
					fo << ", ";
				if(j >= insn->proto.numArgs)
					fo << "0";
				else
					fo << (S4)(S1)insn->proto.argSizes[0];
			}
			fo << "}, ";
			for(std::map<U4, std::string>::iterator k = all_tags_insn.begin(); k != all_tags_insn.end(); ++k)
			{
				if(k != all_tags_insn.begin())
					fo << ",";
				fo << "/*" << k->second << "*/";
				if(insn->proto.tags.find(k->first) == insn->proto.tags.end())
					fo << "0";
				else
					fo << "1";
			}
			fo << " }";
		}
		fo << "\n};\n";
		fo << std::endl;
	}
	return 0;
}


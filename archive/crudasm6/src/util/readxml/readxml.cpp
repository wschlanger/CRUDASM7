// readxml.cpp
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "readxml.h"

std::string IntelSyntaxEncoding::get_insn_name(class IntelSyntax *syn, U4 insn)
{
	return syn->getSymbol(syn->getInsn(insn)->proto.name);
}

struct Tokenizer
{
	public:
	std::vector<std::string> tokens;
	
	void dump()
	{
		for(size_t i = 0; i < tokens.size(); ++i)
		{
			std::cout << "[" << tokens[i] << "]";
		}
		std::cout << std::endl;
	}
	
	Tokenizer(std::string s)
	{
		s += " ";
		std::string tmp;
		for(size_t i = 0; i < s.size(); ++i)
		{
			if(s[i] == '\'' || s[i] == '\"')
			{
				if(!tmp.empty())
					tokens.push_back(tmp);
				tmp.clear();
				int j = i;
				for(++i; i != s.size() && s[j] != s[i] && s[i] != '\0'; ++i)
					;
				if(i == s.size() || s[i] == '\0')
					throw -1;
				++i;
				tokens.push_back(std::string(s.begin() + j, s.begin() + i));
				--i;
			}
			else
			if(isspace(s[i]) || s[i] == '<' || s[i] == '>' || s[i] == '/' || s[i] == '=')
			{
				if(!tmp.empty())
					tokens.push_back(tmp);
				tmp.clear();
				if(!isspace(s[i]))
				{
					tmp += s[i];
					tokens.push_back(tmp);
					tmp.clear();
				}
			}
			else
			{
				tmp += s[i];
			}
		}
	}
};

class XmlReader
{
	std::vector<std::string> stk;
	std::vector<XmlElement *> nodes;
	XmlElement *current;
	public:
	XmlElement root;
	
	XmlReader()
	{
		root.name = NULL;
		nodes.push_back(&root);
		current = &root;
	}
	
	void accept(std::string token)
	{
		if(token == ">")
		{
			process_stack();
			stk.clear();
		}
		else
		if(token == "<")
		{
			if(!stk.empty())
			{
				throw __LINE__;
			}
		}
		else
			stk.push_back(token);
	}
	
	void process_stack()
	{
		if(!stk.empty())
		{
			if(stk[0] == "?xml")
				return;
			size_t count = stk.size();
			if(stk[count - 1] == "/")
			{
				if(stk.size() < 2)
					throw __LINE__;
				open_tag(stk, count - 1);
				close_tag(stk[0]);
			}
			else
			if(stk[0] == "/")
			{
				if(stk.size() != 2)
					throw __LINE__;
				close_tag(stk[1]);
			}
			else
			{
				open_tag(stk, count);
			}
		}
	}
	
	void open_tag(std::vector<std::string> &s, size_t count)
	{
		std::string tag = s[0];
		--count;
		if((count % 3) != 0)
			throw __LINE__;
		size_t attrs = count / 3;
		
		current->children.push_back(XmlElement());
		XmlElement &e = current->children.back();
		current->child.push_back(&e);
		
		std::map<std::string, XmlElement *>::iterator i = current->names.find(tag);
		if(i == current->names.end())
		{
			current->names[tag] = &e;
			i = current->names.find(tag);
		}
		e.name = &i->first;
		
		current = &e;
		nodes.push_back(current);
		
		for(size_t a = 0; a < attrs; ++a)
		{
			std::string k = s[3 * a + 1];
			std::string v = s[3 * a + 3];
			if(current->attributes.find(k) != current->attributes.end())
				throw __LINE__;
			if(v[0] == '\"' || v[0] == '\'' && v.size() >= 2)
				v = std::string(v.begin() + 1, v.end() - 1);
			current->attributes[k] = v;
		}
	}
	
	void close_tag(std::string tag)
	{
		if(nodes.size() < 2)
			throw __LINE__;
		if(nodes.back()->name == NULL)
			throw __LINE__;
		if(*nodes.back()->name != tag)
			throw __LINE__;
		nodes.pop_back();
		current = nodes.back();
	}

	void finish()
	{
		if(nodes.size() != 1)
			throw __LINE__;
	}
};

//===============================================================================================//

// B1 -> 1
// B2 -> 2
// ...
// other -> -1
int getBSize(std::string s)
{
	if(s.size() < 2)
		return -1;
	if(s[0] != 'B')
		return -1;
	int x = 0;
	for(std::string::iterator i = ++s.begin(); i != s.end(); ++i)
	{
		if(*i >= '0' && *i <= '9')
		{
			x = (x * 10) + (*i - '0');
		}
		else
			return -1;
	}
	return x;
}

std::string IntelSyntax::getSymbol(U4 num)
{
	if(num >= strTabSymbols.size())
		return "";	// bad input number
	return *strTabSymbols[num];
}

U4 IntelSyntax::getSymbol(std::string symbol)
{
	std::map<std::string, U4>::iterator i = strTabNums.find(symbol);
	if(i != strTabNums.end())
	{
		return i->second;
	}
	U4 count = strTabSymbols.size();
	strTabNums[symbol] = count;
	strTabSymbols.push_back(&strTabNums.find(symbol)->first);
	return count;
}

std::string IntelSyntax::load(XmlReader &intel_xml)
{
	if(intel_xml.root.child.empty())
		return "Empty XML";
	if(*intel_xml.root.child[0]->name != "intel")
		return "Invalid XML: root tag is not 'intel'!";
	
	// This is version 1.0 of IntelSyntax. Anything else must be a newer version.
	if(intel_xml.root.child[0]->attr("minver") != "1.0")
		return std::string("Minimum version required exceeds current version (1.0): ") + intel_xml.root.child[0]->attr("minver");
	
	XmlElement *intel = intel_xml.root.child[0];	// intel tag
	XmlElement *insns = intel->tag("insns");
	if(insns == NULL)
		return "Invalid XML: Missing tag: insns";
	
	for(size_t insnI = 0; insnI < insns->child.size(); ++insnI)
	{
		XmlElement *insn = insns->child[insnI];
		std::string name = insn->attr("name");
		std::string alias = insn->attr("alias");
		std::string numArgsStr = insn->attr("numargs");

		long numArgs = strtol(numArgsStr.c_str(), NULL, 10);
		if(numArgs < 0 || numArgs > 4)
			return std::string("Invalid XML: Instruction ") + name + std::string(": bad number of arguments '") + numArgsStr + std::string("'!");

		U4 insnNameNum = getSymbol(name);
		if(insnNames.find(insnNameNum) != insnNames.end())
		{
			return std::string("Invalid XML: Instruction ") + name + std::string(": duplicate instruction!");
		}
		
		insnList.push_back(IntelSyntaxInsn());
		IntelSyntaxInsn *dest = &insnList.back();
		U4 insnNumber = insnsX.size();
		dest->insn_num = insnNumber;
		insnsX.push_back(dest);
		insnNames[insnNameNum] = insnNumber;
		dest->proto.name = getSymbol(name);
		dest->proto.alias = getSymbol(alias);
		dest->proto.numArgs = numArgs;

		// Get arg sizes.
		const char *argN[4] = {"arg0", "arg1", "arg2", "arg3"};
		std::map<std::string, S1> templNums;
		S1 nextTemplateNum = -1;
		for(int a = 0; a < 4; ++a)
		{
			std::string argS = argN[a];
			std::string size = insn->attr(argS);
			std::string sizeS = size;
			if(size == "void")
				dest->proto.argSizes[a] = 0;
			int sizeI = getBSize(sizeS);
			if(sizeI != -1)
			{
				if(sizeI > 0 && sizeI <= 127)
					dest->proto.argSizes[a] = sizeI;
				else
					return std::string("Invalid XML: Instruction ") + name + std::string(": unsupported argument size: ") + size;
			}
			else
			{
				if(templNums.find(sizeS) != templNums.end())
				{
					dest->proto.argSizes[a] = templNums[sizeS];
				}
				else
				{
					dest->proto.argSizes[a] = nextTemplateNum;
					templNums[sizeS] = nextTemplateNum;
					--nextTemplateNum;
				}
			}
		}
		
		// Get tags.
		for(std::map<std::string, std::string>::iterator i = insn->attributes.begin();
			i != insn->attributes.end();
			++i
		)
		{
			if(i->second == "true")
			{
				std::string nameS = i->first;
				dest->proto.tags.insert(getSymbol(nameS));
			}
		}
		
		// Go through child tags.
		bool foundEncodings = false;
		for(size_t childI = 0; childI < insn->child.size(); ++childI)
		{
			XmlElement *mm = insn->child[childI];
			if(*mm->name == "dis")
			{
				std::string dis_type = mm->attr("type");
				if(dest->disasm.find(getSymbol(dis_type)) != dest->disasm.end())
				{
					return std::string("Invalid XML: Instruction ") + name + std::string(": repeat disassembly type: ") + dis_type;
				}
				std::list<IntelSyntaxDisStmt> &ilst = dest->disasm[getSymbol(dis_type)];
				
				// Go through all child <dis>...</dis> tags.
				for(size_t childJ = 0; childJ < mm->child.size(); ++childJ)
				{
					XmlElement *nn = mm->child[childJ];
					if(*nn->name == "dis_stmt")
					{
						std::string dis_stmt_ident = nn->attr("ident");
						ilst.push_back(IntelSyntaxDisStmt());

						std::string ident = dis_stmt_ident;
						ilst.back().ident = getSymbol(ident);
						int num_args = 0;
						
						for(size_t childK = 0; childK < nn->child.size(); ++childK)
						{
							XmlElement *qq = nn->child[childK];
							
							if(*qq->name == "dis_arg")
							{
								++num_args;
								ilst.back().args.push_back(qq->attr("value"));
								std::string hexval = ilst.back().args.back();
								// write_arg() has some arguments that can be booleans.
								if(hexval == "true")
									ilst.back().args.back() = "1";
								else
								if(hexval == "false")
									ilst.back().args.back() = "0";
							}
							else
							if(*qq->name == "dis_arg_str")
							{
								++num_args;
								ilst.back().args.push_back(std::string("\"") + qq->attr("value") + std::string("\""));
							}
						}

						if(!isValidDisasmIdent(ident, num_args))
						{
							return std::string("Invalid XML: Instruction ") + name + std::string(": bad disassembly method call: ") + std::string(ident.c_str());
						}
					}
				}
			}
			else
			if(*mm->name == "encodings")
			{
				// Now go through all child <encoding.../> tags.
				
				for(size_t childI = 0; childI < mm->child.size(); ++childI)
				{
					XmlElement *nn = mm->child[childI];
					if(*nn->name == "encoding")
					{
						foundEncodings = true;

						dest->encodings.push_back(IntelSyntaxEncoding());
						IntelSyntaxEncoding *enc = &dest->encodings.back();
						enc->encoding_num = encodings.size();
						encodings.push_back(enc);

						std::string opcode1 = nn->attr("opcode1");
						std::string opcode2 = nn->attr("opcode2");
						std::string mod = nn->attr("mod");
						std::string regop = nn->attr("regop");
						std::string rm = nn->attr("rm");
						std::string op66 = nn->attr("op66");
						std::string fx = nn->attr("fx");

						long tmp;

						tmp = strtol(opcode1.c_str(), NULL, 16);
						if(tmp < 0 || tmp > 0x2ff)
							return std::string("Invalid XML: Instruction ") + name + std::string(": invalid opcode1: ") + opcode1;
						enc->opcode1 = tmp;
						
						if(opcode2 != "")
						{
							tmp = strtol(opcode2.c_str(), NULL, 16);
							if(tmp < 0 || tmp > 0xff)
								return std::string("Invalid XML: Instruction ") + name + std::string(": invalid opcode2: ") + opcode2;
							enc->opcode2 = tmp;
						}
						
						if(mod != "")
						{
							if(mod == "m")
								enc->mod = 4;
							else
							{
								tmp = strtol(mod.c_str(), NULL, 10);
								if(tmp < 0 || tmp > 3)
									return std::string("Invalid XML: Instruction ") + name + std::string(": invalid mod: ") + mod;
								enc->mod = tmp;
							}
						}

						if(regop != "")
						{
							if(regop == "r")
								enc->regop = 8;
							else
							{
								tmp = strtol(regop.c_str(), NULL, 10);
								if(tmp < 0 || tmp > 7)
									return std::string("Invalid XML: Instruction ") + name + std::string(": invalid regop: ") + regop;
								enc->regop = tmp;
							}
						}

						if(rm != "")
						{
							tmp = strtol(rm.c_str(), NULL, 10);
							if(tmp < 0 || tmp > 7)
								return std::string("Invalid XML: Instruction ") + name + std::string(": invalid rm: ") + rm;
							enc->rm = tmp;
						}

						enc->insn = insnNumber;

						if(op66 != "")
						{
							if(op66 == "no66")
								enc->op66 = 1;
							else
							if(op66 == "66")
								enc->op66 = 2;
							else
								return std::string("Invalid XML: Instruction ") + name + std::string(": invalid op66: ") + op66;
						}

						if(fx != "")
						{
							if(fx == "nofx")
								enc->fx = 0;
							else
							if(fx == "f2")
								enc->fx = 2;
							else
							if(fx == "f3")
								enc->fx = 3;
							else
								return std::string("Invalid XML: Instruction ") + name + std::string(": invalid fx: ") + fx;
						}

						// Get tags.
						for(std::map<std::string, std::string>::iterator attrI = nn->attributes.begin();
							attrI != nn->attributes.end();
							++attrI
						)
						{
							if(attrI->second == "true")
							{
								std::string nameS = attrI->first;
								enc->tags.insert(getSymbol(nameS));
							}
						}
						
						// Get arguments here (arg0..arg3).
						for(int i = 0; i < numArgs; ++i)
						{
							std::string tsv = nn->attr(argN[i]);
							if(tsv == "" || tsv == "void")
							{
								return std::string("Invalid XML: Instruction ") + name + std::string(": bad or missing argument type/size/value");
							}
							std::string tmp = tsv;
							std::string tsvT[3];
							int count = 0;
							tmp += ":";
							std::string tmp2;
							for(std::string::iterator ii = tmp.begin(); ii != tmp.end(); ++ii)
							{
								if(*ii == ':')
								{
									if(count >= 3 || tmp2 == "")
										return std::string("Invalid XML: Instruction ") + name + std::string(": bad or missing argument type/size/value");
									tsvT[count] = tmp2;
									++count;
									tmp2 = "";
								}
								else
									tmp2 += *ii;
							}
							if(count != 3)
								return std::string("Invalid XML: Instruction ") + name + std::string(": bad or missing argument type/size/value");
							// tsvT[0] = type, tsvT[1] = size, tsvT[2] = value.

							enc->argtype[i] = IntelArgTypes::getType(tsvT[0]);
							if(enc->argtype[i] == IntelArgTypes::Tvoid)
								return std::string("Invalid XML: Instruction ") + name + std::string(": unsupported argument type: ") + std::string(tsvT[0].c_str());

							enc->argsize[i] = IntelArgSizes::getSize(tsvT[1]);
							if(enc->argsize[i] == IntelArgSizes::Tvoid)
								return std::string("Invalid XML: Instruction ") + name + std::string(": unsupported argument size: ") + std::string(tsvT[1].c_str());

							std::string argvalue = tsvT[2].c_str();
							long argvalueNum = strtol(argvalue.c_str(), NULL, 16);
							if(argvalueNum < 0 || argvalueNum > 0xff)
								return std::string("Invalid XML: Instruction ") + name + std::string(": unsupported argument value: ") + std::string(tsvT[2].c_str());
							enc->argvalue[i] = argvalueNum;

							if(argvalueNum == 0xff && ((enc->argtype[i] & 0x80) == 0) && IntelArgTypes::is_reg(enc->argtype[i]))
							{
								// OK, we got a reg argument that is non-implict. It is not /mem.
								///std::cout << std::hex << qPrintable(opcode1) << " " << (unsigned)enc->argtype[i] << " " << (unsigned)argvalueNum << " " << qPrintable(regop) << std::dec << std::endl;
								if(regop != "r" && enc->tags.find(getSymbol("reg_rm")) == enc->tags.end() && enc->tags.find(getSymbol("reg_base")) == enc->tags.end())
									return std::string("Invalid XML: Instruction ") + name + std::string(": don\'t know where to get register (use reg_rm or reg_base, or /r)");
							}
						}
					}
				}
			}
			else
			if(*mm->name == "emu")
			{
				// Add 'mm' to a list.
				dest->emulst.push_back(mm);
			}
		}	// end for

		if(!foundEncodings)
			return std::string("Invalid XML: Instruction ") + name + std::string(": contains no encodings!");
	}
	
	return "";		// success
}

ReadIntelXml::ReadIntelXml()
{
	readerT = new XmlReader();
}

ReadIntelXml::~ReadIntelXml()
{
	delete readerT;
}

int ReadIntelXml::load(const char *fn)
{
	XmlReader &reader = *readerT;
	std::ifstream fi(fn);
	if(fi == NULL)
	{
		std::cout << "Error: unable to open file: " << fn << std::endl;
		return 1;
	}
	std::string ln;
	int linenum = 0;
	while(std::getline(fi, ln))
	{
		++linenum;
		try
		{
			Tokenizer tokens(ln);
			for(size_t i = 0; i < tokens.tokens.size(); ++i)
				reader.accept(tokens.tokens[i]);
		}
		catch(int c)
		{
			std::cout << "Error: line " << linenum << " (code " << c << ")" << std::endl;
			return 1;
		}
	}
	try
	{
		reader.finish();
	
		std::string status = syntax.load(reader);
		if(status != "")
		{
			std::cout << "Error: " << status << std::endl;
			throw 1;
		}
	}
	catch(int c)
	{
		std::cout << "Error: code " << c << std::endl;
		return 1;
	}
	
	std::cout << "Processing " << syntax.getInsnCount() << " instructions..." << std::endl;
	return 0;
}

//===============================================================================================//
// Begin test program.
//===============================================================================================//

#if 0
int main(int argc, const char **argv)
{
	if(argc != 2)
	{
		std::cout << "Usage: readxml ../../generated/intel.xml" << std::endl;
		return 1;
	}
	std::ifstream fi(argv[1]);
	if(fi == NULL)
	{
		std::cout << "Error: unable to open file: " << argv[1] << std::endl;
		return 1;
	}
	std::string ln;
	int linenum = 0;
	XmlReader reader;
	while(std::getline(fi, ln))
	{
		++linenum;
		try
		{
			Tokenizer tokens(ln);
			for(size_t i = 0; i < tokens.tokens.size(); ++i)
				reader.accept(tokens.tokens[i]);
		}
		catch(int c)
		{
			std::cout << "Error: line " << linenum << " (code " << c << ")" << std::endl;
			return 1;
		}
	}
	try
	{
		reader.finish();
		
		IntelSyntax syntax;
		std::string status = syntax.load(reader);
		if(status != "")
		{
			std::cout << "Error: " << status << std::endl;
			throw 1;
		}
	}
	catch(int c)
	{
		std::cout << "Error: code " << c << std::endl;
		return 1;
	}
	return 0;
}
#endif


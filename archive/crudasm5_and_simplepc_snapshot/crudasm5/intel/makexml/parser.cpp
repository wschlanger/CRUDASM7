// parser.cpp
// Copyright (C) 2011 Willow Schlanger

#include "parser.h"
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <cstdio>
#include <stddef.h>

std::string int_to_string(int x)
{
	char s[33];
	using namespace std;
	sprintf(s, "%d", x);
	return std::string(s);
}

void shouldBe(ParseNode *node, unsigned token, const char *tokenStr)
{
	if(node->getToken() != token)
	{
		throw ParseSpecificError(node->getLine(), node->getFile(), std::string("Internal Error: Expected token: ") + tokenStr);
	}
}
#define SHOULDBE(x, y) shouldBe(x, RULE_ix##y, #y)

int getReg8(std::string s)
{
	const char *regs8[] = {"AL", "CL", "DL", "BL", "SPL", "BPL", "SIL", "DIL", "R8B", "R9B", "R10B", "R11B", "R12B", "R13B", "R14B", "R15B", "AH", "CH", "DH", "BH", NULL};
	for(int i = 0; regs8[i] != NULL; ++i)
	{
		if(s == std::string(regs8[i]))
			return i;
	}
	return -1;
}

int getReg16(std::string s)
{
	const char *regs16[] = {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI", "R8W", "R9W", "R10W", "R11W", "R12W", "R13W", "R14W", "R15W", NULL};
	for(int i = 0; regs16[i] != NULL; ++i)
	{
		if(s == std::string(regs16[i]))
			return i;
	}
	return -1;
}

int getReg32(std::string s)
{
	const char *regs32[] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI", "R8D", "R9D", "R10D", "R11D", "R12D", "R13D", "R14D", "R15D", NULL};
	for(int i = 0; regs32[i] != NULL; ++i)
	{
		if(s == std::string(regs32[i]))
			return i;
	}
	return -1;
}

int getReg64(std::string s)
{
	const char *regs64[] = {"RAX", "RCX", "RDX", "RBX", "RSP", "RBP", "RSI", "RDI", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", NULL};
	for(int i = 0; regs64[i] != NULL; ++i)
	{
		if(s == std::string(regs64[i]))
			return i;
	}
	return -1;
}

int getSegReg(std::string s)
{
	const char *regs[] = {"ES", "CS", "SS", "DS", "FS", "GS", NULL};
	for(int i = 0; regs[i] != NULL; ++i)
	{
		if(s == std::string(regs[i]))
			return i;
	}
	return -1;
}

int getFlag(std::string s)
{
	if(s == "CF")  return 0;
	if(s == "PF")  return 2;
	if(s == "AF")  return 4;
	if(s == "ZF")  return 6;
	if(s == "SF")  return 7;
	if(s == "TF")  return 8;
	if(s == "IF")  return 9;
	if(s == "DF")  return 10;
	if(s == "OF")  return 11;
	if(s == "IOPL0")  return 12;
	if(s == "IOPL1")  return 13;
	if(s == "NT")  return 14;
	if(s == "RF")  return 16;
	if(s == "VM")  return 17;
	if(s == "AC")  return 18;
	if(s == "VIF")  return 19;
	if(s == "VIP")  return 20;
	if(s == "ID")  return 21;
	return -1;
}

std::string getExternalSizeI(std::string s, ParseNode *ident);

std::string getExternalSize(ParseNode *ident)
{
	return getExternalSizeI(ident->getTokenText(), ident);
}

std::string getExternalSizeI(std::string s, ParseNode *ident)
{
	if(getReg8(s) != -1)
		return "B1";
	if(getReg16(s) != -1)
		return "B2";
	if(getReg32(s) != -1)
		return "B4";
	if(getReg64(s) != -1)
		return "B8";
	if(getFlag(s) != -1)
		return "BIT";
	if(s == "undefined")
		return "BIT";
	if(getSegReg(s) != -1)
		return "B2";

	throw ParseSpecificError(ident->getLine(), ident->getFile(), std::string("Unrecognized external identifier: ") + s);
}

void getTerminals(ParseNode *node, std::list<ParseNode *> &lst)
{
	for(ParseNode::iterator_type i = node->begin(); i != node->end(); ++i)
	{
		if((*i)->size() != 0)
			getTerminals(*i, lst);
		else
			lst.push_back(*i);
	}
}

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

// Returns 0 if invalid.
//	-1      : osz
//	-2      : asz
//	-3      : osz_ptr   : 64 bits in 64 bit mode regardless of rex; else 32 bits
//	-4      : osz_max32 : 16 or 32 bits only (ignore rex.w for this argument)
//	-5      : osz_min32 : 32 or 64 bits only (if o16, use 32 bits)
//	-6      : osz_min64 : 64 bits (128 bits if rex.w is used)
//	-7      : osz_seg   : 16 bits unless rex.w is used, then 64 bits (mov dest,sreg [intel manual])
//	-8      : osz_64in64: osz, but ALWAYS 64 bits in 64 bit mode
int getSpecialSize(std::string s)
{
	if(s == "osz")
		return -1;
	if(s == "asz")
		return -2;
	if(s == "osz_ptr")
		return -3;
	if(s == "osz_max32")
		return -4;
	if(s == "osz_min32")
		return -5;
	if(s == "osz_min64")
		return -6;
	if(s == "osz_seg")
		return -7;
	if(s == "osz_64in64")
		return -8;
	return 0;
}

bool is_prefix(std::string s)
{
	if(s == "f2" || s == "f3" || s == "nofx")
		return true;
	if(s == "66" || s == "no66")
		return true;
	return false;
}

bool is_valid_hex(std::string s)
{
	if(s.empty() || s.size() > 2)
		return false;
	for(size_t i = 0; i < s.size(); ++i)
	{
		char c = s[i];
		if((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9'))
			return true;
	}
	return false;
}

bool is_valid_arg_type(std::string s)
{
	if(s == "imm_implict")  return true;
	if(s == "imm_1st")  return true;
	if(s == "imm_2nd")  return true;
	if(s == "imm_both")  return true;
	if(s == "mem_implict_sts")  return true;
	if(s == "mem_implict_std")  return true;
	if(s == "mem_implict_xls")  return true;
	if(s == "mem_fulldisp")  return true;
	if(s == "mem_ea")  return true;
	if(s == "mem_ea_lim")  return true;
	if(s == "mem_ea_dbl")  return true;
	if(s == "mem_ea_seg")  return true;
	if(s == "mem_ea_eal")  return true;
	if(s == "mem_ea_eai")  return true;
	if(s == "mem_ea_fxs")  return true;

	// reg_x/mem
	if(s.size() < 5)
		return false;
	if(s[0] != 'r' || s[1] != 'e' || s[2] != 'g' || s[3] != '_')
		return false;
	size_t len = s.size();
	if(s[len-4] == '/' && s[len-3] == 'm' && s[len-2] == 'e' && s[len-1] == 'm')
		s = std::string(s.begin(), s.end() - 4);	// ends with "/mem", starts with "reg_".

	if(s == "reg_gr")  return true;
	if(s == "reg_xmm")  return true;
	if(s == "reg_mmx")  return true;
	if(s == "reg_sr")  return true;
	if(s == "reg_dr")  return true;
	if(s == "reg_cr")  return true;
	if(s == "reg_st")  return true;
	return false;
}

struct Tokens
{
	std::vector<std::string> tokens;
	Tokens(std::string s)
	{
		s += " ";
		std::string tmp;
		for(std::string::iterator i = s.begin(); i != s.end(); ++i)
		{
			if(std::isspace(*i))
			{
				tokens.push_back(tmp);
				tmp.clear();
			}
			else
				tmp += *i;
		}
	}
};

struct CharTokenizer
{
	std::vector<std::string> tokens;
	CharTokenizer(std::string s, char c)
	{
		s += c;
		std::string tmp;
		for(std::string::iterator i = s.begin(); i != s.end(); ++i)
		{
			if(*i == c)
			{
				tokens.push_back(tmp);
				tmp.clear();
			}
			else
				tmp += *i;
		}
	}
};

struct Prototype
{
	std::string ident;
	std::vector<std::string> argtypes;
	std::vector<std::string> argnames;
	std::map<std::string, size_t> arg;

	Prototype(ParseNode *node)
	{
		SHOULDBE(node, prototype);
		ParseNode::iterator_type i = node->begin();
		ident = (*i)->getTokenText();
		++i;	// skip IDENT
		++i;	// skip '('
		if((*i)->getToken() == RULE_ixargs)
		{
			size_t argNum = 0;
			for(ParseNode::iterator_type j = (*i)->begin(); j != (*i)->end();)
			{
				SHOULDBE(*j, arg);
				ParseNode::iterator_type k = (*j)->begin();
				std::string type = (*k)->getTokenText();
				++k;
				std::string name = (*k)->getTokenText();
				argtypes.push_back(type);
				argnames.push_back(name);
				if(argNum >= 4)
				{
					throw ParseSpecificError(node->getLine(), node->getFile(), "Prototype has too many arguments (max 4)!");
				}
				if(arg.find(name) != arg.end())
				{
					throw ParseSpecificError(node->getLine(), node->getFile(), "Prototype has duplicate argument: " + name);
				}
				arg[name] = argNum;
				++argNum;

				++j;	// skip arg
				if(j == (*i)->end())
					break;
				++j;	// skip ','
			}
		}
	}
};

struct RhsNode
{
	char type;			// '(' for fcn, '@' for pure identifier, '#' for number, 's'=sx, 'z'=zx, 't'=tr, 'o'=sizeof
	std::string ident;	// rhs' identifier or number
	std::string size;
	std::vector<RhsNode *> children;	// sx, zx, tr have only one element. '(' can have 0 or more elements.
};

class ParseEmu
{
	ParseNode *special;
	std::ostream &os;
	Prototype &proto;
	int numTemps;
	std::vector<std::string> tempNames;
	std::vector<std::string> tempTypes;
	std::map<std::string, size_t> temps;
public:
	ParseEmu(ParseNode *specialT, std::ostream &osT, Prototype &protoT) :
		special(specialT),
		os(osT),
		proto(protoT)
	{
	}

	void parse()
	{
		SHOULDBE(special, special);
		bool osz = false, asz = false, ssz = false;
		determineSensitivity(osz, asz, ssz, special);
		int oszCount = (osz) ? 3 : 1;
		int aszCount = (asz) ? 3 : 1;
		int sszCount = (ssz) ? 3 : 1;

		for(int o = 0; o < oszCount; ++o)
		{
			for(int a = 0; a < aszCount; ++a)
			{
				for(int s = 0; s < sszCount; ++s)
				{
					os << "\t<emu";
					os << " osz=\"" << (osz ? int_to_string(16 << o) : "any") << "\"";
					os << " asz=\"" << (asz ? int_to_string(16 << a) : "any") << "\"";
					os << " ssz=\"" << (ssz ? int_to_string(16 << s) : "any") << "\"";
					os << ">\n";

					numTemps = 0;
					tempNames.clear();
					tempTypes.clear();
					temps.clear();
					doParse(o, a, s, true);		// allocate temporaries
					doParse(o, a, s, false);	// translate code

					os << "\t</emu>\n";
				}
			}
		}
	}

	void doParse(int o, int a, int s, bool allocMode)
	{
		std::list<ParseNode *> nodes;

		std::list<ParseNode *>::iterator i = special->begin();
		++i;	// skip "emu"
		++i;	// skip '{'
		while((*i)->getToken() == RULE_ixemu_stmt)
		{
			ParseNode *emu_stmt = *i;
			std::list<ParseNode *>::iterator j = emu_stmt->begin();

			bool skip = false;
			
			while((*j)->getToken() == RULE_ixprefix2)
			{
				ParseNode *prefix2 = *j;

				std::string pre = (*prefix2->begin())->getTokenText();

				if(pre == "o16" && o != 0)  skip = true;  else
				if(pre == "o32" && o != 1)  skip = true;  else
				if(pre == "o64" && o != 2)  skip = true;

				if(pre == "a16" && a != 0)  skip = true;  else
				if(pre == "a32" && a != 1)  skip = true;  else
				if(pre == "a64" && a != 2)  skip = true;

				if(pre == "s16" && s != 0)  skip = true;  else
				if(pre == "s32" && s != 1)  skip = true;  else
				if(pre == "s64" && s != 2)  skip = true;

				if(skip)
					break;

				++j;
			}

			if(!skip)
			{
				nodes.push_back(emu_stmt);
			}

			++i;
		}

		if(allocMode)
			os << "\t\t<allocs>\n";
		else
			os << "\t\t<stmts>\n";

		// We now have a list of relevant nodes.
		for(std::list<ParseNode *>::iterator j = nodes.begin(); j != nodes.end(); ++j)
		{
			bool alloc = false, asgn = false;

			ParseNode *emu_stmt = *j;
			std::list<ParseNode *>::iterator k = emu_stmt->begin();
			while((*k)->getToken() == RULE_ixprefix2)
			{
				++k;
			}

			std::string tempType, tempName;

			if((*k)->getToken() == TOKEN_ixIDENT)
			{
				tempType = (*k)->getTokenText();
				std::list<ParseNode *>::iterator k2 = k;
				++k2;
				if((*k2)->getToken() == TOKEN_ixIDENT)
				{
					tempName = (*k2)->getTokenText();
					alloc = true;
					++k2;
					if((*k2)->getToken() == '=')
					{
						asgn = true;
					}
				}
				else
				if((*k2)->getToken() == '=')
				{
					asgn = true;
				}
			}

			if(alloc && !asgn && !allocMode)
				continue;	// pure allocation, but not in alloc mode

			if(allocMode)
			{
				if(alloc)
				{
					tempNames.push_back(tempName);
					tempTypes.push_back(tempType);
					temps[tempName] = numTemps;

					for(size_t u = 0; u < proto.argnames.size(); ++u)
					{
						if(tempName == proto.argnames[u])
						{
							ParseNode *node = *k;
							throw ParseSpecificError(node->getLine(), node->getFile(), "Declared temporary variable has same name as an argument: " + tempName);
						}
					}

					os << "\t\t\t<alloc_temp num=\"" << numTemps << "\"";
					if(tempType == "BIT")
						os << " size=\"BIT\"";
					else
					if(getBSize(tempType) != -1)
						os << " size=\"" << tempType << "\"";
					else
						os << " size=\"" << getArgType(tempType, *k) << "\"";	// size="0" means the size is determined by the first argument
					os << " />\n";

					++numTemps;
				}
			}
			else
			{
				if(asgn)
				{
					if(alloc)
						++k;
					// 'k' now points to the destination IDENT.
					std::string destName = (*k)->getTokenText();
					++k;	// skip IDENT
					++k;	// skip '='
					ParseNode *rhs = *k;

					os << "\t\t\t<asgn>\n";
					std::string size1 = outIdent(destName, rhs /* node for error reporting */);
					std::string size2 = outRhs(rhs, size1);
					os << "\t\t\t</asgn>\n";

					if(size1 != size2)
					{
						ParseNode *node = rhs;
						throw ParseSpecificError(node->getLine(), node->getFile(), "Left side of assignment has size '" + size1 + "' but right side has size '" + size2 + "'!");
					}
				}
				else
				{
					std::string ident = (*k)->getTokenText();
					os << "\t\t\t<" << ident << ">\n";
					while((*k)->getToken() != ')')
					{
						if((*k)->getToken() == RULE_ixrhs)
						{
							outRhs(*k, "");
						}

						++k;
					}
					os << "\t\t\t</" << ident << ">\n";
				}
			}
		}

		if(allocMode)
			os << "\t\t</allocs>\n";
		else
			os << "\t\t</stmts>\n";
	}

	std::string outIdent(std::string ident, ParseNode *errNode)
	{
		for(size_t u = 0; u < proto.argnames.size(); ++u)
		{
			if(ident == proto.argnames[u])
			{
				os << "\t\t\t\t<arg num=\"" << u << "\"";
				std::string sz;
				if(getBSize(proto.argtypes[u]) != -1)
					sz = proto.argtypes[u];
				else
					sz = int_to_string(getArgType(proto.argtypes[u], errNode));
				os << " size=\"" << sz << "\"";
				os << " />\n";
				return sz;
			}
		}
		if(temps.find(ident) != temps.end())
		{
			os << "\t\t\t\t<temp num=\"" << temps[ident] << "\"";
			std::string tempType = tempTypes[temps[ident]];
			std::string sz;
			if(tempType == "BIT")
				sz = "BIT";
			else
			if(getBSize(tempType) != -1)
				sz = tempType;
			else
				sz = int_to_string(getArgType(tempType, errNode));	// size="0" means the size is determined by the first argument
			os << " size=\"" << sz << "\"";
			os << " />\n";
			return sz;
		}
		std::string sz = getExternalSizeI(ident, errNode);
		os << "\t\t\t\t<extern name=\"" << ident << "\" size=\"" << sz << "\" />\n";
		return sz;
	}

	// Returns size of the rhs.
	std::string outRhs(ParseNode *rhs, std::string numSize = "")
	{
		std::map<ParseNode *, RhsNode> rhsmap;
		getRhs(rhs, rhsmap);

		if(!numSize.empty() && rhsmap[rhs].size == "B16" && rhsmap[rhs].type == '#')
		{
			rhsmap[rhs].size = numSize;
		}

		doOutRhs(rhs, &rhsmap[rhs], rhsmap);

		return rhsmap[rhs].size;
	}

	void doOutRhs(ParseNode *errNode, RhsNode *rhs, std::map<ParseNode *, RhsNode> &dest, int level = 0)
	{
		for(int i = 0; i < level; ++i)
			os << "\t";

		if(rhs->type == '@')
		{
			outIdent(rhs->ident, errNode);
			return;
		}

		os << "\t\t\t\t<rhs size=\"" << rhs->size << "\" value=\"" << rhs->ident << "\"";

		// '(' for fcn, '@' for pure identifier, '#' for number, 's'=sx, 'z'=zx, 't'=tr, 'o'=sizeof
		if(rhs->type == '(')
			os << " type=\"fcn\"";
		else
		if(rhs->type == '#')
			os << " type=\"num\"";
		else
		if(rhs->type == 's')
			os << " type=\"sx\"";
		else
		if(rhs->type == 'z')
			os << " type=\"zx\"";
		else
		if(rhs->type == 't')
			os << " type=\"tr\"";
		else
		if(rhs->type == 'o')
			os << " type=\"sizeof\"";

		if(rhs->children.empty())
			os << " />\n";
		else
		{
			os << ">\n";

			for(size_t k = 0; k < rhs->children.size(); ++k)
			{
				doOutRhs(errNode, rhs->children[k], dest, level + 1);
			}

			for(int i = 0; i < level; ++i)
				os << "\t";
			os << "\t\t\t\t</rhs>\n";
		}
	}

	void getRhs(ParseNode *node, std::map<ParseNode *, RhsNode> &dest)
	{
		ParseNode::iterator_type i = node->begin();

		SHOULDBE(node, rhs);

		if((*i)->getToken() == TOKEN_ixNUM)
		{
			dest[node] = RhsNode();
			dest[node].ident = (*i)->getTokenText();
			dest[node].type = '#';
			dest[node].size = "B16";
			return;
		}

		if((*i)->getToken() == KEYWORD_ixsizeof)
		{
			dest[node] = RhsNode();
			++i;	// skip "sizeof"
			++i;	// skip '('

			std::string s = (*i)->getTokenText();
			dest[node].ident = int_to_string(getArgType(s, node));
			
			dest[node].type = 'o';
			dest[node].size = "B16";
			return;
		}

		if((*i)->getToken() == KEYWORD_ixtr || (*i)->getToken() == KEYWORD_ixzx || (*i)->getToken() == KEYWORD_ixsx)
		{
			dest[node] = RhsNode();
			if((*i)->getToken() == KEYWORD_ixtr)
				dest[node].type = 't';
			else
			if((*i)->getToken() == KEYWORD_ixzx)
				dest[node].type = 'z';
			else
				dest[node].type = 's';

			++i;	// skip keyword
			++i;	// skip '<'
			std::string sz = (*i)->getTokenText();

			if(getBSize(sz) == -1 && sz != "BIT")
			{
				sz = int_to_string(getArgType(sz, *i));
			}

			dest[node].ident = sz;
			dest[node].size = sz;
			++i;	// skip IDENT
			++i;	// skip '>'
			++i;	// skip '(', now at 'rhs'

			if(dest[node].type == 't' && (*(*i)->begin())->getToken() == TOKEN_ixNUM)
			{
				dest[node].ident = (*(*i)->begin())->getTokenText();
				dest[node].type = '#';
				// keep size from tr<>().
				return;
			}

			getRhs(*i, dest);
			dest[node].children.push_back(&dest[*i]);;
			return;
		}

		if(node->size() == 1)
		{
			dest[node] = RhsNode();
			dest[node].ident = (*i)->getTokenText();
			dest[node].type = '@';

			dest[node].size = "";
			{
				std::string ident = (*i)->getTokenText();
				for(size_t u = 0; u < proto.argnames.size(); ++u)
				{
					if(ident == proto.argnames[u])
					{
						if(getBSize(proto.argtypes[u]) != -1)
							dest[node].size = proto.argtypes[u];
						else
							dest[node].size = int_to_string(getArgType(proto.argtypes[u], *i));
					}
				}
				if(temps.find(ident) != temps.end())
				{
					std::string tempType = tempTypes[temps[ident]];
					if(tempType == "BIT")
						dest[node].size = "BIT";
					else
						if(getBSize(tempType) != -1)
							dest[node].size = tempType;
						else
							dest[node].size = int_to_string(getArgType(tempType, *i));
				}
			}

			if(dest[node].size.empty())
			{
				dest[node].size = getExternalSize(*i);
			}
			return;
		}

		// fixme, temporary code!
		dest[node] = RhsNode();
		std::string fcnid = (*i)->getTokenText();
		dest[node].ident = fcnid;
		dest[node].type = '(';
		dest[node].size = "BIT";

		// Must be a function. We demand it has at least one argument.
		// If the function starts with '_', then its output size is BIT.
		// Otherwise its output size matches the size of the last argument.
		++i;	// skip IDENT
		++i;	// skip '('
		ParseNode *lst = *i;
		SHOULDBE(lst, list);

		for(ParseNode::iterator_type h = lst->begin(); h != lst->end(); )
		{
			getRhs(*h, dest);
			dest[node].children.push_back(&dest[*h]);

			++h;	// skip rhs
			if(h == lst->end())
				break;
			++h;	// skip ','
		}

		// Check function arguments here.
		if(fcnid == "_quest")
		{
			// Output size remains BIT.
			if(dest[node].children.size() != 3)
			{
				throw ParseSpecificError(node->getLine(), node->getFile(), "_quest() must have exactly three arguments.");
			}
			if(dest[node].children[0]->size != dest[node].children[1]->size || dest[node].children[0]->size != dest[node].children[2]->size)
			{
				throw ParseSpecificError(node->getLine(), node->getFile(), "All arguments to _quest() must have exactly the same size.");
			}
			if(dest[node].children[2]->size != "BIT")
			{
				throw ParseSpecificError(node->getLine(), node->getFile(), "All arguments to _quest() must have a size of BIT.");
			}
		}
		else
		if(fcnid[0] == '_')
		{
			// Output size remains BIT. Input argument sizes may be anything.
		}
		else
		if(fcnid == "quest")
		{
			if(dest[node].children.size() != 3)
			{
				throw ParseSpecificError(node->getLine(), node->getFile(), "quest() must have exactly three arguments.");
			}
			if(dest[node].children[0]->size != "BIT")
			{
				throw ParseSpecificError(node->getLine(), node->getFile(), "First arguments of quest() must have a size of BIT.");
			}
			if(dest[node].children[1]->size != dest[node].children[2]->size)
			{
				throw ParseSpecificError(node->getLine(), node->getFile(), "Last two arguments of quest() must have exactly the same size.");
			}
			dest[node].size = dest[node].children[2]->size;
		}
		else
		{
			// 1st argument is size to use.
			// All input arguments must have the same size unless we end with '_'.
			for(size_t qu = 1; qu < dest[node].children.size(); ++qu)
			{
				if(dest[node].children[qu]->size != dest[node].children[0]->size)
				{
					if(/*qu == dest[node].children.size() - 1 &&*/ fcnid[fcnid.size() - 1] == '_')
						;
					else
						throw ParseSpecificError(node->getLine(), node->getFile(), "All arguments to " + fcnid + "() must have exactly the same size - " + dest[node].children[qu]->size + " vs. " + dest[node].children[0]->size);
				}
			}
			dest[node].size = dest[node].children[0]->size;
			if(dest[node].size == "BIT")
			{
				throw ParseSpecificError(node->getLine(), node->getFile(), "Function " + fcnid + "() cannot return BIT -- name must begin with '_' character.");
			}

			if(fcnid == "seg16" || fcnid == "seg32" || fcnid == "seg64")
			{
				dest[node].size = "B2";
			}
		}

		/*for(ParseNode::iterator_type i = node->begin(); i != node->end(); ++i)
		{
			ParseNode *tmp = *i;

			if(tmp->size() != 0)
				getRhs(tmp, dest);
		}*/
	}

	int getArgType(std::string type, ParseNode *errNode)
	{
		int x = -1;

		for(size_t i = 0; i < proto.argtypes.size(); ++i)
		{
			if(proto.argtypes[i] == type)
			{
				x = i;
				break;
			}
		}

		if(x == -1)
		{
			ParseNode *node = errNode;
			throw ParseSpecificError(node->getLine(), node->getFile(), "Illegal type: " + type);
		}
		return x;
	}

	void determineSensitivity(bool &osz, bool &asz, bool &ssz, ParseNode *node)
	{
		for(ParseNode::iterator_type i = node->begin(); i != node->end(); ++i)
		{
			if((*i)->size() != 0)
			{
				if((*i)->getToken() == RULE_ixprefix2)
				{
					ParseNode *node2 = *(*i)->begin();
					std::string str2 = node2->getTokenText();
					if(!str2.empty())
					{
						if(str2[0] == 'o')
							osz = true;
						else
						if(str2[0] == 'a')
							asz = true;
						else
						if(str2[0] == 's')
							ssz = true;
					}
				}
				else
					determineSensitivity(osz, asz, ssz, *i);
			}
		}
	}
};

void syntaxParse(ParseNode *root)
{
	std::ostream &os = std::cout;

	SHOULDBE(root, start);
	ParseNode *declaration_seq = *root->begin();
	SHOULDBE(declaration_seq, declaration_seq);

	os << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";

	// The reader of the XML must fail if its version number (a double) is less than minver.
	// The version attribute is an advisory only and is not enforced, unlike minver.
	os << "<intel minver=\"1.0\" version=\"1.0\" copyright=\"Copyright (C) 2010,2011 Willow Schlanger. All rights reserved.\">\n";
	os << "<insns>\n";

	for(ParseNode::iterator_type i = declaration_seq->begin(); i != declaration_seq->end(); ++i)
	{
		ParseNode *declaration = *i;
		SHOULDBE(declaration, declaration);

		std::string flags = "";
		std::string alias = "";

		ParseNode::iterator_type j = declaration->begin();
		++j;	// skip "insn"
		if((*j)->getToken() == RULE_ixflags)
		{
			std::list<ParseNode *> lst;
			getTerminals(*j, lst);
			for(std::list<ParseNode *>::iterator k = lst.begin(); k != lst.end(); ++k)
			{
				flags += " " + (*k)->getTokenText() + "=\"true\"";
			}
			++j;	// skip "flags"
		}
		if((*j)->getToken() == TOKEN_ixLITSTRING)
		{
			alias = (*j)->getTokenText();
			++j;	// skip "LITSTRING"
		}

		SHOULDBE(*j, prototype);
		Prototype proto(*j);

		os << "<insn";
		os << " name=\"" << proto.ident << "\"";
		if(alias.empty())
			os << " alias=\"" << proto.ident << "\"";
		else
			os << " alias=" << alias;	// alias already has quotes
		os << " numargs=\"" << proto.argtypes.size() << "\"";
		os << flags;
		for(size_t i = 0; i < 4; ++i)
		{
			os << " arg" << i << "=\"";
			if(i >= proto.argtypes.size())
				os << "void";
			else
				os << proto.argtypes[i];
			os << "\"";
		}
		os << ">\n";

		// Encodings.
		os << "\t<encodings>\n";
		++j;
		SHOULDBE(*j, machine);

		for(std::list<ParseNode *>::iterator k = (*j)->begin(); k != (*j)->end(); ++k)
		{
			ParseNode *mach = *k;
			SHOULDBE(mach, mach);
			std::list<ParseNode *>::iterator h = mach->begin();
			std::string format = (*h)->getTokenText();				// save format (opcodes and operands)
			++h;
			std::set<std::string> tags;
			size_t numArgs = proto.argtypes.size();
			std::string argsizes[4];
			argsizes[0] = "";
			argsizes[1] = "";
			argsizes[2] = "";
			argsizes[3] = "";
			for(size_t e = 0; e < numArgs; ++e)
			{
				if(getBSize(proto.argtypes[e]) != -1)
					argsizes[e] = proto.argtypes[e];
			}
			if(h != mach->end())
			{
				ParseNode *mach_suffix = *h;
				SHOULDBE(mach_suffix, mach_suffix);

				std::list<ParseNode *>::iterator aa = mach_suffix->begin();
				++aa;	// skip '!'
				while(aa != mach_suffix->end())
				{
					ParseNode *mach_tag = *aa;
					SHOULDBE(mach_tag, mach_tag);

					std::list<ParseNode *>::iterator bb = mach_tag->begin();
					std::string p = (*bb)->getTokenText();
					++bb;	// skip 1st token
					if(bb != mach_tag->end())
					{
						++bb;	// skip '='
						std::string q = (*bb)->getTokenText();

						bool found = false;
						for(size_t cc = 0; cc < numArgs; ++cc)
						{
							if(proto.argtypes[cc] != p)
								continue;
							found = true;
							if(argsizes[cc].empty())
							{
								if(getSpecialSize(q) == 0 && getBSize(q) == -1)
								{
									ParseNode *node = *k;
									throw ParseSpecificError(node->getLine(), node->getFile(), std::string("Instruction ") + proto.ident + " : illegal size identifier '" + q + "'.");
								}
								argsizes[cc] = q;
							}
							else
							if(argsizes[cc] != q)
							{
								ParseNode *node = *k;
								throw ParseSpecificError(node->getLine(), node->getFile(), std::string("Instruction ") + proto.ident + " : Conflict with size of at arguments - specified multiple times: " + argsizes[cc] + " vs. " + q + " (arg " + int_to_string(cc) + ")");
							}
						}

						if(!found)
						{
							ParseNode *node = *k;
							throw ParseSpecificError(node->getLine(), node->getFile(), std::string("Instruction ") + proto.ident + " : unexpected identifier '" + p + "'.");
						}
					}
					else
					{
						tags.insert(p);
					}

					++aa;	// skip mach_tag
					if(aa == mach_suffix->end())
						break;
					++aa;	// skip ','
				}
			}
			for(size_t e = 0; e < numArgs; ++e)
			{
				if(argsizes[e].empty())
				{
					ParseNode *node = *j;
					throw ParseSpecificError(node->getLine(), node->getFile(), std::string("Instruction ") + proto.ident + " : size of argument " + int_to_string(e) + " not specified");
				}
			}

			std::string argtypes[4];	// arg types. rm becomes gr/mem.
			std::string argvalues[4];	// hex values

			// Parse "format". Example: "80 /2 rm,imm"
			// is_valid_arg_type -- or imm, r, rm -- are valid.

			std::string format2 = std::string(++format.begin(), --format.end());

			Tokens tokens(format2);
			std::set<std::string> prefixes;
			std::string opcode, opcode2, mod, regop, rm;
			{
				size_t b = 0, e = tokens.tokens.size();

				if(b != e)
				{
					while(is_prefix(tokens.tokens[b]))
					{
						prefixes.insert(tokens.tokens[b]);
						++b;
						if(b == e)
							break;
					}
				}

				opcode2 = "";
				opcode = "";
				if(b != e)
				{
					if(tokens.tokens[b] == "0f")
					{
						opcode = "1";
						++b;
					}
				}

				if(b != e)
				{
					if(tokens.tokens[b] == "0f")
					{
						opcode[0] = '2';
						++b;
					}
				}

				if(b != e)
				{
					if(is_valid_hex(tokens.tokens[b]))
					{
						opcode += tokens.tokens[b];
						++b;
					}
				}

				if(b != e)
				{
					std::string s = tokens.tokens[b];
					if(s[0] == '&')
					{
						s = std::string(++s.begin(), s.end());
						if(is_valid_hex(s))
						{
							opcode2 = s;
							++b;
						}
					}
				}

				mod = "";
				regop = "";
				rm = "";
				while(b != e && tokens.tokens[b][0] == '/')
				{
					std::string s = tokens.tokens[b];
					bool fail = false;
					if(s.size() == 2 && s[1] >= '0' && s[1] <= '7')
						regop = std::string("") + s[1];
					else
					if(s == "/r")
						regop = "r";
					else
					if(s.size() == 4 && s[1] == 'm' && s[2] == 'd')
					{
						char c = s[3];
						if(c == 'm' || (c >= '0' && c <= '3'))
							mod = std::string("") + c;
						else
							fail = true;
					}
					else
					if(s.size() == 4 && s[1] == 'r' && s[2] == 'm' && (s[3] >= '0' && s[3] <= '7'))
					{
						char c = s[3];
						rm = std::string("") + c;
					}
					else
						fail = true;

					if(fail)
					{
						ParseNode *node = *j;
						throw ParseSpecificError(node->getLine(), node->getFile(), std::string("Instruction ") + proto.ident + " : illegal encoding");
					}

					++b;
				}

				if(b == e)
				{
					ParseNode *node = *j;
					throw ParseSpecificError(node->getLine(), node->getFile(), std::string("Instruction ") + proto.ident + " : argument list expected");
				}

				std::string args = tokens.tokens[b];

				++b;
				if(b != e)
				{
					ParseNode *node = *j;
					throw ParseSpecificError(node->getLine(), node->getFile(), std::string("Instruction ") + proto.ident + " : end of machine code definition expected");
				}

				if(args != "void")
				{
					CharTokenizer tokens(args, ',');
					if(tokens.tokens.size() != numArgs)
					{
						ParseNode *node = *j;
						throw ParseSpecificError(node->getLine(), node->getFile(), std::string("Instruction ") + proto.ident + " : mismatch with number of arguments!");
					}

					for(size_t i = 0; i < numArgs; ++i)
					{
						argvalues[i] = "ff";		// default argument value is 0xff. this is important because when a reg has <0xff size, it's an implict reg.
						CharTokenizer tokens2(tokens.tokens[i], ':');
						if(tokens2.tokens.size() == 2)
							argvalues[i] = tokens2.tokens[1];
						if(tokens2.tokens.size() > 2)
						{
							ParseNode *node = *j;
							throw ParseSpecificError(node->getLine(), node->getFile(), std::string("Instruction ") + proto.ident + " : illegal argument : " + tokens.tokens[i]);
						}
						argtypes[i] = tokens2.tokens[0];
					}
				}
			}

			for(size_t e = 0; e < numArgs; ++e)
			{
				if(argtypes[e].empty() || argvalues[e].empty())
				{
					ParseNode *node = *j;
					throw ParseSpecificError(node->getLine(), node->getFile(), std::string("Instruction ") + proto.ident + " : incomplete specification for argument " + int_to_string(e));
				}
			}

			os << "\t\t<encoding";

			os << " opcode1=\"" << opcode << "\"";
			if(!opcode2.empty())
				os << " opcode2=\"" << opcode2 << "\"";
			if(!mod.empty())
				os << " mod=\"" << mod << "\"";
			if(!regop.empty())
				os << " regop=\"" << regop << "\"";
			if(!rm.empty())
				os << " rm=\"" << rm << "\"";

			if(prefixes.find("no66") != prefixes.end())
				os << " op66=\"no66\"";
			if(prefixes.find("66") != prefixes.end())
				os << " op66=\"66\"";

			if(prefixes.find("f2") != prefixes.end())
				os << " fx=\"f2\"";
			if(prefixes.find("f3") != prefixes.end())
				os << " fx=\"f3\"";
			if(prefixes.find("nofx") != prefixes.end())
				os << " fx=\"nofx\"";

			bool immYet = false;
			for(size_t e = 0; e < 4; ++e)
			{
				os << " arg" << e << "=\"";
				if(e >= numArgs)
					os << "void";
				else
				{
					if(argtypes[e] == "rm")
						os << "reg_gr/mem";
					else
					if(argtypes[e] == "mem")
						os << "mem_ea";
					else
					if(argtypes[e] == "r")
						os << "reg_gr";
					else
					if(argtypes[e] == "imm")
					{
						if(!immYet)
							os << "imm_1st";
						else
							os << "imm_2nd";
						immYet = true;
					}
					else
					{
						if(!is_valid_arg_type(argtypes[e]))
						{
							ParseNode *node = *k;
							throw ParseSpecificError(node->getLine(), node->getFile(), std::string("Instruction ") + proto.ident + " : has invalid argument : " + argtypes[e]);
						}
						os << argtypes[e];
					}
					os << ":" << argsizes[e] << ":" << argvalues[e];
				}
				os << "\"";
			}

			for(std::set<std::string>::iterator e = tags.begin(); e != tags.end(); ++e)
			{
				os << " " << *e << "=\"true\"";
			}

			os << " />\n";
		}

		os << "\t</encodings>\n";

		for(++j; j != declaration->end(); ++j)
		{
			ParseNode *special = *j;
			SHOULDBE(special, special);
			std::list<ParseNode *>::iterator dd = special->begin();
			if((*dd)->getTokenText() == "emu")
			{
				ParseEmu emu(special, os, proto);
				emu.parse();
			}
			else
			if((*dd)->getTokenText() == "dis")
			{
				++dd;	// skip "dis"
				std::string asmtype = (*(*dd)->begin())->getTokenText();
				os << "\t<dis type=\"" << asmtype << "\">\n";
				++dd;	// skip asmtype
				++dd;	// skip '{'
				while((*dd)->getToken() == RULE_ixdis_stmt)
				{
					ParseNode *dis_stmt = *dd;
					SHOULDBE(dis_stmt, dis_stmt);
					std::list<ParseNode *>::iterator ee = dis_stmt->begin();
					std::string ident = (*ee)->getTokenText();
					++ee;	// skip ident
					++ee;	// skip '('

					os << "\t\t<dis_stmt ident=\"" << ident << "\">\n";
					if((*ee)->getToken() == RULE_ixdis_args)
					{
						std::list<ParseNode *>::iterator ff = (*ee)->begin();
						while(ff != (*ee)->end())
						{
							ParseNode *dis_arg = *ff;

							SHOULDBE(dis_arg, dis_arg);
							std::string a = (*dis_arg->begin())->getTokenText();
							os << "\t\t\t<dis_arg";
							if(!a.empty() && a[0] == '"')
								os << "_str value=" << a;
							else
								os << " value=" << "\"" << a << "\"";
							os << " />\n";

							++ff;
							if(ff == (*ee)->end())
								break;
							++ff;	// skip ','
						}
					}
					os << "\t\t</dis_stmt>\n";

					++dd;
				}
				os << "\t</dis>\n";
			}
		}

		os << "</insn>\n" << std::endl;
	}

	os << "</insns>\n";
	os << "</intel>\n";
	os << std::endl;
}

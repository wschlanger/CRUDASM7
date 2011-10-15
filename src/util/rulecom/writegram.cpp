// writegram.cpp - part ii of ruleir.cpp
// Copyright (C) 2009 Willow Schlanger

#include "ruleir.h"
#include "../../common/include/types.h"
#include <stddef.h>
typedef size_t UINT;

static std::string unquote(std::string s)
{
	if(s.empty() || s[0] != '"' || s.size() <= 2)
		return s;
	return std::string(++s.begin(), --s.end());
}

void FaRuleIr::writeHeader(std::ostream &os, std::string filename)
{
	FaGrammar *dfa = getDfa();
	if(dfa == NULL)
		return;		// nothing to do

	os << "// " << filename << " - automatically generated file (do not edit!)" << std::endl;
	if(!copyright.empty())
	os << "// " << copyright << std::endl;

	os << std::endl;
	os << "enum" << std::endl;
	os << "{" << std::endl;
	for(FaGrammar::iterator i = dfa->begin(); i != dfa->end(); ++i)
	{
		os << "\tRULE_" << prefix << *i->first << " = 0x" << std::hex << symTab->getNum(*i->first) << std::dec << "," << std::endl;
	}
	os << "\tRULE__" << prefix << "count = " << dfa->size() << std::endl;
	os << "};" << std::endl;
	
	os << std::endl;
	os << "enum" << std::endl;
	os << "{" << std::endl;
	for(std::set<std::string>::iterator i = tokens.begin(); i != tokens.end(); ++i)
	{
		os << "\tTOKEN_" << prefix << *i << " = 0x" << std::hex << symTab->getNum(*i) << std::dec << "," << std::endl;
	}
	os << "\tTOKEN__" << prefix << "count = " << tokens.size() << std::endl;
	os << "};" << std::endl;
	
	os << std::endl;
	os << "enum" << std::endl;
	os << "{" << std::endl;
	for(std::map<std::string, std::string>::iterator i = keywords.begin(); i != keywords.end(); ++i)
	{
		os << "\tKEYWORD_" << prefix << i->second << " = 0x" << std::hex << symTab->getNum(i->first) << std::dec << ",";
		os << "\t// " << i->first;
		os << std::endl;
	}
	os << "\tKEYWORD__" << prefix << "count = " << keywords.size() << std::endl;
	os << "};" << std::endl;

	os << std::endl;
}

void FaRuleIr::writeParser(std::ostream &os, std::string filename)
{
	FaGrammar *dfa = getDfa();
	if(dfa == NULL)
		return;		// nothing to do
	
	os << "// " << filename << " - automatically generated file (do not edit!)" << std::endl;
	if(!copyright.empty())
	os << "// " << copyright << std::endl;
	os << std::endl;
	
	for(FaGrammar::iterator i = dfa->begin(); i != dfa->end(); ++i)
	{
		os << "ParseNode *parse_" << prefix << "RULE_" << *i->first << "()" << std::endl;
		os << "{" << std::endl;
		os << "\tParseNode *node = new ParseNode(" << "RULE_" << prefix << *i->first << ", getLine(), getFile());" << std::endl;
		os << "\ttry" << std::endl;
		os << "\t{" << std::endl;
		
		FaNfa &fa = *i->second;
		os << "\t\tgoto L_" << fa.getStartState() << ";" << std::endl;
		
		for(unsigned j = 0; j < fa.getNumStates(); ++j)
		{
			FaState &st = fa.getState(j);
			os << "\t\tL_" << j << ":" << std::endl;
			os << "\t\tswitch(getToken())" << std::endl;
			os << "\t\t{" << std::endl;
			
			for(FaState::edge_iterator k = st.begin(); k != st.end(); ++k)
			{
				const std::string *symbol = k->getSymbol();
				unsigned target = k->getTarget();
				
				std::string token = *symbol;
				std::string comment;
				unsigned value = symTab->getNum(*symbol);
				
				bool isrule = false;
				if(value >= 0x10000 && value <= 0x1ffff)
					token = std::string("TOKEN_") + prefix + token;
				else
				if(value > 0x30000 && value <= 0x3ffff)
					isrule = true;
				else
				if(value >= 0x20000 && value <= 0x2ffff)
				{
					comment = token;
					token = std::string("KEYWORD_") + prefix + keywords[token];
				}
				
				if(isrule)
				{
					os << "\t\t\t// begin rule " << *symbol << std::endl;
					for(FaNfaToDfa::start_sym_iterator h = xlat->start_sym_begin(symbol); h != xlat->start_sym_end(symbol); ++h)
					{
						const std::string *sym = *h;
						
						std::string token = *sym;
						std::string comment;
						unsigned val = symTab->getNum(*sym);
						
						if(val >= 0x10000 && val <= 0x1ffff)
							token = std::string("TOKEN_") + prefix + token;
						else
						if(val >= 0x20000 && val <= 0x2ffff)
						{
							comment = token;
							token = std::string("KEYWORD_") + prefix + keywords[token];
						}

						os << "\t\t\tcase "	<< token << ":";
						if(!comment.empty())
							os << "\t// " << comment;
						os << std::endl;
					}
					
					os << "\t\t\t// end rule " << *symbol << std::endl;
					os << "\t\t\t\taddNonterminal(node, parse_" << prefix << "RULE_" << *symbol <<"());" << std::endl;
				}
				else
				{
					os << "\t\t\tcase "	<< token << ":";
					if(!comment.empty())
						os << "\t// " << comment;
					os << std::endl;
					os << "\t\t\t\taddTerminal(node);" << std::endl;
					os << "\t\t\t\tnextToken();" << std::endl;
				}
				os << "\t\t\t\tgoto L_" << target << ";" << std::endl;
			}
			
			os << "\t\t\tdefault:" << std::endl;
			if(st.getAccept())
				os << "\t\t\t\treturn node;\t// accept state" << std::endl;
			else
				os << "\t\t\t\tthrow ParseSyntaxError();" << std::endl;
			os << "\t\t\t\tbreak;" << std::endl;
			os << "\t\t}" << std::endl;
		}
		
		os << "\t}" << std::endl;

		os << "\tcatch(ParseSyntaxError &e)" << std::endl;
		os << "\t{" << std::endl;
		os << "\t\tdelete node;" << std::endl;
		os << "\t\tthrow e;" << std::endl;
		os << "\t}" << std::endl;
		os << "\treturn NULL;" << std::endl;
		os << "}" << std::endl;
		os << std::endl;
	}
	
	os << std::endl;
}

struct FaLexerDfaState
{
	std::map<char, UINT> edges;
	std::string match;						// if accept state, this is what we match (C++ syntax)
};

struct FaLexerDfa
{
	std::vector<FaLexerDfaState> states;	// state 0 is start state
	
	FaLexerDfa()
	{
		states.push_back(FaLexerDfaState());	// create start state
	}
	
	// pre: this should never be called with duplicate 'token' inputs.
	void add(std::string token, std::string match)
	{
		UINT next = 0;
		
		for(std::string::iterator i = token.begin(); i != token.end(); ++i)
		{
			if(states[next].edges.find(*i) != states[next].edges.end())
				next = states[next].edges[*i];
			else
			{
				do
				{
					UINT st = states.size();
					states.push_back(FaLexerDfaState());
					states[next].edges[*i] = st;
					///std::cout << "(" << next << "," << *i << ") -> " << st << std::endl;
					next = st;
					
					++i;
					if(i == token.end())
						states[st].match = match;
				}	while(i != token.end());
			
				break;
			}
		}
	}
};

void FaRuleIr::writeLexer(std::ostream &os, std::string filename, std::map<std::string,std::string> &manualTokens
)
{
	FaGrammar *dfa = getDfa();
	if(dfa == NULL)
		return;		// nothing to do
	os << "// " << filename << " - automatically generated file (do not edit!)" << std::endl;
	if(!copyright.empty())
	os << "// " << copyright << std::endl;
	os << std::endl;
	
	char startChars[256];
	for(int i = 0; i < 256; ++i)
		startChars[i] = 0;
	for(std::map<std::string,std::string>::iterator i = manualTokens.begin(); i != manualTokens.end(); ++i)
	{
		std::string s = i->second;
		unsigned j = 0;
		while(j < s.size())
		{
			unsigned char c1 = s[j];
			unsigned char c2 = s[j + 1];
			for(; c1 <= c2; ++c1)
			{
				startChars[static_cast<unsigned>(c1)] = 1;
				///std::cout << (char)(c1) << " ";
			}
			j += 2;
		}
	}
	
	std::set<std::string> manualKeywords;	// unquoted
	for(std::map<std::string, std::string>::iterator i = keywords.begin(); i != keywords.end(); ++i)
	{
		std::string keyword = unquote(i->first);
		if(startChars[keyword[0]] != 0)
		{
			manualKeywords.insert(keyword);
			///std::cout << keyword << std::endl;
		}
	}
	
	os << "virtual void initManualKeywords()" << std::endl;
	os << "{" << std::endl;
	for(std::set<std::string>::iterator i = manualKeywords.begin(); i != manualKeywords.end(); ++i)
	{
		std::string s = std::string("\"") + *i + std::string("\"");
		os << "\taddManualKeyword(\"" << *i << "\", KEYWORD_" << prefix << keywords[s] << ");" << std::endl;
	}
	os << "}" << std::endl;
	
	std::map<std::string, std::string> tokens;	// text -> C++ match
	for(FaSymbolTable::sym_tab_iterator i = symTab->begin(); i != symTab->end(); ++i)
	{
		std::string symbol = i->first;
		if(symbol.empty())
			continue;
		
		// Match single-character tokens.
		if(symbol[0] == '\'' && symbol.size() == 3)
		{
			char c = symbol[1];
			std::string s = "";
			s += c;
			if(startChars[static_cast<unsigned>(c)] != 0)
			{
				std::cout << "warning: character \'" << c << "\' conflicts with a manual keyword!" << std::endl;
			}
			tokens[s] = symbol;
		}
		else
		if(symbol[0] == '\"' && symbol.size() >= 3)
		{
			std::string s = unquote(symbol);
			std::string t = std::string("KEYWORD_") + prefix + keywords[symbol];
			if(startChars[s[0]] == 0)
			{
				tokens[s] = t;
			}
		}
	}
	
	// Note: we need to make an NFA, then cvt to DFA.
	// Accept states may need ATTRIBUTES such as KEYWORD_rcISDEFAS.
	// We then know what to return if we get there, but is this theoretically sound?
	FaLexerDfa lexDfa;
	for(std::map<std::string, std::string>::iterator i = tokens.begin(); i != tokens.end(); ++i)
	{
		lexDfa.add(i->first, i->second);
	}
	
	os << std::endl;
	os << "unsigned lexNextToken()" << std::endl;
	os << "{" << std::endl;
	os << "\tgoto L0;" << std::endl;
	
	for(unsigned state = 0; state < lexDfa.states.size(); ++state)
	{
		os << "\tL" << state << ":" << std::endl;
		os << "\tswitch(getChar())" << std::endl;
		os << "\t{" << std::endl;
		
		FaLexerDfaState &st = lexDfa.states[state];
		for(std::map<char, UINT>::iterator i = st.edges.begin(); i != st.edges.end(); ++i)
		{
			os << "\t\tcase \'" << i->first << "\':" << std::endl;
			os << "\t\t\tnextChar();" << std::endl;
			os << "\t\t\tgoto L" << i->second << ";" << std::endl;
		}

		os << "\t\tdefault:" << std::endl;
		if(state == 0)
		{
			// this is the start state. can't be an accept state, then.
			os << "\t\t\treturn defaultHandler(0x80000000 + getChar());\t//start state" << std::endl;
		}
		else
		if(lexDfa.states[state].match.empty())
		{
			os << "\t\t\treturn 0x80000000 + getChar();\t// unexpected character" << std::endl;
		}
		else
		{
			os << "\t\t\treturn " << lexDfa.states[state].match << ";\t// accept state" << std::endl;
		}
		os << "\t\t\tbreak;" << std::endl;
		os << "\t}" << std::endl;
	}
	
	os << "\treturn 0;" << std::endl;
	os << "}" << std::endl;
	
	os << std::endl;
}

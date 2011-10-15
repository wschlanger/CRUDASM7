// ruleir.cpp - intermediate representation for rule-based grammars
// Copyright (C) 2009 Willow Schlanger

// Contains bootstrap code.
// We are going to need to be able to look up start symbols from the NFA for a given rule.
// Where is this information stored ? 'startSyms' in the NFA to DFA converter class.

#include "ruleir.h"

void FaRuleIr::addToken(std::string token)
{
	symTab->getNum(token);
	tokens.insert(token);
}

void FaRuleIr::addKeyword(std::string keyword, std::string tagname)
{
	symTab->getNum(keyword);
	keywords[keyword] = tagname;
}

FaNfa *FaRuleIr::addProduction(std::string rule, unsigned &state)
{
	const std::string *symbol = symTab->getSym(rule);
	if(gram.find(symbol) == gram.end())
	{
		// NFA for this production does not exist yet. Create it.
		nfas.push_back(FaNfa(symTab));
		gram[symbol] = &nfas.back();
	}
	state = gram[symbol]->addProduction();
	return gram[symbol];
}

unsigned FaRuleIr::addState(FaNfa &fa)
{
	return fa.addState();
}

void FaRuleIr::makeFinal(FaNfa &fa, unsigned state)
{
	fa.makeFinal(state);
}

// Note on symbol: it may begin with ' or " in which case it must end with the same
// thing and the middle must not be empty. Any \'s have been expanded, so "Fo\"o" will
// become "Fo"o".
bool FaRuleIr::addEdge(FaNfa &fa, unsigned from, unsigned to, std::string symbol)
{
	if(symbol.empty())
		return false;
	if(symbol[0] == '\'' || symbol[0] == '\"')
		if(symbol.size() <= 2)
			return false;
	unsigned symNum = symTab->getNum(symbol);
	if(symNum == 0x30000)
		return false;	// lambda
	if(symNum > 0x30000 && symNum <= 0x3ffff)
	{
		// rule.
		neededRules.insert(symbol);
	}
	else
	if(symNum >= 0x20000 && symNum <= 0x2ffff)
	{
		// "keyword" or ">>=".
		if(keywords.find(symbol) == keywords.end())
		{
			// keyword does not already exist.
			if(symbol.size() <= 2)
				return false;
			std::string tagname;
			for(unsigned i = 1; i < symbol.size() - 1; ++i)
			{
				if(symbol[i] >= 'a' && symbol[i] <= 'z')
					;
				else
				if(symbol[i] >= 'A' && symbol[i] <= 'Z')
					;
				else
				if(symbol[i] >= '0' && symbol[i] <= '9')
					;
				else
				if(symbol[i] != '_')
					return false;	// can't automatically create keyword
				tagname += symbol[i];
			}
			keywords[symbol] = tagname;
		}
	}
	else
	if(symNum >= 0x10000 && symNum <= 0x1ffff)
	{
		// Token. Must be defined before use.
		if(tokens.find(symbol) == tokens.end())
			return false;
	}
	
	// Ok, checks done!
	fa.addEdge(from, to, symTab->getSym(symbol));

	return true;
}

std::string FaRuleIr::convertGrammar()
{
	if(xlat != NULL)
		delete xlat;
	xlat = new FaNfaToDfa(gram, symTab);

	// Make sure all rules we need, exist.
	for(std::set<std::string>::iterator i = neededRules.begin(); i != neededRules.end(); ++i)
	{
		if(gram.find(symTab->getSym(*i)) == gram.end())
		{
			std::string s = "need rule: ";
			s += *i;
			return s;
		}
	}
	
	// Check for lambda self-loops.
	for(FaGrammar::iterator i = gram.begin(); i != gram.end(); ++i)
	{
		FaNfa &fa = *i->second;
		if(fa.hasLambdaSelfLoop())
		{
			std::string s = "internal error: ";
			s += *i->first;
			s += " has a lambda self-loop";
			return s;
		}
	}
	
	// Convert NFA to DFA.
	std::string err = xlat->nfaToDfa();
	if(!err.empty())
	{
		std::string s = "unable to convert to DFA (rule is optional and/or has conflicts): ";
		s += err;
		return s;
	}

	return "";	// success
}

//---
#if 0	// bootstrap code. beware that there is no semicolon in the FA here after copyright/prefix.
#include <fstream>

int main()
{
	FaSymbolTable symTab;
	FaRuleIr ruleIr(&symTab);
	
	ruleIr.setCopyright("Copyright (C) 2009 Willow Schlanger");
	ruleIr.setPrefix("rc");
	ruleIr.addToken("IDENT");
	ruleIr.addToken("LITCHAR");
	ruleIr.addToken("LITSTRING");
	ruleIr.addKeyword("\"::=\"", "ISDEFAS");
	
	// 'start' symbol.
	{
		FaNfa *fa;
		bool ok;
		
		unsigned q1;
		fa = ruleIr.addProduction("start", q1);
		unsigned q2 = ruleIr.addState(*fa);
		ruleIr.makeFinal(*fa, q2);

		ok = ruleIr.addEdge(*fa, q1, q2, "statement");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q2, q2, "statement");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
	}
	
	// 'statement' symbol.
	{
		FaNfa *fa;
		bool ok;

		unsigned q1;
		fa = ruleIr.addProduction("statement", q1);
		unsigned q2 = ruleIr.addState(*fa);
		unsigned q3 = ruleIr.addState(*fa);
		unsigned q4 = ruleIr.addState(*fa);
		ruleIr.makeFinal(*fa, q4);
		ok = ruleIr.addEdge(*fa, q1, q2, "\"token\"");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q2, q3, "IDENT");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q3, q3, "IDENT");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q3, q4, "';'");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		
		unsigned q10;
		fa = ruleIr.addProduction("statement", q10);
		unsigned q11 = ruleIr.addState(*fa);
		unsigned q12 = ruleIr.addState(*fa);
		unsigned q13 = ruleIr.addState(*fa);
		unsigned q14 = ruleIr.addState(*fa);
		ruleIr.makeFinal(*fa, q14);
		ok = ruleIr.addEdge(*fa, q10, q11, "name");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q11, q12, "\"::=\"");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q12, q13, "alternate");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q13, q12, "'|'");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q13, q14, "';'");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		
		unsigned q20;
		fa = ruleIr.addProduction("statement", q20);
		unsigned q21 = ruleIr.addState(*fa);
		unsigned q22 = ruleIr.addState(*fa);
		unsigned q23 = ruleIr.addState(*fa);
		unsigned q24 = ruleIr.addState(*fa);
		ruleIr.makeFinal(*fa, q24);
		ok = ruleIr.addEdge(*fa, q20, q21, "\"keyword\"");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q21, q22, "LITSTRING");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q22, q23, "IDENT");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q23, q24, "';'");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		
		unsigned q30;
		fa = ruleIr.addProduction("statement", q30);
		unsigned q31 = ruleIr.addState(*fa);
		unsigned q32 = ruleIr.addState(*fa);
		ruleIr.makeFinal(*fa, q32);
		ok = ruleIr.addEdge(*fa, q30, q31, "\"copyright\"");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q31, q32, "LITSTRING");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		
		unsigned q40;
		fa = ruleIr.addProduction("statement", q40);
		unsigned q41 = ruleIr.addState(*fa);
		unsigned q42 = ruleIr.addState(*fa);
		ruleIr.makeFinal(*fa, q42);
		ok = ruleIr.addEdge(*fa, q40, q41, "\"prefix\"");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q41, q42, "LITSTRING");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
	}
	
	// 'name' symbol.
	{
		FaNfa *fa;
		bool ok;
		
		unsigned q1;
		fa = ruleIr.addProduction("name", q1);
		unsigned q2 = ruleIr.addState(*fa);
		ruleIr.makeFinal(*fa, q2);
		ok = ruleIr.addEdge(*fa, q1, q2, "IDENT");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q1, q2, "\"start\"");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
	}
	
	// 'alternate' symbol.
	{
		FaNfa *fa;
		bool ok;
		
		unsigned q1;
		fa = ruleIr.addProduction("alternate", q1);
		unsigned q2 = ruleIr.addState(*fa);
		ruleIr.makeFinal(*fa, q2);
		ok = ruleIr.addEdge(*fa, q1, q2, "symbol");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q2, q2, "symbol");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
	}
	
	// 'symbol' symbol.
	{
		FaNfa *fa;
		bool ok;
		
		unsigned q1;
		fa = ruleIr.addProduction("symbol", q1);
		unsigned q2 = ruleIr.addState(*fa);
		unsigned q3 = ruleIr.addState(*fa);
		unsigned q4 = ruleIr.addState(*fa);
		unsigned q5 = ruleIr.addState(*fa);
		unsigned q6 = ruleIr.addState(*fa);
		unsigned q7 = ruleIr.addState(*fa);
		unsigned q8 = ruleIr.addState(*fa);
		ruleIr.makeFinal(*fa, q2);
		ruleIr.makeFinal(*fa, q5);
		ruleIr.makeFinal(*fa, q8);
		ok = ruleIr.addEdge(*fa, q1, q2, "item");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q1, q3, "'{'");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q3, q4, "alternate");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q4, q5, "'}'");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q1, q6, "'['");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q6, q7, "alternate");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q7, q8, "']'");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
	}

	// 'item' symbol.
	{
		FaNfa *fa;
		bool ok;
		
		unsigned q1;
		fa = ruleIr.addProduction("item", q1);
		unsigned q2 = ruleIr.addState(*fa);
		ruleIr.makeFinal(*fa, q2);
		ok = ruleIr.addEdge(*fa, q1, q2, "LITCHAR");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q1, q2, "LITSTRING");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
		ok = ruleIr.addEdge(*fa, q1, q2, "IDENT");
		if(!ok) { std::cerr << "Error on line " << __LINE__ << std::endl; return 0; }
	}
	
	std::string err;
	err = ruleIr.convertGrammar();
	
	if(!err.empty())
	{
		std::cerr << "bootstrap grammar error: " << err << std::endl;
		return 1;
	}
	
	std::ofstream fo1("out_rulecom_parser.h");
	std::ofstream fo2("out_rulecom_header.h");
	ruleIr.writeParser(fo1, "out_rulecom_parser.h");
	ruleIr.writeHeader(fo2, "out_rulecom_header.h");
	
	return 0;
}
#endif


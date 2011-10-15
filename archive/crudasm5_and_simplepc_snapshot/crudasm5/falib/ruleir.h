// ruleir.h - intermediate representation for rule-based grammars
// Copyright (C) 2009 Willow Schlanger

#ifndef l_ruleir_h__ceres_included
#define l_ruleir_h__ceres_included

#include "falib.h"
#include <string>
#include <iostream>
#include <set>
#include <map>
#include <cstdlib>

class FaRuleIr
{
	FaGrammar gram;
	FaSymbolTable *symTab;
	std::set<std::string> neededRules;
	std::map<std::string, std::string> keywords;	// e.g. ">>=" -> SHREQ
	std::set<std::string> tokens;					// e.g. Something
	std::list<FaNfa> nfas;
	FaNfaToDfa *xlat;
	std::string copyright;
	std::string prefix;
	
	public:
	void setCopyright(std::string s) { copyright = s; }
	void setPrefix(std::string s) { prefix = s; }
	
	FaRuleIr(FaSymbolTable *symTabT) :
		symTab(symTabT),
		xlat(NULL)
	{
	}
	
	~FaRuleIr()
	{
		if(xlat != NULL)
			delete xlat;
	}
	
	// Use this after calling convertGrammar().
	FaGrammar *getDfa()
	{
		if(xlat == NULL)
			return NULL;
		return &xlat->dfa;
	}
	
	// 1st char of token name must be A-Z (uppercase letter).
	// otherwise there will be undefined behavior. Tokens are
	// stored in the symbol table.
	void addToken(std::string token);
	
	// String literals must be made out of chars a-z, A-Z, 0-9, _
	// only by default, and result in a keyword automatically being
	// created. For other keywords, such as ">>=", one must create
	// the keyword manually.
	// Note: 'keyword' arg should begin & end with a double-quote char.
	// Example: addKeyword("\">>=\"", "SHREQ");
	// The keyword is added to the symbol table.
	void addKeyword(std::string keyword, std::string tagname);
	
	// Given a rule, this returns the FA together with a new state for
	// use with the new production.
	FaNfa *addProduction(std::string rule, unsigned &state);
	
	// Once a rule has been created, the following methods allow for NFA construction.
	// If addEdge() returns false, an undefined symbol (i.e. a keyword) was used.
	unsigned addState(FaNfa &fa);
	void makeFinal(FaNfa &fa, unsigned state);
	bool addEdge(FaNfa &fa, unsigned from, unsigned to, std::string symbol);

	void addLambda(FaNfa &fa, unsigned from, unsigned to)
	{
		fa.addEdge(from, to, symTab->getLambda());
	}
	
	// "" = success
	// "need rule: myRulename"
	// "internal error: someRule has a lambda self-loop"
	// "unable to convert to DFA (rule is optional and/or has conflicts): someRule"
	// After this is called, getDfa() will return a non-NULL result.
	std::string convertGrammar();
	
	// Create output files. These are implemented in writegram.cpp.
	void writeHeader(std::ostream &os, std::string filename);
	void writeParser(std::ostream &os, std::string filename);
	void writeLexer(std::ostream &os, std::string filename, std::map<std::string,std::string> &manualTokens);
};

#endif	// l_ruleir_h__ceres_included

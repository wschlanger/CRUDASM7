// ruleparse.cpp
// Copyright (C) 2009 Willow Schlanger

#include "ruleparse.h"
#include <iostream>
#include <cstddef>

#ifndef l_out_rulecom_header__ceres_included
#define l_out_rulecom_header__ceres_included
#include "out_rulecom_header.h"
#endif	// l_out_rulecom_header__ceres_included

#include "ruleir.h"
#include "falib.h"

#include <fstream>
#include <cassert>

static std::string unquote(std::string s)
{
	if(s.empty() || s[0] != '"' || s.size() <= 2)
		return s;
	return std::string(++s.begin(), --s.end());
}

// pre: the state 'lastState' is a newly created state that has only a lambda transition coming in to it.
// post: lastState is updated to a new state that has the property that only lambda transitions go to it.
void addSymbols(ParseNode::iterator_type begin, ParseNode::iterator_type end, FaSymbolTable &symTab, FaRuleIr &ruleIr, FaNfa *nfa, unsigned &lastState)
{
	for(ParseNode::iterator_type i = begin; i != end; ++i)
	{
		assert((*i)->getToken() == RULE_rcsymbol);
		unsigned symbolHeadToken = (*(*i)->begin())->getToken();
	
		if(symbolHeadToken == RULE_rcitem)
		{
			ParseNode *item = *(*i)->begin();
			assert(item->getToken() == RULE_rcitem);
			
			ParseNode *item2 = *item->begin();
			std::string item2Text = item2->getTokenText();
			unsigned newState = ruleIr.addState(*nfa);
			
			bool ok = ruleIr.addEdge(*nfa, lastState, newState, item2Text);
			if(!ok)
			{
				std::string msg = "error adding NFA transition (check that keywords and tokens are defined before use)";
				throw ParseSpecificError((*i)->getLine(), (*i)->getFile(), msg);
			}
			
			lastState = newState;
		}
		else
		if(symbolHeadToken == '{' || symbolHeadToken == '[')
		{
			unsigned q0 = ruleIr.addState(*nfa);
			ruleIr.addLambda(*nfa, lastState, q0);
			lastState = q0;
			ParseNode::iterator_type j = (*i)->begin();
			ParseNode *alternate = *++j;
			assert(alternate->getToken() == RULE_rcalternate);
			addSymbols(alternate->begin(), alternate->end(), symTab, ruleIr, nfa, lastState);
			
			if(symbolHeadToken == '{')
				ruleIr.addLambda(*nfa, lastState, q0);
			ruleIr.addLambda(*nfa, q0, lastState);
			unsigned qf = ruleIr.addState(*nfa);
			ruleIr.addLambda(*nfa, lastState, qf);
			lastState = qf;
		}
	}
	
	unsigned newState = ruleIr.addState(*nfa);
	ruleIr.addLambda(*nfa, lastState, newState);
	lastState = newState;
}

// This is called when the atlernate rule in the statement rule is encountered.
void parseAlternate(FaSymbolTable &symTab, FaRuleIr &ruleIr, ParseNode *alternate, std::string ruleName)
{
	// alternate ::= symbol { symbol } ;
	//
	// symbol ::=
	// item |
	// '{' alternate '}' |
	// '[' alternate ']' ;
	//
	// item ::= LITCHAR | LITSTRING | IDENT ;
	
	unsigned firstState;	// 1st state of this alternate
	FaNfa *nfa = ruleIr.addProduction(ruleName, firstState);
	unsigned lastState = firstState;
	
	addSymbols(alternate->begin(), alternate->end(), symTab, ruleIr, nfa, lastState);
	ruleIr.makeFinal(*nfa, lastState);
}

// pre: root != NULL
void doParse(ParseNode *root, std::string inFileName, std::string outBaseName)
{
	std::map<std::string, std::string> firstManualToken;
	FaSymbolTable symTab;
	FaRuleIr ruleIr(&symTab);

	// Iterate thru children of start symbol.
	for(ParseNode::iterator_type i = root->begin(); i != root->end(); ++i)
	{
		ParseNode *statement = *i;
		ParseNode *statementHead = *statement->begin();
		std::string statementHeadText = statementHead->getTokenText();
		if(statementHeadText.empty())
		{
			// name "::=" alternate { '|' alternate } ';'
			// name ::= IDENT | "start" ;
			///std::cout << "[name]" << std::endl;
			std::string ruleName = (*statementHead->begin())->getTokenText();
			ParseNode::iterator_type j = statement->begin();
			++j;	// skip ruleName
			++j;	// skip "::="
			for(;;)
			{
				ParseNode *alternate = *j;
				
				parseAlternate(symTab, ruleIr, alternate, ruleName);
				
				++j;	// skip alternate
				if((*j)->getTokenText() != "|")
					break;	// found ';' at end
				++j;	// skip '|'
			}
		}
		else
		if(statementHeadText == "token")
		{
			for(ParseNode::iterator_type j = ++statement->begin(); j != statement->end(); ++j)
			{
				std::string s = (*j)->getTokenText();
				if(s == ";")
					break;
				ruleIr.addToken(s);
			}
		}
		else
		if(statementHeadText == "keyword")
		{
			std::string s1 = (*(++statement->begin()))->getTokenText();
			std::string s2 = (*(++(++statement->begin())))->getTokenText();
			ruleIr.addKeyword(s1, s2);
		}
		else
		if(statementHeadText == "copyright")
		{
			ruleIr.setCopyright(unquote((*(++statement->begin()))->getTokenText()));
		}
		else
		if(statementHeadText == "prefix")
		{
			ruleIr.setPrefix(unquote((*(++statement->begin()))->getTokenText()));
		}
		else
		if(statementHeadText == "manual")
		{
			ParseNode::iterator_type j = ++statement->begin();
			std::string s0 = (*j)->getTokenText();
			++j;
			std::string s1 = unquote((*j)->getTokenText());
			++j;
			ruleIr.addToken(s0);
			firstManualToken[s0] = s1;
			if(s1.empty() || (s1.size() & 1) != 0)
			{
				std::string msg = "invalid manual token definition (both string literals must be nonempty and even in length)";
				throw ParseSpecificError((*i)->getLine(), (*i)->getFile(), msg);
			}
		}
	}
	
	std::string err = ruleIr.convertGrammar();
	if(!err.empty())
	{
		std::cerr << "error: " << inFileName << ": " << err << std::endl;
		throw ParseError();
	}
	
	std::string fn1 = "out_" + outBaseName + "_parser.h";
	std::string fn2 = "out_" + outBaseName + "_header.h";
	std::string fn3 = "out_" + outBaseName + "_lexer.h";
	
	std::ofstream fo1(fn1.c_str());
	ruleIr.writeParser(fo1, fn1);
	std::ofstream fo2(fn2.c_str());
	ruleIr.writeHeader(fo2, fn2);
	std::ofstream fo3(fn3.c_str());
	ruleIr.writeLexer(fo3, fn3, firstManualToken);
}

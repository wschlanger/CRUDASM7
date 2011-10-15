// falib.h - finite automaton library
// Copyright (C) 2009 Willow Schlanger

// Note: does not support alternates that derrive lambda,
//   i.e. ra ::= [ rb ] ; is not supported.
// Note: do not put a lambda-loop transition directly from a
//   state to itself again. Doing so will cause unpredictable
//   results. Indirect lambda-loops are supported, i.e. from
//   state1 -> state2 -> state1 again, but not state3 -> state3
//   directly with a lambda symbol.
// Note: we assume all rules are defined, i.e. ra ::= rb ; is not
//   supported unless we have another production such as rb ::= 'x' ;
//   This is not checked, the user must check it!

#ifndef l_falib_h__ceres_included
#define l_falib_h__ceres_included

#include "../../common/include/types.h"
#include <stddef.h>
typedef size_t UINT;

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <cstdlib>

enum
{
	FA_LAMBDA = 0x30000
};

// Types of symbols:
//   0..0xffff - 'c' (0 means EOF)
//   0x10000..0x1ffff - Token
//   0x20000..0x2ffff - "keyword" or ">>="
//   0x30000..0x3ffff - rule (0 is "" and means lambda)
// getNum() / getSym() insert the given string into the table
//   if it's not already there.
// FaSymbolTable is a container class for the symbols used by
//   falib.
class FaSymbolTable
{
	std::map<std::string, unsigned> symTab;
	int numTokens;
	int numKeywords;
	int numRules;

	public:
	FaSymbolTable();
	unsigned getNum(std::string symbol);
	const std::string *getSym(std::string symbol);
	const std::string *getLambda() { return getSym(""); }

	typedef std::map<std::string, unsigned>::iterator sym_tab_iterator;
	sym_tab_iterator begin() { return symTab.begin(); }
	sym_tab_iterator end() { return symTab.end(); }
};

class FaEdge
{
	const std::string *symbol;
	unsigned target;

	public:
	FaEdge(const std::string *symbolT, unsigned targetT) : symbol(symbolT), target(targetT) { }
	const std::string *getSymbol() const { return symbol; }
	unsigned getTarget() const { return target; }
	void setTarget(unsigned targetT) { target = targetT; }
};

class FaState
{
	std::list<FaEdge> edges;
	bool accept;

	public:
	typedef std::list<FaEdge>::iterator edge_iterator;
	FaState() : accept(false) { }
	void setAccept(bool value) { accept = value; }
	bool getAccept() const { return accept; }
	edge_iterator begin() { return edges.begin(); }
	edge_iterator end() { return edges.end(); }
	void eraseEdge(edge_iterator iter) { edges.erase(iter); }
	void insert(const std::string *symbol, unsigned target) { edges.push_back(FaEdge(symbol, target)); }
};

// This represents an NFA.
class FaNfa
{
	typedef std::vector<FaState>::iterator states_iterator;
	std::vector<FaState> states;	// 0 is always start state
	FaSymbolTable *symTab;

	public:
	FaNfa(FaSymbolTable *symTabT);
	unsigned getStartState() const { return 0; }
	unsigned addProduction();
	unsigned addState();
	void makeFinal(unsigned state) { states[state].setAccept(true); }
	bool isFinal(unsigned state) const { return states[state].getAccept(); }
	void addEdge(unsigned from, unsigned target, const std::string *symbol);

	friend class FaNfaToDfa;
	
	UINT getNumStates() const { return states.size(); }
	FaState &getState(unsigned index) { return states[index]; }
	
	// Lambda self-loops are not supported. This method checks for them.
	bool hasLambdaSelfLoop()
	{
		for(unsigned i = 0; i < states.size(); ++i)
		{
			for(FaState::edge_iterator j = states[i].begin(); j != states[i].end(); ++j)
			{
				if(j->getTarget() == i && j->getSymbol() == symTab->getLambda())
					return true;
			}
		}
		return false;
	}

#if 0
	void debugDump()
	{
		for(unsigned i = 0; i < states.size(); ++i)
		{
			std::cout << i;
			if(states[i].getAccept())
				std::cout << "*";
			std::cout << " :";
			for(FaState::edge_iterator j = states[i].begin(); j != states[i].end(); ++j)
			{
				std::cout << " " << j->getTarget() << "(" << *j->getSymbol() << ")";
			}
			std::cout << std::endl;
		}
	}
#endif
};

typedef std::map<const std::string *, FaNfa *> FaGrammar;

// Pre: src grammar must not use any nonterminals ('rules') in a production without
// defining the rule, i.e. this grammar is invalid:  ra ::= rb 'c' ;  unless the
// rb rule is defined elsewhere, it's in error. The USER must check this !!
class FaNfaToDfa
{
	FaGrammar &nfa;
	FaSymbolTable *symTab;
	std::map<const std::string *, std::set<const std::string *> > startSyms;
	std::list<FaNfa> dfas;

	public:
	FaGrammar dfa;
	FaNfaToDfa(FaGrammar &nfaT, FaSymbolTable *symTabT) : nfa(nfaT), symTab(symTabT) { }
	std::string nfaToDfa();
	
	typedef std::set<const std::string *>::iterator start_sym_iterator;
	start_sym_iterator start_sym_begin(const std::string *s) { return startSyms[s].begin(); }
	start_sym_iterator start_sym_end(const std::string *s) { return startSyms[s].end(); }

	private:
	std::string removeLambdas();
	void determineStartSyms();
	std::string doNfaToDfa();
	bool nfaIsDfa(FaNfa &dfa);
	bool removeLambdaLoops();
	bool hasCycle(FaNfa &fa, unsigned src, unsigned target);
	void detectCycle(FaNfa &fa, unsigned src, std::set<unsigned> &visited, bool &cycle, unsigned stop);
	bool doRemoveLambdaLoop(FaNfa &fa, class FaSetList &lst);
};


#endif	// l_falib_h__ceres_included

// testbuildc.cpp
// Copyright (C) 2009 Willow Schlanger

#include "out_chdr.h"
#include "token_enums.h"
#include "out_keywords.h"
#include "nfadfa.h"

#include <list>
#include <cstddef>
#include <cassert>
#include <iostream>

struct CNode
{
	std::list<CNode *> children;
	int value;		// if children.empty()

	bool accept;	
	CNode()
	{
		accept = false;
	}
	
	void print()
	{
		if(accept)
			std::cout << "*";
		if(children.empty())
		{
			std::cout << value << " ";
		}
		else
		{
			std::cout << "(";
			for(std::list<CNode *>::iterator i = children.begin(); i != children.end(); ++i)
			{
			
				(*i)->print();
			}
			std::cout << ") ";
		}
	}
};

bool isTerminal(int x)
{
	if(x >= 0x20000 && x <= 0x2ffff)
		return false;	// these are nonterminals
	return true;	// everything else we treat like a terminal
}

struct CFA
{
	const char *ruleName;
	NewQDFA dfa;
	bool unknownStartSymbols;
	std::set<int> startSymbols;
	
	CFA()
	{
		unknownStartSymbols = true;
	}
	
	void printStartSymbols()
	{
		/*for(std::set<int>::iterator i = startSymbols.begin();
			i != startSymbols.end(); ++i
		)
		{
			std::cout << " ";
			if(*i >= 33 && *i <= 127)
				std::cout << ((char)(*i));
			else
				std::cout << std::hex << (unsigned)(*i) << std::dec;
		}*/
	}
};

struct CBuilder :
	public CTokenEnums
{
	std::list<CNode> nodes;
	CNode *root;
	NFA *nfa;
	const char *acceptStr;
	unsigned acceptFlags;
	int acceptRule;
	
	std::map<int, CFA> m;
	
	void build()
	{
		root = NULL;
		nfa = NULL;
#		include "out_cbld.h"
		finish();
	}
	
	// We have a map of int -> DFAs.
	// Make sure the DFAs are deterministic.
	// This is done by determining the start symbols for each production.
	// Print these symbols to the screen.
	// This builds out_parser.h.
	void finish()
	{
		discoverStartSymbols();
		
		std::ofstream fo("out_parser.h");
		
		fo << "// out_parser.h" << std::endl;
		fo << "// " << copyright << std::endl;
		fo << std::endl;
		
		for(std::map<int, CFA>::iterator i = m.begin(); i != m.end(); ++i)
		{
			//std::cout << i->second.ruleName;
			//i->second.printStartSymbols();
			//std::cout << std::endl;
			
			fo << std::endl;
			fo << "ParseNode *parse_" << i->second.ruleName << "()" << std::endl;
			fo << "{" << std::endl;
			fo << "\tParseNode *node = new ParseNode(" << i->second.ruleName << ", getLineNum(), getFileNum());" << std::endl;
			fo << "\ttry" << std::endl;
			fo << "\t{" << std::endl;
			fo << "\t\tgoto L_" << std::hex << ((unsigned)(&(i->second.dfa.transitionMap[i->second.dfa.start]))) << std::dec << ";" << std::endl;
			
			NewQDFA &dfa = i->second.dfa;
			for(std::map<QDFASet, NewQDFAItems>::iterator j = dfa.transitionMap.begin(); j != dfa.transitionMap.end(); ++j)
			{
				fo << "\t\tL_" << std::hex << ((unsigned)(&(j->second))) << std::dec << ":" << std::endl;
				
				fo << "\t\tswitch(getCurrentToken())" << std::endl;
				fo << "\t\t{" << std::endl;
				for(std::map<int, QDFASet>::iterator kk = j->second.targets.begin(); kk != j->second.targets.end(); ++kk)
				{
					std::set<int> s;
					
					if(isTerminal(kk->first))
						s.insert(kk->first);
					else
					{
						std::set<int> &ss = m[kk->first].startSymbols;
						for(std::set<int>::iterator h = ss.begin(); h != ss.end(); ++h)
							s.insert(*h);
					}
					
					for(std::set<int>::iterator k = s.begin(); k != s.end(); ++k)
					{				
						if(*k >= 33 && *k <= 127 && *k != '\'')
							fo << "\t\t\tcase \'" << ((char)(*k)) << "\'" << ":" << std::endl;
						else
							fo << "\t\t\tcase 0x" << std::hex << *k << std::dec << ":" << std::endl;
					}
					
					if(isTerminal(kk->first))
					{
						fo << "\t\t\t\taddTerminal(node);" << std::endl;
						fo << "\t\t\t\tadvanceToken();" << std::endl;
					}
					else
					{
						// Nonterminal! Must call a function.
						const char *rule = m[kk->first].ruleName;
						fo << "\t\t\t\taddNonterminal(node, parse_" << rule << "());" << std::endl;
					}
					fo << "\t\t\t\tgoto L_" << std::hex << ((unsigned)(&(dfa.transitionMap[kk->second]))) << std::dec << ";" << std::endl;
				}
				
				// Handle accept states here.
				fo << "\t\t\tcase 0xffffffff:" << std::endl;
				fo << "\t\t\tdefault:" << std::endl;
				if(j->second.accept)
					fo << "\t\t\t\treturn node;\t// accept state" << std::endl;
				else
					fo << "\t\t\t\tthrow ParseError(\"syntax error\");" << std::endl;
				// This break is bogus - does compiler care?
				fo << "\t\t\t\tbreak;" << std::endl;
				
				fo << "\t\t}" << std::endl;
				
			}
			
			fo << "\t}" << std::endl;
			fo << "\tcatch(ParseError &e)" << std::endl;
			fo << "\t{" << std::endl;
			fo << "\t\tdelete node;" << std::endl;
			fo << "\t\tthrow e;" << std::endl;
			fo << "\t}" << std::endl;
			fo << "\treturn NULL;" << std::endl;
			fo << "}" << std::endl;
		}
		fo << std::endl;
		
		std::cout << m.size() << " rules were meta-compiled" << std::endl;
	}
	
	void discoverStartSymbols()
	{
		bool again = true;
		while(again)
		{
			again = false;
			for(std::map<int, CFA>::iterator i = m.begin(); i != m.end(); ++i)
				again |= doDiscoverStartSymbols(i->second);
		}
		
		// Make sure we really have a DFA.
		for(std::map<int, CFA>::iterator i = m.begin(); i != m.end(); ++i)
		{
			std::set<int> starters;
			CFA &cfa = i->second;
			NewQDFAItems &startItems = cfa.dfa.transitionMap[cfa.dfa.start];
			for(std::map<int, QDFASet>::iterator i = startItems.targets.begin();
				i != startItems.targets.end(); ++i
			)
			{
				if(isTerminal(i->first))
				{
					starters.insert(i->first);
				}
			}
			for(std::map<int, QDFASet>::iterator i = startItems.targets.begin();
				i != startItems.targets.end(); ++i
			)
			{
				if(!isTerminal(i->first))
				{
					// Go thru all the start symbols for i->first.
					// Make sure they are unique.
					CFA &cfaNt = m[i->first];
					for(std::set<int>::iterator ii = cfaNt.startSymbols.begin();
						ii != cfaNt.startSymbols.end(); ++ii
					)
					{
						if(starters.find(*ii) != starters.end())
						{
							std::cout << "Error with rule " << cfa.ruleName << ": ambiguous grammar on ";
							if(*ii >= 33 && *ii <= 127)
								std::cout << ((char)(*ii));
							else
								std::cout << *ii;
							std::cout << " - " << cfaNt.ruleName;
							std::cout << std::endl;
							throw -1;
						}
						starters.insert(*ii);
					}
				}
			}
		}
	}
	
	// This is broken - why do some productions have NO start symbols?
	bool doDiscoverStartSymbols(CFA &cfa)
	{
		bool again = false;
		
		NewQDFAItems &startItems = cfa.dfa.transitionMap[cfa.dfa.start];
		
		for(std::map<int, QDFASet>::iterator i = startItems.targets.begin();
			i != startItems.targets.end(); ++i
		)
		{
			if(isTerminal(i->first))
			{
				cfa.startSymbols.insert(i->first);
			}
		}
		
		for(std::map<int, QDFASet>::iterator i = startItems.targets.begin();
			i != startItems.targets.end(); ++i
		)
		{
			if(!isTerminal(i->first))
			{
				// i->first is a nonterminal.
				assert(m.find(i->first) != m.end());
				CFA &cnfaNt = m[i->first];
				if(cnfaNt.unknownStartSymbols)
					again = true;
				else
				{
					for(std::set<int>::iterator ii = cnfaNt.startSymbols.begin();
						ii != cnfaNt.startSymbols.end(); ++ii
					)
						cfa.startSymbols.insert(*ii);
					/*
					NewQDFAItems &startItems = cnfaNt.dfa.transitionMap[cnfaNt.dfa.start];
					for(std::map<int, QDFASet>::iterator ii = startItems.targets.begin();
						ii != startItems.targets.end(); ++ii
					)
					{
						if(isTerminal(ii->first))
						{
							cfa.startSymbols.insert(ii->first);
						}
					}
					*/
				}
			}
		}
		
		cfa.unknownStartSymbols = again;
		return again;
	}
	
	std::list<int> tmp;
	bool inRep;
	
	void beginRule(int rule, const char *ruleText)
	{
		nfa = new NFA();
		acceptRule = rule;
		acceptStr = ruleText;
		acceptFlags = 0;
	}
	
	void endRule()
	{
		if(root != NULL)
			update();
		useNfa();
		delete nfa;
		nfa = NULL;
		root = NULL;
		nodes.clear();
	}
	
	// next alternative of the production.
	void reset()
	{
		if(root != NULL)
			update();
		inRep = false;
		root = NULL;
		nodes.clear();
		nodes.push_back(CNode());
		root = &nodes.back();
	}
	
	// This is called when a tree has built.
	void update()
	{
		assert(root != NULL);
		setAccepts();
		
		//root->print();
		//std::cout << std::endl;
		
		//--
		NFAState *next = nfa->getStartState();
		for(std::list<CNode *>::iterator i = root->children.begin();
			i != root->children.end(); ++i
		)
		{
			CNode *node = *i;
			if(node->children.empty())
			{
				NFAState *tmp = nfa->newState();
				next->addEdge(tmp, node->value);
				if(node->accept)
					tmp->makeFinal(acceptStr, acceptFlags);
				next = tmp;
			}
			else
			{
				NFAState *tmp = next;
				std::list<CNode *>::iterator j = node->children.begin();
				for(unsigned i = 1; i < node->children.size(); ++i, ++j)
				{
					NFAState *tmp2 = nfa->newState();
					tmp->addEdge(tmp2, (*j)->value);
					tmp = tmp2;
				}
				tmp->addEdge(next, node->children.back()->value);
			}
		}
		//--
	}
	
	void useNfa()
	{
		NewQDFA dfa;
		buildTransitionMap(dfa);
		NFA *tmpNFA = nfa;
		NFA &nfa = *tmpNFA;
		
		//---
		NewQDFA &output = nfa.newQDFA;
		QDFASet startState;
		startState.state.insert(nfa.getStartState());
	
//	std::cout << "---begin NFA---" << std::endl;
		std::set<QDFASet> wanted;
		wanted.insert(startState);
		output.start = startState;
		while(!wanted.empty())
		{
			std::set<QDFASet>::iterator j = wanted.begin();
			QDFASet js = *j;
			wanted.erase(j);
	
			// 'js' has a set of QNFAState pointers.
			if(output.transitionMap.find(js) != output.transitionMap.end())
			{
				std::cout << "NFA to DFA - internal error" << std::endl;
				throw -1;
			}
			output.transitionMap[js] = NewQDFAItems();
			NewQDFAItems &io = output.transitionMap[js];
	
			bool accept = false;
			std::string finalStr = "";
			unsigned finalFlags = 0;
	
//[+]
			// 'js' is a set of QNFAState pointers that we want.
			// Look at each row in 'js'.
			for(std::set<QNFAState *>::iterator k = js.state.begin(); k != js.state.end();
				++k
			)
			{
				if((*k)->isFinal)
				{
					if(accept)
					{
						if(finalStr != (*k)->finalStr || finalFlags != (*k)->finalFlags)
						{
							std::cout << "Ambiguous accept string or flags!" << std::endl;
							throw -1;
						}
					}
					accept = true;
					finalStr = (*k)->finalStr;
					finalFlags = (*k)->finalFlags;
	//std::cout << "accept:";
				}
	//std::cout << std::hex << *k << std::dec << " ";
	
				// Now for each row in 'js', go thru each column.
				for(std::map<int, QNFATarget >::iterator h = (*k)->transitionMap.begin();
					h != (*k)->transitionMap.end(); ++h
				)
				{
					for(std::set<QNFAState *>::iterator hh = h->second.targets.begin();
						hh != h->second.targets.end(); ++hh
					)
					{
						io.targets[h->first].state.insert(*hh);
					}
				}
			}
	
			// Now look at io.targets[x] for all x.
			for(std::map<int, QDFASet>::iterator xi = io.targets.begin();
				xi != io.targets.end(); ++xi
			)
			{
				QDFASet &xt = xi->second;
				if(output.transitionMap.find(xt) == output.transitionMap.end())
				{
					wanted.insert(xt);
				}
			}
	
			if(accept)
			{
				io.accept = true;
				io.finalStr = finalStr;
				io.finalFlags = finalFlags;
			}
		}
//std::cout << std::endl;
//std::cout << "--- end NFA ---" << std::endl;
	
//generateOutput();
		//---
		
		useDfa(output);
	}
	
	void useDfa(NewQDFA &output)
	{
		m[acceptRule].ruleName = acceptStr;
		m[acceptRule].dfa = output;
	}
	
	void setAccepts()
	{
		std::list<CNode *>::iterator i = root->children.end();
		if(i == root->children.begin())
			return;
		for(;;)
		{
			--i;
			
			if((*i)->children.empty())
			{
				(*i)->accept = true;
				break;
			}
			
			(*i)->children.back()->accept = true;
			
			if(i == root->children.begin())
				break;
		}
	}
	
	void add(int value)
	{
		if(inRep)
			tmp.push_back(value);
		else
		{
			// add 'value' here.
			nodes.push_back(CNode());
			nodes.back().value = value;
			root->children.push_back(&nodes.back());
		}
	}
	
	void beginRep()
	{
		tmp.clear();
		inRep = true;
	}
	
	void endRep()
	{
		// add 'tmp' here.
		nodes.push_back(CNode());
		CNode *node = &nodes.back();
		node->value = 0;	// not used
		assert(!tmp.empty());
		for(std::list<int>::iterator i = tmp.begin(); i != tmp.end(); ++i)
		{
			nodes.push_back(CNode());
			nodes.back().value = *i;
			node->children.push_back(&nodes.back());
		}
		root->children.push_back(node);
		
		tmp.clear();
		inRep = false;
	}

	void buildTransitionMap(NewQDFA &dfa)
	{
		for(std::list<NFAState *>::iterator i = nfa->allStates.begin(); i != nfa->allStates.end(); ++i)
		{
			for(std::map<int, std::set<NFAState *> >::iterator j = (*i)->edges.begin(); j != (*i)->edges.end(); ++j)
			{
				for(std::set<NFAState *>::iterator k = j->second.begin(); k != j->second.end(); ++k)
				{
					(*i)->transitionMap[j->first].targets.insert(*k);
				}
			}
		}
	}
};

int main()
{
	CBuilder cbldr;
	try
	{
		cbldr.build();
	}
	catch(int)
	{
		std::cout << "Giving up." << std::endl;
		return 1;
	}
	return 0;
}

// falib.cpp - Finite Automaton Library
// Copyright (C) 2009 Willow Schlanger
// The purpose of this file is to represent an NFA,
// remove lambda transitions, then convert it to a DFA.

// See falib.h for usage information.

#include "falib.h"

void FaNfaToDfa::detectCycle(FaNfa &fa, unsigned src, std::set<unsigned> &visited, bool &cycle, unsigned stop)
{
	if(visited.find(src) != visited.end())
		return;
	visited.insert(src);
	FaState &state = fa.states[src];
	for(FaState::edge_iterator edge = state.begin(); edge != state.end(); ++edge)
	{
		if(edge->getSymbol() == symTab->getLambda())
		{
			if(edge->getTarget() == stop)
			{
				cycle = true;
				return;
			}
			detectCycle(fa, edge->getTarget(), visited, cycle, stop);
		}
	}
}

// Given an FA, and a src state which has an arc to a target state, this does a depth-first-search on the
// target state. If it ever returns to the start state, a cycle is detected and we return true; else false.
bool FaNfaToDfa::hasCycle(FaNfa &fa, unsigned src, unsigned target)
{
	FaState &state = fa.states[target];
	for(FaState::edge_iterator edge = state.begin(); edge != state.end(); ++edge)
	{
		bool cycle = false;
		std::set<unsigned> visited;
		detectCycle(fa, edge->getTarget(), visited, cycle, src);
		if(cycle)
			return true;
	}
	return false;
}

// This is used internally by FaNfaToDfa::removeLambdaLoops().
class FaSetList
{
	public:
	// Elements in this list are sets of at least 2 elements.
	std::list<std::set<unsigned> > mergers;

	FaSetList(std::list<std::pair<unsigned, unsigned> > &joints)
	{
		for(std::list<std::pair<unsigned, unsigned> >::iterator i = joints.begin(); i != joints.end(); ++i)
			merge(i->first, i->second);
		
		/*for(std::list<std::set<unsigned> >::iterator i = mergers.begin(); i != mergers.end(); ++i)
		{
			std::cout << "loop:";
			for(std::set<unsigned>::iterator j = i->begin(); j != i->end(); ++j)
				std::cout << " " << *j;
			std::cout << std::endl;
		}*/
	}
	
	// Given a state in the FA in question, this returns an iterator into 'mergers' for a set that
	// contains that state, or mergers.end() if the state need not be merged with any other states.
	std::list<std::set<unsigned> >::iterator find(unsigned state)
	{
		for(std::list<std::set<unsigned> >::iterator i = mergers.begin(); i != mergers.end(); ++i)
		{
			if(i->find(state) != i->end())
				return i;
		}
		return mergers.end();
	}
	
	private:
	void merge(unsigned a, unsigned b)
	{
		for(std::list<std::set<unsigned> >::iterator i = mergers.begin(); i != mergers.end(); ++i)
		{
			if(i->find(a) != i->end())
			{
				i->insert(b);
				return;
			}
			if(i->find(b) != i->end())
			{
				i->insert(a);
				return;
			}
		}
		// Disjoint relative to existing items.
		mergers.push_back(std::set<unsigned>());
		mergers.back().insert(a);
		mergers.back().insert(b);
	}
};

bool FaNfaToDfa::doRemoveLambdaLoop(FaNfa &fa, FaSetList &lst)
{
	for(std::list<std::set<unsigned> >::iterator i = lst.mergers.begin(); i != lst.mergers.end(); ++i)
	{
		// Sanity checks.
		if(i->find(fa.getStartState()) != i->end())
			return false;	// start state cannot be part of a lambda loop - not supported!
		if(i->size() < 2)
			return false;
		
		// Look at *i states.
		int accept = -1;	// unknown
		for(std::set<unsigned>::iterator j = i->begin(); j != i->end(); ++j)
		{
			if(fa.isFinal(*j))
			{
				if(accept == -1)
					accept = 1;
				else
				if(accept != 1)
					return false;
			}
			else
			{
				if(accept == -1)
					accept = 0;
				else
				if(accept != 0)
					return false;
			}
		}
		if(accept == -1)
			return false;	// unable to determine whether is accept or not
		
		// Create new state, make accept state if necessary.
		unsigned dest = fa.addState();
		if(accept == 1)
			fa.makeFinal(dest);
		
		// Go thru out edges from each state in the merger, copy/translate
		// to new state.
		for(std::set<unsigned>::iterator j = i->begin(); j != i->end(); ++j)
		{
			FaState &st = fa.states[*j];
			for(FaState::edge_iterator k = st.begin(); k != st.end(); ++k)
			{
				const std::string *symbol = k->getSymbol();
				unsigned target = k->getTarget();
				if(i->find(target) != i->end())
				{
					// Arc to ourself. Ignore lambdas to ourself.
					if(symbol != symTab->getLambda())
					{
						fa.addEdge(dest, dest, symbol);
					}
				}
				else
				{
					fa.addEdge(dest, target, symbol);
				}
			}
		}
		
		// Find arcs to the set of states we've merged, move them to point to
		// the new state.
		for(unsigned j = 0; j < fa.states.size(); ++j)
		{
			FaState &st = fa.states[j];
			for(FaState::edge_iterator k = st.begin(); k != st.end(); ++k)
			{
				///const std::string *symbol = k->getSymbol();
				unsigned target = k->getTarget();
				if(i->find(target) != i->end())
				{
					// Found an arc that goes to a state in our old set.
					k->setTarget(dest);
				}
			}
		}
	}
	return true;
}

// This removes lambda loops. It's used after accept states have been back-propagated.
// It merges states that are part of lambda loops. When the merger is done, ALL states
// in the merger must be accept states or ALL must be non-accept states.
// We return error if the START STATE (i.e. state 0) is part of a lambda loop. This
// should not happen because we have lambda transitions from the start state to a new
// state that starts each alternate.
bool FaNfaToDfa::removeLambdaLoops()
{
	for(FaGrammar::iterator i = nfa.begin(); i != nfa.end(); ++i)
	{
		FaNfa &fa = *i->second;
		
		// Collect information on lambda-loop cycles.
		std::list<std::pair<unsigned, unsigned> > joints;
		for(unsigned state = 0; state < fa.states.size(); ++state)
		{
			FaState &st = fa.states[state];
			for(FaState::edge_iterator edge = st.begin(); edge != st.end(); ++edge)
			{
				if(edge->getSymbol() == symTab->getLambda())
				{
					// There is a lambda arc from 'state' to edge->getTarget().
					unsigned state2 = edge->getTarget();
					if(hasCycle(fa, state, state2))
					{
						joints.push_back(std::pair<unsigned, unsigned>(state, state2));
					}
				}
			}
		}
		
		// Process lambda-loop cycle information.
		FaSetList lst(joints);
		
		if(!doRemoveLambdaLoop(fa, lst))
			return false;
	}
	return true;
}

std::string FaNfaToDfa::removeLambdas()
{
/*std::cout << "+" << std::endl;
for(FaGrammar::iterator i = nfa.begin(); i != nfa.end(); ++i)
{
FaNfa &fa = *i->second;
fa.debugDump();
}*/
	for(FaGrammar::iterator i = nfa.begin(); i != nfa.end(); ++i)
	{
		std::string rulename = *i->first;
		FaNfa &fa = *i->second;
		bool changed;
		
		// Back-propagate accept states.
		do
		{
			changed = false;
			for(unsigned state = 0; state < fa.states.size(); ++state)
			{
				FaState &st = fa.states[state];
				for(FaState::edge_iterator edge = st.begin(); edge != st.end(); ++edge)
				{
					if(edge->getSymbol() == symTab->getLambda())
					{
						FaState &target = fa.states[edge->getTarget()];
						if(target.getAccept() && !st.getAccept())
						{
							changed = true;
							st.setAccept(true);
						}
					}
				}
			}
		}	while(changed);
		
		if(!removeLambdaLoops())
			return rulename;

		// Match (lambda)-x's. x may be another lambda.
		do
		{
			changed = false;
			for(unsigned state = 0; state < fa.states.size(); ++state)
			{
				FaState &st = fa.states[state];
				if(st.begin() == st.end())
					continue;	// bugfix 05/14/2010
				for(FaState::edge_iterator edge = st.begin(); ;)
				{
					FaEdge &ed = *edge;
					FaState::edge_iterator old = edge;
					++edge;
					
					if(ed.getSymbol() == symTab->getLambda())
					{
						changed = true;
						unsigned targetNum = ed.getTarget();
						FaState &target = fa.states[targetNum];
						st.eraseEdge(old);
						
						for(FaState::edge_iterator edge2 = target.begin(); edge2 != target.end(); ++edge2)
							fa.addEdge(state, edge2->getTarget(), edge2->getSymbol());
					}
					
					if(edge == st.end())
						break;
				}
			}
		}	while(changed);
		
		// Verify start state is not accept state.
		if(fa.states[fa.getStartState()].getAccept())
			return rulename;
	}

/*std::cout << "-" << std::endl;
for(FaGrammar::iterator i = nfa.begin(); i != nfa.end(); ++i)
{
FaNfa &fa = *i->second;
fa.debugDump();
}*/

	return "";	// success
}

// Pre: the system contains no lambda's. The start state is not an accept state.
// This determines the set of start symbols for each rule.
void FaNfaToDfa::determineStartSyms()
{
	// First generate a basic set of start symbols, excluding rule symbols.
	for(FaGrammar::iterator i = nfa.begin(); i != nfa.end(); ++i)
	{
		FaNfa &fa = *i->second;
		FaState &st = fa.states[fa.getStartState()];
		for(FaState::edge_iterator edge = st.begin(); edge != st.end(); ++edge)
		{
			unsigned x = symTab->getNum(*edge->getSymbol());
			if(x < 0x30000 || x > 0x3ffff)
				startSyms[i->first].insert(edge->getSymbol());
		}
	}
	
	// Now deal with rule start symbols.
	bool changed;
	do
	{
		changed = false;
		
		for(FaGrammar::iterator i = nfa.begin(); i != nfa.end(); ++i)
		{
			FaNfa &fa = *i->second;
			FaState &st = fa.states[fa.getStartState()];
			for(FaState::edge_iterator edge = st.begin(); edge != st.end(); ++edge)
			{
				unsigned sym = symTab->getNum(*edge->getSymbol());
				if(sym > 0x30000 && sym <= 0x3ffff)
				{
					// *edge->getSymbol() is a rule name; Look it up.
					std::set<const std::string *> &rule = startSyms[edge->getSymbol()];
					
					std::set<const std::string *> &dest = startSyms[i->first];
					for(std::set<const std::string *>::iterator i = rule.begin(); i != rule.end(); ++i)
					{
						if(dest.find(*i) == dest.end())
						{
							changed = true;
							dest.insert(*i);
						}
					}
				}
			}
		}
		
	}	while(changed);
}

// This returns false if there was an error.
std::string FaNfaToDfa::nfaToDfa()
{
	dfa.clear();

	// 1- remove lambda transitions from each NFA
	//  a. propagate back accept states
	//  b. match (lambda)x's
	//  c. if the start state is an accept state, return error!
	std::string err = removeLambdas();
///if(!err.empty()) std::cerr << "[1:" << err << "]";
	if(!err.empty())
		return err;

	// 2- determine start symbols for each rule in the grammar
	//  a. repeat until nothing new is learned by an iteration
	//  b. for each iteration, for a ::= X b we want to add X to a.
	//  c. for each iteration, for a ::= c we want to look up c's start symbols.
	determineStartSyms();

	// 3- do actual conversion.
	err = doNfaToDfa();
///if(!err.empty()) std::cerr << "[2:" << err << "]";
	return err;
}

// FaStateSet - internal to implementation.
struct FaStateSet
{
	std::set<unsigned> state;
	
	bool operator!=(const FaStateSet &rhs) const
	{
		if(*this < rhs || rhs < *this)
			return true;
		return false;
	}

	bool operator<(const FaStateSet &rhs) const
	{
		const FaStateSet &lhs = *this;
		std::set<unsigned>::const_iterator li = lhs.state.begin();
		std::set<unsigned>::const_iterator ri = rhs.state.begin();

		while(!(li == lhs.state.end() && ri == rhs.state.end()))
		{
			// They are not both at the end.
			if(li == lhs.state.end())
			{
				// lhs : [a, b]
				// rhs : [a, b, c]
				return true;
			}
			if(ri == rhs.state.end())
			{
				// lhs : [a, b, c]
				// rhs : [a, b]
				return false;
			}
			unsigned lv = *li;
			unsigned rv = *ri;
			
			if(lv < rv)
				return true;
			if(rv < lv)
				return false;

			++li;
			++ri;
		}

		return false;	// they are both equal
	}
};

std::string FaNfaToDfa::doNfaToDfa()
{
	std::string curname;
	for(FaGrammar::iterator i = nfa.begin(); i != nfa.end(); ++i)
	{
		curname = *i->first;
		FaNfa &fa = *i->second;
		std::set<FaStateSet> need;
		FaStateSet startSet;
		startSet.state.insert(fa.getStartState());
		need.insert(startSet);
		std::map<FaStateSet, unsigned> done;
		std::set<FaStateSet> acceptors;
		
		// Create rule in DFA.
		dfas.push_back(FaNfa(symTab));
		FaNfa *out_dfa = dfa[i->first] = &dfas.back();
		
		// Create 'done' states.
		while(!need.empty())
		{
			FaStateSet tmpSet = *need.begin();
			need.erase(need.begin());
			unsigned st = fa.getStartState();
			if(tmpSet != startSet)
				st = out_dfa->addState();
			done[tmpSet] = st;
			
			// Get list of out-edge symbols.
			std::set<const std::string *> outsym;
			for(std::set<unsigned>::iterator j = tmpSet.state.begin(); j != tmpSet.state.end(); ++j)
			{
				FaState &x = fa.states[*j];
				for(FaState::edge_iterator k = x.begin(); k != x.end(); ++k)
				{
					outsym.insert(k->getSymbol());
				}
			}
			
			for(std::set<const std::string *>::iterator j = outsym.begin(); j != outsym.end(); ++j)
			{
				FaStateSet dset;
				
				for(std::set<unsigned>::iterator h = tmpSet.state.begin(); h != tmpSet.state.end(); ++h)
				{
					FaState &x = fa.states[*h];
					for(FaState::edge_iterator k = x.begin(); k != x.end(); ++k)
					{
						if(k->getSymbol() == *j)
							dset.state.insert(k->getTarget());
					}
				}
				
				if(!dset.state.empty() && done.find(dset) == done.end())
					need.insert(dset);
			}
			
			// update 'acceptors' here.
			bool accept = false;
			for(std::set<unsigned>::iterator j = tmpSet.state.begin(); j != tmpSet.state.end(); ++j)
			{
				accept |= fa.states[*j].getAccept();
			}
			if(accept)
				acceptors.insert(tmpSet);
		}
		
		// Create DFA, itself. The states have been created already, now we just need to make some
		// of them accept states and add in transitions.
		for(std::map<FaStateSet, unsigned>::iterator i = done.begin(); i != done.end(); ++i)
		{
			FaStateSet in_set = i->first;
			FaStateSet &tmpSet = in_set;
			unsigned out_index = i->second;
			///FaState &out_state = out_dfa->states[out_index];
			if(acceptors.find(in_set) != acceptors.end())
				out_dfa->makeFinal(out_index);
			
			//out_dfa->addEdge(unsigned from, unsigned target, const std::string *symbol);
			
			// Get list of out-edge symbols.
			std::set<const std::string *> outsym;
			for(std::set<unsigned>::iterator j = tmpSet.state.begin(); j != tmpSet.state.end(); ++j)
			{
				FaState &x = fa.states[*j];
				for(FaState::edge_iterator k = x.begin(); k != x.end(); ++k)
				{
					outsym.insert(k->getSymbol());
				}
			}
			
			// Create out edge in DFA.
			for(std::set<const std::string *>::iterator j = outsym.begin(); j != outsym.end(); ++j)
			{
				FaStateSet dset;
				
				for(std::set<unsigned>::iterator h = tmpSet.state.begin(); h != tmpSet.state.end(); ++h)
				{
					FaState &x = fa.states[*h];
					for(FaState::edge_iterator k = x.begin(); k != x.end(); ++k)
					{
						if(k->getSymbol() == *j)
							dset.state.insert(k->getTarget());
					}
				}
				
				if(!dset.state.empty())
					out_dfa->addEdge(done[in_set], done[dset], *j);
			}
		}
		
		// Check that we really have a DFA. In particular, verify that edges that are
		// rules do not conflict with each other or with any other edges from the same
		// state.
		bool isdfa = nfaIsDfa(*out_dfa);
		if(!isdfa)
		{
///std::cerr << "--- begin dump [" << curname << "] ---" << std::endl;
///out_dfa->debugDump();
			return curname;
		}
	}	// end for loop
	
	return "";	// success
}

bool FaNfaToDfa::nfaIsDfa(FaNfa &dfa)
{
	for(FaNfa::states_iterator i = dfa.states.begin(); i != dfa.states.end(); ++i)
	{
		std::set<unsigned> syms;
		for(FaState::edge_iterator j = i->begin(); j != i->end(); ++j)
		{
			const std::string *symbol = j->getSymbol();
			unsigned value = symTab->getNum(*symbol);
			if(value > 0x30000 && value <= 0x3ffff)
			{
				// symbol is a rule.
				std::set<const std::string *> &starters = startSyms[symbol];
				for(std::set<const std::string *>::iterator k = starters.begin(); k != starters.end(); ++k)
				{
					const std::string *symbol2 = *k;
					unsigned value2 = symTab->getNum(*symbol2);
					if(value2 == 0x30000 || (value2 > 0x30000 && value2 <= 0x3ffff))
					{
///std::cerr << "starters contains this: " << *symbol2 << "!!!" << std::endl;
						return false;	// internal error! (sanity check).
					}
					if(syms.find(value2) == syms.end())
						syms.insert(value2);
					else
					{
///std::cerr << "(alpha conflict here: " << j->getTarget() << ")" << std::endl;
						return false;	// duplicate out edge symbol detected on account of nonterminal!
					}
				}
			}
			else
			{
				if(syms.find(value) == syms.end())
					syms.insert(value);
				else
				{
///std::cerr << "(beta conflict here: " << j->getTarget() << ")" << std::endl;
					return false;	// duplicate out edge symbol detected in DFA (should never happen)
				}
			}
		}
	}

///std::cerr << "(returning true)" << std::endl;
	return true;
}

// ---

FaNfa::FaNfa(FaSymbolTable *symTabT)
{
	symTab = symTabT;
	states.push_back(FaState());	// state 0 is start state
}

void FaNfa::addEdge(unsigned from, unsigned target, const std::string *symbol)
{
	states[from].insert(symbol, target);
}

// This creates a new state with a lambda transition from the start state to it.
// The new state index is returned.
unsigned FaNfa::addProduction()
{
	unsigned x = static_cast<unsigned>(states.size());
	states.push_back(FaState());
	states[getStartState()].insert(symTab->getLambda(), x);
	return x;
}

unsigned FaNfa::addState()
{
	unsigned x = static_cast<unsigned>(states.size());
	states.push_back(FaState());
	return x;
}

// ---

FaSymbolTable::FaSymbolTable()
{
	symTab[""] = FA_LAMBDA;
	numTokens = 1;
	numKeywords = 0;
	numRules = 0;
}

unsigned FaSymbolTable::getNum(std::string symbol)
{
	std::map<std::string, unsigned>::iterator i = symTab.find(symbol);
	if(i != symTab.end())
		return i->second;
	unsigned value;
	if(symbol[0] == '\'')
		value = static_cast<unsigned char>(symbol[1]);
	else
	if(symbol[0] == '\"')
		value = 0x20000 + numKeywords++;
	else
	if(symbol[0] >= 'A' && symbol[0] <= 'Z')
		value = 0x10000 + numTokens++;
	else
		value = 0x30000 + numRules++;
	symTab[symbol] = value;
	return value;
}

const std::string *FaSymbolTable::getSym(std::string symbol)
{
	getNum(symbol);
	sym_tab_iterator i = symTab.find(symbol);
	return &i->first;
}

// nfadfa.h
// Copyright (C) 2009 Willow schlanger

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <cstddef>

typedef class NFAState QNFAState;
typedef unsigned long UINT;	// sizeof(UINT) must equal sizeof(void *).

const char copyright[] = "Copyright (C) 2009 Willow Schlanger";

struct QDFASet
{
	// 'state' must not contain any NULL pointers.
	// It is a set of QNFAState pointers.
	std::set<QNFAState *> state;

	bool operator!=(const QDFASet &rhs) const
	{
		if(*this < rhs || rhs < *this)
			return false;
		return true;
	}

	bool operator<(const QDFASet &rhs) const
	{
		const QDFASet &lhs = *this;
		std::set<QNFAState *>::const_iterator li = lhs.state.begin();
		std::set<QNFAState *>::const_iterator ri = rhs.state.begin();

		while(!(li == lhs.state.end() && ri == rhs.state.end()))
		{
			QNFAState *lv = NULL;
			QNFAState *rv = NULL;

			if(li != lhs.state.end())
				lv = *li;
			if(ri != rhs.state.end())
				rv = *ri;
			
			if(lv < rv)
				return true;
			if(rv < lv)
				return false;

			// Note: since 'state' contains no NULL pointers, we
			// will be here only when li != lhs.state.end() and
			// ri != rhs.state.end().
			++li;
			++ri;
		}

		return false;	// they are both equal
	}
};

struct NewQDFAItems
{
	bool accept;
	std::string finalStr;
	unsigned finalFlags;
	NewQDFAItems() : accept(false), finalStr(""), finalFlags(0) { }
	std::map<int, QDFASet> targets;
	
	// The following is obsolete - should delete it.
	std::map<int, std::string> abstractSymbols;
};

struct NewQDFA
{
	QDFASet start;
	std::map<QDFASet, NewQDFAItems> transitionMap;
};

struct QNFATarget
{
	std::set<QNFAState *> targets;
};

class NFAState
{
	public:
	bool isFinal;
	std::string finalStr;
	unsigned finalFlags;
	std::map<int, std::set<NFAState *> > edges;
	std::map<int, QNFATarget > transitionMap;
	
	NFAState()
	{
		isFinal = false;
		finalStr.clear();
		finalFlags = 0;
	}
	
	void makeFinal(const char *tokenStr, unsigned flags)
	{
		isFinal = true;
		finalStr = tokenStr;
		finalFlags = flags;
	}
	
	void addEdge(NFAState *target, int symbol)
	{
		edges[symbol].insert(target);
	}
};

class NFA
{
	public:
	NFAState *startState;
	std::list<NFAState *> allStates;
	NewQDFA newQDFA;

	NFA()
	{
		startState = newState();
	}

	NFAState *getStartState()
	{
		return startState;
	}
	
	NFAState *newState()
	{
		NFAState *n = new NFAState();
		allStates.push_back(n);
		return n;
	}
	
	~NFA()
	{
		for(std::list<NFAState *>::iterator i = allStates.begin(); i != allStates.end(); ++i)
		{
			delete *i;
			*i = NULL;
		}
	}
};

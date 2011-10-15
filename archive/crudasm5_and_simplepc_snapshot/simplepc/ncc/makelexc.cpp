// makelexc.cpp
// Copyright (C) 2009 Willow Schlanger
//
// This controls the input symbols, except for these things:
// 1. identifiers (including keywords); remember universal character names too
//    starts with a-z, A-Z, _, or \.
// 2. numbers
//    starts with 0-9 OR '.' followed by 0-9.
// 3. string literals
//    stats with "
// 4. character literals
//    starts with '
// 5. whitespace (incl. comments)
//
// Recognition of the above things is done by hand. Note that in this case we need
// to store user data (such as what the identifier was). There is also whitespace,
// and when a preprocessor has not been invoked there are comments and macros.
// Otherwise we have only these: # <line> "<filename>"

#include "nfadfa.h"

class MakeLex
{
	NFA nfa;
	std::set<std::string> allTokens;
	std::set<std::string> tokenNames;

	public:
	void add1(const char *s, bool cppOnly = false);
	void add2(const char *s, const char *tokenName, bool cppOnly = false);
	void add2(const char *s, char token, bool cppOnly = false);
	void finish();
	
	private:
	void buildTransitionMap(NewQDFA &dfa);
	void generateOutput();
};

void MakeLex::add1(const char *s, bool cppOnly)
{
	char q[2] = {'\0', '\0'};
	for(; *s != '\0'; ++s)
	{
		q[0] = *s;
		add2(q, *s, cppOnly);
	}
}

void MakeLex::add2(const char *s, const char *token, bool cppOnly)
{
	if(allTokens.find(s) != allTokens.end())
	{
		std::cerr << "Duplicate token: " << s << std::endl;
		throw -1;
	}
	allTokens.insert(s);
	if(token[0] != '\'')
	{
		tokenNames.insert(token);
	}

	std::vector<NFAState *> states;
	states.push_back(nfa.getStartState());
	for(unsigned index = 1; s[index] != '\0'; ++index)
	{
		states.push_back(nfa.newState());
	}
	states.push_back(nfa.newState());	// final state
	states.back()->makeFinal(token, (cppOnly) ? 1 : 0);
	for(unsigned index = 0; s[index] != '\0'; ++index)
		states[index]->addEdge(states[index + 1], s[index]);
}

void MakeLex::add2(const char *s, char token, bool cppOnly)
{
	std::string q = std::string("\'") + token + "\'";
	add2(s, q.c_str(), cppOnly);
}

void MakeLex::finish()
{
	for(std::set<std::string>::iterator i = tokenNames.begin(); i != tokenNames.end(); ++i)
	{
		std::cout << "TOKEN_" << *i << "," << std::endl;
	}
	NewQDFA dfa;
	buildTransitionMap(dfa);
	
	NewQDFA &output = nfa.newQDFA;
	QDFASet startState;
	startState.state.insert(nfa.getStartState());

//	std::cerr << "---begin NFA---" << std::endl;
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
			std::cerr << "NFA to DFA - internal error" << std::endl;
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
						std::cerr << "Ambiguous accept string or flags!" << std::endl;
						throw -1;
					}
				}
				accept = true;
				finalStr = (*k)->finalStr;
				finalFlags = (*k)->finalFlags;
//std::cerr << "accept:";
			}
//std::cerr << std::hex << *k << std::dec << " ";

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
//std::cerr << std::endl;
//std::cerr << "--- end NFA ---" << std::endl;

	generateOutput();
}

// This creates a function that returns a token.
// It returns UNKNOWN if there is no match.
// It works like this:
// curOffset = input offset.
// After getting a character, we generally do this:
//   ++curOffset;
//   goto NewLabel;
// Accept states call advanceTo(curOffset).
// We use charAt(curOffset) to get a character.
void MakeLex::generateOutput()
{
	NewQDFA &src = nfa.newQDFA;
	if(src.transitionMap[src.start].accept)
	{
		std::cerr << "Langauge is optional!" << std::endl;
		throw -1;
	}
	
	//std::ostream &os = std::cout;
	std::ofstream os("out_lexc.h");
	os << "// out_lexc.h" << std::endl;
	os << "// " << copyright << std::endl;
	os << std::endl;
	
	os << "goto ";
	os << "L_" << std::hex << (UINT)(&(src.transitionMap[src.start])) <<
		std::dec << ";" << std::endl
	;
	
	for(std::map<QDFASet, NewQDFAItems>::iterator j = src.transitionMap.begin();
		j != src.transitionMap.end(); ++j
	)
	{
		os << "L_" << std::hex << (UINT)(&(j->second)) <<
			std::dec << ":" << std::endl
		;
		NewQDFAItems &items = j->second;
		os << "\tswitch(charAt(curOffset))" << std::endl;
		os << "\t{" << std::endl;
		
		std::set<QDFASet> targets;
		for(std::map<int, QDFASet>::iterator kk = items.targets.begin();
			kk != items.targets.end(); ++kk
		)
		{
			targets.insert(kk->second);
		}

		for(std::set<QDFASet>::iterator hh = targets.begin(); hh != targets.end(); ++hh)
		{
			QDFASet *tmp = NULL;
			for(std::map<int, QDFASet>::iterator k = items.targets.begin();
				k != items.targets.end(); ++k
			)
			{
				if(k->second < *hh || *hh < k->second)
					continue;
				os << "\t\tcase '" << (char)(k->first);
				if(k->first == '\\')
					os << "\\";
				os << "':" << std::endl;
				tmp = &(k->second);
			}
			if(tmp == NULL)
			{
				std::cerr << "Internal error" << std::endl;
				throw -1;
			}
			os << "\t\t\t++curOffset;" << std::endl;
			os << "\t\t\tgoto L_" << std::hex << ((UINT)(&(src.transitionMap[*tmp]))) << std::dec << ";" << std::endl;
		}
		
		if(items.accept)
		{
			os << "\t\tcase ATEOF:" << std::endl;
			os << "\t\tdefault:" << std::endl;
			os << "\t\t\tadvanceTo(curOffset);" << std::endl;
			os << "\t\t\tsetFlags(0x" << std::hex << items.finalFlags << std::dec << ");" << std::endl;
			os << "\t\t\treturn ";
			if(items.finalStr[0] != '\'')
				os << "TOKEN_";
			if(items.finalStr == "\'\\\'")
				os << "\'\\\\\'";
			else
				os << items.finalStr;
			os << ";" << std::endl;
		}
		else
		if(!(j->first < src.start || src.start < j->first))
		{
			os << "\t\tcase ATEOF:" << std::endl;
			os << "\t\t\treturn TOKEN_EOF;" << std::endl;
			os << "\t\tdefault:" << std::endl;
			os << "\t\t\treturn TOKEN_UNKNOWN;" << std::endl;
		}
		else
		{
			os << "\t\tcase ATEOF:" << std::endl;
			os << "\t\t\tunexpectedEndOfFile(curOffset);" << std::endl;
			os << "\t\tdefault:" << std::endl;
			os << "\t\t\tunexpectedChar(curOffset);" << std::endl;
		}

		os << "\t}" << std::endl;
	}

	os << std::endl;
}

void MakeLex::buildTransitionMap(NewQDFA &dfa)
{
	for(std::list<NFAState *>::iterator i = nfa.allStates.begin(); i != nfa.allStates.end(); ++i)
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

int main()
{
	MakeLex ml;
	try
	{
		ml.add1("{}[]#?.();:+-*/%^&|~!=<>,\\");
		ml.add2("##", "NUMNUM");
		ml.add2("<%", '{');
		ml.add2("%>", '}');
		ml.add2("<:", '[');
		ml.add2(":>", ']');
		ml.add2("%:", '#');
		ml.add2("%:%:", "NUMNUM");
		ml.add2("::", "SCOPE", true);
		ml.add2(".*", "DOTSTAR", true);
		ml.add2("+=", "PLUSEQ");
		ml.add2("-=", "MINUSEQ");
		ml.add2("*=", "STAREQ");
		ml.add2("/=", "DIVEQ");
		ml.add2("%=", "MODEQ");
		ml.add2("^=", "XOREQ");
		ml.add2("&=", "ANDEQ");
		ml.add2("|=", "OREQ");
		ml.add2("<<", "SHL");
		ml.add2(">>", "SHR");
		ml.add2("<<=", "SHLEQ");
		ml.add2(">>=", "SHREQ");
		ml.add2("==", "EQEQ");
		ml.add2("!=", "NOTEQ");
		ml.add2("<=", "LE");
		ml.add2(">=", "GE");
		ml.add2("&&", "ANDAND");
		ml.add2("||", "OROR");
		ml.add2("++", "PLUSPLUS");
		ml.add2("--", "MINUSMINUS");
		ml.add2("->*", "POINTSTAR", true);
		ml.add2("->", "POINT");
		ml.add2("...", "DOTDOTDOT");
		ml.finish();
	}
	catch(int x)
	{
		std::cerr << "Giving up" << std::endl;
		return 1;
	}
	return 0;
}

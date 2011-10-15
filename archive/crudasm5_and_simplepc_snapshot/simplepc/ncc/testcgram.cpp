// testcgram.cpp
// Copyright (C) 2009 Willow Schlanger

#include <set>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <cctype>

const char copyright[] = "Copyright (C) 2009 Willow Schlanger";

class Terminals
{
	public:
	std::set<std::string> allTokens;
	std::set<std::string> tokenNames;
	std::map<std::string, bool> allKeywords;

	std::map<std::string, std::string> m2t;
	std::map<std::string, bool> m2c;
	std::map<std::string, std::string> cmap;

	void add1(const char *s, bool cppOnly = false);
	void add2x(const char *s, const char *tokenName, bool cppOnly = false);
	void add2x(const char *s, char token, bool cppOnly = false);
	void addk(const char *keyword, bool cppOnly = false);
	void add2(const char *s, const char *tokenName, bool cppOnly = false)
	{
		add2x(s, tokenName, cppOnly);
		if(cppOnly == false)
			cmap[std::string(s)] = std::string(tokenName);
	}
};

void Terminals::addk(const char *keyword, bool cppOnly)
{
	allKeywords[std::string(keyword)] = cppOnly;
}

void Terminals::add1(const char *s, bool cppOnly)
{
	char q[2] = {'\0', '\0'};
	for(; *s != '\0'; ++s)
	{
		q[0] = *s;
		add2x(q, *s, cppOnly);
	}
}

void Terminals::add2x(const char *s, const char *token, bool cppOnly)
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
	
	m2t[std::string(s)] = token;
	m2c[std::string(s)] = cppOnly;
}

void Terminals::add2x(const char *s, char token, bool cppOnly)
{
	std::string q = std::string("\'") + token + "\'";
	add2x(s, q.c_str(), cppOnly);
}

Terminals ml;

int finish();

int main()
{
	ml.add1("{}[]#?.();:+-*/%^&|~!=<>,\\");
	ml.add2("##", "NUMNUM");
	/*
	These alternates will not appear in the grammar.
	ml.add2("<%", '{');
	ml.add2("%>", '}');
	ml.add2("<:", '[');
	ml.add2(":>", ']');
	ml.add2("%:", '#');
	ml.add2("%:%:", "NUMNUM");
	*/
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
	
	/*
	ml.addk("asm");
	ml.addk("do");
	ml.addk("if");
	ml.addk("return");
	ml.addk("typedef");
	ml.addk("auto");
	ml.addk("double");
	ml.addk("inline");
	ml.addk("short");
	ml.addk("typeid", true);
	ml.addk("bool");
	ml.addk("dynamic_cast", true);
	ml.addk("int");
	ml.addk("signed");
	ml.addk("typename", true);
	ml.addk("break");
	ml.addk("else");
	ml.addk("long");
	ml.addk("sizeof");
	ml.addk("union");
	ml.addk("case");
	ml.addk("enum");
	ml.addk("mutable", true);
	ml.addk("static");
	ml.addk("unsigned");
	ml.addk("catch", true);
	ml.addk("explicit", true);
	ml.addk("namespace", true);
	ml.addk("static_cast", true);
	ml.addk("using", true);
	ml.addk("char");
	ml.addk("export", true);
	ml.addk("new", true);
	ml.addk("struct");
	ml.addk("virtual", true);
	ml.addk("class", true);
	ml.addk("extern");
	ml.addk("operator", true);
	ml.addk("switch");
	ml.addk("void");
	ml.addk("const");
	ml.addk("false", true);
	ml.addk("private", true);
	ml.addk("template", true);
	ml.addk("volatile");
	ml.addk("const_cast", true);
	ml.addk("float");
	ml.addk("protected", true);
	ml.addk("this", true);
	ml.addk("wchar_t", true);	// Is ANSI C ever going to add this?
	ml.addk("continue");
	ml.addk("for");
	ml.addk("public", true);
	ml.addk("throw", true);
	ml.addk("while");
	ml.addk("default");
	ml.addk("friend", true);
	ml.addk("register");
	ml.addk("true", true);
	ml.addk("delete", true);
	ml.addk("goto");
	ml.addk("reinterpret_cast", true);
	ml.addk("try", true);
	*/
	ml.addk("insn");
	/*ml.addk("BIT");
	ml.addk("B1");
	ml.addk("B2");
	ml.addk("B4");
	ml.addk("B8");
	ml.addk("P");
	ml.addk("Q");
	ml.addk("R");
	ml.addk("S");*/
	ml.addk("assert");
	ml.addk("sx");
	ml.addk("zx");
	ml.addk("o16");
	ml.addk("o32");
	ml.addk("o64");
	ml.addk("a16");
	ml.addk("a32");
	ml.addk("a64");
	ml.addk("push");
	ml.addk("pop");
	ml.addk("discard");
	ml.addk("tr"); // truncate
	ml.addk("sizeof");
	ml.addk("outport");
	ml.addk("inport");
	ml.addk("extern");
	ml.addk("reserve");
	ml.addk("restore");
	ml.addk("commit");
	std::ofstream fk2("out_keywords_map.h");
	fk2 << "// " << copyright << std::endl;
	for(std::map<std::string, bool>::iterator i = ml.allKeywords.begin(); i != ml.allKeywords.end(); )
	{
		std::string tmp = std::string("KEYWORD_") + i->first;
		for(std::string::iterator j = tmp.begin(); j != tmp.end(); ++j)
			*j = std::toupper(*j);
		if(i->second)
			tmp += "__CPP";
		if(i->second == false)
			fk2 << "keywords[\"" << i->first << "\"] = " << tmp << ";" << std::endl;
		++i;
	}
	
	std::ofstream fk("out_keywords.h");
	fk << "// " << copyright << std::endl;
	fk << "enum {" << std::endl;
	for(std::map<std::string, bool>::iterator i = ml.allKeywords.begin(); i != ml.allKeywords.end(); )
	{
		fk << "\tKEYWORD_";
		std::string tmp = i->first;
		for(std::string::iterator j = tmp.begin(); j != tmp.end(); ++j)
			*j = std::toupper(*j);
		if(i->second)
			tmp += "__CPP";
		fk << tmp;
		if(i == ml.allKeywords.begin())
			fk << " = 0x30000";
		++i;
		if(i != ml.allKeywords.end())
			fk << ",";
		fk << std::endl;
	}
	fk << "};" << std::endl;
	fk << std::endl;
	/*
	Thse alternatives are also reserved words in C++ but they do not appear in the grammar:
	and	and_eq	bitand	bitor	compl	not
	not_eq	or	or_eq	xor	xor_eq
	*/
	
	return finish();
}

struct GramLex
{
	std::vector<std::string> tokens;
	std::string getToken(unsigned index)
	{
		if(index >= tokens.size())
			return "";
		return tokens[index];
	}
	GramLex(std::string s)
	{
		std::string tmp = "";
		s += " ";
		for(std::string::iterator i = s.begin(); i != s.end(); ++i)
		{
			if(std::isspace(*i))
			{
				if(tmp != "")
					tokens.push_back(tmp);
				tmp = "";
			}
			else
			if(*i == '#')
			{
				if(tmp != "")
					tokens.push_back(tmp);
				break;
			}
			else
				tmp += *i;
		}
	}
};

struct GramCom
{
	std::map<std::string, std::list<std::string> > m;
	std::set<std::string> wanted;

	void accept(std::ostream &bldout, std::string &symbol, std::list<std::string> &repl_list)
	{
		m[symbol] = repl_list;
		std::string last = "";
		int optLevel = 0;
		int repLevel = 0;
		for(std::list<std::string>::iterator i = repl_list.begin(); i != repl_list.end(); ++i)
		{
			if((*i)[0] >= 'a' && (*i)[0] <= 'z')
			{
				wanted.insert(*i);
			}
			
			if(last == "{" || last == "[")
			{
				if((*i)[0] >= 'a' && (*i)[0] <= 'z')
					;
				else
				if((*i)[0] >= 'A' && (*i)[0] <= 'Z')
					;
				else
				if((*i)[0] == '\'' || (*i)[0] == '\"')
					;
				else
				{
					std::cerr << "Error in grammar: symbol \'" << symbol << "\': after { or [ must be a terminal or nonterminal!" << std::endl;
					throw -1;
				}
			}
			last = *i;
			
			if(*i == "[")
			{
				++optLevel;
				if(optLevel > 1 || repLevel != 0)
				{
					std::cerr << "Error in grammar: symbol \'" << symbol << "\': nesting too deep!" << std::endl;
					throw -1;
				}
			}
			else
			if(*i == "]")
				--optLevel;
			
			if(*i == "{")
			{
				++repLevel;
				if(repLevel > 1 || optLevel != 0)
				{
					std::cerr << "Error in grammar: symbol \'" << symbol << "\': nesting too deep!" << std::endl;
					throw -1;
				}
			}
			else
			if(*i == "}")
				--repLevel;
		}
		
		if(repLevel != 0 || optLevel != 0)
		{
			std::cerr << "Error in grammar: symbol \'" << symbol << "\': unterminated [] or {} !" << std::endl;
			throw -1;
		}
		
		std::list< std::list<std::string> > alternates;
		//alternates.push_back(std::list<std::string>());
		std::list<std::string> tmp;
		for(std::list<std::string>::iterator i = repl_list.begin(); i != repl_list.end(); ++i)
		{
			if(*i == "|")
			{
				//alternates.push_back(std::list<std::string>());
				// copy 'tmp' into alternates.
				copyIn(symbol, alternates, tmp);
				tmp.clear();
			}
			else
			{
				//alternates.back().push_back(*i);
				tmp.push_back(*i);
			}
		}
		copyIn(symbol, alternates, tmp);
		
		bldout << "beginRule(" << fixname(symbol) << ", \"" << fixname(symbol) << "\");" << std::endl;
		
		for(std::list< std::list<std::string> >::iterator i = alternates.begin(); i != alternates.end(); ++i)
		{
			bldout << "reset();" << std::endl;
			for(std::list<std::string>::iterator j = i->begin(); j != i->end(); ++j)
			{
				if(*j == "{")
					bldout << "beginRep();" << std::endl;
				else
				if(*j == "}")
					bldout << "endRep();" << std::endl;
				else
				{
					std::string &s = *j;
					if(s[0] >= 'a' && s[0] <= 'z')
					{
						bldout << "add(" << fixname(s) << ");" << std::endl;
					}
					else
					if(s[0] >= 'A' && s[0] <= 'Z')
					{
						bldout << "add(TOKEN_" << s << ");" << std::endl;
					}
					else
					if(s[0] == '\"' && !(s[1] == '_' || (s[1] >= 'a' && s[1] <= 'z') || (s[1] >= 'A' && s[1] <= 'Z')))
					{
						std::string tmp(++s.begin(), --s.end());
						bldout << "add(TOKEN_" << ml.m2t[tmp] << ");" << std::endl;
					}
					else
					if(s[0] == '\"')
					{
						std::string tmp(++s.begin(), --s.end());
						tmp = "KEYWORD_" + tmp;
						for(std::string::iterator i = tmp.begin(); i != tmp.end(); ++i)
							*i = std::toupper(*i);
						bldout << "add(" << tmp << ");" << std::endl;
					}
					else
					if(s[0] == '\'')
					{
						bldout << "add(" << s << ");" << std::endl;
					}
					else
					{
						bldout << "// " << s << std::endl;
						bldout << "add(" << s << ");" << std::endl;
					}
				}
			}
		}
		
		bldout << "endRule();" << std::endl;
	}
	
	void copyIn(std::string symbol, std::list< std::list<std::string> > &alternates, std::list<std::string> &src)
	{
		int numOpt = 0;
		int numRep = 0;
		
		for(std::list<std::string>::iterator i = src.begin(); i != src.end(); ++i)
		{
			if(*i == "[")
				++numOpt;
			if(*i == "{")
				++numRep;
		}
		if(numOpt > 3)
		{
			std::cerr << "Error in grammar: symbol \'" << symbol << "\': too many optional tokens (" << numOpt << "; max is 3)" << std::endl;
			throw -1;
		}
		if(numRep > 1)
		{
			std::cerr << "Error in grammar: symbol \'" << symbol << "\': too many repeated tokens (" << numRep << "; max is 1)" << std::endl;
			throw -1;
		}
		
		int numComb = 1 << numOpt;
		
		for(int ii = 0; ii < numComb; ++ii)
		{
			alternates.push_back(std::list<std::string>());
			int optCount = 0;
			bool skip = false;
			for(std::list<std::string>::iterator i = src.begin(); i != src.end(); ++i)
			{
				if(*i == "[")
				{
					if((ii & (1 << optCount)) == 0)
					{
						skip = false;
					}
					else
					{
						skip = true;
					}
					++optCount;
				}
				else
				if(*i == "]")
				{
					skip = false;
				}
				else
				if(!skip)
				{
					alternates.back().push_back(*i);
				}
			}
		}
	}
	
	void finish(std::ostream &hdrout, std::ostream &bldout)
	{
		for(std::set<std::string>::iterator i = wanted.begin(); i != wanted.end(); ++i)
		{
			if(m.find(*i) == m.end())
			{
				std::cerr << "Error in grammar: symbol \'" << *i << "\' is used but not defined anywhere!" << std::endl;
				throw -1;
			}
		}
		
		hdrout << "// " << copyright << std::endl;
		hdrout << "// This file was automatically generated." << std::endl;
		hdrout << std::endl;
		hdrout << "enum {" << std::endl;
		wanted.insert("start");	// generated code will be bad if there are any orphans
		for(std::set<std::string>::iterator i = wanted.begin(); i != wanted.end();)
		{
			hdrout << "\t" << fixname(*i);
			if(i == wanted.begin())
				hdrout << " = 0x20000";
			++i;
			if(i != wanted.end())
				hdrout << ",";
			hdrout << std::endl;
		}
		hdrout << "};" << std::endl;
		hdrout << std::endl;
		hdrout << std::endl;
				
		bldout << std::endl;
		bldout << std::endl;
	}
	
	std::string fixname(std::string s)
	{
		std::string t = "RULE_";
		for(std::string::iterator i = s.begin(); i != s.end(); ++i)
		{
			if(*i >= 'a' && *i <= 'z')
				t += std::toupper(*i);
			else
			if(*i == '-')
				t += "_D_";
			else
			if(*i >= '0' && *i <= '9')
				t += std::string("N_") + *i + "_";
			else
			if(*i == '_')
				t += "_U_";
			else
				t += "_O_";
		}
		return t;
	}
};

struct MetaCom
{
	std::string symbol;
	std::list<std::string> repl_list;
	int state;
	GramCom gc;
	std::ostream &hdrout;
	std::ostream &bldout;
	
	MetaCom(std::ostream &hdroutT, std::ostream &bldoutT) :
		hdrout(hdroutT),
		bldout(bldoutT)
	{
		state = 0;
		bldout << "// " << copyright << std::endl;
		bldout << "// This file was automatically generated." << std::endl;
		bldout << std::endl;
	}
	void acceptToken(int lineNum, std::string token)
	{
		if(state == 0)
		{
			symbol = token;
			state = 1;
		}
		else
		if(state == 1)
		{
			if(token == "::=")
			{
				state = 2;
				repl_list.clear();
			}
			else
			{
				std::cerr << "error: line " << lineNum << ": expected: \'::=\'" << std::endl;
				throw 1;
			}
		}
		else
		if(state == 2)
		{
			if(token == ";")
			{
				state = 0;
				gc.accept(bldout, symbol, repl_list);
				//std::cerr << "[" << symbol << "] defined." << std::endl;
			}
			else
				repl_list.push_back(token);
		}
	}
	void end(int lineNum)
	{
		if(state != 0)
		{
			std::cerr << "error: line " << lineNum << ": unexpected end of file!" << std::endl;
			throw 1;
		}
		gc.finish(hdrout, bldout);
	}
};

int finish()
{
	std::ifstream fi("cgram.txt");
	if(fi == NULL)
	{
		std::cerr << "error: can\'t open file: \'cgram.txt\'." << std::endl;
		return 1;
	}
	
	std::ofstream chdr("out_chdr.h");
	std::ofstream cbld("out_cbld.h");
	MetaCom mc(chdr, cbld);
	
	std::string line;
	int lineNum = 0;
	try
	{
		while(std::getline(fi, line) != NULL)
		{
			++lineNum;
			// now look at 'line'.
			GramLex gl(line);
			for(unsigned i = 0; gl.getToken(i) != ""; ++i)
				mc.acceptToken(lineNum, gl.getToken(i));
		}
		mc.end(lineNum);
	}
	catch(int x)
	{
		return x;
	}
	
	std::ofstream fd("out_debug_map.h");
	fd << "// " << copyright << std::endl;
	for(std::map<std::string, bool>::iterator i = ml.allKeywords.begin(); i != ml.allKeywords.end(); )
	{
		std::string tmp = std::string("KEYWORD_") + i->first;
		for(std::string::iterator j = tmp.begin(); j != tmp.end(); ++j)
			*j = std::toupper(*j);
		if(i->second)
			tmp += "__CPP";
		fd << "debugMap[" << tmp << "] = \"" << i->first << "\";" << std::endl;
		++i;
	}
	for(std::map<std::string, std::string>::iterator i = ml.cmap.begin(); i != ml.cmap.end(); ++i)
	{
		fd << "debugMap[TOKEN_" << i->second << "] = \"" << i->first << "\";" << std::endl;
	}
	// now do rules.
	for(std::set<std::string>::iterator i = mc.gc.wanted.begin(); i != mc.gc.wanted.end(); ++i)
	{
		//fd << *i << " " << mc.gc.fixname(*i) << std::endl;
		fd << "debugMap[" << mc.gc.fixname(*i) << "] = \"" << *i << "\";" << std::endl;
	}

	return 0;
}

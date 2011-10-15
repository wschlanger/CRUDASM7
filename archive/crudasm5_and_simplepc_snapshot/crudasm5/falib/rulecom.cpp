// rulecom.cpp - rule compiler main program
// Copyright (C) 2009 Willow Schlanger

#include "rulelex.h"
#include "ruleparse.h"

#ifndef l_out_rulecom_header__ceres_included
#define l_out_rulecom_header__ceres_included
#include "out_rulecom_header.h"
#endif	// l_out_rulecom_header__ceres_included

class RulecomLexer :
	public RulecomLexerGeneric
{
	public:
	RulecomLexer(std::istream &fiT) :
		RulecomLexerGeneric(fiT)
	{
	}
	
	protected:

#	include "out_rulecom_lexer.h"

	unsigned defaultHandler(unsigned token)
	{
		if(token > 0x80000000 && token <= 0x800000ff)
		{
			char c = token & 0xff;
			if(c == '\'' || c == '\"')
				return getLiteral();
			if(isIdentStartChar(c))
			{
				do
					++curTokenEnd;
				while(curTokenEnd != curLine.end() && isIdentMiddleChar(*curTokenEnd));
				std::string s(curTokenBegin, curTokenEnd);
				unsigned keyword = getManualKeyword(s);
				token = (keyword == 0) ? TOKEN_rcIDENT : keyword;
			}
		}
		return token;
	}
	
	virtual bool fixupToken()
	{
		return true;
	}
	
	unsigned getLiteral()
	{
		char c = *curTokenEnd;
		for(;;)
		{
			++curTokenEnd;
			if(curTokenEnd == curLine.end())
				return 0x80000000;
			if(*curTokenEnd == c)
				break;
			if(*curTokenEnd == '\\')
			{
				++curTokenEnd;	// skip \ char
				if(curTokenEnd == curLine.end())
					return 0x80000000;
			}
		}
		++curTokenEnd;
		return (c == '\'') ? TOKEN_rcLITCHAR : TOKEN_rcLITSTRING;
	}
	
	bool isIdentStartChar(char c) { return isIdentChar(c); }
	bool isIdentMiddleChar(char c) { return isIdentChar(c); }

	bool isIdentChar(char c)
	{
		if(c >= '0' && c <= '9')
			return true;
		if(c >= 'a' && c <= 'z')
			return true;
		if(c >= 'A' && c <= 'Z')
			return true;
		if(c == '_')
			return true;
		return false;
	}
};

class RulecomParser
{
	RulecomLexer &lexer;

	public:
	RulecomParser(RulecomLexer &lexerT) :
		lexer(lexerT)
	{
	}
	
	ParseNode *parse()
	{
		if(lexer.getToken() == 0)
			return NULL;	// empty source file
		ParseNode *node = parse_rcRULE_start();
		if(lexer.getToken() != 0)
		{
			delete node;
			throw ParseSyntaxError();
		}
		return node;
	}

	private:
	int getLine() { return lexer.getLine(); }
	int getFile() { return lexer.getFile(); }	
	unsigned getToken() { return lexer.getToken(); }
	void nextToken() { lexer.nextToken(); }

	void addTerminal(ParseNode *node)
	{
		node->addTerminal(lexer.getToken(), lexer.getLine(), lexer.getFile(), lexer.getTokenText());
	}
	
	void addNonterminal(ParseNode *node, ParseNode *child)
	{
		node->addNonterminal(child);
	}
	
#	include "out_rulecom_parser.h"
};

#include <fstream>

int main(int argc, char **argv)
{
std::cout << (sizeof(void *) * 8) << " bit mode enabled." << std::endl;
////argc = 2;
////argv[1] = "rulecom";
	if(argc != 2)
	{
		std::cout << "usage: rulecom OutFileBaseName" << std::endl;
		return 1;
	}
	std::string outFileBaseName(argv[1]);

	std::string inFileName = "<stdin>";
////std::ifstream fi("in.txt");
////if(!fi)  std::cout << "Unable to open file!" << std::endl;
	RulecomLexer lexer(std::cin);	// was fi
	lexer.init();
	ParseNode *root = NULL;
	try
	{
		RulecomParser parser(lexer);
		
		try
		{
			root = parser.parse();
			if(root == NULL)
			{
				std::cout << "rulecom: nothing to do! (input file is empty)" << std::endl;
				return 1;
			}
			
			try
			{
				// doParse() implementation is in ruleparse.cpp.
				doParse(root, inFileName, outFileBaseName);
			}
			catch(ParseSpecificError &e)
			{
				e.printError(lexer.fileNameFromNum(e.getFile()));
				delete root;
				return 1;
			}
			
			delete root;
		}
		catch(ParseSyntaxError)
		{
			std::cout << "error: " << lexer.fileNameFromNum(lexer.getFile()) << ": " << lexer.getLine();
			std::cout << ": syntax error" << std::endl;
			if(root != NULL)
				delete root;
			return 1;
		}
		catch(ParseError)
		{
			std::cout << "error: giving up" << std::endl;
			if(root != NULL)
				delete root;
			return 1;
		}
	}
	catch(RulecomLexerError)
	{
		std::cout << "error: giving up" << std::endl;
		if(root != NULL)
			delete root;
		return 1;
	}
	return 0;
}

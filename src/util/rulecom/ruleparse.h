// ruleparse.h
// Copyright (C) 2009 Willow Schlanger

#ifndef ruleparse_h__ceres_included
#define ruleparse_h__ceres_included

#include <list>
#include <string>
#include <iostream>
#include "../../common/include/types.h"
#include <stddef.h>
typedef size_t UINT;

class ParseNode
{
	std::list<ParseNode *> children;
	unsigned token;
	int line;
	int file;
	std::string text;			// "" for nonterminals
	
	public:
	ParseNode(unsigned tokenT, int lineT, int fileT) :
		token(tokenT),
		line(lineT),
		file(fileT)
	{
	}
	
	typedef std::list<ParseNode *>::iterator iterator_type;
	iterator_type begin() { return children.begin(); }
	iterator_type end() { return children.end(); }
	UINT size() { return children.size(); }
	int getLine() { return line; }
	int getFile() { return file; }
	unsigned getToken() { return token; }
	std::string getTokenText() { return text; }
	
	~ParseNode()
	{
		for(std::list<ParseNode *>::iterator i = children.begin(); i != children.end(); ++i)
			delete *i;
	}
	
	void addTerminal(unsigned tokenT, int lineT, int fileT, std::string textT)
	{
		children.push_back(new ParseNode(tokenT, lineT, fileT));
		children.back()->text = textT;
	}
	
	void addNonterminal(ParseNode *child)
	{
		children.push_back(child);
	}
};

void doParse(ParseNode *root, std::string inFileName, std::string outBaseName);

class ParseError { };

class ParseSyntaxError : public ParseError { };

class ParseSpecificError :
	public ParseError
{
	int lineNum;
	int fileNum;
	std::string message;
	
	public:
	
	ParseSpecificError(int lineNumT, int fileNumT, std::string msgT) :
		lineNum(lineNumT),
		fileNum(fileNumT),
		message(msgT)
	{
	}
	
	int getFile() { return fileNum; }

	void printError(std::string fileName)
	{
		std::cout << "error: " << fileName << ": " << lineNum << ": " << message << std::endl;
	}

	void printError(std::string fileName, std::ostream &os)
	{
		os << "error: " << fileName << ": " << lineNum << ": " << message << std::endl;
	}
};

#endif	// ruleparse_h__ceres_included

// parse.h
// Copyright (C) 2009 Willow Schlanger

#ifndef l_parse_h__ncc_included
#define l_parse_h__ncc_included

#include <stdexcept>
#include <fstream>
#include <iostream>
#include <list>
#include <cstddef>

#include "out_chdr.h"
#include "lexc.h"
#include "scope.h"

struct ParseNode
{
	std::list<ParseNode *> children;
	U4 token;
	std::string text;	// may be ""
	U4 lineNum;
	U4 fileNum;
	void *userPtr;		// a Declaration for declaration's, an Expression for expression's.

	ParseNode(U4 tokenT, U4 lineNumT, U4 fileNumT)
	{
		token = tokenT;
		lineNum = lineNumT;
		fileNum = fileNumT;
		userPtr = NULL;
	}
	
	void addNode(ParseNode *n)
	{
		children.push_back(n);
	}
	
	~ParseNode()
	{
		for(std::list<ParseNode *>::iterator i = children.begin(); i != children.end(); ++i)
			delete *i;
	}
	
	bool isTerminal()
	{
		U4 x = token;
		if(x >= 0x20000 && x <= 0x2ffff)
			return false;	// these are nonterminals
		return true;	// everything else we treat like a terminal
	}
	
	class ConstantValue *getConstantValue();
};

struct ParseError : public
	std::runtime_error
{
	int lineNum;
	int fileNum;
	int srcLine;
	ParseError(std::string msg) :
		std::runtime_error(msg.c_str()),
		lineNum(0),
		fileNum(0),
		srcLine(0)
	{
	}
	ParseError(std::string msg, int lineNumT, int fileNumT, int srcLineT = 0) :
		std::runtime_error(msg.c_str()),
		lineNum(lineNumT),
		fileNum(fileNumT),
		srcLine(srcLineT)
	{
	}
};

struct DebugMap :
	public CTokenEnums
{
	public:
	std::map<U4, std::string> debugMap;
	DebugMap()
	{
		build();
	}
	void out(U4 x)
	{
		if(x <= 0xff)
			std::cout << ((char)(x));
		else
		if(debugMap.find(x) != debugMap.end())
			std::cout << debugMap[x];
		else
			std::cout << std::hex << x << std::dec;
	}
	std::string get(U4 x)
	{
		if(debugMap.find(x) != debugMap.end())
			return debugMap[x];
		return "(unknown)";
	}
	private:
	void build();
};

extern DebugMap dMap;

#endif	// l_parse_h__ncc_included

// rulelex.h
// Copyright (C) 2009 Willow Schlanger

#ifndef l_rulelex_h__ceres_included
#define l_rulelex_h__ceres_included

#include <list>
#include <iostream>
#include <set>
#include <map>
#include <cctype>
#include <string>

class RulecomLexerError { };

class RulecomLexerBase
{
	protected:
	std::istream &fi;
	std::map<std::string, int> fileNames;
	std::map<int, const std::string *> fileNums;
	int fileNum;
	int lineNum;
	std::string curLine;
	std::string::iterator curTokenBegin, curTokenEnd;
	unsigned curToken;
	
	virtual bool doNextToken() = 0;
	virtual bool doControlLine() = 0;
	public:
	RulecomLexerBase(std::istream &fiT) :
		fi(fiT)
	{
		addFileName("<stdin>");		// this will be file number 0
		lineNum = 0;
		fileNum = 0;
		curLine.clear();
		curTokenBegin = curLine.end();
		curTokenEnd = curLine.end();
		curToken = ~0;				// means call nextToken()
	}
	
	virtual void processLine(std::string &s) { }
	
	void nextToken()
	{
		if(curToken == 0)
			return;					// already at end of file
		for(;;)
		{
			while(curTokenEnd != curLine.end() && std::isspace(*curTokenEnd))
				++curTokenEnd;
			if(curTokenEnd == curLine.end())
			{
				// We reached the end of the line.
				for(;;)
				{
					++lineNum;
					if(std::getline(fi, curLine) == NULL)
					{
						curLine.clear();
						curTokenBegin = curLine.end();
						curTokenEnd = curLine.end();
						curToken = 0;	// end of file
						return;
					}
					processLine(curLine);
					curTokenEnd = curLine.begin();
					while(curTokenEnd != curLine.end() && std::isspace(*curTokenEnd))
						++curTokenEnd;
					if(curTokenEnd != curLine.end() && *curTokenEnd == '#')
					{
						bool ok = doControlLine();
						if(!ok)
						{
							std::cout << "error: " << fileNameFromNum(getFile()) << ": " << getLine();
							std::cout << ": invalid or unsupported control line format" << std::endl;
							throw RulecomLexerError();
						}
						continue;
					}
					break;
				}
				continue;
			}
			curTokenBegin = curTokenEnd;
			bool ok = doNextToken();
			if(!ok)
			{
				std::cout << "error: " << fileNameFromNum(getFile()) << ": " << getLine() << ": lexer error" << std::endl;
				throw RulecomLexerError();
			}
///std::cout << "[" << getTokenText() << "]";
			break;
		}
	}
	
	int getLine()
	{
		return lineNum;
	}
	
	int getFile()
	{
		return fileNum;
	}
	
	std::string fileNameFromNum(int num)
	{
		if(fileNums.find(num) == fileNums.end())
			return "";	// not found
		return *fileNums[num];
	}
	
	unsigned getToken()
	{
		if(curToken == ~0)
			nextToken();
		return curToken;
	}
	
	std::string getTokenText()
	{
		if(curToken == ~0)
			nextToken();
		if(curToken == 0)
			return "<eof>";
		return std::string(curTokenBegin, curTokenEnd);
	}
	
	protected:
	int addFileName(std::string s)
	{
		if(fileNames.find(s) != fileNames.end())
			return fileNames[s];
		int count = static_cast<int>(fileNames.size());
		fileNames[s] = count;
		fileNums[count] = &fileNames.find(s)->first;
		return count;
	}
};

class RulecomLexerControl :
	public RulecomLexerBase
{
	public:
	RulecomLexerControl(std::istream &fiT) :
		RulecomLexerBase(fiT)
	{
	}
	
	protected:
	virtual bool doControlLine()
	{
		std::string newFileName;
		int newLineNum;
		
		++curTokenEnd;		// skip '#'
		while(curTokenEnd != curLine.end() && std::isspace(*curTokenEnd))
			++curTokenEnd;	// skip spaces
		if(curTokenEnd == curLine.end())
			return true;	// empty # on a line
		
		newLineNum = 0;
		while(std::isdigit(*curTokenEnd))
		{
			newLineNum = (newLineNum * 10) + (*curTokenEnd - '0');
			++curTokenEnd;
			if(curTokenEnd == curLine.end())
				return false;
		}
		
		while(curTokenEnd != curLine.end() && std::isspace(*curTokenEnd))
			++curTokenEnd;	// skip spaces
		if(curTokenEnd == curLine.end() || *curTokenEnd != '\"')
			return false;
		++curTokenEnd;
		newFileName.clear();
		while(curTokenEnd != curLine.end() && *curTokenEnd != '\"')
		{
			newFileName += *curTokenEnd;
			++curTokenEnd;
		}
		if(curTokenEnd == curLine.end())
			return false;
		++curTokenEnd;	// skip end double quote
		while(curTokenEnd != curLine.end() && std::isspace(*curTokenEnd))
			++curTokenEnd;	// skip spaces
		if(curTokenEnd != curLine.end())
			return false;	// expecting end of line
		
		lineNum = newLineNum - 1;
		fileNum = addFileName(newFileName);
		return true;
	}
};

class RulecomLexerGeneric :
	public RulecomLexerControl
{
	public:
	RulecomLexerGeneric(std::istream &fiT) :
		RulecomLexerControl(fiT)
	{
	}
	
	protected:

	// pre: curTokenEnd != curLine.end() and we've already skipped past whitespace
	virtual bool doNextToken()
	{
		curTokenEnd = curTokenBegin;
		unsigned token = lexNextToken();
		if(token == 0x80000000)
		{
			std::cout << "error: " << fileNameFromNum(getFile()) << ": " << getLine() << ": ";
			std::cout << "lexer error: unexpected end of line";
			std::cout << std::endl;
			return false;
		}
		else
		if(token > 0x80000000)
		{
			std::cout << "error: " << fileNameFromNum(getFile()) << ": " << getLine() << ": ";
			std::cout << "lexer error: unexpected character ";
			token &= 0xffff;
			if(token >= 0x21 && token <= 0x7e)
				std::cout << "\'" << static_cast<char>(token) << "\'";
			else
				std::cout << std::hex << "0x" << token << std::dec;
			std::cout << std::endl;
			return false;
		}
		curToken = token;
		return fixupToken();
	}

	virtual bool fixupToken() = 0;
	virtual unsigned lexNextToken() = 0;

	unsigned getChar()
	{
		if(curTokenEnd == curLine.end())
			return 0;
		return static_cast<unsigned char>(*curTokenEnd);
	}

	void nextChar()
	{
		if(curTokenEnd != curLine.end())
			++curTokenEnd;
	}
	
	virtual void initManualKeywords() = 0;
	
	private:
	std::map<std::string, unsigned> manualKeywords;
	protected:
	
	void addManualKeyword(std::string s, unsigned value)
	{
		manualKeywords[s] = value;
	}
	
	unsigned getManualKeyword(std::string s)
	{
		std::map<std::string, unsigned>::iterator i = manualKeywords.find(s);
		if(i == manualKeywords.end())
			return 0;
		return i->second;
	}
	
	public:
	void init()
	{
		initManualKeywords();
	}
};

// Below is a sample of how to use rulelex.
/*
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
*/

#endif		// l_rulelex_h__ceres_included

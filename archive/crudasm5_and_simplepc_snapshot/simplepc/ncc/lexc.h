// lexc.h
// Copyright (C) 2009 Willow Schlanger

#ifndef l_lexc_h__ncc_included
#define l_lexc_h__ncc_included

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <cstddef>
#include <string>
#include <cctype>
#include <map>
#include <set>
#include <vector>
#include <list>

#include "token_enums.h"
#include "out_keywords.h"

class LexerError :
	public std::runtime_error
{
	public:
	int lineNum;
	std::string fileName;
	LexerError(int lineNumT, std::string s, std::string fileNameT) :
		lineNum(lineNumT),
		fileName(fileNameT),
		std::runtime_error(s)
	{
	}
	virtual ~LexerError() throw()
	{
	}
};

class TokenStream :
	public CTokenEnums
{
	std::istream &fi;
	int lineNum;
	std::string curLine;
	U4 token;
	U4 nextOffset;
	U4 begin;
	U4 end;
	std::map<std::string, U4> keywords;
	U4 fileNum;
	std::map<std::string, U4> fileNames;
	std::vector<std::map<std::string, U4>::iterator> fileNameVec;
	
	enum
	{
		ATEOF = TOKEN_EOF
	};

	public:
	int getLineNum()
	{
		return lineNum;
	}
	
	U4 getFileNum()
	{
		return fileNum;
	}
	
	std::string getFileName(U4 num)
	{
		return fileNameVec[num]->first;
	}
	
	void setFileName(std::string s)
	{
		std::map<std::string, U4>::iterator i = fileNames.find(s);
		if(i == fileNames.end())
		{
			fileNum = fileNameVec.size();
			fileNames[s] = fileNum;
			fileNameVec.push_back(fileNames.find(s));
		}
		else
			fileNum = i->second;
	}
	
	TokenStream(std::istream &fiT, std::string fnT) :
		fi(fiT)
	{
		fileNum = 0;
		setFileName(fnT);
		lineNum = 0;
		token = TOKEN_EOF;
		begin = 0;
		end = 0;
		initKeywords();
		
		scope.push_back(std::map<std::string, char>());

		getNextToken();
		
		/*while(token != TOKEN_EOF)
		{
			std::cerr << "+" <<std::hex << token << std::dec << std::endl;
			getNextToken();
		}*/
	}
	
	U4 getCurrentToken()
	{
		return token;
	}
	
	void advanceToken()
	{
		getNextToken();
	}
	
	U4 getBegin()
	{
		return begin;
	}
	
	U4 getEnd()
	{
		return end;
	}
	
	std::string::iterator getLineBegin()
	{
		return curLine.begin();
	}
	
	U4 charAt(U4 offset);
	
	void addToScope(std::string &s, char t);

	private:
	std::list< std::map<std::string, char> > scope;
	
	bool isTypeRefId(std::string &s)
	{
		//std::cout << "(searching for: " << s << " - " << scope.size() << ")";
		std::list< std::map<std::string, char> >::iterator i = scope.end();
		if(i == scope.begin())
			return false;	// will never happen
		--i;
		for(;;)
		{
			//std::cout << "<" << i->size() << ">";
			//if(!i->empty())
			//	std::cout << "[" << i->begin()->second << "]";
		
			if(i->find(s) != i->end())
				return (*i)[s] == 't';
			if(i == scope.begin())
				break;
			--i;
		}
		return false;
	}
	
	void initKeywords();
	void readNextLine();
	void getNextToken();
	U4 nextToken();
	void advanceTo(U4 offset);
	void unexpectedEndOfFile(U4 offset);
	void unexpectedChar(U4 offset);
	void setFlags(U4 flags);
	bool isIdentChar(char c);
	void handleControlLine();
};

#endif	// l_lexc_h__ncc_included

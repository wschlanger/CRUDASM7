// lexc.cpp
// Copyright (C) 2009 Willow Schlanger

#include "lexc.h"

void TokenStream::readNextLine()
{
	do
	{
		++lineNum;
		if(std::getline(fi, curLine) == NULL)
		{
			curLine.clear();
			break;
		}
	}	while(curLine.empty());
	nextOffset = 0;
	//std::cerr << lineNum << ": " << curLine << std::endl;
	begin = end = 0;
}

void TokenStream::handleControlLine()
{
	std::string s = curLine + " ";
	std::string tmp;
	std::vector<std::string> tokens;
	for(std::string::iterator i = s.begin(); i != s.end(); ++i)
	{
		if(std::isspace(*i))
		{
			if(!tmp.empty())
			{
				//std::cout << "[" << tmp << "]";
				tokens.push_back(tmp);
			}
			tmp.clear();
		}
		else
			tmp += *i;
	}
	//std::cout << std::endl;
	if(tokens[0] != "#")
		throw LexerError(getLineNum(), "[4] unsupported control line", getFileName(getFileNum()));
	if(tokens.size() == 1)
		return;	// # by itself is allowed
	if(tokens.size() < 3)
		throw LexerError(getLineNum(), "[3] unsupported control line", getFileName(getFileNum()));
	std::string lineNumStr = tokens[1];
	int num = 0;
	for(std::string::iterator i = lineNumStr.begin(); i != lineNumStr.end(); ++i)
	{
		if(*i < '0' || *i > '9')
			throw LexerError(getLineNum(), "[2] unsupported control line", getFileName(getFileNum()));
		num = (num * 10) + *i - '0';
	}
	std::string fileNameStr = tokens[2];
	if(fileNameStr.size() < 3)
		throw LexerError(getLineNum(), "[1] unsupported control line", getFileName(getFileNum()));
	fileNameStr = std::string(++fileNameStr.begin(), --fileNameStr.end());
	setFileName(fileNameStr);
	lineNum = num - 1;
}

void TokenStream::addToScope(std::string &s, char t)
{
	if(scope.back().find(s) != scope.back().end())
	{
		throw LexerError(lineNum, std::string("type \'") + s + "\' is already in scope - redefinition not allowed", getFileName(getFileNum()));
	}
	(scope.back())[s] = t;
	//std::cout << "((" << s << "))";
	//std::cout << scope.back().size() << " ";
}

void TokenStream::getNextToken()
{
	bool at_start = false;
	for(;;)
	{
		begin = end;
		token = nextToken();
		end = nextOffset;
		U4 c = charAt(begin);
		
		if(at_start && token == '#')
		{
			handleControlLine();
			
			// skip to next line.
			at_start = false;
			end = curLine.size();
			nextOffset = end;
			continue;
		}
		if(token != TOKEN_EOF)
			at_start = false;
		
		bool fpNum = false;
		if(token == '.' && charAt(begin + 1) >= '0' && charAt(begin + 1) <= '9')
		{
			// '.' by itself is recognized as a token.
			end = begin;
			fpNum = true;
		}
		
		if(token == '{')
		{
			scope.push_back(std::map<std::string, char>());
		}
		else
		if(token == '}')
		{
			if(scope.size() == 1)
				throw LexerError(lineNum, "too many '}' while tracking scope", getFileName(getFileNum()));
			scope.pop_back();
		}
		
		if(token == TOKEN_UNKNOWN || fpNum)
		{
			if(c <= 0xff && std::isspace(c))
			{
				++end;
				begin = end;
				nextOffset = end;
				continue;
			}
			// handle tokens not recognized by the lexer here.
			
			U4 i = begin;
			
			//std::cerr << "+" << (char)(c);
			//if(isIdentChar(c)) std::cerr << "*";
			
			//---
			// note--numbers can start with a decimal point.
			if(fpNum || (c >= '0' && c <= '9'))
			{
				do
				{
					++i;
					c = charAt(i);
				}	while(c == '.' || isIdentChar(c));
				token = TOKEN_NUM;
			}
			else
			if(isIdentChar(c))
			{
				do
				{
					++i;
					c = charAt(i);
				}	while(isIdentChar(c));
				token = TOKEN_IDENT;
				std::string s(curLine.begin() + begin, curLine.begin() + (end + i - begin));
				std::map<std::string, U4>::iterator i = keywords.find(s);
				if(i != keywords.end())
				{
					token = i->second;
					//std::cout << "IsKeyword:";
				}
				else
				{
					// *** handle TYPEREFID's here.
					if(isTypeRefId(s))
					{
						token = TOKEN_TYPEREFID;
						//std::cout << "+" << s;
					}
					else
					{
						//std::cout << "-" << s;
					}
				}
				//std::cout << "(" << s << ")";
			}
			else
			if(c == '\'' || c == '\"')
			{
				U4 d = c;
				do
				{
					++i;
					c = charAt(i);
					if(c == '\\')
					{
						++i;
					}
					if(c == TOKEN_EOF)
					{
						throw LexerError(lineNum, "unexpected end of line", getFileName(getFileNum()));
					}
				}	while(c != d);
				++i;
				token = (d == '\'') ? TOKEN_CLITERAL : TOKEN_SLITERAL;
			}
			else
			{
				//std::cerr << "[" << std::hex << (c) << std::dec << " " << c << "]" << std::endl;
				std::string s = "unsupported character";
				if(c >= 32 && c <= 127)
					s += std::string(": \'") + (char)(c) + "\'";
				throw LexerError(lineNum, s, getFileName(getFileNum()));
			}
	
			end += i - begin;
			nextOffset = end;
			//---
		}
		if(token != TOKEN_EOF)
			break;
		// if here, we're at LEAST at end of line
		readNextLine();
		at_start = true;
		if(curLine.empty())
			return;				// at end of file, for real
	}
}

U4 TokenStream::nextToken()
{
	U4 curOffset = nextOffset;
#		include "out_lexc.h"
	return TOKEN_UNKNOWN;
}

U4 TokenStream::charAt(U4 offset)
{
	if(offset >= curLine.size())
		return TOKEN_EOF;
	return curLine[offset];
}

void TokenStream::advanceTo(U4 offset)
{
	nextOffset = offset;
}

void TokenStream::unexpectedEndOfFile(U4 offset)
{
	throw LexerError(lineNum, "unexpected end of line", getFileName(getFileNum()));
}

void TokenStream::unexpectedChar(U4 offset)
{
	std::string s = "unrecognized token \'";
	for(U4 x = nextOffset; x != offset; ++x)
		s += (char)(charAt(x));
	throw LexerError(lineNum, s + "\'", getFileName(getFileNum()));
}

void TokenStream::setFlags(U4 flags)
{
	if(flags != 0)
		throw LexerError(lineNum, "C++ token encountered", getFileName(getFileNum()));
}

bool TokenStream::isIdentChar(char c)
{
	if(c == '_' || c == '\\')
		return true;
	if(c >= '0' && c <= '9')
		return true;
	if(c >= 'a' && c <= 'z')
		return true;
	if(c >= 'A' && c <= 'Z')
		return true;
	return false;
}

void TokenStream::initKeywords()
{
	keywords.clear();
#	include "out_keywords_map.h"
}

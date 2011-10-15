// mkintelxml.cpp
// Copyright (C) 2009-2011 Willow Schlanger

#include "parser.h"

class IxLexer :
	public RulecomLexerGeneric
{
	public:
	IxLexer(std::istream &fiT) :
		RulecomLexerGeneric(fiT)
	{
	}
	
	protected:

#	include "out_intel_lexer.h"

	unsigned defaultHandler(unsigned token)
	{
		if(token > 0x80000000 && token <= 0x800000ff)
		{
///std::cout << (char)(token & 0xff);
			char c = token & 0xff;
			if(c == '\"')
				return getLiteral();
			if(isIdentStartChar(c))
			{
				do
					++curTokenEnd;
				while(curTokenEnd != curLine.end() && isIdentMiddleChar(*curTokenEnd));
				std::string s(curTokenBegin, curTokenEnd);
				unsigned keyword = getManualKeyword(s);
				token = (keyword == 0) ? TOKEN_ixIDENT : keyword;
				//if(token == KEYWORD_ixdis)
				//	handle_dis();
			}
			else
			if(isNumChar(c))
			{
				do
					++curTokenEnd;
				while(curTokenEnd != curLine.end() && isIdentMiddleChar(*curTokenEnd));
				token = TOKEN_ixNUM;	// could be for example: 0x1234abcd
			}
			/**else
			if(c == '{')
			{
				handle_dis();
				token = '}';	// this is an ugly hack to get it to work.
			}**/
		}
		return token;
	}
	
	private:
	std::string disText;
	void handle_dis()
	{
		///std::string disText;
		disText.clear();
		
		// We just encountered "dis".
		int level = 0;
		for(;;)
		{
			if(curTokenEnd != curLine.end() && std::isspace(*curTokenEnd))
			{
				// note: even if we are in quotation marks, spaces become one space.
				// note also that if we do this inside a dis block:
				//    write("}");
				// that '}' will be treated like a close curly brace, regardless of
				// the fact that it's in quotes. this applies to single-quotes too.
				disText += " ";
				while(curTokenEnd != curLine.end() && std::isspace(*curTokenEnd))
					++curTokenEnd;
			}
			if(curTokenEnd == curLine.end())
			{
				disText += "\n";
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
						///return;
						std::cerr << "error: " << fileNameFromNum(getFile()) << ": " << getLine();
						std::cerr << ": unexpected end of file" << std::endl;
						throw RulecomLexerError();
					}
					curTokenEnd = curLine.begin();
					std::string spaces;
					while(curTokenEnd != curLine.end() && std::isspace(*curTokenEnd))
					{
						spaces += *curTokenEnd;
						++curTokenEnd;
					}
					if(curTokenEnd != curLine.end() && *curTokenEnd == '#')
					{
						bool ok = doControlLine();
						if(!ok)
						{
							std::cerr << "error: " << fileNameFromNum(getFile()) << ": " << getLine();
							std::cerr << ": invalid or unsupported control line format" << std::endl;
							throw RulecomLexerError();
						}
						continue;
					}
					///for(int i = 0; i < numTabs; ++i)
					///	disText += "\t";
					disText += spaces;
					break;
				}
				continue;
			}
			curTokenBegin = curTokenEnd;
			
			disText += *curTokenEnd;
			
			if(*curTokenEnd == '{')
				++level;
			else
			if(*curTokenEnd == '}')
			{
				--level;
				if(level == 0)
					break;
			}
			
			++curTokenEnd;	// next character
		}

		// text matched will be "}" but curToken will be KEYWORD_ixdis.
		// meaning is: call getDisText() instead of getTokenText().
		++curTokenEnd;
		
		///std::cout << disText << std::endl;
		
		std::string::iterator iter = disText.begin();
		while(iter != disText.end())
		{
			if(std::isspace(*iter))
				++iter;
			else
				break;
		}
		disText = std::string(iter, disText.end());
	}
	public:
	std::string getDisText()
	{
		return disText;
	}
	protected:
	
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
		return TOKEN_ixLITSTRING;
	}
	
	bool isIdentStartChar(char c) { return isIdentChar(c); }
	bool isIdentMiddleChar(char c) { return isIdentChar(c) || isNumChar(c); }

	bool isIdentChar(char c)
	{
		if(c >= 'a' && c <= 'z')
			return true;
		if(c >= 'A' && c <= 'Z')
			return true;
		if(c == '_')
			return true;
		return false;
	}

	bool isNumChar(char c)
	{
		if(c >= '0' && c <= '9')
			return true;
		return false;
	}
};

class IxParser
{
	IxLexer &lexer;

	public:
	IxParser(IxLexer &lexerT) :
		lexer(lexerT)
	{
	}
	
	ParseNode *parse()
	{
		if(lexer.getToken() == 0)
			return NULL;	// empty source file
		ParseNode *node = parse_ixRULE_start();
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
		std::string text = (lexer.getToken() == '}') ? lexer.getDisText() : lexer.getTokenText();
		node->addTerminal(lexer.getToken(), lexer.getLine(), lexer.getFile(), text);
		///std::cout << "[" << text << "]" << std::endl;
	}
	
	void addNonterminal(ParseNode *node, ParseNode *child)
	{
		node->addNonterminal(child);
	}
	
#	include "out_intel_parser.h"
};

///#include <fstream>
int main()
{
	std::cerr << (8 * sizeof(void *)) << "-bit mode enabled." << std::endl;
	std::string inFileName = "<stdin>";
///std::ifstream x("a.txt");
///IxLexer lexer(x/*std::cin*/);
	std::istream &myin = std::cin;
IxLexer lexer(myin);
	lexer.init();
	ParseNode *root = NULL;
	try
	{
#if 0
		while(lexer.getToken() != 0)
		{
			std::cout << "[" << lexer.getTokenText() << "]";
			lexer.nextToken();
		}
#endif
		IxParser parser(lexer);
		
		try
		{
			root = parser.parse();
			if(root == NULL)
			{
				std::cerr << "error: nothing to do! (input file is empty)" << std::endl;
				return 1;
			}

			try
			{
				// syntaxParse() implementation is in parser.cpp.
				syntaxParse(root);
			}
			catch(ParseSpecificError &e)
			{
				e.printError(lexer.fileNameFromNum(e.getFile()), std::cerr);
				delete root;
				return 1;
			}
			
			delete root;
		}
		catch(ParseSyntaxError)
		{
			std::cerr << "error: " << lexer.fileNameFromNum(lexer.getFile()) << ": " << lexer.getLine();
			std::cerr << ": syntax error" << std::endl;
			if(root != NULL)
				delete root;
			// prevent preprocessor from generating an error message.
			if(!myin.eof())
			{
				std::string s;
				while(std::getline(myin, s) != NULL)  ;
			}
			return 1;
		}
		catch(ParseError)
		{
			std::cerr << "error: giving up" << std::endl;
			if(root != NULL)
				delete root;
			// prevent preprocessor from generating an error message.
			if(!myin.eof())
			{
				std::string s;
				while(std::getline(myin, s) != NULL)  ;
			}
			return 1;
		}
	}
	catch(RulecomLexerError)
	{
		std::cerr << "error: giving up" << std::endl;
		if(root != NULL)
			delete root;
		// prevent preprocessor from generating an error message.
		if(!myin.eof())
		{
			std::string s;
			while(std::getline(myin, s) != NULL)  ;
		}
		return 1;
	}
	return 0;
}

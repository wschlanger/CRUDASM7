// test_parser.cpp
// Copyright (C) 2009 Willlow Schlanger

#include "parse.h"
#include "expr.h"
#include "pass1.h"
#include "make_out_externals.h"
#include "make_out_emulator.h"
#include "make_out_metadata.h"

ConstantValue *ParseNode::getConstantValue()
{
	//if(token != RULE_EXPRESSION || userPtr == NULL)
	//	return NULL;
	Expression *ex = (Expression *)(userPtr);
	if(ex->cv.isConstant())
		return &ex->cv;
	return NULL;
}

class CParser :
	public CTokenEnums
{
	TokenStream &ts;
	
	public:
	
	CParser(TokenStream &tsT) :
		ts(tsT)
	{
	}
	
	// Note--need to allow for empty files (!)
	ParseNode *parse()
	{
		if(getCurrentToken() == TOKEN_EOF)
			return NULL;
		ParseNode *n = parse_RULE_START();
		if(getCurrentToken() != TOKEN_EOF)
		{
			delete n;
			throw ParseError("syntax error");
		}
		return n;
	}

	private:

#	include "out_parser.h"

	// We should return EOF only when the real EOF is encountered.
	// Also we should never return SPACE. Finally note that
	// adjacent string literals must be COMBINED somehow.
	unsigned getCurrentToken()
	{
		return ts.getCurrentToken();
	}
	
	void advanceToken()
	{
		ts.advanceToken();
	}
	
	// This adds the current token to 'node'.
	// If the token is an IDENT/TYPEREFID, CLITERAL, SLITERAL, or NUM,
	// then we have to add extra information from the
	// token text, itself.
	void addTerminal(ParseNode *node)
	{
		/*std::cout << "[";
		U4 b = ts.getBegin();
		U4 e = ts.getEnd();
		for(; b != e; ++b)
			std::cout << (char)(ts.charAt(b));
		std::cout << "]";*/
		U4 token = getCurrentToken();
		bool needString = false;
		switch(token)
		{
			case TOKEN_TYPEREFID:
			case TOKEN_IDENT:
			case TOKEN_CLITERAL:
			case TOKEN_SLITERAL:
			case TOKEN_NUM:
				needString = true;
				break;
		}
		node->children.push_back(new ParseNode(token, getLineNum(), getFileNum()));
		if(needString)
		{
			std::string::iterator sb = ts.getLineBegin() + ts.getBegin();
			std::string::iterator se = ts.getLineBegin() + ts.getEnd();
			node->children.back()->text = std::string(sb, se);
		}
	}
	
	void addNonterminal(ParseNode *node, ParseNode *src)
	{
		node->addNode(src);
	}
	
	U4 getLineNum()
	{
		return ts.getLineNum();
	}
	
	U4 getFileNum()
	{
		return ts.getFileNum();
	}
	
	private:
	void collectScope(ParseNode *src)
	{
		if(src->token == TOKEN_IDENT || src->token == TOKEN_TYPEREFID)
		{
			if(!src->text.empty())
				ts.addToScope(src->text, 't');	// t for typedef
		}
		for(std::list<ParseNode *>::iterator i = src->children.begin();
			i != src->children.end(); ++i
		)
			collectScope(*i);
	}
	
	ParseNode *findNode(ParseNode *src, U4 token)
	{
		if(src->token == token)
			return src;
		for(std::list<ParseNode *>::iterator i = src->children.begin();
			i != src->children.end(); ++i
		)
		{
			ParseNode *tmp = findNode(*i, token);
			if(tmp != NULL)
				return tmp;
		}
		return NULL;
	}
};

DebugMap dMap;

void DebugMap::build()
{
#	include "out_debug_map.h"
}

void print_node(ParseNode *n, int level)
{
	for(int i = 0; i < level; ++i)
		std::cout << " ";

	std::cout << n->lineNum << ":";

	bool any = false;
	while(!n->isTerminal() && n->children.size() == 1)
	{
		any = true;
		std::cout << "[";
		dMap.out(n->token);
		n = n->children.back();
		std::cout << "] ";
	}
	if(any)
	{
		std::cout << std::endl;
		level += 4;
		for(int i = 0; i < level; ++i)
			std::cout << " ";
	
		std::cout << n->lineNum << ":";
	}
	
	if(n->token >= 33 && n->token <= 127)
		std::cout << (char)(n->token);
	else
	{
		dMap.out(n->token);
	}
	if(!n->text.empty())
		std::cout << "('" << n->text << "')";
	std::cout << std::endl;
	for(std::list<ParseNode *>::iterator i = n->children.begin(); i != n->children.end(); ++i)
	{
		print_node(*i, level + 4);
	}
}

int main()
{
	std::ifstream fi("a.in");
	if(fi == NULL)
	{
		std::cerr << "Unable to open file \'a.in\'!" << std::endl;
		return 1;
	}
	
	try
	{
		TokenStream ts(fi, "a.in");
		try
		{
			CParser parser(ts);
			ParseNode *n = parser.parse();

#if 0
			if(n == NULL)
			{
				std::cout << "(no parse tree generated)" << std::endl;
			}
			else
			{
				std::cout << "--- begin parse tree ---" << std::endl;
				print_node(n, 0);
				std::cout << "--- end parse tree ---" << std::endl;
			}
#endif

			if(n == NULL)
			{
				std::cerr << "Error: Nothing to do! (Input is empty)." << std::endl;
				return 1;
			}
			
			Pass1 p1;
			p1.accept(n);
			
			// Generate outputs for external things.
			make_out_externals(p1);
			
			// Generate out_emulator.cpp.
			make_out_emulator(p1);
			
			// Generate out_metadata.cpp.
			make_out_metadata(p1);

			//***ncc.parseStart(n);
			///make_emulator(n);
			
			delete n;
		}
		catch(ParseError &e)
		{
			int fileNum = ts.getFileNum();
			int lineNum = ts.getLineNum();
			if(e.lineNum != 0)
			{
				lineNum = e.lineNum;
				fileNum = e.fileNum;
			}
			std::cerr << "error: " << ts.getFileName(fileNum) << ": parsing line " << lineNum << ": " << e.what() << std::endl;
			if(e.srcLine != 0)
				std::cerr << "note: ref. line number: " << e.srcLine << std::endl;
			return 1;
		}
	}
	catch(LexerError &e)
	{
		std::cerr << "error: " << e.fileName << ": lexing line " << e.lineNum << ": " << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}

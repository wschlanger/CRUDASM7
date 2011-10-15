// out_rulecom_parser.h - automatically generated file (do not edit!)
// Copyright (C) 2009 Willow Schlanger

ParseNode *parse_rcRULE_start()
{
	ParseNode *node = new ParseNode(RULE_rcstart, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule statement
			case TOKEN_rcIDENT:
			case KEYWORD_rctoken:	// "token"
			case KEYWORD_rckeyword:	// "keyword"
			case KEYWORD_rccopyright:	// "copyright"
			case KEYWORD_rcprefix:	// "prefix"
			case KEYWORD_rcmanual:	// "manual"
			case KEYWORD_rcstart:	// "start"
			// end rule statement
				addNonterminal(node, parse_rcRULE_statement());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			// begin rule statement
			case TOKEN_rcIDENT:
			case KEYWORD_rctoken:	// "token"
			case KEYWORD_rckeyword:	// "keyword"
			case KEYWORD_rccopyright:	// "copyright"
			case KEYWORD_rcprefix:	// "prefix"
			case KEYWORD_rcmanual:	// "manual"
			case KEYWORD_rcstart:	// "start"
			// end rule statement
				addNonterminal(node, parse_rcRULE_statement());
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule statement
			case TOKEN_rcIDENT:
			case KEYWORD_rctoken:	// "token"
			case KEYWORD_rckeyword:	// "keyword"
			case KEYWORD_rccopyright:	// "copyright"
			case KEYWORD_rcprefix:	// "prefix"
			case KEYWORD_rcmanual:	// "manual"
			case KEYWORD_rcstart:	// "start"
			// end rule statement
				addNonterminal(node, parse_rcRULE_statement());
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
	}
	catch(ParseSyntaxError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_rcRULE_statement()
{
	ParseNode *node = new ParseNode(RULE_rcstatement, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_rctoken:	// "token"
				addTerminal(node);
				nextToken();
				goto L_1;
			// begin rule name
			case TOKEN_rcIDENT:
			case KEYWORD_rcstart:	// "start"
			// end rule name
				addNonterminal(node, parse_rcRULE_name());
				goto L_5;
			case KEYWORD_rckeyword:	// "keyword"
				addTerminal(node);
				nextToken();
				goto L_11;
			case KEYWORD_rccopyright:	// "copyright"
				addTerminal(node);
				nextToken();
				goto L_15;
			case KEYWORD_rcprefix:	// "prefix"
				addTerminal(node);
				nextToken();
				goto L_18;
			case KEYWORD_rcmanual:	// "manual"
				addTerminal(node);
				nextToken();
				goto L_21;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case TOKEN_rcIDENT:
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_2:
		switch(getToken())
		{
			case TOKEN_rcIDENT:
				addTerminal(node);
				nextToken();
				goto L_3;
			case ';':
				addTerminal(node);
				nextToken();
				goto L_4;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case TOKEN_rcIDENT:
				addTerminal(node);
				nextToken();
				goto L_3;
			case ';':
				addTerminal(node);
				nextToken();
				goto L_4;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_4:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_5:
		switch(getToken())
		{
			case KEYWORD_rcISDEFAS:	// "::="
				addTerminal(node);
				nextToken();
				goto L_6;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_6:
		switch(getToken())
		{
			// begin rule alternate
			case TOKEN_rcIDENT:
			case TOKEN_rcLITCHAR:
			case TOKEN_rcLITSTRING:
			case '{':
			case '[':
			// end rule alternate
				addNonterminal(node, parse_rcRULE_alternate());
				goto L_7;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_7:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_10;
			case '|':
				addTerminal(node);
				nextToken();
				goto L_8;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_8:
		switch(getToken())
		{
			// begin rule alternate
			case TOKEN_rcIDENT:
			case TOKEN_rcLITCHAR:
			case TOKEN_rcLITSTRING:
			case '{':
			case '[':
			// end rule alternate
				addNonterminal(node, parse_rcRULE_alternate());
				goto L_9;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_9:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_10;
			case '|':
				addTerminal(node);
				nextToken();
				goto L_8;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_10:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_11:
		switch(getToken())
		{
			case TOKEN_rcLITSTRING:
				addTerminal(node);
				nextToken();
				goto L_12;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_12:
		switch(getToken())
		{
			case TOKEN_rcIDENT:
				addTerminal(node);
				nextToken();
				goto L_13;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_13:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_14;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_14:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_15:
		switch(getToken())
		{
			case TOKEN_rcLITSTRING:
				addTerminal(node);
				nextToken();
				goto L_16;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_16:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_17;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_17:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_18:
		switch(getToken())
		{
			case TOKEN_rcLITSTRING:
				addTerminal(node);
				nextToken();
				goto L_19;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_19:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_20;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_20:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_21:
		switch(getToken())
		{
			case TOKEN_rcIDENT:
				addTerminal(node);
				nextToken();
				goto L_22;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_22:
		switch(getToken())
		{
			case TOKEN_rcLITSTRING:
				addTerminal(node);
				nextToken();
				goto L_23;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_23:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_24;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_24:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
	}
	catch(ParseSyntaxError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_rcRULE_name()
{
	ParseNode *node = new ParseNode(RULE_rcname, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case TOKEN_rcIDENT:
				addTerminal(node);
				nextToken();
				goto L_1;
			case KEYWORD_rcstart:	// "start"
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
	}
	catch(ParseSyntaxError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_rcRULE_alternate()
{
	ParseNode *node = new ParseNode(RULE_rcalternate, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule symbol
			case TOKEN_rcIDENT:
			case TOKEN_rcLITCHAR:
			case TOKEN_rcLITSTRING:
			case '{':
			case '[':
			// end rule symbol
				addNonterminal(node, parse_rcRULE_symbol());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			// begin rule symbol
			case TOKEN_rcIDENT:
			case TOKEN_rcLITCHAR:
			case TOKEN_rcLITSTRING:
			case '{':
			case '[':
			// end rule symbol
				addNonterminal(node, parse_rcRULE_symbol());
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule symbol
			case TOKEN_rcIDENT:
			case TOKEN_rcLITCHAR:
			case TOKEN_rcLITSTRING:
			case '{':
			case '[':
			// end rule symbol
				addNonterminal(node, parse_rcRULE_symbol());
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
	}
	catch(ParseSyntaxError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_rcRULE_symbol()
{
	ParseNode *node = new ParseNode(RULE_rcsymbol, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule item
			case TOKEN_rcIDENT:
			case TOKEN_rcLITCHAR:
			case TOKEN_rcLITSTRING:
			// end rule item
				addNonterminal(node, parse_rcRULE_item());
				goto L_1;
			case '{':
				addTerminal(node);
				nextToken();
				goto L_2;
			case '[':
				addTerminal(node);
				nextToken();
				goto L_5;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule alternate
			case TOKEN_rcIDENT:
			case TOKEN_rcLITCHAR:
			case TOKEN_rcLITSTRING:
			case '{':
			case '[':
			// end rule alternate
				addNonterminal(node, parse_rcRULE_alternate());
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case '}':
				addTerminal(node);
				nextToken();
				goto L_4;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_4:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_5:
		switch(getToken())
		{
			// begin rule alternate
			case TOKEN_rcIDENT:
			case TOKEN_rcLITCHAR:
			case TOKEN_rcLITSTRING:
			case '{':
			case '[':
			// end rule alternate
				addNonterminal(node, parse_rcRULE_alternate());
				goto L_6;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_6:
		switch(getToken())
		{
			case ']':
				addTerminal(node);
				nextToken();
				goto L_7;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_7:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
	}
	catch(ParseSyntaxError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_rcRULE_item()
{
	ParseNode *node = new ParseNode(RULE_rcitem, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case TOKEN_rcIDENT:
				addTerminal(node);
				nextToken();
				goto L_3;
			case TOKEN_rcLITCHAR:
				addTerminal(node);
				nextToken();
				goto L_1;
			case TOKEN_rcLITSTRING:
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_3:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
	}
	catch(ParseSyntaxError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}



// out_parser.h
// Copyright (C) 2009 Willow Schlanger


ParseNode *parse_RULE_ARG()
{
	ParseNode *node = new ParseNode(RULE_ARG, getLineNum(), getFileNum());
	try
	{
		goto L_102378;
		L_102478:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addTerminal(node);
				advanceToken();
				goto L_1022e8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_102378:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addTerminal(node);
				advanceToken();
				goto L_102068;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_102068:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addTerminal(node);
				advanceToken();
				goto L_102178;
			case '*':
				addTerminal(node);
				advanceToken();
				goto L_102208;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1022e8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_102178:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_102208:
		switch(getCurrentToken())
		{
			case 0xffffff02:
				addTerminal(node);
				advanceToken();
				goto L_102478;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_ARGS()
{
	ParseNode *node = new ParseNode(RULE_ARGS, getLineNum(), getFileNum());
	try
	{
		goto L_100798;
		L_100d38:
		switch(getCurrentToken())
		{
			case ',':
				addTerminal(node);
				advanceToken();
				goto L_100c78;
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_100c78:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addNonterminal(node, parse_RULE_ARG());
				goto L_100d38;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_100798:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addNonterminal(node, parse_RULE_ARG());
				goto L_100d38;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_BODY()
{
	ParseNode *node = new ParseNode(RULE_BODY, getLineNum(), getFileNum());
	try
	{
		goto L_101358;
		L_103578:
		switch(getCurrentToken())
		{
			case ';':
				addTerminal(node);
				advanceToken();
				goto L_1013e8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_101358:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0x30000:
			case 0x30001:
			case 0x30002:
			case 0x30003:
			case 0x30004:
			case 0x30005:
			case 0x30007:
			case 0x30009:
			case 0x3000a:
			case 0x3000b:
			case 0x3000c:
			case 0x3000d:
			case 0x3000e:
			case 0x3000f:
			case 0x30010:
				addNonterminal(node, parse_RULE_STATEMENT());
				goto L_103578;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1013e8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0x30000:
			case 0x30001:
			case 0x30002:
			case 0x30003:
			case 0x30004:
			case 0x30005:
			case 0x30007:
			case 0x30009:
			case 0x3000a:
			case 0x3000b:
			case 0x3000c:
			case 0x3000d:
			case 0x3000e:
			case 0x3000f:
			case 0x30010:
				addNonterminal(node, parse_RULE_STATEMENT());
				goto L_1014c8;
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_1014c8:
		switch(getCurrentToken())
		{
			case ';':
				addTerminal(node);
				advanceToken();
				goto L_1013e8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_DECLARATION()
{
	ParseNode *node = new ParseNode(RULE_DECLARATION, getLineNum(), getFileNum());
	try
	{
		goto L_1032b8;
		L_103458:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addNonterminal(node, parse_RULE_PROTOTYPE());
				goto L_103198;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_103198:
		switch(getCurrentToken())
		{
			case '{':
				addTerminal(node);
				advanceToken();
				goto L_1027d8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1032b8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addNonterminal(node, parse_RULE_PROTOTYPE_D_TYPE());
				goto L_1028e8;
			case 0x30006:
				addTerminal(node);
				advanceToken();
				goto L_102dd8;
			case 0x30008:
				addTerminal(node);
				advanceToken();
				goto L_103458;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1027d8:
		switch(getCurrentToken())
		{
			case '}':
				addTerminal(node);
				advanceToken();
				goto L_103028;
			case 0xffffff01:
			case 0x30000:
			case 0x30001:
			case 0x30002:
			case 0x30003:
			case 0x30004:
			case 0x30005:
			case 0x30007:
			case 0x30009:
			case 0x3000a:
			case 0x3000b:
			case 0x3000c:
			case 0x3000d:
			case 0x3000e:
			case 0x3000f:
			case 0x30010:
				addNonterminal(node, parse_RULE_BODY());
				goto L_1030b8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1030b8:
		switch(getCurrentToken())
		{
			case '}':
				addTerminal(node);
				advanceToken();
				goto L_102f98;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_102f98:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_103028:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_1028e8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addNonterminal(node, parse_RULE_PROTOTYPE());
				goto L_102eb8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_102eb8:
		switch(getCurrentToken())
		{
			case ';':
				addTerminal(node);
				advanceToken();
				goto L_1029c8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1029c8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_102dd8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addNonterminal(node, parse_RULE_PROTOTYPE_D_TYPE());
				goto L_102a58;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_102a58:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addTerminal(node);
				advanceToken();
				goto L_102ca8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_102ca8:
		switch(getCurrentToken())
		{
			case ',':
				addTerminal(node);
				advanceToken();
				goto L_102b38;
			case ';':
				addTerminal(node);
				advanceToken();
				goto L_102c18;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_102b38:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addTerminal(node);
				advanceToken();
				goto L_102ca8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_102c18:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_DECLARATION_D_SEQ()
{
	ParseNode *node = new ParseNode(RULE_DECLARATION_D_SEQ, getLineNum(), getFileNum());
	try
	{
		goto L_100938;
		L_100a18:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0x30006:
			case 0x30008:
				addNonterminal(node, parse_RULE_DECLARATION());
				goto L_100a18;
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_100938:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0x30006:
			case 0x30008:
				addNonterminal(node, parse_RULE_DECLARATION());
				goto L_100a18;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_LIST()
{
	ParseNode *node = new ParseNode(RULE_LIST, getLineNum(), getFileNum());
	try
	{
		goto L_105268;
		L_1057c8:
		switch(getCurrentToken())
		{
			case ',':
				addTerminal(node);
				advanceToken();
				goto L_105148;
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_105268:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_1057c8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_105148:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_1057c8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_PREFIX()
{
	ParseNode *node = new ParseNode(RULE_PREFIX, getLineNum(), getFileNum());
	try
	{
		goto L_103de8;
		L_103de8:
		switch(getCurrentToken())
		{
			case 0x30000:
				addTerminal(node);
				advanceToken();
				goto L_104598;
			case 0x30001:
				addTerminal(node);
				advanceToken();
				goto L_104058;
			case 0x30002:
				addTerminal(node);
				advanceToken();
				goto L_104298;
			case 0x30009:
				addTerminal(node);
				advanceToken();
				goto L_104658;
			case 0x3000a:
				addTerminal(node);
				advanceToken();
				goto L_104138;
			case 0x3000b:
				addTerminal(node);
				advanceToken();
				goto L_103fc8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_103ce8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_103d58:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_104058:
		switch(getCurrentToken())
		{
			case ':':
				addTerminal(node);
				advanceToken();
				goto L_104738;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_104138:
		switch(getCurrentToken())
		{
			case ':':
				addTerminal(node);
				advanceToken();
				goto L_103ce8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_104528:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_103fc8:
		switch(getCurrentToken())
		{
			case ':':
				addTerminal(node);
				advanceToken();
				goto L_104528;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_104598:
		switch(getCurrentToken())
		{
			case ':':
				addTerminal(node);
				advanceToken();
				goto L_104408;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_104408:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_104658:
		switch(getCurrentToken())
		{
			case ':':
				addTerminal(node);
				advanceToken();
				goto L_103d58;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_104378:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_104738:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_104298:
		switch(getCurrentToken())
		{
			case ':':
				addTerminal(node);
				advanceToken();
				goto L_104378;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_PROTOTYPE()
{
	ParseNode *node = new ParseNode(RULE_PROTOTYPE, getLineNum(), getFileNum());
	try
	{
		goto L_101b38;
		L_101c38:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_101a18;
			case 0xffffff01:
				addNonterminal(node, parse_RULE_ARGS());
				goto L_101978;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_101868:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_101c38;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_101b38:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addTerminal(node);
				advanceToken();
				goto L_101868;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_101978:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_101aa8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_101a18:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_101aa8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_PROTOTYPE_D_TYPE()
{
	ParseNode *node = new ParseNode(RULE_PROTOTYPE_D_TYPE, getLineNum(), getFileNum());
	try
	{
		goto L_101258;
		L_100b18:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_101258:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addTerminal(node);
				advanceToken();
				goto L_1010b8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1010b8:
		switch(getCurrentToken())
		{
			case '*':
				addTerminal(node);
				advanceToken();
				goto L_101178;
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_101178:
		switch(getCurrentToken())
		{
			case 0xffffff02:
				addTerminal(node);
				advanceToken();
				goto L_100b18;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_RHS()
{
	ParseNode *node = new ParseNode(RULE_RHS, getLineNum(), getFileNum());
	try
	{
		goto L_108968;
		L_108d68:
		switch(getCurrentToken())
		{
			case '(':
				addNonterminal(node, parse_RULE_RHS_D_ARGS());
				goto L_108848;
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_108bf8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_108c88:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addNonterminal(node, parse_RULE_RHS_D_SIZE());
				goto L_108768;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_108688:
		switch(getCurrentToken())
		{
			case '<':
				addTerminal(node);
				advanceToken();
				goto L_108c88;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_108968:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addTerminal(node);
				advanceToken();
				goto L_108d68;
			case 0xffffff02:
				addTerminal(node);
				advanceToken();
				goto L_108bf8;
			case 0x30011:
				addTerminal(node);
				advanceToken();
				goto L_107ea8;
			case 0x30012:
				addTerminal(node);
				advanceToken();
				goto L_108688;
			case 0x30013:
				addTerminal(node);
				advanceToken();
				goto L_107568;
			case 0x30014:
				addTerminal(node);
				advanceToken();
				goto L_108148;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_108768:
		switch(getCurrentToken())
		{
			case '>':
				addTerminal(node);
				advanceToken();
				goto L_108068;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1088d8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_108848:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_107568:
		switch(getCurrentToken())
		{
			case '<':
				addTerminal(node);
				advanceToken();
				goto L_1085a8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1085a8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addNonterminal(node, parse_RULE_RHS_D_SIZE());
				goto L_1083e8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1083e8:
		switch(getCurrentToken())
		{
			case '>':
				addTerminal(node);
				advanceToken();
				goto L_107c08;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1084c8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_107a48;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_107f88:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_108308;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_108308:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_1078d8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_108228:
		switch(getCurrentToken())
		{
			case '>':
				addTerminal(node);
				advanceToken();
				goto L_107f88;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_108068:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_1084c8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_108148:
		switch(getCurrentToken())
		{
			case '<':
				addTerminal(node);
				advanceToken();
				goto L_1075f8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1075f8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addNonterminal(node, parse_RULE_RHS_D_SIZE());
				goto L_108228;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_107ea8:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_107ce8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_107ce8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addNonterminal(node, parse_RULE_RHS_D_SIZE());
				goto L_107dc8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_107dc8:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_1076d8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1076d8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_107c08:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_107768;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_107768:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_107b28;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_107b28:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_107848;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_107848:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_107a48:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_1088d8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1078d8:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_1079b8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1079b8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_RHS_D_ARGS()
{
	ParseNode *node = new ParseNode(RULE_RHS_D_ARGS, getLineNum(), getFileNum());
	try
	{
		goto L_103a88;
		L_104dc8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_103a88:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_105418;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1037d8:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_105388;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_105388:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_105418:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_104dc8;
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_LIST());
				goto L_1037d8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_RHS_D_SIZE()
{
	ParseNode *node = new ParseNode(RULE_RHS_D_SIZE, getLineNum(), getFileNum());
	try
	{
		goto L_103b38;
		L_104ef8:
		switch(getCurrentToken())
		{
			case '*':
				addTerminal(node);
				advanceToken();
				goto L_104e38;
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_105068:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_103b38:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addTerminal(node);
				advanceToken();
				goto L_104ef8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_104e38:
		switch(getCurrentToken())
		{
			case 0xffffff02:
				addTerminal(node);
				advanceToken();
				goto L_105068;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_SIMPLE_D_STATEMENT()
{
	ParseNode *node = new ParseNode(RULE_SIMPLE_D_STATEMENT, getLineNum(), getFileNum());
	try
	{
		goto L_10bbb8;
		L_10c798:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addTerminal(node);
				advanceToken();
				goto L_10c698;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10c598:
		switch(getCurrentToken())
		{
			case '=':
				addTerminal(node);
				advanceToken();
				goto L_10c078;
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_10c698:
		switch(getCurrentToken())
		{
			case '=':
				addTerminal(node);
				advanceToken();
				goto L_10c158;
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_10c238:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addTerminal(node);
				advanceToken();
				goto L_10c598;
			case '*':
				addTerminal(node);
				advanceToken();
				goto L_10c478;
			case '=':
				addTerminal(node);
				advanceToken();
				goto L_10bad8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10c478:
		switch(getCurrentToken())
		{
			case 0xffffff02:
				addTerminal(node);
				advanceToken();
				goto L_10c798;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10bbb8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
				addTerminal(node);
				advanceToken();
				goto L_10c238;
			case 0x30003:
				addTerminal(node);
				advanceToken();
				goto L_10b9f8;
			case 0x30004:
				addTerminal(node);
				advanceToken();
				goto L_10ae68;
			case 0x30005:
				addTerminal(node);
				advanceToken();
				goto L_10b198;
			case 0x30007:
				addTerminal(node);
				advanceToken();
				goto L_10a808;
			case 0x3000c:
				addTerminal(node);
				advanceToken();
				goto L_10aa58;
			case 0x3000d:
				addTerminal(node);
				advanceToken();
				goto L_10b3e8;
			case 0x3000e:
				addTerminal(node);
				advanceToken();
				goto L_10b7a8;
			case 0x3000f:
				addTerminal(node);
				advanceToken();
				goto L_10a318;
			case 0x30010:
				addTerminal(node);
				advanceToken();
				goto L_10a0c8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10c158:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_10bfe8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10bfe8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_10c078:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_10b5a8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10b5a8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_10bad8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_10b968;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10b968:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_10b9f8:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_10b638;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10b638:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_10b888;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10b888:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_10b718;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10b718:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_10b7a8:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_1099d8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_1099d8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_10b4c8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10b4c8:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_10b358;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10b358:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_10b3e8:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_10b028;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10b028:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_10b278;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10b278:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_10b108;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10b108:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_10b198:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_109a68;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_109a68:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_10af48;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10af48:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_10add8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10add8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_10ae68:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_109c28;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10aa58:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_10acf8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10acf8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_10ab38;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10ab38:
		switch(getCurrentToken())
		{
			case ',':
				addTerminal(node);
				advanceToken();
				goto L_10ac18;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10ac18:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_109b48;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_109b48:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_10a9c8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10a9c8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_10a808:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_10a8e8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10a8e8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_10a4d8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10a4d8:
		switch(getCurrentToken())
		{
			case ',':
				addTerminal(node);
				advanceToken();
				goto L_10a728;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10a728:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_10a5b8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10a5b8:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_10a698;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10a698:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_109c28:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_10a3f8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10a3f8:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_10a288;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10a288:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_10a318:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_109d08;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_109d08:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_10a1a8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10a1a8:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_109de8;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_109de8:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_10a0c8:
		switch(getCurrentToken())
		{
			case '(':
				addTerminal(node);
				advanceToken();
				goto L_109e78;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_109e78:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0xffffff02:
			case 0x30011:
			case 0x30012:
			case 0x30013:
			case 0x30014:
				addNonterminal(node, parse_RULE_RHS());
				goto L_109f58;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_109f58:
		switch(getCurrentToken())
		{
			case ')':
				addTerminal(node);
				advanceToken();
				goto L_10a038;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_10a038:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_START()
{
	ParseNode *node = new ParseNode(RULE_START, getLineNum(), getFileNum());
	try
	{
		goto L_1005f8;
		L_1005f8:
		switch(getCurrentToken())
		{
			case 0xffffff01:
			case 0x30006:
			case 0x30008:
				addNonterminal(node, parse_RULE_DECLARATION_D_SEQ());
				goto L_100688;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
		L_100688:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_RULE_STATEMENT()
{
	ParseNode *node = new ParseNode(RULE_STATEMENT, getLineNum(), getFileNum());
	try
	{
		goto L_102558;
		L_101d88:
		switch(getCurrentToken())
		{
			case 0xffffffff:
			default:
				return node;	// accept state
				break;
		}
		L_102558:
		switch(getCurrentToken())
		{
			case 0x30000:
			case 0x30001:
			case 0x30002:
			case 0x30009:
			case 0x3000a:
			case 0x3000b:
				addNonterminal(node, parse_RULE_PREFIX());
				goto L_102558;
			case 0xffffff01:
			case 0x30003:
			case 0x30004:
			case 0x30005:
			case 0x30007:
			case 0x3000c:
			case 0x3000d:
			case 0x3000e:
			case 0x3000f:
			case 0x30010:
				addNonterminal(node, parse_RULE_SIMPLE_D_STATEMENT());
				goto L_101d88;
			case 0xffffffff:
			default:
				throw ParseError("syntax error");
				break;
		}
	}
	catch(ParseError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}


// out_intel_parser.h - automatically generated file (do not edit!)
// Copyright (C) 2009,2011 Willow Schlanger

ParseNode *parse_ixRULE_group_def()
{
	ParseNode *node = new ParseNode(RULE_ixgroup_def, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixgroup:	// "group"
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
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
			case TOKEN_ixSQUARE:
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case ',':
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
			case TOKEN_ixSQUARE:
				addTerminal(node);
				nextToken();
				goto L_5;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_5:
		switch(getToken())
		{
			case ';':
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

ParseNode *parse_ixRULE_insn_arg_list()
{
	ParseNode *node = new ParseNode(RULE_ixinsn_arg_list, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule insn_arg
			case TOKEN_ixIDENT:
			// end rule insn_arg
				addNonterminal(node, parse_ixRULE_insn_arg());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule insn_arg
			case TOKEN_ixIDENT:
			// end rule insn_arg
				addNonterminal(node, parse_ixRULE_insn_arg());
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
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

ParseNode *parse_ixRULE_insn_body()
{
	ParseNode *node = new ParseNode(RULE_ixinsn_body, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixemu:	// "emu"
				addTerminal(node);
				nextToken();
				goto L_27;
			case KEYWORD_ixgroup:	// "group"
				addTerminal(node);
				nextToken();
				goto L_1;
			case KEYWORD_ixflags:	// "flags"
				addTerminal(node);
				nextToken();
				goto L_4;
			case KEYWORD_ixsynopsis:	// "synopsis"
				addTerminal(node);
				nextToken();
				goto L_9;
			case KEYWORD_ixdesc:	// "desc"
				addTerminal(node);
				nextToken();
				goto L_13;
			case KEYWORD_ixvalid:	// "valid"
				addTerminal(node);
				nextToken();
				goto L_17;
			case KEYWORD_ixverified:	// "verified"
				addTerminal(node);
				nextToken();
				goto L_23;
			case KEYWORD_ixcode:	// "code"
				addTerminal(node);
				nextToken();
				goto L_34;
			case KEYWORD_ixnotes:	// "notes"
				addTerminal(node);
				nextToken();
				goto L_37;
			case KEYWORD_ixdis:	// "dis"
				addTerminal(node);
				nextToken();
				goto L_40;
			case KEYWORD_ixasm:	// "asm"
				addTerminal(node);
				nextToken();
				goto L_46;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
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
			case ';':
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_4:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_5;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_5:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_6;
			case ';':
				addTerminal(node);
				nextToken();
				goto L_8;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_6:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
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
			case ',':
				addTerminal(node);
				nextToken();
				goto L_6;
			case ';':
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
			default:
				return node;	// accept state
				break;
		}
		L_9:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_10;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_10:
		switch(getToken())
		{
			// begin rule remarks
			case TOKEN_ixSQUARE:
			// end rule remarks
				addNonterminal(node, parse_ixRULE_remarks());
				goto L_11;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_11:
		switch(getToken())
		{
			case ';':
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
			default:
				return node;	// accept state
				break;
		}
		L_13:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
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
			// begin rule remarks
			case TOKEN_ixSQUARE:
			// end rule remarks
				addNonterminal(node, parse_ixRULE_remarks());
				goto L_15;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_15:
		switch(getToken())
		{
			case ';':
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
			default:
				return node;	// accept state
				break;
		}
		L_17:
		switch(getToken())
		{
			case KEYWORD_ixmodes:	// "modes"
				addTerminal(node);
				nextToken();
				goto L_18;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_18:
		switch(getToken())
		{
			case '(':
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
			// begin rule ident_list
			case TOKEN_ixIDENT:
			// end rule ident_list
				addNonterminal(node, parse_ixRULE_ident_list());
				goto L_20;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_20:
		switch(getToken())
		{
			case ')':
				addTerminal(node);
				nextToken();
				goto L_21;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_21:
		switch(getToken())
		{
			case ';':
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
			default:
				return node;	// accept state
				break;
		}
		L_23:
		switch(getToken())
		{
			case KEYWORD_ixemu:	// "emu"
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
			case TOKEN_ixSQUARE:
				addTerminal(node);
				nextToken();
				goto L_25;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_25:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_26;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_26:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_27:
		switch(getToken())
		{
			case '(':
				addTerminal(node);
				nextToken();
				goto L_28;
			// begin rule emu_body
			case KEYWORD_ixbegin:	// "begin"
			// end rule emu_body
				addNonterminal(node, parse_ixRULE_emu_body());
				goto L_31;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_28:
		switch(getToken())
		{
			// begin rule emu_sens
			case TOKEN_ixIDENT:
			// end rule emu_sens
				addNonterminal(node, parse_ixRULE_emu_sens());
				goto L_29;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_29:
		switch(getToken())
		{
			case ')':
				addTerminal(node);
				nextToken();
				goto L_30;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_30:
		switch(getToken())
		{
			// begin rule emu_body
			case KEYWORD_ixbegin:	// "begin"
			// end rule emu_body
				addNonterminal(node, parse_ixRULE_emu_body());
				goto L_31;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_31:
		switch(getToken())
		{
			case KEYWORD_ixemu:	// "emu"
				addTerminal(node);
				nextToken();
				goto L_32;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_32:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_33;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_33:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_34:
		switch(getToken())
		{
			// begin rule machine_code
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule machine_code
				addNonterminal(node, parse_ixRULE_machine_code());
				goto L_35;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_35:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_36;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_36:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_37:
		switch(getToken())
		{
			// begin rule remarks
			case TOKEN_ixSQUARE:
			// end rule remarks
				addNonterminal(node, parse_ixRULE_remarks());
				goto L_38;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_38:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_39;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_39:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_40:
		switch(getToken())
		{
			case KEYWORD_ixbegin:	// "begin"
				addTerminal(node);
				nextToken();
				goto L_41;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_41:
		switch(getToken())
		{
			case KEYWORD_ixend:	// "end"
				addTerminal(node);
				nextToken();
				goto L_43;
			// begin rule dis_body
			case KEYWORD_ixif:	// "if"
			case TOKEN_ixIDENT:
			// end rule dis_body
				addNonterminal(node, parse_ixRULE_dis_body());
				goto L_42;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_42:
		switch(getToken())
		{
			case KEYWORD_ixend:	// "end"
				addTerminal(node);
				nextToken();
				goto L_43;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_43:
		switch(getToken())
		{
			case KEYWORD_ixdis:	// "dis"
				addTerminal(node);
				nextToken();
				goto L_44;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_44:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_45;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_45:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_46:
		switch(getToken())
		{
			case KEYWORD_ixnames:	// "names"
				addTerminal(node);
				nextToken();
				goto L_47;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_47:
		switch(getToken())
		{
			case TOKEN_ixSQUARE:
				addTerminal(node);
				nextToken();
				goto L_48;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_48:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_49;
			case ';':
				addTerminal(node);
				nextToken();
				goto L_51;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_49:
		switch(getToken())
		{
			case TOKEN_ixSQUARE:
				addTerminal(node);
				nextToken();
				goto L_50;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_50:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_49;
			case ';':
				addTerminal(node);
				nextToken();
				goto L_51;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_51:
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

ParseNode *parse_ixRULE_emu_body()
{
	ParseNode *node = new ParseNode(RULE_ixemu_body, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixbegin:	// "begin"
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case KEYWORD_ixend:	// "end"
				addTerminal(node);
				nextToken();
				goto L_3;
			// begin rule emu_stmt
			case KEYWORD_ixexcept_assert:	// "except_assert"
			case KEYWORD_ixreturn:	// "return"
			case KEYWORD_ixtimes:	// "times"
			case TOKEN_ixIDENT:
			// end rule emu_stmt
				addNonterminal(node, parse_ixRULE_emu_stmt());
				goto L_2;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_2:
		switch(getToken())
		{
			case KEYWORD_ixend:	// "end"
				addTerminal(node);
				nextToken();
				goto L_3;
			// begin rule emu_stmt
			case KEYWORD_ixexcept_assert:	// "except_assert"
			case KEYWORD_ixreturn:	// "return"
			case KEYWORD_ixtimes:	// "times"
			case TOKEN_ixIDENT:
			// end rule emu_stmt
				addNonterminal(node, parse_ixRULE_emu_stmt());
				goto L_2;
			default:
				throw ParseSyntaxError();
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

ParseNode *parse_ixRULE_emu_sens()
{
	ParseNode *node = new ParseNode(RULE_ixemu_sens, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
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

ParseNode *parse_ixRULE_ident_list()
{
	ParseNode *node = new ParseNode(RULE_ixident_list, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
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

ParseNode *parse_ixRULE_insn_arg()
{
	ParseNode *node = new ParseNode(RULE_ixinsn_arg, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case '*':
				addTerminal(node);
				nextToken();
				goto L_2;
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_4;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_2:
		switch(getToken())
		{
			case TOKEN_ixLITNUM:
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
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
			case '=':
				addTerminal(node);
				nextToken();
				goto L_5;
			default:
				return node;	// accept state
				break;
		}
		L_5:
		switch(getToken())
		{
			case TOKEN_ixLITNUM:
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

ParseNode *parse_ixRULE_machine_code()
{
	ParseNode *node = new ParseNode(RULE_ixmachine_code, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule ascii
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule ascii
				addNonterminal(node, parse_ixRULE_ascii());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case '(':
				addTerminal(node);
				nextToken();
				goto L_4;
			// begin rule slash_code_item
			case '+':
			case '/':
			// end rule slash_code_item
				addNonterminal(node, parse_ixRULE_slash_code_item());
				goto L_3;
			// begin rule ascii
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule ascii
				addNonterminal(node, parse_ixRULE_ascii());
				goto L_2;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_2:
		switch(getToken())
		{
			case '(':
				addTerminal(node);
				nextToken();
				goto L_4;
			// begin rule slash_code_item
			case '+':
			case '/':
			// end rule slash_code_item
				addNonterminal(node, parse_ixRULE_slash_code_item());
				goto L_3;
			// begin rule ascii
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule ascii
				addNonterminal(node, parse_ixRULE_ascii());
				goto L_2;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case '(':
				addTerminal(node);
				nextToken();
				goto L_4;
			// begin rule slash_code_item
			case '+':
			case '/':
			// end rule slash_code_item
				addNonterminal(node, parse_ixRULE_slash_code_item());
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_4:
		switch(getToken())
		{
			// begin rule machine_arg_list
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule machine_arg_list
				addNonterminal(node, parse_ixRULE_machine_arg_list());
				goto L_5;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_5:
		switch(getToken())
		{
			case ')':
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
			case '<':
				addTerminal(node);
				nextToken();
				goto L_7;
			default:
				return node;	// accept state
				break;
		}
		L_7:
		switch(getToken())
		{
			// begin rule machine_extra_list
			case TOKEN_ixIDENT:
			// end rule machine_extra_list
				addNonterminal(node, parse_ixRULE_machine_extra_list());
				goto L_8;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_8:
		switch(getToken())
		{
			case '>':
				addTerminal(node);
				nextToken();
				goto L_9;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_9:
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

ParseNode *parse_ixRULE_dis_body()
{
	ParseNode *node = new ParseNode(RULE_ixdis_body, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule dis_stmt
			case KEYWORD_ixif:	// "if"
			case TOKEN_ixIDENT:
			// end rule dis_stmt
				addNonterminal(node, parse_ixRULE_dis_stmt());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			// begin rule dis_stmt
			case KEYWORD_ixif:	// "if"
			case TOKEN_ixIDENT:
			// end rule dis_stmt
				addNonterminal(node, parse_ixRULE_dis_stmt());
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule dis_stmt
			case KEYWORD_ixif:	// "if"
			case TOKEN_ixIDENT:
			// end rule dis_stmt
				addNonterminal(node, parse_ixRULE_dis_stmt());
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

ParseNode *parse_ixRULE_dis_stmt()
{
	ParseNode *node = new ParseNode(RULE_ixdis_stmt, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixif:	// "if"
				addTerminal(node);
				nextToken();
				goto L_6;
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case '(':
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
			case ')':
				addTerminal(node);
				nextToken();
				goto L_4;
			// begin rule rhs_list
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs_list
				addNonterminal(node, parse_ixRULE_rhs_list());
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case ')':
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
			case ';':
				addTerminal(node);
				nextToken();
				goto L_5;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_5:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_6:
		switch(getToken())
		{
			case '(':
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
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_8;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_8:
		switch(getToken())
		{
			case ')':
				addTerminal(node);
				nextToken();
				goto L_9;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_9:
		switch(getToken())
		{
			case KEYWORD_ixbegin:	// "begin"
				addTerminal(node);
				nextToken();
				goto L_10;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_10:
		switch(getToken())
		{
			case KEYWORD_ixend:	// "end"
				addTerminal(node);
				nextToken();
				goto L_12;
			// begin rule dis_body
			case KEYWORD_ixif:	// "if"
			case TOKEN_ixIDENT:
			// end rule dis_body
				addNonterminal(node, parse_ixRULE_dis_body());
				goto L_11;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_11:
		switch(getToken())
		{
			case KEYWORD_ixend:	// "end"
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
			case ';':
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

ParseNode *parse_ixRULE_rhs_list()
{
	ParseNode *node = new ParseNode(RULE_ixrhs_list, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
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

ParseNode *parse_ixRULE_rhs()
{
	ParseNode *node = new ParseNode(RULE_ixrhs, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule rhs_prefix
			case '-':
			case '~':
			case '!':
			// end rule rhs_prefix
				addNonterminal(node, parse_ixRULE_rhs_prefix());
				goto L_1;
			// begin rule rhs_terminal
			case '(':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs_terminal
				addNonterminal(node, parse_ixRULE_rhs_terminal());
				goto L_2;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			// begin rule rhs_terminal
			case '(':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs_terminal
				addNonterminal(node, parse_ixRULE_rhs_terminal());
				goto L_2;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_2:
		switch(getToken())
		{
			case '(':
				addTerminal(node);
				nextToken();
				goto L_5;
			// begin rule binary_op
			case '*':
			case '+':
			case KEYWORD_ixsigned:	// "signed"
			case KEYWORD_ixunsigned:	// "unsigned"
			case '-':
			case '|':
			case '^':
			case KEYWORD_ixEQ:	// "=="
			case KEYWORD_ixNE:	// "!="
			case KEYWORD_ixOROR:	// "||"
			case KEYWORD_ixANDAND:	// "&&"
			case KEYWORD_ixSHL:	// "<<"
			case '&':
			// end rule binary_op
				addNonterminal(node, parse_ixRULE_binary_op());
				goto L_3;
			default:
				return node;	// accept state
				break;
		}
		L_3:
		switch(getToken())
		{
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
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
			case ')':
				addTerminal(node);
				nextToken();
				goto L_6;
			// begin rule rhs_list
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs_list
				addNonterminal(node, parse_ixRULE_rhs_list());
				goto L_10;
			// begin rule signed_cast
			case KEYWORD_ixsigned:	// "signed"
			case KEYWORD_ixunsigned:	// "unsigned"
			// end rule signed_cast
				addNonterminal(node, parse_ixRULE_signed_cast());
				goto L_9;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_6:
		switch(getToken())
		{
			// begin rule binary_op
			case '*':
			case '+':
			case KEYWORD_ixsigned:	// "signed"
			case KEYWORD_ixunsigned:	// "unsigned"
			case '-':
			case '|':
			case '^':
			case KEYWORD_ixEQ:	// "=="
			case KEYWORD_ixNE:	// "!="
			case KEYWORD_ixOROR:	// "||"
			case KEYWORD_ixANDAND:	// "&&"
			case KEYWORD_ixSHL:	// "<<"
			case '&':
			// end rule binary_op
				addNonterminal(node, parse_ixRULE_binary_op());
				goto L_7;
			default:
				return node;	// accept state
				break;
		}
		L_7:
		switch(getToken())
		{
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_8;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_8:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_9:
		switch(getToken())
		{
			// begin rule rhs_list
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs_list
				addNonterminal(node, parse_ixRULE_rhs_list());
				goto L_10;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_10:
		switch(getToken())
		{
			case ')':
				addTerminal(node);
				nextToken();
				goto L_11;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_11:
		switch(getToken())
		{
			// begin rule binary_op
			case '*':
			case '+':
			case KEYWORD_ixsigned:	// "signed"
			case KEYWORD_ixunsigned:	// "unsigned"
			case '-':
			case '|':
			case '^':
			case KEYWORD_ixEQ:	// "=="
			case KEYWORD_ixNE:	// "!="
			case KEYWORD_ixOROR:	// "||"
			case KEYWORD_ixANDAND:	// "&&"
			case KEYWORD_ixSHL:	// "<<"
			case '&':
			// end rule binary_op
				addNonterminal(node, parse_ixRULE_binary_op());
				goto L_12;
			default:
				return node;	// accept state
				break;
		}
		L_12:
		switch(getToken())
		{
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_13;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_13:
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

ParseNode *parse_ixRULE_slash_code_item()
{
	ParseNode *node = new ParseNode(RULE_ixslash_code_item, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case '+':
				addTerminal(node);
				nextToken();
				goto L_3;
			case '/':
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			// begin rule slash_code
			case KEYWORD_ixr:	// "r"
			case KEYWORD_ixmod:	// "mod"
			case TOKEN_ixLITNUM:
			// end rule slash_code
				addNonterminal(node, parse_ixRULE_slash_code());
				goto L_2;
			default:
				throw ParseSyntaxError();
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
			case KEYWORD_ixr:	// "r"
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
	}
	catch(ParseSyntaxError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_ixRULE_machine_arg_list()
{
	ParseNode *node = new ParseNode(RULE_ixmachine_arg_list, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule argtype
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule argtype
				addNonterminal(node, parse_ixRULE_argtype());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule argtype
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule argtype
				addNonterminal(node, parse_ixRULE_argtype());
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
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

ParseNode *parse_ixRULE_machine_extra_list()
{
	ParseNode *node = new ParseNode(RULE_ixmachine_extra_list, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule machine_extra_item
			case TOKEN_ixIDENT:
			// end rule machine_extra_item
				addNonterminal(node, parse_ixRULE_machine_extra_item());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule machine_extra_item
			case TOKEN_ixIDENT:
			// end rule machine_extra_item
				addNonterminal(node, parse_ixRULE_machine_extra_item());
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
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

ParseNode *parse_ixRULE_slash_code()
{
	ParseNode *node = new ParseNode(RULE_ixslash_code, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixr:	// "r"
				addTerminal(node);
				nextToken();
				goto L_2;
			case KEYWORD_ixmod:	// "mod"
				addTerminal(node);
				nextToken();
				goto L_3;
			case TOKEN_ixLITNUM:
				addTerminal(node);
				nextToken();
				goto L_1;
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
			case ':':
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
			// begin rule mod_code
			case KEYWORD_ixmem_only:	// "mem_only"
			// end rule mod_code
				addNonterminal(node, parse_ixRULE_mod_code());
				goto L_5;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_5:
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

ParseNode *parse_ixRULE_mod_code()
{
	ParseNode *node = new ParseNode(RULE_ixmod_code, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixmem_only:	// "mem_only"
				addTerminal(node);
				nextToken();
				goto L_1;
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
	}
	catch(ParseSyntaxError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_ixRULE_machine_extra_item()
{
	ParseNode *node = new ParseNode(RULE_ixmachine_extra_item, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case '=':
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule machine_extra_asgn
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule machine_extra_asgn
				addNonterminal(node, parse_ixRULE_machine_extra_asgn());
				goto L_3;
			default:
				throw ParseSyntaxError();
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

ParseNode *parse_ixRULE_machine_extra_asgn()
{
	ParseNode *node = new ParseNode(RULE_ixmachine_extra_asgn, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule ascii
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule ascii
				addNonterminal(node, parse_ixRULE_ascii());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case '(':
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule machine_extra_tag_list
			case TOKEN_ixLITNUM:
			// end rule machine_extra_tag_list
				addNonterminal(node, parse_ixRULE_machine_extra_tag_list());
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case ')':
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
	}
	catch(ParseSyntaxError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_ixRULE_machine_extra_tag_list()
{
	ParseNode *node = new ParseNode(RULE_ixmachine_extra_tag_list, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case TOKEN_ixLITNUM:
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			case TOKEN_ixLITNUM:
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
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

ParseNode *parse_ixRULE_emu_stmt()
{
	ParseNode *node = new ParseNode(RULE_ixemu_stmt, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixexcept_assert:	// "except_assert"
				addTerminal(node);
				nextToken();
				goto L_8;
			case KEYWORD_ixreturn:	// "return"
				addTerminal(node);
				nextToken();
				goto L_15;
			case KEYWORD_ixtimes:	// "times"
				addTerminal(node);
				nextToken();
				goto L_18;
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case '(':
				addTerminal(node);
				nextToken();
				goto L_25;
			case '=':
				addTerminal(node);
				nextToken();
				goto L_3;
			case TOKEN_ixIDENT:
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
			case '=':
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_5;
			case KEYWORD_ixundefined:	// "undefined"
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
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_5;
			case ';':
				addTerminal(node);
				nextToken();
				goto L_7;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_5:
		switch(getToken())
		{
			case ';':
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
			default:
				return node;	// accept state
				break;
		}
		L_7:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_8:
		switch(getToken())
		{
			case '(':
				addTerminal(node);
				nextToken();
				goto L_9;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_9:
		switch(getToken())
		{
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_10;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_10:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_11;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_11:
		switch(getToken())
		{
			case TOKEN_ixLITNUM:
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
			case ')':
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
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
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
			// begin rule sizeref
			case TOKEN_ixIDENT:
			// end rule sizeref
				addNonterminal(node, parse_ixRULE_sizeref());
				goto L_19;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_19:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
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
			case '=':
				addTerminal(node);
				nextToken();
				goto L_21;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_21:
		switch(getToken())
		{
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_22;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_22:
		switch(getToken())
		{
			// begin rule emu_body
			case KEYWORD_ixbegin:	// "begin"
			// end rule emu_body
				addNonterminal(node, parse_ixRULE_emu_body());
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
		L_25:
		switch(getToken())
		{
			case TOKEN_ixLITNUM:
				addTerminal(node);
				nextToken();
				goto L_26;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_26:
		switch(getToken())
		{
			case ')':
				addTerminal(node);
				nextToken();
				goto L_27;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_27:
		switch(getToken())
		{
			case ':':
				addTerminal(node);
				nextToken();
				goto L_28;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_28:
		switch(getToken())
		{
			// begin rule emu_stmt
			case KEYWORD_ixexcept_assert:	// "except_assert"
			case KEYWORD_ixreturn:	// "return"
			case KEYWORD_ixtimes:	// "times"
			case TOKEN_ixIDENT:
			// end rule emu_stmt
				addNonterminal(node, parse_ixRULE_emu_stmt());
				goto L_29;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_29:
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

ParseNode *parse_ixRULE_sizeref()
{
	ParseNode *node = new ParseNode(RULE_ixsizeref, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case '*':
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			case TOKEN_ixLITNUM:
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				throw ParseSyntaxError();
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

ParseNode *parse_ixRULE_rhs_prefix()
{
	ParseNode *node = new ParseNode(RULE_ixrhs_prefix, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case '-':
				addTerminal(node);
				nextToken();
				goto L_1;
			case '~':
				addTerminal(node);
				nextToken();
				goto L_2;
			case '!':
				addTerminal(node);
				nextToken();
				goto L_3;
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

ParseNode *parse_ixRULE_rhs_terminal()
{
	ParseNode *node = new ParseNode(RULE_ixrhs_terminal, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case '(':
				addTerminal(node);
				nextToken();
				goto L_3;
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_1;
			case TOKEN_ixLITNUM:
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
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_4;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_4:
		switch(getToken())
		{
			case ')':
				addTerminal(node);
				nextToken();
				goto L_6;
			// begin rule alternates
			case '?':
			// end rule alternates
				addNonterminal(node, parse_ixRULE_alternates());
				goto L_5;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_5:
		switch(getToken())
		{
			case ')':
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

ParseNode *parse_ixRULE_binary_op()
{
	ParseNode *node = new ParseNode(RULE_ixbinary_op, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case '*':
				addTerminal(node);
				nextToken();
				goto L_20;
			case '+':
				addTerminal(node);
				nextToken();
				goto L_1;
			case KEYWORD_ixsigned:	// "signed"
				addTerminal(node);
				nextToken();
				goto L_6;
			case KEYWORD_ixunsigned:	// "unsigned"
				addTerminal(node);
				nextToken();
				goto L_11;
			case '-':
				addTerminal(node);
				nextToken();
				goto L_2;
			case '|':
				addTerminal(node);
				nextToken();
				goto L_3;
			case '^':
				addTerminal(node);
				nextToken();
				goto L_4;
			case KEYWORD_ixEQ:	// "=="
				addTerminal(node);
				nextToken();
				goto L_17;
			case KEYWORD_ixNE:	// "!="
				addTerminal(node);
				nextToken();
				goto L_16;
			case KEYWORD_ixOROR:	// "||"
				addTerminal(node);
				nextToken();
				goto L_18;
			case KEYWORD_ixANDAND:	// "&&"
				addTerminal(node);
				nextToken();
				goto L_19;
			case KEYWORD_ixSHL:	// "<<"
				addTerminal(node);
				nextToken();
				goto L_25;
			case '&':
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
			default:
				return node;	// accept state
				break;
		}
		L_6:
		switch(getToken())
		{
			case '<':
				addTerminal(node);
				nextToken();
				goto L_7;
			case '>':
				addTerminal(node);
				nextToken();
				goto L_8;
			case '%':
				addTerminal(node);
				nextToken();
				goto L_24;
			case KEYWORD_ixLE:	// "<="
				addTerminal(node);
				nextToken();
				goto L_9;
			case KEYWORD_ixGE:	// ">="
				addTerminal(node);
				nextToken();
				goto L_10;
			case KEYWORD_ixSHR:	// ">>"
				addTerminal(node);
				nextToken();
				goto L_27;
			case '/':
				addTerminal(node);
				nextToken();
				goto L_23;
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
		L_8:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_9:
		switch(getToken())
		{
			default:
				return node;	// accept state
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
			case '<':
				addTerminal(node);
				nextToken();
				goto L_12;
			case '>':
				addTerminal(node);
				nextToken();
				goto L_13;
			case '%':
				addTerminal(node);
				nextToken();
				goto L_22;
			case KEYWORD_ixLE:	// "<="
				addTerminal(node);
				nextToken();
				goto L_14;
			case KEYWORD_ixGE:	// ">="
				addTerminal(node);
				nextToken();
				goto L_15;
			case KEYWORD_ixSHR:	// ">>"
				addTerminal(node);
				nextToken();
				goto L_26;
			case '/':
				addTerminal(node);
				nextToken();
				goto L_21;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_12:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_13:
		switch(getToken())
		{
			default:
				return node;	// accept state
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
			default:
				return node;	// accept state
				break;
		}
		L_16:
		switch(getToken())
		{
			default:
				return node;	// accept state
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
			default:
				return node;	// accept state
				break;
		}
		L_19:
		switch(getToken())
		{
			default:
				return node;	// accept state
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
			default:
				return node;	// accept state
				break;
		}
		L_22:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_23:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_24:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_25:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_26:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_27:
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

ParseNode *parse_ixRULE_signed_cast()
{
	ParseNode *node = new ParseNode(RULE_ixsigned_cast, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixsigned:	// "signed"
				addTerminal(node);
				nextToken();
				goto L_1;
			case KEYWORD_ixunsigned:	// "unsigned"
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

ParseNode *parse_ixRULE_alternates()
{
	ParseNode *node = new ParseNode(RULE_ixalternates, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case '?':
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_2;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_2:
		switch(getToken())
		{
			case ':':
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			// begin rule rhs
			case '(':
			case '-':
			case '~':
			case '!':
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
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
	}
	catch(ParseSyntaxError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_ixRULE_start()
{
	ParseNode *node = new ParseNode(RULE_ixstart, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule item
			case KEYWORD_ixinsn:	// "insn"
			case KEYWORD_ixemu:	// "emu"
			case KEYWORD_ixknown:	// "known"
			case KEYWORD_ixset:	// "set"
			case KEYWORD_ixgroups:	// "groups"
			// end rule item
				addNonterminal(node, parse_ixRULE_item());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			// begin rule item
			case KEYWORD_ixinsn:	// "insn"
			case KEYWORD_ixemu:	// "emu"
			case KEYWORD_ixknown:	// "known"
			case KEYWORD_ixset:	// "set"
			case KEYWORD_ixgroups:	// "groups"
			// end rule item
				addNonterminal(node, parse_ixRULE_item());
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule item
			case KEYWORD_ixinsn:	// "insn"
			case KEYWORD_ixemu:	// "emu"
			case KEYWORD_ixknown:	// "known"
			case KEYWORD_ixset:	// "set"
			case KEYWORD_ixgroups:	// "groups"
			// end rule item
				addNonterminal(node, parse_ixRULE_item());
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

ParseNode *parse_ixRULE_item()
{
	ParseNode *node = new ParseNode(RULE_ixitem, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixinsn:	// "insn"
				addTerminal(node);
				nextToken();
				goto L_13;
			case KEYWORD_ixemu:	// "emu"
				addTerminal(node);
				nextToken();
				goto L_23;
			case KEYWORD_ixknown:	// "known"
				addTerminal(node);
				nextToken();
				goto L_1;
			case KEYWORD_ixset:	// "set"
				addTerminal(node);
				nextToken();
				goto L_4;
			case KEYWORD_ixgroups:	// "groups"
				addTerminal(node);
				nextToken();
				goto L_7;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			// begin rule known_item
			case KEYWORD_ixmodes:	// "modes"
			// end rule known_item
				addNonterminal(node, parse_ixRULE_known_item());
				goto L_2;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_2:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_4:
		switch(getToken())
		{
			// begin rule set_item
			case KEYWORD_ixmodes:	// "modes"
			// end rule set_item
				addNonterminal(node, parse_ixRULE_set_item());
				goto L_5;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_5:
		switch(getToken())
		{
			case ';':
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
			default:
				return node;	// accept state
				break;
		}
		L_7:
		switch(getToken())
		{
			case KEYWORD_ixbegin:	// "begin"
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
			// begin rule group_def
			case KEYWORD_ixgroup:	// "group"
			// end rule group_def
				addNonterminal(node, parse_ixRULE_group_def());
				goto L_9;
			case KEYWORD_ixend:	// "end"
				addTerminal(node);
				nextToken();
				goto L_10;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_9:
		switch(getToken())
		{
			// begin rule group_def
			case KEYWORD_ixgroup:	// "group"
			// end rule group_def
				addNonterminal(node, parse_ixRULE_group_def());
				goto L_9;
			case KEYWORD_ixend:	// "end"
				addTerminal(node);
				nextToken();
				goto L_10;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_10:
		switch(getToken())
		{
			case KEYWORD_ixgroups:	// "groups"
				addTerminal(node);
				nextToken();
				goto L_11;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_11:
		switch(getToken())
		{
			case ';':
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
			default:
				return node;	// accept state
				break;
		}
		L_13:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
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
			case '(':
				addTerminal(node);
				nextToken();
				goto L_15;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_15:
		switch(getToken())
		{
			// begin rule insn_arg_list
			case TOKEN_ixIDENT:
			// end rule insn_arg_list
				addNonterminal(node, parse_ixRULE_insn_arg_list());
				goto L_16;
			case ')':
				addTerminal(node);
				nextToken();
				goto L_17;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_16:
		switch(getToken())
		{
			case ')':
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
			case KEYWORD_ixbegin:	// "begin"
				addTerminal(node);
				nextToken();
				goto L_18;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_18:
		switch(getToken())
		{
			case KEYWORD_ixend:	// "end"
				addTerminal(node);
				nextToken();
				goto L_20;
			// begin rule insn_body
			case KEYWORD_ixemu:	// "emu"
			case KEYWORD_ixgroup:	// "group"
			case KEYWORD_ixflags:	// "flags"
			case KEYWORD_ixsynopsis:	// "synopsis"
			case KEYWORD_ixdesc:	// "desc"
			case KEYWORD_ixvalid:	// "valid"
			case KEYWORD_ixverified:	// "verified"
			case KEYWORD_ixcode:	// "code"
			case KEYWORD_ixnotes:	// "notes"
			case KEYWORD_ixdis:	// "dis"
			case KEYWORD_ixasm:	// "asm"
			// end rule insn_body
				addNonterminal(node, parse_ixRULE_insn_body());
				goto L_19;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_19:
		switch(getToken())
		{
			case KEYWORD_ixend:	// "end"
				addTerminal(node);
				nextToken();
				goto L_20;
			// begin rule insn_body
			case KEYWORD_ixemu:	// "emu"
			case KEYWORD_ixgroup:	// "group"
			case KEYWORD_ixflags:	// "flags"
			case KEYWORD_ixsynopsis:	// "synopsis"
			case KEYWORD_ixdesc:	// "desc"
			case KEYWORD_ixvalid:	// "valid"
			case KEYWORD_ixverified:	// "verified"
			case KEYWORD_ixcode:	// "code"
			case KEYWORD_ixnotes:	// "notes"
			case KEYWORD_ixdis:	// "dis"
			case KEYWORD_ixasm:	// "asm"
			// end rule insn_body
				addNonterminal(node, parse_ixRULE_insn_body());
				goto L_19;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_20:
		switch(getToken())
		{
			case KEYWORD_ixinsn:	// "insn"
				addTerminal(node);
				nextToken();
				goto L_21;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_21:
		switch(getToken())
		{
			case ';':
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
			default:
				return node;	// accept state
				break;
		}
		L_23:
		switch(getToken())
		{
			case KEYWORD_ixfunction:	// "function"
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
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_25;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_25:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_26;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_26:
		switch(getToken())
		{
			case '(':
				addTerminal(node);
				nextToken();
				goto L_27;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_27:
		switch(getToken())
		{
			// begin rule insn_arg_list
			case TOKEN_ixIDENT:
			// end rule insn_arg_list
				addNonterminal(node, parse_ixRULE_insn_arg_list());
				goto L_28;
			case ')':
				addTerminal(node);
				nextToken();
				goto L_29;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_28:
		switch(getToken())
		{
			case ')':
				addTerminal(node);
				nextToken();
				goto L_29;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_29:
		switch(getToken())
		{
			// begin rule emu_body
			case KEYWORD_ixbegin:	// "begin"
			// end rule emu_body
				addNonterminal(node, parse_ixRULE_emu_body());
				goto L_30;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_30:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_31;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_31:
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

ParseNode *parse_ixRULE_ascii()
{
	ParseNode *node = new ParseNode(RULE_ixascii, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_1;
			case TOKEN_ixLITNUM:
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

ParseNode *parse_ixRULE_argtype()
{
	ParseNode *node = new ParseNode(RULE_ixargtype, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule argbase
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule argbase
				addNonterminal(node, parse_ixRULE_argbase());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case '&':
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule argbase
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule argbase
				addNonterminal(node, parse_ixRULE_argbase());
				goto L_3;
			default:
				throw ParseSyntaxError();
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

ParseNode *parse_ixRULE_argbase()
{
	ParseNode *node = new ParseNode(RULE_ixargbase, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule ascii
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule ascii
				addNonterminal(node, parse_ixRULE_ascii());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case '/':
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule ascii
			case TOKEN_ixIDENT:
			case TOKEN_ixLITNUM:
			// end rule ascii
				addNonterminal(node, parse_ixRULE_ascii());
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case '/':
				addTerminal(node);
				nextToken();
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

ParseNode *parse_ixRULE_remarks()
{
	ParseNode *node = new ParseNode(RULE_ixremarks, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case TOKEN_ixSQUARE:
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case TOKEN_ixSQUARE:
				addTerminal(node);
				nextToken();
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			case TOKEN_ixSQUARE:
				addTerminal(node);
				nextToken();
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

ParseNode *parse_ixRULE_known_item()
{
	ParseNode *node = new ParseNode(RULE_ixknown_item, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixmodes:	// "modes"
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case '=':
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
			case '(':
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case ')':
				addTerminal(node);
				nextToken();
				goto L_5;
			// begin rule ident_list
			case TOKEN_ixIDENT:
			// end rule ident_list
				addNonterminal(node, parse_ixRULE_ident_list());
				goto L_4;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_4:
		switch(getToken())
		{
			case ')':
				addTerminal(node);
				nextToken();
				goto L_5;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_5:
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

ParseNode *parse_ixRULE_set_item()
{
	ParseNode *node = new ParseNode(RULE_ixset_item, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixmodes:	// "modes"
				addTerminal(node);
				nextToken();
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
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
			case '=':
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case '(':
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
			case ')':
				addTerminal(node);
				nextToken();
				goto L_6;
			// begin rule ident_list
			case TOKEN_ixIDENT:
			// end rule ident_list
				addNonterminal(node, parse_ixRULE_ident_list());
				goto L_5;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_5:
		switch(getToken())
		{
			case ')':
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



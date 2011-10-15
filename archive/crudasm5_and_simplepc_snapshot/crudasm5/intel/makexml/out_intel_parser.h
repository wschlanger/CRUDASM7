// out_intel_parser.h - automatically generated file (do not edit!)
// Copyright (C) 2009-2011 Willow Schlanger

ParseNode *parse_ixRULE_start()
{
	ParseNode *node = new ParseNode(RULE_ixstart, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule declaration_seq
			case KEYWORD_ixinsn:	// "insn"
			// end rule declaration_seq
				addNonterminal(node, parse_ixRULE_declaration_seq());
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

ParseNode *parse_ixRULE_declaration_seq()
{
	ParseNode *node = new ParseNode(RULE_ixdeclaration_seq, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule declaration
			case KEYWORD_ixinsn:	// "insn"
			// end rule declaration
				addNonterminal(node, parse_ixRULE_declaration());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			// begin rule declaration
			case KEYWORD_ixinsn:	// "insn"
			// end rule declaration
				addNonterminal(node, parse_ixRULE_declaration());
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule declaration
			case KEYWORD_ixinsn:	// "insn"
			// end rule declaration
				addNonterminal(node, parse_ixRULE_declaration());
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

ParseNode *parse_ixRULE_declaration()
{
	ParseNode *node = new ParseNode(RULE_ixdeclaration, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixinsn:	// "insn"
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
			case TOKEN_ixLITSTRING:
				addTerminal(node);
				nextToken();
				goto L_3;
			// begin rule flags
			case KEYWORD_ixlockable:	// "lockable"
			case KEYWORD_ixlock_always:	// "lock_always"
			case KEYWORD_ixrepable:	// "repable"
			case KEYWORD_ixrepcond:	// "repcond"
			case KEYWORD_ixctrlxfer:	// "ctrlxfer"
			case KEYWORD_ixfwaitable:	// "fwaitable"
			// end rule flags
				addNonterminal(node, parse_ixRULE_flags());
				goto L_2;
			// begin rule prototype
			case TOKEN_ixIDENT:
			// end rule prototype
				addNonterminal(node, parse_ixRULE_prototype());
				goto L_4;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_2:
		switch(getToken())
		{
			case TOKEN_ixLITSTRING:
				addTerminal(node);
				nextToken();
				goto L_3;
			// begin rule prototype
			case TOKEN_ixIDENT:
			// end rule prototype
				addNonterminal(node, parse_ixRULE_prototype());
				goto L_4;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			// begin rule prototype
			case TOKEN_ixIDENT:
			// end rule prototype
				addNonterminal(node, parse_ixRULE_prototype());
				goto L_4;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_4:
		switch(getToken())
		{
			// begin rule machine
			case TOKEN_ixLITSTRING:
			// end rule machine
				addNonterminal(node, parse_ixRULE_machine());
				goto L_5;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_5:
		switch(getToken())
		{
			// begin rule special
			case KEYWORD_ixemu:	// "emu"
			case KEYWORD_ixdis:	// "dis"
			// end rule special
				addNonterminal(node, parse_ixRULE_special());
				goto L_6;
			default:
				return node;	// accept state
				break;
		}
		L_6:
		switch(getToken())
		{
			// begin rule special
			case KEYWORD_ixemu:	// "emu"
			case KEYWORD_ixdis:	// "dis"
			// end rule special
				addNonterminal(node, parse_ixRULE_special());
				goto L_6;
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

ParseNode *parse_ixRULE_flags()
{
	ParseNode *node = new ParseNode(RULE_ixflags, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule flag
			case KEYWORD_ixlockable:	// "lockable"
			case KEYWORD_ixlock_always:	// "lock_always"
			case KEYWORD_ixrepable:	// "repable"
			case KEYWORD_ixrepcond:	// "repcond"
			case KEYWORD_ixctrlxfer:	// "ctrlxfer"
			case KEYWORD_ixfwaitable:	// "fwaitable"
			// end rule flag
				addNonterminal(node, parse_ixRULE_flag());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			// begin rule flag
			case KEYWORD_ixlockable:	// "lockable"
			case KEYWORD_ixlock_always:	// "lock_always"
			case KEYWORD_ixrepable:	// "repable"
			case KEYWORD_ixrepcond:	// "repcond"
			case KEYWORD_ixctrlxfer:	// "ctrlxfer"
			case KEYWORD_ixfwaitable:	// "fwaitable"
			// end rule flag
				addNonterminal(node, parse_ixRULE_flag());
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule flag
			case KEYWORD_ixlockable:	// "lockable"
			case KEYWORD_ixlock_always:	// "lock_always"
			case KEYWORD_ixrepable:	// "repable"
			case KEYWORD_ixrepcond:	// "repcond"
			case KEYWORD_ixctrlxfer:	// "ctrlxfer"
			case KEYWORD_ixfwaitable:	// "fwaitable"
			// end rule flag
				addNonterminal(node, parse_ixRULE_flag());
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

ParseNode *parse_ixRULE_prototype()
{
	ParseNode *node = new ParseNode(RULE_ixprototype, getLine(), getFile());
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
			// begin rule args
			case TOKEN_ixIDENT:
			// end rule args
				addNonterminal(node, parse_ixRULE_args());
				goto L_3;
			case ')':
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

ParseNode *parse_ixRULE_machine()
{
	ParseNode *node = new ParseNode(RULE_ixmachine, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule mach
			case TOKEN_ixLITSTRING:
			// end rule mach
				addNonterminal(node, parse_ixRULE_mach());
				goto L_1;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			// begin rule mach
			case TOKEN_ixLITSTRING:
			// end rule mach
				addNonterminal(node, parse_ixRULE_mach());
				goto L_2;
			default:
				return node;	// accept state
				break;
		}
		L_2:
		switch(getToken())
		{
			// begin rule mach
			case TOKEN_ixLITSTRING:
			// end rule mach
				addNonterminal(node, parse_ixRULE_mach());
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

ParseNode *parse_ixRULE_special()
{
	ParseNode *node = new ParseNode(RULE_ixspecial, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixemu:	// "emu"
				addTerminal(node);
				nextToken();
				goto L_1;
			case KEYWORD_ixdis:	// "dis"
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
			case '{':
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
			// begin rule emu_stmt
			case TOKEN_ixIDENT:
			case KEYWORD_ixo16:	// "o16"
			case KEYWORD_ixo32:	// "o32"
			case KEYWORD_ixo64:	// "o64"
			case KEYWORD_ixa16:	// "a16"
			case KEYWORD_ixa32:	// "a32"
			case KEYWORD_ixa64:	// "a64"
			case KEYWORD_ixs16:	// "s16"
			case KEYWORD_ixs32:	// "s32"
			case KEYWORD_ixs64:	// "s64"
			case KEYWORD_ixassert:	// "assert"
			case KEYWORD_ixpush:	// "push"
			case KEYWORD_ixpop:	// "pop"
			case KEYWORD_ixreserve:	// "reserve"
			case KEYWORD_ixdiscard:	// "discard"
			case KEYWORD_ixoutport:	// "outport"
			case KEYWORD_ixinport:	// "inport"
			// end rule emu_stmt
				addNonterminal(node, parse_ixRULE_emu_stmt());
				goto L_3;
			case '}':
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
			// begin rule emu_stmt
			case TOKEN_ixIDENT:
			case KEYWORD_ixo16:	// "o16"
			case KEYWORD_ixo32:	// "o32"
			case KEYWORD_ixo64:	// "o64"
			case KEYWORD_ixa16:	// "a16"
			case KEYWORD_ixa32:	// "a32"
			case KEYWORD_ixa64:	// "a64"
			case KEYWORD_ixs16:	// "s16"
			case KEYWORD_ixs32:	// "s32"
			case KEYWORD_ixs64:	// "s64"
			case KEYWORD_ixassert:	// "assert"
			case KEYWORD_ixpush:	// "push"
			case KEYWORD_ixpop:	// "pop"
			case KEYWORD_ixreserve:	// "reserve"
			case KEYWORD_ixdiscard:	// "discard"
			case KEYWORD_ixoutport:	// "outport"
			case KEYWORD_ixinport:	// "inport"
			// end rule emu_stmt
				addNonterminal(node, parse_ixRULE_emu_stmt());
				goto L_3;
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
			// begin rule asmtype
			case KEYWORD_ixall:	// "all"
			case KEYWORD_ixnasm:	// "nasm"
			case KEYWORD_ixmasm:	// "masm"
			case KEYWORD_ixas:	// "as"
			// end rule asmtype
				addNonterminal(node, parse_ixRULE_asmtype());
				goto L_6;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_6:
		switch(getToken())
		{
			case '{':
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
			case '}':
				addTerminal(node);
				nextToken();
				goto L_9;
			// begin rule dis_stmt
			case TOKEN_ixIDENT:
			// end rule dis_stmt
				addNonterminal(node, parse_ixRULE_dis_stmt());
				goto L_8;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_8:
		switch(getToken())
		{
			case '}':
				addTerminal(node);
				nextToken();
				goto L_9;
			// begin rule dis_stmt
			case TOKEN_ixIDENT:
			// end rule dis_stmt
				addNonterminal(node, parse_ixRULE_dis_stmt());
				goto L_8;
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

ParseNode *parse_ixRULE_flag()
{
	ParseNode *node = new ParseNode(RULE_ixflag, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixlockable:	// "lockable"
				addTerminal(node);
				nextToken();
				goto L_1;
			case KEYWORD_ixlock_always:	// "lock_always"
				addTerminal(node);
				nextToken();
				goto L_2;
			case KEYWORD_ixrepable:	// "repable"
				addTerminal(node);
				nextToken();
				goto L_3;
			case KEYWORD_ixrepcond:	// "repcond"
				addTerminal(node);
				nextToken();
				goto L_4;
			case KEYWORD_ixctrlxfer:	// "ctrlxfer"
				addTerminal(node);
				nextToken();
				goto L_5;
			case KEYWORD_ixfwaitable:	// "fwaitable"
				addTerminal(node);
				nextToken();
				goto L_6;
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

ParseNode *parse_ixRULE_mach()
{
	ParseNode *node = new ParseNode(RULE_ixmach, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case TOKEN_ixLITSTRING:
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
			// begin rule mach_suffix
			case '!':
			// end rule mach_suffix
				addNonterminal(node, parse_ixRULE_mach_suffix());
				goto L_2;
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

ParseNode *parse_ixRULE_mach_suffix()
{
	ParseNode *node = new ParseNode(RULE_ixmach_suffix, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case '!':
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
			// begin rule mach_tag
			case TOKEN_ixIDENT:
			case KEYWORD_ixasm_skip:	// "asm_skip"
			case KEYWORD_ixsx_byte:	// "sx_byte"
			case KEYWORD_iximm64_sx32:	// "imm64_sx32"
			case KEYWORD_ixno_rex_w:	// "no_rex_w"
			case KEYWORD_iximm64_disp:	// "imm64_disp"
			case KEYWORD_ixis64:	// "is64"
			case KEYWORD_ixno64:	// "no64"
			case KEYWORD_ixlike_movsxd:	// "like_movsxd"
			case KEYWORD_ixlike_arpl:	// "like_arpl"
			case KEYWORD_ixreg_base:	// "reg_base"
			case KEYWORD_ixhas_cc:	// "has_cc"
			case KEYWORD_ixrelative:	// "relative"
			case KEYWORD_ixreg_rm:	// "reg_rm"
			// end rule mach_tag
				addNonterminal(node, parse_ixRULE_mach_tag());
				goto L_2;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_2:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_3;
			default:
				return node;	// accept state
				break;
		}
		L_3:
		switch(getToken())
		{
			// begin rule mach_tag
			case TOKEN_ixIDENT:
			case KEYWORD_ixasm_skip:	// "asm_skip"
			case KEYWORD_ixsx_byte:	// "sx_byte"
			case KEYWORD_iximm64_sx32:	// "imm64_sx32"
			case KEYWORD_ixno_rex_w:	// "no_rex_w"
			case KEYWORD_iximm64_disp:	// "imm64_disp"
			case KEYWORD_ixis64:	// "is64"
			case KEYWORD_ixno64:	// "no64"
			case KEYWORD_ixlike_movsxd:	// "like_movsxd"
			case KEYWORD_ixlike_arpl:	// "like_arpl"
			case KEYWORD_ixreg_base:	// "reg_base"
			case KEYWORD_ixhas_cc:	// "has_cc"
			case KEYWORD_ixrelative:	// "relative"
			case KEYWORD_ixreg_rm:	// "reg_rm"
			// end rule mach_tag
				addNonterminal(node, parse_ixRULE_mach_tag());
				goto L_4;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_4:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_3;
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

ParseNode *parse_ixRULE_mach_tag()
{
	ParseNode *node = new ParseNode(RULE_ixmach_tag, getLine(), getFile());
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
			case KEYWORD_ixasm_skip:	// "asm_skip"
				addTerminal(node);
				nextToken();
				goto L_4;
			case KEYWORD_ixsx_byte:	// "sx_byte"
				addTerminal(node);
				nextToken();
				goto L_5;
			case KEYWORD_iximm64_sx32:	// "imm64_sx32"
				addTerminal(node);
				nextToken();
				goto L_6;
			case KEYWORD_ixno_rex_w:	// "no_rex_w"
				addTerminal(node);
				nextToken();
				goto L_7;
			case KEYWORD_iximm64_disp:	// "imm64_disp"
				addTerminal(node);
				nextToken();
				goto L_8;
			case KEYWORD_ixis64:	// "is64"
				addTerminal(node);
				nextToken();
				goto L_9;
			case KEYWORD_ixno64:	// "no64"
				addTerminal(node);
				nextToken();
				goto L_10;
			case KEYWORD_ixlike_movsxd:	// "like_movsxd"
				addTerminal(node);
				nextToken();
				goto L_11;
			case KEYWORD_ixlike_arpl:	// "like_arpl"
				addTerminal(node);
				nextToken();
				goto L_12;
			case KEYWORD_ixreg_base:	// "reg_base"
				addTerminal(node);
				nextToken();
				goto L_13;
			case KEYWORD_ixhas_cc:	// "has_cc"
				addTerminal(node);
				nextToken();
				goto L_14;
			case KEYWORD_ixrelative:	// "relative"
				addTerminal(node);
				nextToken();
				goto L_15;
			case KEYWORD_ixreg_rm:	// "reg_rm"
				addTerminal(node);
				nextToken();
				goto L_16;
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
			default:
				return node;	// accept state
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
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_2;
			// begin rule prefix2
			case KEYWORD_ixo16:	// "o16"
			case KEYWORD_ixo32:	// "o32"
			case KEYWORD_ixo64:	// "o64"
			case KEYWORD_ixa16:	// "a16"
			case KEYWORD_ixa32:	// "a32"
			case KEYWORD_ixa64:	// "a64"
			case KEYWORD_ixs16:	// "s16"
			case KEYWORD_ixs32:	// "s32"
			case KEYWORD_ixs64:	// "s64"
			// end rule prefix2
				addNonterminal(node, parse_ixRULE_prefix2());
				goto L_1;
			case KEYWORD_ixassert:	// "assert"
				addTerminal(node);
				nextToken();
				goto L_11;
			case KEYWORD_ixpush:	// "push"
				addTerminal(node);
				nextToken();
				goto L_16;
			case KEYWORD_ixpop:	// "pop"
				addTerminal(node);
				nextToken();
				goto L_21;
			case KEYWORD_ixreserve:	// "reserve"
				addTerminal(node);
				nextToken();
				goto L_26;
			case KEYWORD_ixdiscard:	// "discard"
				addTerminal(node);
				nextToken();
				goto L_31;
			case KEYWORD_ixoutport:	// "outport"
				addTerminal(node);
				nextToken();
				goto L_36;
			case KEYWORD_ixinport:	// "inport"
				addTerminal(node);
				nextToken();
				goto L_43;
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
			// begin rule prefix2
			case KEYWORD_ixo16:	// "o16"
			case KEYWORD_ixo32:	// "o32"
			case KEYWORD_ixo64:	// "o64"
			case KEYWORD_ixa16:	// "a16"
			case KEYWORD_ixa32:	// "a32"
			case KEYWORD_ixa64:	// "a64"
			case KEYWORD_ixs16:	// "s16"
			case KEYWORD_ixs32:	// "s32"
			case KEYWORD_ixs64:	// "s64"
			// end rule prefix2
				addNonterminal(node, parse_ixRULE_prefix2());
				goto L_1;
			case KEYWORD_ixassert:	// "assert"
				addTerminal(node);
				nextToken();
				goto L_11;
			case KEYWORD_ixpush:	// "push"
				addTerminal(node);
				nextToken();
				goto L_16;
			case KEYWORD_ixpop:	// "pop"
				addTerminal(node);
				nextToken();
				goto L_21;
			case KEYWORD_ixreserve:	// "reserve"
				addTerminal(node);
				nextToken();
				goto L_26;
			case KEYWORD_ixdiscard:	// "discard"
				addTerminal(node);
				nextToken();
				goto L_31;
			case KEYWORD_ixoutport:	// "outport"
				addTerminal(node);
				nextToken();
				goto L_36;
			case KEYWORD_ixinport:	// "inport"
				addTerminal(node);
				nextToken();
				goto L_43;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_2:
		switch(getToken())
		{
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_3;
			case '=':
				addTerminal(node);
				nextToken();
				goto L_8;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_3:
		switch(getToken())
		{
			case '=':
				addTerminal(node);
				nextToken();
				goto L_5;
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
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_6;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_6:
		switch(getToken())
		{
			case ';':
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
		L_8:
		switch(getToken())
		{
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
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
			case '(':
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
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
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
			case ')':
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
			case ';':
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
			default:
				return node;	// accept state
				break;
		}
		L_16:
		switch(getToken())
		{
			case '(':
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
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_18;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_18:
		switch(getToken())
		{
			case ')':
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
			case '(':
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
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_23;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_23:
		switch(getToken())
		{
			case ')':
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
			case ';':
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
			default:
				return node;	// accept state
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
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_28;
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
			case ';':
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
			default:
				return node;	// accept state
				break;
		}
		L_31:
		switch(getToken())
		{
			case '(':
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
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_33;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_33:
		switch(getToken())
		{
			case ')':
				addTerminal(node);
				nextToken();
				goto L_34;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_34:
		switch(getToken())
		{
			case ';':
				addTerminal(node);
				nextToken();
				goto L_35;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_35:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_36:
		switch(getToken())
		{
			case '(':
				addTerminal(node);
				nextToken();
				goto L_37;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_37:
		switch(getToken())
		{
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_38;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_38:
		switch(getToken())
		{
			case ',':
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
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_40;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_40:
		switch(getToken())
		{
			case ')':
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
			case ';':
				addTerminal(node);
				nextToken();
				goto L_42;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_42:
		switch(getToken())
		{
			default:
				return node;	// accept state
				break;
		}
		L_43:
		switch(getToken())
		{
			case '(':
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
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_45;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_45:
		switch(getToken())
		{
			case ',':
				addTerminal(node);
				nextToken();
				goto L_46;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_46:
		switch(getToken())
		{
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_47;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_47:
		switch(getToken())
		{
			case ')':
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
			case ';':
				addTerminal(node);
				nextToken();
				goto L_49;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_49:
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

ParseNode *parse_ixRULE_asmtype()
{
	ParseNode *node = new ParseNode(RULE_ixasmtype, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixall:	// "all"
				addTerminal(node);
				nextToken();
				goto L_1;
			case KEYWORD_ixnasm:	// "nasm"
				addTerminal(node);
				nextToken();
				goto L_2;
			case KEYWORD_ixmasm:	// "masm"
				addTerminal(node);
				nextToken();
				goto L_3;
			case KEYWORD_ixas:	// "as"
				addTerminal(node);
				nextToken();
				goto L_4;
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
			// begin rule dis_args
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case TOKEN_ixLITSTRING:
			// end rule dis_args
				addNonterminal(node, parse_ixRULE_dis_args());
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
	}
	catch(ParseSyntaxError &e)
	{
		delete node;
		throw e;
	}
	return NULL;
}

ParseNode *parse_ixRULE_args()
{
	ParseNode *node = new ParseNode(RULE_ixargs, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule arg
			case TOKEN_ixIDENT:
			// end rule arg
				addNonterminal(node, parse_ixRULE_arg());
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
			// begin rule arg
			case TOKEN_ixIDENT:
			// end rule arg
				addNonterminal(node, parse_ixRULE_arg());
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

ParseNode *parse_ixRULE_arg()
{
	ParseNode *node = new ParseNode(RULE_ixarg, getLine(), getFile());
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

ParseNode *parse_ixRULE_dis_arg()
{
	ParseNode *node = new ParseNode(RULE_ixdis_arg, getLine(), getFile());
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
			case TOKEN_ixNUM:
				addTerminal(node);
				nextToken();
				goto L_2;
			case TOKEN_ixLITSTRING:
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

ParseNode *parse_ixRULE_dis_args()
{
	ParseNode *node = new ParseNode(RULE_ixdis_args, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule dis_arg
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case TOKEN_ixLITSTRING:
			// end rule dis_arg
				addNonterminal(node, parse_ixRULE_dis_arg());
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
			// begin rule dis_arg
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case TOKEN_ixLITSTRING:
			// end rule dis_arg
				addNonterminal(node, parse_ixRULE_dis_arg());
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

ParseNode *parse_ixRULE_prefix2()
{
	ParseNode *node = new ParseNode(RULE_ixprefix2, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			case KEYWORD_ixo16:	// "o16"
				addTerminal(node);
				nextToken();
				goto L_1;
			case KEYWORD_ixo32:	// "o32"
				addTerminal(node);
				nextToken();
				goto L_3;
			case KEYWORD_ixo64:	// "o64"
				addTerminal(node);
				nextToken();
				goto L_5;
			case KEYWORD_ixa16:	// "a16"
				addTerminal(node);
				nextToken();
				goto L_7;
			case KEYWORD_ixa32:	// "a32"
				addTerminal(node);
				nextToken();
				goto L_9;
			case KEYWORD_ixa64:	// "a64"
				addTerminal(node);
				nextToken();
				goto L_11;
			case KEYWORD_ixs16:	// "s16"
				addTerminal(node);
				nextToken();
				goto L_13;
			case KEYWORD_ixs32:	// "s32"
				addTerminal(node);
				nextToken();
				goto L_15;
			case KEYWORD_ixs64:	// "s64"
				addTerminal(node);
				nextToken();
				goto L_17;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_1:
		switch(getToken())
		{
			case ':':
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
			default:
				return node;	// accept state
				break;
		}
		L_5:
		switch(getToken())
		{
			case ':':
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
			case ':':
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
			case ':':
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
			default:
				return node;	// accept state
				break;
		}
		L_11:
		switch(getToken())
		{
			case ':':
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
			case ':':
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
			case ':':
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
			case ':':
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
			case TOKEN_ixIDENT:
				addTerminal(node);
				nextToken();
				goto L_1;
			case TOKEN_ixNUM:
				addTerminal(node);
				nextToken();
				goto L_30;
			case KEYWORD_ixsx:	// "sx"
				addTerminal(node);
				nextToken();
				goto L_5;
			case KEYWORD_ixzx:	// "zx"
				addTerminal(node);
				nextToken();
				goto L_12;
			case KEYWORD_ixtr:	// "tr"
				addTerminal(node);
				nextToken();
				goto L_19;
			case KEYWORD_ixsizeof:	// "sizeof"
				addTerminal(node);
				nextToken();
				goto L_26;
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
			// begin rule list
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule list
				addNonterminal(node, parse_ixRULE_list());
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
		L_5:
		switch(getToken())
		{
			case '<':
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
			case '>':
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
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
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
			default:
				return node;	// accept state
				break;
		}
		L_12:
		switch(getToken())
		{
			case '<':
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
			case '>':
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
			case '(':
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
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_17;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_17:
		switch(getToken())
		{
			case ')':
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
			default:
				return node;	// accept state
				break;
		}
		L_19:
		switch(getToken())
		{
			case '<':
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
			case TOKEN_ixIDENT:
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
			case '>':
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
			case '(':
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
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
			// end rule rhs
				addNonterminal(node, parse_ixRULE_rhs());
				goto L_24;
			default:
				throw ParseSyntaxError();
				break;
		}
		L_24:
		switch(getToken())
		{
			case ')':
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
			default:
				return node;	// accept state
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
			case TOKEN_ixIDENT:
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
			default:
				return node;	// accept state
				break;
		}
		L_30:
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

ParseNode *parse_ixRULE_list()
{
	ParseNode *node = new ParseNode(RULE_ixlist, getLine(), getFile());
	try
	{
		goto L_0;
		L_0:
		switch(getToken())
		{
			// begin rule rhs
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
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
			case TOKEN_ixIDENT:
			case TOKEN_ixNUM:
			case KEYWORD_ixsx:	// "sx"
			case KEYWORD_ixzx:	// "zx"
			case KEYWORD_ixtr:	// "tr"
			case KEYWORD_ixsizeof:	// "sizeof"
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



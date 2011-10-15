// out_intel_lexer.h - automatically generated file (do not edit!)
// Copyright (C) 2009,2011 Willow Schlanger

virtual void initManualKeywords()
{
	addManualKeyword("begin", KEYWORD_ixbegin);
	addManualKeyword("code", KEYWORD_ixcode);
	addManualKeyword("desc", KEYWORD_ixdesc);
	addManualKeyword("dis", KEYWORD_ixdis);
	addManualKeyword("emu", KEYWORD_ixemu);
	addManualKeyword("end", KEYWORD_ixend);
	addManualKeyword("except_assert", KEYWORD_ixexcept_assert);
	addManualKeyword("flags", KEYWORD_ixflags);
	addManualKeyword("function", KEYWORD_ixfunction);
	addManualKeyword("group", KEYWORD_ixgroup);
	addManualKeyword("groups", KEYWORD_ixgroups);
	addManualKeyword("if", KEYWORD_ixif);
	addManualKeyword("insn", KEYWORD_ixinsn);
	addManualKeyword("known", KEYWORD_ixknown);
	addManualKeyword("mem_only", KEYWORD_ixmem_only);
	addManualKeyword("mod", KEYWORD_ixmod);
	addManualKeyword("modes", KEYWORD_ixmodes);
	addManualKeyword("notes", KEYWORD_ixnotes);
	addManualKeyword("r", KEYWORD_ixr);
	addManualKeyword("return", KEYWORD_ixreturn);
	addManualKeyword("set", KEYWORD_ixset);
	addManualKeyword("signed", KEYWORD_ixsigned);
	addManualKeyword("synopsis", KEYWORD_ixsynopsis);
	addManualKeyword("times", KEYWORD_ixtimes);
	addManualKeyword("undefined", KEYWORD_ixundefined);
	addManualKeyword("unsigned", KEYWORD_ixunsigned);
	addManualKeyword("valid", KEYWORD_ixvalid);
	addManualKeyword("verified", KEYWORD_ixverified);
}

unsigned lexNextToken()
{
	goto L0;
	L0:
	switch(getChar())
	{
		case '!':
			nextChar();
			goto L1;
		case '%':
			nextChar();
			goto L3;
		case '&':
			nextChar();
			goto L4;
		case '(':
			nextChar();
			goto L6;
		case ')':
			nextChar();
			goto L7;
		case '*':
			nextChar();
			goto L8;
		case '+':
			nextChar();
			goto L9;
		case ',':
			nextChar();
			goto L10;
		case '-':
			nextChar();
			goto L11;
		case '/':
			nextChar();
			goto L12;
		case ':':
			nextChar();
			goto L13;
		case ';':
			nextChar();
			goto L14;
		case '<':
			nextChar();
			goto L15;
		case '=':
			nextChar();
			goto L18;
		case '>':
			nextChar();
			goto L20;
		case '?':
			nextChar();
			goto L23;
		case '^':
			nextChar();
			goto L24;
		case '|':
			nextChar();
			goto L25;
		case '~':
			nextChar();
			goto L27;
		default:
			return defaultHandler(0x80000000 + getChar());	//start state
			break;
	}
	L1:
	switch(getChar())
	{
		case '=':
			nextChar();
			goto L2;
		default:
			return '!';	// accept state
			break;
	}
	L2:
	switch(getChar())
	{
		default:
			return KEYWORD_ixNE;	// accept state
			break;
	}
	L3:
	switch(getChar())
	{
		default:
			return '%';	// accept state
			break;
	}
	L4:
	switch(getChar())
	{
		case '&':
			nextChar();
			goto L5;
		default:
			return '&';	// accept state
			break;
	}
	L5:
	switch(getChar())
	{
		default:
			return KEYWORD_ixANDAND;	// accept state
			break;
	}
	L6:
	switch(getChar())
	{
		default:
			return '(';	// accept state
			break;
	}
	L7:
	switch(getChar())
	{
		default:
			return ')';	// accept state
			break;
	}
	L8:
	switch(getChar())
	{
		default:
			return '*';	// accept state
			break;
	}
	L9:
	switch(getChar())
	{
		default:
			return '+';	// accept state
			break;
	}
	L10:
	switch(getChar())
	{
		default:
			return ',';	// accept state
			break;
	}
	L11:
	switch(getChar())
	{
		default:
			return '-';	// accept state
			break;
	}
	L12:
	switch(getChar())
	{
		default:
			return '/';	// accept state
			break;
	}
	L13:
	switch(getChar())
	{
		default:
			return ':';	// accept state
			break;
	}
	L14:
	switch(getChar())
	{
		default:
			return ';';	// accept state
			break;
	}
	L15:
	switch(getChar())
	{
		case '<':
			nextChar();
			goto L16;
		case '=':
			nextChar();
			goto L17;
		default:
			return '<';	// accept state
			break;
	}
	L16:
	switch(getChar())
	{
		default:
			return KEYWORD_ixSHL;	// accept state
			break;
	}
	L17:
	switch(getChar())
	{
		default:
			return KEYWORD_ixLE;	// accept state
			break;
	}
	L18:
	switch(getChar())
	{
		case '=':
			nextChar();
			goto L19;
		default:
			return '=';	// accept state
			break;
	}
	L19:
	switch(getChar())
	{
		default:
			return KEYWORD_ixEQ;	// accept state
			break;
	}
	L20:
	switch(getChar())
	{
		case '=':
			nextChar();
			goto L21;
		case '>':
			nextChar();
			goto L22;
		default:
			return '>';	// accept state
			break;
	}
	L21:
	switch(getChar())
	{
		default:
			return KEYWORD_ixGE;	// accept state
			break;
	}
	L22:
	switch(getChar())
	{
		default:
			return KEYWORD_ixSHR;	// accept state
			break;
	}
	L23:
	switch(getChar())
	{
		default:
			return '?';	// accept state
			break;
	}
	L24:
	switch(getChar())
	{
		default:
			return '^';	// accept state
			break;
	}
	L25:
	switch(getChar())
	{
		case '|':
			nextChar();
			goto L26;
		default:
			return '|';	// accept state
			break;
	}
	L26:
	switch(getChar())
	{
		default:
			return KEYWORD_ixOROR;	// accept state
			break;
	}
	L27:
	switch(getChar())
	{
		default:
			return '~';	// accept state
			break;
	}
	return 0;
}


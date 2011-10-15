// out_rulecom_lexer.h - automatically generated file (do not edit!)
// Copyright (C) 2009 Willow Schlanger

virtual void initManualKeywords()
{
	addManualKeyword("copyright", KEYWORD_rccopyright);
	addManualKeyword("keyword", KEYWORD_rckeyword);
	addManualKeyword("manual", KEYWORD_rcmanual);
	addManualKeyword("prefix", KEYWORD_rcprefix);
	addManualKeyword("start", KEYWORD_rcstart);
	addManualKeyword("token", KEYWORD_rctoken);
}

unsigned lexNextToken()
{
	goto L0;
	L0:
	switch(getChar())
	{
		case ':':
			nextChar();
			goto L1;
		case ';':
			nextChar();
			goto L4;
		case '[':
			nextChar();
			goto L5;
		case ']':
			nextChar();
			goto L6;
		case '{':
			nextChar();
			goto L7;
		case '|':
			nextChar();
			goto L8;
		case '}':
			nextChar();
			goto L9;
		default:
			return defaultHandler(0x80000000 + getChar());	//start state
			break;
	}
	L1:
	switch(getChar())
	{
		case ':':
			nextChar();
			goto L2;
		default:
			return 0x80000000 + getChar();	// unexpected character
			break;
	}
	L2:
	switch(getChar())
	{
		case '=':
			nextChar();
			goto L3;
		default:
			return 0x80000000 + getChar();	// unexpected character
			break;
	}
	L3:
	switch(getChar())
	{
		default:
			return KEYWORD_rcISDEFAS;	// accept state
			break;
	}
	L4:
	switch(getChar())
	{
		default:
			return ';';	// accept state
			break;
	}
	L5:
	switch(getChar())
	{
		default:
			return '[';	// accept state
			break;
	}
	L6:
	switch(getChar())
	{
		default:
			return ']';	// accept state
			break;
	}
	L7:
	switch(getChar())
	{
		default:
			return '{';	// accept state
			break;
	}
	L8:
	switch(getChar())
	{
		default:
			return '|';	// accept state
			break;
	}
	L9:
	switch(getChar())
	{
		default:
			return '}';	// accept state
			break;
	}
	return 0;
}


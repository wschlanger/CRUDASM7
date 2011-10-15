// out_intel_lexer.h - automatically generated file (do not edit!)
// Copyright (C) 2009-2011 Willow Schlanger

virtual void initManualKeywords()
{
	addManualKeyword("a16", KEYWORD_ixa16);
	addManualKeyword("a32", KEYWORD_ixa32);
	addManualKeyword("a64", KEYWORD_ixa64);
	addManualKeyword("all", KEYWORD_ixall);
	addManualKeyword("as", KEYWORD_ixas);
	addManualKeyword("asm_skip", KEYWORD_ixasm_skip);
	addManualKeyword("assert", KEYWORD_ixassert);
	addManualKeyword("ctrlxfer", KEYWORD_ixctrlxfer);
	addManualKeyword("dis", KEYWORD_ixdis);
	addManualKeyword("discard", KEYWORD_ixdiscard);
	addManualKeyword("emu", KEYWORD_ixemu);
	addManualKeyword("fwaitable", KEYWORD_ixfwaitable);
	addManualKeyword("has_cc", KEYWORD_ixhas_cc);
	addManualKeyword("imm64_disp", KEYWORD_iximm64_disp);
	addManualKeyword("imm64_sx32", KEYWORD_iximm64_sx32);
	addManualKeyword("inport", KEYWORD_ixinport);
	addManualKeyword("insn", KEYWORD_ixinsn);
	addManualKeyword("is64", KEYWORD_ixis64);
	addManualKeyword("like_arpl", KEYWORD_ixlike_arpl);
	addManualKeyword("like_movsxd", KEYWORD_ixlike_movsxd);
	addManualKeyword("lock_always", KEYWORD_ixlock_always);
	addManualKeyword("lockable", KEYWORD_ixlockable);
	addManualKeyword("masm", KEYWORD_ixmasm);
	addManualKeyword("nasm", KEYWORD_ixnasm);
	addManualKeyword("no64", KEYWORD_ixno64);
	addManualKeyword("no_rex_w", KEYWORD_ixno_rex_w);
	addManualKeyword("o16", KEYWORD_ixo16);
	addManualKeyword("o32", KEYWORD_ixo32);
	addManualKeyword("o64", KEYWORD_ixo64);
	addManualKeyword("outport", KEYWORD_ixoutport);
	addManualKeyword("pop", KEYWORD_ixpop);
	addManualKeyword("push", KEYWORD_ixpush);
	addManualKeyword("reg_base", KEYWORD_ixreg_base);
	addManualKeyword("reg_rm", KEYWORD_ixreg_rm);
	addManualKeyword("relative", KEYWORD_ixrelative);
	addManualKeyword("repable", KEYWORD_ixrepable);
	addManualKeyword("repcond", KEYWORD_ixrepcond);
	addManualKeyword("reserve", KEYWORD_ixreserve);
	addManualKeyword("s16", KEYWORD_ixs16);
	addManualKeyword("s32", KEYWORD_ixs32);
	addManualKeyword("s64", KEYWORD_ixs64);
	addManualKeyword("sizeof", KEYWORD_ixsizeof);
	addManualKeyword("sx", KEYWORD_ixsx);
	addManualKeyword("sx_byte", KEYWORD_ixsx_byte);
	addManualKeyword("tr", KEYWORD_ixtr);
	addManualKeyword("zx", KEYWORD_ixzx);
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
		case '(':
			nextChar();
			goto L2;
		case ')':
			nextChar();
			goto L3;
		case ',':
			nextChar();
			goto L4;
		case ':':
			nextChar();
			goto L5;
		case ';':
			nextChar();
			goto L6;
		case '<':
			nextChar();
			goto L7;
		case '=':
			nextChar();
			goto L8;
		case '>':
			nextChar();
			goto L9;
		case '{':
			nextChar();
			goto L10;
		case '}':
			nextChar();
			goto L11;
		default:
			return defaultHandler(0x80000000 + getChar());	//start state
			break;
	}
	L1:
	switch(getChar())
	{
		default:
			return '!';	// accept state
			break;
	}
	L2:
	switch(getChar())
	{
		default:
			return '(';	// accept state
			break;
	}
	L3:
	switch(getChar())
	{
		default:
			return ')';	// accept state
			break;
	}
	L4:
	switch(getChar())
	{
		default:
			return ',';	// accept state
			break;
	}
	L5:
	switch(getChar())
	{
		default:
			return ':';	// accept state
			break;
	}
	L6:
	switch(getChar())
	{
		default:
			return ';';	// accept state
			break;
	}
	L7:
	switch(getChar())
	{
		default:
			return '<';	// accept state
			break;
	}
	L8:
	switch(getChar())
	{
		default:
			return '=';	// accept state
			break;
	}
	L9:
	switch(getChar())
	{
		default:
			return '>';	// accept state
			break;
	}
	L10:
	switch(getChar())
	{
		default:
			return '{';	// accept state
			break;
	}
	L11:
	switch(getChar())
	{
		default:
			return '}';	// accept state
			break;
	}
	return 0;
}


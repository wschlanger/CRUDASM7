// token_enums.h
// Copyright (C) 2009 Willow Schlanger

#ifndef	token_enums_h__ncc__included
#define	token_enums_h__ncc__included

typedef unsigned char U1;
typedef unsigned short U2;
typedef unsigned U4;
typedef unsigned long U8;
typedef signed char S1;
typedef signed short S2;
typedef signed S4;
typedef signed long S8;	// this is used for enumeration's.
typedef long double LongDouble;
typedef unsigned long UINT;
typedef signed long SINT;

// 0..0xffff : corresponding UNICODE character
// 0x10000..0x1ffff : symbolic terminals (e.g. PLUSEQ which means "+=")
// 0x20000..0x2ffff : rules, e.g. nonterminals (see out_chdr.h)
// 0x30000..0x3ffff : identifier keywords (see out_keywords.h)
// 0xfffffe00..0xfffffeff : special tokens we introduce to avoid ambiguities
// 0xffffff00..0xffffffff : special tokens introduced by lexer such as TOKEN_EOF.
// Note: TOKEN_EOF is indicated by the current NCC lexer when we reach end of an
// already-preprocessed line. We want to automatically go to the next line when
// this happens, and return TOKEN_EOF only when we're at the end of the LAST line.

struct CTokenEnums
{
	enum
	{
		TOKEN_ANDAND = 0x10000,
		TOKEN_ANDEQ,
		TOKEN_DIVEQ,
		TOKEN_DOTDOTDOT,
		TOKEN_DOTSTAR,
		TOKEN_EQEQ,
		TOKEN_GE,
		TOKEN_LE,
		TOKEN_MINUSEQ,
		TOKEN_MINUSMINUS,
		TOKEN_MODEQ,
		TOKEN_NOTEQ,
		TOKEN_NUMNUM,
		TOKEN_OREQ,
		TOKEN_OROR,
		TOKEN_PLUSEQ,
		TOKEN_PLUSPLUS,
		TOKEN_POINT,
		TOKEN_POINTSTAR,
		TOKEN_SCOPE,
		TOKEN_SHL,
		TOKEN_SHLEQ,
		TOKEN_SHR,
		TOKEN_SHREQ,
		TOKEN_STAREQ,
		TOKEN_XOREQ,
		TOKEN_TYPEREFID = 0xfffffe00,
		TOKEN_SPACE = 0xffffff00,
		TOKEN_IDENT,
		TOKEN_NUM,
		TOKEN_CLITERAL,
		TOKEN_SLITERAL,
		TOKEN_UNKNOWN = 0xfffffffe,
		TOKEN_EOF = 0xffffffff
	};
};

#endif	// token_enums_h__ncc__included

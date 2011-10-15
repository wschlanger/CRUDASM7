// out_rulecom_header.h - automatically generated file (do not edit!)
// Copyright (C) 2009 Willow Schlanger

enum
{
	RULE_rcstart = 0x30000,
	RULE_rcstatement = 0x30001,
	RULE_rcname = 0x30002,
	RULE_rcalternate = 0x30003,
	RULE_rcsymbol = 0x30004,
	RULE_rcitem = 0x30005,
	RULE__rccount = 6
};

enum
{
	TOKEN_rcIDENT = 0x10001,
	TOKEN_rcLITCHAR = 0x10002,
	TOKEN_rcLITSTRING = 0x10003,
	TOKEN__rccount = 3
};

enum
{
	KEYWORD_rcISDEFAS = 0x20000,	// "::="
	KEYWORD_rccopyright = 0x20003,	// "copyright"
	KEYWORD_rckeyword = 0x20002,	// "keyword"
	KEYWORD_rcmanual = 0x20005,	// "manual"
	KEYWORD_rcprefix = 0x20004,	// "prefix"
	KEYWORD_rcstart = 0x20006,	// "start"
	KEYWORD_rctoken = 0x20001,	// "token"
	KEYWORD__rccount = 7
};


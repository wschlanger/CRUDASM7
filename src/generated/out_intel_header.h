// out_intel_header.h - automatically generated file (do not edit!)
// Copyright (C) 2009,2011 Willow Schlanger

enum
{
	RULE_ixstart = 0x30000,
	RULE_ixitem = 0x30001,
	RULE_ixascii = 0x30002,
	RULE_ixargtype = 0x30003,
	RULE_ixargbase = 0x30004,
	RULE_ixremarks = 0x30005,
	RULE_ixknown_item = 0x30006,
	RULE_ixset_item = 0x30007,
	RULE_ixgroup_def = 0x30008,
	RULE_ixinsn_arg_list = 0x30009,
	RULE_ixinsn_body = 0x3000a,
	RULE_ixemu_body = 0x3000b,
	RULE_ixident_list = 0x3000c,
	RULE_ixinsn_arg = 0x3000d,
	RULE_ixmachine_code = 0x3000e,
	RULE_ixdis_body = 0x3000f,
	RULE_ixdis_stmt = 0x30010,
	RULE_ixrhs_list = 0x30011,
	RULE_ixrhs = 0x30012,
	RULE_ixslash_code = 0x30013,
	RULE_ixmachine_arg_list = 0x30014,
	RULE_ixmachine_extra_list = 0x30015,
	RULE_ixmod_code = 0x30016,
	RULE_ixmachine_extra_item = 0x30017,
	RULE_ixemu_stmt = 0x30018,
	RULE_ixrhs_prefix = 0x30019,
	RULE_ixrhs_terminal = 0x3001a,
	RULE_ixbinary_op = 0x3001b,
	RULE_ixsigned_cast = 0x3001c,
	RULE_ixalternates = 0x3001d,
	RULE__ixcount = 30
};

enum
{
	TOKEN_ixIDENT = 0x10001,
	TOKEN_ixLITNUM = 0x10002,
	TOKEN_ixSQUARE = 0x10003,
	TOKEN__ixcount = 3
};

enum
{
	KEYWORD_ixNE = 0x20003,	// "!="
	KEYWORD_ixANDAND = 0x20005,	// "&&"
	KEYWORD_ixSHL = 0x20006,	// "<<"
	KEYWORD_ixLE = 0x20000,	// "<="
	KEYWORD_ixEQ = 0x20002,	// "=="
	KEYWORD_ixGE = 0x20001,	// ">="
	KEYWORD_ixSHR = 0x20007,	// ">>"
	KEYWORD_ixbegin = 0x2000b,	// "begin"
	KEYWORD_ixcode = 0x20017,	// "code"
	KEYWORD_ixdesc = 0x20014,	// "desc"
	KEYWORD_ixdis = 0x20019,	// "dis"
	KEYWORD_ixemu = 0x2000e,	// "emu"
	KEYWORD_ixend = 0x2000c,	// "end"
	KEYWORD_ixexcept_assert = 0x2001f,	// "except_assert"
	KEYWORD_ixflags = 0x20012,	// "flags"
	KEYWORD_ixfunction = 0x2000f,	// "function"
	KEYWORD_ixgroup = 0x20011,	// "group"
	KEYWORD_ixgroups = 0x2000a,	// "groups"
	KEYWORD_ixif = 0x2001a,	// "if"
	KEYWORD_ixinsn = 0x2000d,	// "insn"
	KEYWORD_ixknown = 0x20008,	// "known"
	KEYWORD_ixmem_only = 0x2001d,	// "mem_only"
	KEYWORD_ixmod = 0x2001c,	// "mod"
	KEYWORD_ixmodes = 0x20010,	// "modes"
	KEYWORD_ixnotes = 0x20018,	// "notes"
	KEYWORD_ixr = 0x2001b,	// "r"
	KEYWORD_ixreturn = 0x20020,	// "return"
	KEYWORD_ixset = 0x20009,	// "set"
	KEYWORD_ixsigned = 0x20021,	// "signed"
	KEYWORD_ixsynopsis = 0x20013,	// "synopsis"
	KEYWORD_ixundefined = 0x2001e,	// "undefined"
	KEYWORD_ixunsigned = 0x20022,	// "unsigned"
	KEYWORD_ixvalid = 0x20015,	// "valid"
	KEYWORD_ixverified = 0x20016,	// "verified"
	KEYWORD_ixOROR = 0x20004,	// "||"
	KEYWORD__ixcount = 35
};


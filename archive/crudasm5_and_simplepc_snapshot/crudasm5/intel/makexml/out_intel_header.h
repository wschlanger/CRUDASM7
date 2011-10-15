// out_intel_header.h - automatically generated file (do not edit!)
// Copyright (C) 2009-2011 Willow Schlanger

enum
{
	RULE_ixstart = 0x30000,
	RULE_ixdeclaration_seq = 0x30001,
	RULE_ixdeclaration = 0x30002,
	RULE_ixflags = 0x30003,
	RULE_ixprototype = 0x30004,
	RULE_ixmachine = 0x30005,
	RULE_ixspecial = 0x30006,
	RULE_ixflag = 0x30007,
	RULE_ixmach = 0x30008,
	RULE_ixmach_suffix = 0x30009,
	RULE_ixmach_tag = 0x3000a,
	RULE_ixemu_stmt = 0x3000b,
	RULE_ixasmtype = 0x3000c,
	RULE_ixdis_stmt = 0x3000d,
	RULE_ixargs = 0x3000e,
	RULE_ixarg = 0x3000f,
	RULE_ixdis_arg = 0x30010,
	RULE_ixdis_args = 0x30011,
	RULE_ixprefix2 = 0x30012,
	RULE_ixrhs = 0x30013,
	RULE_ixlist = 0x30014,
	RULE__ixcount = 21
};

enum
{
	TOKEN_ixIDENT = 0x10001,
	TOKEN_ixLITSTRING = 0x10003,
	TOKEN_ixNUM = 0x10002,
	TOKEN__ixcount = 3
};

enum
{
	KEYWORD_ixa16 = 0x2001d,	// "a16"
	KEYWORD_ixa32 = 0x2001e,	// "a32"
	KEYWORD_ixa64 = 0x2001f,	// "a64"
	KEYWORD_ixall = 0x20016,	// "all"
	KEYWORD_ixas = 0x20019,	// "as"
	KEYWORD_ixasm_skip = 0x20007,	// "asm_skip"
	KEYWORD_ixassert = 0x20023,	// "assert"
	KEYWORD_ixctrlxfer = 0x20005,	// "ctrlxfer"
	KEYWORD_ixdis = 0x20015,	// "dis"
	KEYWORD_ixdiscard = 0x20027,	// "discard"
	KEYWORD_ixemu = 0x20014,	// "emu"
	KEYWORD_ixfwaitable = 0x20006,	// "fwaitable"
	KEYWORD_ixhas_cc = 0x20011,	// "has_cc"
	KEYWORD_iximm64_disp = 0x2000b,	// "imm64_disp"
	KEYWORD_iximm64_sx32 = 0x20009,	// "imm64_sx32"
	KEYWORD_ixinport = 0x20029,	// "inport"
	KEYWORD_ixinsn = 0x20000,	// "insn"
	KEYWORD_ixis64 = 0x2000c,	// "is64"
	KEYWORD_ixlike_arpl = 0x2000f,	// "like_arpl"
	KEYWORD_ixlike_movsxd = 0x2000e,	// "like_movsxd"
	KEYWORD_ixlock_always = 0x20002,	// "lock_always"
	KEYWORD_ixlockable = 0x20001,	// "lockable"
	KEYWORD_ixmasm = 0x20018,	// "masm"
	KEYWORD_ixnasm = 0x20017,	// "nasm"
	KEYWORD_ixno64 = 0x2000d,	// "no64"
	KEYWORD_ixno_rex_w = 0x2000a,	// "no_rex_w"
	KEYWORD_ixo16 = 0x2001a,	// "o16"
	KEYWORD_ixo32 = 0x2001b,	// "o32"
	KEYWORD_ixo64 = 0x2001c,	// "o64"
	KEYWORD_ixoutport = 0x20028,	// "outport"
	KEYWORD_ixpop = 0x20025,	// "pop"
	KEYWORD_ixpush = 0x20024,	// "push"
	KEYWORD_ixreg_base = 0x20010,	// "reg_base"
	KEYWORD_ixreg_rm = 0x20013,	// "reg_rm"
	KEYWORD_ixrelative = 0x20012,	// "relative"
	KEYWORD_ixrepable = 0x20003,	// "repable"
	KEYWORD_ixrepcond = 0x20004,	// "repcond"
	KEYWORD_ixreserve = 0x20026,	// "reserve"
	KEYWORD_ixs16 = 0x20020,	// "s16"
	KEYWORD_ixs32 = 0x20021,	// "s32"
	KEYWORD_ixs64 = 0x20022,	// "s64"
	KEYWORD_ixsizeof = 0x2002d,	// "sizeof"
	KEYWORD_ixsx = 0x2002a,	// "sx"
	KEYWORD_ixsx_byte = 0x20008,	// "sx_byte"
	KEYWORD_ixtr = 0x2002c,	// "tr"
	KEYWORD_ixzx = 0x2002b,	// "zx"
	KEYWORD__ixcount = 46
};


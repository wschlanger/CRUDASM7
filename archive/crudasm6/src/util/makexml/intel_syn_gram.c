// intel_syn_gram.c - grammar for intel syntax
// Copyright (C) 2009-2011 Willow Schlanger

manual IDENT "__azAZ" ;		// starts with _, a-z, A-Z
manual NUM "09" ;		// starts with 0..9
token LITSTRING ;

copyright "Copyright (C) 2009-2011 Willow Schlanger" ;
prefix "ix" ;			// for intel x86/x64

start ::= declaration_seq ;

declaration_seq ::= declaration { declaration } ;

// Note: if the insn name begins with a '_' then we need either a 'dis' section
// or we need a LITSTRING providing a name to use when disassembling.
declaration
::=	"insn" [ flags ] [ LITSTRING ] prototype machine { special }
;

flags ::= flag { flag } ;

flag
::=	"lockable"
|	"lock_always"
|	"repable"
|	"repcond"
|	"ctrlxfer"
|	"fwaitable"
;

machine ::= mach { mach } ;

mach ::= LITSTRING [ mach_suffix ] ;

mach_suffix ::= '!' mach_tag { ',' mach_tag } ;

mach_tag
::=	IDENT '=' IDENT			// e.g. P = B1
|	"asm_skip"
|	"sx_byte"
|	"imm64_sx32"
|	"no_rex_w"
|	"imm64_disp"
|	"is64"
|	"no64"
|	"like_movsxd"
|	"like_arpl"
|	"reg_base"
|	"has_cc"
|	"relative"
|	"reg_rm"
;

special
::=	"emu" '{' { emu_stmt } '}'
|	"dis" asmtype '{' { dis_stmt } '}'
;

// Precedence rules. If no 'dis' section exists, we do this:
//   write(<insn name>);
// if args exist:
//   space();
//   write_args();
// If an 'all' exists, that section is used UNLESS we are in
// a personality (asmtype) that exactly matches a more specific
// asmtype assembler listed.
asmtype
::=	"all"
|	"nasm"
|	"masm"
|	"as"		// GNU assembler
;

prototype
::=	IDENT '(' [ args ] ')'
;

args
::= arg { ',' arg }
;

arg
::=	IDENT IDENT
;

dis_arg
::=	IDENT
|	NUM
|	LITSTRING
;

dis_stmt
::=	IDENT '(' [ dis_args ] ')' ';'
;

dis_args
::=	dis_arg { ',' dis_arg }
;

prefix2
::=	"o16" ':'
|	"o32" ':'
|	"o64" ':'
|	"a16" ':'
|	"a32" ':'
|	"a64" ':'
|	"s16" ':'
|	"s32" ':'
|	"s64" ':'
;

emu_stmt
::=	{ prefix2 } IDENT IDENT ';'
|	{ prefix2 } IDENT IDENT '=' rhs ';'
|	{ prefix2 } IDENT '=' rhs ';'
|	{ prefix2 } "assert" '(' rhs ',' rhs ')' ';'		// 1st rhs must be a bit, 2nd should be an 8-bit #. 0..31=generate intr; 32..63=available; 64=unimplemented if assertion goes off
|	{ prefix2 } "push" '(' rhs ')' ';'
|	{ prefix2 } "pop" '(' rhs ')' ';'		// writes to rhs
|	{ prefix2 } "reserve" '(' rhs ')' ';'		// allocate space on the stack (rhs size should match stack size)
|	{ prefix2 } "discard" '(' rhs ')' ';'		// deallocate space on the stack (rhs size should match stack size)
|	{ prefix2 } "outport" '(' rhs ',' rhs ')' ';'	// port[rhs_left] := rhs_right;
|	{ prefix2 } "inport" '(' rhs ',' rhs ')' ';'	// rhs_left := port[rhs_right];
;

rhs
::=	IDENT [ '(' list ')' ]
|	"sx" '<' IDENT '>' '(' rhs ')'			// sign-extend, keep same, or truncate
|	"zx" '<' IDENT '>' '(' rhs ')'			// zero-extend, keep same, or truncate
|	"tr" '<' IDENT '>' '(' rhs ')'			// truncate -- fails if grows
							// note: tr<SIZE>(NUM) is handled specially.
							// also note that tr<BIT>(NUM) is allowed.
							// otherwise, zx<BIT>(...), sx<BIT>(...), and
							// tr<BIT>(...) are not normally allowed.
|	"sizeof" '(' IDENT ')'				// sizeof returns a B16 (same size as literals have by default)
|	NUM
;

list
::=	rhs { ',' rhs }
;

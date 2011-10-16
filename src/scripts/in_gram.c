// Copyright (C) 2009,2011 Willow Schlanger

copyright "Copyright (C) 2009,2011 Willow Schlanger" ;
prefix "ix"	;

// if a token starts with _, a-z, or A-Z then we'll recognize it manually
manual IDENT "__azAZ" ;

// If a token starts with 0-9, we'll recognize it manually.
manual LITNUM "09";

// [...] is a SQUARE bracket token.
// Whitespace at the beginning and end are trimmed, so "[ ]" becomes empty (same as "[]"). 
token SQUARE ;

keyword "<=" LE ;
keyword ">=" GE ;
keyword "==" EQ ;
keyword "!=" NE ;
keyword "||" OROR ;
keyword "&&" ANDAND ;
keyword "<<" SHL ;
keyword ">>" SHR ;

start ::= item { item } ;

ascii ::= IDENT | LITNUM ;

// This matches:
//   reg/mem
//   1
//   imm
//   rAX
//   mem&mem
//   and other argument types
argtype ::= argbase [ '&' argbase ] ;

argbase ::= ascii { '/' ascii } ;

remarks ::= SQUARE { SQUARE } ;

item
::= "known" known_item ';'
|   "set" set_item ';'
|   "groups" "begin" { group_def } "end" "groups" ';'
|   "insn" IDENT '(' [ insn_arg_list ] ')' "begin" { insn_body } "end" "insn" ';'
|   "emu" "function" IDENT IDENT '(' [ insn_arg_list ] ')' emu_body ';'
;

emu_sens
::= IDENT { ',' IDENT }
;

known_item
::= "modes" '=' '(' [ ident_list ] ')'
;

set_item
::= "modes" IDENT '=' '(' [ ident_list ] ')'
;

ident_list
::= IDENT { ',' IDENT }
;

group_def
::= "group" IDENT SQUARE ',' SQUARE ';'
;

insn_arg_list
::= insn_arg { ',' insn_arg }
;

insn_arg
::= IDENT [ '*' LITNUM ] IDENT [ '=' LITNUM ]
;

insn_body
::= "group" IDENT ';'
|   "flags" IDENT { ',' IDENT } ';'
|   "synopsis" IDENT remarks ';'
|   "desc" IDENT remarks ';'
|   "valid" "modes" '(' ident_list ')' ';'
|   "verified" "emu" SQUARE ';'
|   "emu" [ '(' emu_sens ')' ] emu_body "emu" ';'
|   "code" machine_code ';'
|   "valid" "modes" '(' ident_list ')' ';'
|   "notes" remarks ';'
|   "dis" "begin" [ dis_body ] "end" "dis" ';'
|	"asm" "names" SQUARE { ',' SQUARE } ';'
;

dis_body
::= dis_stmt { dis_stmt }
;

dis_stmt
::= IDENT '(' [ rhs_list ] ')' ';'
|   "if" '(' rhs ')' "begin" [ dis_body ] "end" ';'
;

machine_code
::=	ascii { ascii }
	{ slash_code_item }
	'(' machine_arg_list ')'
	[ '<' machine_extra_list '>' ]
;

slash_code_item
::= '/' slash_code
|   '+' "r"
;

slash_code
::= LITNUM
|   "r"
|	"mod" ':' mod_code
;

mod_code
::= "mem_only"
|   "reg_only"
;

machine_arg_list
::= argtype { ',' argtype }    // example: void
;

machine_extra_list
::= machine_extra_item { ',' machine_extra_item }
;

machine_extra_item
::= IDENT [ '=' machine_extra_asgn ]
;

machine_extra_asgn
::= ascii [ '(' machine_extra_tag_list ')' ]
;

machine_extra_tag_list
::= LITNUM { ',' LITNUM }
;

emu_body ::= "begin" { emu_stmt } "end" ;

emu_stmt
::= [ IDENT ] IDENT '=' ["undefined"] rhs ';'
|   [ IDENT ] IDENT '=' "undefined" ';'
|   "except_assert" '(' rhs ',' LITNUM ')' ';'      // generate the given exception if rhs is 0. rhs should be a BIT.
|   "return" rhs ';'                                // allowed only in emu functions, not in the emu section of an insn
|   "times" sizeref IDENT '=' rhs emu_body ';'
|	IDENT '(' LITNUM ')' ':' emu_stmt
;

sizeref
::= IDENT [ '*' LITNUM ]
;

rhs
::= [ rhs_prefix ] rhs_terminal [ binary_op rhs ]
|   [ rhs_prefix ] rhs_terminal '(' ')' [ binary_op rhs ]
|   [ rhs_prefix ] rhs_terminal '(' [ signed_cast ] rhs_list ')' [ binary_op rhs ]
;

// This is used for e.g. P(signed x) or B4(unsigned y)
signed_cast
::= "signed"
|   "unsigned"
;

rhs_list
::= rhs { ',' rhs }
;

rhs_prefix
::= '-'             // negation (operates on words)
|   '~'             // bitwise not (operats on words)
|   '!'             // logical not -- argument MUST be a bit
;

rhs_terminal
::= IDENT
|   LITNUM
|	'(' rhs [ alternates ] ')'
;

// (a ? b : c) is valid -- note the enclosing parenthesis, which are required.
alternates
::= '?' rhs ':' rhs
;

binary_op
::= '+'
|   '-'
|   '|'
|   '^'
|   '&'
|   "signed" '<'
|   "signed" '>'
|   "signed" "<="
|   "signed" ">="
|   "unsigned" '<'
|   "unsigned" '>'
|   "unsigned" "<="
|   "unsigned" ">="
|   "!="
|   "=="
|   "||"
|   "&&"
|   '*'
|   "unsigned" '/'
|   "unsigned" '%'
|   "signed" '/'
|   "signed" '%'
|	"<<"
|	"unsigned" ">>"
|	"signed" ">>"
;

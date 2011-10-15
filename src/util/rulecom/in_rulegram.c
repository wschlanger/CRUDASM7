// in_rulegram.c - grammar for rulecom grammars
// Copyright (C) 2009 Willow Schlanger

copyright "Copyright (C) 2009 Willow Schlanger" ;
prefix "rc"	;		// for rulecom

// if a token starts with _, a-z, A-Z, 0-9 then we'll recognize it manually
manual IDENT "__azAZ09" ;

token LITCHAR LITSTRING ;
keyword "::=" ISDEFAS ;

start ::=
statement { statement } ;

statement ::=
"token" IDENT { IDENT } ';' |
name "::=" alternate { '|' alternate } ';' |
"keyword" LITSTRING IDENT ';' |
"copyright" LITSTRING ';' |
"prefix" LITSTRING ';' |
"manual" IDENT LITSTRING ';' ;		// token to be recognized manually

name ::= IDENT | "start" ;

alternate ::= symbol { symbol } ;

symbol ::=
item |
'{' alternate '}' |
'[' alternate ']' ;

item ::=
LITCHAR |
LITSTRING |
IDENT ;		// note: IDENT may start with 0-9 too

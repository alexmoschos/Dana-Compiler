%{
	#define YYSTYPE astNode*
	#include <stdio.h>
	#include <iostream>
	#include <stdlib.h>
	#include "ast.hpp"
	#include <string>
	using namespace std;
	extern int nl;
	int yylex(void);
	
	void yyerror (char const *s) {
	 	fprintf (stderr, "%s on line %d\n",s, nl);
	}
	
%}
%expect 1


%token IDENTIFIER INT BYTE IF ELSE STRINGLITERAL
%token LOOP AS SKIP DECL DEF BEG END VAR IS
%token EQ DIFF LARGER SMALLER RETURN EXIT REF
%token LARGEREQ SMALLEREQ ASSIGNMENT CONST
%token AND NOT OR ELIF TRUE FALSE BREAK CONT


%left OR
%left AND
%nonassoc NOT
%nonassoc SMALLER LARGER LARGEREQ SMALLEREQ EQ DIFF
%left '+' '-' '|'
%left '*' '/' '&'
%precedence UNARYPL 
%precedence UNARYMINUS
%precedence BANGBANG



%%
fdef
	:DEF header BEG stmt_list END
	;

fdecl
	:DECL header
	;

header
	: IDENTIFIER opt
	| IDENTIFIER ':' optparam
	| IDENTIFIER
	;
opt
	: IS type ':' optparam 
	| IS type
	;

optparam
	: idlist AS ftype
	| idlist AS ftype ',' optparam
	;

ftype
	: REF INT
	| REF BYTE
	| INT '['']' bp
	| BYTE '['']' bp
	| INT bp
	| BYTE bp

bp
	: %empty
	| '['CONST']'
	;


stmt_list
	: stmt
	| stmt_list stmt
	;

lval
	: IDENTIFIER
	| STRINGLITERAL
	| lval '['expression']'
	;

type
	: INT 
	| BYTE
	| type '[' CONST ']'		
	;


pc
	: IDENTIFIER ':' param
	| IDENTIFIER
	;

param
	: expression ',' param
	| expression
	;

stmt
	: SKIP 
	| pc 		{printf("HERE BOY\n");}
	| mif
	| fdef
	| loop
	| lval ASSIGNMENT expression	{printf("Found assignment\n");}
	| VAR idlist IS type {printf("Found type decl\n");}
	| BREAK
	| BREAK ':' IDENTIFIER
	| CONT
	| CONT ':' IDENTIFIER
	| fdecl
	| EXIT
	| RETURN ':' expression
	| fcall
	;

fcall
	: IDENTIFIER '(' param ')'
	| IDENTIFIER '(' ')'
	;

idlist
	: IDENTIFIER
	| IDENTIFIER idlist
	;
loop
	: LOOP IDENTIFIER ':' BEG stmt_list END {printf("Found loop\n");}
	| LOOP ':' BEG stmt_list END	{printf("Found loop without identifier\n");}
	;

mif
	: IF condition ':' BEG stmt_list END ELSE ':' BEG stmt_list END	{printf("Found matched if\n");}
	| IF condition ':' BEG stmt_list END ELIF condition ':' BEG stmt_list END eliftstmt
	| IF condition ':' BEG stmt_list END	{printf("Found unmatched if\n");}
	;

condition
	: expression LARGER expression 
	| expression SMALLER expression 
	| expression LARGEREQ expression
	| expression SMALLEREQ expression
	| expression EQ expression
	| expression DIFF expression
	| TRUE
	| FALSE
	| condition AND condition
	| condition OR condition
	| NOT condition
	| '('condition ')'
	| expression 	{printf("This fucking rule\n");}
	;

eliftstmt
	: ELSE ':' BEG stmt_list END
	| ELIF condition ':' BEG stmt_list END eliftstmt
	;

expression
	: expression '+' expression 
	| expression '-' expression	
	| expression '*' expression	
	| expression '/' expression	
	| expression '&' expression {printf("Bitwise &\n");}
	| expression '|' expression {printf("Bitwise OR\n");}
	| '+' expression %prec UNARYPL	
	| '-' expression %prec UNARYMINUS	
	| '!' expression %prec BANGBANG	{printf("Bang !\n");/*pew pew!*/}
	| IDENTIFIER 	{printf("expression with identifier\n");}
	| STRINGLITERAL	{printf("String literal\n");}
	| CONST 		{}
	| '(' expression ')'
	| fcall
	;

%%


int main(){
	cout << "Parser Version 0.0.0.0001" << endl;
	yyparse();
	printf("Hello World");
}
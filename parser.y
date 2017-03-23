%{
	#include <stdio.h>
	#include <iostream>
	using namespace std;
	int yylex(void);
	void yyerror (char const *s) {
	 	fprintf (stderr, "%s\n", s);
	}
%}


%token IDENTIFIER INT BYTE IF ELSE STRINGLITERAL
%token LOOP AS SKIP DECL DEF BEG END VAR IS
%token EQ DIFF LARGER SMALLER RETURN EXIT
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
	:IDENTIFIER opt
	|IDENTIFIER
	;
opt
	:IS type ':' optparam 
	|IS type
	;

optparam
	: IDENTIFIER AS type
	| IDENTIFIER AS type ',' optparam
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
	:INT '[' CONST ']'	
	| BYTE '[' CONST ']'
	| INT
	| BYTE		
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
	| pc
	| mif
	| fdef
	| loop
	| lval ASSIGNMENT expression	{printf("Found assignment\n");}
	| VAR lval IS type {printf("Found type decl\n");}
	| BREAK
	| BREAK ':' IDENTIFIER
	| CONT
	| CONT ':' IDENTIFIER
	| fdecl
	| EXIT
	| RETURN ':' expression
	;

loop
	: LOOP IDENTIFIER ':' BEG stmt_list END {printf("Found loop\n");}
	| LOOP ':' BEG stmt_list END	{printf("Found loop without identifier\n");}
	;

mif
	: IF condition ':' BEG stmt_list END ELSE ':' BEG stmt_list END	{printf("Found matched if\n");}
	| IF expression ':' BEG stmt_list END ELIF expression ':' BEG stmt_list END eliftstmt
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
	;


eliftstmt
	: ELSE ':' BEG stmt_list END
	| ELIF expression ':' BEG stmt_list END eliftstmt
	;

expression
	: expression '+' expression {printf("addition\n");}
	| expression '-' expression	{printf("subtraction\n");}
	| expression '*' expression	{printf("multiplication\n");}
	| expression '/' expression	{printf("division \n");}
	| expression '&' expression {printf("Bitwise &\n");}
	| expression '|' expression {printf("Bitwise OR\n");}
	| '+' expression %prec UNARYPL	{printf("unary plus\n");}
	| '-' expression %prec UNARYMINUS	{printf("unary minus\n");}
	| '!' expression %prec BANGBANG	{printf("Bang !\n");/*pew pew!*/}
	| IDENTIFIER 	{printf("expression with identifier\n");}
	| STRINGLITERAL	{printf("String literal\n");}
	| CONST 		{printf("Const\n");}
	| '(' expression ')'
	;

%%


int main(){
	cout << "Parser Version 0.0.0.0001" << endl;
	yyparse();
	printf("Hello World");
}
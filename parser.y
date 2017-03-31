%{
	#include <stdio.h>
	#include <iostream>
	using namespace std;
	extern int nl;
	int yylex(void);
	void yyerror (char const *s) {
	 	fprintf (stderr, "Syntax error on line %d %s\n",nl, s);
	}
	
%}
%expect 1

%token IDENTIFIER INT BYTE IF ELSE STRINGLITERAL
%token LOOP AS SKIP DECL DEF  END VAR IS
%token EQ DIFF LARGER SMALLER RETURN EXIT REF
%token LARGEREQ SMALLEREQ ASSIGNMENT CONST
%token AND NOT OR ELIF TRUE FALSE BREAK CONT BEG

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
	:DEF header  stmt_list END
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
	| INT
	| BYTE

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
	| pc
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
	: LOOP IDENTIFIER ':'  stmt_list END {printf("Found loop\n");}
	| LOOP ':'  stmt_list END	{printf("Found loop without identifier\n");}
	;

mif
	: IF condition ':'  stmt_list END ELSE ':'  stmt_list END	{printf("Found matched if\n");}
	| IF condition ':'  stmt_list END ELIF condition ':'  stmt_list END eliftstmt
	| IF condition ':'  stmt_list END	{printf("Found unmatched if\n");}
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
	: ELSE ':'  stmt_list END
	| ELIF condition ':'  stmt_list END eliftstmt
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
	| fcall
	;

%%


int main(){
	cout << "Parser Version 0.0.0.0001" << endl;
	yyparse();
	printf("Hello World");
}

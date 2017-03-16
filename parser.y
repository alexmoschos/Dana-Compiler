%{
	#include <stdio.h>
	int yylex(void);
	void yyerror (char const *s) {
	 	fprintf (stderr, "%s\n", s);
	}
%}


%token IDENTIFIER INT BYTE IF ELSE STRINGLITERAL
%token LOOP AS SKIP DECL DEF BEG END VAR IS
%token ADD MUL DIV SUB EQ DIFF LARGER SMALLER
%token LARGEREQ SMALLEREQ COLON ASSIGNMENT CONST

%%
body
	: BEG stmt_list END	{printf("BODY FOUND\n");}


stmt_list
	: stmt
	| stmt_list stmt


lval
	: IDENTIFIER
	| STRINGLITERAL
	| lval '['expression']'

type
	:INT '[' CONST ']'	
	| BYTE '[' CONST ']'
	| INT
	| BYTE		


stmt
	: SKIP
	| lval ASSIGNMENT expression	{printf("Found assignment\n");}
	| VAR lval IS type {printf("Found type decl\n");}


expression
	: IDENTIFIER 	{printf("expression with identifier\n");}
	| STRINGLITERAL	{printf("String literal\n");}
	| CONST 		{printf("Const\n");}
	| '(' expression ')'
	;

%%


int main(){
	return yyparse();
	printf("Hello World");
}
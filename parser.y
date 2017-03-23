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
%token AND NOT OR ELIF TRUE FALSE BREAK CONT
%left '+' '-'
%left '*' '/' 

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
	| mif
	| loop
	| lval ASSIGNMENT expression	{printf("Found assignment\n");}
	| VAR lval IS type {printf("Found type decl\n");}


loop
	: LOOP IDENTIFIER ':' BEG stmt_list END


mif
	: IF expression ':' BEG stmt_list END ELSE ':' BEG stmt_list END	{printf("Found matched if\n");}
	| IF expression ':' BEG stmt_list END ELIF expression ':' BEG stmt_list END eliftstmt
	| IF expression ':' BEG stmt_list END	{printf("Found unmatched if\n");}

eliftstmt
	: ELSE ':' BEG stmt_list END
	| ELIF expression ':' BEG stmt_list END eliftstmt

expression
	: expression '+' expression {printf("addition\n");}
	| expression '-' expression	{printf("subtraction\n");}
	| expression '*' expression	{printf("multiplication\n");}
	| expression '/' expression	{printf("division \n");}
	| IDENTIFIER 	{printf("expression with identifier\n");}
	| STRINGLITERAL	{printf("String literal\n");}
	| CONST 		{printf("Const\n");}
	| '(' expression ')'
	;

%%


int main(){
	return yyparse();
	printf("Hello World");
}
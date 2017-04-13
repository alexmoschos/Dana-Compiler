%{
	#include "ast.h"
	#include <stdio.h>
	#include <string>
	#include <iostream>
    #include <vector>

	using namespace std;
	extern int nl;
	int yylex(void);
	void yyerror (char const *s) {
	 	fprintf (stderr, "Syntax error on line %d %s\n",nl, s);
	}

%}
%union{
	ASTfdef* func;
	ASTExpr* expr;
	ASTstmt* statement;
    ASTlval* lvalue;
	int const_val;
	char* idstring;
}
%expect 1

%token INT BYTE IF ELSE
%token LOOP AS SKIP DECL DEF  END VAR IS TEOF
%token EQ DIFF LARGER SMALLER RETURN EXIT REF
%token LARGEREQ SMALLEREQ ASSIGNMENT
%token AND NOT OR ELIF TRUE FALSE BREAK CONT BEG
%token<const_val> CONST
%token<idstring> IDENTIFIER STRINGLITERAL
%type<func> fdef
%type<statement> stmt_list stmt
%type<expr> expression condition
%type<lvalue> lval
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
main
	: fdef
	;
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
	| bp
	| INT
	| BYTE

bp
	: INT '['']'
	| BYTE '['']'
	| INT '[' CONST ']'
	| BYTE '[' CONST ']'
	| bp '['CONST']'
	;
stmt_list
	: stmt
	| stmt_list stmt
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
	: IF condition ':'  stmt_list END ELSE ':'  stmt_list END	{}
	| IF condition ':'  stmt_list END ELIF condition ':'  stmt_list END eliftstmt
	| IF condition ':'  stmt_list END	{}
	;

condition
	: expression LARGER expression     {$$=new ASTExpr('>',NULL,0,$1,$3);}
	| expression SMALLER expression    {$$=new ASTExpr('<',NULL,0,$1,$3);}
	| expression LARGEREQ expression   {$$=new ASTExpr('l',NULL,0,$1,$3);}
	| expression SMALLEREQ expression  {$$=new ASTExpr('s',NULL,0,$1,$3);}
	| expression EQ expression         {$$=new ASTExpr('e',NULL,0,$1,$3);}
	| expression DIFF expression       {$$=new ASTExpr('d',NULL,0,$1,$3);}
	| TRUE                             {$$=new ASTExpr('b',NULL,1,NULL,NULL);}
	| FALSE                            {$$=new ASTExpr('b',NULL,0,NULL,NULL);}
	| condition AND condition          {$$=new ASTExpr('a',NULL,0,$1,$3);}
	| condition OR condition           {$$=new ASTExpr('o',NULL,0,$1,$3);}
	| NOT condition                    {$$=new ASTExpr('n',NULL,0,NULL,$2);}
	| '(' condition ')'                {$$ = $2;}
	| expression                       {$$ = $1;}
	;


eliftstmt
	: ELSE ':'  stmt_list END
	| ELIF condition ':'  stmt_list END eliftstmt
	;

expression
	: expression '+' expression    {$$=new ASTExpr('+',NULL,0,$1,$3);}
	| expression '-' expression	   {$$=new ASTExpr('-',NULL,0,$1,$3);}
	| expression '*' expression	   {$$=new ASTExpr('*',NULL,0,$1,$3);}
	| expression '/' expression	   {$$=new ASTExpr('/',NULL,0,$1,$3);}
	| expression '&' expression    {$$=new ASTExpr('&',NULL,0,$1,$3);}
	| expression '|' expression    {$$=new ASTExpr('|',NULL,0,$1,$3);}
	| '+' expression %prec UNARYPL {$$=new ASTExpr('+',NULL,0,NULL,$2);}
	| '-' expression %prec UNARYMINUS	{$$=new ASTExpr('-',NULL,0,NULL,$2);}
    | '!' expression %prec BANGBANG     {$$=new ASTExpr('!',NULL,0,NULL,$2);}
	| lval                         {$$ = new ASTExpr('i',$1,0,NULL,NULL);}
	| CONST 		               {$$ = new ASTExpr('c',NULL,$1,NULL,NULL);}
	| '(' expression ')'           {$$ = $2;}
	| fcall
	;
lval
	: IDENTIFIER                   {$$ = new ASTlval(false,$1);}
	| STRINGLITERAL                {$$ = new ASTlval(true,$1);}
	| lval '['expression']'        {
                                        $1->indices->push_back($3);
                                        $$=$1;
                                        cout << $$->identifier << endl;
                                        $$->print();
                                   }
	;
%%


int main(){
    cout << "Parser Version 0.0.0.0011" << endl;
	yyparse();
	//while(yylex());
	printf("Hello World");
}

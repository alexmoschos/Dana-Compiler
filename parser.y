%{
	#include "ast.h"
	#include <stdio.h>
	#include <string>
	#include <iostream>
    	#include <vector>

	using namespace std;
	extern int nl;
    	vector<ASTExpr*> *lastparam;
	int yylex(void);
	void yyerror (char const *s) {
	 	fprintf (stderr, "Syntax error on line %d %s\n",nl, s);
	}

%}
%union{
	ASTfdef* func;
	ASTExpr* expr;
	ASTstmt* statement;
    	ASTparam* parameter;
    	ASTheader* head;
    	ASTlval* lvalue;
		int const_val;
		char* idstring;
    	ASTfcall* funccall;
    	ptype p;
}
%expect 1

%token INT BYTE IF ELSE
%token LOOP AS SKIP DECL DEF  END VAR IS TEOF
%token EQ DIFF LARGER SMALLER RETURN EXIT REF
%token LARGEREQ SMALLEREQ ASSIGNMENT
%token AND NOT OR ELIF TRUE FALSE BREAK CONT BEG
%token<const_val> CONST CHAR_CONST
%token<idstring> IDENTIFIER STRINGLITERAL
%type<func> fdef fdecl
%type<statement> stmt_list stmt loop mif
%type<expr> expression condition param
%type<head> header
%type<lvalue> lval
%type<parameter> optparam
%type<p> ftype
%type<funccall> fcall pc


%left OR
%left AND
%nonassoc NOT
%nonassoc SMALLER LARGER LARGEREQ SMALLEREQ EQ DIFF
%left '+' '-' '|'
%left '*' '/' '&'
%precedence UNARYPL
%precedence UNARYMINUS
%precedence BANG



%%
fdef
	:DEF header  stmt_list END {$$ = new ASTfdef($2,$3);}
	;

fdecl
	:DECL header       {$$ = new ASTfdef($2,NULL);}
	;

header
	: IDENTIFIER IS type ':' optparam       {$$ = new ASTheader(0,$5);}
    	| IDENTIFIER IS type           {$$ = new ASTheader(0,NULL);}
	| IDENTIFIER ':' optparam      {$$ = new ASTheader(0,$3);}
	| IDENTIFIER                   {$$ = new ASTheader(0,NULL);}
	;

optparam
	: idlist AS ftype              {$$ = new ASTparam("test",$3,NULL);}
	| idlist AS ftype ',' optparam {$$ = new ASTparam("test1",$3,$5);}
	;

ftype
	: REF INT          {$$ = RINT;}
	| REF BYTE         {$$ = RBYTE;}
    	| bp               {$$ = INTA;/*lathos*/}
	| INT              {$$ = INTEG;}
	| BYTE             {$$ = BYT;}

bp
	: INT '['']'
	| BYTE '['']'
	| INT '[' CONST ']'
	| BYTE '[' CONST ']'
	| bp '['CONST']'
	;
stmt_list
	: stmt {$$ = $1;}
	| stmt stmt_list{
                            $1->tail=$2;
                            $$ = $1;
                       	}
	;



type
	: INT
	| BYTE
	| type '[' CONST ']'
	;


pc
	: IDENTIFIER ':' param     {
					$$ = new ASTfcall($1);
					$$->parameters = lastparam;
					lastparam = new vector<ASTExpr*>();
				   }
	| IDENTIFIER               {$$ = new ASTfcall($1); $$->parameters = NULL;}
	;

param
	: expression ',' param {lastparam->push_back($1);}
	| expression           {lastparam->push_back($1);}
	;

stmt
	: SKIP                     {$$ = new ASTstmt(TSKIP,NULL,NULL,"");}
	| pc                   	{
                                    $$ = new ASTstmt(TPC,NULL,NULL,"");
                                    $$->expr = new ASTExpr('f',NULL,0,NULL,NULL);
                                    $$->expr->f = $1;
                               	}
	| mif                      {$$ = $1;}
	| fdef                 	{
                                    $$ = new ASTstmt(TFDEF,NULL,NULL,"");
                                    $$->def = $1;
                               	}
	| loop                     {$$ = $1;}
	| lval ASSIGNMENT expression   {
                                            $$ = new ASTstmt(TASSIGN,NULL,NULL,"");
                                            $$->expr = $3;
                                            $$->lvalue = $1;
                                       }
	| VAR idlist IS type       {$$ = new ASTstmt(TDECL,NULL,NULL,"");}
	| BREAK                    {$$ = new ASTstmt(TBREAK,NULL,NULL,"");}
	| BREAK ':' IDENTIFIER     {$$ = new ASTstmt(TBREAKM,NULL,NULL,$3);}
	| CONT                     {$$ = new ASTstmt(TCONT,NULL,NULL,"");}
	| CONT ':' IDENTIFIER      {$$ = new ASTstmt(TCONTM,NULL,NULL,$3);}
	| fdecl                {
                                    $$ = new ASTstmt(TFDECL,NULL,NULL,"");
                                    $$->def = $1;
                               }
	| EXIT                     {$$ = new ASTstmt(TEXIT,NULL,NULL,"");}
	| RETURN ':' expression    {
                                    $$ = new ASTstmt(TRET,NULL,NULL,"");
                                    $$->expr = $3;
                               }
	| fcall             {
                                $$ = new ASTstmt(TFCALL,NULL,NULL,"");
                                $$->expr = new ASTExpr('f',NULL,0,NULL,NULL);
                                $$->expr->f = $1;
                            }
	;

fcall
	: IDENTIFIER '(' param ')' {$$ = new ASTfcall($1); $$->parameters = lastparam;lastparam = new vector<ASTExpr*>();}
	| IDENTIFIER '(' ')'       {$$ = new ASTfcall($1); $$->parameters = NULL;}
	;

idlist
	: IDENTIFIER
	| IDENTIFIER idlist
	;
loop
	: LOOP IDENTIFIER ':'  stmt_list END {$$ = new ASTstmt(TLOOP,NULL,NULL,$2);}
	| LOOP ':'  stmt_list END	         {$$ = new ASTstmt(TLOOP,$3,NULL,"");}
	;

mif
	: IF condition ':'  stmt_list END ELSE ':'  stmt_list END	{$$ = new ASTstmt(TIF,$4,NULL,"");}
	| IF condition ':'  stmt_list END ELIF condition ':'  stmt_list END eliftstmt {$$ = new ASTstmt(TIF,$4,NULL,"");}
	| IF condition ':'  stmt_list END  {$$ = new ASTstmt(TIF,$4,NULL,"");}
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
	: expression '+' expression        {$$=new ASTExpr('+',NULL,0,$1,$3);}
	| expression '-' expression	       {$$=new ASTExpr('-',NULL,0,$1,$3);}
	| expression '*' expression	       {$$=new ASTExpr('*',NULL,0,$1,$3);}
	| expression '/' expression	       {$$=new ASTExpr('/',NULL,0,$1,$3);}
	| expression '&' expression        {$$=new ASTExpr('&',NULL,0,$1,$3);}
	| expression '|' expression        {$$=new ASTExpr('|',NULL,0,$1,$3);}
	| '+' expression %prec UNARYPL     {$$=new ASTExpr('+',NULL,0,NULL,$2);}
	| '-' expression %prec UNARYMINUS  {$$=new ASTExpr('-',NULL,0,NULL,$2);}
    	| '!' expression %prec BANG    {$$=new ASTExpr('!',NULL,0,NULL,$2);}
	| lval                             {$$ = new ASTExpr('i',$1,0,NULL,NULL);}
	| CONST                            {$$ = new ASTExpr('c',NULL,$1,NULL,NULL);}
	| '(' expression ')'               {$$ = $2;}
	| fcall                            {$$ = new ASTExpr('f',NULL,0,NULL,NULL); $$->f = $1;}
	| CHAR_CONST                       {$$ = new ASTExpr('x',NULL,$1,NULL,NULL);}
	;
lval
	: IDENTIFIER                   {$$ = new ASTlval(false,$1);}
	| STRINGLITERAL                {$$ = new ASTlval(true,$1);}
	| lval '['expression']'    {
                                        $1->indices->push_back($3);
                                        $$=$1;
                                        cout << $$->identifier << endl;
                                        $$->print();
                                   }
	;
%%


int main(){
	cout << "Parser Version 0.0.0.0011" << endl;
	lastparam = new vector<ASTExpr*>();
	yyparse();
	//while(yylex());
	printf("Hello World");
}

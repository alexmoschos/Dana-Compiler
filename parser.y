%{
	extern "C"{
      #include "symbol.h"
    }
	#include "llvm.h"
    #include "sem.h"
	#include "ast.h"
	#include <stdio.h>
	#include <string>
	#include <iostream>
	#include <stack>
	#include <vector>
	#include <algorithm>

	using namespace std;
	extern int nl;
    vector<ASTExpr*> *lastparam;
    vector<string>   *identifiers;
    stack<ASTfdef*>   FUNCTION_NAMES;

 	int yylex(void);
    ASTfdef* main_f;

	void yyerror (char const *s) {
        fprintf (stderr, "Syntax error on line %d %s\n",nl, s);
	}


%}
%union{
	ASTfdef   *func;
	ASTExpr   *expr;
	ASTstmt   *statement;
    ASTparam  *parameter;
    ASTheader *head;
    ASTlval   *lvalue;
    ASTfcall  *funccall;
    ASTif     *ifp;
	int   const_val;
	char  *idstring;
	Type  var_type;
    vector<string> *list;
}
%expect 1

%token INT BYTE IF ELSE
%token LOOP AS SKIP DECL DEF  END VAR IS TEOF
%token EQ DIFF LARGER SMALLER RETURN EXIT REF
%token LARGEREQ SMALLEREQ ASSIGNMENT
%token AND NOT OR ELIF TRUE FALSE BREAK CONT BEG
%token<const_val> CONST CHAR_CONST
%token<idstring> IDENTIFIER STRINGLITERAL
%type<func> fdef fdecl mainfunc
%type<statement> stmt_list stmt loop decl decl_list
%type<ifp> mif eliftstmt
%type<expr> expression condition param
%type<head> header
%type<lvalue> lval
%type<parameter> optparam
%type<var_type> ftype type bp type_term reftype
%type<funccall> fcall pc
%type<list> idlist

%left OR
%left AND
%nonassoc NOT
%nonassoc SMALLER LARGER LARGEREQ SMALLEREQ EQ DIFF
%left '+' '-' '|'
%left '*' '/' '&' '%'
%precedence UNARYPL
%precedence UNARYMINUS
%precedence BANG

%%

mainfunc
    : fdef 								{$$ = $1;main_f = $1;}
    ;

fdef
	: DEF header {FUNCTION_NAMES.push(new ASTfdef($2,NULL));} decl_list END {$$ = new ASTfdef($2,$4); FUNCTION_NAMES.pop();}
	;

fdecl
	: DECL header       					 {$$ = new ASTfdef($2,NULL);}
	;

header
	: IDENTIFIER IS type ':' optparam    {$$ = new ASTheader($3,$5,$1);}
    | IDENTIFIER IS type                 {$$ = new ASTheader($3,NULL,$1);}
	| IDENTIFIER ':' optparam            {$$ = new ASTheader(typeVoid,$3,$1);}
	| IDENTIFIER                         {$$ = new ASTheader(typeVoid,NULL,$1);}
	;

optparam
	: idlist AS ftype                    {$$ = new ASTparam($1,$3,NULL);}
    | idlist AS reftype                  {$$ = new ASTparam($1,$3,NULL); $$->byref = 1;}
	| idlist AS ftype ',' optparam       {$$ = new ASTparam($1,$3,$5);}
    | idlist AS reftype','optparam       {$$ = new ASTparam($1,$3,$5); $$->byref = 1;}
	;

ftype
    : bp      							 {$$ = $1;}
	| INT     							 {$$ = typeInteger;}
	| BYTE    							 {$$ = typeChar;}
    ;

reftype
    : REF INT          					 {$$ = typeInteger;}
    | REF BYTE        					 {$$ = typeChar;}
    ;

bp
	: INT '['']'       					 {$$ = typeIArray(typeInteger);}
	| BYTE '['']'      					 {$$ = typeIArray(typeChar);}
	| INT '['CONST']'  					 {$$ = typeArray($3,typeInteger);}
	| BYTE '['CONST']' 					 {$$ = typeArray($3,typeChar);}
	| bp '['CONST']'   					 {$$ = typeArray($3,$1);}
	;

stmt_list
	: stmt 								 {$$ = $1;}
	| stmt stmt_list 					 {$1->tail=$2; $$ = $1;}
	;

decl_list
    : stmt_list 						 {$$ = $1;}
    | decl decl_list      				 {$$ = $1; $1->tail=$2;}
    ;

decl
    : fdef                  			 {$$ = new ASTstmt(TFDEF,NULL,NULL,""); $$->def = $1;}
    | fdecl                 			 {$$ = new ASTstmt(TFDECL,NULL,NULL,""); $$->def = $1;}
    | VAR idlist IS type    			 {$$ = new ASTstmt(TDECL,NULL,NULL,""); $$->identifiers=$2;$$->t=$4;}
    ;

type
	: type '[' CONST ']' 				 {$$ = typeArray($3,$1);}
    | type_term          				 {$$ = $1;}
	;
type_term
	: INT      							 {$$ = typeInteger;}
	| BYTE     							 {$$ = typeChar;}

pc
	: IDENTIFIER ':' param     			 {
		$$ = new ASTfcall($1);
		$$->parameters = lastparam;
		//reverse(lastparam->begin(),lastparam->end());
	 	lastparam = new vector<ASTExpr*>();
	}
	| IDENTIFIER               			 {$$ = new ASTfcall($1); $$->parameters = NULL;}
	;

param
	: expression ',' param 				 {lastparam->push_back($1);}
	| expression           				 {lastparam->push_back($1);}
	;

stmt
	: SKIP                       		 {$$ = new ASTstmt(TSKIP,NULL,NULL,"");}
	| mif                        		 {$$ = new ASTstmt(TIF,NULL,NULL,""); $$->ifnode = $1;}
	| loop                       		 {$$ = $1;}
	| lval ASSIGNMENT expression 		 {$$ = new ASTstmt(TASSIGN,NULL,NULL,""); $$->expr = $3; $$->lvalue = $1;}
	| BREAK                      		 {$$ = new ASTstmt(TBREAK,NULL,NULL,"");}
	| BREAK ':' IDENTIFIER       		 {$$ = new ASTstmt(TBREAKM,NULL,NULL,$3);}
	| CONT                       		 {$$ = new ASTstmt(TCONT,NULL,NULL,"");}
	| CONT ':' IDENTIFIER        		 {$$ = new ASTstmt(TCONTM,NULL,NULL,$3);}
	| EXIT                       		 {$$ = new ASTstmt(TEXIT,NULL,NULL,""); $$->def = FUNCTION_NAMES.top();}
	| RETURN ':' expression      		 {$$ = new ASTstmt(TRET,NULL,NULL,""); $$->expr = $3; $$->def = FUNCTION_NAMES.top();}
    | pc                   	     		 {
                                  			 $$ = new ASTstmt(TPC,NULL,NULL,"");
                                   			 $$->expr = new ASTExpr('f',NULL,0,NULL,NULL);
                                   			 $$->expr->f = $1;
                                 		 }
	| fcall                      		 {
                                   			 $$ = new ASTstmt(TFCALL,NULL,NULL,"");
                                   			 $$->expr = new ASTExpr('f',NULL,0,NULL,NULL);
                                   			 $$->expr->f = $1;
                                 		 }
	;

fcall
	: IDENTIFIER '(' param ')'
	{
		$$ = new ASTfcall($1);
		$$->parameters = lastparam;
		//reverse($$->parameters->begin(),$$->parameters->end());
		lastparam = new vector<ASTExpr*>();
	}
	| IDENTIFIER '(' ')'       			 {$$ = new ASTfcall($1); $$->parameters = NULL;}
	;

idlist
	: IDENTIFIER         				 {identifiers = new vector<string>(); identifiers->push_back($1); $$ = identifiers;}
	| idlist IDENTIFIER  				 {identifiers->push_back($2);}
	;

loop
	: LOOP IDENTIFIER ':'  stmt_list END {$$ = new ASTstmt(TLOOP,$4,NULL,$2);}
	| LOOP ':'  stmt_list END	    	 {$$ = new ASTstmt(TLOOP,$3,NULL,"");}
	;

mif
	: IF condition ':'  stmt_list END ELSE ':'  stmt_list END	{
		                                                            $$ = new ASTif($2,$4);
		                                                            auto else_node = new ASTif(NULL,$8);
		                                                            $$->tail = else_node;
		                                                            else_node->tail = NULL;
	                                                             }
	| IF condition ':'  stmt_list END ELIF condition ':'  stmt_list END eliftstmt {
		       		                                                                 $$ = new ASTif($2,$4);
		                                                                             auto else_node = new ASTif($7,$9);
		                                                                             $$->tail = else_node;
		                                                                             else_node->tail = $11;
	                                                                               }
	| IF condition ':'  stmt_list END  {$$ = new ASTif($2,$4);}
	;

condition
	: expression LARGER expression       {$$=new ASTExpr('>',NULL,0,$1,$3);}
	| expression SMALLER expression      {$$=new ASTExpr('<',NULL,0,$1,$3);}
	| expression LARGEREQ expression     {$$=new ASTExpr('l',NULL,0,$1,$3);}
	| expression SMALLEREQ expression    {$$=new ASTExpr('s',NULL,0,$1,$3);}
	| expression EQ expression           {$$=new ASTExpr('e',NULL,0,$1,$3);}
	| expression DIFF expression         {$$=new ASTExpr('d',NULL,0,$1,$3);}
	| condition AND condition            {$$=new ASTExpr('a',NULL,0,$1,$3);}
	| condition OR condition             {$$=new ASTExpr('o',NULL,0,$1,$3);}
	| NOT condition                      {$$=new ASTExpr('n',NULL,0,NULL,$2);}
	| '(' condition ')'                  {$$ = $2;}
	| expression                         {$$ = $1;}
	;


eliftstmt
	: ELSE ':'  stmt_list END 			 {$$ = new ASTif(NULL,$3);}
    | %empty 							 {$$ = NULL;}
	| ELIF condition ':'  stmt_list END eliftstmt {$$ = new ASTif($2,$4);$$->tail=$6;}
	;

expression
	: expression '+' expression          {$$=new ASTExpr('+',NULL,0,$1,$3);}
	| expression '-' expression	         {$$=new ASTExpr('-',NULL,0,$1,$3);}
    | expression '*' expression	         {$$=new ASTExpr('*',NULL,0,$1,$3);}
    | expression '/' expression	         {$$=new ASTExpr('/',NULL,0,$1,$3);}
    | expression '%' expression          {$$=new ASTExpr('%',NULL,0,$1,$3);}
	| expression '&' expression          {$$=new ASTExpr('&',NULL,0,$1,$3);}
	| expression '|' expression          {$$=new ASTExpr('|',NULL,0,$1,$3);}
	| '+' expression %prec UNARYPL       {$$=new ASTExpr('+',NULL,0,NULL,$2);}
	| '-' expression %prec UNARYMINUS    {$$=new ASTExpr('-',NULL,0,NULL,$2);}
    | '!' expression %prec BANG          {$$=new ASTExpr('!',NULL,0,NULL,$2);}
	| lval                               {$$ = new ASTExpr('i',$1,0,NULL,NULL);}
	| CONST                              {$$ = new ASTExpr('c',NULL,$1,NULL,NULL);}
	| '(' expression ')'                 {$$ = $2;}
	| fcall                              {$$ = new ASTExpr('f',NULL,0,NULL,NULL); $$->f = $1;}
	| CHAR_CONST                         {$$ = new ASTExpr('x',NULL,$1,NULL,NULL);}
    | TRUE                               {$$=new ASTExpr('b',NULL,1,NULL,NULL);}
    | FALSE                              {$$=new ASTExpr('b',NULL,0,NULL,NULL);}
	;

lval
	: IDENTIFIER                  		 {$$ = new ASTlval(false,$1);}
	| STRINGLITERAL               		 {$$ = new ASTlval(true,$1);}
	| lval '['expression']'       		 {$1->indices->push_back($3); $$=$1;}
	;

%%

int main(){
	//cout << "Parser Version 0.0.1.00" << endl;
	initSymbolTable(997);
    openScope();
    add_lib_Functions();
    main_f = NULL;
    lastparam = new vector<ASTExpr*>();
    FUNCTION_NAMES =  stack<ASTfdef*>();
    if(yyparse()) return -1;
    //cout << "Parser is done!" << endl;
    sem_check_fdef(main_f);
    //cout << "Semantic analysis is done!" << endl;
	//cout << "Emitting LLVM" << endl;
	//cout << endl;
	Compile(main_f);
    closeScope();
    destroySymbolTable();
}

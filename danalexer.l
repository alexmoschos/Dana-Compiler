%x COMMENT

	int nl = 1;
	int comment_nesting = 0;  


ID [a-zA-Z_][a-zA-Z0-9_]*


%{
#define YYSTYPE astNode*
#include <stdio.h>
#include "ast.hpp"
#include "parser.hpp"	

%}


%%

\n 			{nl++;}
\r 				;
[ \t]			;

[0-9][0-9]* 		{yylval = new astNode(AST_num,NULL,NULL,"",atoi(yytext));return(CONST);}

if			{printf("Keyword: if\n");return(IF);}
else			{printf("Keyword: else\n");return(ELSE);}
elif			{printf("Keyword: elif\n");return(ELIF);}
loop			{printf("Keyword: loop\n");return(LOOP);}
int			{printf("Keyword: int\n");return(INT);}
byte			{printf("Keyword: byte\n");return(BYTE);}
as			{printf("Keyword: as\n");return(AS);}
skip			{printf("Keyword: skip\n");return(SKIP);}
decl			{printf("Keyword: decl\n");return(DECL);}
begin			{printf("Keyword: begin\n");return(BEG);}
end			{printf("Keyword: end\n");return(END);}
def			{printf("Keyword: def\n");return(DEF);}
is			{printf("Keyword: is\n");return(IS);}
var			{printf("Keyword: var\n");return(VAR);}
not			{printf("Keyword: not\n");return(NOT);}
and			{printf("Keyword: and\n");return(AND);}
or 			{printf("Keyword: or\n");return(OR);}
true		{printf("Keyword: true\n");return(TRUE);}
false		{printf("Keyword: false\n");return(FALSE);}
break		{printf("Keyword: break\n");return(BREAK);}
continue	{printf("Keyword: continue\n");return(CONT);}
exit		{printf("Keyword: exit\n");return(EXIT);}
return		{printf("Keyword: return\n");return(RETURN);}
ref 		{printf("Keyword: ref\n");return(REF);}

{ID} 			{printf("Identifier: %s\n",yytext);return(IDENTIFIER);}
"("			{printf("Left Parenthesis (\n");return('(');}
")"			{printf("Right Parenthesis )\n");return(')');}
"["			{printf("Left Brace [\n");return('[');}
"]"			{printf("Right Brace ]\n");return(']');}

"."			{printf("Dot .\n");return('.');}
","			{printf("Comma ,\n");return(',');}
"+"			{printf("Plus +\n");return('+');}
"-"			{printf("Minus -\n");return('-');}
"*"			{printf("Times *\n");return('*');}
"/"			{printf("Division /\n");return('/');}
":="			{printf("Assignment Operator :=\n");return(ASSIGNMENT);}
"=="			{printf("Equality operator ==\n");return(EQ);}
"!"			{printf("Bang !\n");return('!');}
[%]			{printf("Mod Operator!\n");return('%');}
"&" 		{printf("Ampersand &\n");return('&');}
"|"			{printf("Kagkelo |\n");return('|');}

"<"			{printf("Less than operator <\n");return(SMALLER);}
"<="		{printf("Less than or equal <=\n");return(SMALLEREQ);}
">="		{printf("Greateer than or equal >=\n");return(LARGEREQ);}
"<>"		{printf("Inequality <>\n");return(DIFF);}
">"			{printf("Greater than operator >\n");return(LARGER);}
":"			{printf("Colon :\n");return(':');}


\"(\\.|[^\\"])*\"	{printf("String Literal %s \n",yytext);return(STRINGLITERAL);}

"#".*\n           	{printf("comment found\n");nl++;}

"(*"             	{ ++comment_nesting; BEGIN(COMMENT); }

<COMMENT>{
  "(*"           	{ ++comment_nesting; }
  "*"+")"        	{ --comment_nesting; if(!comment_nesting) BEGIN(INITIAL);} 
  "*"+           	; 
  [^(*\n]+       	; 
  [(]            	; 
  "\n"             	; {nl++;}
  <<EOF>>        ; 	{if (comment_nesting>0) printf("error: unclosed comment in line %d\n",nl); return -1;}
}

 <<EOF>> 		{printf("Read %d lines\n",nl);exit(0);}
.			{printf("Unexpected Character %d\n",yytext[0]);exit(-1);}





%%

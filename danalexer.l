%x COMMENT

	int nl = 1;
	int comment_nesting = 0;  /* Line 4 */


ID [a-zA-Z_][a-zA-Z0-9_]*


%{
#include <stdio.h>
#include "parser.h"	
static void comment();
%}


%%

\n 				{nl++;}
[ \t]			;

[0-9][0-9]* 		{printf("Integer %s\n",yytext);return(CONST);}

if				{printf("Keyword: if\n");return(IF);}
else			{printf("Keyword: else\n");return(ELSE);}
loop			{printf("Keyword: loop\n");return(LOOP);}
int				{printf("Keyword: int\n");return(INT);}
byte			{printf("Keyword: byte\n");return(BYTE);}
as				{printf("Keyword: as\n");return(AS);}
skip			{printf("Keyword: skip\n");return(SKIP);}
decl			{printf("Keyword: decl\n");return(DECL);}
begin			{printf("Keyword: begin\n");return(BEG);}
end				{printf("Keyword: end\n");return(END);}
def				{printf("Keyword: def\n");return(DEF);}
is				{printf("Keyword: is\n");return(IS);}
var				{printf("Keyword: var\n");return(VAR);}

{ID} 			{printf("Identifier: %s\n",yytext);return(IDENTIFIER);}
"("				{printf("Left Parenthesis (\n");return('(');}
")"				{printf("Right Parenthesis )\n");return(')');}
"{"				{printf("Left Curly Brace {\n");return('{');}
"}"				{printf("Right Curly Brace }\n");return('}');}
"."				{printf("Dot .\n");return('.');}
","				{printf("Comma ,\n");return(',');}
"+"				{printf("Plus +\n");return('+');}
"-"				{printf("Minus -\n");return('-');}
"*"				{printf("Times *\n");return('*');}
"/"				{printf("Division /\n");return('/');}
":="			{printf("Assignment Operator :=\n");return(ASSIGNMENT);}
"=="			{printf("Equality operator ==\n");return(EQ);}
"!"				{printf("Bang !\n");return('!');}

"<"				{printf("Less than operator <\n");return(SMALLER);}
">"				{printf("Greater than operator >\n");return(LARGER);}
":"				{printf("Colon :\n");return(':');}
"["				{printf("Left [\n");return('[');}
"]"				{printf("Right ]\n");return(']');}

\"(\\.|[^\\"])*\"	{printf("String Literal %s \n",yytext);return(STRINGLITERAL);}

"#".*            {printf("comment found\n");}

"(*"             { ++comment_nesting; BEGIN(COMMENT); }

<COMMENT>{
  "(*"           { ++comment_nesting; }
  "*"+")"        { --comment_nesting; if(!comment_nesting) BEGIN(INITIAL);} 
  "*"+           ; /* Line 11 - ignore */
  [^(*\n]+       ; /* Line 12  - also ignore*/
  [(]            ; /* Line 13 */
  "\n"             ; {nl++;}
  <<EOF>>        ; {if (comment_nesting>0) printf("error: unclosed comment in line %d\n",nl); return 0;}
}

 <<EOF>> 		{printf("Read %d lines\n",nl);exit(0);}
.				{printf("Unexpected Character %s\n",yytext);exit(-1);}





%%

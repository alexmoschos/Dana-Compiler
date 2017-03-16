
	int nl = 0;


ID [a-zA-Z_][a-zA-Z0-9_]*


%{
#include <stdio.h>
#include "parser.h"	
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
"."				printf("Dot .\n");
","				printf("Comma ,\n");
"+"				printf("Plus +\n");
"-"				printf("Minus -\n");
"*"				printf("Times *\n");
"/"				printf("Division /\n");
":="			{printf("Assignment Operator :=\n");return(ASSIGNMENT);}
"=="			printf("Equality operator ==\n");
"<"				printf("Less than operator <\n");
">"				printf("Greater than operator >\n");
":"				{printf("Colon :\n");return(':');}
"["				{printf("Left [\n");return('[');}
"]"				{printf("Right ]\n");return(']');}
\"(\\.|[^\\"])*\"	printf("String Literal %s \n",yytext);

.				printf("Unexpected Character %s\n",yytext);

%%



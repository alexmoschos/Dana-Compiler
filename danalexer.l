
	int nl = 0;


ID [a-zA-Z_][a-zA-Z0-9_]*


%{
#include <stdio.h>
#include "parser.h"	
%}


%%

\n 				{nl++;}
[ \t]			;

[1-9][0-9]*		{printf("Integer %s\n",yytext);return(CONST);}

if				{printf("Keyword: if\n");return(IF);}
else			{printf("Keyword: else\n");return(ELSE);}
loop			printf("Keyword: loop\n");
int				printf("Keyword: int\n");
byte			printf("Keyword: byte\n");
as				printf("Keyword: as\n");
skip			{printf("Keyword: skip\n");return(SKIP);}
decl			printf("Keyword: decl\n");
begin			{printf("Keyword: begin\n");return(BEG);}
end				{printf("Keyword: end\n");return(END);}
def				printf("Keyword: def\n");


{ID} 			{printf("Identifier: %s\n",yytext);return(IDENTIFIER);}

"("				printf("Left Parenthesis (\n");
")"				printf("Right Parenthesis )\n");
"{"				printf("Left Curly Brace {\n");
"}"				printf("Right Curly Brace }\n");
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
":"				printf("Colon :\n");
"["				printf("Left [\n");
"]"				printf("Right ]\n");
\"(\\.|[^\\"])*\"	printf("String Literal %s \n",yytext);

.				printf("Unexpected Character %s\n",yytext);

%%



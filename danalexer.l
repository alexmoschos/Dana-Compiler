
	int nl = 0;


ID [a-zA-Z_][a-zA-Z0-9_]*



%%

\n 				{nl++;}
[ \t]			;

[1-9][0-9]*		printf("Integer %s\n",yytext);

if             	printf("Keyword: if\n");
else			printf("Keyword: else\n");
loop 			printf("Keyword: loop\n");
int 			printf("Keyword: int\n");
byte			printf("Keyword: byte\n");
as 				printf("Keyword: as\n");
skip			printf("Keyword: skip\n");
decl 			printf("Keyword: decl\n");
begin			printf("Keyword: begin\n");
end				printf("Keyword: end\n");
def				printf("Keyword: def\n");


{ID} 			printf("Identifier: %s\n",yytext);

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
":="			printf("Assignment Operator :=\n");
"=="			printf("Equality operator ==\n");
"<"				printf("Less than operator <\n");
">"				printf("Greater than operator >\n");
":"				printf("Colon :\n");
"["				printf("Left [\n");
"]"				printf("Right ]\n");
\"(\\.|[^\\"])*\"	printf("String Literal %s \n",yytext);



%%


int main(){
	yylex();
	printf("Read %d lines",nl);
	return 0;
}

	int nl = 0;


ID [a-zA-Z_][a-zA-Z0-9_]*



%%

\n 				nl++;
[ \t]			;

[1-9][0-9]*		printf("integer %s\n",yytext);

if|else|for|while|int|byte|def 	printf("keyword %s\n",yytext);

{ID} 			printf("identifier %s\n",yytext);

"("				printf("Left Parenthesis (\n");
")"				printf("Right Parenthesis )\n");
"{"				printf("Left Curly Brace {\n");
"}"				printf("Right Curly Brace }\n");
"."				printf("Dot .\n");
"+"				printf("Plus +\n");
"-"				printf("Minus -\n");
"*"				printf("Times *\n");
"/"				printf("Division /\n");
"="				printf("Assignment Operator =\n");
"=="			printf("Equality operator ==\n");
"<"				printf("Less than operator <\n");
">"				printf("Greater than operator >\n");
":"				printf("Range operator :\n");


%%


int main(){
	yylex();
	printf("Read %d lines",nl);
	return 0;
}
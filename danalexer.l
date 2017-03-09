
	int nl = 0;


ID [a-zA-Z_][a-zA-Z0-9_]*



%%

\n 				nl++;
\t 				;


[1-9][0-9]*		printf("integer %s\n",yytext);

if|else|for		printf("keyword %s\n",yytext);

{ID} 			printf("identifier %s\n",yytext);

"("				printf("Left Parenthesis (");
")" 			printf("Right Parenthesis)");
"{"				printf("Left Curly Brace {");
"}"				printf("Right Curly Brace }");
"."				printf("Dot .");
"+"				printf("Plus +");
"-"				printf("Minus -");
"*"				printf("Times *");



%%


int main(){
	yylex();
	printf("Read %d lines",nl);
	return 0;
}
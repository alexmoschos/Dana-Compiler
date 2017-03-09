D [0-9]
W [ \t\n]
ID [a-zA-Z][a-zA-Z0-9]*



%%



[1-9][0-9]+		printf("integer %s\n",yytext);

if|else|for		printf("keyword %s\n",yytext);

{ID} 			print("identifier %s\n",yytext);

"(" | ")"		printf("Parenthesis %s\n",yytext);

%%


int main(){
	yylex();
	return 0;
}
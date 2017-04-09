%x COMMENT

	int nl = 1;
	int comment_nesting = 0;
	#define MAX_DEPTH 72

	unsigned int indent_stack[MAX_DEPTH] ;
	unsigned int level = 0 ;

	unsigned int first = 1 ;
	unsigned int indent = 0;
	int howmany = 0;
	int process_indent(char* line) ;
	unsigned int white_count(char* line);
	int flag = 0;


ID [a-zA-Z_][a-zA-Z0-9_]*


%{
#include <stdio.h>
#include "ast.h"
#include "parser.hpp"
#include <string.h>
%}


%%

\n 			{nl++;}
\r 				;
[ \t]			;
^[ ]*\n       {/* Ignore blank lines. */}
^[ ]*[^ \n]+    {
                    char* yynew;
                    int last = yyleng - 1;
                    int indent = white_count(yytext);
                    int counter = process_indent(yytext) ;
                    yynew = strdup(yytext);
                    while ((last >= 0) && (yynew[last] != ' ')) {
                	       unput(yynew[last]);
                           last-- ;
                    }
                    if(counter !=0)
                        howmany = counter;
                    //printf("howmanyother %d \n",howmany);

                    while(counter > 0) {
                	       unput('}');
                	       counter--;
                    }

                }

[0-9][0-9]* 		{
						printf("Integer %s\n",yytext);
						yylval.const_val = atoi(yytext);
						return(CONST);
					}

if			{indent_stack[++level] = indent;printf("Keyword: if\n");return(IF);}
else		{indent_stack[++level] = indent;printf("Keyword: else\n");return(ELSE);}
elif		{indent_stack[++level] = indent;printf("Keyword: elif\n");return(ELIF);}
loop		{indent_stack[++level] = indent;printf("Keyword: loop\n");return(LOOP);}
int			{printf("Keyword: int\n");return(INT);}
byte		{printf("Keyword: byte\n");return(BYTE);}
as			{printf("Keyword: as\n");return(AS);}
skip		{printf("Keyword: skip\n");return(SKIP);}
decl		{printf("Keyword: decl\n");return(DECL);}
begin		{printf("Keyword: begin\n");return(BEG);}
"}"			{
				//printf("howmany %d\n",howmany);
				if(howmany>0){
					printf("Keyword: end\n");
					howmany--;
					return(END);
				}
				else{
					printf("Unexpected Character }");
					exit(-1);
				}
			}
def			{indent_stack[++level] = indent;printf("Found def with indent %d\n",indent);printf("Keyword: def\n");return(DEF);}
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

{ID} 			{
					printf("Identifier: %s\n",yytext);
					yylval.idstring = strdup(yytext);
					return(IDENTIFIER);
				}
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
":="		{printf("Assignment Operator :=\n");return(ASSIGNMENT);}
"=="		{printf("Equality operator ==\n");return(EQ);}
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

"\""			{printf("Unclosed String Error\n");exit(-1);}


"#".*            	{printf("comment found\n");}

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
\"(\\.|[^\\"])*\" { //"{
                            printf("String Literal %s \n",yytext);
						    yylval.idstring = strdup(yytext);
						    return(STRINGLITERAL);
					    }
<<EOF>>             {
                        unput(0);
                        howmany=level;
                        //printf("the level is %d\n",level);
                        while(level>0){
                            unput('}');
                            level--;
                        }
                        //printf("howmanyeof %d \n",howmany);

                        printf("Read %d lines\n",nl);
				}

.           {
				if(yytext[0]==0) {
					return 0;
				}
				printf("Unexpected Character %d\n",yytext[0]);
				exit(-1);
			}





%%
unsigned int white_count(char* line) {
  unsigned int count = 0 ;
  while (*line == ' ')
    count++, line++ ;
  return count ;
}

int process_indent(char* line) {
	indent = white_count(line) ;

	if (indent == 0) return 0;

	int i = 0;
	//printf("indent:%d indent_stack:%d level%d\n",indent,indent_stack[level],level);
  	while (indent <= indent_stack[level]) {
    	--level ;
   		i++;
    	if(level == -1) break;
	}
	return i;
}
/*
int main(int argc, char const *argv[])
{
	//indent_stack[0]=0;
	int token;
	while(yylex());
	printf("Level is %d\n",level);
	/*
	while(level>0){
		printf("}\n");
		level--;
	}

	return 0;
}*/

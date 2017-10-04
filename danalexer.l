
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
HEX [0-9|A-F|a-f]
ESCAPE \\(n|t|r|0|\\|\'|\"|x{HEX}{HEX})


%{
	//'
#include <stdio.h>
#include "ast.h"
#include "parser.hpp"
#include <string.h>
char* escapeStr(char *s);
int fixChar (char *str, int *shift);
int fixHex (char *str);
int charToInt (char);

%}


%%

\n	{nl++;}
\r 	;
[ \t]	;
^[ ]*\n         {/* Ignore blank lines. */}
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

[0-9][0-9]* 	{
    //printf("Integer %s\n",yytext);
    yylval.const_val = atoi(yytext);
    return(CONST);
}

if			{indent_stack[++level] = indent;return(IF);}
else		{indent_stack[++level] = indent;return(ELSE);}
elif		{indent_stack[++level] = indent;return(ELIF);}
loop		{indent_stack[++level] = indent;return(LOOP);}
int			{return(INT);}
byte		{return(BYTE);}
as			{return(AS);}
skip		{return(SKIP);}
decl		{return(DECL);}

"}"		    {
    //printf("howmany %d\n",howmany);
    if(howmany>0){
        howmany--;
        return(END);
    } else {
        printf("Unexpected Character }");
        exit(-1);
    }
}
def			{indent_stack[++level] = indent;return(DEF);}
is			{return(IS);}
var			{return(VAR);}
not			{return(NOT);}
and			{return(AND);}
or 			{return(OR);}
true		{return(TRUE);}
false		{return(FALSE);}
break		{return(BREAK);}
continue	{return(CONT);}
exit		{return(EXIT);}
return		{return(RETURN);}
ref 		{return(REF);}

{ID} 		{
    //printf("Identifier: %s\n",yytext);
    yylval.idstring = strdup(yytext);
    return(IDENTIFIER);
}
"("			{return('(');}
")"			{return(')');}
"["			{return('[');}
"]"			{return(']');}

"."			{return('.');}
","			{return(',');}
"+"			{return('+');}
"-"			{return('-');}
"*"			{return('*');}
"/"			{return('/');}
":="		{return(ASSIGNMENT);}
"="			{return(EQ);}
"!"			{return('!');}
[%]			{return('%');}
"&" 		{return('&');}
"|"			{return('|');}
"<"			{return(SMALLER);}
"<="		{return(SMALLEREQ);}
">="		{return(LARGEREQ);}
"<>"		{return(DIFF);}
">"			{return(LARGER);}
":"			{return(':');}
"\""		{printf("Unclosed String Error\n");exit(-1);}
\"([^\n\"\'\\]|{ESCAPE})*\" {
    //printf("String Literal %s \n",yytext);
    //printf("String literal %s \n" , yytext);
    //printf("Fixed %s",escapeStr(yytext));
    yylval.idstring = strdup(escapeStr(yytext));
    return(STRINGLITERAL);
}
\'([^\"\'\\]|{ESCAPE})\'    {
 				//"
				yylval.const_val = fixChar(yytext+1,NULL);
				return CHAR_CONST;
			    }

"#".*       {;}

"(*"        { ++comment_nesting; BEGIN(COMMENT); }

<COMMENT>{
  "(*"           	{ ++comment_nesting; }
  "*"+")"        	{ --comment_nesting; if(!comment_nesting) BEGIN(INITIAL);}
  "*"+           	;
  [^(*\n]+       	;
  [(]            	;
  "\n"             	; {nl++;}
  <<EOF>>        ; 	{if (comment_nesting>0) printf("error: unclosed comment in line %d\n",nl); return -1;}
}





<<EOF>>     {
    unput(0);
    howmany=level;
    while(level>0){
        unput('}');
        level--;
    }
    //printf("Read %d lines\n",nl);
    //printf("Syntax analysis is Done!\n");
}

.       {
    if(yytext[0]==0) {
        return 0;
    }
    printf("Unexpected Character %d\n",yytext[0]);
    printf("Line no: %d \n",nl);
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

int fixChar(char * str, int *shift)
{
	int dummy;
	if(shift==NULL) shift=&dummy;

	*shift=1;
	if(str[0]!='\\') return str[0];

	*shift=2;
	switch(str[1]){
		case 'n': return '\n';
		case 't': return '\t';
		case 'r': return '\r';
		case '0': return '\0';
		case '\\': return '\\';
		case '\'': return '\'';
		case '\"': return '\'';
		case 'x': {*shift=4;  return fixHex(str+2); }
		default : {printf("lexer: escape char, unmatched case"); exit(1);}
	}
}

int fixHex(char * str)
{
	int d1,d2,hex;
	d1=charToInt(str[0]);
	d2=charToInt(str[1]);
	hex = d1*16 + d2;
	return hex;
}

int charToInt(char c)
{
	if(c>='0' && c<='9')
		return c-'0';
	else if(c>='a' && c<='f')
		return c-'a'+10;
	else if(c>='A' && c<='F')
		return c-'A'+10;
	else
		printf("lexer: %s not a proper char expression",yytext);
	return -1;
}
char* escapeStr(char *s) {
    int len = strlen(s);
    char c;
    char* result;
    result = (char *)malloc(len*sizeof(char));
    int j = 0;
    for (int i = 0; i < len; ++i){
        c = s[i];
        if(c=='\\'){
            if(s[i+1] == 'x') {
                c = fixChar(s+i,NULL);
                i+=3;
            } else {
                c = fixChar(s+i,NULL);
                i++;
            }

        }
        result[j] = c;
        j++;
    }
    return result;
}
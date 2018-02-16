
%x COMMENT

	int nl = 1;
	int comment_nesting = 0;
	#define MAX_DEPTH 72
    #define T_eof 0





ID [a-zA-Z_][a-zA-Z0-9_]*
HEX [0-9|A-F|a-f]
ESCAPE \\(n|t|r|0|\\|\'|\"|x{HEX}{HEX})


%{
	//'
#include <stdio.h>
#include "ast.h"
#include "blockparser.hpp"
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

[0-9][0-9]* 	{
    //printf("Integer %s\n",yytext);
    yylval.const_val = atoi(yytext);
    return(CONST);
}

if			{return(IF);}
else		{return(ELSE);}
elif		{return(ELIF);}
loop		{return(LOOP);}
int			{return(INT);}
byte		{return(BYTE);}
as			{return(AS);}
skip		{return(SKIP);}
decl		{return(DECL);}
begin		{return(BEG);}
end         {return(END);}
def			{return(DEF);}
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





.       {
    printf("Unexpected Character %d\n",yytext[0]);
    printf("Line no: %d \n",nl);
    exit(-1);
}





%%

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
    result[len]='\0';
    return result;
}
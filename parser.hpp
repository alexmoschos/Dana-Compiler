/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    BYTE = 259,
    IF = 260,
    ELSE = 261,
    LOOP = 262,
    AS = 263,
    SKIP = 264,
    DECL = 265,
    DEF = 266,
    END = 267,
    VAR = 268,
    IS = 269,
    TEOF = 270,
    EQ = 271,
    DIFF = 272,
    LARGER = 273,
    SMALLER = 274,
    RETURN = 275,
    EXIT = 276,
    REF = 277,
    LARGEREQ = 278,
    SMALLEREQ = 279,
    ASSIGNMENT = 280,
    AND = 281,
    NOT = 282,
    OR = 283,
    ELIF = 284,
    TRUE = 285,
    FALSE = 286,
    BREAK = 287,
    CONT = 288,
    BEG = 289,
    CONST = 290,
    CHAR_CONST = 291,
    IDENTIFIER = 292,
    STRINGLITERAL = 293,
    UNARYPL = 294,
    UNARYMINUS = 295,
    BANG = 296
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 30 "parser.y" /* yacc.c:1909  */

	ASTfdef   *func;
	ASTExpr   *expr;
	ASTstmt   *statement;
    ASTparam  *parameter;
    ASTheader *head;
    ASTlval   *lvalue;
    ASTfcall  *funccall;
    ASTif     *ifp;
	int   const_val;
	char  *idstring;
	Type  var_type;
    vector<string> *list;

#line 111 "parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */

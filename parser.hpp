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
    STRINGLITERAL = 262,
    LOOP = 263,
    AS = 264,
    SKIP = 265,
    DECL = 266,
    DEF = 267,
    END = 268,
    VAR = 269,
    IS = 270,
    TEOF = 271,
    EQ = 272,
    DIFF = 273,
    LARGER = 274,
    SMALLER = 275,
    RETURN = 276,
    EXIT = 277,
    REF = 278,
    LARGEREQ = 279,
    SMALLEREQ = 280,
    ASSIGNMENT = 281,
    AND = 282,
    NOT = 283,
    OR = 284,
    ELIF = 285,
    TRUE = 286,
    FALSE = 287,
    BREAK = 288,
    CONT = 289,
    BEG = 290,
    CONST = 291,
    IDENTIFIER = 292,
    UNARYPL = 293,
    UNARYMINUS = 294,
    BANGBANG = 295
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 15 "parser.y" /* yacc.c:1909  */

	ASTfdef* func;
	ASTExpr* expr;
	ASTstmt* statement;
	int const_val;
	char* idstring;

#line 103 "parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */

/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

	extern "C"
	{
        #include "symbol.h"
        }
        #include "sem.h"
	#include "ast.h"
	#include <stdio.h>
	#include <string>
	#include <iostream>
	#include <vector>

	using namespace std;
	extern int nl;
        vector<ASTExpr*> *lastparam;
        vector<string> *identifiers;
 	int yylex(void);
        int i = 0;
        ASTfdef* main_f;
	void yyerror (char const *s) {
            fprintf (stderr, "Syntax error on line %d %s\n",nl, s);
	}

#line 90 "parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.hpp".  */
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
#line 24 "parser.y" /* yacc.c:355  */

	ASTfdef* func;
	ASTExpr* expr;
	ASTstmt* statement;
        ASTparam* parameter;
        ASTheader* head;
        ASTlval* lvalue;
	int const_val;
	char* idstring;
        ASTfcall* funccall;
	Type var_type;
        vector<string>* list;
        ASTif* ifp;

#line 187 "parser.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 204 "parser.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   248

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  86
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  171

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    54,     2,     2,     2,     2,    44,     2,
      52,    53,    42,    39,    49,    40,     2,    43,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    48,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    50,     2,    51,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    41,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    45,    46,    47
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    73,    73,    76,    80,    84,    85,    86,    87,    91,
      97,   106,   107,   108,   109,   110,   114,   115,   116,   117,
     118,   121,   122,   131,   134,   137,   138,   141,   146,   150,
     151,   155,   156,   161,   165,   169,   170,   175,   176,   177,
     178,   179,   180,   184,   185,   189,   197,   198,   202,   208,
     214,   215,   219,   225,   231,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   252,   253,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   274,   275,   276
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "BYTE", "IF", "ELSE", "LOOP",
  "AS", "SKIP", "DECL", "DEF", "END", "VAR", "IS", "TEOF", "EQ", "DIFF",
  "LARGER", "SMALLER", "RETURN", "EXIT", "REF", "LARGEREQ", "SMALLEREQ",
  "ASSIGNMENT", "AND", "NOT", "OR", "ELIF", "TRUE", "FALSE", "BREAK",
  "CONT", "BEG", "CONST", "CHAR_CONST", "IDENTIFIER", "STRINGLITERAL",
  "'+'", "'-'", "'|'", "'*'", "'/'", "'&'", "UNARYPL", "UNARYMINUS",
  "BANG", "':'", "','", "'['", "']'", "'('", "')'", "'!'", "$accept",
  "mainfunc", "fdef", "fdecl", "header", "optparam", "ftype", "bp",
  "stmt_list", "type", "type_term", "pc", "param", "stmt", "fcall",
  "idlist", "loop", "mif", "condition", "eliftstmt", "expression", "lval", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,    43,
      45,   124,    42,    47,    38,   294,   295,   296,    58,    44,
      91,    93,    40,    41,    33
};
# endif

#define YYPACT_NINF -58

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-58)))

#define YYTABLE_NINF -85

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       7,    19,    28,   -58,   -10,   158,   -58,    55,    24,   145,
      12,   -58,    19,    24,    39,   -58,    43,    48,     0,   -58,
     -58,   -58,    98,   -58,   158,   -58,   -58,   -58,   -13,   -58,
     -58,    38,   -58,   -58,   -58,     8,   145,   -58,   -58,   -58,
     -58,    59,   171,   171,   145,   171,   -58,    29,    96,    31,
      79,   158,   -58,    -1,   171,    81,    92,   171,   165,   -58,
     -58,   171,   171,    24,    95,    70,   -58,   -58,   171,   -58,
     -58,    -2,    -9,   -58,   145,   145,   158,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   158,
     120,    55,   204,   -58,   -58,   -58,   193,   -58,    89,   204,
      82,   -58,    94,    93,    97,    90,   104,   105,   187,   -58,
     -58,   -58,   128,   134,   204,   204,   204,   204,   204,   204,
     -21,   -21,   -21,   -58,   -58,   -58,   144,   -58,   107,   171,
     -58,   -58,   -58,    -8,    18,   -58,   -58,    24,   125,    11,
     -58,   -58,   113,   -58,   115,   -58,   -58,   119,   126,   145,
     -58,   -58,   -58,   158,    42,   161,   158,   -58,   174,    33,
     129,   145,   -58,   158,    69,   175,   158,   -58,   176,    33,
     -58
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     8,     0,     1,     0,     0,     0,
       0,    31,     0,     0,     0,    43,    38,    40,    28,    85,
      34,    42,     0,    32,    21,    45,    35,    33,     0,    25,
      26,     6,    24,    48,     7,     0,     0,    61,    62,    80,
      83,    84,     0,     0,     0,     0,    82,     0,    67,    79,
       0,     0,     4,     0,     0,     0,     0,     0,     0,     3,
      22,     0,     0,     0,     0,     0,    49,    65,     0,    76,
      77,     0,    67,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    44,    39,    41,    27,    30,    47,     0,    36,
       0,     5,     0,    14,    15,     0,     9,    13,     0,    66,
      81,    63,    64,     0,    59,    60,    55,    56,    57,    58,
      70,    71,    75,    72,    73,    74,     0,    51,    37,     0,
      46,    86,    23,     0,     0,    11,    12,     0,     0,    54,
      50,    29,     0,    16,     0,    17,    10,     0,     0,     0,
      18,    19,    20,     0,     0,     0,     0,    52,     0,     0,
       0,     0,    53,     0,     0,     0,     0,    68,     0,     0,
      69
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -58,   -58,   189,   -58,   180,   -57,   -58,   -58,   -22,   102,
     -58,   -58,   -53,   -58,    -5,   181,   -58,   -58,   -33,    44,
      21,    -4
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    20,    21,     5,    34,   106,   107,    22,    31,
      32,    23,    95,    24,    46,    35,    26,    27,    47,   162,
      48,    49
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,    28,    60,    67,     7,    98,   101,    77,    78,    79,
      80,    71,    61,    91,    81,    82,    65,   148,     1,    25,
      28,    86,    87,    88,    74,   -84,    75,   142,     6,    90,
      83,    84,    85,    86,    87,    88,    66,    62,     8,   160,
     149,   111,   112,   143,   110,    66,    25,    28,    57,    50,
     -84,   109,    58,   144,   113,    74,     4,    75,    29,    30,
      51,    33,   161,    69,    70,    72,    73,   126,    74,   145,
      75,    25,    28,   103,   104,    92,   141,    76,    96,    96,
     146,    62,    99,   100,    25,    28,    63,    54,    64,   108,
     156,    55,   105,   135,   136,    74,    56,    75,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      59,    58,    77,    78,    79,    80,   154,   166,    93,    81,
      82,    83,    84,    85,    86,    87,    88,    89,   164,    94,
     102,   155,   127,   131,   158,    83,    84,    85,    86,    87,
      88,   165,   130,   133,   168,   132,   139,   134,    25,    28,
      96,    25,    28,   137,    74,   138,   140,    64,    25,    28,
     147,    25,    28,     9,   150,    10,   151,    11,    12,     1,
     152,    13,    36,   157,   153,    37,    38,   163,    14,    15,
      39,    40,    41,    19,    42,    43,   159,   167,   169,     3,
      16,    17,    52,   128,    53,    18,    19,    44,     0,    45,
      39,    40,    41,    19,    42,    43,    39,    40,    41,    19,
      42,    43,     0,   170,     0,     0,     0,    68,    97,    45,
       0,     0,     0,    68,     0,    45,    83,    84,    85,    86,
      87,    88,    83,    84,    85,    86,    87,    88,     0,     0,
     110,     0,   129,    83,    84,    85,    86,    87,    88
};

static const yytype_int16 yycheck[] =
{
       5,     5,    24,    36,    14,    58,    63,    16,    17,    18,
      19,    44,    25,    14,    23,    24,     8,     6,    11,    24,
      24,    42,    43,    44,    26,    25,    28,    35,     0,    51,
      39,    40,    41,    42,    43,    44,    37,    50,    48,     6,
      29,    74,    75,    51,    53,    37,    51,    51,    48,    37,
      50,    53,    52,    35,    76,    26,    37,    28,     3,     4,
      48,    37,    29,    42,    43,    44,    45,    89,    26,    51,
      28,    76,    76,     3,     4,    54,   129,    48,    57,    58,
     137,    50,    61,    62,    89,    89,    48,    48,    50,    68,
      48,    48,    22,     3,     4,    26,    48,    28,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      12,    52,    16,    17,    18,    19,   149,    48,    37,    23,
      24,    39,    40,    41,    42,    43,    44,    48,   161,    37,
      35,   153,    12,    51,   156,    39,    40,    41,    42,    43,
      44,   163,    53,    50,   166,    51,    12,    50,   153,   153,
     129,   156,   156,    49,    26,    50,    12,    50,   163,   163,
      35,   166,   166,     5,    51,     7,    51,     9,    10,    11,
      51,    13,    27,    12,    48,    30,    31,    48,    20,    21,
      35,    36,    37,    38,    39,    40,    12,    12,    12,     0,
      32,    33,    12,    91,    13,    37,    38,    52,    -1,    54,
      35,    36,    37,    38,    39,    40,    35,    36,    37,    38,
      39,    40,    -1,   169,    -1,    -1,    -1,    52,    53,    54,
      -1,    -1,    -1,    52,    -1,    54,    39,    40,    41,    42,
      43,    44,    39,    40,    41,    42,    43,    44,    -1,    -1,
      53,    -1,    49,    39,    40,    41,    42,    43,    44
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    11,    56,    57,    37,    59,     0,    14,    48,     5,
       7,     9,    10,    13,    20,    21,    32,    33,    37,    38,
      57,    58,    63,    66,    68,    69,    71,    72,    76,     3,
       4,    64,    65,    37,    60,    70,    27,    30,    31,    35,
      36,    37,    39,    40,    52,    54,    69,    73,    75,    76,
      37,    48,    59,    70,    48,    48,    48,    48,    52,    12,
      63,    25,    50,    48,    50,     8,    37,    73,    52,    75,
      75,    73,    75,    75,    26,    28,    48,    16,    17,    18,
      19,    23,    24,    39,    40,    41,    42,    43,    44,    48,
      63,    14,    75,    37,    37,    67,    75,    53,    67,    75,
      75,    60,    35,     3,     4,    22,    61,    62,    75,    53,
      53,    73,    73,    63,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    63,    12,    64,    49,
      53,    51,    51,    50,    50,     3,     4,    49,    50,    12,
      12,    67,    35,    51,    35,    51,    60,    35,     6,    29,
      51,    51,    51,    48,    73,    63,    48,    12,    63,    12,
       6,    29,    74,    48,    73,    63,    48,    12,    63,    12,
      74
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    55,    56,    57,    58,    59,    59,    59,    59,    60,
      60,    61,    61,    61,    61,    61,    62,    62,    62,    62,
      62,    63,    63,    64,    64,    65,    65,    66,    66,    67,
      67,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    69,    69,    70,    70,
      71,    71,    72,    72,    72,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    74,    74,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    76,    76,    76
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     4,     2,     5,     3,     3,     1,     3,
       5,     2,     2,     1,     1,     1,     3,     3,     4,     4,
       4,     1,     2,     4,     1,     1,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     3,     4,     1,     3,
       1,     3,     1,     1,     3,     1,     4,     3,     1,     2,
       5,     4,     9,    11,     5,     3,     3,     3,     3,     3,
       3,     1,     1,     3,     3,     2,     3,     1,     4,     6,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     1,
       1,     3,     1,     1,     1,     1,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 73 "parser.y" /* yacc.c:1646  */
    {(yyval.func) = (yyvsp[0].func);main_f = (yyvsp[0].func);}
#line 1413 "parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 76 "parser.y" /* yacc.c:1646  */
    {(yyval.func) = new ASTfdef((yyvsp[-2].head),(yyvsp[-1].statement));}
#line 1419 "parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 80 "parser.y" /* yacc.c:1646  */
    {(yyval.func) = new ASTfdef((yyvsp[0].head),NULL);}
#line 1425 "parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 84 "parser.y" /* yacc.c:1646  */
    {(yyval.head) = new ASTheader((yyvsp[-2].var_type),(yyvsp[0].parameter),(yyvsp[-4].idstring));}
#line 1431 "parser.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 85 "parser.y" /* yacc.c:1646  */
    {(yyval.head) = new ASTheader((yyvsp[0].var_type),NULL,(yyvsp[-2].idstring));}
#line 1437 "parser.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 86 "parser.y" /* yacc.c:1646  */
    {(yyval.head) = new ASTheader(typeVoid,(yyvsp[0].parameter),(yyvsp[-2].idstring));}
#line 1443 "parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 87 "parser.y" /* yacc.c:1646  */
    {(yyval.head) = new ASTheader(typeVoid,NULL,(yyvsp[0].idstring));}
#line 1449 "parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 91 "parser.y" /* yacc.c:1646  */
    {
                                           (yyval.parameter) = new ASTparam((yyvsp[-2].list),(yyvsp[0].var_type),NULL);
                                           //cout << endl;
                                           //printType($3);
                                           //cout << endl;
                                       }
#line 1460 "parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 97 "parser.y" /* yacc.c:1646  */
    {
		                           (yyval.parameter) = new ASTparam((yyvsp[-4].list),(yyvsp[-2].var_type),(yyvsp[0].parameter));
		                           //cout << endl;
                                           //printType($3);
                                           //cout << endl;
	                               }
#line 1471 "parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 106 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = typeInteger;}
#line 1477 "parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 107 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = typeChar;}
#line 1483 "parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 108 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = (yyvsp[0].var_type);}
#line 1489 "parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 109 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = typeInteger;}
#line 1495 "parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 110 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = typeChar;}
#line 1501 "parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 114 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = typeIArray(typeInteger);}
#line 1507 "parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 115 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = typeIArray(typeChar);}
#line 1513 "parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 116 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = typeArray((yyvsp[-1].const_val),typeInteger);}
#line 1519 "parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 117 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = typeArray((yyvsp[-1].const_val),typeChar);}
#line 1525 "parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 118 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = typeArray((yyvsp[-1].const_val),(yyvsp[-3].var_type));}
#line 1531 "parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 121 "parser.y" /* yacc.c:1646  */
    {(yyval.statement) = (yyvsp[0].statement);}
#line 1537 "parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 122 "parser.y" /* yacc.c:1646  */
    {
                            (yyvsp[-1].statement)->tail=(yyvsp[0].statement);
                            (yyval.statement) = (yyvsp[-1].statement);
                        }
#line 1546 "parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 131 "parser.y" /* yacc.c:1646  */
    {
			        (yyval.var_type) = typeArray((yyvsp[-1].const_val),(yyvsp[-3].var_type));
		             }
#line 1554 "parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 134 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = (yyvsp[0].var_type);}
#line 1560 "parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 137 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = typeInteger;}
#line 1566 "parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 138 "parser.y" /* yacc.c:1646  */
    {(yyval.var_type) = typeChar;}
#line 1572 "parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 141 "parser.y" /* yacc.c:1646  */
    {
                                       (yyval.funccall) = new ASTfcall((yyvsp[-2].idstring));
                                       (yyval.funccall)->parameters = lastparam;
                                       lastparam = new vector<ASTExpr*>();
			           }
#line 1582 "parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 146 "parser.y" /* yacc.c:1646  */
    {(yyval.funccall) = new ASTfcall((yyvsp[0].idstring)); (yyval.funccall)->parameters = NULL;}
#line 1588 "parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 150 "parser.y" /* yacc.c:1646  */
    {lastparam->push_back((yyvsp[-2].expr));}
#line 1594 "parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 151 "parser.y" /* yacc.c:1646  */
    {lastparam->push_back((yyvsp[0].expr));}
#line 1600 "parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 155 "parser.y" /* yacc.c:1646  */
    {(yyval.statement) = new ASTstmt(TSKIP,NULL,NULL,"");}
#line 1606 "parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 156 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.statement) = new ASTstmt(TPC,NULL,NULL,"");
                                    (yyval.statement)->expr = new ASTExpr('f',NULL,0,NULL,NULL);
                                    (yyval.statement)->expr->f = (yyvsp[0].funccall);
                                }
#line 1616 "parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 161 "parser.y" /* yacc.c:1646  */
    {
		                        (yyval.statement) = new ASTstmt(TIF,NULL,NULL,"");
		                        (yyval.statement)->ifnode = (yyvsp[0].ifp);
	                        }
#line 1625 "parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 165 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.statement) = new ASTstmt(TFDEF,NULL,NULL,"");
                                    (yyval.statement)->def = (yyvsp[0].func);
                                }
#line 1634 "parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 169 "parser.y" /* yacc.c:1646  */
    {(yyval.statement) = (yyvsp[0].statement);}
#line 1640 "parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 170 "parser.y" /* yacc.c:1646  */
    {
                                       (yyval.statement) = new ASTstmt(TASSIGN,NULL,NULL,"");
                                       (yyval.statement)->expr = (yyvsp[0].expr);
                                       (yyval.statement)->lvalue = (yyvsp[-2].lvalue);
                                   }
#line 1650 "parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 175 "parser.y" /* yacc.c:1646  */
    {(yyval.statement) = new ASTstmt(TDECL,NULL,NULL,"");}
#line 1656 "parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 176 "parser.y" /* yacc.c:1646  */
    {(yyval.statement) = new ASTstmt(TBREAK,NULL,NULL,"");}
#line 1662 "parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 177 "parser.y" /* yacc.c:1646  */
    {(yyval.statement) = new ASTstmt(TBREAKM,NULL,NULL,(yyvsp[0].idstring));}
#line 1668 "parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 178 "parser.y" /* yacc.c:1646  */
    {(yyval.statement) = new ASTstmt(TCONT,NULL,NULL,"");}
#line 1674 "parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 179 "parser.y" /* yacc.c:1646  */
    {(yyval.statement) = new ASTstmt(TCONTM,NULL,NULL,(yyvsp[0].idstring));}
#line 1680 "parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 180 "parser.y" /* yacc.c:1646  */
    {
                                       (yyval.statement) = new ASTstmt(TFDECL,NULL,NULL,"");
                                       (yyval.statement)->def = (yyvsp[0].func);
                                   }
#line 1689 "parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 184 "parser.y" /* yacc.c:1646  */
    {(yyval.statement) = new ASTstmt(TEXIT,NULL,NULL,"");}
#line 1695 "parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 185 "parser.y" /* yacc.c:1646  */
    {
                                       (yyval.statement) = new ASTstmt(TRET,NULL,NULL,"");
                                       (yyval.statement)->expr = (yyvsp[0].expr);
                                   }
#line 1704 "parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 189 "parser.y" /* yacc.c:1646  */
    {
                               (yyval.statement) = new ASTstmt(TFCALL,NULL,NULL,"");
                               (yyval.statement)->expr = new ASTExpr('f',NULL,0,NULL,NULL);
                               (yyval.statement)->expr->f = (yyvsp[0].funccall);
                           }
#line 1714 "parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 197 "parser.y" /* yacc.c:1646  */
    {(yyval.funccall) = new ASTfcall((yyvsp[-3].idstring)); (yyval.funccall)->parameters = lastparam;lastparam = new vector<ASTExpr*>();}
#line 1720 "parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 198 "parser.y" /* yacc.c:1646  */
    {(yyval.funccall) = new ASTfcall((yyvsp[-2].idstring)); (yyval.funccall)->parameters = NULL;}
#line 1726 "parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 202 "parser.y" /* yacc.c:1646  */
    {
                                //cout << "first" << endl;
                                identifiers = new vector<string>();
                                identifiers->push_back((yyvsp[0].idstring));
                                (yyval.list) = identifiers;
                            }
#line 1737 "parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 208 "parser.y" /* yacc.c:1646  */
    {
                                //cout << "second" << endl;
                                identifiers->push_back((yyvsp[0].idstring));
                            }
#line 1746 "parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 214 "parser.y" /* yacc.c:1646  */
    {(yyval.statement) = new ASTstmt(TLOOP,(yyvsp[-1].statement),NULL,(yyvsp[-3].idstring));}
#line 1752 "parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 215 "parser.y" /* yacc.c:1646  */
    {(yyval.statement) = new ASTstmt(TLOOP,(yyvsp[-1].statement),NULL,"");}
#line 1758 "parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 219 "parser.y" /* yacc.c:1646  */
    {
		                                                            (yyval.ifp) = new ASTif((yyvsp[-7].expr),(yyvsp[-5].statement));
		                                                            auto else_node = new ASTif(NULL,(yyvsp[-1].statement));
		                                                            (yyval.ifp)->tail = else_node;
		                                                            else_node->tail = NULL;
	                                                                }
#line 1769 "parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 225 "parser.y" /* yacc.c:1646  */
    {
		       		                                                          (yyval.ifp) = new ASTif((yyvsp[-9].expr),(yyvsp[-7].statement));
		                                                                          auto else_node = new ASTif((yyvsp[-4].expr),(yyvsp[-2].statement));
		                                                                          (yyval.ifp)->tail = else_node;
		                                                                          else_node->tail = (yyvsp[0].ifp);
	                                                                              }
#line 1780 "parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 231 "parser.y" /* yacc.c:1646  */
    {(yyval.ifp) = new ASTif((yyvsp[-3].expr),(yyvsp[-1].statement));}
#line 1786 "parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 235 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('>',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1792 "parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 236 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('<',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1798 "parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 237 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('l',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1804 "parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 238 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('s',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1810 "parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 239 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('e',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1816 "parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 240 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('d',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1822 "parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 241 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('b',NULL,1,NULL,NULL);}
#line 1828 "parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 242 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('b',NULL,0,NULL,NULL);}
#line 1834 "parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 243 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('a',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1840 "parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 244 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('o',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1846 "parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 245 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('n',NULL,0,NULL,(yyvsp[0].expr));}
#line 1852 "parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 246 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = (yyvsp[-1].expr);}
#line 1858 "parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 247 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = (yyvsp[0].expr);}
#line 1864 "parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 252 "parser.y" /* yacc.c:1646  */
    {(yyval.ifp) = new ASTif(NULL,(yyvsp[-1].statement));}
#line 1870 "parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 253 "parser.y" /* yacc.c:1646  */
    {(yyval.ifp) = new ASTif((yyvsp[-4].expr),(yyvsp[-2].statement));(yyval.ifp)->tail=(yyvsp[0].ifp);}
#line 1876 "parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 257 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('+',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1882 "parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 258 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('-',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1888 "parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 259 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('*',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1894 "parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 260 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('/',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1900 "parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 261 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('&',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1906 "parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 262 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('|',NULL,0,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1912 "parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 263 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('+',NULL,0,NULL,(yyvsp[0].expr));}
#line 1918 "parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 264 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('-',NULL,0,NULL,(yyvsp[0].expr));}
#line 1924 "parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 265 "parser.y" /* yacc.c:1646  */
    {(yyval.expr)=new ASTExpr('!',NULL,0,NULL,(yyvsp[0].expr));}
#line 1930 "parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 266 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new ASTExpr('i',(yyvsp[0].lvalue),0,NULL,NULL);}
#line 1936 "parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 267 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new ASTExpr('c',NULL,(yyvsp[0].const_val),NULL,NULL);}
#line 1942 "parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 268 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = (yyvsp[-1].expr);}
#line 1948 "parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 269 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new ASTExpr('f',NULL,0,NULL,NULL); (yyval.expr)->f = (yyvsp[0].funccall);}
#line 1954 "parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 270 "parser.y" /* yacc.c:1646  */
    {(yyval.expr) = new ASTExpr('x',NULL,(yyvsp[0].const_val),NULL,NULL);}
#line 1960 "parser.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 274 "parser.y" /* yacc.c:1646  */
    {(yyval.lvalue) = new ASTlval(false,(yyvsp[0].idstring));}
#line 1966 "parser.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 275 "parser.y" /* yacc.c:1646  */
    {(yyval.lvalue) = new ASTlval(true,(yyvsp[0].idstring));}
#line 1972 "parser.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 276 "parser.y" /* yacc.c:1646  */
    {
                                        (yyvsp[-3].lvalue)->indices->push_back((yyvsp[-1].expr));
                                        (yyval.lvalue)=(yyvsp[-3].lvalue);
                                        //cout << $$->identifier << endl;
                                        //$$->print();
                                   }
#line 1983 "parser.cpp" /* yacc.c:1646  */
    break;


#line 1987 "parser.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 283 "parser.y" /* yacc.c:1906  */



int main(){
	cout << "Parser Version 0.0.1.00" << endl;
	initSymbolTable(997);
        main_f = NULL;
	lastparam = new vector<ASTExpr*>();
	if(yyparse()) return -1;

        sem_check_fdef(main_f);
}

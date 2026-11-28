/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_PGAA_YY_SRC_PARSER_TAB_H_INCLUDED
# define YY_PGAA_YY_SRC_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef PGAA_YYDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define PGAA_YYDEBUG 1
#  else
#   define PGAA_YYDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define PGAA_YYDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined PGAA_YYDEBUG */
#if PGAA_YYDEBUG
extern int pgaa_yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "src/parser.y"

    #include "parser_util.h"

#line 61 "src/parser.tab.h"

/* Token kinds.  */
#ifndef PGAA_YYTOKENTYPE
# define PGAA_YYTOKENTYPE
  enum pgaa_yytokentype
  {
    PGAA_YYEMPTY = -2,
    PGAA_YYEOF = 0,                /* "end of file"  */
    PGAA_YYerror = 256,            /* error  */
    PGAA_YYUNDEF = 257,            /* "invalid token"  */
    VARIABLE = 258,                /* VARIABLE  */
    NUMBER = 259,                  /* NUMBER  */
    STRING = 260,                  /* STRING  */
    EQ = 261,                      /* EQ  */
    NEQ = 262,                     /* NEQ  */
    SEQ = 263,                     /* SEQ  */
    GR = 264,                      /* GR  */
    GRE = 265,                     /* GRE  */
    LE = 266,                      /* LE  */
    LEE = 267,                     /* LEE  */
    MOD = 268                      /* MOD  */
  };
  typedef enum pgaa_yytokentype pgaa_yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined PGAA_YYSTYPE && ! defined PGAA_YYSTYPE_IS_DECLARED
union PGAA_YYSTYPE
{
#line 20 "src/parser.y"

    PGAA_Token tok;
    PGAA_ASTNode *node;

#line 96 "src/parser.tab.h"

};
typedef union PGAA_YYSTYPE PGAA_YYSTYPE;
# define PGAA_YYSTYPE_IS_TRIVIAL 1
# define PGAA_YYSTYPE_IS_DECLARED 1
#endif




int pgaa_yyparse (PGAA_ParserCtx *ctx);


#endif /* !YY_PGAA_YY_SRC_PARSER_TAB_H_INCLUDED  */

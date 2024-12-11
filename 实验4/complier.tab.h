
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NE = 258,
     BE = 259,
     LE = 260,
     COMMA = 261,
     NOT = 262,
     BT = 263,
     LT = 264,
     EQ = 265,
     ASSIGN = 266,
     SEMICOLON = 267,
     PLUS = 268,
     MINUS = 269,
     MULT = 270,
     DIV = 271,
     PERCENT = 272,
     LP = 273,
     RP = 274,
     LS = 275,
     RS = 276,
     LB = 277,
     RB = 278,
     CONST = 279,
     INT = 280,
     FLOAT = 281,
     VOID = 282,
     IF = 283,
     ELSE = 284,
     WHILE = 285,
     CONTINUE = 286,
     BREAK = 287,
     RETURN = 288,
     INT_CONST = 289,
     FLOAT_CONST = 290,
     ID = 291,
     AND = 292,
     OR = 293
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 10 "complier.y"

    int INTNUM;
    float FLOATNUM;
    



/* Line 1676 of yacc.c  */
#line 98 "complier.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;



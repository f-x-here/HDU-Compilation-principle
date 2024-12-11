%{
    #include <stdio.h>
    #include "complier.tab.h"
%}

%option yylineno
%option noyywrap

WHITESPACE [ \t\n]
OCTAL_CONST 0[0-7]*
DECIMAL_CONST [1-9][0-9]*
FLOAT_CONST [1-9][0-9]*[/.][0-9]*
HEXADECIMAL_CONST (0X|0x)[0-9a-fA-F]+
IDENT [a-zA-Z_][a-zA-Z0-9_]*

%%

{WHITESPACE}    {}

"!="                                {return NE;}
">="                                {return BE;} 
"<="                                {return LE;}
","                                 {return COMMA;}
"!"                                 {return NOT;}
">"                                 {return BT;}
"<"                                 {return LT;}
"=="                                {return EQ;}
"="                                 {return ASSIGN;}
";"                                 {return SEMICOLON;}
"+"                                 {return PLUS;}
"-"                                 {return MINUS;}
"*"                                 {return MULT;}
"/"                                 {return DIV;}
"%"                                 {return PERCENT;}
"("                                 {return LP;}
")"                                 {return RP;}
"["                                 {return LS;}
"]"                                 {return RS;}
"{"                                 {return LB;}
"}"                                 {return RB;}
"||"                                {return OR;}
"&&"                                {return AND;}
"const"                             {return CONST;}
"int"                               {return INT;}
"float"                             {return FLOAT;} 
"void"                              {return VOID;}
"if"                                {return IF;}
"else"                              {return ELSE;}
"while"                             {return WHILE;}
"continue"                          {return CONTINUE;}
"break"                             {return BREAK;} 
"return"                            {return RETURN;}

{OCTAL_CONST}                       {return INT_CONST;}
{HEXADECIMAL_CONST}                 {return INT_CONST;}
{DECIMAL_CONST}                     {return INT_CONST;}
{IDENT}                             {return ID;}
{FLOAT_CONST}                       {return FLOAT_CONST;}

"//".*                              { }
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/] { }
[/][*]                              { perror("fatal_error"); }

.                                   {printf("ERROR: NOT SUPPORT CHARACTER!\n");}

%%
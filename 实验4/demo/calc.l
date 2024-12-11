%{
  #include "calc.tab.h"  // 包含 Bison 生成的头文件
  #include <stdio.h>      // 包含标准输入输出头文件
  void yyerror(const char *s); 
%}

%option noyywrap

EXP ([Ee][+-]?[0-9]+)

%%

"+" |
"-" |
"*" |
"/" |
"|" |
"(" |
")" { return yytext[0]; }

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? {
  yylval.num = atof(yytext);
  return NUMBER;
}

\n { return EOL; }
[ \t] { /* ignore whitespace characters */ }
. { yyerror("invalid input!"); }

%%

%{
  #include <stdio.h>
  int yylex(void);
  void yyerror(char *s);
%}

%union {
  char operator;
  double num;
}

%token <num> NUMBER
%token <operator> EOL
%type <num> exp factor term

%%

calclist: /* empty */
         | calclist exp EOL { printf("= %4.4g\n> ", $2); }
         | calclist EOL { printf("> "); }
         ;

exp: factor
    | exp '+' factor { $$ = $1 + $3; }
    | exp '-' factor { $$ = $1 - $3; }
    ;

factor: term
      | factor '*' term { $$ = $1 * $3; }
      | factor '/' term { $$ = $1 / $3; }
      ;

term: NUMBER { $$ = $1; }
    | '|' term { $$ = $2 < 0 ? -$2 : $2; }
    | '(' exp ')' { $$ = $2; }
    | '-' term { $$ = -$2; }
    ;

%%

void yyerror(char *s)
{
  fprintf(stderr, "%s\n", s);
}

int main()
{
  printf("This is a simple calculator.\n");
  printf("> ");
  yyparse();
  return 0;
}

%{
    #include<stdio.h>
    extern int yylineno;
    extern FILE* yyin;
    int yylex(void);
    int yyerror(char* msg);
    int yyrestart(FILE *f);
%}

%union{
    int INTNUM;
    float FLOATNUM;
    
}

//tokens
%token  NE
        BE
        LE
        COMMA
        NOT
        BT
        LT
        EQ
        ASSIGN
        SEMICOLON
        PLUS
        MINUS
        MULT
        DIV
        PERCENT
        LP
        RP
        LS
        RS
        LB
        RB
        CONST
        VOID
        IF
        ELSE
        WHILE
        CONTINUE
        BREAK
        RETURN
        INT_CONST
        FLOAT_CONST
        ID
        AND
        OR

%token <INTNUM>INT
%token <FLOATNUM>FLOAT


//associations 运算符优先级定义越早出现的优先级越低
%right ASSIGN //赋值优先级最低
%left AND OR //&& ||
%left LT BT LE BE EQ NE //< > <= >= == !=
%left PLUS MINUS MULT DIV PERCENT // + - * / %
%right NOT //"!"
%left LS RS LP RP //括号优先级最高

%nonassoc WHILE ELSE RETURN //表示这些运算符不能在不确定优先级的情况下连续出现

%%

CompUnit        :CompUnit Decl
                |CompUnit FuncDef
                |Decl
                |FuncDef
                ;

Decl            :ConstDecl
                |VarDecl
                ;

ConstDecl       :CONST Type ConstDef ConstDecl_R SEMICOLON
                ;

ConstDecl_R     :ConstDecl_R COMMA ConstDef
                |/*empty*/
                ;



ConstDef        :ID VecDef_R ASSIGN ConstInitVal
                ;

VecDef_R        :VecDef_R LS ConstExp RS
                |/*empty*/
                ;

ConstInitVal    :ConstExp
                |LB ConstInitVal ConstInitVal_R RB
                |LB RB
                ;

ConstInitVal_R  :ConstInitVal_R COMMA ConstInitVal
                |/*empty*/
                ;

VarDecl         :Type VarDef VarDef_R SEMICOLON
                ;

VarDef          :ID VecDef_R
                |ID VecDef_R ASSIGN InitVal

VarDef_R        :VarDef_R COMMA VarDef
                |/*empty*/
                ;

InitVal         :Exp
                |LB InitVal InitVal_R RB
                |LB RB
                ;

InitVal_R       :InitVal_R COMMA InitVal
                |/*empty*/
                ;

FuncDef         :Type ID LP RP Block
                |Type ID LP FuncFParams RP Block
                ;

Type            :VOID
                |INT
                |FLOAT
                ;

FuncFParams     :FuncFParam FuncFParam_R
                ;

FuncFParam_R    :FuncFParam_R COMMA FuncFParam
                |/*empty*/
                ;

FuncFParam      :Type ID
                |Type ID LS RS Vec_R
                ;

Vec_R           :Vec_R LS Exp RS
                |/*empty*/
                ;

Block           :LB BlockItem_R RB
                ;

BlockItem       :Decl
                |Stmt
                ;

BlockItem_R     :BlockItem_R BlockItem
                |/*empty*/
                ;

Stmt            :LVal ASSIGN Exp SEMICOLON
                |Exp SEMICOLON
                |SEMICOLON
                |Block
                |IF LP Cond RP Stmt
                |IF LP Cond RP Stmt ELSE Stmt
                |WHILE LP Cond RP Stmt
                |BREAK SEMICOLON
                |CONTINUE SEMICOLON
                |RETURN SEMICOLON
                |RETURN Exp SEMICOLON

Exp             :AddExp
                ;

Cond            :LOrExp
                ;

LVal            :ID Vec_R
                ;


PrimaryExp      :LP Exp RP
                |LVal
                |NUMBER
                ;

NUMBER          :INT_CONST
                |FLOAT_CONST
                ;

UnaryExp        :PrimaryExp
                |ID LP FuncRParams RP
                |ID LP RP
                |UnaryOp UnaryExp
                ;

UnaryOp         :PLUS
                |MINUS
                |NOT
                ;

FuncRParams     :Exp Exp_R
                ;

Exp_R           :Exp_R COMMA Exp
                |/*empty*/
                ;

MulExp          :UnaryExp 
                |MulExp MULT UnaryExp
                |MulExp DIV UnaryExp
                |MulExp PERCENT UnaryExp
                ;

AddExp          :MulExp
                |AddExp PLUS MulExp
                |AddExp MINUS MulExp
                ;

RelExp          :AddExp
                |RelExp LT AddExp
                |RelExp BT AddExp
                |RelExp LE AddExp
                |RelExp BE AddExp
                ;

EqExp           :RelExp
                |EqExp EQ RelExp
                |EqExp NE RelExp
                ;

LAndExp         :EqExp
                |LAndExp AND EqExp
                ;

LOrExp          :LAndExp
                |LOrExp OR LAndExp
                ;

ConstExp        :AddExp
                ;

%%

int yyerror(char *msg) {
    fprintf(stderr, "Error: %s at line %d\n", msg, yylineno);
    return 0;
}


int main(int argc, char **argv) {
    FILE *file;
    if (argc > 1) {
        file = fopen(argv[1], "r");
        if (!file) {
            perror("Error opening file");
            return 1;
        }
    } else {
        file = stdin;
    }
    if (file == stdin) {
        yyparse();
    } else {
        yyin = file;
        yyparse();
        fclose(file);
    }
    return 0;
}
%code requires {
    #include "parser_util.h"
}

%{
    #include "postgres.h"
    #include "parser_util.h"
    #include "utilfunc.h"
    #include "re2c_adapt.h"
    int pgaa_yylex(PGAA_YYSTYPE *yylval_param, PGAA_ParserCtx *ctx);
    void pgaa_yyerror(PGAA_ParserCtx *ctx, const char *s);
%}

%define api.pure full
%define api.prefix {pgaa_yy}

%parse-param { PGAA_ParserCtx *ctx }
%lex-param { PGAA_ParserCtx *ctx }

%union {
    PGAA_Token tok;
    PGAA_ASTNode *node;
}
%token <tok> VARIABLE
%token <tok> NUMBER
%token <tok> STRING

%token <tok> EQ
%token <tok> NEQ
%token <tok> SEQ
%token <tok> GR
%token <tok> GRE
%token <tok> LE
%token <tok> LEE
%token <tok> MOD

%type <node> predicate
%type <tok> cmp_op
%type <node> operand
%type <tok> mod_op

%start predicate

%%

predicate:
    VARIABLE cmp_op operand {
        PGAA_ASTNode *var = pgaa_create_node($1, NULL, NULL);
        $$ = pgaa_create_node($2, var, $3);
        ctx->result = $$;
    }
    | VARIABLE mod_op operand cmp_op operand {
        PGAA_ASTNode *var = pgaa_create_node($1, NULL, NULL);
        PGAA_ASTNode *op1 = pgaa_create_node($2, var, $3);
        $$ = pgaa_create_node($4, op1, $5);
        ctx->result = $$;
    }
;
cmp_op:
    EQ { $$ = $1; }
    | NEQ { $$ = $1; }
    | SEQ { $$ = $1; }
    | GR { $$ = $1; }
    | GRE { $$ = $1; }
    | LE { $$ = $1; }
    | LEE { $$ = $1; }
;
mod_op:
    MOD { $$ = $1; }
;

operand:
    NUMBER {
        $$ = pgaa_create_node($1, NULL, NULL);
    }
    | STRING {
        $$ = pgaa_create_node($1, NULL, NULL);
    }
;
    

%%
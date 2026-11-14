#include "postgres.h"
#include "parser.tab.h"
#include "parser_util.h"
#include "re2c_adapt.h"

int yylex(YYSTYPE *yylval_param, ParserCtx *ctx) {
    Token t;
    if (yylval_param == NULL || ctx == NULL) {
        //erep
    }
    t = get_next(ctx);
    switch(t.type) {
        case T_VARIABLE:
            yylval_param->tok = t;
            return VARIABLE;
        case T_NUMBER:
            yylval_param->tok = t;
            return NUMBER;
        case T_STRING:
            yylval_param->tok = t;
            return STRING;
        case T_EQ:
            yylval_param->tok = t;
            return EQ;
        case T_SEQ:
            yylval_param->tok = t;
            return SEQ;
        case T_NEQ:
            yylval_param->tok = t;
            return NEQ;
        case T_GR:
            yylval_param->tok = t;
            return GR;
        case T_GRE:
            yylval_param->tok = t;
            return GRE;
        case T_LE:
            yylval_param->tok = t;
            return LE;
        case T_LEE:
            yylval_param->tok = t;
            return LEE;
        case T_MOD:
            yylval_param->tok = t;
            return MOD;
        case T_END:
            return 0;
        default:
            return YYUNDEF;
    }
}

void yyerror(ParserCtx *ctx, const char *message)
{
    ereport(ERROR,
            (errcode(ERRCODE_SYNTAX_ERROR),
             errmsg("predicate parser: %s", message)));
    pg_unreachable();
}
#pragma once
#include "postgres.h"
#include "parser.tab.h"
#include "parser_util.h"

int yylex(YYSTYPE *yylval_param, ParserCtx *ctx);
void yyerror(ParserCtx *ctx, const char *message);
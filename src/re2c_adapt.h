#pragma once
#include "postgres.h"
#include "parser.tab.h"
#include "parser_util.h"

int			pgaa_yylex(PGAA_YYSTYPE * yylval_param, PGAA_ParserCtx * ctx);
void		pgaa_yyerror(PGAA_ParserCtx * ctx, const char *message);

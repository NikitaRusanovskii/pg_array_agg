#pragma once
#include "postgres.h"
#include "predicate.h"

PGAA_ASTNode *pgaa_create_node(PGAA_Token value, PGAA_ASTNode * left, PGAA_ASTNode * right);
int			pgaa_free_ast(PGAA_ASTNode * root);
PGAA_Token	pgaa_get_next(PGAA_ParserCtx * ctx);
PGAA_Token	pgaa_construct_token(PGAA_TokentType type, const char *token, int32 length);

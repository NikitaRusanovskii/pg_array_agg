#pragma once
#include "postgres.h"
#include "predicate.h"

ASTNode    *create_node(Token value, ASTNode *left, ASTNode *right);
int			free_ast(ASTNode *root);
Token		get_next(ParserCtx *ctx);
Token		construct_token(TokenType type, const char *token, int32 lenght);

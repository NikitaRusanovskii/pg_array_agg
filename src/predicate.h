#pragma once
#include "postgres.h"

typedef enum
{
	OP_EQ, //==
	OP_NEQ, //!=
	OP_GR, //>
	OP_LE, //<
	OP_GRE, //>=
	OP_LEE, //<=
	OP_MOD, //%
}			PGAA_Operator;

typedef enum
{
	T_VARIABLE,
	T_NUMBER,
	T_STRING,
	T_EQ, //==
	T_GR, //>
	T_NEQ, //!=
	T_LE, //<
	T_GRE, //>=
	T_LEE, //<=
	T_MOD, //%
	T_SEQ, //=(short equal)
	T_END
}			PGAA_TokentType;

typedef struct PGAA_Token
{
	PGAA_TokentType type;
	const char *token;
	int32		length;
}			PGAA_Token;

typedef struct PGAA_ASTNode
{
	PGAA_Token	value;
	struct PGAA_ASTNode *left,
			   *right;
	Datum		raw;
	bool		is_cached;
}			PGAA_ASTNode;

typedef struct PGAA_ParserCtx
{
	PGAA_Token *tokens;
	size_t		token_len;
	size_t		token_pos;
	char	   *predicate_string;
	PGAA_ASTNode *result;
	MemoryContext execution_ctx;
	MemoryContext cache;
}			PGAA_ParserCtx;

PGAA_ParserCtx *pgaa_prepare_predicate(Datum predicate_text);
bool		pgaa_execute_predicate(PGAA_ParserCtx * ctx, Datum element, Oid target_oid, Oid collid);
void		pgaa_free_predicate(PGAA_ParserCtx * ctx);

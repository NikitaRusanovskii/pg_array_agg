#include "postgres.h"
#include "nodes/pg_list.h"
#include "utils/lsyscache.h"
#include "parser_util.h"

PGAA_Token
pgaa_construct_token(PGAA_TokenType type, const char *token, int32 length)
{
	return (PGAA_Token)
	{
		type, token, length
	};
}

PGAA_ASTNode *
pgaa_create_node(PGAA_Token value, PGAA_ASTNode * left, PGAA_ASTNode * right)
{
	PGAA_ASTNode *n = (PGAA_ASTNode *) palloc0(sizeof(PGAA_ASTNode));

	n->value = value;
	n->left = left;
	n->right = right;
	return n;
}
int
pgaa_free_ast(PGAA_ASTNode * root)
{
	List	   *l = NULL;

	if (root == NULL)
		return 0;
	l = lappend(l, root);
	while (l != NIL)
	{
		PGAA_ASTNode *tmp = (PGAA_ASTNode *) linitial(l);

		l = list_delete_first(l);
		if (tmp->left)
			l = lappend(l, tmp->left);
		if (tmp->right)
			l = lappend(l, tmp->right);
		pfree(tmp);
	}
	list_free(l);
	return 0;
}


PGAA_Token
pgaa_get_next(PGAA_ParserCtx * ctx)
{
	PGAA_Token	res;

	if (ctx == NULL)
	{
		ereport(ERROR, errcode(ERRCODE_INVALID_PARAMETER_VALUE), errmsg(
																		"pgaa_get_next: ctx is NULL"));
		pg_unreachable();
	}
	if (ctx->token_pos >= ctx->token_len)
	{
		ereport(ERROR, errcode(ERRCODE_INVALID_PARAMETER_VALUE), errmsg(
																		"pgaa_get_next: token_pos >= token_len"));
		pg_unreachable();
	}
	res = ctx->tokens[ctx->token_pos++];
	return res;
}

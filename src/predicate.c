#include "postgres.h"
#include "fmgr.h"
#include "utils/elog.h"
#include "utils/memutils.h"
#if PG_VERSION_NUM >= 160000
#include "varatt.h"
#endif
#include "predicate.h"
#include "parser.tab.h"
#include "scaner.h"
#include "utilfunc.h"
/*
 * void dfs_print(PGAA_ASTNode *node)
 * {
 * if (node == NULL)
 * return;
 *
 * dfs_print(node->left);
 * dfs_print(node->right);
 *
 * PGAA_Token t = node->value;
 * printf("%.*s\n", t.length, t.token);
 * }
 */

static int32
lexical_analyze(char *predicate_string, int32 len, PGAA_Token * result, int32 token_len)
{
	return pgaa_scaner(predicate_string, len, result, token_len);
}

PGAA_ParserCtx *
pgaa_prepare_predicate(Datum predicate_text)
{
	int			yyparse_res;
	int32		predicate_len;
	PGAA_ParserCtx *ctx = (PGAA_ParserCtx *) palloc0(sizeof(PGAA_ParserCtx));
	text	   *t = DatumGetTextPP(predicate_text);

	predicate_len = VARSIZE_ANY_EXHDR(t);
	ctx->predicate_string = palloc0(predicate_len + 1);
	memcpy(ctx->predicate_string, VARDATA_ANY(t), predicate_len);
	if ((Pointer) t != DatumGetPointer(predicate_text))
		pfree(t);				/* по сути, развёрнутый
								 * PG_FREE_IF_COPY */
	ctx->tokens = palloc0(sizeof(PGAA_Token) * (predicate_len + 2));
	if (predicate_len == 0)
	{
		/* это значит, что строка predicate = "" */
		pfree(ctx->tokens);
		pfree(ctx->predicate_string);
		pfree(ctx);
		return 0;
	}
	ctx->token_len = lexical_analyze(
									 ctx->predicate_string,
									 predicate_len,
									 ctx->tokens,
									 predicate_len + 2
		);
	if (ctx->token_len == 1)
	{
		pfree(ctx->tokens);
		pfree(ctx->predicate_string);
		pfree(ctx);
		return 0;
	}
	yyparse_res = pgaa_yyparse(ctx);
	if (yyparse_res == 1)
	{
		ereport(ERROR, errcode(ERRCODE_INVALID_PARAMETER_VALUE), errmsg(
																		"pgaa_prepare_predicate: invalid syntax"));
		pg_unreachable();
	}
	if (yyparse_res == 2)
	{
		ereport(ERROR, errcode(ERRCODE_INVALID_PARAMETER_VALUE), errmsg(
																		"pgaa_prepare_predicate: memory allocate error"));
		pg_unreachable();
	}
	ctx->execution_ctx = AllocSetContextCreate(
											   CurrentMemoryContext,
											   "ExecutionMemoryContext",
											   ALLOCSET_DEFAULT_SIZES
		);

	ctx->cache = AllocSetContextCreate(
									   CurrentMemoryContext,
									   "CacheMemoryContext",
									   ALLOCSET_DEFAULT_SIZES
		);
	/* dfs_print(ctx->result); */
	return ctx;
}

static Datum
dfs_execute(PGAA_ASTNode * node, Datum element, Oid toid, Oid collid, MemoryContext cache_ctx)
{

#define UNI_CMP_OP(op) do { if (node->left == NULL || node->right == NULL) { ereport( \
                ERROR, \
                errcode(ERRCODE_INVALID_PARAMETER_VALUE), \
                errmsg("dfs_execute: bad ast") \
            ); pg_unreachable(); } \
            return BoolGetDatum(pgaa_cmp_op(toid, left_res, right_res, collid) op 0); } while(0)

	Datum		left_res = (Datum) 0;
	Datum		right_res = (Datum) 0;
	MemoryContext old_ctx;

	if (node->left)
	{
		left_res = dfs_execute(node->left, element, toid, collid, cache_ctx);
	}
	if (node->right)
	{
		right_res = dfs_execute(node->right, element, toid, collid, cache_ctx);
	}
	switch (node->value.type)
	{
		case T_VARIABLE:
			return element;
		case T_NUMBER:
		case T_STRING:
			if (node->is_cached)
				return node->raw;
			else
			{
				old_ctx = MemoryContextSwitchTo(cache_ctx);
				node->raw = pgaa_string_to_datum(node->value.token, node->value.length, toid);
				MemoryContextSwitchTo(old_ctx);
				node->is_cached = true;
				return node->raw;
			}
		case T_EQ:
		case T_SEQ:
			UNI_CMP_OP(==);
		case T_NEQ:
			UNI_CMP_OP(!=);
		case T_GR:
			UNI_CMP_OP(>);
		case T_GRE:
			UNI_CMP_OP(>=);
		case T_LE:
			UNI_CMP_OP(<);
		case T_LEE:
			UNI_CMP_OP(<=);
		case T_MOD:
			if (node->left == NULL || node->right == NULL)
			{
				ereport(
						ERROR,
						errcode(ERRCODE_INVALID_PARAMETER_VALUE),
						errmsg("dfs_execute: bad ast"));
				pg_unreachable();
			}
			return pgaa_mod_op(toid, left_res, right_res, collid);
		default:
			ereport(
					ERROR,
					errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					errmsg("dfs_execute: bad ast"));
			pg_unreachable();
	}
#undef UNI_CMP_OP
}

bool
pgaa_execute_predicate(PGAA_ParserCtx * ctx, Datum element, Oid target_oid, Oid collid)
{
	bool		result;
	MemoryContext old_ctx;

	if (ctx == NULL)
	{
		ereport(ERROR,
				errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				errmsg("pgaa_execute_predicate: ctx == NULL"));
		pg_unreachable();
	}
	if (ctx->predicate_string == NULL)
	{
		ereport(ERROR,
				errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				errmsg("pgaa_execute_predicate: predicate_string == NULL"));
		pg_unreachable();
	}
	if (ctx->result == NULL)
	{
		ereport(ERROR,
				errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				errmsg("pgaa_execute_predicate: AST == NULL"));
		pg_unreachable();
	}
	if (ctx->tokens == NULL)
	{
		ereport(ERROR,
				errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				errmsg("pgaa_execute_predicate: tokens == NULL"));
		pg_unreachable();
	}
	old_ctx = MemoryContextSwitchTo(ctx->execution_ctx);
	result = DatumGetBool(dfs_execute(ctx->result, element, target_oid, collid, ctx->cache));
	MemoryContextSwitchTo(old_ctx);
	MemoryContextReset(ctx->execution_ctx);
	return result;
}
void
pgaa_free_predicate(PGAA_ParserCtx * ctx)
{
	pfree(ctx->tokens);
	pfree(ctx->predicate_string);
	pgaa_free_ast(ctx->result);
	MemoryContextDelete(ctx->execution_ctx);
	MemoryContextDelete(ctx->cache);
	pfree(ctx);
}

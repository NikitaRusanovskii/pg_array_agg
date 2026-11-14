#include "postgres.h"
#include "fmgr.h"
#include "utils/elog.h"
#include "varatt.h"
#include "predicate.h"
#include "parser.tab.h"
#include "scaner.h"
#include "utilfunc.h"
/*
void dfs_print(ASTNode *node)
{
    if (node == NULL)
        return;

    dfs_print(node->left);
    dfs_print(node->right);

    Token t = node->value;
    printf("%.*s\n", t.lenght, t.token);
}
*/

static int32 lexical_analyze(char* predicate_string, int32 len, Token* result, int32 token_len) {
    return scaner(predicate_string, len, result, token_len);
}

ParserCtx *prepare_predicate(Datum predicate_text) {
    int yyparse_res;
    int32 predicate_len;
    ParserCtx *ctx = (ParserCtx*)palloc0(sizeof(ParserCtx));
    text *t = DatumGetTextPP(predicate_text);
    predicate_len = VARSIZE_ANY_EXHDR(t);
    ctx->predicate_string = palloc0(predicate_len + 1);
    memcpy(ctx->predicate_string, VARDATA_ANY(t), predicate_len);
    ctx->tokens = palloc0(sizeof(Token) * (predicate_len+2));
    if (predicate_len == 0) {
        // это значит, что строка predicate = ""
        pfree(ctx->tokens);
        pfree(ctx->predicate_string); 
        pfree(ctx);
        return 0;
    }
    ctx->token_len = lexical_analyze(
        ctx->predicate_string,
        predicate_len,
        ctx->tokens,
        predicate_len+2
    );
    if (ctx->token_len == 1) {// строка состоит из пробелов, значит, обработка пропускается
        pfree(ctx->tokens);
        pfree(ctx->predicate_string); 
        pfree(ctx);
        return 0;
    }
    yyparse_res = yyparse(ctx);
    if(yyparse_res == 1) { ereport(ERROR, errcode(ERRCODE_INVALID_PARAMETER_VALUE), errmsg(
        "prepare_predicate: invalid syntax"));
        pg_unreachable();
    }
    if(yyparse_res == 2) { ereport(ERROR, errcode(ERRCODE_INVALID_PARAMETER_VALUE), errmsg(
        "prepare_predicate: memory allocate error"));
        pg_unreachable();
    }
    /* dfs_print(ctx->result); */
    return ctx;
}

static Datum dfs_execute(ASTNode *node, Datum element, Oid toid, Oid collid) {

#define UNI_CMP_OP(op) do { if (node->left == NULL || node->right == NULL) { ereport( \
                ERROR, \
                errcode(ERRCODE_INVALID_PARAMETER_VALUE), \
                errmsg("dfs_execute: bad ast") \
            ); pg_unreachable(); } \
            return BoolGetDatum(cmp_op(toid, left_res, right_res, collid) op 0); } while(0)

    Datum left_res;
    Datum right_res;
    if (node->left) {
        left_res = dfs_execute(node->left, element, toid, collid);
    }
    if (node->right) {
        right_res = dfs_execute(node->right, element, toid, collid);
    }
    switch(node->value.type) {
        case T_VARIABLE:
            return element;
        case T_NUMBER:
        case T_STRING:
            return string_to_datum(node->value.token, node->value.lenght, toid);
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
            if (node->left == NULL || node->right == NULL) { ereport(
                ERROR,
                errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                errmsg("dfs_execute: bad ast"));
                pg_unreachable();
            }
            return mod_op(toid, left_res, right_res, collid);
        default:
            ereport(
                ERROR,
                errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                errmsg("dfs_execute: bad ast"));
                pg_unreachable();   
    }
    #undef UNI_CMP_OP(op)
}

bool execute_predicate(ParserCtx *ctx, Datum element, Oid target_oid, Oid collid) {
    if (ctx == NULL) {
        ereport(ERROR,
            errcode(ERRCODE_INVALID_PARAMETER_VALUE),
            errmsg("execute_predicate: ctx == NULL"));
        pg_unreachable();
    }
    if (ctx->predicate_string == NULL) {
        ereport(ERROR,
            errcode(ERRCODE_INVALID_PARAMETER_VALUE),
            errmsg("execute_predicate: predicate_string == NULL"));
        pg_unreachable();
    }
    if (ctx->result == NULL) {
        ereport(ERROR,
            errcode(ERRCODE_INVALID_PARAMETER_VALUE),
            errmsg("execute_predicate: AST == NULL"));
        pg_unreachable();
    }
    if (ctx->tokens == NULL) {
        ereport(ERROR,
            errcode(ERRCODE_INVALID_PARAMETER_VALUE),
            errmsg("execute_predicate: tokens == NULL"));
        pg_unreachable();
    }
    return DatumGetBool(dfs_execute(ctx->result, element, target_oid, collid));
}
void free_predicate(ParserCtx *ctx) {
    pfree(ctx->tokens);
    pfree(ctx->predicate_string);
    free_ast(ctx->result);
    pfree(ctx);
}

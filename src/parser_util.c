#include "postgres.h"
#include "nodes/pg_list.h"
#include "parser_util.h"

Token construct_token(TokenType type, const char* token, int32 lenght) {
    return (Token){type, token, lenght};
}

ASTNode *create_node(Token value, ASTNode *left, ASTNode *right) {
    ASTNode *n = (ASTNode*)palloc0(sizeof(ASTNode));
    n->value = value;
    n->left = left;
    n->right = right;
    return n;
}
int free_ast(ASTNode *root) {
    if (root == NULL) return 0;
    List *l = NULL;
    l = lappend(l, root);
    while(list_length(l) > 0) {
        ASTNode *tmp = (ASTNode*)linitial(l);
        l = list_delete_first(l);
        if(tmp->left) l = lappend(l, tmp->left);
        if(tmp->right) l = lappend(l, tmp->right);
        pfree(tmp);
    }
    list_free(l);
    return 0;
}


Token get_next(ParserCtx *ctx) {
    if(ctx == NULL); //erep
    if(ctx->token_pos >= TOKEN_SLICE_LEN - 1); // erep потом нужно избавиться от макроса
    Token res = ctx->slise[ctx->token_pos];
    return res;
}

Token get_prev(ParserCtx *ctx) {
    if(ctx == NULL); //erep
    if (0 >= ctx->token_pos); // erep
    ctx->token_pos--;
    Token res = ctx->slise[ctx->token_pos];
    return res;
}

Token get_curr(ParserCtx *ctx) {
    if(ctx == NULL); //erep
    if (0 > ctx->token_pos || TOKEN_SLICE_LEN <= ctx->token_pos); // erep
    Token res = ctx->slise[ctx->token_pos];
    return res;
}
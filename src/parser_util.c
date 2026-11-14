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
    while(l != NIL) {
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
    if(ctx == NULL) 
        ereport(ERROR, errcode(ERRCODE_INVALID_PARAMETER_VALUE), errmsg(
        "get_next: ctx is NULL"
    ));
    if(ctx->token_pos >= ctx->token_len)
        ereport(ERROR, errcode(ERRCODE_INVALID_PARAMETER_VALUE), errmsg(
        "get_next: token_pos >= token_len"
    ));
    Token res = ctx->tokens[ctx->token_pos++];
    return res;
}
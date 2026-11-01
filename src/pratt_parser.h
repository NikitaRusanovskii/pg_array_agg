#pragma once
#include "postgres.h"
#include "utilfunc.h"

typedef struct {
    Token token;
    ASTNode *left, *right, *parent;
    //Datum element;
    //Oid type_oid;
    //bool is_const;
} ASTNode;

typedef struct {
    ASTNode* root;
}  ASTTree;

ASTNode* get_left(ASTNode* cur) {
    if (cur == NULL) return NULL;
    return cur->left;
}

ASTNode* get_right(ASTNode* cur) {
    if (cur == NULL) return NULL;
    return cur->right;
}

ASTNode* get_parent(ASTNode* cur) {
    if (cur == NULL) return NULL;
    return cur->parent;
}
/*
Datum get_value(ASTNode* cur) {
    return cur->element;
}

Oid get_value_type(ASTNode* cur) {
    return cur->type_oid;
}

bool is_const(ASTNode* cur) {
    return cur->is_const;
}
*/

typedef struct {
    Token* tokens;
    int32 count;
    int32 pos;
} PState;

Token current(PState* state) {
    if (state == NULL); // ereport
    if (state->tokens == NULL); // ereport
    if (state->pos > 0) return state->tokens[state->pos-1];
    else return state->tokens[0];
}


Token current(PState* state) {
    if (state == NULL); // ereport
    if (state->tokens == NULL); // ereport
    if (state->pos < state->count) return state->tokens[state->pos++];
    else {} // ereport?
}

bool match(PState* state, TokenType t) {
    if (state == NULL); // ereport
    if (state->tokens == NULL); // ereport
    if (state->pos < state->count) {
        if (t == state->tokens[state->pos].type) {
            state->pos++;
            return true;
        }
        return false;
    }
    else {} // ereport?
}

typedef struct {
    int32 left, right;
} BindPower;

BindPower get_bp(TokenType t) {
    switch (t) {
        case T_MOD:
            return (BindPower){20, 21};
        case T_EQ:
        case T_GR: 
        case T_NEQ:
        case T_LE: 
        case T_GRE:
        case T_LEE:
        case T_SEQ:
            return (BindPower){10, 11};

        default:
            return (BindPower){-1, -1};
    }
} 


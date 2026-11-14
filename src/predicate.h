#pragma once
#include "postgres.h"

typedef enum {
    OP_EQ, // ==
    OP_NEQ, // !=
    OP_GR, // >
    OP_LE, // <
    OP_GRE, // >=
    OP_LEE, // <=
    OP_MOD, // %
    OP_SEQ, // = (short equal)
} Operator;

typedef enum {
    T_VARIABLE,
    T_NUMBER,
    T_STRING,
    T_EQ, // ==
    T_GR, // >
    T_NEQ, // !=
    T_LE, // <
    T_GRE, // >=
    T_LEE, // <=
    T_MOD, // %
    T_SEQ, // = (short equal)
    T_END
} TokenType;

typedef struct Token {
    TokenType type;
    const char* token; // указатель на начало подстроки
    int32 lenght; // длина подстроки
} Token;

typedef struct ASTNode {
    Token value;
    struct ASTNode *left, *right;
} ASTNode;

typedef struct ParseCtx {
    Token *tokens;
    size_t token_len;
    size_t token_pos;
    char *predicate_string;
    ASTNode *result;    
} ParserCtx;

ParserCtx *prepare_predicate(Datum predicate_text);
bool execute_predicate(ParserCtx *ctx, Datum element, Oid target_oid, Oid collid);
void free_predicate(ParserCtx *ctx);
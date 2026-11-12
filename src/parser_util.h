#pragma once
#include "postgres.h"

typedef enum {
    CMP,
    MOD,
} PredicateType;

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

typedef struct {
   PredicateType predicate_type;
   Operator op;
   char* const1;
   char* const2;

   int32 const1len;
   int32 const2len;
} ParsedPredicate;

typedef struct {
    PredicateType predicate_type;
    Operator op;
    Oid target_oid;

    Datum const1;
    Datum const2;
} Predicate;

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

typedef struct {
    TokenType type;
    const char* token; // указатель на начало подстроки
    int32 lenght; // длина подстроки
} Token;

Token construct_token(TokenType type, const char* token, int32 lenght);
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

bool type_is_expected(Oid type_oid, const Oid* expected_types, uint32 len);
int32 cmp_op(Oid target_oid, Datum first, Datum second, Oid collid);
Numeric datum_to_numeric(Datum element, Oid type_oid);
int32 prepare_predicate(Datum predicate_text, Oid target_oid, Predicate* predicate, Oid collid);
bool execute_predicate(const Predicate* predicate, Datum element, Oid target_oid, Oid collid);
void free_predicate(const Predicate* predicate);
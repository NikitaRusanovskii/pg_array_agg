#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"
#include "utils/numeric.h"
#include "utils/array.h"
#include "regex/regex.h"
#include "catalog/pg_type_d.h"
#include "utils/fmgrprotos.h"

Datum array_product(PG_FUNCTION_ARGS);							
Datum array_sum(PG_FUNCTION_ARGS);								
Datum array_avg(PG_FUNCTION_ARGS);								
Datum array_min(PG_FUNCTION_ARGS);								
Datum array_max(PG_FUNCTION_ARGS);								
Datum array_count(PG_FUNCTION_ARGS);
Datum array_count_if(PG_FUNCTION_ARGS);
Datum array_all(PG_FUNCTION_ARGS);
Datum array_any(PG_FUNCTION_ARGS);
Datum array_none(PG_FUNCTION_ARGS);

typedef enum {
    CMP,
    MOD
} PredicateType;

typedef enum {
    EQ, // ==
    NEQ, // !=
    GR, // >
    LE, // <
    GRE, // >=
    LEE, // <=
    MOD, // %
    SEQ, // = (short equal)
} Operator;

bool type_is_expected(Oid type_oid, Oid* expected_types, uint32 len) {
    for (int i = 0; i < len; i++) {
        if (expected_types[i] == type_oid) return true;
    }
    return false;
}

Datum string_to_datum(char* str, int len, Oid target_oid) {
    Oid input;
    Oid param;
    Datum res;
    char* str_ = pnstrdup(str, len);
    getTypeInputInfo(target_oid, &input, &param);
    res = OidInputFunctionCall(input, str_, param, -1);
    pfree(str_);
    return res;
}


typedef struct {
   PredicateType predicate_type;
   Operator op;
   text* const1;
   text* const2;

} ParsedPredicate;

typedef struct {
    PredicateType predicate_type;
    Operator op;
    Oid target_oid;

    Datum const1;
    Datum const2;
} Predicate;
// лексический разбор
// синтаксический разбор
// проверить структуру предиката
// проверить допустимость операций
// привести предикат к нужному виду


PG_FUNCTION_INFO_V1(array_count_if);
Datum array_count_if(PG_FUNCTION_ARGS) {
    ArrayType* arr;
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Datum current_element;
    ArrayIterator iterator;
    bool is_null;
    int32 result;

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL(); // функция STRIСT, этого никогда не произойдёт
    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_count_if: searching elements in manydimensional arrays is not supported yet")));
    if (ARR_NDIM(arr) < 1) // пустой, нульмерный
        PG_RETURN_NULL();
    predicate = PG_GETARG_DATUM(1);
    // считаем, что распарсили predicate

    result = 0;
    iterator = array_create_iterator(arr, 0, NULL);
    while(array_iterate(iterator, &current_element, &is_null)) {
        if (is_null) {
            array_free_iterator(iterator);
            PG_RETURN_NULL();
        }
    }

    PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
    array_free_iterator(iterator);
    PG_RETURN_INT32(result);
}
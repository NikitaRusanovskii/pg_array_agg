#include "postgres.h"
#include "fmgr.h"
#include "utils/array.h"
#include "utils/typcache.h"

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
    ONE_OP,
    TWO_OP
} predicate_type;

typedef struct {
    Oid operand_type;
    text* varname;
    text* operator1;
    text* const1;
    text* operator2;
    text* const2;
    predicate_type pt;
} predicate;

typedef enum {
    EQ, // ==
    NEQ, // !=
    GR, // >
    LE, // <
    GRE, // >=
    LEE, // <=
    REM, // %
    SEQ, // = (short equal)
} operators;

PG_FUNCTION_INFO_V1(array_count_if);
Datum array_count_if(PG_FUNCTION_ARGS) {
    ArrayType* arr;
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Datum current_element;
    TypeCacheEntry* typentry;
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
    array_free_iterator(iterator);
    PG_RETURN_INT32(result);
}
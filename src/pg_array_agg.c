#include "postgres.h"
#include "fmgr.h"
#include "utils/array.h"
#include "utils/elog.h"
#include "catalog/pg_type_d.h"
#include "utilfunc.h"

PG_MODULE_MAGIC;

// Datum array_product(PG_FUNCTION_ARGS);							
// Datum array_sum(PG_FUNCTION_ARGS);								
// Datum array_avg(PG_FUNCTION_ARGS);								
// Datum array_min(PG_FUNCTION_ARGS);								
// Datum array_max(PG_FUNCTION_ARGS);								
// Datum array_count(PG_FUNCTION_ARGS);
Datum array_count_if(PG_FUNCTION_ARGS);
Datum array_all(PG_FUNCTION_ARGS);
Datum array_any(PG_FUNCTION_ARGS);
Datum array_none(PG_FUNCTION_ARGS);

//PG_FUNCTION_INFO_V1(array_product);
//PG_FUNCTION_INFO_V1(array_sum);
//PG_FUNCTION_INFO_V1(array_avg);
//PG_FUNCTION_INFO_V1(array_min);
//PG_FUNCTION_INFO_V1(array_max);
//PG_FUNCTION_INFO_V1(array_count);
PG_FUNCTION_INFO_V1(array_count_if);
PG_FUNCTION_INFO_V1(array_all);
PG_FUNCTION_INFO_V1(array_any);
PG_FUNCTION_INFO_V1(array_none);

Datum array_count_if(PG_FUNCTION_ARGS) {
    ArrayType* arr;
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Datum current_element;
    ArrayIterator iterator;
    bool is_null;
    int32 result;
    Oid expected_types[7] = {INT2OID, INT4OID, INT8OID,
                             FLOAT4OID, FLOAT8OID, NUMERICOID, TEXTOID};

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL(); // функция STRIСT, этого никогда не произойдёт
    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_count_if: searching elements in manydimensional arrays is not supported yet")));
    if (!type_is_expected(arr->elemtype, expected_types, 7))
        ereport(ERROR, errcode(ERRCODE_DATATYPE_MISMATCH), errmsg("array_count_if: incorrect datatype"));
    
    predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));
    elemtype = arr->elemtype;
    result = 0;
    iterator = array_create_iterator(arr, 0, NULL);
    while(array_iterate(iterator, &current_element, &is_null)) {
        if (is_null) {
            array_free_iterator(iterator);
            PG_RETURN_NULL();
        }

        if (process(current_element, predicate,elemtype, collation)) result++;
    }

    PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
    array_free_iterator(iterator);
    PG_RETURN_INT32(result);
}


Datum array_any(PG_FUNCTION_ARGS) {
    ArrayType* arr;
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Datum current_element;
    ArrayIterator iterator;
    bool is_null;
    bool result;
    Oid expected_types[7] = {INT2OID, INT4OID, INT8OID,
                             FLOAT4OID, FLOAT8OID, NUMERICOID, TEXTOID};

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL(); // функция STRIСT, этого никогда не произойдёт
    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_any: searching elements in manydimensional arrays is not supported yet")));
    if (!type_is_expected(arr->elemtype, expected_types, 7))
        ereport(ERROR, errcode(ERRCODE_DATATYPE_MISMATCH), errmsg("array_any: incorrect datatype"));
    
    predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));
    elemtype = arr->elemtype;
    result = false;
    iterator = array_create_iterator(arr, 0, NULL);
    while(array_iterate(iterator, &current_element, &is_null)) {
        if (is_null) {
            array_free_iterator(iterator);
            PG_RETURN_NULL();
        }

        if (process(current_element, predicate,elemtype, collation)) result = true;
    }

    PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
    array_free_iterator(iterator);
    PG_RETURN_BOOL(result);
}

Datum array_all(PG_FUNCTION_ARGS) {
    ArrayType* arr;
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Datum current_element;
    ArrayIterator iterator;
    bool is_null;
    bool result;
    Oid expected_types[7] = {INT2OID, INT4OID, INT8OID,
                             FLOAT4OID, FLOAT8OID, NUMERICOID, TEXTOID};

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL(); // функция STRIСT, этого никогда не произойдёт
    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_all: searching elements in manydimensional arrays is not supported yet")));
    if (!type_is_expected(arr->elemtype, expected_types, 7))
        ereport(ERROR, errcode(ERRCODE_DATATYPE_MISMATCH), errmsg("array_all: incorrect datatype"));
    
    predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));
    elemtype = arr->elemtype;
    result = true;
    iterator = array_create_iterator(arr, 0, NULL);
    while(array_iterate(iterator, &current_element, &is_null)) {
        if (is_null) {
            array_free_iterator(iterator);
            PG_RETURN_NULL();
        }

        if (!process(current_element, predicate,elemtype, collation)) result = false;
    }

    PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
    array_free_iterator(iterator);
    PG_RETURN_BOOL(result);
}

Datum array_none(PG_FUNCTION_ARGS) {
    ArrayType* arr;
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Datum current_element;
    ArrayIterator iterator;
    bool is_null;
    bool result;
    Oid expected_types[7] = {INT2OID, INT4OID, INT8OID,
                             FLOAT4OID, FLOAT8OID, NUMERICOID, TEXTOID};

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL(); // функция STRIСT, этого никогда не произойдёт
    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_none: searching elements in manydimensional arrays is not supported yet")));
    if (!type_is_expected(arr->elemtype, expected_types, 7))
        ereport(ERROR, errcode(ERRCODE_DATATYPE_MISMATCH), errmsg("array_none: incorrect datatype"));
    
    predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));
    elemtype = arr->elemtype;
    result = true;
    iterator = array_create_iterator(arr, 0, NULL);
    while(array_iterate(iterator, &current_element, &is_null)) {
        if (is_null) {
            array_free_iterator(iterator);
            PG_RETURN_NULL();
        }

        if (process(current_element, predicate,elemtype, collation)) result = false;
    }

    PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
    array_free_iterator(iterator);
    PG_RETURN_BOOL(result);
}
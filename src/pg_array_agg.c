#include "postgres.h"
#include "fmgr.h"
#include "utils/array.h"
#include "utils/elog.h"
#include "utils/numeric.h"
#include "catalog/pg_type_d.h"
#include "utilfunc.h"


PG_MODULE_MAGIC;

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

PG_FUNCTION_INFO_V1(array_product);
PG_FUNCTION_INFO_V1(array_sum);
PG_FUNCTION_INFO_V1(array_avg);
PG_FUNCTION_INFO_V1(array_min);
PG_FUNCTION_INFO_V1(array_max);
PG_FUNCTION_INFO_V1(array_count);
PG_FUNCTION_INFO_V1(array_count_if);
PG_FUNCTION_INFO_V1(array_all);
PG_FUNCTION_INFO_V1(array_any);
PG_FUNCTION_INFO_V1(array_none);

Datum array_product(PG_FUNCTION_ARGS) {
    ArrayType *arr;
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID}; // они должны совпадать с функцией валидатором. Это небезопасно с точки зрения рефакторинга. Надо уединить их в одном месте
    ArrayIterator arr_it;
    Datum elem;
    bool is_null;
    Numeric res;
    Numeric tmp_elem;
    Numeric tmp_res;
    bool err = false;

    if (PG_NARGS() == 0) PG_RETURN_NULL();
    if (PG_NARGS() > 1) ereport(ERROR,
        (errcode(ERRCODE_TOO_MANY_ARGUMENTS),
         errmsg("array_product: Too many arguments: %d, functions need: %d", PG_NARGS(), 1)));

    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_product: Invalid ndim of array: %d", ARR_NDIM(arr))));
    }

    if (!type_is_expected(arr->elemtype, expected_types, 6)) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_product: Invalid elemtype of array. OID: %d", arr->elemtype)));
    }

    arr_it = array_create_iterator(arr, 0, NULL);

    // для итератора

    //результирующая переменная, тут накопим результат умножений. Инициализируем её значением 1
    res = int64_to_numeric(1);

    while(array_iterate(arr_it, &elem, &is_null)) {

        if (is_null) {
            PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            array_free_iterator(arr_it);
            PG_RETURN_NULL();
        }
        tmp_elem = datum_to_numeric(elem, arr->elemtype);
        tmp_res = numeric_mul_opt_error(res, tmp_elem, &err);
        pfree(res);
        pfree(tmp_elem);
        if(err) {
            PG_FREE_IF_COPY(arr, 0);
            array_free_iterator(arr_it);        
            ereport(ERROR,
            (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
             errmsg("array_product: An error occurred during a numeric multiplication.")));
        }
        res = tmp_res;
    }

    PG_FREE_IF_COPY(arr, 0);
    array_free_iterator(arr_it);

    PG_RETURN_NUMERIC(res);
}

Datum array_sum(PG_FUNCTION_ARGS) {
    ArrayType *arr;
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID};
    ArrayIterator arr_it;
    Datum elem;
    bool is_null;
    Numeric res;
    Numeric tmp_elem;
    Numeric tmp_res;
    bool err = false;

    if (PG_NARGS() == 0) PG_RETURN_NULL();
    if (PG_NARGS() > 1) ereport(ERROR,
        (errcode(ERRCODE_TOO_MANY_ARGUMENTS),
         errmsg("array_sum: Too many arguments: %d, functions need: %d", PG_NARGS(), 1)));

    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_sum: Invalid ndim of array: %d", ARR_NDIM(arr))));
    }

    if (!type_is_expected(arr->elemtype, expected_types, 6)) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_sum: Invalid elemtype of array. OID: %d", arr->elemtype)));
    }

    arr_it = array_create_iterator(arr, 0, NULL);

    // для итератора

    //результирующая переменная, тут накопим результат сложений. Инициализируем её значением 0
    res = int64_to_numeric(0);

    while(array_iterate(arr_it, &elem, &is_null)) {

        if (is_null) {
            PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            array_free_iterator(arr_it);
            PG_RETURN_NULL();
        }

        tmp_elem = datum_to_numeric(elem, arr->elemtype);
        tmp_res = numeric_add_opt_error(res, tmp_elem, &err);
        pfree(res);
        pfree(tmp_elem);
        if(err) {
            PG_FREE_IF_COPY(arr, 0);
            array_free_iterator(arr_it);        
            ereport(ERROR,
            (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
             errmsg("array_sum: An error occurred during a numeric sum")));
        }
        res = tmp_res;
    }
    PG_FREE_IF_COPY(arr, 0);
    array_free_iterator(arr_it);

    PG_RETURN_NUMERIC(res);
}

Datum array_avg(PG_FUNCTION_ARGS) {
    ArrayType *arr;
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID};
    ArrayIterator arr_it;
    Datum elem;
    bool is_null;
    Numeric res;
    Numeric total;
    Numeric tmp_elem;
    Numeric tmp_res;
    int64 count;
    bool err = false;

    if (PG_NARGS() == 0) PG_RETURN_NULL();
    if (PG_NARGS() > 1) ereport(ERROR,
        (errcode(ERRCODE_TOO_MANY_ARGUMENTS),
         errmsg("array_avg: Too many arguments: %d, functions need: %d", PG_NARGS(), 1)));

    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_avg: Invalid ndim of array: %d", ARR_NDIM(arr))));
    }

    if (!type_is_expected(arr->elemtype, expected_types, 6)) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_avg: Invalid elemtype of array. OID: %d", arr->elemtype)));
    }

    if (ArrayGetNItems(ARR_NDIM(arr), ARR_DIMS(arr)) == 0){
        PG_FREE_IF_COPY(arr, 0);
        PG_RETURN_NULL();
    }

    arr_it = array_create_iterator(arr, 0, NULL);

    // для итератора

    //результирующая переменная, тут накопим результат сложений. Инициализируем её значением 0
    res = int64_to_numeric(0);
    count = 0; // будем считать кол-во элементов самостоятельно, так как если изменится поведение относительно NULL элементов, ручной подсчёт более гибкий

    while(array_iterate(arr_it, &elem, &is_null)) {

        if (is_null) {
            PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            array_free_iterator(arr_it);
            PG_RETURN_NULL();
        }

        tmp_elem = datum_to_numeric(elem, arr->elemtype);
        tmp_res = numeric_add_opt_error(res, tmp_elem, &err);
        pfree(res);
        pfree(tmp_elem);
        if(err) {
            ereport(ERROR,
            (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
             errmsg("array_avg: An error occurred during a numeric sum.")));
        }
        res = tmp_res;
        count++;
    }

    PG_FREE_IF_COPY(arr, 0);
    array_free_iterator(arr_it);
    total = int64_to_numeric(count);
    res = numeric_div_opt_error(res, total, &err);
    pfree(total);
    if (err)      
        ereport(ERROR,
        (errcode(ERRCODE_DIVISION_BY_ZERO),
         errmsg("array_avg: Division by zero error")));

    PG_RETURN_NUMERIC(res);
}

Datum array_min(PG_FUNCTION_ARGS) {
    ArrayType *arr;
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID};
    ArrayIterator arr_it;
    Datum elem;
    Datum res;
    bool is_null = false;
    bool is_first = true;

    if (PG_NARGS() == 0) PG_RETURN_NULL();
    if (PG_NARGS() > 1) ereport(ERROR,
        (errcode(ERRCODE_TOO_MANY_ARGUMENTS),
         errmsg("array_min: Too many arguments: %d, functions need: %d", PG_NARGS(), 1)));

    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_min: Invalid ndim of array: %d", ARR_NDIM(arr))));
    }

    if (!type_is_expected(arr->elemtype, expected_types, 6)) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_min: Invalid elemtype of array. OID: %d", arr->elemtype)));
    }

    if (ArrayGetNItems(ARR_NDIM(arr), ARR_DIMS(arr)) == 0){
        PG_FREE_IF_COPY(arr, 0);
        PG_RETURN_NULL();
    }

    arr_it = array_create_iterator(arr, 0, NULL);

    // для итератора

    while(array_iterate(arr_it, &elem, &is_null)) {

        if (is_null) {
            PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            array_free_iterator(arr_it);
            PG_RETURN_NULL();
        }

        if (is_first){
            res = elem;
            is_first = false;
            continue;
        }

        if (cmp_op(arr->elemtype, res, elem, PG_GET_COLLATION()) > 0) res = elem;

    }
    array_free_iterator(arr_it);
    PG_RETURN_DATUM(res);
}

Datum array_max(PG_FUNCTION_ARGS) {
    ArrayType *arr;
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID};
    ArrayIterator arr_it;
    Datum elem;
    Datum res;
    bool is_null = false;
    bool is_first = true;

    if (PG_NARGS() == 0) PG_RETURN_NULL();
    if (PG_NARGS() > 1) ereport(ERROR,
        (errcode(ERRCODE_TOO_MANY_ARGUMENTS),
         errmsg("array_max: Too many arguments: %d, functions need: %d", PG_NARGS(), 1)));

    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_max: Invalid ndim of array: %d", ARR_NDIM(arr))));
    }

    if (!type_is_expected(arr->elemtype, expected_types, 6)) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_max: Invalid elemtype of array. OID: %d", arr->elemtype)));
    }

    if (ArrayGetNItems(ARR_NDIM(arr), ARR_DIMS(arr)) == 0){
        PG_FREE_IF_COPY(arr, 0);
        PG_RETURN_NULL();
    }

    arr_it = array_create_iterator(arr, 0, NULL);

    // для итератора

    while(array_iterate(arr_it, &elem, &is_null)) {

        if (is_null) {
            PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            array_free_iterator(arr_it);
            PG_RETURN_NULL();
        }

        if (is_first){
            res = elem;
            is_first = false;
            continue;
        }

        if (cmp_op(arr->elemtype, res, elem, PG_GET_COLLATION()) < 0) res = elem;

    }

    array_free_iterator(arr_it);
    PG_RETURN_DATUM(res);
}

Datum array_count(PG_FUNCTION_ARGS) {
    ArrayType *arr;
    Datum element;
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID,
                            FLOAT4OID, FLOAT8OID, TEXTOID, VARCHAROID, BPCHAROID};
    ArrayIterator arr_it;
    Datum elem;
    int32 result = 0;
    bool is_null = false;

    if (PG_NARGS() < 2 ) PG_RETURN_NULL();
    if (PG_NARGS() > 2) ereport(ERROR,
        (errcode(ERRCODE_TOO_MANY_ARGUMENTS),
         errmsg("array_count: Too many arguments: %d, functions need: %d", PG_NARGS(), 2)));

    arr = PG_GETARG_ARRAYTYPE_P(0);
    element = PG_GETARG_DATUM(1);

    if (ARR_NDIM(arr) > 1) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_count: Invalid ndim of array: %d", ARR_NDIM(arr))));
    }

    if (!type_is_expected(arr->elemtype, expected_types, 9)) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_count: Invalid elemtype of array. OID: %d", arr->elemtype)));
    }

    //if (ArrayGetNItems(ARR_NDIM(arr), ARR_DIMS(arr)) == 0) PG_RETURN_NULL();
    arr_it = array_create_iterator(arr, 0, NULL);

    // для итератора

    while(array_iterate(arr_it, &elem, &is_null)) {

        if (is_null) {
            //PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            //array_free_iterator(arr_it);
            //PG_RETURN_INT32(0);
            continue;
        }

        if (cmp_op(arr->elemtype, element, elem, PG_GET_COLLATION()) == 0) result++;

    }
    PG_FREE_IF_COPY(arr, 0);
    array_free_iterator(arr_it);
    PG_RETURN_INT32(result);
}

Datum array_count_if(PG_FUNCTION_ARGS) {
    ArrayType* arr;
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Predicate compiled_predicate;
    Datum current_element;
    ArrayIterator iterator;
    bool is_null;
    int32 result;
    int pred_len;
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID,
                            FLOAT4OID, FLOAT8OID, TEXTOID, VARCHAROID, BPCHAROID};

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL(); // функция STRIСT, этого никогда не произойдёт
    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_count_if: searching elements in manydimensional arrays is not supported yet")));

    if (!type_is_expected(arr->elemtype, expected_types, 9))
        ereport(ERROR,
            (errcode(ERRCODE_DATATYPE_MISMATCH),
             errmsg("array_count_if: incorrect datatype")));

    predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));
    elemtype = arr->elemtype;
    result = 0;
    iterator = array_create_iterator(arr, 0, NULL);

    pred_len = prepare_predicate(predicate, elemtype, &compiled_predicate, collation);
    if (pred_len == 0) {
        PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
        // так как внутри prepare_predicate в таком случае память не выделяется, compiled_predicate можно не очищать
        array_free_iterator(iterator);
        PG_RETURN_INT32(0);
    }
    while(array_iterate(iterator, &current_element, &is_null)) {
        if (is_null) {
            //PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            //PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
            //array_free_iterator(iterator);
            //free_predicate(&compiled_predicate);
            //PG_RETURN_NULL();
            continue;
        }

        if (execute_predicate(&compiled_predicate, current_element,elemtype, collation)) result++;
    }

    PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
    PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
    array_free_iterator(iterator);
    free_predicate(&compiled_predicate);
    PG_RETURN_INT32(result);
}


Datum array_any(PG_FUNCTION_ARGS) {
    ArrayType* arr;
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Predicate compiled_predicate;
    Datum current_element;
    ArrayIterator iterator;
    bool is_null;
    int pred_len;
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID,
                            FLOAT4OID, FLOAT8OID, TEXTOID, VARCHAROID, BPCHAROID};

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL(); // функция STRIСT, этого никогда не произойдёт
    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_any: searching elements in manydimensional arrays is not supported yet")));

    if (!type_is_expected(arr->elemtype, expected_types, 9))
        ereport(ERROR,
            (errcode(ERRCODE_DATATYPE_MISMATCH),
             errmsg("array_any: incorrect datatype")));

    predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));
    elemtype = arr->elemtype;
    iterator = array_create_iterator(arr, 0, NULL);

    pred_len = prepare_predicate(predicate, elemtype, &compiled_predicate, collation);
    if (pred_len == 0) {
        PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
        // так как внутри prepare_predicate в таком случае память не выделяется, compiled_predicate можно не очищать
        array_free_iterator(iterator);
        PG_RETURN_BOOL(false);
    }
    while(array_iterate(iterator, &current_element, &is_null)) {
        if (is_null) {
            //PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            //PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
            //array_free_iterator(iterator);
            //free_predicate(&compiled_predicate);
            //PG_RETURN_NULL();
            continue;
        }

        if (execute_predicate(&compiled_predicate, current_element,elemtype, collation)) {
            PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
            array_free_iterator(iterator);
            free_predicate(&compiled_predicate);
            PG_RETURN_BOOL(true);
        }
    }

    PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
    PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
    array_free_iterator(iterator);
    free_predicate(&compiled_predicate);
    PG_RETURN_BOOL(false);
}

Datum array_all(PG_FUNCTION_ARGS) {
    ArrayType* arr;
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Predicate compiled_predicate;
    Datum current_element;
    ArrayIterator iterator;
    bool is_null;
    int pred_len;
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID,
                            FLOAT4OID, FLOAT8OID, TEXTOID, VARCHAROID, BPCHAROID};

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL(); // функция STRIСT, этого никогда не произойдёт
    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_all: searching elements in manydimensional arrays is not supported yet")));

    if (!type_is_expected(arr->elemtype, expected_types, 9))
        ereport(ERROR,
            (errcode(ERRCODE_DATATYPE_MISMATCH),
             errmsg("array_all: incorrect datatype")));

    predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));
    elemtype = arr->elemtype;
    iterator = array_create_iterator(arr, 0, NULL);

    pred_len = prepare_predicate(predicate, elemtype, &compiled_predicate, collation);
    if (pred_len == 0) {
        PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
        // так как внутри prepare_predicate в таком случае память не выделяется, compiled_predicate можно не очищать
        array_free_iterator(iterator);
        if(ArrayGetNItems(ARR_NDIM(arr), ARR_DIMS(arr)) == 0) PG_RETURN_BOOL(true);
        PG_RETURN_BOOL(false);
    }
    while(array_iterate(iterator, &current_element, &is_null)) {
        if (is_null) {
            //PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            //PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
            //array_free_iterator(iterator);
            //free_predicate(&compiled_predicate);
            //PG_RETURN_NULL();
            continue;
        }

        if (!execute_predicate(&compiled_predicate, current_element,elemtype, collation)){
            PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
            array_free_iterator(iterator);
            free_predicate(&compiled_predicate);
            PG_RETURN_BOOL(false);
        }
    }

    PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
    PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
    array_free_iterator(iterator);
    free_predicate(&compiled_predicate);
    PG_RETURN_BOOL(true);
}

Datum array_none(PG_FUNCTION_ARGS) {
    ArrayType* arr;
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Predicate compiled_predicate;
    Datum current_element;
    ArrayIterator iterator;
    bool is_null;
    int pred_len;
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID,
                            FLOAT4OID, FLOAT8OID, TEXTOID, VARCHAROID, BPCHAROID};

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL(); // функция STRIСT, этого никогда не произойдёт
    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_none: searching elements in manydimensional arrays is not supported yet")));

    if (!type_is_expected(arr->elemtype, expected_types, 9))
        ereport(ERROR,
            (errcode(ERRCODE_DATATYPE_MISMATCH),
             errmsg("array_none: incorrect datatype")));

    predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));
    elemtype = arr->elemtype;
    iterator = array_create_iterator(arr, 0, NULL);

    pred_len = prepare_predicate(predicate, elemtype, &compiled_predicate, collation);
    if (pred_len == 0) {
        PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
        // так как внутри prepare_predicate в таком случае память не выделяется, compiled_predicate можно не очищать
        array_free_iterator(iterator);
        PG_RETURN_BOOL(true);
    }
    if (pred_len == 0) {
        PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
        // так как внутри prepare_predicate в таком случае память не выделяется, compiled_predicate можно не очищать
        array_free_iterator(iterator);
        PG_RETURN_NULL();
    }
    while(array_iterate(iterator, &current_element, &is_null)) {
        if (is_null) {

            //PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            //PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
            //array_free_iterator(iterator);
            //free_predicate(&compiled_predicate);
            //PG_RETURN_NULL();
            continue;
        }

        if (execute_predicate(&compiled_predicate, current_element, elemtype, collation)){
            PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
            PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
            array_free_iterator(iterator);
            free_predicate(&compiled_predicate);
            PG_RETURN_BOOL(false);
        }
    }

    PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
    PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
    array_free_iterator(iterator);
    free_predicate(&compiled_predicate);
    PG_RETURN_BOOL(true);
}
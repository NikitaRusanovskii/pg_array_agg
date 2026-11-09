#include "postgres.h"
#include "fmgr.h"
#include "pg_config.h"
#include "utils/array.h"
#include "utils/elog.h"
#include "utils/numeric.h"
#include "catalog/pg_type_d.h"
#include "utilfunc.h"
#include "utils/arrayaccess.h"
#include "utils/lsyscache.h"


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

#define ARITHM_ALLOWED_TYPES {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID}
#define CMP_ALLOWED_TYPES {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID}
#define CNT_ALLOWED_TYPES {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID,\
                            TEXTOID, VARCHAROID, BPCHAROID}
#define PRED_ALLOWED_TYPES {INT2OID, INT4OID, INT8OID, NUMERICOID,\
                            FLOAT4OID, FLOAT8OID, TEXTOID, VARCHAROID, BPCHAROID}

Datum array_product(PG_FUNCTION_ARGS) {
    Oid const expected_types[] = ARITHM_ALLOWED_TYPES;
    AnyArrayType *arr;
    Oid array_elemtype;
    int ndims;
    int nitems;
    
    int16 typlen;
    bool typbyval;
    char typalign;
    
    array_iter iter;
    
    Datum elem;
    Numeric res;
    Numeric tmp_elem;
    Numeric tmp_res;
    
    bool is_null = false;
    int i;
    
    #if PG_VERSION_NUM < 190000
    bool err = false;
    #endif
    arr = PG_GETARG_ANY_ARRAY_P(0);
    array_elemtype = AARR_ELEMTYPE(arr);
    ndims = AARR_NDIM(arr);
    nitems = ArrayGetNItems(ndims, AARR_DIMS(arr));

    if (ndims > 2) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_product: Invalid ndim of array: %d", ndims)));
    }

    if (!type_is_expected(array_elemtype, expected_types, 6)) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_product: Invalid elemtype of array. OID: %d", array_elemtype)));
    }

    get_typlenbyvalalign(array_elemtype, &typlen, &typbyval, &typalign);

    array_iter_setup(&iter, arr, typlen, typbyval, typalign);

    res = int64_to_numeric(1);

    for(i = 0; i < nitems; i++) {

        
        elem = array_iter_next(&iter, &is_null, i);
        if (is_null) {
            PG_FREE_IF_COPY(arr, 0);
            pfree(res);
            PG_RETURN_NULL();
        }
        tmp_elem = datum_to_numeric(elem, array_elemtype);
        #if PG_VERSION_NUM >= 190000
        tmp_res = numeric_mul_safe(res, tmp_elem, NULL);
        #else
        tmp_res = numeric_mul_opt_error(res, tmp_elem, &err);
        if(err) {
            pfree(res);
            pfree(tmp_elem);
            PG_FREE_IF_COPY(arr, 0);    
            ereport(ERROR,
            (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
             errmsg("array_product: An error occurred during a numeric multiplication.")));
            }
        #endif
        pfree(res);
        pfree(tmp_elem);
        res = tmp_res;
    }

    PG_FREE_IF_COPY(arr, 0);
    PG_RETURN_NUMERIC(res);
}

Datum array_sum(PG_FUNCTION_ARGS) {
    Oid const expected_types[] = ARITHM_ALLOWED_TYPES;
    Datum elem;
    bool is_null = false;
    Numeric res;
    Numeric tmp_elem;
    Numeric tmp_res;
    #if PG_VERSION_NUM < 190000
    bool err = false;
    #endif

    AnyArrayType *arr;
    Oid array_elemtype;
    int ndims;
    int nitems;
    int i;

    array_iter iter;
    
    int16 typlen;
    bool typbyval;
    char typalign;

    arr = PG_GETARG_ANY_ARRAY_P(0);
    array_elemtype = AARR_ELEMTYPE(arr);
    ndims = AARR_NDIM(arr);
    nitems = ArrayGetNItems(ndims, AARR_DIMS(arr));

    if (ndims > 2) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_sum: Invalid ndim of array: %d", ndims)));
    }

    if (!type_is_expected(array_elemtype, expected_types, 6)) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_sum: Invalid elemtype of array. OID: %d", array_elemtype)));
    }

    get_typlenbyvalalign(array_elemtype, &typlen, &typbyval, &typalign);

    array_iter_setup(&iter, arr, typlen, typbyval, typalign);


    res = int64_to_numeric(0);

    for(i = 0; i < nitems; i++) {
        elem = array_iter_next(&iter, &is_null, i);

        if (is_null) {
            PG_FREE_IF_COPY(arr, 0);
            pfree(res);
            PG_RETURN_NULL();
        }

        tmp_elem = datum_to_numeric(elem, array_elemtype);
        #if PG_VERSION_NUM >= 190000
        tmp_res = numeric_add_safe(res, tmp_elem, NULL);
        #else
        tmp_res = numeric_add_opt_error(res, tmp_elem, &err);
        if(err) {
            pfree(res);
            pfree(tmp_elem);
            PG_FREE_IF_COPY(arr, 0);     
            ereport(ERROR,
            (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
             errmsg("array_sum: An error occurred during a numeric sum.")));
            }
        #endif
        pfree(res);
        pfree(tmp_elem);
        res = tmp_res;
    }
    PG_FREE_IF_COPY(arr, 0);
    PG_RETURN_NUMERIC(res);
}

Datum array_avg(PG_FUNCTION_ARGS) {
    Oid const expected_types[] = ARITHM_ALLOWED_TYPES;
    Datum elem;
    bool is_null = false;
    Numeric res;
    Numeric total;
    Numeric tmp_elem;
    Numeric tmp_res;
    Numeric final_res;
    uint64_t count;
    #if PG_VERSION_NUM < 190000
    bool err = false;
    #endif
    
    AnyArrayType *arr;
    Oid array_elemtype;
    int ndims;
    int nitems;
    int i;

    array_iter iter;
    
    int16 typlen;
    bool typbyval;
    char typalign;

    arr = PG_GETARG_ANY_ARRAY_P(0);
    array_elemtype = AARR_ELEMTYPE(arr);
    ndims = AARR_NDIM(arr);
    nitems = ArrayGetNItems(ndims, AARR_DIMS(arr));

    if (ndims > 2) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_avg: Invalid ndim of array: %d", ndims)));
    }

    if (!type_is_expected(array_elemtype, expected_types, 6)) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_avg: Invalid elemtype of array. OID: %d", array_elemtype)));
    }

    if (ArrayGetNItems(ndims, AARR_DIMS(arr)) == 0){
        PG_FREE_IF_COPY(arr, 0);
        PG_RETURN_NULL();
    }

    get_typlenbyvalalign(array_elemtype, &typlen, &typbyval, &typalign);
    array_iter_setup(&iter, arr, typlen, typbyval, typalign);


    res = int64_to_numeric(0);
    count = 0;

    for(i = 0; i < nitems; i++) {
        elem = array_iter_next(&iter, &is_null, i);
        if (is_null) {
            PG_FREE_IF_COPY(arr, 0);
            pfree(res);
            PG_RETURN_NULL();
        }

        tmp_elem = datum_to_numeric(elem, array_elemtype);
        #if PG_VERSION_NUM >= 190000
        tmp_res = numeric_add_safe(res, tmp_elem, NULL);
        #else
        tmp_res = numeric_add_opt_error(res, tmp_elem, &err);
        if(err) {
            pfree(res);
            pfree(tmp_elem);
            PG_FREE_IF_COPY(arr, 0);
            ereport(ERROR,
            (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
             errmsg("array_avg: An error occurred during a numeric sum.")));
            }
        #endif
        pfree(res);
        pfree(tmp_elem);
        res = tmp_res;
        count++;
    }
    total = UInt64GetNumeric(count);
    #if PG_VERSION_NUM >= 190000
    final_res = numeric_div_safe(res, total, NULL);
    #else
    final_res = numeric_div_opt_error(res, total, &err);
    if(err) {
        pfree(res);
        pfree(total);
        PG_FREE_IF_COPY(arr, 0);
        ereport(ERROR,
        (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
         errmsg("array_avg: An error occurred during a numeric div.")));
        }
    #endif
    PG_FREE_IF_COPY(arr, 0);
    pfree(res);
    pfree(total);
    PG_RETURN_NUMERIC(final_res);
}

Datum array_min(PG_FUNCTION_ARGS) {
    Oid const expected_types[] = CMP_ALLOWED_TYPES;
    Datum elem;
    Datum res;
    bool is_null = false;
    bool is_first = true;

    
    AnyArrayType *arr;
    Oid array_elemtype;
    int ndims;
    int nitems;
    int i;

    array_iter iter;
    
    int16 typlen;
    bool typbyval;
    char typalign;

    arr = PG_GETARG_ANY_ARRAY_P(0);
    array_elemtype = AARR_ELEMTYPE(arr);
    ndims = AARR_NDIM(arr);
    nitems = ArrayGetNItems(ndims, AARR_DIMS(arr));

    if (ndims > 2) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_min: Invalid ndim of array: %d", ndims)));
    }

    if (!type_is_expected(array_elemtype, expected_types, 6)) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_min: Invalid elemtype of array. OID: %d", array_elemtype)));
    }

    if (ArrayGetNItems(ndims, AARR_DIMS(arr)) == 0){
        PG_FREE_IF_COPY(arr, 0);
        PG_RETURN_NULL();
    }

    get_typlenbyvalalign(array_elemtype, &typlen, &typbyval, &typalign);
    array_iter_setup(&iter, arr, typlen, typbyval, typalign);


    for(i = 0; i < nitems; i++) {
        elem = array_iter_next(&iter, &is_null, i);
        if (is_null) {
            PG_FREE_IF_COPY(arr, 0);
            PG_RETURN_NULL();
        }

        if (is_first){
            res = elem;
            is_first = false;
            continue;
        }

        if (cmp_op(array_elemtype, res, elem, PG_GET_COLLATION()) > 0) res = elem;

    }

    // is_first = true, когда нет не NULL элементов в массиве, либо len = 0
    // таким образом, мы избегаем возврата неинициализированной переменной
    if (is_first) PG_RETURN_NULL();
    PG_RETURN_DATUM(res);
}

Datum array_max(PG_FUNCTION_ARGS) {
    Oid const expected_types[] = CMP_ALLOWED_TYPES;
    Datum elem;
    Datum res;
    bool is_null = false;
    bool is_first = true;

    AnyArrayType *arr;
    Oid array_elemtype;
    int ndims;
    int nitems;
    int i;

    array_iter iter;
    
    int16 typlen;
    bool typbyval;
    char typalign;

    arr = PG_GETARG_ANY_ARRAY_P(0);
    array_elemtype = AARR_ELEMTYPE(arr);
    ndims = AARR_NDIM(arr);
    nitems = ArrayGetNItems(ndims, AARR_DIMS(arr));

    if (ndims > 2) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_max: Invalid ndim of array: %d", ndims)));
    }

    if (!type_is_expected(array_elemtype, expected_types, 6)) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_max: Invalid elemtype of array. OID: %d", array_elemtype)));
    }

    if (ArrayGetNItems(ndims, ARR_DIMS(arr)) == 0){
        PG_FREE_IF_COPY(arr, 0);
        PG_RETURN_NULL();
    }

    get_typlenbyvalalign(array_elemtype, &typlen, &typbyval, &typalign);
    array_iter_setup(&iter, arr, typlen, typbyval, typalign);


    for(i = 0; i < nitems; i++) {
        elem = array_iter_next(&iter, &is_null, i);
        if (is_null) {
            PG_FREE_IF_COPY(arr, 0);
            PG_RETURN_NULL();
        }

        if (is_first){
            res = elem;
            is_first = false;
            continue;
        }

        if (cmp_op(array_elemtype, res, elem, PG_GET_COLLATION()) < 0) res = elem;

    }
    // is_first = true, когда нет не NULL элементов в массиве, либо len = 0
    // таким образом, мы избегаем возврата неинициализированной переменной
    if (is_first) PG_RETURN_NULL();
    PG_RETURN_DATUM(res);
}

Datum array_count(PG_FUNCTION_ARGS) {
    Datum element;
    Oid const expected_types[] = CNT_ALLOWED_TYPES;
    Datum elem;
    uint64_t result = 0;
    bool is_null = false;

    AnyArrayType *arr;
    Oid array_elemtype;
    int ndims;
    int nitems;
    int i;

    array_iter iter;
    
    int16 typlen;
    bool typbyval;
    char typalign;

    arr = PG_GETARG_ANY_ARRAY_P(0);
    array_elemtype = AARR_ELEMTYPE(arr);
    ndims = AARR_NDIM(arr);
    nitems = ArrayGetNItems(ndims, AARR_DIMS(arr));


    element = PG_GETARG_DATUM(1);

    if (ndims > 2) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_count: Invalid ndim of array: %d", ndims)));
    }

    if (!type_is_expected(array_elemtype, expected_types, 9)) {
        ereport(ERROR,
            (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
             errmsg("array_count: Invalid elemtype of array. OID: %d", array_elemtype)));
    }

    get_typlenbyvalalign(array_elemtype, &typlen, &typbyval, &typalign);
    array_iter_setup(&iter, arr, typlen, typbyval, typalign);


    for(i = 0; i < nitems; i++) {
        elem = array_iter_next(&iter, &is_null, i);

        if (is_null) {
            continue;
        }

        if (cmp_op(array_elemtype, element, elem, PG_GET_COLLATION()) == 0) result++;
        if (result > INT32_MAX) ereport(ERROR, 
                                        errcode(ERRCODE_ARRAY_ELEMENT_ERROR),
                                        errmsg("Too many elements in array"));

    }
    PG_FREE_IF_COPY(arr, 0);
    PG_RETURN_INT32((int32)result);
}

Datum array_count_if(PG_FUNCTION_ARGS) {
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Predicate compiled_predicate;
    Datum current_element;
    bool is_null;
    uint64_t result;
    int pred_len;
    Oid const expected_types[] = PRED_ALLOWED_TYPES;

    AnyArrayType *arr;
    Oid array_elemtype;
    int ndims;
    int nitems;
    int i;

    array_iter iter;
    
    int16 typlen;
    bool typbyval;
    char typalign;

    arr = PG_GETARG_ANY_ARRAY_P(0);
    array_elemtype = AARR_ELEMTYPE(arr);
    ndims = AARR_NDIM(arr);
    nitems = ArrayGetNItems(ndims, AARR_DIMS(arr));


    if (ndims > 2)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_count_if: searching elements in manydimensional [when ndim > 2] arrays is not supported yet")));

    if (!type_is_expected(array_elemtype, expected_types, 9))
        ereport(ERROR,
            (errcode(ERRCODE_DATATYPE_MISMATCH),
             errmsg("array_count_if: incorrect datatype")));

    predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));
    elemtype = array_elemtype;
    result = 0;
    get_typlenbyvalalign(array_elemtype, &typlen, &typbyval, &typalign);
    array_iter_setup(&iter, arr, typlen, typbyval, typalign);

    pred_len = prepare_predicate(predicate, elemtype, &compiled_predicate, collation);
    if (pred_len == 0) {
        PG_FREE_IF_COPY(arr, 0);
        PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
        PG_RETURN_INT32(0);
    }
    for(i = 0; i < nitems; i++) {
        current_element = array_iter_next(&iter, &is_null, i);

        if (is_null) {
            
            continue;
        }

        if (execute_predicate(&compiled_predicate, current_element,elemtype, collation)) result++;
        if (result > INT32_MAX) ereport(ERROR, 
                                        errcode(ERRCODE_ARRAY_ELEMENT_ERROR),
                                        errmsg("Too many elements in array"));
    }

    PG_FREE_IF_COPY(arr, 0);
    PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
    free_predicate(&compiled_predicate);
    PG_RETURN_INT32((int32)result);
}


Datum array_any(PG_FUNCTION_ARGS) {
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Predicate compiled_predicate;
    Datum current_element;
    bool is_null;
    int pred_len;
    Oid const expected_types[] = PRED_ALLOWED_TYPES;

    AnyArrayType *arr;
    Oid array_elemtype;
    int ndims;
    int nitems;
    int i;

    array_iter iter;
    
    int16 typlen;
    bool typbyval;
    char typalign;

    arr = PG_GETARG_ANY_ARRAY_P(0);
    array_elemtype = AARR_ELEMTYPE(arr);
    ndims = AARR_NDIM(arr);
    nitems = ArrayGetNItems(ndims, AARR_DIMS(arr));

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL();

    if (ndims > 2)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_any: searching elements in manydimensional [when ndim > 2] arrays is not supported yet")));

    if (!type_is_expected(array_elemtype, expected_types, 9))
        ereport(ERROR,
            (errcode(ERRCODE_DATATYPE_MISMATCH),
             errmsg("array_any: incorrect datatype")));

    predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));
    elemtype = array_elemtype;

    get_typlenbyvalalign(array_elemtype, &typlen, &typbyval, &typalign);
    array_iter_setup(&iter, arr, typlen, typbyval, typalign);

    pred_len = prepare_predicate(predicate, elemtype, &compiled_predicate, collation);
    if (pred_len == 0) {
        PG_FREE_IF_COPY(arr, 0);
        PG_FREE_IF_COPY(DatumGetPointer(predicate), 1); 
        PG_RETURN_BOOL(false);
    }
    for(i = 0; i < nitems; i++) {
        current_element = array_iter_next(&iter, &is_null, i);
        if (is_null) {
            continue;
        }

        if (execute_predicate(&compiled_predicate, current_element,elemtype, collation)) {
            PG_FREE_IF_COPY(arr, 0);
            PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
            free_predicate(&compiled_predicate);
            PG_RETURN_BOOL(true);
        }
    }

    PG_FREE_IF_COPY(arr, 0);
    PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
    free_predicate(&compiled_predicate);
    PG_RETURN_BOOL(false);
}

Datum array_all(PG_FUNCTION_ARGS) {
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Predicate compiled_predicate;
    Datum current_element;
    bool is_null;
    int pred_len;
    int arr_len;
    Oid const expected_types[] = PRED_ALLOWED_TYPES;

    AnyArrayType *arr;
    Oid array_elemtype;
    int ndims;
    int nitems;
    int i;

    array_iter iter;
    
    int16 typlen;
    bool typbyval;
    char typalign;

    arr = PG_GETARG_ANY_ARRAY_P(0);
    array_elemtype = AARR_ELEMTYPE(arr);
    ndims = AARR_NDIM(arr);
    nitems = ArrayGetNItems(ndims, AARR_DIMS(arr));

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL();
    arr_len = ArrayGetNItems(ndims, AARR_DIMS(arr));

    if (ndims > 2)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_all: searching elements in manydimensional [when ndim > 2] arrays is not supported yet")));

    if (!type_is_expected(array_elemtype, expected_types, 9))
        ereport(ERROR,
            (errcode(ERRCODE_DATATYPE_MISMATCH),
             errmsg("array_all: incorrect datatype")));

    predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));
    elemtype = array_elemtype;
    get_typlenbyvalalign(array_elemtype, &typlen, &typbyval, &typalign);
    array_iter_setup(&iter, arr, typlen, typbyval, typalign);

    pred_len = prepare_predicate(predicate, elemtype, &compiled_predicate, collation);
    if (pred_len == 0) {
        PG_FREE_IF_COPY(arr, 0);
        PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
        if(arr_len == 0) PG_RETURN_BOOL(true);
        PG_RETURN_BOOL(false);
    }
    for(i = 0; i < nitems; i++) {
        current_element = array_iter_next(&iter, &is_null, i);
        if (is_null) {
            continue;
        }

        if (!execute_predicate(&compiled_predicate, current_element,elemtype, collation)){
            PG_FREE_IF_COPY(arr, 0);
            PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
            free_predicate(&compiled_predicate);
            PG_RETURN_BOOL(false);
        }
    }

    PG_FREE_IF_COPY(arr, 0);
    PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
    free_predicate(&compiled_predicate);
    PG_RETURN_BOOL(true);
}

Datum array_none(PG_FUNCTION_ARGS) {
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Predicate compiled_predicate;
    Datum current_element;
    bool is_null;
    int pred_len;
    Oid const expected_types[] = PRED_ALLOWED_TYPES;

    AnyArrayType *arr;
    Oid array_elemtype;
    int ndims;
    int nitems;
    int i;

    array_iter iter;
    
    int16 typlen;
    bool typbyval;
    char typalign;

    arr = PG_GETARG_ANY_ARRAY_P(0);
    array_elemtype = AARR_ELEMTYPE(arr);
    ndims = AARR_NDIM(arr);
    nitems = ArrayGetNItems(ndims, AARR_DIMS(arr));

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL(); // функция STRIСT, этого никогда не произойдёт

    if (ndims > 2)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_none: searching elements in manydimensional [when ndim > 2] arrays is not supported yet")));

    if (!type_is_expected(array_elemtype, expected_types, 9))
        ereport(ERROR,
            (errcode(ERRCODE_DATATYPE_MISMATCH),
             errmsg("array_none: incorrect datatype")));
             
    elemtype = array_elemtype;
    predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));
    pred_len = prepare_predicate(predicate, elemtype, &compiled_predicate, collation);
    if (pred_len == 0) {
        PG_FREE_IF_COPY(arr, 0);
        PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
        PG_RETURN_BOOL(true);
    }
    get_typlenbyvalalign(array_elemtype, &typlen, &typbyval, &typalign);
    array_iter_setup(&iter, arr, typlen, typbyval, typalign);
    
    for(i = 0; i < nitems; i++) {
        current_element = array_iter_next(&iter, &is_null, i);
        if (is_null) {

            continue;
        }

        if (execute_predicate(&compiled_predicate, current_element, elemtype, collation)){
            PG_FREE_IF_COPY(arr, 0);
            PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
            free_predicate(&compiled_predicate);
            PG_RETURN_BOOL(false);
        }
    }

    PG_FREE_IF_COPY(arr, 0);
    PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
    free_predicate(&compiled_predicate);
    PG_RETURN_BOOL(true);
}
#include "array_functions.h"
#include "postgres.h"
#include "fmgr.h"
#include "utils/array.h"
#include "utils/numeric.h"
#include "catalog/pg_type_d.h"

PG_MODULE_MAGIC;

void _PG_init(void);
void _PG_fini(void);

void _PG_init(void) {}
void _PG_fini(void) {}

/*
    Эту функцию потом вынести в utils:
*/

bool type_is_expected(Oid type_oid, Oid* expected_types, uint32 len) {
    for(int i = 0; i < len; i++) {
        if (expected_types[i] == type_oid) return true;
    }
    return false;
}

PG_FUNCTION_INFO_V1(array_product);
Datum array_product(PG_FUNCTION_ARGS) {
    ArrayType *arr = PG_GETARG_ARRAYTYPE_P(0);
    if (arr->ndim > 1) {
        ereport(ERROR, errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
            errmsg("Invalid ndim of array: %d", arr->ndim));
    }
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID};
    if (!type_is_expected(arr->elemtype, &expected_types, 6)) {
        ereport(ERROR, errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
            errmsg("Invalid elemtype of array. OID: %d", arr->elemtype));
    }
    ArrayIterator arr_it = array_create_iterator(arr, 0, NULL);

    // для итератора
    Datum elem;
    bool is_null;
    int pos = 0;

    //результирующая переменная, тут накопим результат умножений. Инициализируем её значением 1
    Numeric res = int64_to_numeric(1);

    bool err = false;
    while(array_iterate(arr_it, &elem, &is_null)) {
        pos++;

        if (is_null) {
            array_free_iterator(arr_it);
            PG_RETURN_NULL();
        }

        numeric_mul_opt_error(res, DatumGetNumeric(elem), &err);
        if(err) ereport(ERROR, ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE,
            errmsg("An error occurred during a numeric multiplication. A variable overflow likely took place."));

    }

    array_free_iterator(arr_it);

    PG_RETURN_NUMERIC(res);
}


PG_FUNCTION_INFO_V1(array_sum);
Datum array_sum(PG_FUNCTION_ARGS) {
    ArrayType *arr = PG_GETARG_ARRAYTYPE_P(0);
    if (arr->ndim > 1) {
        ereport(ERROR, errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
            errmsg("Invalid ndim of array: %d", arr->ndim));
    }
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID};
    if (!type_is_expected(arr->elemtype, &expected_types, 6)) {
        ereport(ERROR, errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
            errmsg("Invalid elemtype of array. OID: %d", arr->elemtype));
    }
    ArrayIterator arr_it = array_create_iterator(arr, 0, NULL);

    // для итератора
    Datum elem;
    bool is_null;
    int pos = 0;

    //результирующая переменная, тут накопим результат сложений. Инициализируем её значением 0
    Numeric res = int64_to_numeric(0);

    bool err = false;
    while(array_iterate(arr_it, &elem, &is_null)) {
        pos++;

        if (is_null) {
            array_free_iterator(arr_it);
            PG_RETURN_NULL();
        }

        numeric_sum_opt_error(res, DatumGetNumeric(elem), &err);
        if(err) ereport(ERROR, ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE,
            errmsg("An error occurred during a numeric sum. A variable overflow likely took place."));

    }

    array_free_iterator(arr_it);

    PG_RETURN_NUMERIC(res);
}

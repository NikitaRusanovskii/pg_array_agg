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
    Вспомогательные функции потом вынести в utils:
*/

extern Datum float4_numeric(PG_FUNCTION_ARGS);
extern Datum float8_numeric(PG_FUNCTION_ARGS);

static bool type_is_expected(Oid type_oid, Oid* expected_types, uint32 len) {
    for(int i = 0; i < len; i++) {
        if (expected_types[i] == type_oid) return true;
    }
    return false;
}

static inline Numeric datum_to_numeric(Datum element, Oid type_oid) {
    Numeric tmp;
    switch(type_oid) {
        case INT2OID:
            tmp = int64_to_numeric((int64)DatumGetInt16(element));
            break;
        case INT4OID:
            tmp = int64_to_numeric((int64)DatumGetInt32(element));
            break;
        case INT8OID:
            tmp = int64_to_numeric((int64)DatumGetInt64(element));
            break;
        
        case FLOAT4OID: // как привести числа с плавающей точкой к numeric?
            tmp = Float4GetDatum(DirectFunctionCall1(float4_numeric, DatumGetFloat4(element)));
            break;
        case FLOAT8OID:
            tmp = Float4GetDatum(DirectFunctionCall1(float4_numeric, DatumGetFloat4(element)));
            break;
        case NUMERICOID:
            tmp = DatumGetNumeric(element);
            break; 
        default:
            ereport(ERROR, errcode(ERRCODE_WRONG_OBJECT_TYPE),
                errmsg("The function does not support this OID: %d", type_oid));
            tmp = int64_to_numeric(0);
    }
    return tmp;
}

PG_FUNCTION_INFO_V1(array_product);
Datum array_product(PG_FUNCTION_ARGS) {
    if (PG_NARGS() == 0) PG_RETURN_NULL();
    if (PG_NARGS() > 1) ereport(ERROR, errcode(ERRCODE_TOO_MANY_ARGUMENTS),
    errmsg("To many arguments: %d, functions need: %d", PG_NARGS(), 1));
    
    ArrayType *arr = PG_GETARG_ARRAYTYPE_P(0);
    
    if (ARR_NDIM(arr) > 1) {
        ereport(ERROR, errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
            errmsg("Invalid ndim of array: %d", ARR_NDIM(arr)));
    }
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID}; // они должны совпадать с функцией валидатором. Это небезопасно с точки зрения рефакторинга. Надо уединить их в одном месте
    if (!type_is_expected(arr->elemtype, expected_types, 6)) {
        ereport(ERROR, errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
            errmsg("Invalid elemtype of array. OID: %d", arr->elemtype));
    }
    ArrayIterator arr_it = array_create_iterator(arr, 0, NULL);

    // для итератора
    Datum elem;
    bool is_null;

    //результирующая переменная, тут накопим результат умножений. Инициализируем её значением 1
    Numeric res = int64_to_numeric(1);

    bool err = false;
    while(array_iterate(arr_it, &elem, &is_null)) {

        if (is_null) {
            array_free_iterator(arr_it);
            PG_RETURN_NULL();
        }
        res = numeric_mul_opt_error(res, datum_to_numeric(elem, arr->elemtype), &err); // вот здесь может быть ошибка, тогда надо освободить итератор
        if(err) ereport(ERROR, errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
            errmsg("An error occurred during a numeric multiplication. A variable overflow likely took place."));

    }

    array_free_iterator(arr_it);

    PG_RETURN_NUMERIC(res);
}


PG_FUNCTION_INFO_V1(array_sum);
Datum array_sum(PG_FUNCTION_ARGS) {
    if (PG_NARGS() == 0) PG_RETURN_NULL();
    if (PG_NARGS() > 1) ereport(ERROR, errcode(ERRCODE_TOO_MANY_ARGUMENTS),
    errmsg("To many arguments: %d, functions need: %d", PG_NARGS(), 1));
    
    ArrayType *arr = PG_GETARG_ARRAYTYPE_P(0);
    
    if (ARR_NDIM(arr) > 1) {
        ereport(ERROR, errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
            errmsg("Invalid ndim of array: %d", ARR_NDIM(arr)));
    }
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID};
    if (!type_is_expected(arr->elemtype, expected_types, 6)) {
        ereport(ERROR, errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
            errmsg("Invalid elemtype of array. OID: %d", arr->elemtype));
    }
    ArrayIterator arr_it = array_create_iterator(arr, 0, NULL);

    // для итератора
    Datum elem;
    bool is_null;
    //результирующая переменная, тут накопим результат сложений. Инициализируем её значением 0
    Numeric res = int64_to_numeric(0);

    bool err = false;
    while(array_iterate(arr_it, &elem, &is_null)) {

        if (is_null) {
            array_free_iterator(arr_it);
            PG_RETURN_NULL();
        }

        res = numeric_add_opt_error(res, datum_to_numeric(elem, arr->elemtype), &err); // вот здесь может быть ошибка, тогда надо освободить итератор
        if(err) ereport(ERROR, errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
            errmsg("An error occurred during a numeric sum. A variable overflow likely took place."));

    }

    array_free_iterator(arr_it);

    PG_RETURN_NUMERIC(res);
}


PG_FUNCTION_INFO_V1(array_avg);
Datum array_avg(PG_FUNCTION_ARGS) {
    if (PG_NARGS() == 0) PG_RETURN_NULL();
    if (PG_NARGS() > 1) ereport(ERROR, errcode(ERRCODE_TOO_MANY_ARGUMENTS),
    errmsg("To many arguments: %d, functions need: %d", PG_NARGS(), 1));
    
    ArrayType *arr = PG_GETARG_ARRAYTYPE_P(0);
    
    if (ARR_NDIM(arr) > 1) {
        ereport(ERROR, errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
            errmsg("Invalid ndim of array: %d", ARR_NDIM(arr)));
    }
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID};
    if (!type_is_expected(arr->elemtype, expected_types, 6)) {
        ereport(ERROR, errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
            errmsg("Invalid elemtype of array. OID: %d", arr->elemtype));
    }

    if (ArrayGetNItems(ARR_NDIM(arr), ARR_DIMS(arr)) == 0) PG_RETURN_NULL();

    ArrayIterator arr_it = array_create_iterator(arr, 0, NULL);

    // для итератора
    Datum elem;
    bool is_null;
    //результирующая переменная, тут накопим результат сложений. Инициализируем её значением 0
    Numeric res = int64_to_numeric(0);
    Numeric count = int64_to_numeric(0); // будем считать кол-во элементов самостоятельно, так как если изменится поведение относительно NULL элементов, ручной подсчёт более гибкий
    bool err = false;
    while(array_iterate(arr_it, &elem, &is_null)) {

        if (is_null) {
            array_free_iterator(arr_it);
            PG_RETURN_NULL();
        }
        res = numeric_add_opt_error(res, datum_to_numeric(elem, arr->elemtype), &err); // вот здесь может быть ошибка, тогда надо освободить итератор
        if(err) ereport(ERROR, errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
            errmsg("An error occurred during a numeric sum. A variable overflow likely took place."));
        count = numeric_add_opt_error(count, int64_to_numeric(1), &err); // есть ли способ проще? инкремент?
        if(err) ereport(ERROR, errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
            errmsg("An error occurred during a numeric sum. A variable overflow likely took place."));
    }

    array_free_iterator(arr_it);
    res = numeric_div_opt_error(res, count, &err);
    if (err) ereport(ERROR, errcode(ERRCODE_DIVISION_BY_ZERO),
                    errmsg("Division by zero error"));
    PG_RETURN_NUMERIC(res);
}


PG_FUNCTION_INFO_V1(array_min);
Datum array_min(PG_FUNCTION_ARGS) {
    if (PG_NARGS() == 0) PG_RETURN_NULL();
    if (PG_NARGS() > 1) ereport(ERROR, errcode(ERRCODE_TOO_MANY_ARGUMENTS),
    errmsg("To many arguments: %d, functions need: %d", PG_NARGS(), 1));
    
    ArrayType *arr = PG_GETARG_ARRAYTYPE_P(0);
    
    if (ARR_NDIM(arr) > 1) {
        ereport(ERROR, errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
            errmsg("Invalid ndim of array: %d", ARR_NDIM(arr)));
    }
    Oid expected_types[] = {INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID};
    if (!type_is_expected(arr->elemtype, expected_types, 6)) {
        ereport(ERROR, errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
            errmsg("Invalid elemtype of array. OID: %d", arr->elemtype));
    }

    if (ArrayGetNItems(ARR_NDIM(arr), ARR_DIMS(arr)) == 0) PG_RETURN_NULL();

    ArrayIterator arr_it = array_create_iterator(arr, 0, NULL);

    // для итератора
    Datum elem;
    bool is_null;
    //результирующая переменная, тут накопим результат сложений. Инициализируем её значением 0
    Numeric res = int64_to_numeric(0);
    Numeric count = int64_to_numeric(0); // будем считать кол-во элементов самостоятельно, так как если изменится поведение относительно NULL элементов, ручной подсчёт более гибкий
    bool err = false;
    while(array_iterate(arr_it, &elem, &is_null)) {

        if (is_null) {
            array_free_iterator(arr_it);
            PG_RETURN_NULL();
        }
        res = numeric_add_opt_error(res, datum_to_numeric(elem, arr->elemtype), &err); // вот здесь может быть ошибка, тогда надо освободить итератор
        if(err) ereport(ERROR, errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
            errmsg("An error occurred during a numeric sum. A variable overflow likely took place."));
        count = numeric_add_opt_error(count, int64_to_numeric(1), &err); // есть ли способ проще? инкремент?
        if(err) ereport(ERROR, errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
            errmsg("An error occurred during a numeric sum. A variable overflow likely took place."));
    }

    array_free_iterator(arr_it);
    res = numeric_div_opt_error(res, count, &err);
    if (err) ereport(ERROR, errcode(ERRCODE_DIVISION_BY_ZERO),
                    errmsg("Division by zero error"));
    PG_RETURN_NUMERIC(res);
}

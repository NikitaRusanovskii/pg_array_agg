#include "postgres.h"
#include "utils/numeric.h"
#include "catalog/pg_type_d.h"
#include "utils/float.h"
#include "utils/numeric.h"
#include "pg_array_utils.h"


extern Datum float4_numeric(PG_FUNCTION_ARGS);
extern Datum float8_numeric(PG_FUNCTION_ARGS);
extern Datum numeric_cmp(PG_FUNCTION_ARGS);

bool type_is_expected(Oid type_oid, Oid* expected_types, uint32 len) {
    for(int i = 0; i < len; i++) {
        if (expected_types[i] == type_oid) return true;
    }
    return false;
}

Numeric datum_to_numeric(Datum element, Oid type_oid) {
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
            tmp = DatumGetNumeric(DirectFunctionCall1(float4_numeric, element));
            break;
        case FLOAT8OID:
            tmp = DatumGetNumeric(DirectFunctionCall1(float8_numeric, element));
            break;
        case NUMERICOID:
            tmp = DatumGetNumeric(element);
            break; 
        default:
            ereport(ERROR, errcode(ERRCODE_WRONG_OBJECT_TYPE),
                errmsg("The function does not support this OID: %d", type_oid));
            tmp = int64_to_numeric(0);
            break;
    }
    return tmp;
}

int32 universal_cmp(Oid type_oid, Datum first, Datum second) {
    /*
        returns:
            0: if first == second
            < 0: if first < second
            > 0: if first > second
    */
    int32 res = 0;
    switch(type_oid) {
        case INT2OID:
            int16 a16 = DatumGetInt16(first);
            int16 b16 = DatumGetInt16(second);
            int16 tmp16 = (a16 < b16) - (a16 > b16);
            if (tmp16 > 0) res = 1;
            else if (tmp16 == 0) res = 0;
            else if (tmp16 < 0) res = -1;
            break;
        case INT4OID:
            int32 a32 = DatumGetInt32(first);
            int32 b32 = DatumGetInt32(second);
            int32 tmp32 = (a32 < b32) - (a32 > b32);
            if (tmp32 > 0) res = 1;
            else if (tmp32 == 0) res = 0;
            else if (tmp32 < 0) res = -1;
            break;
        case INT8OID:
            int64 a64 = DatumGetInt64(first);
            int64 b64 = DatumGetInt64(second);
            int64 tmp64 = (a64 < b64) - (a64 > b64);
            if (tmp64 > 0) res = 1;
            else if (tmp64 == 0) res = 0;
            else if (tmp64 < 0) res = -1;
            break;
        
        case FLOAT4OID: // как привести числа с плавающей точкой к numeric?
            float4 fa32 = DatumGetFloat4(first);
            float4 fb32 = DatumGetFloat4(second);
            if (isnan(fa32)) {
                if (isnan(fb32)) res = 0;
                res = 1;
            }
            else if (isnan(fb32)) res = -1;
            else {
                float4 ftmp32 = (fa32 < fb32) - (fa32 > fb32);
                if (ftmp32 > 0) res = 1;
                else if (ftmp32 == 0) res = 0;
                else if (ftmp32 < 0) res = -1;
            }
            break;
        case FLOAT8OID:
            float8 fa64 = DatumGetFloat8(first);
            float8 fb64 = DatumGetFloat8(second);
            if (isnan(fa64)) {
                if (isnan(fb64)) res = 0;
                res = 1;
            }
            else if (isnan(fb64)) res = -1;
            else {
                float8 ftmp64 = (fa64 < fb64) - (fa64 > fb64);
                if (ftmp64 > 0) res = 1;
                else if (ftmp64 == 0) res = 0;
                else if (ftmp64 < 0) res = -1;
            }
            break;
        case NUMERICOID:
            res = DatumGetInt32(DirectFunctionCall2(numeric_cmp, second, first));
            break; 
        default:
            ereport(ERROR, errcode(ERRCODE_WRONG_OBJECT_TYPE),
                errmsg("The function does not support this OID: %d", type_oid));
            break;
    }
    return res;
}
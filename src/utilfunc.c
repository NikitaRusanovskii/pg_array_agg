#include "postgres.h"
#include "fmgr.h"
#include "utils/lsyscache.h"
#include "utils/elog.h"
#include "utils/builtins.h"
#include "utils/typcache.h"
#include "utils/numeric.h"
#include "catalog/pg_type_d.h"
#include "utilfunc.h"
#include "parser_util.h"
#include "scaner.h"
#include "parser.tab.h"

bool type_is_expected(Oid type_oid, const Oid* expected_types, uint32 len) {
    for (uint32 i = 0; i < len; i++) {
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

        case FLOAT4OID:
            tmp = DatumGetNumeric(DirectFunctionCall1(float4_numeric, element));
            break;
        case FLOAT8OID:
            tmp = DatumGetNumeric(DirectFunctionCall1(float8_numeric, element));
            break;
        case NUMERICOID:
            tmp = (Numeric)PG_DETOAST_DATUM_COPY(element);
            break;
        default:
            ereport(ERROR,
                (errcode(ERRCODE_WRONG_OBJECT_TYPE),
                 errmsg("datum_to_numeric: The function does not support this OID: %d", type_oid)));
            pg_unreachable();
    }
    return tmp;
}

Numeric UInt64GetNumeric(uint64_t num) {
    char uint_buf[20] = {0};
    sprintf(uint_buf, UINT64_FORMAT, num);
    return DatumGetNumeric(DirectFunctionCall3(numeric_in, CStringGetDatum(uint_buf), ObjectIdGetDatum(InvalidOid), Int32GetDatum(-1)));
}

Datum string_to_datum(const char* str, int len, Oid target_oid) {
    Oid input;
    Oid param;
    Datum res;
    char* str_ = pnstrdup(str, len);
    getTypeInputInfo(target_oid, &input, &param);
    res = OidInputFunctionCall(input, str_, param, -1);
    pfree(str_);
    return res;
}

int32 cmp_op(Oid target_oid, Datum first, Datum second, Oid collid) {
    TypeCacheEntry* typentry;
    typentry = lookup_type_cache(target_oid, TYPECACHE_CMP_PROC_FINFO);
    if(!OidIsValid(typentry->cmp_proc))
        ereport(ERROR,
            (errcode(ERRCODE_UNDEFINED_FUNCTION),
             errmsg("cmp_op: Invalid oid of cmp func")));

    return DatumGetInt32(FunctionCall2Coll(&typentry->cmp_proc_finfo, collid, first, second));

}

static bool datum_is_zero(Datum value, Oid type_oid, Oid collid) {
    Datum zero;
    bool res;
    switch (type_oid) {
        case INT2OID:
            zero = Int16GetDatum(0);
            break;
        case INT4OID:
            zero = Int32GetDatum(0);
            break;
        case INT8OID:
            zero = Int64GetDatum(0);
            break;
        case FLOAT4OID:
            zero = Float4GetDatum(0.0f);
            break;
        case FLOAT8OID:
            zero = Float8GetDatum(0.0);
            break;
        case NUMERICOID:
            zero = DirectFunctionCall1(int8_numeric, Int64GetDatum(0));
            break;
        default:
            ereport(ERROR,
                    (errcode(ERRCODE_DATATYPE_MISMATCH),
                     errmsg("datum_is_zero: incorrect datatype")));
            pg_unreachable();
    }
    res = cmp_op(type_oid, value, zero, collid) == 0;
    if (type_oid == NUMERICOID) pfree(DatumGetPointer(zero));
    return res;
}

Datum mod_op(Oid target_oid, Datum first, Datum second, Oid collid) {
    Datum result;

    if (datum_is_zero(second, target_oid, collid))
        ereport(ERROR,
            (errcode(ERRCODE_DIVISION_BY_ZERO),
             errmsg("process: division by zero")));

    switch(target_oid) {
        case NUMERICOID: {
            result = DirectFunctionCall2(numeric_mod, first, second);
            break;
        }
        case INT2OID: {
            if (DatumGetInt16(first) == INT16_MIN && DatumGetInt16(second) == -1) {
                result = Int16GetDatum(0);
                break;
            }
            result = Int16GetDatum(DatumGetInt16(first) % DatumGetInt16(second));
            break;
        }
        case INT4OID: {
            if (DatumGetInt32(first) == INT32_MIN && DatumGetInt32(second) == -1) {
                result = Int32GetDatum(0);
                break;
            }
            result = Int32GetDatum(DatumGetInt32(first) % DatumGetInt32(second));
            break;
        }
        case INT8OID: {
            if (DatumGetInt64(first) == INT64_MIN && DatumGetInt64(second) == -1) {
                result = Int64GetDatum(0);
                break;
            }
            result = Int64GetDatum(DatumGetInt64(first) % DatumGetInt64(second));
            break;
        }
        default:
            ereport(ERROR,
                (errcode(ERRCODE_WRONG_OBJECT_TYPE),
                errmsg("mod_op: wrong oid: %d", target_oid)));
            pg_unreachable();
    }
    return result;
}
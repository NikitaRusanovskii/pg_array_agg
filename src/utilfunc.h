#pragma once
#include "postgres.h"
#include "utils/numeric.h"
#include "parser_util.h"



bool		pgaa_type_is_expected(Oid type_oid, const Oid *expected_types, uint32 len);
int32		pgaa_cmp_op(Oid target_oid, Datum first, Datum second, Oid collid);
Numeric		pgaa_datum_to_numeric(Datum element, Oid type_oid);
Numeric		pgaa_uint64_get_numeric(uint64 num);
Datum		pgaa_string_to_datum(const char *str, int len, Oid target_oid);
Datum		pgaa_mod_op(Oid target_oid, Datum first, Datum second, Oid collid);

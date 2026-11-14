#pragma once
#include "postgres.h"
#include "utils/numeric.h"
#include "parser_util.h"



bool		type_is_expected(Oid type_oid, const Oid *expected_types, uint32 len);
int32		cmp_op(Oid target_oid, Datum first, Datum second, Oid collid);
Numeric		datum_to_numeric(Datum element, Oid type_oid);
Numeric		UInt64GetNumeric(uint64_t num);
Datum		string_to_datum(const char *str, int len, Oid target_oid);
Datum		mod_op(Oid target_oid, Datum first, Datum second, Oid collid);

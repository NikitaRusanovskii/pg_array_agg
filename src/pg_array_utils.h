#pragma once

#include "postgres.h"
#include "utils/numeric.h"

extern Datum float4_numeric(PG_FUNCTION_ARGS);
extern Datum float8_numeric(PG_FUNCTION_ARGS);

bool type_is_expected(Oid type_oid, Oid* expected_types, uint32 len);
Numeric datum_to_numeric(Datum element, Oid type_oid);

int32 universal_cmp(Oid type_oid, Datum first, Datum second);

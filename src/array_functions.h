#pragma once
#include "postgres.h"
#include "fmgr.h"

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
#pragma once
#include "postgres.h"
#include "utils/numeric.h"
#include "parser_util.h"



bool type_is_expected(Oid type_oid, const Oid* expected_types, uint32 len);
int32 cmp_op(Oid target_oid, Datum first, Datum second, Oid collid);
Numeric datum_to_numeric(Datum element, Oid type_oid);
int32 prepare_predicate(Datum predicate_text, Oid target_oid, Predicate* predicate, Oid collid);
bool execute_predicate(const Predicate* predicate, Datum element, Oid target_oid, Oid collid);
void free_predicate(const Predicate* predicate);
Numeric UInt64GetNumeric(uint64_t num);

#pragma once
#include "postgres.h"
#include "parser_util.h"
int pgaa_scaner(char* predicate_string, int32 len, PGAA_Token* result, int32 token_len);
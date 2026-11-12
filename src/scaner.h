#pragma once
#include "postgres.h"
#include "parser_util.h"
int scaner(char* predicate_string, int32 len, Token* result, int32 token_len);
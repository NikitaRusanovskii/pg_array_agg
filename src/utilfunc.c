#include "postgres.h"
#include "fmgr.h"
#include "utils/lsyscache.h"
#include "utils/elog.h"
#include "utils/builtins.h"
#include "utils/typcache.h"
#include "utils/numeric.h"
#include "catalog/pg_type_d.h"
#include "utilfunc.h"


bool type_is_expected(Oid type_oid, const Oid* expected_types, uint32 len) {
    for (int i = 0; i < len; i++) {
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
            tmp = DatumGetNumeric(element);
            break;
        default:
            ereport(ERROR, errcode(ERRCODE_WRONG_OBJECT_TYPE),
                errmsg("datum_to_numeric: The function does not support this OID: %d", type_oid));
            tmp = int64_to_numeric(0);
            break;
    }
    return tmp;
}

Datum string_to_datum(char* str, int len, Oid target_oid) {
    Oid input;
    Oid param;
    Datum res;
    char* str_ = pnstrdup(str, len);
    getTypeInputInfo(target_oid, &input, &param);
    res = OidInputFunctionCall(input, str_, param, -1);
    pfree(str_);
    return res;
}

Token construct_token(TokenType type, const char* token, int32 lenght) {
    return (Token){type, token, lenght};
}

int32 lexical_analyze(char* predicate_string, int32 len, Token* result, int32 token_len) {
    memset(result, 0, token_len * sizeof(*result));
    int32 token_position = 0;
    for (int32 i = 0; i < len; i++) {
        if (token_position >= token_len) break; // вместо ошибки остановим цикл
        if (predicate_string[i] ==  ' ') continue;
        else if (predicate_string[i] ==  'x' && token_position == 0) {
            result[token_position++] = construct_token(T_VARIABLE, &predicate_string[i], 1);
        }
        else if (predicate_string[i] ==  '%') {
            result[token_position++] = construct_token(T_MOD, &predicate_string[i], 1);
        }
        else if (predicate_string[i] ==  '=' && i < (len - 1) && predicate_string[i+1] == '=') {
            result[token_position++] = construct_token(T_EQ, &predicate_string[i], 2);
            i++;
        }
        else if (predicate_string[i] ==  '!' && i < (len - 1) && predicate_string[i+1] == '=') {
            result[token_position++] = construct_token(T_NEQ, &predicate_string[i], 2);
            i++;
        }
        else if (predicate_string[i] ==  '<' && i < (len - 1) && predicate_string[i+1] == '=') {
            result[token_position++] = construct_token(T_LEE, &predicate_string[i], 2);
            i++;
        }
        else if (predicate_string[i] ==  '>' && i < (len - 1) && predicate_string[i+1] == '=') {
            result[token_position++] = construct_token(T_GRE, &predicate_string[i], 2);
            i++;
        }
        else if (predicate_string[i] ==  '<') {
            result[token_position++] = construct_token(T_LE, &predicate_string[i], 1);
        }
        else if (predicate_string[i] ==  '>') {
            result[token_position++] = construct_token(T_GR, &predicate_string[i], 1);
        }
        else if (predicate_string[i] ==  '=') {
            result[token_position++] = construct_token(T_SEQ, &predicate_string[i], 1);
        }
        else if (predicate_string[i] == '"') {
            int32 start = i;
            i++;
            bool success = false;
            while(i < len) {
                if (predicate_string[i] == '"') {
                    result[token_position++] = construct_token(T_STRING, &predicate_string[start+1], i - start - 1);
                    success = true;
                    i++;
                    break;
                }
                i++;
            }
            if (!success) ereport(ERROR, ERRCODE_SYNTAX_ERROR, errmsg("lexical_analyze: invalid predicate string"));
        }
        else if (predicate_string[i] == '\'') {
            // Для '+-Infinity', 'NaN', 
            int32 start = i;
            i++;
            bool success = false;
            while(i < len) {
                if (predicate_string[i] == '\'') {
                    result[token_position++] = construct_token(T_NUMBER, &predicate_string[start+1], i - start - 1);
                    success = true;
                    i++;
                    break;
                }
                i++;
            }
            if (!success)
                ereport(ERROR, ERRCODE_SYNTAX_ERROR, errmsg("lexical_analyze: invalid predicate string"));
        }
        else {
            int32 start = i;

            while (i < len &&
                   predicate_string[i] != '=' &&
                   predicate_string[i] != '<' &&
                   predicate_string[i] != '>' &&
                   predicate_string[i] != '!' &&
                   predicate_string[i] != '%' &&
                   predicate_string[i] != ' ') i++;

            int32 token_length = i - start;

            if (token_length == 0)
                ereport(ERROR, ERRCODE_SYNTAX_ERROR, errmsg("lexical_analyze: invalid token_lenght = 0"));

            result[token_position++] = construct_token(T_NUMBER, &predicate_string[start], token_length);
        }
    }
    result[token_position++] = construct_token(T_END, NULL, 0);
    return token_position;
}

// синтаксический разбор
int32 syntaxical_analize(Token* parsed_tokens, int32 len, ParsedPredicate* predicate) {
    // %: x op const = const 5 токенов + конечный = 6 токенов
    // др: x op const
    int32 modlen = 6;
    int32 cmplen = 4;
    int32 varpos = 0;
    int32 op1pos = 1;
    int32 const1pos = 2;
    int32 op2pos = 3;
    int32 const2pos = 4;
    memset(predicate, 0, sizeof(*predicate));
    if (len <= 0) ereport(ERROR, ERRCODE_SYNTAX_ERROR, errmsg("syntaxical_analize: Invalid token array len: %d", len));
    if (parsed_tokens[len-1].type != T_END || (len != cmplen && len != modlen))
        ereport(ERROR, ERRCODE_SYNTAX_ERROR, errmsg("syntaxical_analize: Invalid token array format. Array len: %d", len));
    if (parsed_tokens[varpos].type != T_VARIABLE)
        ereport(ERROR, ERRCODE_SYNTAX_ERROR, errmsg("syntaxical_analize: Invalid token array. First token is not variable"));
    if (len == cmplen) {
        predicate->predicate_type = CMP;
        switch(parsed_tokens[op1pos].type) {
            case T_EQ: {
                predicate->op = OP_EQ;
                break;
            }
            case T_NEQ: {
                predicate->op = OP_NEQ;
                break;
            }
            case T_GR: {
                predicate->op = OP_GR;
                break;
            }
            case T_GRE: {
                predicate->op = OP_GRE;
                break;
            }
            case T_LE: {
                predicate->op = OP_LE;
                break;
            }
            case T_LEE: {
                predicate->op = OP_LEE;
                break;
            }
            default: {
                ereport(ERROR, ERRCODE_SYNTAX_ERROR, errmsg("syntaxical_analize: Invalid first operator: %d", parsed_tokens[op1pos].type));
                break;
            }
        }
        predicate->const1 = parsed_tokens[const1pos].token;
        predicate->const1len = parsed_tokens[const1pos].lenght;
        predicate->const2 = NULL;
    }
    else if (len == modlen) {
        if (parsed_tokens[op1pos].type == T_MOD) {
            predicate->predicate_type = MOD;    
            predicate->op = OP_MOD;
        }
        else ereport(ERROR, ERRCODE_SYNTAX_ERROR, errmsg("syntaxical_analize: Invalid token array len: %d", len));
        if (parsed_tokens[op2pos].type != T_SEQ)
            ereport(ERROR, ERRCODE_SYNTAX_ERROR, errmsg("syntaxical_analize: Invalid second operator: %d", parsed_tokens[op2pos].type));
        predicate->const1 = parsed_tokens[const1pos].token;
        predicate->const2 = parsed_tokens[const2pos].token;
        predicate->const1len = parsed_tokens[const1pos].lenght;
        predicate->const2len = parsed_tokens[const2pos].lenght;
    }
    return 0;
}

// проверить допустимость операций
// привести предикат к нужному виду

void cast_predicate(ParsedPredicate* parsed, Predicate* result, Oid target_oid) {
    if (parsed == NULL)
        ereport(ERROR, ERRCODE_INVALID_PARAMETER_VALUE, errmsg("cast_predicate: Argument PARSED is null"));
    if (result == NULL)
        ereport(ERROR, ERRCODE_INVALID_PARAMETER_VALUE, errmsg("cast_predicate: argument RESULT is null"));
    // проверка на согласованность target_oid и операций
    switch(target_oid) {
        case TEXTOID: {
            if (parsed->op != OP_EQ && parsed->op != OP_NEQ)
                ereport(ERROR, ERRCODE_INVALID_PARAMETER_VALUE,
                    errmsg("cast_predicate: Invalid operator. Type: TEXT, operator: %d", parsed->op));
            break;
        }
        case FLOAT4OID: {
            if (parsed->op == OP_MOD)
                ereport(ERROR, ERRCODE_INVALID_PARAMETER_VALUE,
                    errmsg("cast_predicate: Invalid operator. Type: FLOAT4, operator: %d", parsed->op));
            break;
        }
        case FLOAT8OID: {
            if (parsed->op == OP_MOD)
                ereport(ERROR, ERRCODE_INVALID_PARAMETER_VALUE,
                    errmsg("cast_predicate: Invalid operator. Type: FLOAT8, operator: %d", parsed->op));
            break;
        }

        //для остальных типов на данный момент всё разрешено
    }

    memset(result, 0, sizeof(*result));
    result->target_oid = target_oid;
    result->predicate_type = parsed->predicate_type;
    result->op = parsed->op;
    result->const1 = string_to_datum(parsed->const1, parsed->const1len, target_oid);
    switch(target_oid) {
        case INT2OID:
        case INT4OID:
        case INT8OID:
        case NUMERICOID:
        if(parsed->predicate_type == MOD) {
            result->const2 = string_to_datum(parsed->const2, parsed->const2len, target_oid);
        }
        break;
    }
}

int32 cmp_op(Oid target_oid, Datum first, Datum second, Oid collid) {
    TypeCacheEntry* typentry;
    typentry = lookup_type_cache(target_oid, TYPECACHE_CMP_PROC_FINFO);
    if(!OidIsValid(typentry->cmp_proc))
        ereport(ERROR, ERRCODE_UNDEFINED_FUNCTION,
            errmsg("cmp_op: Invalid oid of cmp func"));

    return DatumGetInt32(FunctionCall2Coll(&typentry->cmp_proc_finfo, collid, first, second));

}

bool datum_is_zero(Datum value, Oid type_oid, Oid collid) {
    Datum zero;
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
    }
    return cmp_op(type_oid, value, zero, collid) == 0;
}


Datum mod_op(Oid target_oid, Datum first, Datum second, Oid collid) {
    if (datum_is_zero(second, target_oid, collid))
                ereport(ERROR, ERRCODE_DIVISION_BY_ZERO, errmsg("process: division by zero"));
    Datum result;
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
    }
    return result;
}

bool process(Datum element, Datum predicate_text, Oid target_oid, Oid collid) {
    char *predicate_string = text_to_cstring(DatumGetTextP(predicate_text));
    int32 predicate_len = strlen(predicate_string);

    Token tokens[10];
    ParsedPredicate parsed;
    Predicate predicate = {0};

    int32 token_count = lexical_analyze(
        predicate_string,
        predicate_len,
        tokens,
        lengthof(tokens) - 1
    );
    syntaxical_analize(tokens, token_count, &parsed);
    cast_predicate(&parsed, &predicate, target_oid);

    bool res;
    switch(predicate.op) {
        case OP_EQ: {
            res = (cmp_op(target_oid, element, predicate.const1, collid) == 0);
            break;
        }
        case OP_NEQ: {
            res = (cmp_op(target_oid, element, predicate.const1, collid) != 0);
            break;
        }
        case OP_GR: {
            res = (cmp_op(target_oid, element, predicate.const1, collid) > 0);
            break;
        }
        case OP_GRE: {
            res = (cmp_op(target_oid, element, predicate.const1, collid) >= 0);
            break;
        }
        case OP_LE: {
            res = (cmp_op(target_oid, element, predicate.const1, collid) < 0);
            break;
        }
        case OP_LEE: {
            res = (cmp_op(target_oid, element, predicate.const1, collid) <= 0);
            break;
        }
        case OP_MOD: {
            if (datum_is_zero(predicate.const1, target_oid, collid))
                ereport(ERROR, ERRCODE_DIVISION_BY_ZERO, errmsg("process: division by zero"));
            Datum tmp = mod_op(target_oid, element, predicate.const1, collid);
            res = (cmp_op(target_oid, tmp, predicate.const2, collid) == 0);
            break;
        }
        default: {
            ereport(ERROR, ERRCODE_INVALID_PARAMETER_VALUE, errmsg("process: invalid operator"));
            break;
        }
    }
    return res;

}
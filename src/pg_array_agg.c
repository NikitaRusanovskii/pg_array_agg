#include "postgres.h"
#include "fmgr.h"
#include "utils/array.h"
#include "regex/regex.h"

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

typedef enum {
    ONE_OP,
    TWO_OP
} predicate_type;

typedef enum {
    EQ, // ==
    NEQ, // !=
    GR, // >
    LE, // <
    GRE, // >=
    LEE, // <=
    MOD, // %
    SEQ, // = (short equal)
} operator;

typedef struct {
    Oid operand_type;
    text* varname;
    operator operator1;
    text* const1;
    operator operator2;
    text* const2;
    predicate_type pt;
} predicate;

bool parse_predicate(predicate* p, text* predicate_expr, Oid collid) {
    const char *parse_pattern = "^([A-Za-z]+)[[:space:]]*(<=|>=|==|!=|<|>|%)[[:space:]]*"
    "('(?:NaN|Infinity|-Infinity)'|\\\"(?:[^\\\"]|\\\"\\\")*\\\"|[+-]?"
    "(?:[0-9]+(?:[.][0-9]*)?|[.][0-9]+)(?:[eE][+-]?[0-9]+)?)(?:[[:space:]]*(=)[[:space:]]*"
    "('(?:NaN|Infinity|-Infinity)'|\\\""
    "(?:[^\\\"]|\\\"\\\")*\\\"|[+-]?(?:[0-9]+"
    "(?:[.][0-9]*)?|[.][0-9]+)(?:[eE][+-]?[0-9]+)?))?$";
    regmatch_t matched[6];
    text* regexpr_text = cstring_to_text(parse_pattern);
    memset(p, 0, sizeof(*p)); // зануляем структуру
    if (!RE_compile_and_execute(regexpr_text, VARDATA_ANY(predicate_expr), VARSIZE_ANY_EXHDR(predicate_expr) ,REG_ADVANCED,
        collid,lengthof(matched), matched)) {
            pfree(regexpr_text);
            return false;
        }

    pfree(regexpr_text);

    p->pt = ONE_OP;
    #define EXTRACT(dst, ind) \
    do { \
        if (matched[ind].rm_so >= 0) \
            (dst) = cstring_to_text_with_len( \
                VARDATA_ANY(predicate_expr) + matched[ind].rm_so, \
                matched[ind].rm_eo - matched[ind].rm_so); \
    } while (0)
    EXTRACT(p->varname, 1);
    EXTRACT(p->const1, 3);
    if (matched[4].rm_so >= 0) {
        p->pt = TWO_OP;
        EXTRACT(p->const2, 5);
        p->operator2 = SEQ;
    }
    #undef EXTRACT

    char* opstr = pnstrdup(VARDATA_ANY(predicate_expr) + matched[2].rm_so,
                            matched[2].rm_eo - matched[2].rm_so);

    if (strcmp(opstr, "==") == 0) {
        p->operator1 = EQ;
    }
    else if (strcmp(opstr, "!=") == 0) {
        p->operator1 = NEQ;
    }
    else if (strcmp(opstr, ">") == 0) {
        p->operator1 = GR;
    }
    else if (strcmp(opstr, "<") == 0) {
        p->operator1 = LE;
    }
    else if (strcmp(opstr, "<=") == 0) {
        p->operator1 = LEE;
    }
    else if (strcmp(opstr, ">=") == 0) {
        p->operator1 = GRE;
    }
    else if (strcmp(opstr, "%") == 0) {
        p->operator1 = MOD;
    }
    else {
        //error
    }

    pfree(opstr);
    return true;
}

PG_FUNCTION_INFO_V1(array_count_if);
Datum array_count_if(PG_FUNCTION_ARGS) {
    ArrayType* arr;
    Oid collation = PG_GET_COLLATION();
    Oid elemtype;
    Datum predicate;
    Datum current_element;
    ArrayIterator iterator;
    bool is_null;
    int32 result;

    if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) PG_RETURN_NULL(); // функция STRIСT, этого никогда не произойдёт
    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) > 1)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("array_count_if: searching elements in manydimensional arrays is not supported yet")));
    if (ARR_NDIM(arr) < 1) // пустой, нульмерный
        PG_RETURN_NULL();
    predicate = PG_GETARG_DATUM(1);
    // считаем, что распарсили predicate

    result = 0;
    iterator = array_create_iterator(arr, 0, NULL);
    while(array_iterate(iterator, &current_element, &is_null)) {
        if (is_null) {
            array_free_iterator(iterator);
            PG_RETURN_NULL();
        }
    }

    PG_FREE_IF_COPY(arr, 0); // если массив был разбит на тосты, избежим утечек так
    array_free_iterator(iterator);
    PG_RETURN_INT32(result);
}
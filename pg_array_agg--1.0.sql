\echo Use "CREATE EXTENSION pg_array_agg" to load this file \quit

CREATE DOMAIN predicate AS text;

CREATE FUNCTION array_product(arr anyarray)
RETURNS numeric
AS 'MODULE_PATHNAME', 'array_product'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION array_sum(arr anyarray)
RETURNS numeric
AS 'MODULE_PATHNAME', 'array_sum'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION array_avg(arr anyarray)
RETURNS numeric
AS 'MODULE_PATHNAME', 'array_avg'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION array_min(arr anyarray)
RETURNS anyelement
AS 'MODULE_PATHNAME', 'array_min'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION array_max(arr anyarray)
RETURNS anyelement
AS 'MODULE_PATHNAME', 'array_max'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION array_count(arr anyarray, el anyelement)
RETURNS integer
AS 'MODULE_PATHNAME', 'array_count'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION array_count_if(arr anyarray, pr predicate)
RETURNS integer
AS 'MODULE_PATHNAME', 'array_count'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION array_any(arr anyarray, pr predicate)
RETURNS boolean
AS 'MODULE_PATHNAME', 'array_count'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION array_all(arr anyarray, pr predicate)
RETURNS boolean
AS 'MODULE_PATHNAME', 'array_count'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION array_none(arr anyarray, pr predicate)
RETURNS boolean
AS 'MODULE_PATHNAME', 'array_count'
LANGUAGE C STRICT IMMUTABLE;
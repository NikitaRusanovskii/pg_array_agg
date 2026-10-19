\echo Use "CREATE EXTENSION pg_array_agg" to load this file \quit

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
-- ТЕСТЫ array_count:
-- КОРРЕКТНЫЕ
SELECT array_count(ARRAY[1, 2, 3, 4], 1);
SELECT array_count(ARRAY[1, 1], 1);
SELECT array_count(ARRAY[2], 2);
SELECT array_count('{}'::integer[], 1);
SELECT array_count('{1, 2, 1, 3}'::integer[], 1);
SELECT array_count('{1, 2, 1, 3}'::integer[], 3);
SELECT array_count('{1, 2, 1, 3}'::integer[], 4);
SELECT array_count('{1, NULL, 1, NULL}'::integer[], 1);
SELECT array_count('{NULL, NULL}'::integer[], 1);
SELECT array_count(ARRAY[0.0::real], -0.0::real);
SELECT array_count('{Infinity, Infinity}'::real[], 'Infinity'::real);
SELECT array_count('{-Infinity, -Infinity}'::real[], '-Infinity'::real);
SELECT array_count('{NaN, NaN}'::real[], 'NaN'::real);
SELECT array_count(ARRAY['a', 'b', 'a'], 'a');
SELECT array_count(ARRAY['a', 'b', 'a'], 'b');
SELECT array_count(ARRAY['a', 'b', 'a'], 'c');
SELECT array_count(ARRAY['hello', 'world', 'hello']::varchar[], 'hello'::varchar);
SELECT array_count(ARRAY['hello', 'world', 'hello']::character varying(10)[], 'hello'::character varying(10));
SELECT array_count(ARRAY['cat', 'dog', 'cat']::char(5)[], 'cat'::char(5));
SELECT array_count(ARRAY['cat', 'dog', 'cat']::character(5)[], 'cat'::character(5));
SELECT array_count(ARRAY['cat'::char(5), 'cat  '::char(5), 'dog'::char(5)], 'cat'::char(5));
SELECT array_count(ARRAY[[1, 2], [3, 4]], 1);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_count(NULL::integer[], 1);
SELECT array_count(ARRAY[1, 2, 3], NULL);

-- НЕКОРРЕКТНЫЕ
SELECT array_count(ARRAY[[[1, 2]], [[3, 4]]], 1);
SELECT array_count(ARRAY[1, 2, 3], '1'::text);
SELECT array_count(ARRAY['a', 'b'], 1);
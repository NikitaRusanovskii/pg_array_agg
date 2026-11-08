-- ТЕСТЫ array_all:
-- КОРРЕКТНЫЕ
SELECT array_all(ARRAY[1, 2, 3, 4], 'xab == 1');
SELECT array_all(ARRAY[1, 1], 'x == 1');
SELECT array_all(ARRAY[2], 'x_a == 2');
SELECT array_all('{1, 2, 1, 3}'::integer[], 'x == 1');
SELECT array_all('{1, 2, 3, 4}'::integer[], 'x > 1');
SELECT array_all('{1, 2}'::integer[], '_x == 3');
SELECT array_all('{Infinity}'::numeric[], '_ == ''Infinity''');
SELECT array_all('{-Infinity}'::real[], 'x == ''-Infinity''');
SELECT array_all('{NaN}'::double precision[], 'x == ''NaN''');
SELECT array_all('{-0.0}'::numeric[], 'x == 0.0');
SELECT array_all(ARRAY['a', 'b', 'a']::text[], 'x == "a"');
SELECT array_all('{}'::integer[], 'x == 1');
SELECT array_all(ARRAY['hello', 'world', 'hello']::varchar[], 'x == "hello"');
SELECT array_all(ARRAY['hello', 'world', 'hello']::character varying(10)[], 'x == "hello"');
SELECT array_all(ARRAY['cat', 'dog', 'cat']::char(5)[], 'x == "cat"');
SELECT array_all(ARRAY['cat', 'dog', 'cat']::character(5)[], 'x == "cat"');
SELECT array_all(ARRAY[[1, 2], [3, 4]], 'x == 1');

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_all(NULL::integer[], 'x == 1');
SELECT array_all(ARRAY[1, 2, 3], NULL::text);

-- НЕКОРРЕКТНЫЕ
SELECT array_all(ARRAY[[[1, 2]], [[3, 4]]], 'x == 1');
SELECT array_all(ARRAY['a', 'b']::text[], 'x > ''a''');
SELECT array_all(ARRAY[1, 2, 3], 'x @@@ 1');
SELECT array_all(ARRAY[1, 2, 3], 'x ==');
SELECT array_all(ARRAY[1, 2, 3], 'x == ''abc''');
SELECT array_all(ARRAY[1, 2, 3], 'x % 0 = 0');
SELECT array_all(ARRAY[true, false], 'x == 1');

-- ТЕСТЫ array_any:
-- КОРРЕКТНЫЕ
SELECT array_any(ARRAY[1, 2, 3, 4], 'x == 1');
SELECT array_any(ARRAY[1, 1], 'x == 1');
SELECT array_any(ARRAY[2], 'x == 2');
SELECT array_any('{1, 2, 1, 3}'::integer[], 'x == 1');
SELECT array_any('{1, 2, 3, 4}'::integer[], 'x > 1');
SELECT array_any('{1, 2}'::integer[], 'x == 3');
SELECT array_any('{Infinity}'::numeric[], 'x == ''Infinity''');
SELECT array_any('{-Infinity}'::real[], 'x == ''-Infinity''');
SELECT array_any('{NaN}'::double precision[], 'x == ''NaN''');
SELECT array_any('{-0.0}'::numeric[], 'x == 0.0');
SELECT array_any(ARRAY['a', 'b', 'a']::text[], 'x == "a"');
SELECT array_any('{}'::integer[], 'x == 1');
SELECT array_any(ARRAY['hello', 'world', 'hello']::varchar[], 'x == "hello"');
SELECT array_any(ARRAY['hello', 'world', 'hello']::character varying(10)[], 'x == "hello"');
SELECT array_any(ARRAY['cat', 'dog', 'cat']::char(5)[], 'x == "cat"');
SELECT array_any(ARRAY['cat', 'dog', 'cat']::character(5)[], 'x == "cat"');
SELECT array_any(ARRAY[[1, 2], [3, 4]], 'x == 1');

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_any(NULL::integer[], 'x == 1');
SELECT array_any(ARRAY[1, 2, 3], NULL::text);

-- НЕКОРРЕКТНЫЕ
SELECT array_any(ARRAY[[[1, 2]], [[3, 4]]], 'x == 1');
SELECT array_any(ARRAY['a', 'b']::text[], 'x > ''a''');
SELECT array_any(ARRAY[1, 2, 3], 'x @@@ 1');
SELECT array_any(ARRAY[1, 2, 3], 'x ==');
SELECT array_any(ARRAY[1, 2, 3], 'x == ''abc''');
SELECT array_any(ARRAY[1, 2, 3], 'x % 0 = 0');
SELECT array_any(ARRAY[true, false], 'x == 1');

-- ТЕСТЫ array_none:
-- КОРРЕКТНЫЕ
SELECT array_none(ARRAY[1, 2, 3, 4], 'x == 1');
SELECT array_none(ARRAY[1, 1], 'x == 1');
SELECT array_none(ARRAY[2], 'x == 2');
SELECT array_none('{1, 2, 1, 3}'::integer[], 'x == 1');
SELECT array_none('{1, 2, 3, 4}'::integer[], 'x > 1');
SELECT array_none('{1, 2}'::integer[], 'x == 3');
SELECT array_none('{Infinity}'::numeric[], 'x == ''Infinity''');
SELECT array_none('{-Infinity}'::real[], 'x == ''-Infinity''');
SELECT array_none('{NaN}'::double precision[], 'x == ''NaN''');
SELECT array_none('{-0.0}'::numeric[], 'x == 0.0');
SELECT array_none(ARRAY['a', 'b', 'a']::text[], 'x == "a"');
SELECT array_none('{}'::integer[], 'x == 1');
SELECT array_none(ARRAY['hello', 'world', 'hello']::varchar[], 'x == "hello"');
SELECT array_none(ARRAY['hello', 'world', 'hello']::character varying(10)[], 'x == "hello"');
SELECT array_none(ARRAY['cat', 'dog', 'cat']::char(5)[], 'x == "cat"');
SELECT array_none(ARRAY['cat', 'dog', 'cat']::character(5)[], 'x == "cat"');
SELECT array_none(ARRAY[[1, 2], [3, 4]], 'x == 1');

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_none(NULL::integer[], 'x == 1');
SELECT array_none(ARRAY[1, 2, 3], NULL::text);

-- НЕКОРРЕКТНЫЕ
SELECT array_none(ARRAY[[[1, 2]], [[3, 4]]], 'x == 1');
SELECT array_none(ARRAY['a', 'b']::text[], 'x > ''a''');
SELECT array_none(ARRAY[1, 2, 3], 'x @@@ 1');
SELECT array_none(ARRAY[1, 2, 3], 'x ==');
SELECT array_none(ARRAY[1, 2, 3], 'x == ''abc''');
SELECT array_none(ARRAY[1, 2, 3], 'x % 0 = 0');
SELECT array_none(ARRAY[true, false], 'x == 1');

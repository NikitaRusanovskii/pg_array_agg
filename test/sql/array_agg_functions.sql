CREATE EXTENSION IF NOT EXISTS pg_array_agg;


--- ======================================================================================
-- ТЕСТЫ array_count_if:
-- КОРРЕКТНЫЕ
SELECT array_count_if(ARRAY[1, 2, 3, 4], 'x == 1');
SELECT array_count_if(ARRAY[1, 1], 'x == 1');
SELECT array_count_if(ARRAY[2], 'x == 2');
SELECT array_count_if('{1, 2, 1, 3}'::integer[], 'x == 1');
SELECT array_count_if('{1, 2, 3, 4}'::integer[], 'x > 1');
SELECT array_count_if('{1, 2}'::integer[], 'x == 3');
SELECT array_count_if('{Infinity}'::numeric[], 'x == ''Infinity''');
SELECT array_count_if('{-Infinity}'::real[], 'x == ''-Infinity''');
SELECT array_count_if('{NaN}'::double precision[], 'x == ''NaN''');
SELECT array_count_if('{-0.0}'::numeric[], 'x == 0.0');
SELECT array_count_if(ARRAY['a', 'b', 'a']::text[], 'x == "a"');
-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_count_if(NULL::integer[], 'x == 1');
SELECT array_count_if('{}'::integer[], 'x == 1');
SELECT array_count_if(ARRAY[1, 2, 3], NULL::text);

-- НЕКОРРЕКТНЫЕ
SELECT array_count_if(ARRAY[[1, 2], [3, 4]], 'x == 1');
SELECT array_count_if(ARRAY['a', 'b']::text[], 'x > ''a''');
SELECT array_count_if(ARRAY[1, 2, 3], 'x @@@ 1');
SELECT array_count_if(ARRAY[1, 2, 3], 'x ==');
SELECT array_count_if(ARRAY[1, 2, 3], 'x == ''abc''');
SELECT array_count_if(ARRAY[1, 2, 3], 'x % 0 = 0');
SELECT array_count_if(ARRAY[true, false], 'x == 1');

--- ======================================================================================
-- ТЕСТЫ array_all:
-- КОРРЕКТНЫЕ
SELECT array_all(ARRAY[1, 2, 3, 4], 'x == 1');
SELECT array_all(ARRAY[1, 1], 'x == 1');
SELECT array_all(ARRAY[2], 'x == 2');
SELECT array_all('{1, 2, 1, 3}'::integer[], 'x == 1');
SELECT array_all('{1, 2, 3, 4}'::integer[], 'x > 1');
SELECT array_all('{1, 2}'::integer[], 'x == 3');
SELECT array_all('{Infinity}'::numeric[], 'x == ''Infinity''');
SELECT array_all('{-Infinity}'::real[], 'x == ''-Infinity''');
SELECT array_all('{NaN}'::double precision[], 'x == ''NaN''');
SELECT array_all('{-0.0}'::numeric[], 'x == 0.0');
SELECT array_all(ARRAY['a', 'b', 'a']::text[], 'x == "a"');
SELECT array_all('{}'::integer[], 'x == 1');
-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_all(NULL::integer[], 'x == 1');
SELECT array_all(ARRAY[1, 2, 3], NULL::text);

-- НЕКОРРЕКТНЫЕ
SELECT array_all(ARRAY[[1, 2], [3, 4]], 'x == 1');
SELECT array_all(ARRAY['a', 'b']::text[], 'x > ''a''');
SELECT array_all(ARRAY[1, 2, 3], 'x @@@ 1');
SELECT array_all(ARRAY[1, 2, 3], 'x ==');
SELECT array_all(ARRAY[1, 2, 3], 'x == ''abc''');
SELECT array_all(ARRAY[1, 2, 3], 'x % 0 = 0');
SELECT array_all(ARRAY[true, false], 'x == 1');

--- ======================================================================================
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
-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_any(NULL::integer[], 'x == 1');
SELECT array_any(ARRAY[1, 2, 3], NULL::text);

-- НЕКОРРЕКТНЫЕ
SELECT array_any(ARRAY[[1, 2], [3, 4]], 'x == 1');
SELECT array_any(ARRAY['a', 'b']::text[], 'x > ''a''');
SELECT array_any(ARRAY[1, 2, 3], 'x @@@ 1');
SELECT array_any(ARRAY[1, 2, 3], 'x ==');
SELECT array_any(ARRAY[1, 2, 3], 'x == ''abc''');
SELECT array_any(ARRAY[1, 2, 3], 'x % 0 = 0');
SELECT array_any(ARRAY[true, false], 'x == 1');

--- ======================================================================================
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
-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_none(NULL::integer[], 'x == 1');
SELECT array_none(ARRAY[1, 2, 3], NULL::text);

-- НЕКОРРЕКТНЫЕ
SELECT array_none(ARRAY[[1, 2], [3, 4]], 'x == 1');
SELECT array_none(ARRAY['a', 'b']::text[], 'x > ''a''');
SELECT array_none(ARRAY[1, 2, 3], 'x @@@ 1');
SELECT array_none(ARRAY[1, 2, 3], 'x ==');
SELECT array_none(ARRAY[1, 2, 3], 'x == ''abc''');
SELECT array_none(ARRAY[1, 2, 3], 'x % 0 = 0');
SELECT array_none(ARRAY[true, false], 'x == 1');


DROP EXTENSION IF EXISTS pg_array_agg;
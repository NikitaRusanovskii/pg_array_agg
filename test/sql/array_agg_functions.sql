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
DROP EXTENSION IF EXISTS pg_array_agg;
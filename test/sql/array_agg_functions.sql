CREATE EXTENSION IF NOT EXISTS pg_array_agg;
--- ======================================================================================
-- ТЕСТЫ array_product:
-- КОРРЕКТНЫЕ
SELECT array_product(ARRAY[1,2,3,4]);
SELECT array_product(ARRAY[1, 1]);
SELECT array_product(ARRAY[2]);
SELECT array_product('{}'::integer[]);
SELECT array_product('{2.3, 2.4, 2.3}'::real[]);
SELECT array_product('{2.3, 2.4, 2.3}'::numeric[]);
SELECT array_product('{0, 1, 2}'::integer[]);
SELECT array_product('{1, -2, 3}'::integer[]);
SELECT array_product('{NaN}'::real[]);
SELECT array_product('{Infinity}'::real[]);
SELECT array_product('{-Infinity}'::real[]);
SELECT array_product('{Infinity, 2}'::real[]);
SELECT array_product('{-Infinity, 2}'::real[]);
SELECT array_product('{Infinity, 0}'::real[]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_product(NULL::integer[]);
SELECT array_product('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_product(ARRAY[[1, 2], [3, 4]]);
SELECT array_product(ARRAY['a', 'b']::text[]);

--- ======================================================================================
-- ТЕСТЫ array_sum:
-- КОРРЕКТНЫЕ
SELECT array_sum(ARRAY[1,2,3,4]);
SELECT array_sum(ARRAY[1, 1]);
SELECT array_sum(ARRAY[2]);
SELECT array_sum('{}'::integer[]);
SELECT array_sum('{2.3, 2.4, 2.3}'::real[]);
SELECT array_sum('{2.3, 2.4, 2.3}'::numeric[]);
SELECT array_sum('{0, 1, 2}'::integer[]);
SELECT array_sum('{1, -2, 3}'::integer[]);
SELECT array_sum('{NaN}'::real[]);
SELECT array_sum('{Infinity}'::real[]);
SELECT array_sum('{-Infinity}'::real[]);
SELECT array_sum('{Infinity, 2}'::real[]);
SELECT array_sum('{-Infinity, 2}'::real[]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_sum(NULL::integer[]);
SELECT array_sum('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_sum(ARRAY[[1, 2], [3, 4]]);
SELECT array_sum(ARRAY['a', 'b']::text[]);

--- ======================================================================================
-- ТЕСТЫ array_avg:
-- КОРРЕКТНЫЕ
SELECT array_avg(ARRAY[1,2,3,4]);
SELECT array_avg(ARRAY[1, 1]);
SELECT array_avg(ARRAY[2]);
SELECT array_avg('{2.3, 2.4, 2.3}'::real[]);
SELECT array_avg('{2.3, 2.4, 2.3}'::numeric[]);
SELECT array_avg('{0, 1, 2}'::integer[]);
SELECT array_avg('{1, -2, 3}'::integer[]);
SELECT array_avg('{NaN}'::real[]);
SELECT array_avg('{Infinity}'::real[]);
SELECT array_avg('{-Infinity}'::real[]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_avg('{}'::integer[]);
SELECT array_avg(NULL::integer[]);
SELECT array_avg('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_avg(ARRAY[[1, 2], [3, 4]]);
SELECT array_avg(ARRAY['a', 'b']::text[]);

--- ======================================================================================
-- ТЕСТЫ array_min:
-- КОРРЕКТНЫЕ
SELECT array_min(ARRAY[1,2,3,4]);
SELECT array_min(ARRAY[1, 1]);
SELECT array_min(ARRAY[2]);
SELECT array_min('{2.3, 2.4, 2.3}'::real[]);
SELECT array_min('{2.3, 2.4, 2.3}'::numeric[]);
SELECT array_min('{-Infinity, NaN}'::real[]);
SELECT array_min('{Infinity, -Infinity}'::real[]);
SELECT array_min('{NaN, Infinity}'::real[]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_min('{}'::integer[]);
SELECT array_min(NULL::integer[]);
SELECT array_min('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_min(ARRAY[[1, 2], [3, 4]]);
SELECT array_min(ARRAY['a', 'b']::text[]);

--- ======================================================================================
-- ТЕСТЫ array_max:
-- КОРРЕКТНЫЕ
SELECT array_max(ARRAY[1,2,3,4]);
SELECT array_max(ARRAY[1, 1]);
SELECT array_max(ARRAY[2]);
SELECT array_max('{2.3, 2.4, 2.3}'::real[]);
SELECT array_max('{2.3, 2.4, 2.3}'::numeric[]);
SELECT array_max('{NaN, Infinity}'::real[]);
SELECT array_max('{Infinity, -Infinity}'::real[]);
SELECT array_max('{-Infinity, 2}'::real[]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_max('{}'::integer[]);
SELECT array_max(NULL::integer[]);
SELECT array_max('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_max(ARRAY[[1, 2], [3, 4]]);
SELECT array_max(ARRAY['a', 'b']::text[]);

--- ======================================================================================
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

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_count(NULL::integer[], 1);
SELECT array_count(ARRAY[1, 2, 3], NULL);

-- НЕКОРРЕКТНЫЕ
SELECT array_count(ARRAY[[1, 2], [3, 4]], 1);
SELECT array_count(ARRAY[1, 2, 3], '1'::text);
SELECT array_count(ARRAY['a', 'b'], 1);

--- ======================================================================================
-- ТЕСТЫ array_count_if:
-- КОРРЕКТНЫЕ
SELECT array_count_if(ARRAY[1, 2, 3, 4], 'x == 1');
SELECT array_count_if(ARRAY[1, 1], 'x == 1');
SELECT array_count_if(ARRAY[2], 'x == 2');
SELECT array_count_if('{}'::integer[], 'x == 1');
SELECT array_count_if('{1, 2, 1, 3}'::integer[], 'x == 1');
SELECT array_count_if('{1, 2, 3, 4}'::integer[], 'x > 1');
SELECT array_count_if('{1, 2}'::integer[], 'x == 3');
SELECT array_count_if('{Infinity}'::real[], 'x == Infinity');
SELECT array_count_if('{-Infinity}'::real[], 'x == -Infinity');
SELECT array_count_if('{NaN}'::real[], 'x == NaN');
SELECT array_count_if('{-0.0}'::real[], 'x == 0.0');
SELECT array_count_if(ARRAY['a', 'b', 'a']::text[], 'x == ''a''');
SELECT array_count_if(ARRAY['a', 'b', 'a']::text[], 'x != ''a''');

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_count_if(NULL::integer[], 'x == 1');
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
SELECT array_all(ARRAY[1, 2, 3, 4], 'x >= 1');
SELECT array_all(ARRAY[1, 1], 'x == 1');
SELECT array_all(ARRAY[2], 'x == 2');
SELECT array_all('{}'::integer[], 'x == 1');
SELECT array_all('{NULL, NULL}'::integer[], 'x == 1');
SELECT array_all('{1, 2, 3, 4}'::integer[], 'x >= 1');
SELECT array_all('{1, 2}'::integer[], 'x == 3');
SELECT array_all('{Infinity}'::real[], 'x == Infinity');
SELECT array_all('{-Infinity}'::real[], 'x == -Infinity');
SELECT array_all('{NaN}'::real[], 'x == NaN');
SELECT array_all('{-0.0}'::real[], 'x == 0.0');
SELECT array_all(ARRAY['a', 'a']::text[], 'x == ''a''');

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
SELECT array_any('{}'::integer[], 'x == 1');
SELECT array_any('{NULL, NULL}'::integer[], 'x == 1');
SELECT array_any('{1, 2, 3, 4}'::integer[], 'x >= 1');
SELECT array_any('{1, 2}'::integer[], 'x == 3');
SELECT array_any('{Infinity}'::real[], 'x == Infinity');
SELECT array_any('{-Infinity}'::real[], 'x == -Infinity');
SELECT array_any('{NaN}'::real[], 'x == NaN');
SELECT array_any('{-0.0}'::real[], 'x == 0.0');
SELECT array_any(ARRAY['a', 'b']::text[], 'x == ''b''');

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
SELECT array_none(ARRAY[1, 2, 3, 4], 'x == 5');
SELECT array_none(ARRAY[1, 1], 'x == 2');
SELECT array_none(ARRAY[2], 'x == 1');
SELECT array_none('{}'::integer[], 'x == 1');
SELECT array_none('{NULL, NULL}'::integer[], 'x == 1');
SELECT array_none('{1, 2, 3, 4}'::integer[], 'x == 5');
SELECT array_none('{1, 2}'::integer[], 'x == 3');
SELECT array_none('{Infinity}'::real[], 'x == -Infinity');
SELECT array_none('{-Infinity}'::real[], 'x == Infinity');
SELECT array_none('{NaN}'::real[], 'x == Infinity');
SELECT array_none('{-0.0}'::real[], 'x == 1.0');
SELECT array_none(ARRAY['a', 'b']::text[], 'x == ''c''');

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



DROP EXTENSION IF EXISTS pg_array_agg;
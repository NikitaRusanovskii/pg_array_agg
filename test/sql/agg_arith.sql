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
SELECT array_product('{Infinity, -Infinity}'::real[]);
SELECT array_product('{NaN, 0}'::real[]);
SELECT array_product(ARRAY[[1, 2], [3, 4]]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_product(NULL::integer[]);
SELECT array_product('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_product(ARRAY[[[1, 2]], [[3, 4]]]);
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
SELECT array_sum('{Infinity, -Infinity}'::real[]);
SELECT array_sum(ARRAY[[1, 2], [3, 4]]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_sum(NULL::integer[]);
SELECT array_sum('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_sum(ARRAY[[[1, 2]], [[3, 4]]]);
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
SELECT array_avg('{Infinity, -Infinity}'::real[]);
SELECT array_avg(ARRAY[[1, 2], [3, 4]]);


-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_avg('{}'::integer[]);
SELECT array_avg(NULL::integer[]);
SELECT array_avg('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_avg(ARRAY[[[1, 2]], [[3, 4]]]);
SELECT array_avg(ARRAY['a', 'b']::text[]);
CREATE EXTENSION IF NOT EXISTS pg_array_agg;

-- ТЕСТЫ array_product:
-- КОРРЕКТНЫЕ
SELECT array_product(ARRAY[1,2,3,4]);
SELECT array_product(ARRAY[1, 1]);
SELECT array_product(ARRAY[2]);
SELECT array_product('{}'::integer[]);
SELECT array_product('{2.3, 2.4, 2.3}'::real[]);
SELECT array_product('{2.3, 2.4, 2.3}'::numeric[]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL

-- НЕКОРРЕКТНЫЕ

-- ТЕСТЫ array_sum:
-- КОРРЕКТНЫЕ
SELECT array_sum(ARRAY[1,2,3,4]);
SELECT array_sum(ARRAY[1, 1]);
SELECT array_sum(ARRAY[2]);
SELECT array_sum('{}'::integer[]);
SELECT array_sum('{2.3, 2.4, 2.3}'::real[]);
SELECT array_sum('{2.3, 2.4, 2.3}'::numeric[]);


-- ДОЛЖНЫ ВЕРНУТЬ NULL

-- НЕКОРРЕКТНЫЕ

-- ТЕСТЫ array_avg:
-- КОРРЕКТНЫЕ
SELECT array_avg(ARRAY[1,2,3,4]);
SELECT array_avg(ARRAY[1, 1]);
SELECT array_avg(ARRAY[2]);
SELECT array_avg('{2.3, 2.4, 2.3}'::real[]);
SELECT array_avg('{2.3, 2.4, 2.3}'::numeric[]);


-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_avg('{}'::integer[]);

-- НЕКОРРЕКТНЫЕ

-- ТЕСТЫ array_min:
-- КОРРЕКТНЫЕ
SELECT array_min(ARRAY[1,2,3,4]);
SELECT array_min(ARRAY[1, 1]);
SELECT array_min(ARRAY[2]);
SELECT array_min('{2.3, 2.4, 2.3}'::real[]);
SELECT array_min('{2.3, 2.4, 2.3}'::numeric[]);


-- ДОЛЖНЫ ВЕРНУТЬ NULL
-- НЕКОРРЕКТНЫЕ

-- ТЕСТЫ array_max:
-- КОРРЕКТНЫЕ
SELECT array_max(ARRAY[1,2,3,4]);
SELECT array_max(ARRAY[1, 1]);
SELECT array_max(ARRAY[2]);
SELECT array_max('{2.3, 2.4, 2.3}'::real[]);
SELECT array_max('{2.3, 2.4, 2.3}'::numeric[]);


-- ДОЛЖНЫ ВЕРНУТЬ NULL
-- НЕКОРРЕКТНЫЕ

DROP EXTENSION IF EXISTS pg_array_agg;
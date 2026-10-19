CREATE EXTENSION IF NOT EXISTS pg_array_agg;

-- ТЕСТЫ array_product:
-- КОРРЕКТНЫЕ
SELECT array_product(ARRAY[1,2,3,4]);
SELECT array_product(ARRAY[1, 1]);
SELECT array_product(ARRAY[2]);
SELECT array_product('{}'::integer[]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL

-- НЕКОРРЕКТНЫЕ

-- ТЕСТЫ array_sum:
-- КОРРЕКТНЫЕ
SELECT array_sum(ARRAY[1,2,3,4]);
SELECT array_sum(ARRAY[1, 1]);
SELECT array_sum(ARRAY[2]);
SELECT array_sum('{}'::integer[]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL

-- НЕКОРРЕКТНЫЕ

-- ТЕСТЫ array_avg:
-- КОРРЕКТНЫЕ
SELECT array_avg(ARRAY[1,2,3,4]);
SELECT array_avg(ARRAY[1, 1]);
SELECT array_avg(ARRAY[2]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_avg('{}'::integer[]);

-- НЕКОРРЕКТНЫЕ


DROP EXTENSION IF EXISTS pg_array_agg;
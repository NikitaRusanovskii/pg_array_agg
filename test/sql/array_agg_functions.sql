CREATE EXTENSION IF NOT EXISTS pg_array_agg;

-- ТЕСТЫ array_product:
-- КОРРЕКТНЫЕ
SELECT array_product(ARRAY[1,2,3,4]);
SELECT array_product(ARRAY[1, 1]);
SELECT array_product(ARRAY[2]);
SELECT array_product('{}'::integer[]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL

-- НЕКОРРЕКТНЫЕ
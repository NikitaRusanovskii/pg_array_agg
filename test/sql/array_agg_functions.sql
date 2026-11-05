
-- CREATE EXTENSION IF NOT EXISTS pg_array_agg;
-- ======================================================================================
-- ТЕСТЫ array_product:
-- КОРРЕКТНЫЕ
SELECT array_product(ARRAY[1,2,3,4]);
--DROP EXTENSION IF EXISTS pg_array_agg;

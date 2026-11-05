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
SELECT array_min(ARRAY[[1, 2], [3, 4]]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_min('{}'::integer[]);
SELECT array_min(NULL::integer[]);
SELECT array_min('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_min(ARRAY[[[1, 2]], [[3, 4]]]);
SELECT array_min(ARRAY[true, false]::boolean[]);

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
SELECT array_max(ARRAY[[1, 2], [3, 4]]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_max('{}'::integer[]);
SELECT array_max(NULL::integer[]);
SELECT array_max('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_max(ARRAY[[[1, 2]], [[3, 4]]]);
SELECT array_max(ARRAY[true, false]::boolean[]);
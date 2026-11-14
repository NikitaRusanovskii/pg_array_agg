SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], '_x > 2');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], 'x > 2');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], '1x > 2');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], '_ > 2');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], '123 > 2');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], 'abc > 2');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], '_123 > 2');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], '_abc > 2');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], 'abc123 > 2');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], '_abc123 > 2');

SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], 'x % 2 < 1');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], 'x % 2 > 0');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], 'x % 2 <= 1');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], 'x % 2 >= 0');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], 'x % 2 = 0');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], 'x % 2 == 0');
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], 'x % 2 != 0');

SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], 'x > 2');
SELECT array_count_if(ARRAY[1.0, 2.0, 3.0, 4.0], 'x > 2.5');

SELECT array_count_if(ARRAY[1.1, 2.2, 3.3, 4.4]::numeric[], 'x > 2.2');
SELECT array_count_if(ARRAY[1.1, 2.2, 3.3, 4.4]::numeric[], 'x % 2 > 0.5');

SELECT array_any(ARRAY[1, 2, 3, 4, 5], 'x % 2 == 0');
SELECT array_all(ARRAY[1, 2, 3, 4, 5], 'x % 2 >= 0');
SELECT array_none(ARRAY[1, 2, 3, 4, 5], 'x % 2 > 2');

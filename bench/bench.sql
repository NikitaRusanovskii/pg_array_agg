
SELECT array_product(arr) FROM bench_table;
SELECT array_sum(arr) FROM bench_table;
SELECT array_avg(arr) FROM bench_table;

SELECT array_min(arr) FROM bench_table;
SELECT array_max(arr) FROM bench_table;

SELECT array_count(arr, (floor(random() * :n + 1)::int)) FROM bench_table;

SELECT array_count_if(arr, 'x > ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_count_if(arr, 'x < ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_count_if(arr, 'x == ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_count_if(arr, 'x != ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_count_if(arr, 'x >= ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_count_if(arr, 'x <= ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_count_if(
    arr,
    'x % ' || (floor(random() * :n + 1)::int) || '=' || (floor(random() * :n + 1)::int)) FROM bench_table;


SELECT array_any(arr, 'x > ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_any(arr, 'x < ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_any(arr, 'x == ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_any(arr, 'x != ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_any(arr, 'x >= ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_any(arr, 'x <= ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_any(
    arr,
    'x % ' || (floor(random() * :n + 1)::int) || '=' || (floor(random() * :n + 1)::int)) FROM bench_table;


SELECT array_none(arr, 'x > ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_none(arr, 'x < ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_none(arr, 'x == ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_none(arr, 'x != ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_none(arr, 'x >= ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_none(arr, 'x <= ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_none(
    arr,
    'x % ' || (floor(random() * :n + 1)::int) || '=' || (floor(random() * :n + 1)::int)) FROM bench_table;


SELECT array_all(arr, 'x > ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_all(arr, 'x < ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_all(arr, 'x == ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_all(arr, 'x != ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_all(arr, 'x >= ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_all(arr, 'x <= ' || (floor(random() * :n + 1)::int)) FROM bench_table;
SELECT array_all(
    arr,
    'x % ' || (floor(random() * :n + 1)::int) || '=' || (floor(random() * :n + 1)::int)) FROM bench_table;
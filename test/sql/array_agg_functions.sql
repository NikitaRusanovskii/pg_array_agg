
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
SELECT array_avg(ARRAY(SELECT generate_series(1, 65000000))::Integer[]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_avg('{}'::integer[]);
SELECT array_avg(NULL::integer[]);
SELECT array_avg('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_avg(ARRAY[[[1, 2]], [[3, 4]]]);
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
SELECT array_min(ARRAY[[1, 2], [3, 4]]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_min('{}'::integer[]);
SELECT array_min(NULL::integer[]);
SELECT array_min('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_min(ARRAY[[[1, 2]], [[3, 4]]]);
SELECT array_min(ARRAY[true, false]::boolean[]);

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
SELECT array_max(ARRAY[[1, 2], [3, 4]]);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_max('{}'::integer[]);
SELECT array_max(NULL::integer[]);
SELECT array_max('{1, NULL, 3}'::integer[]);

-- НЕКОРРЕКТНЫЕ
SELECT array_max(ARRAY[[[1, 2]], [[3, 4]]]);
SELECT array_max(ARRAY[true, false]::boolean[]);

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
SELECT array_count(ARRAY['hello', 'world', 'hello']::varchar[], 'hello'::varchar);
SELECT array_count(ARRAY['hello', 'world', 'hello']::character varying(10)[], 'hello'::character varying(10));
SELECT array_count(ARRAY['cat', 'dog', 'cat']::char(5)[], 'cat'::char(5));
SELECT array_count(ARRAY['cat', 'dog', 'cat']::character(5)[], 'cat'::character(5));
SELECT array_count(ARRAY['cat'::char(5), 'cat  '::char(5), 'dog'::char(5)], 'cat'::char(5));
SELECT array_count(ARRAY[[1, 2], [3, 4]], 1);

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_count(NULL::integer[], 1);
SELECT array_count(ARRAY[1, 2, 3], NULL);

-- НЕКОРРЕКТНЫЕ
SELECT array_count(ARRAY[[[1, 2]], [[3, 4]]], 1);
SELECT array_count(ARRAY[1, 2, 3], '1'::text);
SELECT array_count(ARRAY['a', 'b'], 1);

--- ======================================================================================

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
SELECT array_count_if('{}'::integer[], 'x == 1');
SELECT array_count_if(ARRAY['hello', 'world', 'hello']::varchar[], 'x == "hello"');
SELECT array_count_if(ARRAY['hello', 'world', 'hello']::character varying(10)[], 'x == "hello"');
SELECT array_count_if(ARRAY['cat', 'dog', 'cat']::char(5)[], 'x == "cat"');
SELECT array_count_if(ARRAY['cat', 'dog', 'cat']::character(5)[], 'x == "cat"');
SELECT array_count_if(ARRAY['cat'::char(5), 'cat  '::char(5), 'dog'::char(5)], 'x == "cat"');
SELECT array_count_if(ARRAY[[1, 2], [3, 4]], 'x == 1');

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_count_if(NULL::integer[], 'x == 1');
SELECT array_count_if(ARRAY[1, 2, 3], NULL::text);

-- НЕКОРРЕКТНЫЕ
SELECT array_count_if(ARRAY[[[1, 2]], [[3, 4]]], 'x == 1');
SELECT array_count_if(ARRAY['a', 'b']::text[], 'x > ''a''');
SELECT array_count_if(ARRAY[1, 2, 3], 'x @@@ 1');
SELECT array_count_if(ARRAY[1, 2, 3], 'x ==');
SELECT array_count_if(ARRAY[1, 2, 3], 'x == ''abc''');
SELECT array_count_if(ARRAY[1, 2, 3], 'x % 0 = 0');
SELECT array_count_if(ARRAY[true, false], 'x == 1');

--- ======================================================================================
-- ТЕСТЫ array_all:
-- КОРРЕКТНЫЕ
SELECT array_all(ARRAY[1, 2, 3, 4], 'xab == 1');
SELECT array_all(ARRAY[1, 1], 'x == 1');
SELECT array_all(ARRAY[2], 'x_a == 2');
SELECT array_all('{1, 2, 1, 3}'::integer[], 'x == 1');
SELECT array_all('{1, 2, 3, 4}'::integer[], 'x > 1');
SELECT array_all('{1, 2}'::integer[], '_x == 3');
SELECT array_all('{Infinity}'::numeric[], '_ == ''Infinity''');
SELECT array_all('{-Infinity}'::real[], 'x == ''-Infinity''');
SELECT array_all('{NaN}'::double precision[], 'x == ''NaN''');
SELECT array_all('{-0.0}'::numeric[], 'x == 0.0');
SELECT array_all(ARRAY['a', 'b', 'a']::text[], 'x == "a"');
SELECT array_all('{}'::integer[], 'x == 1');
SELECT array_all(ARRAY['hello', 'world', 'hello']::varchar[], 'x == "hello"');
SELECT array_all(ARRAY['hello', 'world', 'hello']::character varying(10)[], 'x == "hello"');
SELECT array_all(ARRAY['cat', 'dog', 'cat']::char(5)[], 'x == "cat"');
SELECT array_all(ARRAY['cat', 'dog', 'cat']::character(5)[], 'x == "cat"');
SELECT array_all(ARRAY[[1, 2], [3, 4]], 'x == 1');

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_all(NULL::integer[], 'x == 1');
SELECT array_all(ARRAY[1, 2, 3], NULL::text);

-- НЕКОРРЕКТНЫЕ
SELECT array_all(ARRAY[[[1, 2]], [[3, 4]]], 'x == 1');
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
SELECT array_any('{1, 2, 1, 3}'::integer[], 'x == 1');
SELECT array_any('{1, 2, 3, 4}'::integer[], 'x > 1');
SELECT array_any('{1, 2}'::integer[], 'x == 3');
SELECT array_any('{Infinity}'::numeric[], 'x == ''Infinity''');
SELECT array_any('{-Infinity}'::real[], 'x == ''-Infinity''');
SELECT array_any('{NaN}'::double precision[], 'x == ''NaN''');
SELECT array_any('{-0.0}'::numeric[], 'x == 0.0');
SELECT array_any(ARRAY['a', 'b', 'a']::text[], 'x == "a"');
SELECT array_any('{}'::integer[], 'x == 1');
SELECT array_any(ARRAY['hello', 'world', 'hello']::varchar[], 'x == "hello"');
SELECT array_any(ARRAY['hello', 'world', 'hello']::character varying(10)[], 'x == "hello"');
SELECT array_any(ARRAY['cat', 'dog', 'cat']::char(5)[], 'x == "cat"');
SELECT array_any(ARRAY['cat', 'dog', 'cat']::character(5)[], 'x == "cat"');
SELECT array_any(ARRAY[[1, 2], [3, 4]], 'x == 1');

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_any(NULL::integer[], 'x == 1');
SELECT array_any(ARRAY[1, 2, 3], NULL::text);

-- НЕКОРРЕКТНЫЕ
SELECT array_any(ARRAY[[[1, 2]], [[3, 4]]], 'x == 1');
SELECT array_any(ARRAY['a', 'b']::text[], 'x > ''a''');
SELECT array_any(ARRAY[1, 2, 3], 'x @@@ 1');
SELECT array_any(ARRAY[1, 2, 3], 'x ==');
SELECT array_any(ARRAY[1, 2, 3], 'x == ''abc''');
SELECT array_any(ARRAY[1, 2, 3], 'x % 0 = 0');
SELECT array_any(ARRAY[true, false], 'x == 1');

--- ======================================================================================
-- ТЕСТЫ array_none:
-- КОРРЕКТНЫЕ
SELECT array_none(ARRAY[1, 2, 3, 4], 'x == 1');
SELECT array_none(ARRAY[1, 1], 'x == 1');
SELECT array_none(ARRAY[2], 'x == 2');
SELECT array_none('{1, 2, 1, 3}'::integer[], 'x == 1');
SELECT array_none('{1, 2, 3, 4}'::integer[], 'x > 1');
SELECT array_none('{1, 2}'::integer[], 'x == 3');
SELECT array_none('{Infinity}'::numeric[], 'x == ''Infinity''');
SELECT array_none('{-Infinity}'::real[], 'x == ''-Infinity''');
SELECT array_none('{NaN}'::double precision[], 'x == ''NaN''');
SELECT array_none('{-0.0}'::numeric[], 'x == 0.0');
SELECT array_none(ARRAY['a', 'b', 'a']::text[], 'x == "a"');
SELECT array_none('{}'::integer[], 'x == 1');
SELECT array_none(ARRAY['hello', 'world', 'hello']::varchar[], 'x == "hello"');
SELECT array_none(ARRAY['hello', 'world', 'hello']::character varying(10)[], 'x == "hello"');
SELECT array_none(ARRAY['cat', 'dog', 'cat']::char(5)[], 'x == "cat"');
SELECT array_none(ARRAY['cat', 'dog', 'cat']::character(5)[], 'x == "cat"');
SELECT array_none(ARRAY[[1, 2], [3, 4]], 'x == 1');

-- ДОЛЖНЫ ВЕРНУТЬ NULL
SELECT array_none(NULL::integer[], 'x == 1');
SELECT array_none(ARRAY[1, 2, 3], NULL::text);

-- НЕКОРРЕКТНЫЕ
SELECT array_none(ARRAY[[[1, 2]], [[3, 4]]], 'x == 1');
SELECT array_none(ARRAY['a', 'b']::text[], 'x > ''a''');
SELECT array_none(ARRAY[1, 2, 3], 'x @@@ 1');
SELECT array_none(ARRAY[1, 2, 3], 'x ==');
SELECT array_none(ARRAY[1, 2, 3], 'x == ''abc''');
SELECT array_none(ARRAY[1, 2, 3], 'x % 0 = 0');
SELECT array_none(ARRAY[true, false], 'x == 1');

-- Дополнительные тесты
CREATE TEMP TABLE huge_arr AS
SELECT
    ARRAY(
        SELECT 1
        FROM generate_series(1, 1000000)
    )::integer[] AS million_int,

    ARRAY(
        SELECT 1::smallint
        FROM generate_series(1, 65000000)
    ) AS sixtyfive_million_smallint;


-- array_product

-- Массив из 1 млн элементов
SELECT array_product(million_int) FROM huge_arr;

-- Массив из 65 млн элементов smallint
SELECT array_product(sixtyfive_million_smallint) FROM huge_arr;

-- Переполнение numeric
SELECT array_product(array_fill(100000, ARRAY[10000000]));

-- Различные нижние границы массива
SELECT array_product(array_fill(1, ARRAY[3], ARRAY[lb]))
FROM (VALUES (0), (1), (9)) AS bounds(lb);


-- array_sum

-- Массив из 1 млн элементов
SELECT array_sum(million_int) FROM huge_arr;

-- Массив из 65 млн элементов smallint
SELECT array_sum(sixtyfive_million_smallint) FROM huge_arr;

-- Различные нижние границы массива
SELECT array_sum(array_fill(1, ARRAY[3], ARRAY[lb]))
FROM (VALUES (0), (1), (9)) AS bounds(lb);


-- array_avg

-- Массив из 1 млн элементов
SELECT array_avg(million_int) FROM huge_arr;

-- Массив из 65 млн элементов smallint
SELECT array_avg(sixtyfive_million_smallint) FROM huge_arr;

-- Различные нижние границы массива
SELECT array_avg(array_fill(1, ARRAY[3], ARRAY[lb]))
FROM (VALUES (0), (1), (9)) AS bounds(lb);


-- array_min

-- Массив из 1 млн элементов
SELECT array_min(million_int) FROM huge_arr;

-- Массив из 65 млн элементов smallint
SELECT array_min(sixtyfive_million_smallint) FROM huge_arr;

-- Различные нижние границы массива
SELECT array_min(array_fill(1, ARRAY[3], ARRAY[lb]))
FROM (VALUES (0), (1), (9)) AS bounds(lb);


-- array_max

-- Массив из 1 млн элементов
SELECT array_max(million_int) FROM huge_arr;

-- Массив из 65 млн элементов smallint
SELECT array_max(sixtyfive_million_smallint) FROM huge_arr;

-- Различные нижние границы массива
SELECT array_max(array_fill(1, ARRAY[3], ARRAY[lb]))
FROM (VALUES (0), (1), (9)) AS bounds(lb);


-- array_count

-- Массив из 1 млн элементов
SELECT array_count(million_int, 1) FROM huge_arr;

-- Массив из 65 млн элементов smallint
SELECT array_count(sixtyfive_million_smallint, 1::smallint)
FROM huge_arr;

-- Различные нижние границы массива
SELECT array_count(array_fill(1, ARRAY[3], ARRAY[lb]), 1)
FROM (VALUES (0), (1), (9)) AS bounds(lb);


-- array_count_if

-- Массив из 1 млн элементов
SELECT array_count_if(million_int, 'x == 1')
FROM huge_arr;

-- Массив из 65 млн элементов smallint
SELECT array_count_if(sixtyfive_million_smallint, 'x == 1')
FROM huge_arr;

-- Различные нижние границы массива
SELECT array_count_if(array_fill(1, ARRAY[3], ARRAY[lb]), 'x == 1')
FROM (VALUES (0), (1), (9)) AS bounds(lb);

-- Пустой предикат обработка согласно ТЗ
SELECT array_count_if(ARRAY[1, 2, 3], '');

-- Ошибка парсинга
SELECT array_count_if(ARRAY[1, 2, 3], 'H % 5 + 1 - 3 = 7');


-- array_all

-- Массив из 1 млн элементов
SELECT array_all(million_int, 'x == 1')
FROM huge_arr;

-- Массив из 65 млн элементов smallint
SELECT array_all(sixtyfive_million_smallint, 'x == 1')
FROM huge_arr;

-- Различные нижние границы массива
SELECT array_all(array_fill(1, ARRAY[3], ARRAY[lb]), 'x == 1')
FROM (VALUES (0), (1), (9)) AS bounds(lb);

-- Пустой предикат обработка согласно ТЗ
SELECT array_all(ARRAY[1, 2, 3], '');

-- Ошибка парсинга
SELECT array_all(ARRAY[1, 2, 3], 'H % 5 + 1 - 3 = 7');


-- array_any

-- Массив из 1 млн элементов
SELECT array_any(million_int, 'x == 1')
FROM huge_arr;

-- Массив из 65 млн элементов smallint
SELECT array_any(sixtyfive_million_smallint, 'x == 1')
FROM huge_arr;

-- Различные нижние границы массива
SELECT array_any(array_fill(1, ARRAY[3], ARRAY[lb]), 'x == 1')
FROM (VALUES (0), (1), (9)) AS bounds(lb);

-- Пустой предикат обработка согласно ТЗ
SELECT array_any(ARRAY[1, 2, 3], '');

-- Ошибка парсинга
SELECT array_any(ARRAY[1, 2, 3], 'H % 5 + 1 - 3 = 7');

-- array_none

-- Массив из 1 млн элементов
SELECT array_none(million_int, 'x == 1')
FROM huge_arr;

-- Массив из 65 млн элементов smallint
SELECT array_none(sixtyfive_million_smallint, 'x == 1')
FROM huge_arr;

-- Различные нижние границы массива
SELECT array_none(array_fill(1, ARRAY[3], ARRAY[lb]), 'x == 1')
FROM (VALUES (0), (1), (9)) AS bounds(lb);

-- Пустой предикат обработка согласно ТЗ
SELECT array_none(ARRAY[1, 2, 3], '');

-- Ошибка парсинга
SELECT array_none(ARRAY[1, 2, 3], 'H % 5 + 1 - 3 = 7');

-- Ошибка парсинга имени в предикате (исправлена. Ранее обрезался 1 символ сразу после последнего символа имени переменной)
SELECT array_all(ARRAY[1, 2, 3, 4, 5], "x<=5"); -- ожидаемый результат: t


-- Многие варианты синтаксиса предикатов

SELECT array_count_if(ARRAY[1, 2, 3], 'x == 2');


SELECT array_count_if(ARRAY[1, 2, 3], 'x = 2');



SELECT array_count_if(ARRAY[1, 2, 3], 'x != 2');

SELECT array_count_if(ARRAY[1, 2, 3], 'x > 2');



SELECT array_count_if(ARRAY[1, 2, 3], 'x >= 2');

SELECT array_count_if(ARRAY[1, 2, 3], 'x < 2');
SELECT array_count_if(ARRAY[1, 2, 3], 'x <= 2');
SELECT array_count_if(ARRAY[1, 2, 3], 'x==2');

SELECT array_count_if(ARRAY[1, 2, 3], 'x=2');

SELECT array_count_if(ARRAY[1, 2, 3], 'x!=2');

SELECT array_count_if(ARRAY[1, 2, 3], 'x>2');

SELECT array_count_if(ARRAY[1, 2, 3], 'x>=2');

SELECT array_count_if(ARRAY[1, 2, 3], 'x<2');

SELECT array_count_if(ARRAY[1, 2, 3], 'x<=2');

SELECT array_count_if(ARRAY[1, 2, 3], 'x    >=    2');

SELECT array_count_if(ARRAY[1, 2, 3], '  x >= 2  ');

SELECT array_count_if(ARRAY[-2, -1, 0, 1, 2], 'x < -1');

SELECT array_count_if(ARRAY[100, 200, 300], 'x >= 200');

SELECT array_count_if(
    ARRAY[1.25, 2.50, 3.75]::numeric[],
    'x >= 2.50'
);

SELECT array_count_if(
    ARRAY[1.0, 2.0, 3.0]::real[],
    'x > 1.0'
);

SELECT array_count_if(
    ARRAY[1.0, 2.0, 3.0]::double precision[],
    'x <= 2.0'
);

SELECT array_count_if(
    ARRAY['-Infinity'::float8, 0, 'Infinity'::float8],
    'x == ''Infinity'''
);



SELECT array_count_if(
    ARRAY['alpha', 'beta', 'gamma']::text[],
    'x == "beta"'
);



SELECT array_count_if(
    ARRAY['alpha', 'beta', 'gamma']::text[],
    'x = "beta"'
);



SELECT array_count_if(
    ARRAY['alpha', 'beta', 'gamma']::text[],
    'x != "beta"'
);


SELECT array_count_if(
    ARRAY['alpha', 'beta', 'gamma']::varchar[],
    'x == "alpha"'
);

SELECT array_count_if(
    ARRAY['a', 'b', 'c']::char(1)[],
    'x != "b"'
);
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5, 6], 'x % 2 = 0');

SELECT array_count_if(ARRAY[1, 2, 3, 4, 5, 6], 'x % 2 = 1');

SELECT array_count_if(ARRAY[-3, -2, -1, 0, 1, 2, 3], 'x % 2 = 0');
SELECT array_count_if(ARRAY[1, 2, 3, 4], 'x % -2 = 0');
SELECT array_count_if(ARRAY[3, 12, 123], 'x % 10 = 3');


SELECT array_count_if(ARRAY[1, 2, 3, 4], 'x   %   2   =   0');

SELECT array_count_if(ARRAY[1, 2, 3, 4], 'x%2=0');

SELECT array_all(ARRAY[1, 2, 3], 'x >= 1');
SELECT array_any(ARRAY[1, 2, 3], 'x > 2'); 

SELECT array_none(ARRAY[1, 2, 3], 'x < 1'); 
SELECT array_all(ARRAY[1, 2, 3], 'x > 1'); 

SELECT array_any(ARRAY[1, 2, 3], 'x > 3'); 

SELECT array_none(ARRAY[1, 2, 3], 'x >= 1'); 

SELECT array_count_if(ARRAY[1, 2, 3], 'x ==');

SELECT array_count_if(ARRAY[1, 2, 3], 'x 2');

SELECT array_count_if(ARRAY[1, 2, 3], 'x @@@ 2');

SELECT array_count_if(ARRAY[1, 2, 3], 'x == 1 == 2');

SELECT array_count_if(ARRAY[1, 2, 3], '2 == x');

SELECT array_count_if(ARRAY['a', 'b']::text[], 'x == "a');


SELECT array_count_if(
    ARRAY['a', 'b', 'c']::text[],
    'x > "a"'
);

SELECT array_count_if(
    ARRAY[1.0, 2.0, 3.0]::real[],
    'x % 2 = 0'
);
SELECT array_count_if(
    ARRAY[1.0, 2.0, 3.0]::double precision[],
    'x % 2 = 0'
);
SELECT array_count_if(
    ARRAY['a', 'b']::text[],
    'x % 2 = 0'
);

SELECT array_count_if(ARRAY[1, 2, 3], 'x % 0 = 0');


SELECT array_count_if(ARRAY[]::int[], 'x > 0');
SELECT array_all(ARRAY[]::int[], 'x > 0');
SELECT array_any(ARRAY[]::int[], 'x > 0');
SELECT array_none(ARRAY[]::int[], 'x > 0');

SELECT array_count_if(ARRAY[1, NULL, 3], 'x > 1');
SELECT array_all(ARRAY[1, NULL, 3], 'x > 0');

DROP TABLE huge_arr;
DROP EXTENSION IF EXISTS pg_array_agg;

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

-- пустой предикат + пустой массив

SELECT array_count_if('{}'::int[], '');
SELECT array_any('{}'::int[], '');
SELECT array_all('{}'::int[], '');
SELECT array_none('{}'::int[], '');

DROP TABLE huge_arr;
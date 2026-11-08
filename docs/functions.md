### Сигнатуры функций

| Функция | Аргументы | Возвращаемый тип | Описание |
|---|---|---|---|
| `array_product(anyarray)` | массив | `numeric` | Произведение всех элементов массива |
| `array_sum(anyarray)` | массив | `numeric` | Сумма всех элементов массива |
| `array_avg(anyarray)` | массив | `numeric` | Среднее арифметическое элементов массива |
| `array_min(anyarray)` | массив | `anyelement` | Минимальный элемент массива |
| `array_max(anyarray)` | массив | `anyelement` | Максимальный элемент массива |
| `array_count(anyarray, anyelement)` | массив, значение | `integer` | Количество вхождений заданного значения |
| `array_count_if(anyarray, text)` | массив, предикат | `integer` | Количество элементов, удовлетворяющих предикату |
| `array_all(anyarray, text)` | массив, предикат | `boolean` | Истина, если все элементы удовлетворяют предикату |
| `array_any(anyarray, text)` | массив, предикат | `boolean` | Истина, если хотя бы один элемент удовлетворяет предикату |
| `array_none(anyarray, text)` | массив, предикат | `boolean` | Истина, если ни один элемент не удовлетворяет предикату |

Все функции объявлены как `IMMUTABLE` и `STRICT` (за исключением особых случаев, оговоренных ниже). Это означает, что при передаче `NULL` в качестве массива или элемента массива результат также будет `NULL` (для функций с накоплением и экстремумов), а для функций с предикатами `NULL`-элементы массива пропускаются.

### Общие правила

Поддерживаются массивы с размерностями ndim = 0, 1, 2. Пустой массив с ndim = 0 трактуется как пустой одномерный массив. Массивы с ndim > 2 вызывают ошибку.

Для функций array_product, array_sum, array_avg, array_min, array_max: если массив содержит NULL или сам массив NULL, результат — NULL.

Для функций array_count, array_count_if, array_all, array_any, array_none: элементы массива, равные NULL, пропускаются (не учитываются при подсчёте и проверке условий).

Пустой массив:
    1) array_product RETURN: 1
    
    2) array_sum RETURN: 0
    
    3) array_avg RETURN: NULL
    
    4) array_min, array_max RETURN: NULL
    
    5) array_count RETURN: 0
    
    6) array_count_if RETURN: 0
    
    7) array_all RETURN: true
    
    8) array_any RETURN: false
    
    9) array_none RETURN: true

+ Если тип элемента массива не входит в список поддерживаемых, генерируется ошибка.
+ При переполнении или делении на ноль (например, в операции %) генерируется ошибка.

### Некоторые примеры
``` sql
-- Произведение элементов
SELECT array_product(ARRAY[1, 2, 3, 4]);  -- 24
SELECT array_product(ARRAY[]::integer[]); -- 1
SELECT array_product(ARRAY[1, NULL, 3]);  -- NULL

-- Сумма и среднее
SELECT array_sum(ARRAY[1, 2, 3, 4]);      -- 10
SELECT array_avg(ARRAY[1, 2, 3, 4]);      -- 2.5
SELECT array_avg(ARRAY[]::integer[]);     -- NULL

-- Минимум и максимум
SELECT array_min(ARRAY[5, 2, 8, 1]);      -- 1
SELECT array_max(ARRAY[5, 2, 8, 1]);      -- 8

-- Подсчёт вхождений
SELECT array_count(ARRAY[1, 2, 1, 3, 1], 1); -- 3
SELECT array_count(ARRAY['a', 'b', 'a'], 'a'); -- 2

-- Подсчёт по предикату
SELECT array_count_if(ARRAY[1, 2, 3, 4, 5], 'x > 3'); -- 2
SELECT array_count_if(ARRAY['apple', 'banana', 'cherry'], 'x == "banana"'); -- 1

-- Проверка условий
SELECT array_all(ARRAY[2, 4, 6], 'x % 2 = 0');   -- true
SELECT array_any(ARRAY[1, 3, 5], 'x % 2 = 0');   -- false
SELECT array_none(ARRAY[1, 3, 5], 'x % 2 = 0');  -- true

-- Работа со специальными значениями
SELECT array_sum(ARRAY['Infinity'::numeric, 1]); -- Infinity
SELECT array_min(ARRAY['NaN'::numeric, 'Infinity'::numeric]); -- Infinity
```
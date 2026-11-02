#!/bin/bash

CLIENTS=4
TRANSACTIONS=1000
ARRAY_LEN=16000

set -e

#createdb bench_db

psql bench_db -c "CREATE TABLE IF NOT EXISTS bench_table AS\
    SELECT ARRAY(SELECT generate_series(1, $ARRAY_LEN)) AS arr;
"

psql bench_db -c "CREATE EXTENSION IF NOT EXISTS pg_array_agg"
psql bench_db -c "\dx"

pgbench -n -D n="$ARRAY_LEN" -c "$CLIENTS" -j "$(nproc)" -t "$TRANSACTIONS" -r -f bench.sql bench_db
# -c это клиенты
# -j это потоки
# -t это транзакции на клиентов
# -r это подробный отчёт по функциям
# -f это пользовательский сценарий
# -D n= знач. параметра в скрипте


psql bench_db -c "DROP EXTENSION IF EXISTS pg_array_agg"
psql bench_db -c "DROP TABLE IF EXISTS bench_table"
#!/bin/bash

# запустите это через source env.sh


export PG_PREFIX="${HOME}/PostgresRepo/Build/PostgreSQL_master"
export PG_SRC="${HOME}/postgres"

export PGDATA="${HOME}/pgdata_valgrind" # сюда поставьте кластер
export PATH=$PG_PREFIX/bin:$PATH

export LD_LIBRARY_PATH=$PG_PREFIX/lib:$LD_LIBRARY_PATH

export PGPORT=65432
export PGUSER=$(whoami)
export PGHOST=localhost
EXTENSION = pg_array_agg

TAP_TESTS = t/001_transactions.pl t/002_parallel.pl t/003_error_query.pl

DATA = pg_array_agg--1.0.sql

MODULE_big = pg_array_agg

OBJS = src/pg_array_agg.o src/utilfunc.o

REGRESS = array_agg_functions
REGRESS_OPTS = --inputdir=test --outputdir=test

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
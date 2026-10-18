EXTENSION = pg_array_agg

DATA = sql/pg_array_agg--1.0.sql

MODULE_big = pg_array_agg

OBJS = src/array_functions.o

REGRESS = array_agg_functions
REGRESS_OPTS = --inputdir=test --outputdir=test

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
EXTENSION = pg_array_agg
DATA = pg_array_agg--1.0.sql
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
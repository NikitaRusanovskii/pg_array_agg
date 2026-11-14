EXTENSION = pg_array_agg

TAP_TESTS = 1 # t/001_transactions.pl t/002_parallel.pl t/003_error_query.pl

DATA = pg_array_agg--1.0.sql

MODULE_big = pg_array_agg

OBJS = src/pg_array_agg.o \
src/utilfunc.o \
src/scaner.o \
src/parser_util.o \
src/parser.tab.o \
src/re2c_adapt.o \
src/predicate.o


# REGRESS = agg_allanynone agg_arith agg_count_if agg_count agg_minmax agg_additional agg_predicate_additional
REGRESS = agg_predicate_additional
REGRESS_OPTS = --inputdir=test --outputdir=test --load-extension=$(EXTENSION)
ifdef USE_PGXS
PG_CONFIG ?= pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
PG_CFLAGS += -Wall -Werror -Wextra
subdir = contrib/pg_array_agg
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
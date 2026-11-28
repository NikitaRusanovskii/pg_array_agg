#include "postgres.h"
#include "fmgr.h"
#include "pg_config.h"
#include "utils/array.h"
#include "utils/elog.h"
#include "utils/numeric.h"
#include "catalog/pg_type_d.h"
#include "utils/datum.h"
#include "utils/lsyscache.h"
#include "utilfunc.h"
#include "predicate.h"


PG_MODULE_MAGIC;

Datum		array_product(PG_FUNCTION_ARGS);
Datum		array_sum(PG_FUNCTION_ARGS);
Datum		array_avg(PG_FUNCTION_ARGS);
Datum		array_min(PG_FUNCTION_ARGS);
Datum		array_max(PG_FUNCTION_ARGS);
Datum		array_count(PG_FUNCTION_ARGS);
Datum		array_count_if(PG_FUNCTION_ARGS);
Datum		array_all(PG_FUNCTION_ARGS);
Datum		array_any(PG_FUNCTION_ARGS);
Datum		array_none(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(array_product);
PG_FUNCTION_INFO_V1(array_sum);
PG_FUNCTION_INFO_V1(array_avg);
PG_FUNCTION_INFO_V1(array_min);
PG_FUNCTION_INFO_V1(array_max);
PG_FUNCTION_INFO_V1(array_count);
PG_FUNCTION_INFO_V1(array_count_if);
PG_FUNCTION_INFO_V1(array_all);
PG_FUNCTION_INFO_V1(array_any);
PG_FUNCTION_INFO_V1(array_none);

static const Oid pgaa_arithm_allowed_types[] = {
	INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID
};

static inline size_t
pgaa_arithm_allowed_types_len(void)
{
	return lengthof(pgaa_arithm_allowed_types);
}

static const Oid pgaa_cmp_allowed_types[] = {
	INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID
};

static inline size_t
pgaa_cmp_allowed_types_len(void)
{
	return lengthof(pgaa_cmp_allowed_types);
}

static const Oid pgaa_cnt_allowed_types[] = {
	INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID,
	TEXTOID, VARCHAROID, BPCHAROID
};

static inline size_t
pgaa_cnt_allowed_types_len(void)
{
	return lengthof(pgaa_cnt_allowed_types);
}

static const Oid pgaa_pred_allowed_types[] = {
	INT2OID, INT4OID, INT8OID, NUMERICOID, FLOAT4OID, FLOAT8OID,
	TEXTOID, VARCHAROID, BPCHAROID
};

static inline size_t
pgaa_pred_allowed_types_len(void)
{
	return lengthof(pgaa_pred_allowed_types);
}

static void
func_arg_check(const char *func_name, size_t ndim, Oid elemtype, const Oid *expected_types, size_t arr_len)
{
	if (ndim > 2)
	{
		ereport(ERROR,
				(errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
				 errmsg("%s: Invalid ndim of array: %ld", func_name, ndim)));
		pg_unreachable();
	}

	if (!pgaa_type_is_expected(elemtype, expected_types, arr_len))
	{
		ereport(ERROR,
				(errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
				 errmsg("%s: Invalid elemtype of array. OID: %d", func_name, elemtype)));
		pg_unreachable();
	}
}

Datum
array_product(PG_FUNCTION_ARGS)
{
	ArrayType  *arr;
	ArrayIterator arr_it;
	Datum		elem;
	bool		is_null = false;
	Numeric		res;
	Numeric		tmp_elem;
	Numeric		tmp_res;
#if PG_VERSION_NUM < 190000
	bool		err = false;
#endif

	arr = PG_GETARG_ARRAYTYPE_P(0);

	func_arg_check("array_product", ARR_NDIM(arr), arr->elemtype, pgaa_arithm_allowed_types, pgaa_arithm_allowed_types_len());

	arr_it = array_create_iterator(arr, 0, NULL);

	res = int64_to_numeric(1);

	while (array_iterate(arr_it, &elem, &is_null))
	{

		if (is_null)
		{
			PG_FREE_IF_COPY(arr, 0);
			array_free_iterator(arr_it);
			pfree(res);
			PG_RETURN_NULL();
		}
		tmp_elem = pgaa_datum_to_numeric(elem, arr->elemtype);
#if PG_VERSION_NUM >= 190000
		tmp_res = numeric_mul_safe(res, tmp_elem, NULL);
#else
		tmp_res = numeric_mul_opt_error(res, tmp_elem, &err);
		if (err)
		{
			pfree(res);
			pfree(tmp_elem);
			PG_FREE_IF_COPY(arr, 0);
			array_free_iterator(arr_it);
			ereport(ERROR,
					(errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
					 errmsg("array_product: An error occurred during a numeric multiplication.")));
		}
#endif
		pfree(res);
		pfree(tmp_elem);
		res = tmp_res;
	}

	PG_FREE_IF_COPY(arr, 0);
	array_free_iterator(arr_it);

	PG_RETURN_NUMERIC(res);
}

Datum
array_sum(PG_FUNCTION_ARGS)
{
	ArrayType  *arr;
	ArrayIterator arr_it;
	Datum		elem;
	bool		is_null = false;
	Numeric		res;
	Numeric		tmp_elem;
	Numeric		tmp_res;
#if PG_VERSION_NUM < 190000
	bool		err = false;
#endif

	arr = PG_GETARG_ARRAYTYPE_P(0);

	func_arg_check("array_sum", ARR_NDIM(arr), arr->elemtype, pgaa_arithm_allowed_types, pgaa_arithm_allowed_types_len());

	arr_it = array_create_iterator(arr, 0, NULL);


	res = int64_to_numeric(0);

	while (array_iterate(arr_it, &elem, &is_null))
	{

		if (is_null)
		{
			PG_FREE_IF_COPY(arr, 0);
			array_free_iterator(arr_it);
			pfree(res);
			PG_RETURN_NULL();
		}

		tmp_elem = pgaa_datum_to_numeric(elem, arr->elemtype);
#if PG_VERSION_NUM >= 190000
		tmp_res = numeric_add_safe(res, tmp_elem, NULL);
#else
		tmp_res = numeric_add_opt_error(res, tmp_elem, &err);
		if (err)
		{
			pfree(res);
			pfree(tmp_elem);
			PG_FREE_IF_COPY(arr, 0);
			array_free_iterator(arr_it);
			ereport(ERROR,
					(errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
					 errmsg("array_sum: An error occurred during a numeric sum.")));
		}
#endif
		pfree(res);
		pfree(tmp_elem);
		res = tmp_res;
	}
	PG_FREE_IF_COPY(arr, 0);
	array_free_iterator(arr_it);

	PG_RETURN_NUMERIC(res);
}

Datum
array_avg(PG_FUNCTION_ARGS)
{
	ArrayType  *arr;
	ArrayIterator arr_it;
	Datum		elem;
	bool		is_null = false;
	Numeric		res;
	Numeric		total;
	Numeric		tmp_elem;
	Numeric		tmp_res;
	Numeric		final_res;
	uint64		count;
#if PG_VERSION_NUM < 190000
	bool		err = false;
#endif

	arr = PG_GETARG_ARRAYTYPE_P(0);

	func_arg_check("array_avg", ARR_NDIM(arr), arr->elemtype, pgaa_arithm_allowed_types, pgaa_arithm_allowed_types_len());

	if (ArrayGetNItems(ARR_NDIM(arr), ARR_DIMS(arr)) == 0)
	{
		PG_FREE_IF_COPY(arr, 0);
		PG_RETURN_NULL();
	}

	arr_it = array_create_iterator(arr, 0, NULL);


	res = int64_to_numeric(0);
	count = 0;

	while (array_iterate(arr_it, &elem, &is_null))
	{

		if (is_null)
		{
			PG_FREE_IF_COPY(arr, 0);
			array_free_iterator(arr_it);
			pfree(res);
			PG_RETURN_NULL();
		}

		tmp_elem = pgaa_datum_to_numeric(elem, arr->elemtype);
#if PG_VERSION_NUM >= 190000
		tmp_res = numeric_add_safe(res, tmp_elem, NULL);
#else
		tmp_res = numeric_add_opt_error(res, tmp_elem, &err);
		if (err)
		{
			pfree(res);
			pfree(tmp_elem);
			PG_FREE_IF_COPY(arr, 0);
			array_free_iterator(arr_it);
			ereport(ERROR,
					(errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
					 errmsg("array_avg: An error occurred during a numeric sum.")));
		}
#endif
		pfree(res);
		pfree(tmp_elem);
		res = tmp_res;
		count++;
	}
	total = pgaa_uint64_get_numeric(count);
#if PG_VERSION_NUM >= 190000
	final_res = numeric_div_safe(res, total, NULL);
#else
	final_res = numeric_div_opt_error(res, total, &err);
	if (err)
	{
		pfree(res);
		pfree(total);
		PG_FREE_IF_COPY(arr, 0);
		array_free_iterator(arr_it);
		ereport(ERROR,
				(errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
				 errmsg("array_avg: An error occurred during a numeric div.")));
	}
#endif
	PG_FREE_IF_COPY(arr, 0);
	array_free_iterator(arr_it);
	pfree(res);
	pfree(total);
	PG_RETURN_NUMERIC(final_res);
}

Datum
array_min(PG_FUNCTION_ARGS)
{
	ArrayType  *arr;
	ArrayIterator arr_it;
	Datum		elem;
	Datum		res;
	bool		typ_by_val;
	int16		typ_len;
	bool		is_null = false;

	arr = PG_GETARG_ARRAYTYPE_P(0);

	func_arg_check("array_min", ARR_NDIM(arr), arr->elemtype, pgaa_cmp_allowed_types, pgaa_cmp_allowed_types_len());

	if (ArrayGetNItems(ARR_NDIM(arr), ARR_DIMS(arr)) == 0)
	{
		PG_FREE_IF_COPY(arr, 0);
		PG_RETURN_NULL();
	}

	arr_it = array_create_iterator(arr, 0, NULL);

	if (array_contains_nulls(arr))
	{
		PG_FREE_IF_COPY(arr, 0);
		array_free_iterator(arr_it);
		PG_RETURN_NULL();
	}


	array_iterate(arr_it, &elem, &is_null); /* После проверки
											 * длины, это всегда
											 * true, поскольку существует хотя бы
											 * 1 элемент */
	res = elem;
	while (array_iterate(arr_it, &elem, &is_null))
	{
		if (pgaa_cmp_op(arr->elemtype, res, elem, PG_GET_COLLATION()) > 0)
			res = elem;

	}

	array_free_iterator(arr_it);

	get_typlenbyval(arr->elemtype, &typ_len, &typ_by_val);
	res = datumCopy(res, typ_by_val, typ_len);

	PG_FREE_IF_COPY(arr, 0);
	PG_RETURN_DATUM(res);
}

Datum
array_max(PG_FUNCTION_ARGS)
{
	ArrayType  *arr;
	ArrayIterator arr_it;
	Datum		elem;
	Datum		res;
	bool		typ_by_val;
	int16		typ_len;
	bool		is_null = false;

	arr = PG_GETARG_ARRAYTYPE_P(0);

	func_arg_check("array_max", ARR_NDIM(arr), arr->elemtype, pgaa_cmp_allowed_types, pgaa_cmp_allowed_types_len());

	if (ArrayGetNItems(ARR_NDIM(arr), ARR_DIMS(arr)) == 0)
	{
		PG_FREE_IF_COPY(arr, 0);
		PG_RETURN_NULL();
	}

	arr_it = array_create_iterator(arr, 0, NULL);
	if (array_contains_nulls(arr))
	{
		PG_FREE_IF_COPY(arr, 0);
		array_free_iterator(arr_it);
		PG_RETURN_NULL();
	}

	array_iterate(arr_it, &elem, &is_null);
	res = elem;
	while (array_iterate(arr_it, &elem, &is_null))
	{
		if (pgaa_cmp_op(arr->elemtype, res, elem, PG_GET_COLLATION()) < 0)
			res = elem;

	}

	array_free_iterator(arr_it);

	get_typlenbyval(arr->elemtype, &typ_len, &typ_by_val);
	res = datumCopy(res, typ_by_val, typ_len);

	PG_FREE_IF_COPY(arr, 0);
	PG_RETURN_DATUM(res);
}

Datum
array_count(PG_FUNCTION_ARGS)
{
	ArrayType  *arr;
	Datum		element;
	ArrayIterator arr_it;
	Datum		elem;
	uint64		result = 0;
	bool		is_null = false;

	arr = PG_GETARG_ARRAYTYPE_P(0);
	element = PG_GETARG_DATUM(1);

	func_arg_check("array_count", ARR_NDIM(arr), arr->elemtype, pgaa_cnt_allowed_types, pgaa_cnt_allowed_types_len());

	arr_it = array_create_iterator(arr, 0, NULL);
	while (array_iterate(arr_it, &elem, &is_null))
	{

		if (is_null)
		{
			continue;
		}

		if (pgaa_cmp_op(arr->elemtype, element, elem, PG_GET_COLLATION()) == 0)
			result++;
		if (result > INT32_MAX)
			ereport(ERROR,
					errcode(ERRCODE_ARRAY_ELEMENT_ERROR),
					errmsg("Too many elements in array"));

	}
	PG_FREE_IF_COPY(arr, 0);
	array_free_iterator(arr_it);
	PG_RETURN_INT32((int32) result);
}

Datum
array_count_if(PG_FUNCTION_ARGS)
{
	ArrayType  *arr;
	Oid			collation = PG_GET_COLLATION();
	Oid			elemtype;
	Datum		predicate;
	Datum		current_element;
	ArrayIterator iterator;
	bool		is_null;
	uint64		result;
	PGAA_ParserCtx *predicate_ctx;

	arr = PG_GETARG_ARRAYTYPE_P(0);

	func_arg_check("array_count_if", ARR_NDIM(arr), arr->elemtype, pgaa_pred_allowed_types, pgaa_pred_allowed_types_len());

	predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));

	elemtype = arr->elemtype;
	result = 0;
	iterator = array_create_iterator(arr, 0, NULL);

	predicate_ctx = pgaa_prepare_predicate(predicate);
	if (predicate_ctx == NULL)
	{
		PG_FREE_IF_COPY(arr, 0);
		PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
		array_free_iterator(iterator);
		PG_RETURN_INT32(0);
	}
	while (array_iterate(iterator, &current_element, &is_null))
	{
		if (is_null)
		{

			continue;
		}

		if (pgaa_execute_predicate(predicate_ctx, current_element, elemtype, collation))
			result++;
		if (result > INT32_MAX)
			ereport(ERROR,
					errcode(ERRCODE_ARRAY_ELEMENT_ERROR),
					errmsg("Too many elements in array"));
	}

	PG_FREE_IF_COPY(arr, 0);
	PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
	array_free_iterator(iterator);
	pgaa_free_predicate(predicate_ctx);
	PG_RETURN_INT32((int32) result);
}


Datum
array_any(PG_FUNCTION_ARGS)
{
	ArrayType  *arr;
	Oid			collation = PG_GET_COLLATION();
	Oid			elemtype;
	Datum		predicate;
	PGAA_ParserCtx *predicate_ctx;
	Datum		current_element;
	ArrayIterator iterator;
	bool		is_null;

	arr = PG_GETARG_ARRAYTYPE_P(0);

	func_arg_check("array_any", ARR_NDIM(arr), arr->elemtype, pgaa_pred_allowed_types, pgaa_pred_allowed_types_len());

	predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));

	elemtype = arr->elemtype;
	iterator = array_create_iterator(arr, 0, NULL);

	predicate_ctx = pgaa_prepare_predicate(predicate);
	if (predicate_ctx == NULL)
	{
		PG_FREE_IF_COPY(arr, 0);
		PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
		array_free_iterator(iterator);
		PG_RETURN_BOOL(false);
	}
	while (array_iterate(iterator, &current_element, &is_null))
	{
		if (is_null)
		{
			continue;
		}

		if (pgaa_execute_predicate(predicate_ctx, current_element, elemtype, collation))
		{
			PG_FREE_IF_COPY(arr, 0);
			PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
			array_free_iterator(iterator);
			pgaa_free_predicate(predicate_ctx);
			PG_RETURN_BOOL(true);
		}
	}

	PG_FREE_IF_COPY(arr, 0);
	PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
	array_free_iterator(iterator);
	pgaa_free_predicate(predicate_ctx);
	PG_RETURN_BOOL(false);
}

Datum
array_all(PG_FUNCTION_ARGS)
{
	ArrayType  *arr;
	Oid			collation = PG_GET_COLLATION();
	Oid			elemtype;
	Datum		predicate;
	PGAA_ParserCtx *predicate_ctx;
	Datum		current_element;
	ArrayIterator iterator;
	bool		is_null;
	int			arr_len;

	arr = PG_GETARG_ARRAYTYPE_P(0);
	arr_len = ArrayGetNItems(ARR_NDIM(arr), ARR_DIMS(arr));

	func_arg_check("array_all", ARR_NDIM(arr), arr->elemtype, pgaa_pred_allowed_types, pgaa_pred_allowed_types_len());

	predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));

	elemtype = arr->elemtype;
	iterator = array_create_iterator(arr, 0, NULL);

	predicate_ctx = pgaa_prepare_predicate(predicate);
	if (predicate_ctx == NULL)
	{
		PG_FREE_IF_COPY(arr, 0);
		PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
		array_free_iterator(iterator);
		if (arr_len == 0)
			PG_RETURN_BOOL(true);
		PG_RETURN_BOOL(false);
	}
	while (array_iterate(iterator, &current_element, &is_null))
	{
		if (is_null)
		{
			continue;
		}

		if (!pgaa_execute_predicate(predicate_ctx, current_element, elemtype, collation))
		{
			PG_FREE_IF_COPY(arr, 0);
			PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
			array_free_iterator(iterator);
			pgaa_free_predicate(predicate_ctx);
			PG_RETURN_BOOL(false);
		}
	}

	PG_FREE_IF_COPY(arr, 0);
	PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
	array_free_iterator(iterator);
	pgaa_free_predicate(predicate_ctx);
	PG_RETURN_BOOL(true);
}

Datum
array_none(PG_FUNCTION_ARGS)
{
	ArrayType  *arr;
	Oid			collation = PG_GET_COLLATION();
	Oid			elemtype;
	Datum		predicate;
	PGAA_ParserCtx *predicate_ctx;
	Datum		current_element;
	ArrayIterator iterator;
	bool		is_null;

	arr = PG_GETARG_ARRAYTYPE_P(0);

	func_arg_check("array_none", ARR_NDIM(arr), arr->elemtype, pgaa_pred_allowed_types, pgaa_pred_allowed_types_len());

	predicate = PointerGetDatum(PG_GETARG_TEXT_P(1));

	elemtype = arr->elemtype;
	iterator = array_create_iterator(arr, 0, NULL);

	predicate_ctx = pgaa_prepare_predicate(predicate);
	if (predicate_ctx == NULL)
	{
		PG_FREE_IF_COPY(arr, 0);
		PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
		array_free_iterator(iterator);
		PG_RETURN_BOOL(true);
	}
	while (array_iterate(iterator, &current_element, &is_null))
	{
		if (is_null)
		{

			continue;
		}

		if (pgaa_execute_predicate(predicate_ctx, current_element, elemtype, collation))
		{
			PG_FREE_IF_COPY(arr, 0);
			PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
			array_free_iterator(iterator);
			pgaa_free_predicate(predicate_ctx);
			PG_RETURN_BOOL(false);
		}
	}

	PG_FREE_IF_COPY(arr, 0);
	PG_FREE_IF_COPY(DatumGetPointer(predicate), 1);
	array_free_iterator(iterator);
	pgaa_free_predicate(predicate_ctx);
	PG_RETURN_BOOL(true);
}

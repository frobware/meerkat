#include <string.h>
#include <dsio/dsio.h>
#include "array.h"

static char *element_ptr(struct dsio_array *a, size_t idx)
{
	return (char *)a->data + (idx * a->element_size);
}

void dsio_array_init(const struct dsio_allocator *allocator,
		     struct dsio_array *array,
		     size_t element_size)
{
	array->data = NULL;
	array->len = 0;
	array->cap = 0;
	array->element_size = element_size;
	array->allocator = allocator;
}

int dsio_array_capacity_set(struct dsio_array *array, size_t new_size)
{
	void *new_data;
	size_t nbytes, curr_cap = array->cap;

	if (new_size < curr_cap)
		return DSIO_OK;

	if (array->cap == 0)
		curr_cap = 1;

	while (curr_cap <= new_size)
		curr_cap *= 2;

	nbytes = curr_cap * array->element_size;
	new_data = DSIO_REALLOC(array->allocator, array->data, nbytes);

	if (new_data == NULL)
		return DSIO_NOMEM;

	array->cap = curr_cap;
	array->data = new_data;

	return DSIO_OK;
}

void dsio_array_release(struct dsio_array *array)
{
	DSIO_FREE(array->allocator, array->data);
	array->data = NULL;
	array->len = array->cap = 0;
}

int dsio_array_append(struct dsio_array *array, const void *val)
{
	int rc;

	if ((rc = dsio_array_capacity_set(array, array->len)) == DSIO_OK) {
		memcpy(element_ptr(array, array->len), val, array->element_size);
		array->len++;
	}

	return rc;
}

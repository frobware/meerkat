/* dynamic array, similar to glib */

#pragma once

#include <stddef.h>

#define dsio_array_len(A) ((A)->len)
#define dsio_array_element_size(A) ((A)->element_size)
#define dsio_array_cap(A) ((A)->cap)
#define dsio_array_val(A, TYPE, IDX) (((TYPE *)(void *)(A)->data)[(IDX)])
#define dsio_array_val_ptr(A, TYPE, IDX) (&((TYPE *)(void *)(A)->data)[(IDX)])

struct dsio_array {
	void *data;
	size_t len;
	size_t cap;
	size_t element_size;
	const struct dsio_allocator *allocator;
};

/* Initializes the array.
 *
 * Its initial capacity and length is 0.
 *
 * @array	 - pointer to array to initialize
 * @allocator	 - memory allocator
 * @element_size - size of one element (e.g., sizeof(int)).
 */
extern void dsio_array_init(
	const struct dsio_allocator *allocator,
	struct dsio_array *array,
	size_t element_size);

/* Increase the capacity to NEW_CAP.
 *
 * Return DSIO_OK on success, otherwise DSIO_NOMEM if out of memory.
 */
extern int dsio_array_capacity_set(struct dsio_array *array, size_t new_cap);

/* Release the data used to manage the elements. */
extern void dsio_array_release(struct dsio_array *array);

/* Add element to the end of the array, expanding the array if necessary.
 *
 * Returns DSIO_OK on success, otherwise DSIO_NOMEM if out of memory.
 *
 * @val - value to append
 */
extern int dsio_array_append(struct dsio_array *array, const void *val);

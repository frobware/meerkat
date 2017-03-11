#pragma once

#include <stdlib.h>

#define DSIO_ALLOC(A, SIZE) (((A)->alloc)((A), (SIZE)))
#define DSIO_FREE(A, PTR) (((A)->free)((A), (PTR)))
#define DSIO_REALLOC(A, PTR, SIZE) (((A)->realloc)((A), (PTR), (SIZE)))

struct dsio_allocator {
	void *(*alloc) (const struct dsio_allocator *, size_t size);
	void *(*realloc) (const struct dsio_allocator *, void *p, size_t size);
	void (*free) (const struct dsio_allocator *, void *p);
};

extern struct dsio_allocator *dsio_stdlib_allocator;

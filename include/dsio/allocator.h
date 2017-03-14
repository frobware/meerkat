#pragma once

#include <stdlib.h>

#define DSIO_MALLOC(A, SIZE) (((A)->malloc)((A), (SIZE)))
#define DSIO_FREE(A, PTR) (((A)->free)((A), (PTR)))
#define DSIO_REALLOC(A, PTR, SIZE) (((A)->realloc)((A), (PTR), (SIZE)))

struct dsio_allocator;

typedef void *(*DSIO_MALLOC_FUNC) (const struct dsio_allocator *, size_t size);
typedef void *(*DSIO_REALLOC_FUNC) (const struct dsio_allocator *, void *p, size_t size);
typedef void (*DSIO_FREE_FUNC) (const struct dsio_allocator *, void *p);

struct dsio_allocator {
	DSIO_MALLOC_FUNC malloc;
	DSIO_REALLOC_FUNC realloc;
	DSIO_FREE_FUNC free;
};

extern struct dsio_allocator *dsio_stdlib_allocator;

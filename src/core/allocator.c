#include <stdio.h>
#include <stdlib.h>
#include <dsio/allocator.h>
#include <assert.h>

struct stdlib_allocator {
	struct dsio_allocator base;
	unsigned long long nallocs;
	unsigned long long nfrees;
};

static void *dsio_malloc(const struct dsio_allocator *a, size_t n)
{
	void *p = malloc(n);
	if (p != NULL) {
		((struct stdlib_allocator *)a)->nallocs++;
	}
	return p;
}

static void *dsio_realloc(const struct dsio_allocator *a, void *ptr, size_t n)
{
	if (ptr == NULL) {
		return dsio_malloc(a, n);
	} else {
		return realloc(ptr, n);
	}
}

static void dsio_free(const struct dsio_allocator *a, void *ptr)
{
	if (ptr != NULL) {
		((struct stdlib_allocator *)a)->nfrees++;
		free(ptr);
	}
}

static struct stdlib_allocator dsio_allocator = {
	{dsio_malloc, dsio_realloc, dsio_free}, 0, 0
};

struct dsio_allocator *dsio_stdlib_allocator = &dsio_allocator.base;

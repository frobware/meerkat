#include <stdio.h>
#include <stdlib.h>
#include <dsio/allocator.h>
#include <assert.h>

struct stdlib_allocator {
	struct dsio_allocator base;
	unsigned long long nallocs;
	unsigned long long nfrees;
};

static void *dsio_alloc(const struct dsio_allocator *a, size_t size)
{
	void *p = malloc(size);
	if (p != NULL) {
		((struct stdlib_allocator *)a)->nallocs++;
	}
	return p;
}

static void *dsio_realloc(const struct dsio_allocator *a, void *ptr,
			  size_t size)
{
	if (ptr == NULL)
		return dsio_alloc(a, size);
	else
		return realloc(ptr, size);
}

static void dsio_free(const struct dsio_allocator *a, void *ptr)
{
	if (ptr != NULL) {
		((struct stdlib_allocator *)a)->nfrees++;
		free(ptr);
	}
}

static struct stdlib_allocator dsio_allocator = {
	{ dsio_alloc, dsio_realloc, dsio_free }, 0, 0
};

struct dsio_allocator *dsio_allocator_create(void)
{
	return &dsio_allocator.base;
}

void dsio_allocator_delete(const struct dsio_allocator *allocator)
{
	assert(allocator == &dsio_allocator.base);
}

struct dsio_allocator *dsio_stdlib_allocator = &dsio_allocator.base;

#if 0
static void allocator_dtor(void) __attribute__ ((destructor));

static void allocator_dtor(void)
{
	if (getenv("DSIO_ALLOCATOR_PRINT_STATS") == NULL)
		return;

	printf("dsio_allocator_nallocs = %llu\n"
	       "dsio_allocator_nfrees  = %llu\n",
	       dsio_allocator.nallocs,
	       dsio_allocator.nfrees);
}
#endif

/*
 * Copyright 2017 Andrew McDermott
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <dsio/allocator.h>
#include <assert.h>

struct stdlib_allocator {
	struct dsio_allocator base;
	unsigned long long nallocs;
	unsigned long long nfrees;
};

static void *dsio_alloc(const struct dsio_allocator *a, size_t n)
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
		return dsio_alloc(a, n);
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
	{dsio_alloc, dsio_realloc, dsio_free}, 0, 0
};

struct dsio_allocator *dsio_stdlib_allocator = &dsio_allocator.base;

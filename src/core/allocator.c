/* Copyright (C) 2017 Andrew McDermott
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

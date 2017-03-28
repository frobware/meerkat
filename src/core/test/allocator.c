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

#include <dsio/allocator.h>

struct test_allocator {
	struct dsio_allocator base;
	int malloc_fail;
	int realloc_fail;
};

static void *test_allocator_realloc(const struct dsio_allocator *a, void *ptr, size_t size)
{
	struct test_allocator *ta = (struct test_allocator *)a;

	if (ta->realloc_fail) {
		return NULL;
	}

	return DSIO_REALLOC(dsio_stdlib_allocator, ptr, size);
}

static void *test_allocator_malloc(const struct dsio_allocator *a, size_t size)
{
	struct test_allocator *ta = (struct test_allocator *)a;

	if (ta->malloc_fail) {
		return NULL;
	}

	return test_allocator_realloc(a, NULL, size);
}

static void test_allocator_free(const struct dsio_allocator *a, void *ptr)
{
	DSIO_FREE(dsio_stdlib_allocator, ptr);
}

static struct test_allocator _test_allocator = {
	{test_allocator_malloc, test_allocator_realloc, test_allocator_free}, 0, 0
};

static struct dsio_allocator *test_allocator = &_test_allocator.base;

static void test_allocator_malloc_fail(int value)
{
	_test_allocator.malloc_fail = value;
}

static void test_allocator_realloc_fail(int value)
{
	_test_allocator.realloc_fail = value;
}

static void test_allocator_reset(void)
{
	test_allocator_malloc_fail(0);
	test_allocator_realloc_fail(0);
}

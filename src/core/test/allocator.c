/* Copyright 2017 Andrew McDermott
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

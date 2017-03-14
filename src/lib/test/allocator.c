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

static DSIO_MALLOC_FUNC orig_malloc;
static DSIO_REALLOC_FUNC orig_realloc;
static DSIO_FREE_FUNC orig_free;

static int test_allocator_malloc_fail_next;
static int test_allocator_realloc_fail_next;

static void test_allocator_restore(void)
{
	dsio_stdlib_allocator->malloc = orig_malloc;
	dsio_stdlib_allocator->realloc = orig_realloc;
	dsio_stdlib_allocator->free = orig_free;
}

static void *test_allocator_malloc(const struct dsio_allocator *a, size_t size)
{
	if (test_allocator_malloc_fail_next) {
		return NULL;
	}

	return orig_malloc(a, size);
}

static void *test_allocator_realloc(const struct dsio_allocator *a, void *ptr, size_t size)
{
	if (test_allocator_realloc_fail_next) {
		return NULL;
	}

	return orig_realloc(a, ptr, size);
}

static void test_allocator_free(const struct dsio_allocator *a, void *ptr)
{
	orig_free(a, ptr);
}

static void test_allocator_intercept(void)
{
	if (!orig_malloc || !orig_realloc || !orig_free) {
		orig_malloc = dsio_stdlib_allocator->malloc;
		orig_realloc = dsio_stdlib_allocator->realloc;
		orig_free = dsio_stdlib_allocator->free;
	}
	dsio_stdlib_allocator->malloc = test_allocator_malloc;
	dsio_stdlib_allocator->realloc = test_allocator_realloc;
	dsio_stdlib_allocator->free = test_allocator_free;
	test_allocator_malloc_fail_next = 0;
	test_allocator_realloc_fail_next = 0;
}

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

struct test_allocator {
	struct dsio_allocator base;
	int alloc_fail_now;
	int realloc_fail_now;
};

static void *test_allocator_alloc(const struct dsio_allocator *a, size_t size)
{
	void *p;
	struct test_allocator *ta = (struct test_allocator *)a;

	if (ta->alloc_fail_now) {
		return NULL;
	}

	if ((p = malloc(size)) == NULL) {
		return NULL;
	}

	return p;
}

static void *test_allocator_realloc(const struct dsio_allocator *a, void *ptr, size_t size)
{
	void *p;
	struct test_allocator *ta = (struct test_allocator *)a;

	if (ta->realloc_fail_now) {
		return NULL;
	}

	if ((p = realloc(ptr, size)) == NULL) {
		return NULL;
	}

	return p;
}

static void test_allocator_free(const struct dsio_allocator *a, void *ptr)
{
	if (ptr != NULL)
		free(ptr);
}

static struct test_allocator _test_allocator = {
	{
		test_allocator_alloc,
		test_allocator_realloc,
		test_allocator_free,
	}
};

static void test_allocator_reset(struct test_allocator *a)
{
	a->alloc_fail_now = 0;
	a->realloc_fail_now = 0;
}

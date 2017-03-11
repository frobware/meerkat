/* Copyright 2017 Andrew McDermott
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You
 * may obtain a copy of the License at:
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <dsio/dsio.h>
#include "../mprintf.h"
#include "allocator.c"
#include "CUnitTest.h"

static struct test_allocator *ta = &_test_allocator;

static int test_alloc_succeeds(void)
{
	test_allocator_reset(ta);
	void *p = ta->base.alloc(&ta->base, 100);
	CUT_ASSERT_NOT_NULL(p);
	ta->base.free(&ta->base, p);
	return 0;
}

static int test_alloc_fails(void)
{
	test_allocator_reset(ta);
	ta->alloc_fail_now = 1;
	void *p = ta->base.alloc(&ta->base, 100);
	CUT_ASSERT_NULL(p);
	return 0;
}

static int test_realloc_succeeds(void)
{
	test_allocator_reset(ta);
	void *p = ta->base.alloc(&ta->base, 100);
	CUT_ASSERT_NOT_NULL(p);
	p = ta->base.realloc(&ta->base, p, 200);
	CUT_ASSERT_NOT_NULL(p);
	ta->base.free(&ta->base, p);
	return 0;
}

static int test_realloc_fails(void)
{
	test_allocator_reset(ta);
	void *p = ta->base.alloc(&ta->base, 100);
	CUT_ASSERT_NOT_NULL(p);
	ta->realloc_fail_now = 1;
	void *q = ta->base.realloc(&ta->base, p, 200);
	CUT_ASSERT_NULL(q);
	ta->base.free(&ta->base, p);
	return 0;
}

CUT_BEGIN_TEST_HARNESS(allocator_suite)
    CUT_RUN_TEST(test_alloc_succeeds);
CUT_RUN_TEST(test_alloc_fails);
CUT_RUN_TEST(test_realloc_succeeds);
CUT_RUN_TEST(test_realloc_fails);
CUT_END_TEST_HARNESS

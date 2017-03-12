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
#include <dsio/dsio.h>
#include <dsio/allocator.h>
#include <assert.h>
#include "allocator.c"
#include "../mprintf.h"
#include "CUnitTest.h"

static struct test_allocator *ta = &_test_allocator;

static int test_mprintf_empty_fails(void)
{
	test_allocator_reset(ta);
	char *s = dsio_mprintf(&ta->base, "%Q", NULL);
	CUT_ASSERT_NOT_NULL(s);
	ta->base.free(&ta->base, s);
	return 0;
}

static int test_mprintf_alloc_succeeds(void)
{
	test_allocator_reset(ta);
	char *s = dsio_mprintf(&ta->base, "%s", "test_mprintf_alloc_succeeds");
	CUT_ASSERT_NOT_NULL(s);
	ta->base.free(&ta->base, s);
	return 0;
}

static int test_mprintf_alloc_fails(void)
{
	test_allocator_reset(ta);
	ta->alloc_fail_now = 1;
	ta->realloc_fail_now = 1;
	char *s = dsio_mprintf(&ta->base, "%s", "test_mprintf_succeeds");
	CUT_ASSERT_NULL(s);
	return 0;
}

CUT_BEGIN_TEST_HARNESS(mprintf_suite)
    CUT_RUN_TEST(test_mprintf_empty_fails);
CUT_RUN_TEST(test_mprintf_alloc_succeeds);
CUT_RUN_TEST(test_mprintf_alloc_fails);
CUT_END_TEST_HARNESS
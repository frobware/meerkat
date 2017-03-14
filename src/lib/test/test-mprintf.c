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
#include <string.h>
#include <dsio/dsio.h>
#include <dsio/allocator.h>
#include <assert.h>
#include "../mprintf.h"
#include "allocator.c"
#include "CUnitTest.h"

static int test_mprintf_alloc_succeeds(void)
{
	test_allocator_intercept();
	char *s = dsio_mprintf(dsio_stdlib_allocator, "%s", "test_mprintf_alloc_succeeds");
	CUT_ASSERT_NOT_NULL(s);
	DSIO_FREE(dsio_stdlib_allocator, s);
	return 0;
}

static int test_mprintf_alloc_fails(void)
{
	test_allocator_intercept();
	test_allocator_malloc_fail_next = 1;
	test_allocator_realloc_fail_next = 1;
	char *s = dsio_mprintf(dsio_stdlib_allocator, "%s", "test_mprintf_succeeds");
	CUT_ASSERT_NULL(s);
	test_allocator_restore();
	return 0;
}

CUT_BEGIN_TEST_HARNESS(mprintf_suite)
CUT_RUN_TEST(test_mprintf_alloc_succeeds);
CUT_RUN_TEST(test_mprintf_alloc_fails);
CUT_END_TEST_HARNESS

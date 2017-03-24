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
#include <string.h>
#include <assert.h>
#include <dsio/dsio.h>
#include "../mprintf.h"
#include "allocator.c"
#include "CUnitTest.h"

static int test_alloc_succeeds(void)
{
	void *p;
	test_allocator_reset();
	p = DSIO_MALLOC(test_allocator, 100);
	CUT_ASSERT_NOT_NULL(p);
	DSIO_FREE(test_allocator, p);
	return 0;
}

static int test_alloc_fails(void)
{
	test_allocator_reset();
	test_allocator_malloc_fail(1);
	void *p = DSIO_MALLOC(test_allocator, 100);
	CUT_ASSERT_NULL(p);
	return 0;
}

static int test_realloc_succeeds(void)
{
	void *p;
	test_allocator_reset();
	p = DSIO_MALLOC(test_allocator, 100);
	CUT_ASSERT_NOT_NULL(p);
	p = DSIO_REALLOC(test_allocator, p, 200);
	CUT_ASSERT_NOT_NULL(p);
	DSIO_FREE(test_allocator, p);
	return 0;
}

static int test_realloc_fails(void)
{
	void *p, *q;
	test_allocator_reset();
	p = DSIO_MALLOC(test_allocator, 100);
	CUT_ASSERT_NOT_NULL(p);
	test_allocator_realloc_fail(1);
	q = DSIO_REALLOC(test_allocator, p, 200);
	CUT_ASSERT_NULL(q);
	DSIO_FREE(test_allocator, p);
	return 0;
}

CUT_BEGIN_TEST_HARNESS(allocator_suite)
CUT_RUN_TEST(test_alloc_succeeds);
CUT_RUN_TEST(test_alloc_fails);
CUT_RUN_TEST(test_realloc_succeeds);
CUT_RUN_TEST(test_realloc_fails);
CUT_END_TEST_HARNESS

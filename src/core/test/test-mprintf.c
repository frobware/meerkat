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
#include <dsio/dsio.h>
#include <dsio/allocator.h>
#include <assert.h>
#include "../mprintf.h"
#include "allocator.c"
#include "CUnitTest.h"

static int test_mprintf_alloc_succeeds(void)
{
	test_allocator_reset();
	char *s = dsio_mprintf(test_allocator, "%s", "test_mprintf_alloc_succeeds");
	CUT_ASSERT_NOT_NULL(s);
	DSIO_FREE(test_allocator, s);
	return 0;
}

static int test_mprintf_alloc_fails(void)
{
	test_allocator_malloc_fail(1);
	test_allocator_realloc_fail(1);
	char *s = dsio_mprintf(test_allocator, "%s", "test_mprintf_succeeds");
	CUT_ASSERT_NULL(s);
	test_allocator_reset();
	return 0;
}

CUT_BEGIN_TEST_HARNESS(mprintf_suite)
CUT_RUN_TEST(test_mprintf_alloc_succeeds);
CUT_RUN_TEST(test_mprintf_alloc_fails);
CUT_END_TEST_HARNESS

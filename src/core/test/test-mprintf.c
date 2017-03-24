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

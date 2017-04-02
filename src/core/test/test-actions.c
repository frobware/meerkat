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
#include "../actions.h"
#include "CUnitTest.h"

static int cmp_dsio_action_type(const void *p1, const void *p2)
{
	return strcmp(((struct dsio_action_type *)p1)->ident,
		      ((struct dsio_action_type *)p2)->ident);
}

static int test_actions_are_sorted(void)
{
	size_t i;
	struct dsio_action_type t[DSIO_ACTION_NR_ACTIONS];

	for (i = 0; i < DSIO_ACTION_NR_ACTIONS; i++) {
		t[i].ident = strdup(dsio_actions[i].ident);
		t[i].ident_len = dsio_actions[i].ident_len;
		t[i].descr = strdup(dsio_actions[i].descr);
		t[i].type = dsio_actions[i].type;
		CUT_ASSERT_NOT_NULL(t[i].ident);
		CUT_ASSERT_EQUAL(t[i].ident_len, strlen(dsio_actions[i].ident));
	}

	qsort(t, DSIO_ACTION_NR_ACTIONS, sizeof(t[0]), cmp_dsio_action_type);

	for (i = 0; i < DSIO_ACTION_NR_ACTIONS; i++) {
		CUT_ASSERT_EQUAL(strcmp(t[i].ident, dsio_actions[i].ident), 0);
		CUT_ASSERT_EQUAL(dsio_actions[i].ident_len, t[i].ident_len);
		CUT_ASSERT_EQUAL(strcmp(t[i].descr, dsio_actions[i].descr), 0);
		CUT_ASSERT_EQUAL(dsio_actions[i].type, t[i].type);
		free((void *)t[i].ident);
		free((void *)t[i].descr);
	}

	return 0;
}

CUT_BEGIN_TEST_HARNESS(actions_suite)
CUT_RUN_TEST(test_actions_are_sorted);
CUT_END_TEST_HARNESS

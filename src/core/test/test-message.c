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
#include "../message.h"
#include "../builder.h"
#include "allocator.c"
#include "CUnitTest.h"

static int test_empty_message(void)
{
}

static int test_parse_msg(void)
{
	int rc;
	char *msg1;

	struct dsio_msg_list msg_list;

	test_allocator_reset();

	msg1 = dsio_msg_build(test_allocator, DSIO_TOPIC_AUTH, DSIO_ACTION_ACK, "hello", "world", NULL);
	dsio_msg_list_init(test_allocator, &msg_list);
	rc = dsio_msg_parse(msg1, &msg_list);
	CUT_ASSERT_EQUAL(DSIO_OK, rc);

	return 0;
}

#if 0
static int test_topic_null_ident(void)
{
	int rc;
	struct dsio_msg msg;
	test_allocator_reset();
	rc = dsio_msg_parse(test_allocator, NULL, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_empty_ident(void)
{
	int rc;
	struct dsio_msg msg;
	test_allocator_reset();
	rc = dsio_msg_parse(test_allocator, "", &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_missing_unit_separator(void)
{
	int rc;
	struct dsio_msg msg;
	test_allocator_reset();
	rc = dsio_msg_parse(test_allocator, "E", &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_bad_topic(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'!',
		DSIO_MSG_PART_SEPARATOR,
		'\0'
	};
	test_allocator_reset();
	rc = dsio_msg_parse(test_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_incomplete_topic(void)
{
	int rc;
	struct dsio_msg msg;
	test_allocator_reset();
	rc = dsio_msg_parse(test_allocator, "E", &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_good_ident_but_no_action(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E',
		DSIO_MSG_PART_SEPARATOR,
		'\0'
	};
	test_allocator_reset();
	rc = dsio_msg_parse(test_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	CUT_ASSERT_EQUAL(strcmp("E", msg.topic->ident), 0);
	return 0;
}

static int test_topic_good_topic_bad_action(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E',
		DSIO_MSG_PART_SEPARATOR,
		'!',
		DSIO_MSG_PART_SEPARATOR,
		DSIO_MSG_SEPARATOR,
		'\0',
	};
	test_allocator_reset();
	rc = dsio_msg_parse(test_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	CUT_ASSERT_EQUAL(strcmp("E", msg.topic->ident), 0);
	return 0;
}

static int test_topic_and_action_without_record_separator(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E',
		DSIO_MSG_PART_SEPARATOR,
		'C',
		DSIO_MSG_PART_SEPARATOR,
		'\0',
	};
	test_allocator_reset();
	rc = dsio_msg_parse(test_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_and_action_and_one_data(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'C',
		DSIO_MSG_PART_SEPARATOR,
		'C',
		'H',
		'R',
		DSIO_MSG_PART_SEPARATOR,
		'1',
		DSIO_MSG_PART_SEPARATOR,
		DSIO_MSG_SEPARATOR,
		'\0',
	};
	test_allocator_reset();
	rc = dsio_msg_parse(test_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_OK, rc);
	CUT_ASSERT_NOT_NULL(msg.data);
	CUT_ASSERT_EQUAL(1, msg.ndata);
	CUT_ASSERT_EQUAL(strncmp(msg.data[0].start, "1", msg.data[0].len), 0);
	DSIO_FREE(test_allocator, msg.data);
	return 0;
}

static int test_topic_connection_and_action_challenge(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'C',
		DSIO_MSG_PART_SEPARATOR,
		'C',
		'H',
		DSIO_MSG_SEPARATOR,
		'\0',
	};
	test_allocator_reset();
	rc = dsio_msg_parse(test_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_OK, rc);
	CUT_ASSERT_NULL(msg.data);
	CUT_ASSERT_EQUAL(0, msg.ndata);
	return 0;
}

static int test_topic_and_action_and_multiple_data(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E',
		DSIO_MSG_PART_SEPARATOR,
		'C',
		'H',
		'R',
		DSIO_MSG_PART_SEPARATOR,
		'1',
		DSIO_MSG_PART_SEPARATOR,
		'2',
		DSIO_MSG_PART_SEPARATOR,
		'3',
		DSIO_MSG_PART_SEPARATOR,
		DSIO_MSG_SEPARATOR,
		'\0',
	};
	test_allocator_reset();
	rc = dsio_msg_parse(test_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_OK, rc);
	CUT_ASSERT_NOT_NULL(msg.data);
	CUT_ASSERT_EQUAL(3, msg.ndata);
	CUT_ASSERT_EQUAL(strncmp(msg.data[0].start, "1", msg.data[0].len), 0);
	CUT_ASSERT_EQUAL(strncmp(msg.data[1].start, "2", msg.data[1].len), 0);
	CUT_ASSERT_EQUAL(strncmp(msg.data[2].start, "3", msg.data[2].len), 0);
	DSIO_FREE(test_allocator, msg.data);
	return 0;
}

static int test_topic_and_action_and_multiple_data_without_record_separator(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E',
		DSIO_MSG_PART_SEPARATOR,
		'C',
		'H',
		'R',
		DSIO_MSG_PART_SEPARATOR,
		'1',
		DSIO_MSG_PART_SEPARATOR,
		'2',
		DSIO_MSG_PART_SEPARATOR,
		'3',
		DSIO_MSG_PART_SEPARATOR,
		'\0',
	};
	test_allocator_reset();
	rc = dsio_msg_parse(test_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	DSIO_FREE(test_allocator, msg.data);
	return 0;
}

static int test_payload_data_alloc_fails(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E',
		DSIO_MSG_PART_SEPARATOR,
		'C',
		'H',
		'R',
		DSIO_MSG_PART_SEPARATOR,
		'1',
		DSIO_MSG_PART_SEPARATOR,
		DSIO_MSG_SEPARATOR,
		'\0',
	};
	test_allocator_reset();
	test_allocator_realloc_fail(1);
	rc = dsio_msg_parse(test_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_NOMEM, rc);
	CUT_ASSERT_NULL(msg.data);
	CUT_ASSERT_EQUAL(0, msg.ndata);
	test_allocator_reset();
	return 0;
}

static int test_all_topics_and_actions(int alloc_failure)
{
	size_t i = 0;
	struct dsio_topic_type *t;
	struct dsio_action_type *a;

	for (t = dsio_topics; t->ident; t++, i++) {
		size_t j = 0;
		for (a = dsio_actions; a->ident; a++, j++) {
			struct dsio_msg msg;
			char *input;
			int rc;
			test_allocator_reset();
			test_allocator_malloc_fail(alloc_failure);
			test_allocator_realloc_fail(alloc_failure);
			input = make_msg(t->ident, a->ident);
			if (alloc_failure) {
				CUT_ASSERT_NULL(input);
				continue;
			}
			CUT_ASSERT_NOT_NULL(input);
			rc = dsio_msg_parse(test_allocator, input, &msg);
			CUT_ASSERT_EQUAL(DSIO_OK, rc);
			CUT_ASSERT_EQUAL(dsio_topics[i].type, msg.topic->type);
			CUT_ASSERT_EQUAL(dsio_actions[j].action, msg.action->action);
			CUT_ASSERT_EQUAL(strcmp(t->ident, msg.topic->ident), 0);
			CUT_ASSERT_EQUAL(strcmp(a->ident, msg.action->ident), 0);
			CUT_ASSERT_NULL(msg.data);
			CUT_ASSERT_EQUAL(0, msg.ndata);
			DSIO_FREE(test_allocator, input);
		}
	}

	return 0;
}

static int test_all_topics_and_actions_with_alloc_failure(void)
{
	return test_all_topics_and_actions(1);
}

static int test_all_topics_and_actions_without_alloc_failure(void)
{
	return test_all_topics_and_actions(0);
}

#if 0
static int test_multiple_messages(void)
{
	int rc;
	struct dsio_msg_list msg_list;

	char input[] = {
		'C',
		DSIO_MSG_PART_SEPARATOR,
		'C',
		'H',
		DSIO_MSG_SEPARATOR,

		'A',
		DSIO_MSG_PART_SEPARATOR,
		'A',
		DSIO_MSG_SEPARATOR,

		'\0',
	};
	test_allocator_reset();
	rc = dsio_msg_parse2(test_allocator, &msg_list, input);
	CUT_ASSERT_EQUAL(DSIO_OK, rc);
	return 0;
}
#endif


CUT_BEGIN_TEST_HARNESS(message_suite)
CUT_RUN_TEST(test_topic_null_ident);
CUT_RUN_TEST(test_topic_empty_ident);
CUT_RUN_TEST(test_topic_missing_unit_separator);
CUT_RUN_TEST(test_topic_bad_topic);
CUT_RUN_TEST(test_incomplete_topic);
CUT_RUN_TEST(test_topic_good_ident_but_no_action);
CUT_RUN_TEST(test_topic_good_topic_bad_action);
CUT_RUN_TEST(test_topic_and_action_and_one_data);
CUT_RUN_TEST(test_topic_and_action_and_multiple_data);
CUT_RUN_TEST(test_topic_and_action_and_multiple_data_without_record_separator);
CUT_RUN_TEST(test_topic_connection_and_action_challenge);
CUT_RUN_TEST(test_payload_data_alloc_fails);
CUT_RUN_TEST(test_topic_and_action_without_record_separator);
CUT_RUN_TEST(test_all_topics_and_actions_with_alloc_failure);
CUT_RUN_TEST(test_all_topics_and_actions_without_alloc_failure);
#endif

CUT_BEGIN_TEST_HARNESS(message_suite)
CUT_RUN_TEST(test_parse_msg);
/* CUT_RUN_TEST(test_multiple_messages); */
CUT_END_TEST_HARNESS

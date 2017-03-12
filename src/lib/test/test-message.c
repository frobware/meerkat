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
#include "../message.h"
#include "../mprintf.h"
#include "allocator.c"
#include "CUnitTest.h"

static struct test_allocator *ta = &_test_allocator;

static char *make_msg(const char *topic, const char *action)
{
	return dsio_mprintf(dsio_stdlib_allocator,
			    "%s%c%s%c%c",
			    topic,
			    DSIO_MSG_PART_SEPARATOR,
			    action,
			    DSIO_MSG_PART_SEPARATOR,
			    DSIO_MSG_RECORD_SEPARATOR);
}

static int test_topic_null_ident(void)
{
	int rc;
	struct dsio_msg msg;
	rc = dsio_msg_parse(dsio_stdlib_allocator, NULL, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_empty_ident(void)
{
	int rc;
	struct dsio_msg msg;
	rc = dsio_msg_parse(dsio_stdlib_allocator, "", &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_missing_unit_separator(void)
{
	int rc;
	struct dsio_msg msg;
	rc = dsio_msg_parse(dsio_stdlib_allocator, "E", &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_bad_topic(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'!', DSIO_MSG_PART_SEPARATOR,
		'\0'
	};
	rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_good_ident_but_missing_unit_separator(void)
{
	int rc;
	struct dsio_msg msg;
	rc = dsio_msg_parse(dsio_stdlib_allocator, "E", &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_good_ident_but_no_action(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E', DSIO_MSG_PART_SEPARATOR,
		'\0'
	};
	rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	CUT_ASSERT_EQUAL(strcmp("E", msg.topic->ident), 0);
	return 0;
}

static int test_topic_good_action_but_no_unit_separator(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E', DSIO_MSG_PART_SEPARATOR,
		'C', DSIO_MSG_RECORD_SEPARATOR,
		'\0',
	};
	rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	CUT_ASSERT_EQUAL(strcmp("E", msg.topic->ident), 0);
	return 0;
}

static int test_topic_good_topic_bad_action(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E', DSIO_MSG_PART_SEPARATOR,
		'!', DSIO_MSG_PART_SEPARATOR,
		DSIO_MSG_RECORD_SEPARATOR,
		'\0',
	};
	rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	CUT_ASSERT_EQUAL(strcmp("E", msg.topic->ident), 0);
	return 0;
}

static int test_topic_and_action_without_record_separator(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E', DSIO_MSG_PART_SEPARATOR,
		'C', DSIO_MSG_PART_SEPARATOR,
		'\0',
	};
	rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_and_action_and_one_data(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E', DSIO_MSG_PART_SEPARATOR,
		'C', 'H', 'R', DSIO_MSG_PART_SEPARATOR,
		'1', DSIO_MSG_PART_SEPARATOR,
		DSIO_MSG_RECORD_SEPARATOR,
		'\0',
	};
	rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_OK, rc);
	CUT_ASSERT_NOT_NULL(msg.data);
	CUT_ASSERT_EQUAL(1, msg.ndata);
	CUT_ASSERT_EQUAL(strcmp(msg.data[0], "1"), 0);
	return 0;
}

static int test_topic_and_action_and_multiple_data(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E', DSIO_MSG_PART_SEPARATOR,
		'C', 'H', 'R', DSIO_MSG_PART_SEPARATOR,
		'1', DSIO_MSG_PART_SEPARATOR,
		'2', DSIO_MSG_PART_SEPARATOR,
		'3', DSIO_MSG_PART_SEPARATOR,
		DSIO_MSG_RECORD_SEPARATOR,
		'\0',
	};
	rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_OK, rc);
	CUT_ASSERT_NOT_NULL(msg.data);
	CUT_ASSERT_EQUAL(3, msg.ndata);
	CUT_ASSERT_EQUAL(strcmp(msg.data[0], "1"), 0);
	CUT_ASSERT_EQUAL(strcmp(msg.data[1], "2"), 0);
	CUT_ASSERT_EQUAL(strcmp(msg.data[2], "3"), 0);
	return 0;
}

static int test_payload_data_alloc_fails(void)
{
	int rc;
	struct dsio_msg msg;
	char input[] = {
		'E', DSIO_MSG_PART_SEPARATOR,
		'C', 'H', 'R', DSIO_MSG_PART_SEPARATOR,
		'1', DSIO_MSG_PART_SEPARATOR,
		DSIO_MSG_RECORD_SEPARATOR,
		'\0',
	};
	test_allocator_reset(ta);
	ta->alloc_fail_now = 1;
	ta->realloc_fail_now = 1;
	rc = dsio_msg_parse(&ta->base, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_NOMEM, rc);
	CUT_ASSERT_NULL(msg.data);
	CUT_ASSERT_EQUAL(0, msg.ndata);
	return 0;
}

static int test_all_topics_and_actions(void)
{
	size_t i = 0;
	for (struct dsio_topic_type *t = topics; t->ident; t++, i++) {
		size_t j = 0;
		for (struct dsio_action_type *a = actions; a->ident; a++, j++) {
			struct dsio_msg msg;
			char *input = make_msg(t->ident, a->ident);
			CUT_ASSERT_NOT_NULL(input);
			int rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
			CUT_ASSERT_EQUAL(DSIO_OK, rc);
			CUT_ASSERT_EQUAL(topics[i].type, msg.topic->type);
			CUT_ASSERT_EQUAL(actions[j].type, msg.action->type);
			CUT_ASSERT_EQUAL(strcmp(t->ident, msg.topic->ident), 0);
			CUT_ASSERT_EQUAL(strcmp(a->ident, msg.action->ident), 0);
			CUT_ASSERT_NULL(msg.data);
			CUT_ASSERT_EQUAL(0, msg.ndata);
			DSIO_FREE(dsio_stdlib_allocator, input);
		}
	}

	return 0;
}

CUT_BEGIN_TEST_HARNESS(message_suite)
CUT_RUN_TEST(test_topic_null_ident);
CUT_RUN_TEST(test_topic_empty_ident);
CUT_RUN_TEST(test_topic_missing_unit_separator);
CUT_RUN_TEST(test_topic_bad_topic);
CUT_RUN_TEST(test_topic_good_ident_but_missing_unit_separator);
CUT_RUN_TEST(test_topic_good_ident_but_no_action);
CUT_RUN_TEST(test_topic_good_action_but_no_unit_separator);
CUT_RUN_TEST(test_topic_good_topic_bad_action);
CUT_RUN_TEST(test_topic_and_action_and_one_data);
CUT_RUN_TEST(test_topic_and_action_and_multiple_data);
CUT_RUN_TEST(test_payload_data_alloc_fails);
CUT_RUN_TEST(test_topic_and_action_without_record_separator);
CUT_RUN_TEST(test_all_topics_and_actions);
CUT_END_TEST_HARNESS

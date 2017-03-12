#include <stdio.h>
#include <string.h>
#include <dsio/dsio.h>
#include "../message.h"
#include "../mprintf.h"

#include "CUnitTest.h"

static char *make_msg(const char *topic, const char *action)
{
	return dsio_mprintf(dsio_stdlib_allocator,
			    "%s%c%s%c%c",
			    topic,
			    DSIO_MSG_UNIT_SEPARATOR,
			    action,
			    DSIO_MSG_UNIT_SEPARATOR,
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
	const char input[] = {
		'!', DSIO_MSG_UNIT_SEPARATOR,
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
	const char input[] = {
		'E', DSIO_MSG_UNIT_SEPARATOR,
		'\0'
	};
	rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	CUT_ASSERT_EQUAL(1, msg.topic.len);
	CUT_ASSERT_EQUAL(strcmp("E", msg.topic.ident), 0);
	return 0;
}

static int test_topic_good_action_but_no_unit_separator(void)
{
	int rc;
	struct dsio_msg msg;
	const char input[] = {
		'E', DSIO_MSG_UNIT_SEPARATOR,
		'C', DSIO_MSG_RECORD_SEPARATOR,
		'\0',
	};
	rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	CUT_ASSERT_EQUAL(1, msg.topic.len);
	CUT_ASSERT_EQUAL(strcmp("E", msg.topic.ident), 0);
	return 0;
}

static int test_topic_good_topic_bad_action(void)
{
	int rc;
	struct dsio_msg msg;
	const char input[] = {
		'E', DSIO_MSG_UNIT_SEPARATOR,
		'!', DSIO_MSG_UNIT_SEPARATOR,
		DSIO_MSG_RECORD_SEPARATOR,
		'\0',
	};
	rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	CUT_ASSERT_EQUAL(1, msg.topic.len);
	CUT_ASSERT_EQUAL(strcmp("E", msg.topic.ident), 0);
	return 0;
}

static int test_topic_and_action_without_record_separator(void)
{
	int rc;
	struct dsio_msg msg;
	const char input[] = {
		'E', DSIO_MSG_UNIT_SEPARATOR,
		'C', DSIO_MSG_UNIT_SEPARATOR,
		'\0',
	};
	rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_and_action_and_one_record(void)
{
	int rc;
	struct dsio_msg msg;
	const char input[] = {
		'E', DSIO_MSG_UNIT_SEPARATOR,
		'C', DSIO_MSG_UNIT_SEPARATOR,
		'1', DSIO_MSG_UNIT_SEPARATOR,
		DSIO_MSG_RECORD_SEPARATOR,
		'\0',
	};
	rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_all_topics_and_actions(void)
{
	for (struct topic *t = topics; t->ident; t++) {
		for (struct action *a = actions; a->ident; a++) {
			struct dsio_msg msg;
			char *input = make_msg(t->ident, a->ident);
			CUT_ASSERT_NOT_NULL(input);
			int rc = dsio_msg_parse(dsio_stdlib_allocator, input, &msg);
			CUT_ASSERT_EQUAL(DSIO_OK, rc);
			CUT_ASSERT_EQUAL(strlen(t->ident), msg.topic.len);
			CUT_ASSERT_EQUAL(strcmp(t->ident, msg.topic.ident), 0);
			CUT_ASSERT_EQUAL(strlen(a->ident), msg.action.len);
			CUT_ASSERT_EQUAL(strcmp(a->ident, msg.action.ident), 0);
			CUT_ASSERT_EQUAL(0, msg.npayload);
			CUT_ASSERT_EQUAL(NULL, msg.payload);
			dsio_stdlib_allocator->free(dsio_stdlib_allocator, input);
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
CUT_RUN_TEST(test_topic_and_action_and_one_record);
CUT_RUN_TEST(test_topic_and_action_without_record_separator);
CUT_RUN_TEST(test_all_topics_and_actions);
CUT_END_TEST_HARNESS

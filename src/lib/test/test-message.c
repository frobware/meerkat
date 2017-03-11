#include <string.h>
#include <dsio/dsio.h>
#include "../message.h"

#include "CUnitTest.h"

static int test_topic_null_ident(void)
{
	int rc;
	struct dsio_message msg;
	rc = dsio_message_parse(dsio_stdlib_allocator, NULL, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_empty_ident(void)
{
	int rc;
	struct dsio_message msg;
	rc = dsio_message_parse(dsio_stdlib_allocator, "", &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_bad_ident(void)
{
	int rc;
	struct dsio_message msg;
	rc = dsio_message_parse(dsio_stdlib_allocator, "!", &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_good_ident_but_missing_unit_separator(void)
{
	int rc;
	struct dsio_message msg;
	rc = dsio_message_parse(dsio_stdlib_allocator, "E", &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	return 0;
}

static int test_topic_good_ident_but_no_action(void)
{
	int rc;
	struct dsio_message msg;
	const char input[] = { 
		'E', DSIO_MESSAGE_UNIT_SEPARATOR, '\0' 
	};
	rc = dsio_message_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	CUT_ASSERT_EQUAL(msg.topic.len, 1);
	CUT_ASSERT_EQUAL(strcmp("E", msg.topic.ident), 0);
	return 0;
}

static int test_topic_good_action_but_no_unit_separator(void)
{
	int rc;
	struct dsio_message msg;
	const char input[] = {
		'E', DSIO_MESSAGE_UNIT_SEPARATOR,
		'C', DSIO_MESSAGE_RECORD_SEPARATOR,
		'\0',
	};
	rc = dsio_message_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_ERROR, rc);
	CUT_ASSERT_EQUAL(msg.topic.len, 1);
	CUT_ASSERT_EQUAL(strcmp("E", msg.topic.ident), 0);
	return 0;
}

static int test_topic_good_topic_and_good_action(void)
{
	int rc;
	struct dsio_message msg;
	const char input[] = {
		'E', DSIO_MESSAGE_UNIT_SEPARATOR,
		'C', DSIO_MESSAGE_UNIT_SEPARATOR,
		'\0',
	};
	rc = dsio_message_parse(dsio_stdlib_allocator, input, &msg);
	CUT_ASSERT_EQUAL(DSIO_OK, rc);
	CUT_ASSERT_EQUAL(msg.topic.len, 1);
	CUT_ASSERT_EQUAL(strcmp("E", msg.topic.ident), 0);
	CUT_ASSERT_EQUAL(msg.action.len, 1);
	CUT_ASSERT_EQUAL(strcmp("C", msg.action.ident), 0);
	CUT_ASSERT_EQUAL(msg.npayload, 0);
	CUT_ASSERT_EQUAL(msg.payload, NULL);
	return 0;
}

static int test_all_topics_and_actions(void)
{
	struct topic *topic;
	
	for (topic = topics; topic->ident; topic++) {
		if (strcmp(topic->ident, "private") == 0)
			continue;
		struct action *action;
		for (action = actions; action->ident; action++) {
			fprintf(stdout, "%s %s\n", topic->ident, action->ident);
		}
	}

	return 0;
}


CUT_BEGIN_TEST_HARNESS(message_suite)
CUT_RUN_TEST(test_topic_null_ident);
CUT_RUN_TEST(test_topic_empty_ident);
CUT_RUN_TEST(test_topic_bad_ident);
CUT_RUN_TEST(test_topic_good_ident_but_missing_unit_separator);
CUT_RUN_TEST(test_topic_good_ident_but_no_action);
CUT_RUN_TEST(test_topic_good_action_but_no_unit_separator);
CUT_RUN_TEST(test_topic_good_topic_and_good_action);
CUT_RUN_TEST(test_all_topics_and_actions);
CUT_END_TEST_HARNESS

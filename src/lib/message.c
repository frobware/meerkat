/*
 *
 * Message Structure Overview
 * ==========================
 *
 * deepstream messages are transmitted using a proprietary, minimal,
 * string-based protocol. Every message follows the same structure:
 *
 * <topic>|<action>|<data[0]>|...|<data[n]>+
 *
 * | and + are used above as placeholders, but messages are actually
 * separated by ASCII control characters ("unit separator" 31) and
 * "record separator" 30).
 *
 * Every message has a topic (e.g., RECORD, EVENT, AUTH) and an action
 * (e.g., CREATE, DELETE, SUBSCRIBE).
 *
 * TOPICS
 *
 * | name       | value    | server | client |
 * |------------+----------+--------+--------|
 * | CONNECTION | C        | YES    | YES    |
 * | AUTH       | A        | YES    | YES    |
 * | ERROR      | X        | YES    | YES    |
 * | EVENT      | E        | YES    | YES    |
 * | RECORD     | R        | YES    | YES    |
 * | RPC        | P        | YES    | YES    |
 * | PRIVATE    | PRIVATE/ | YES    | YES    |
 *
 * ACTIONS
 *
 * Messages always start with TOPIC, then ACTION, but can contain zero
 * or more data fields.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <dsio/dsio.h>
#include "message.h"

/* This table _MUST_ remain sorted on ident. It is used by bsearch. */

struct dsio_topic_type topics[] = {
	{"A", "AUTH"},
	{"C", "CONNECTION"},
	{"E", "ERROR"},
	{"P", "RPC"},
	{"R", "RECORD"},
	{"X", "ERROR"},
	{"private", "PRIVATE/"},
	{NULL, NULL},
};

/* This table _MUST_ remain sorted on ident. It is used by bsearch. */

struct dsio_action_type actions[] = {
	{"A", "ACK"},
	{"C", "CREATE"},
	{"CH", "CHALLENGE"},
	{"CHR", "CHALLENGE_RESPONSE"},
	{"CR", "CREATEORREAD"},
	{"D", "DELETE"},
	{"E", "ERROR"},
	{"EVT", "EVENT"},
	{"H", "HAS"},
	{"L", "LISTEN"},
	{"LA", "LISTEN_ACCEPT"},
	{"LR", "LISTEN_REJECT"},
	{"P", "PATCH"},
	{"PU", "PROVIDER_UPDATE"},
	{"Q", "QUERY"},
	{"R", "READ"},
	{"RED", "REDIRECT"},
	{"REJ", "REJECTION"},
	{"REQ", "REQUEST"},
	{"RES", "RESPONSE"},
	{"S", "SUBSCRIBE"},
	{"SH", "SUBSCRIPTION_HAS_PROVIDER"},
	{"SN", "SNAPSHOT"},
	{"U", "UPDATE"},
	{"UL", "UNLISTEN"},
	{"US", "UNSUBSCRIBE"},
	{NULL, NULL},
};

struct scanner {
	char *input;
	char *curr;
	struct dsio_msg *msg;
	const struct dsio_allocator *allocator;
};

static int topic_bsearch_comparator(const void *a, const void *b)
{
	return strcmp(((const struct dsio_topic_type *)a)->ident,
		      ((const struct dsio_topic_type *)b)->ident);
}

static int is_valid_topic_p(const char *ident)
{
	struct dsio_topic_type key = {.ident = ident };

	return bsearch(&key, topics, DSIO_NELEMENTS(topics) - 1,
		       sizeof key,
		       topic_bsearch_comparator) != NULL;
}

static int action_bsearch_comparator(const void *a, const void *b)
{
	return strcmp(((const struct dsio_action_type *)a)->ident,
		      ((const struct dsio_action_type *)b)->ident);
}

static int is_valid_action_p(const char *ident)
{
	struct dsio_action_type key = {.ident = ident };

	return bsearch(&key, actions, DSIO_NELEMENTS(actions) - 1,
		       sizeof key,
		       action_bsearch_comparator) != NULL;
}

static int parse_topic(struct scanner *s)
{
	char *found = strchr(s->curr, DSIO_MSG_PART_SEPARATOR);

	if (!found) {
		return DSIO_ERROR;
	}

	s->msg->topic.ident = s->curr;
	s->msg->topic.len = found - s->curr;
	*found = '\0';		/* punch hole in input */

	if (!is_valid_topic_p(s->msg->topic.ident)) {
		return DSIO_ERROR;
	}

	s->curr = ++found;

	return DSIO_OK;
}

static int parse_action(struct scanner *s)
{
	char *found = strchr(s->curr, DSIO_MSG_PART_SEPARATOR);

	if (!found) {
		return DSIO_ERROR;
	}

	s->msg->action.ident = s->curr;
	s->msg->action.len = found - s->curr;
	*found = '\0';		/* punch hole in input */

	if (!is_valid_action_p(s->msg->action.ident)) {
		return DSIO_ERROR;
	}

	s->curr = ++found;

	return DSIO_OK;
}

static int parse_payload(struct scanner *s)
{
	char *mark = s->curr;
  
	for (; *s->curr; s->curr++) {
		switch (*s->curr) {
		case DSIO_MSG_RECORD_SEPARATOR:
			*s->curr++ = '\0';
			return DSIO_OK;
		case DSIO_MSG_PART_SEPARATOR:
			*s->curr++ = '\0';
			fprintf(stdout, "<<<%.*s>>>\n", (int)(s->curr - mark), mark);
			mark = s->curr;
			break;
		}
	}

	return DSIO_ERROR;
}

int dsio_msg_parse(const struct dsio_allocator *a, char *const input, struct dsio_msg *msg)
{
	int rc;
	struct scanner s;

	memset(msg, 0, sizeof *msg);

	if (input == NULL || *input == '\0')
		return DSIO_ERROR;

	memset(&s, 0, sizeof s);
	s.allocator = a;
	s.curr = s.input = input;
	s.msg = msg;

	if ((rc = parse_topic(&s)) != DSIO_OK)
		return rc;

	if ((rc = parse_action(&s)) != DSIO_OK)
		return rc;

	if ((rc = parse_payload(&s)) != DSIO_OK)
		return rc;

	return DSIO_OK;
}

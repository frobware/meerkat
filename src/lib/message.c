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

struct topic topics[] = {
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

struct action actions[] = {
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

struct parser {
	const char *input;
	const char *curr;
	struct dsio_msg *msg;
	const struct dsio_allocator *allocator;
};

static int topic_bsearch_comparator(const void *a, const void *b)
{
	return strcmp(((const struct topic *)a)->ident, ((const struct topic *)b)->ident);
}

static int is_valid_topic_p(const char *ident)
{
	struct topic key = {.ident = ident };

	return bsearch(&key, topics, DSIO_NELEMENTS(topics) - 1, sizeof key, topic_bsearch_comparator) != NULL;
}

static int action_bsearch_comparator(const void *a, const void *b)
{
	return strcmp(((const struct action *)a)->ident, ((const struct action *)b)->ident);
}

static int is_valid_action_p(const char *ident)
{
	struct action key = {.ident = ident };

	return bsearch(&key, actions, DSIO_NELEMENTS(actions) - 1, sizeof key, action_bsearch_comparator) != NULL;
}

static int parse_topic(struct parser *p)
{
	char *found = strchr(p->curr, DSIO_MSG_UNIT_SEPARATOR);

	if (found == NULL) {
		return DSIO_ERROR;
	}

	p->msg->topic.ident = p->curr;
	p->msg->topic.len = found - p->curr;
	*found = '\0';		/* punch hole in input */

	if (!is_valid_topic_p(p->msg->topic.ident)) {
		return DSIO_ERROR;
	}

	p->curr = ++found;

	return DSIO_OK;
}

static int parse_action(struct parser *p)
{
	char *found = strchr(p->curr, DSIO_MSG_UNIT_SEPARATOR);

	if (found == NULL) {
		return DSIO_ERROR;
	}

	p->msg->action.ident = p->curr;
	p->msg->action.len = found - p->curr;
	*found = '\0';		/* punch hole in input */

	if (!is_valid_action_p(p->msg->action.ident)) {
		return DSIO_ERROR;
	}

	p->curr = ++found;

	return DSIO_OK;
}

static int parse_payload(struct parser *p)
{
	for (;;) {
		switch (*p->curr) {
		case '\0':
			return DSIO_ERROR;
		case DSIO_MSG_RECORD_SEPARATOR:
			p->curr++;
			return DSIO_OK;
		case DSIO_MSG_UNIT_SEPARATOR:
			p->curr++;
			break;
		}
		p->curr++;
	}

	return DSIO_ERROR;
}

int dsio_msg_parse(const struct dsio_allocator *a, const char *input, struct dsio_msg *msg)
{
	int rc;
	struct parser p;

	memset(msg, 0, sizeof *msg);

	if (input == NULL || *input == '\0')
		return DSIO_ERROR;

	memset(&p, 0, sizeof p);
	p.allocator = a;
	p.curr = p.input = input;
	p.msg = msg;

	if ((rc = parse_topic(&p)) != DSIO_OK)
		return rc;

	if ((rc = parse_action(&p)) != DSIO_OK)
		return rc;

	if ((rc = parse_payload(&p)) != DSIO_OK)
		return rc;

	return DSIO_OK;
}

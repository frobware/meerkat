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

#include <string.h>
#include <stdlib.h>

#include <dsio/dsio.h>
#include "message.h"

struct topic topics[] = {
	{"C", "CONNECTION"},
	{"A", "AUTH"},
	{"X", "ERROR"},
	{"E", "ERROR"},
	{"R", "RECORD"},
	{"P", "RPC"},
	{"private", "PRIVATE/"},
	{NULL},
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
	{NULL},
};

struct parser {
	const char *input;
	const char *curr;
	struct dsio_message *message;
	const struct dsio_allocator *allocator;
};

static int is_valid_topic_p(const char *ident)
{
	for (size_t i = 0; i < DSIO_NELEMENTS(topics) - 1; i++) {
		if (strcmp(ident, topics[i].ident) == 0) {
			return 1;
		}
	}

	return 0;
}

static int is_valid_action_p(const char *ident)
{
	for (size_t i = 0; i < DSIO_NELEMENTS(actions) - 1; i++) {
		if (strcmp(ident, actions[i].ident) == 0) {
			return 1;
		}
	}

	return 0;
}

#if 0
static int action_bsearch_comparator(const void *a, const void *b)
{
	const struct dsio_message *x = a;
	const struct action *y = b;
	return strncmp(x->action.ident, y->ident, x->action.len);
}
#endif

static int parse_topic(struct parser *p)
{
	char *found = strchr(p->curr, DSIO_MESSAGE_UNIT_SEPARATOR);

	if (found == NULL) {
		return DSIO_ERROR;
	}

	p->message->topic.ident = p->curr;
	p->message->topic.len = found - p->curr;
	*found = '\0';		/* punch hole in input */

	if (!is_valid_topic_p(p->message->topic.ident)) {
		return DSIO_ERROR;
	}

	p->curr = ++found;

	return DSIO_OK;
}

static int parse_action(struct parser *p)
{
	char *found = strchr(p->curr, DSIO_MESSAGE_UNIT_SEPARATOR);

	if (found == NULL) {
		return DSIO_ERROR;
	}

	p->message->action.ident = p->curr;
	p->message->action.len = found - p->curr;
	*found = '\0';		/* punch hole in input */

	if (!is_valid_action_p(p->message->action.ident)) {
		return DSIO_ERROR;
	}

	p->curr = ++found;

	return DSIO_OK;
}

static int parse_payload(struct parser *p)
{
	return DSIO_OK;
}

int dsio_message_parse(const struct dsio_allocator *a,
		       const char *input,
		       struct dsio_message *msg)
{
	int rc;
	struct parser p;

	memset(msg, 0, sizeof *msg);

	if (input == NULL || *input == '\0')
		return DSIO_ERROR;

	memset(&p, 0, sizeof p);
	p.allocator = a;
	p.curr = p.input = input;
	p.message = msg;

	if ((rc = parse_topic(&p)) != DSIO_OK)
		return rc;

	if ((rc = parse_action(&p)) != DSIO_OK)
		return rc;

	if ((rc = parse_payload(&p)) != DSIO_OK)
		return rc;

	return DSIO_OK;
}

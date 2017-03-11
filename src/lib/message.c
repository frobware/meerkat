/*

Message Structure Overview
==========================

deepstream messages are transmitted using a proprietary, minimal,
string-based protocol. Every message follows the same structure:

<topic>|<action>|<data[0]>|...|<data[n]>+

| and + are used above as placeholders, but messages are actually
separated by ASCII control characters ("unit separator" 31) and
"record separator" 30).

Every message has a topic (e.g., RECORD, EVENT, AUTH) and an action
(e.g., CREATE, DELETE, SUBSCRIBE).

TOPICS

| name       | value    | server | client |
|------------+----------+--------+--------|
| CONNECTION | C        | YES    | YES    |
| AUTH       | A        | YES    | YES    |
| ERROR      | X        | YES    | YES    |
| EVENT      | E        | YES    | YES    |
| RECORD     | R        | YES    | YES    |
| RPC        | P        | YES    | YES    |
| PRIVATE    | PRIVATE/ | YES    | YES    |

ACTIONS

Messages always start with TOPIC, then ACTION, but can contain zero or
more data fields.

*/

#include <string.h>
#include <stdlib.h>

#include <dsio/dsio.h>
#include "message.h"

struct topic {
	const char *ident;
	const char *descr;
};

struct action {
	const char *ident;
	const char *descr;
};

static struct topic topics[] = {
	{"C",		"CONNECTION"},
	{"A",		"AUTH"},
	{"X",		"ERROR"},
	{"E",		"ERROR"},
	{"R",		"RECORD"},
	{"P",		"RPC"},
	{"private",	"PRIVATE/"},
};

/* This table _MUST_ remain sorted on ident. It is used by bsearch. */

static struct action actions[] = {
        {"A",	"ACK"},
        {"C",	"CREATE"},
        {"CH",	"CHALLENGE"},
        {"CHR",	"CHALLENGE_RESPONSE"},
        {"CR",	"CREATEORREAD"},
        {"D",	"DELETE"},
        {"E",	"ERROR"},
        {"EVT",	"EVENT"},
        {"H",	"HAS"},
        {"L",	"LISTEN"},
        {"LA",	"LISTEN_ACCEPT"},
        {"LR",	"LISTEN_REJECT"},
        {"P",	"PATCH"},
        {"PU",	"PROVIDER_UPDATE"},
        {"Q",	"QUERY"},
        {"R",	"READ"},
        {"RED",	"REDIRECT"},
        {"REJ",	"REJECTION"},
        {"REQ",	"REQUEST"},
        {"RES",	"RESPONSE"},
        {"S",	"SUBSCRIBE"},
        {"SH",	"SUBSCRIPTION_HAS_PROVIDER"},
        {"SN",	"SNAPSHOT"},
        {"U",	"UPDATE"},
        {"UL",	"UNLISTEN"},
        {"US",	"UNSUBSCRIBE"},
};

struct scanner {
	const char *input;
	const char *curr;
	struct dsio_message *message;
	const struct dsio_allocator *allocator;
};

static int is_valid_topic_p(const char *ident, size_t len)
{
	for (size_t i = 0; i < DSIO_NELEMENTS(topics); i++) {
		if (strncmp(ident, topics[i].ident, len) == 0) {
			return 1;
		}
	}

	return 0;
}

static int is_valid_action_p(const char *ident, size_t len)
{
	for (size_t i = 0; i < DSIO_NELEMENTS(actions); i++) {
		if (strncmp(ident, actions[i].ident, len) == 0) {
			return 1;
		}
	}

	return 0;
}

static int action_bsearch_comparator(const void *a, const void *b)
{
	const struct dsio_message *x = a;
	const struct action *y = b;
	return strncmp(x->action.ident, y->ident, x->action.len); 
}

static int parse_topic(struct scanner *s)
{
	char *found = strchr(s->curr, DSIO_MESSAGE_UNIT_SEPARATOR);

	if (found == NULL) {
		return DSIO_ERROR;
	}

	s->message->topic.ident = s->curr;
	s->message->topic.len = found - s->curr;
	*found = '\0';
	
	if (!is_valid_topic_p(s->message->topic.ident, s->message->topic.len)) {
		return DSIO_ERROR;
	}

	s->curr = ++found;

	return DSIO_OK;
}

static int parse_action(struct scanner *s)
{
	char *found = strchr(s->curr, DSIO_MESSAGE_UNIT_SEPARATOR);

	if (found == NULL) {
		return DSIO_ERROR;
	}

	s->message->action.ident = s->curr;
	s->message->action.len = found - s->curr;
	*found = '\0';

	if (!is_valid_action_p(s->message->action.ident, s->message->action.len)) {
		return DSIO_ERROR;
	}

	s->curr = ++found;

	return DSIO_OK;
}

static int parse_payload(struct scanner *s)
{
	return DSIO_OK;
}

int dsio_message_parse(const struct dsio_allocator *a, const char *input, struct dsio_message *msg)
{
	int rc;
	struct scanner s;

	memset(msg, 0, sizeof *msg);
	
	if (input == NULL || *input == '\0')
		return DSIO_ERROR;
	
	memset(&s, 0, sizeof s);
	s.allocator = a;
	s.curr = s.input = input;
	s.message = msg;

	if ((rc = parse_topic(&s)) != DSIO_OK)
		return rc;

	if ((rc = parse_action(&s)) != DSIO_OK)
		return rc;

	if ((rc = parse_payload(&s)) != DSIO_OK)
		return rc;

	return DSIO_OK;
}

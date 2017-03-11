#include <string.h>
#include <stdlib.h>

#include <dsio/dsio.h>
#include "message.h"

struct message_topic {
	const char *ident;
	const char *descr;
};

struct message_action {
	const char *ident;
	const char *descr;
};

static struct message_topic message_topics[] = {
	{"C",		"CONNECTION"},
	{"A",		"AUTH"},
	{"X",		"ERROR"},
	{"E",		"ERROR"},
	{"R",		"RECORD"},
	{"P",		"RPC"},
	{"private",	"PRIVATE/"},
};

/* This table _MUST_ remain sorted on ident. It is used by bsearch. */

static struct message_action message_actions[] = {
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
	for (size_t i = 0; i < DSIO_NELEMENTS(message_topics); i++) {
		if (strncmp(ident, message_topics[i].ident, len) == 0) {
			return 1;
		}
	}

	return 0;
}

static int message_action_bsearch_comparator(const void *a, const void *b)
{
	const struct dsio_message *x = a;
	const struct message_action *y = b;
	return strncmp(x->action.ident, y->ident, x->action.len); 
}

static int is_valid_action_p(const char *ident, size_t len)
{
	for (size_t i = 0; i < DSIO_NELEMENTS(message_actions); i++) {
		if (strncmp(ident, message_actions[i].ident, len) == 0) {
			return 1;
		}
	}

	return 0;
}

static int parse_topic(struct scanner *s)
{
	const char *found = strchr(s->curr, DSIO_MESSAGE_UNIT_SEPARATOR);

	if (found == NULL) {
		return DSIO_ERROR;
	}

	s->message->topic.ident = s->curr;
	s->message->topic.len = found - s->curr;

	if (!is_valid_topic_p(s->message->topic.ident, s->message->topic.len)) {
		return DSIO_ERROR;
	}

	s->curr = ++found;

	return DSIO_OK;
}

static int parse_action(struct scanner *s)
{
	const char *found = strchr(s->curr, DSIO_MESSAGE_UNIT_SEPARATOR);

	if (found == NULL) {
		return DSIO_ERROR;
	}

	s->message->action.ident = s->curr;
	s->message->action.len = found - s->curr;

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

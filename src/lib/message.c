#include <string.h>
#include <stdlib.h>

#include <dsio/dsio.h>
#include "message.h"

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

#include <string.h>
#include <dsio/dsio.h>
#include "message.h"

#define SCANNER_ERR 1

enum scanner_state {
	SCANNER_TOPIC,
	SCANNER_ACTION,
	SCANNER_DATA
};

struct scanner {
	const char *input;
	const char *next;
	enum scanner_state state;
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

static void scanner_init(struct scanner *s, const struct dsio_allocator *a, const char *input)
{
	memset(s, 0, sizeof *s);
	s->allocator = a;
	s->state = SCANNER_TOPIC;
	s->next = s->input = input;
}

static int scanner_next(struct scanner *s, const struct dsio_allocator *a, const char *input)
{
	return 0;
}

int message_parse(const struct dsio_allocator *a, const char *input)
{
	struct scanner s;
	scanner_init(&s, a, input);
	
	return DSIO_OK;
}

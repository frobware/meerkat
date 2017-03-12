#pragma once

#include <dsio/allocator.h>

#define DSIO_MSG_UNIT_SEPARATOR	0x1f
#define DSIO_MSG_RECORD_SEPARATOR	0x1e

typedef enum dsio_connection_state {
	CONNECTION_STATE_CLOSED,
	CONNECTION_STATE_AWAITING_CONNECTION,
	CONNECTION_STATE_CHALLENGING,
	CONNECTION_STATE_AWAITING_AUTHENTICATION,
	CONNECTION_STATE_AUTHENTICATING,
	CONNECTION_STATE_OPEN,
	CONNECTION_STATE_ERROR,
	CONNECTION_STATE_RECONNECTING
} dsio_connection_state_t;

struct topic {
	const char *ident;
	const char *descr;
};

struct action {
	const char *ident;
	const char *descr;
};

struct dsio_topic {
	const char *ident;
	size_t len;
};

struct dsio_action {
	const char *ident;
	size_t len;
};

struct dsio_payload {
	const char *data;
	size_t len;
};

struct dsio_msg {
	struct dsio_topic topic;
	struct dsio_action action;
	struct dsio_payload *payload;
	size_t npayload;
};

extern struct topic topics[];
extern struct action actions[];

extern int dsio_msg_parse(const struct dsio_allocator *a, const char *input,
			      struct dsio_msg *msg);

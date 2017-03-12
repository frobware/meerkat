#pragma once

/* Status codes. */

#define DSIO_OK		0
#define DSIO_ERROR	1

#define DSIO_NELEMENTS(A) ((sizeof((A)) / sizeof((A))[0]))

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

struct dsio_topic_type {
	const char *ident;
	const char *descr;
};

struct dsio_action_type {
	const char *ident;
	const char *descr;
};


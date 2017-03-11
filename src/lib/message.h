#pragma once

#include <dsio/allocator.h>

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

struct dsio_message {
	struct dsio_topic topic;
	struct dsio_action action;
	struct dsio_payload *payload;
	size_t npayload;
};

extern int dsio_message_parse(const struct dsio_allocator *a, const char *input,
			      struct dsio_message *msg);

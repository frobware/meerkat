/*
 * Copyright 2017 Andrew McDermott
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <dsio/dsio.h>
#include "message.h"

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

/* This table _MUST_ remain sorted on ident. It is used by bsearch. */

struct dsio_topic_type topics[] = {
	{"A",	"AUTH",		TOPIC_AUTH},
	{"C",	"CONNECTION",	TOPIC_CONNECTION},
	{"E",	"EVENT",	TOPIC_EVENT},
	{"P",	"RPC",		TOPIC_RPC},
	{"R",	"RECORD",	TOPIC_RECORD},
	{"X",	"ERROR",	TOPIC_ERROR},
	{NULL, NULL},
};

/* This table _MUST_ remain sorted on ident. It is used by bsearch. */

struct dsio_action_type actions[] = {
	{"A",	"ACK",				ACTION_ACK},
	{"C",	"CREATE",			ACTION_CREATE},
	{"CH",	"CHALLENGE",			ACTION_CHALLENGE},
	{"CHR", "CHALLENGE_RESPONSE",		ACTION_CHALLENGE_RESPONSE},
	{"CR",	"CREATEORREAD",			ACTION_CREATEORREAD},
	{"D",	"DELETE",			ACTION_DELETE},
	{"E",	"ERROR",			ACTION_ERROR},
	{"EVT", "EVENT",			ACTION_EVENT},
	{"H",	"HAS",				ACTION_HAS},
	{"L",	"LISTEN",			ACTION_LISTEN},
	{"LA",	"LISTEN_ACCEPT",		ACTION_LISTEN_ACCEPT},
	{"LR",	"LISTEN_REJECT",		ACTION_LISTEN_REJECT},
	{"P",	"PATCH",			ACTION_PATCH},
	{"PU",	"PROVIDER_UPDATE",		ACTION_PROVIDER_UPDATE},
	{"Q",	"QUERY",			ACTION_QUERY},
	{"R",	"READ",				ACTION_READ},
	{"RED", "REDIRECT",			ACTION_REDIRECT},
	{"REJ", "REJECTION",			ACTION_REJECTION},
	{"REQ", "REQUEST",			ACTION_REQUEST},
	{"RES", "RESPONSE",			ACTION_RESPONSE},
	{"S",	"SUBSCRIBE",			ACTION_SUBSCRIBE},
	{"SH",	"SUBSCRIPTION_HAS_PROVIDER",	ACTION_SUBSCRIPTION_HAS_PROVIDER},
	{"SN",	"SNAPSHOT",			ACTION_SNAPSHOT},
	{"U",	"UPDATE",			ACTION_UPDATE},
	{"UL",	"UNLISTEN",			ACTION_UNLISTEN},
	{"US",	"UNSUBSCRIBE",			ACTION_UNSUBSCRIBE},
	{NULL, NULL, 0},
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

static struct dsio_topic_type *is_valid_topic(const char *ident)
{
	struct dsio_topic_type key = {.ident = ident };

	return bsearch(&key, topics, DSIO_NELEMENTS(topics) - 1,
		       sizeof key,
		       topic_bsearch_comparator);
}

static int action_bsearch_comparator(const void *a, const void *b)
{
	return strcmp(((const struct dsio_action_type *)a)->ident,
		      ((const struct dsio_action_type *)b)->ident);
}

static struct dsio_action_type *is_valid_action(const char *ident)
{
	struct dsio_action_type key = {.ident = ident };

	return bsearch(&key, actions, DSIO_NELEMENTS(actions) - 1,
		       sizeof key,
		       action_bsearch_comparator);
}

static int parse_topic(struct scanner *s)
{
	char *found = strchr(s->curr, DSIO_MSG_PART_SEPARATOR);

	if (!found) {
		return DSIO_ERROR;
	}

	*found = '\0';		/* punch hole in input */

	if ((s->msg->topic = is_valid_topic(s->curr)) == NULL)
		return DSIO_ERROR;

	s->curr = ++found;

	return DSIO_OK;
}

static int parse_action(struct scanner *s)
{
	char *found = strchr(s->curr, DSIO_MSG_PART_SEPARATOR);

	if (!found) {
		return DSIO_ERROR;
	}

	*found = '\0';		/* punch hole in input */

	if ((s->msg->action = is_valid_action(s->curr)) == NULL)
		return DSIO_ERROR;

	s->curr = ++found;

	return DSIO_OK;
}

static int parse_payload(struct scanner *s)
{
	char *token = s->curr;

	for (; *s->curr; s->curr++) {
		switch (*s->curr) {
		case DSIO_MSG_RECORD_SEPARATOR:
			*s->curr = '\0';
			return DSIO_OK;
		case DSIO_MSG_PART_SEPARATOR:
			*s->curr = '\0';
			s->msg->data = DSIO_REALLOC(s->allocator,
						    s->msg->data,
						    (1 + s->msg->ndata) * sizeof(char *));
			if (s->msg->data == NULL) {
				return DSIO_NOMEM;
			}
			s->msg->data[s->msg->ndata++] = token;
			token = s->curr + 1;
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
	s.msg->raw = input;

	if ((rc = parse_topic(&s)) != DSIO_OK)
		return rc;

	if ((rc = parse_action(&s)) != DSIO_OK)
		return rc;

	if ((rc = parse_payload(&s)) != DSIO_OK)
		return rc;

	return DSIO_OK;
}

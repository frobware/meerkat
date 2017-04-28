/* Copyright (C) 2017 Andrew McDermott
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <dsio/dsio.h>
#include "message.h"
#include "topics.h"
#include "actions.h"
#include "mprintf.h"

static const char MSG_SEP[] = { DSIO_MSG_SEPARATOR, '\0' };
static const char MSG_PART_SEP[] = { DSIO_MSG_PART_SEPARATOR, '\0' };

/*
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
 * Messages always start with TOPIC, then ACTION, but can contain zero
 * or more data fields.
 */

struct scanner {
	const char *input;
	const char *curr;
	struct dsio_msg *msg;
	const struct dsio_allocator *allocator;
	int parse_complete;
};

static int parse_topic(struct scanner *s)
{
	const char *token = s->curr;

	for (; *s->curr != '\0'; s->curr++) {
		switch (*s->curr) {
		case DSIO_MSG_PART_SEPARATOR:
			s->msg->topic = dsio_topic_lookup(token, s->curr - token);
			s->curr++;
			return s->msg->topic ? DSIO_OK : DSIO_ERROR;
		}
	}

	return DSIO_ERROR;
}

static int parse_action(struct scanner *s)
{
	const char *token = s->curr;

	for (; *s->curr != '\0'; s->curr++) {
		int c = *s->curr;
		switch (c) {
		case DSIO_MSG_SEPARATOR:
			s->parse_complete = 1;
			/* fallthrough */
		case DSIO_MSG_PART_SEPARATOR:
			s->msg->action = dsio_action_lookup(token, s->curr - token);
			s->curr++;
			return s->msg->action ? DSIO_OK : DSIO_ERROR;
		}
	}

	return DSIO_ERROR;
}

static int parse_payload(struct scanner *s)
{
	const char *token = s->curr;

	for (; *s->curr != '\0'; s->curr++) {
		switch (*s->curr) {
		case DSIO_MSG_SEPARATOR:
			s->parse_complete = 1;
			return DSIO_OK;
		case DSIO_MSG_PART_SEPARATOR:
			s->msg->data = DSIO_REALLOC(s->allocator,
						    s->msg->data,
						    (1 + s->msg->ndata) * sizeof(*s->msg->data));
			if (s->msg->data == NULL) {
				return DSIO_NOMEM;
			}
			s->msg->data[s->msg->ndata].start = token;
			s->msg->data[s->msg->ndata++].len = s->curr - token;
			token = s->curr + 1;
			break;
		}
	}

	return DSIO_OK;
}

int dsio_msg_parse(const struct dsio_allocator *a, const char *input, struct dsio_msg *msg)
{
	int rc;
	struct scanner s;

	if (input == NULL || *input == '\0')
		return DSIO_ERROR;

	memset(msg, 0, sizeof *msg);
	s.input = input;
	s.curr = input;
	s.parse_complete = 0;
	s.msg = msg;
	s.msg->raw = input;
	s.allocator = a;

	if ((rc = parse_topic(&s)) != DSIO_OK)
		return rc;

	if ((rc = parse_action(&s)) != DSIO_OK)
		return rc;

	if (!s.parse_complete) {
		if ((rc = parse_payload(&s)) != DSIO_OK)
			return rc;
	}

	return s.parse_complete ? DSIO_OK : DSIO_ERROR;
}

int parse_parts(const struct dsio_allocator *allocator, struct dsio_msg_list *msg_list, char *input)
{
	char *p, *q;

	for (p = input; /* true */; p = NULL) {
		char *token = strtok_r(p, MSG_PART_SEP, &q);
		if (token == NULL)
			break;
		printf("part: %s\n", token);
	}

	return 0;
}

int dsio_msg_parse2(const struct dsio_allocator *allocator, struct dsio_msg_list *msg_list, char *input)
{
	char *p, *q;

	for (p = input; /* true */; p = NULL) {
		char *token = strtok_r(p, MSG_SEP, &q);
		if (token == NULL)
			break;
		printf("token: %s\n", token);
		parse_parts(allocator, msg_list, token);
	}

	return 0;
}

char *dsio_msg_create(const struct dsio_allocator *allocator,
		      enum dsio_topic_tag topic,
		      enum dsio_action_tag action,
		      const char *payload)
{
	if (payload == NULL || *payload == '\0') {
		return dsio_mprintf(allocator,
				    "%s%c%s%c",
				    dsio_topics[topic].ident,
				    DSIO_MSG_PART_SEPARATOR,
				    dsio_actions[action].ident,
				    DSIO_MSG_SEPARATOR);
	}
	
	return dsio_mprintf(allocator,
			    "%s%c%s%c%s%c",
			    dsio_topics[topic].ident,
			    DSIO_MSG_PART_SEPARATOR,
			    dsio_actions[action].ident,
			    DSIO_MSG_PART_SEPARATOR,
			    payload,
			    DSIO_MSG_SEPARATOR);
}


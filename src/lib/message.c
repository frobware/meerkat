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
#include <dsio/message.h>
#include <dsio/topics.h>
#include <dsio/actions.h>

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
	char *input;
	char *curr;
	int parse_complete;
	struct dsio_msg *msg;
	const struct dsio_allocator *allocator;
};

static int parse_topic(struct scanner *s)
{
	char *token = s->curr;

	for (; *s->curr != '\0'; s->curr++) {
		switch (*s->curr) {
		case DSIO_MSG_PART_SEPARATOR:
			*s->curr = '\0';
			if ((s->msg->topic = topic_lookup(token)) == NULL)
				return DSIO_ERROR;
			s->curr++;
			return DSIO_OK;
		}
	}

	return DSIO_ERROR;
}

static int parse_action(struct scanner *s)
{
	char *token = s->curr;

	for (; *s->curr != '\0'; s->curr++) {
		switch (*s->curr) {
		case DSIO_MSG_RECORD_SEPARATOR:
			s->parse_complete = 1;
			/* fall through */
		case DSIO_MSG_PART_SEPARATOR:
			*s->curr = '\0';
			if ((s->msg->action = action_lookup(token)) == NULL)
				return DSIO_ERROR;
			s->curr++;
			return DSIO_OK;
		}
	}

	return DSIO_ERROR;
}

static int parse_payload(struct scanner *s)
{
	char *token = s->curr;

	for (; !s->parse_complete && *s->curr != '\0'; s->curr++) {
		switch (*s->curr) {
		case DSIO_MSG_RECORD_SEPARATOR:
			*s->curr = '\0';
			s->parse_complete = 1;
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

	return DSIO_OK;
}

int dsio_msg_parse(const struct dsio_allocator *a, char *const input, struct dsio_msg *msg)
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

	if ((rc = parse_payload(&s)) != DSIO_OK)
		return rc;

	return s.parse_complete ? DSIO_OK : DSIO_ERROR;
}

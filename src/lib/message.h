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

#pragma once

#include <dsio/allocator.h>

#define DSIO_MSG_PART_SEPARATOR	0x1f
#define DSIO_MSG_RECORD_SEPARATOR 0x1e

typedef enum {
	TOPIC_AUTH = 1,
	TOPIC_CONNECTION,
	TOPIC_EVENT,
	TOPIC_RPC,
	TOPIC_RECORD,
	TOPIC_ERROR,
} dsio_topic_tag;

typedef enum {
	ACTION_ACK = 1,
	ACTION_CHALLENGE,
	ACTION_CHALLENGE_RESPONSE,
	ACTION_CREATE,
	ACTION_CREATEORREAD,
	ACTION_DELETE,
	ACTION_ERROR,
	ACTION_EVENT,
	ACTION_HAS,
	ACTION_LISTEN,
	ACTION_LISTEN_ACCEPT,
	ACTION_LISTEN_REJECT,
	ACTION_PATCH,
	ACTION_PROVIDER_UPDATE,
	ACTION_QUERY,
	ACTION_READ,
	ACTION_REDIRECT,
	ACTION_REJECTION,
	ACTION_REQUEST,
	ACTION_RESPONSE,
	ACTION_SNAPSHOT,
	ACTION_SUBSCRIBE,
	ACTION_SUBSCRIPTION_HAS_PROVIDER,
	ACTION_UNLISTEN,
	ACTION_UNSUBSCRIBE,
	ACTION_UPDATE,
} dsio_action_tag;

struct dsio_topic_type {
	const char *ident;
	const char *descr;
	const dsio_topic_tag type;
};

struct dsio_action_type {
	const char *ident;
	const char *descr;
	const dsio_action_tag type;
};

struct dsio_msg {
	char *raw;
	const struct dsio_topic_type *topic;
	const struct dsio_action_type *action;
	char **data;
	size_t ndata;
};

extern struct dsio_topic_type topics[];
extern struct dsio_action_type actions[];

extern int dsio_msg_parse(const struct dsio_allocator *a, char *const input, struct dsio_msg *msg);
extern int dsio_msg_to_string(const struct dsio_msg *msg);

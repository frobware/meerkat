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

typedef enum {
	TOPIC_AUTH = 0,
	TOPIC_CONNECTION,
	TOPIC_EVENT,
	TOPIC_RPC,
	TOPIC_RECORD,
	TOPIC_ERROR,
	TOPIC_NR_TOPICS,
} dsio_topic_tag;

struct dsio_topic_type {
	const char *const ident;
	const char *const descr;
	const dsio_topic_tag type;
};

extern struct dsio_topic_type dsio_topics[];
extern struct dsio_topic_type *dsio_topic_lookup(const char *ident);

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

#include <stdlib.h>
#include <string.h>
#include <dsio/topics.h>

struct dsio_topic_type topics[] = {
	{"E",	"EVENT",	TOPIC_EVENT},
	{"P",	"RPC",		TOPIC_RPC},
	{"R",	"RECORD",	TOPIC_RECORD},
	{"A",	"AUTH",		TOPIC_AUTH},
	{"C",	"CONNECTION",	TOPIC_CONNECTION},
	{"X",	"ERROR",	TOPIC_ERROR},
	{NULL,	NULL,		TOPIC_NR_TOPICS},
};

struct dsio_topic_type *topic_lookup(const char *ident)
{
	/* Faster than bsearch given the table size. */
	for (size_t i = 0; i < TOPIC_NR_TOPICS; i++) {
		if (strcmp(ident, topics[i].ident) == 0)
			return &topics[i];
	}

	return NULL;
}

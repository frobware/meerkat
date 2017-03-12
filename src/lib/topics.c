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
#include "topics.h"

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

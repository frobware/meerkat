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
#include "actions.h"

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
	{NULL,	NULL,				ACTION_NR_ACTIONS},
};

static int bsearch_comparator(const void *a, const void *b)
{
	return strcmp(((const struct dsio_action_type *)a)->ident,
		      ((const struct dsio_action_type *)b)->ident);
}

struct dsio_action_type *action_lookup(const char *ident)
{
	struct dsio_action_type key = {.ident = ident };

	return bsearch(&key, actions, ACTION_NR_ACTIONS,
		       sizeof key,
		       bsearch_comparator);
}

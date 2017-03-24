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

#include <stdlib.h>
#include <string.h>
#include <dsio/actions.h>

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

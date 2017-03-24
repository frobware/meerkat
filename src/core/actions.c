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

struct dsio_action_type dsio_actions[] = {
	{"A",	"ACK",				DSIO_ACTION_ACK},
	{"C",	"CREATE",			DSIO_ACTION_CREATE},
	{"CH",	"CHALLENGE",			DSIO_ACTION_CHALLENGE},
	{"CHR", "CHALLENGE_RESPONSE",		DSIO_ACTION_CHALLENGE_RESPONSE},
	{"CR",	"CREATEORREAD",			DSIO_ACTION_CREATEORREAD},
	{"D",	"DELETE",			DSIO_ACTION_DELETE},
	{"E",	"ERROR",			DSIO_ACTION_ERROR},
	{"EVT", "EVENT",			DSIO_ACTION_EVENT},
	{"H",	"HAS",				DSIO_ACTION_HAS},
	{"L",	"LISTEN",			DSIO_ACTION_LISTEN},
	{"LA",	"LISTEN_ACCEPT",		DSIO_ACTION_LISTEN_ACCEPT},
	{"LR",	"LISTEN_REJECT",		DSIO_ACTION_LISTEN_REJECT},
	{"P",	"PATCH",			DSIO_ACTION_PATCH},
	{"PU",	"PROVIDER_UPDATE",		DSIO_ACTION_PROVIDER_UPDATE},
	{"Q",	"QUERY",			DSIO_ACTION_QUERY},
	{"R",	"READ",				DSIO_ACTION_READ},
	{"RED", "REDIRECT",			DSIO_ACTION_REDIRECT},
	{"REJ", "REJECTION",			DSIO_ACTION_REJECTION},
	{"REQ", "REQUEST",			DSIO_ACTION_REQUEST},
	{"RES", "RESPONSE",			DSIO_ACTION_RESPONSE},
	{"S",	"SUBSCRIBE",			DSIO_ACTION_SUBSCRIBE},
	{"SH",	"SUBSCRIPTION_HAS_PROVIDER",	DSIO_ACTION_SUBSCRIPTION_HAS_PROVIDER},
	{"SN",	"SNAPSHOT",			DSIO_ACTION_SNAPSHOT},
	{"U",	"UPDATE",			DSIO_ACTION_UPDATE},
	{"UL",	"UNLISTEN",			DSIO_ACTION_UNLISTEN},
	{"US",	"UNSUBSCRIBE",			DSIO_ACTION_UNSUBSCRIBE},
	{NULL,	NULL,				DSIO_ACTION_NR_ACTIONS},
};

static int bsearch_comparator(const void *a, const void *b)
{
	return strcmp(((const struct dsio_action_type *)a)->ident,
		      ((const struct dsio_action_type *)b)->ident);
}

struct dsio_action_type *dsio_action_lookup(const char *ident)
{
	struct dsio_action_type key = {.ident = ident };

	return bsearch(&key, dsio_actions, 
		       DSIO_ACTION_NR_ACTIONS,
		       sizeof key,
		       bsearch_comparator);
}

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
#include "actions.h"

/* The first column must remain sorted. */

struct dsio_action_type dsio_actions[] = {
	{"A",	1,	"ACK",				DSIO_ACTION_ACK},
	{"C",	1,	"CREATE",			DSIO_ACTION_CREATE},
	{"CH",	2,	"CHALLENGE",			DSIO_ACTION_CHALLENGE},
	{"CHR",	3,	"CHALLENGE_RESPONSE",		DSIO_ACTION_CHALLENGE_RESPONSE},
	{"CR",	2,	"CREATEORREAD",			DSIO_ACTION_CREATEORREAD},
	{"D",	1,	"DELETE",			DSIO_ACTION_DELETE},
	{"E",	1,	"ERROR",			DSIO_ACTION_ERROR},
	{"EVT",	3,	"EVENT",			DSIO_ACTION_EVENT},
	{"H",	1,	"HAS",				DSIO_ACTION_HAS},
	{"L",	1,	"LISTEN",			DSIO_ACTION_LISTEN},
	{"LA",	2,	"LISTEN_ACCEPT",		DSIO_ACTION_LISTEN_ACCEPT},
	{"LR",	2,	"LISTEN_REJECT",		DSIO_ACTION_LISTEN_REJECT},
	{"P",	1,	"PATCH",			DSIO_ACTION_PATCH},
	{"PI",	2,	"PING",				DSIO_ACTION_PING},
	{"PO",	2,	"PONG",				DSIO_ACTION_PONG},
	{"PU",	2,	"PROVIDER_UPDATE",		DSIO_ACTION_PROVIDER_UPDATE},
	{"Q",	1,	"QUERY",			DSIO_ACTION_QUERY},
	{"R",	1,	"READ",				DSIO_ACTION_READ},
	{"RED", 3,	"REDIRECT",			DSIO_ACTION_REDIRECT},
	{"REJ",	3,	"REJECTION",			DSIO_ACTION_REJECTION},
	{"REQ",	3,	"REQUEST",			DSIO_ACTION_REQUEST},
	{"RES", 3,	"RESPONSE",			DSIO_ACTION_RESPONSE},
	{"S",	1,	"SUBSCRIBE",			DSIO_ACTION_SUBSCRIBE},
	{"SH",	2,	"SUBSCRIPTION_HAS_PROVIDER",	DSIO_ACTION_SUBSCRIPTION_HAS_PROVIDER},
	{"SN",	2,	"SNAPSHOT",			DSIO_ACTION_SNAPSHOT},
	{"U",	1,	"UPDATE",			DSIO_ACTION_UPDATE},
	{"UL",	2,	"UNLISTEN",			DSIO_ACTION_UNLISTEN},
	{"US",	2,	"UNSUBSCRIBE",			DSIO_ACTION_UNSUBSCRIBE},
	{NULL,	0,	NULL,				DSIO_ACTION_NR_ACTIONS},
};

#define DSIO_MAX(A, B) ((A) > (B) ? (A) : (B))

static int bsearch_comparator(const void *a, const void *b)
{
	const struct dsio_action_type *x = a;
	const struct dsio_action_type *y = b;
	return strncmp(x->ident, y->ident, DSIO_MAX(x->ident_len, y->ident_len));
}

struct dsio_action_type *dsio_action_lookup(const char *ident, size_t len)
{
	struct dsio_action_type key;

	key.ident = ident;
	key.ident_len = len;

	return bsearch(&key, dsio_actions,
		       DSIO_ACTION_NR_ACTIONS,
		       sizeof key,
		       bsearch_comparator);
}

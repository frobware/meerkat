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
#include <dsio/topics.h>

struct dsio_topic_type dsio_topics[] = {
	{"E",	1,	"EVENT",	DSIO_TOPIC_EVENT},
	{"P",	1,	"RPC",		DSIO_TOPIC_RPC},
	{"R",	1,	"RECORD",	DSIO_TOPIC_RECORD},
	{"A",	1,	"AUTH",		DSIO_TOPIC_AUTH},
	{"C",	1,	"CONNECTION",	DSIO_TOPIC_CONNECTION},
	{"X",	1,	"ERROR",	DSIO_TOPIC_ERROR},
	{NULL,	0,	NULL,		DSIO_TOPIC_NR_TOPICS},
};

#define DSIO_MAX(A, B) ((A) > (B) ? (A) : (B))

struct dsio_topic_type *dsio_topic_lookup(const void *p, size_t len)
{
	size_t i;
	
	/* Faster than bsearch given the table size and the expected
	 * frequency of topic types. */

	for (i = 0; i < DSIO_TOPIC_NR_TOPICS; i++) {
		size_t n = DSIO_MAX(len, dsio_topics[i].ident_len);
		if (strncmp(p, dsio_topics[i].ident, n) == 0)
			return &dsio_topics[i];
	}

	return NULL;
}

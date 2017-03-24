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
	{"E",	"EVENT",	TOPIC_EVENT},
	{"P",	"RPC",		TOPIC_RPC},
	{"R",	"RECORD",	TOPIC_RECORD},
	{"A",	"AUTH",		TOPIC_AUTH},
	{"C",	"CONNECTION",	TOPIC_CONNECTION},
	{"X",	"ERROR",	TOPIC_ERROR},
	{NULL,	NULL,		TOPIC_NR_TOPICS},
};

struct dsio_topic_type *dsio_topic_lookup(const char *ident)
{
	size_t i;
	
	/* Faster than bsearch given the table size. */
	for (i = 0; i < TOPIC_NR_TOPICS; i++) {
		if (strcmp(ident, dsio_topics[i].ident) == 0)
			return &dsio_topics[i];
	}

	return NULL;
}

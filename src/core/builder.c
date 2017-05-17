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

#include <stdio.h>
#include <string.h>

#include <dsio/allocator.h>

#include "mprintf.h"
#include "message.h"
#include "builder.h"
#include "bb.h"

char *dsio_msg_build(const struct dsio_allocator *allocator,
		     enum dsio_topic_tag topic,
		     enum dsio_action_tag action, ...)
{
	va_list ap;
	char *result;

	va_start(ap, action);
	result = dsio_msg_vbuild(allocator, topic, action, ap);
	va_end(ap);

	return result;
}

char *dsio_msg_vbuild(const struct dsio_allocator *allocator,
		      enum dsio_topic_tag topic,
		      enum dsio_action_tag action,
		      va_list ap)
{
	struct dsio_bb bb;
	va_list ap_local;
	size_t len = 0;
	char *arg;

	dsio_strbuf_init(&bb, allocator);

	va_copy(ap_local, ap);
	len += dsio_topics[topic].ident_len;
	len += dsio_actions[action].ident_len;

	while ((arg = va_arg(ap_local, char *)) != NULL) {
		printf("XXXX=%s\n", arg);
		len += strlen(arg) + 1; /* the 1 is for DSIO_MSG_PART_SEPARATOR */
	}

	va_end(ap_local);

	len +=2;		/* DSIO_MSG_SEPARATOR + NUL byte */

	printf("XXXX=%zd\n", len);

	return dsio_mprintf(allocator,
			    "%s%c%s%c",
			    dsio_topics[topic].ident,
			    DSIO_MSG_PART_SEPARATOR,
			    dsio_actions[action].ident,
			    DSIO_MSG_SEPARATOR);
}

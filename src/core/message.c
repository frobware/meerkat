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
#include <stdlib.h>
#include <assert.h>

#include <dsio/dsio.h>
#include "message.h"
#include "topics.h"
#include "actions.h"
#include "mprintf.h"

static const char MSG_SEP[] = { DSIO_MSG_SEPARATOR, '\0' };
static const char MSG_PART_SEP[] = { DSIO_MSG_PART_SEPARATOR, '\0' };

static int part_list_grow(const struct dsio_allocator *a, struct dsio_msg_part_list *l)
{
	if (l->len == l->cap) {
		size_t new_cap = l->cap;
		void *new_partv;

		if (l->len == 0) {
			new_cap = 1;
		} else {
			new_cap *= 2;
		}

		new_partv = DSIO_REALLOC(a, l->partv, new_cap * sizeof *l->partv);

		if (new_partv == NULL) {
			return DSIO_NOMEM;
		}

		l->cap = new_cap;
		l->partv = new_partv;
	}

	return DSIO_OK;
}

static int msg_list_grow(const struct dsio_allocator *a, struct dsio_msg_list *l)
{
	if (l->len == l->cap) {
		size_t nbytes;
		size_t new_cap = l->cap;
		size_t element_size = sizeof *l->msgv;
		void *new_msgv;

		if (l->len == 0) {
			new_cap = 1;
		} else {
			new_cap *= 2;
		}

		new_msgv = DSIO_REALLOC(a, l->msgv, new_cap * element_size);

		if (new_msgv == NULL) {
			return DSIO_NOMEM;
		}

		nbytes = (new_cap - l->cap) * element_size;
		memset((char *)new_msgv + (l->cap * element_size), 0, nbytes);

		l->cap = new_cap;
		l->msgv = new_msgv;
	}

	return DSIO_OK;
}

static void msg_list_reset(struct dsio_msg_list *l)
{
	size_t i, j;

	for (i = 0; i < l->cap; i++) {
		for (j = 0; j < l->msgv[i].part_list.cap; j++) {
			l->msgv[i].part_list.partv[j].len = 0;
		}
		l->len = 0;
	}
}

void dsio_msg_list_init(const struct dsio_allocator *a, struct dsio_msg_list *l)
{
	memset(l, 0, sizeof *l);
	l->allocator = a;
}

static int parse_parts(char *input, const struct dsio_allocator *a, struct dsio_msg_part_list *l)
{
	int rc;
	char *token, *q;

	token = strtok_r(input, MSG_PART_SEP, &q);

	while (token != NULL) {
		printf("part->len = %zd\n", l->len);
		printf("part->cap = %zd\n", l->cap);
		printf("part: %s\n", token);
		if ((rc = part_list_grow(a, l)) != DSIO_OK)
			return rc;
		l->len++;
		token = strtok_r(NULL, MSG_PART_SEP, &q);
	}

	return DSIO_OK;
}

static int parse_messages(char *input, struct dsio_msg_list *l)
{
	int rc;
	char *token, *q;

	msg_list_reset(l);
	token = strtok_r(input, MSG_SEP, &q);

	while (token != NULL) {
		rc = msg_list_grow(l->allocator, l);

		if (rc != DSIO_OK)
			return rc;

		rc = parse_parts(token, l->allocator, &l->msgv[l->len].part_list);

		if (rc != DSIO_OK)
			return rc;

		l->len++;
		token = strtok_r(NULL, MSG_SEP, &q);
	}

	return DSIO_OK;
}

int dsio_msg_parse(char *input, struct dsio_msg_list *msg_list)
{
	printf("N=%s\n", input);
	return parse_messages(input, msg_list);
}

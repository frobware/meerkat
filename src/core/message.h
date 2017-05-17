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

#pragma once

#include <dsio/allocator.h>
#include "topics.h"
#include "actions.h"

enum {
	DSIO_MSG_SEPARATOR = 0x1e,
	DSIO_MSG_PART_SEPARATOR = 0x1f
};

struct dsio_msg_part {
	void *data;
	size_t len;
};

struct dsio_msg_part_list {
	struct dsio_msg_part *partv;
	size_t len;
	size_t cap;
};

struct dsio_msg {
	const struct dsio_topic_type *topic;
	const struct dsio_action_type *action;
	struct dsio_msg_part_list part_list;
};

struct dsio_msg_list {
	struct dsio_msg *msgv;
	size_t len;
	size_t cap;
	const struct dsio_allocator *allocator;
};

extern void dsio_msg_list_init(const struct dsio_allocator *allocator, struct dsio_msg_list *list);
extern int dsio_msg_parse(char *input, struct dsio_msg_list *messages);
extern int dsio_msg_release(struct dsio_msg_list *messages);

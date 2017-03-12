#pragma once

#include <dsio/allocator.h>

#define DSIO_MSG_PART_SEPARATOR	0x1f
#define DSIO_MSG_RECORD_SEPARATOR 0x1e

struct dsio_msg {
	char *raw;
	const struct dsio_topic_type *topic;
	const struct dsio_action_type *action;
	char **data;
	size_t ndata;
};

extern struct dsio_topic_type topics[];
extern struct dsio_action_type actions[];

extern int dsio_msg_parse(const struct dsio_allocator *a, char *const input, struct dsio_msg *msg);
extern int dsio_msg_to_string(const struct dsio_msg *msg);

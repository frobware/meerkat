#pragma once

#include <dsio/allocator.h>

#define DSIO_MSG_PART_SEPARATOR	0x1f
#define DSIO_MSG_RECORD_SEPARATOR 0x1e

struct dsio_payload {
	const char *data;
	size_t len;
};

struct dsio_msgdata {
	size_t len;
	const char data[1];
};

struct dsio_msg {
	const struct dsio_topic_type *topic;
	const struct dsio_action_type *action;
	struct dsio_payload *payload;
	size_t npayload;
};

extern struct dsio_topic_type topics[];
extern struct dsio_action_type actions[];

extern int dsio_msg_parse(const struct dsio_allocator *a, char *const input, struct dsio_msg *msg);

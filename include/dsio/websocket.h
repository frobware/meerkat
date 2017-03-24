#pragma once

#include <stdlib.h>
#include <dsio/allocator.h>

struct dsio_client;

struct dsio_websocket {
	int (*close)();
	int (*shutdown)();
	int (*send)(void *buffer, size_t length);
	int (*recv)(void *buffer, size_t *length);
 	void *private;
 	void *userdata;
	struct dsio_client *client;
};

typedef int (*DSIO_WEBSOCKET_BROKER)(struct dsio_client *client);
typedef int (*DSIO_WEBSOCKET_MSGPUMP)(struct dsio_websocket *ws);

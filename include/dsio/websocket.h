#pragma once

#include <stdlib.h>
#include <dsio/allocator.h>

struct dsio_client;

struct dsio_websocket {
	int (*close)();
	int (*send)(void *buffer, size_t length);
	struct dsio_client *client;
};

typedef int (*DSIO_WEBSOCKET_CONNECT)(struct dsio_client *client);
typedef void (*DSIO_WEBSOCKET_DISCONNECT)(struct dsio_client *client);
typedef int (*DSIO_WEBSOCKET_MSGPUMP)(struct dsio_websocket *ws);

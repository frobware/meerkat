#pragma once

#include <stdlib.h>
#include <dsio/allocator.h>

struct dsio_websocket {
 	void *userdata;
	int (*close)();
	int (*shutdown)();
	int (*send)(void *buffer, size_t length);
	int (*recv)(void *buffer, size_t *length);
	int (*on_open)(struct dsio_websocket *ws, void *userdata);
};

typedef int (*DSIO_WEBSOCKET_CONNECT)(char *uri,
				      struct dsio_allocator *a,
				      struct dsio_websocket **out);

typedef int (*DSIO_WEBSOCKET_SERVICE)(struct dsio_websocket *ws);

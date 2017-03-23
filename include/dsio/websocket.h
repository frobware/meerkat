#pragma once

#include <stdlib.h>
#include <dsio/allocator.h>

struct dsio_websocket {
	int (*close)();
	int (*shutdown)();
	int (*send)(void *buffer, size_t length);
	int (*recv)(void *buffer, size_t *length);
	int (*on_open)(struct dsio_websocket *ws, void *userdata);
 	void *userdata;
};

typedef int (*DSIO_WEBSOCKET_FACTORY)(char *uri,
				      const struct dsio_allocator *a,
				      struct dsio_websocket **out);

typedef int (*DSIO_WEBSOCKET_RUNLOOP)(struct dsio_websocket *ws);

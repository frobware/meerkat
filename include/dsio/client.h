#pragma once

#include <dsio/websocket.h>

#define DSIO_CLIENT_OK 0
#define DSIO_CLIENT_CONNECT_ERROR 1

typedef enum {
	DSIO_CLIENT_OPEN,
	DSIO_CLIENT_CLOSED
} client_state_t;

struct dsio_client {
	struct dsio_websocket *ws;
};

struct dsio_client_cfg {
	const struct dsio_allocator *allocator;
	const char *uri;
	const char *name;
	const char *passwd;
};

extern int dsio_login(const struct dsio_client_cfg *cfg,
		      struct dsio_client *client,
		      DSIO_WEBSOCKET_CONNECT *wsf);

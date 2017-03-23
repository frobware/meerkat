#pragma once

#include <dsio/websocket.h>

#define DSIO_CLIENT_OK 0
#define DSIO_CLIENT_CONNECT_ERROR 1

enum dsio_client_state {
	DSIO_CLIENT_OPEN,
	DSIO_CLIENT_CLOSED
};

struct dsio_client_cfg {
	const struct dsio_allocator *allocator;
	const char *uri;
	const char *username;
	const char *password;
	DSIO_WEBSOCKET_FACTORY wsk_factory;
};

struct dsio_client {
	enum dsio_client_state state;
	const struct dsio_client_cfg *cfg;
	struct dsio_websocket *ws;
};

extern struct dsio_client *dsio_login(const struct dsio_client_cfg *cfg);
extern void dsio_logout(struct dsio_client *c);

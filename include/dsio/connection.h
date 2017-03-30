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

#include <stdlib.h>
#include <dsio/client.h>
#include <dsio/websocket.h>

enum dsio_connection_state {
	DSIO_CONNECTION_CLOSED = 0,
	DSIO_CONNECTION_AWAITING_CONNECTION,
	DSIO_CONNECTION_CHALLENGING,
	DSIO_CONNECTION_AWAITING_AUTHENTICATION,
	DSIO_CONNECTION_AUTHENTICATING,
	DSIO_CONNECTION_OPEN,
	DSIO_CONNECTION_ERROR,
	DSIO_CONNECTION_RECONNECTING,
	DSIO_CONNECTION_NR_STATES
};

struct dsio_client;

struct dsio_connection_cfg {
	const struct dsio_allocator *allocator;
	const char *uri;
	const char *username;
	const char *password;
	const char *ssl_cert_filepath;
	const char *ssl_private_key_filepath;
	int allow_self_signed_certs;
	DSIO_WEBSOCKET_CONNECT websocket_connect;
	DSIO_WEBSOCKET_DISCONNECT websocket_disconnect;
};

struct dsio_connection {
	struct dsio_connection_cfg *cfg;
	struct dsio_client *client;
	enum dsio_connection_state state;
	struct dsio_websocket endpoint;
};

extern int dsio_conn_init(struct dsio_connection *conn, struct dsio_client *client, struct dsio_connection_cfg *cfg);
extern const char *const dsio_connection_state_names[DSIO_CONNECTION_NR_STATES];

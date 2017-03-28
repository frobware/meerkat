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

enum dsio_conn_state {
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

struct dsio_connection {
	struct dsio_client *client;
	enum dsio_conn_state state;
	int (*on_open)(struct dsio_connection *conn);
	int (*on_close)(struct dsio_connection *conn);
	int (*on_error)(struct dsio_connection *conn, const char *msg);
	int (*on_message)(struct dsio_connection *conn, void *buffer, size_t len);
	struct dsio_websocket *endpoint;
	void *userdata;
};

extern int dsio_conn_init(struct dsio_connection *conn, struct dsio_client *client);
extern const char *const dsio_connection_state_names[DSIO_CONNECTION_NR_STATES];

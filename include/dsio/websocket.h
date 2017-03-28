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

struct dsio_websocket;
struct dsio_connection;

enum dsio_websocket_readystate {
	/* The connection is not yet open. */
	DSIO_WEBSOCKET_READYSTATE_CONNECTING = 0,
	/* The connection is open and ready to communicate. */
	DSIO_WEBSOCKET_READYSTATE_OPEN,
	/* The connection is in the process of closing. */
	DSIO_WEBSOCKET_READYSTATE_CLOSING,
	/* The connection is closed or couldn't be opened. */
	DSIO_WEBSOCKET_READYSTATE_CLOSED
};

struct dsio_websocket {
	struct dsio_connection *conn;
	enum dsio_websocket_readystate state;
	void *userdata;
	int (*close)();
	int (*send)(void *buffer, size_t length);
#if 0
	int (*on_open)(struct dsio_websocket *ws);
	int (*on_close)(struct dsio_websocket *ws);
	int (*on_error)(struct dsio_websocket *ws, const char *msg);
	int (*on_message)(struct dsio_websocket *ws, void *buffer, size_t len);
#endif
};

typedef int (*DSIO_WEBSOCKET_CONNECT)(struct dsio_connection *conn);
typedef void (*DSIO_WEBSOCKET_DISCONNECT)(struct dsio_connection *conn);
typedef int (*DSIO_WEBSOCKET_MSGPUMP)(struct dsio_connection *conn);

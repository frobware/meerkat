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
#include <dsio/websocket.h>
#include "message.h"
#include "connection-state.h"

/* Synthetic events for the Ragel state machine. The settings here
 * must match the events defined in connection-state.rl or it will all
 * break. */

#define EVENT_CLOSE "C"
#define EVENT_ERROR "E"
#define EVENT_MESSAGE "M"
#define EVENT_OPEN "O"

struct dsio_client;

struct dsio_connection {
	struct dsio_client *client;
	struct connection_fsm state;
	struct dsio_websocket endpoint;
	int (*on_error)(struct dsio_connection *conn, const char *errmsg);
	int (*on_message)(struct dsio_connection *conn, struct dsio_msg *msg);
};

extern int dsio_conn_init(struct dsio_connection *conn, struct dsio_client *client);

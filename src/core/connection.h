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

struct dsio_client;

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

struct dsio_connection {
        int cs;			/* current state -- for Ragel */
        enum dsio_connection_state state;
	struct dsio_client *client;
	struct dsio_websocket endpoint;
};

extern int connection_init(struct dsio_connection *conn, struct dsio_client *client);
extern int connection_state_init(struct dsio_connection *conn, struct dsio_client *client);
extern int connection_state_assert(struct dsio_connection *conn, const char *event);
extern int connection_state_exec(struct dsio_connection *conn, const char *event, size_t len);
extern int connection_state_finish(struct dsio_connection *conn);
extern int connection_state_done(struct dsio_connection *conn, const char *event);
extern const char *const dsio_connection_state_names[DSIO_CONNECTION_NR_STATES];
extern int connection_send_challenge_response(struct dsio_connection *connection);

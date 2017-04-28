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

#include <stdio.h>
#include <string.h>

#include <dsio/dsio.h>
#include <dsio/log.h>
#include "message.h"
#include "mprintf.h"
#include "client.h"
#include "connection.h"

const char *const dsio_connection_state_names[] = {
	"CLOSED",
	"AWAITING CONNECTION",
	"CHALLENGING",
	"AWAITING AUTHENTICATION",
	"AUTHENTICATING",
	"OPEN",
	"ERROR",
	"RECONNECTING",
	NULL
};

static int on_open(struct dsio_websocket *ws)
{
	DSIO_LOG_NOTICE("on_open\n");
	sm_init(&ws->client->connection, ws->client);
	return sm_exec(&ws->client->connection, "WS_OPEN", 7);
}

static int on_close(struct dsio_websocket *ws)
{
	DSIO_LOG_NOTICE("on_close\n");
	return sm_exec(&ws->client->connection, "WS_CLOSE", 8);
}

static int on_error(struct dsio_websocket *ws, const char *msg)
{
	DSIO_LOG_NOTICE("on_error\n");
	return sm_exec(&ws->client->connection, "WS_ERROR", 8);
}

static int on_message(struct dsio_websocket *ws, void *data, size_t len)
{
	return sm_exec(&ws->client->connection, data, len);
}

int connection_send_challenge_response(struct dsio_connection *c)
{
	char *m = dsio_msg_create(c->client->cfg->allocator,
				  DSIO_TOPIC_CONNECTION,
				  DSIO_ACTION_CHALLENGE_RESPONSE,
				  c->client->cfg->uri);
	return c->endpoint.send(&c->endpoint, m, strlen(m));
}

int connection_send_auth_response(struct dsio_connection *c)
{
	char *m = dsio_msg_create(c->client->cfg->allocator,
				  DSIO_TOPIC_AUTH,
				  DSIO_ACTION_REQUEST,
				  "{\"username\":\"bob\", \"password\":\"bob\"}");
	return c->endpoint.send(&c->endpoint, m, strlen(m));
}

int connection_send_pong_response(struct dsio_connection *c)
{
	char *m = dsio_msg_create(c->client->cfg->allocator,
				  DSIO_TOPIC_CONNECTION,
				  DSIO_ACTION_PONG,
				  NULL);
	return c->endpoint.send(&c->endpoint, m, strlen(m));
}

void sm_state_set(struct dsio_connection *conn,
		  enum dsio_connection_state newstate)
{
	DSIO_CONNECTION_STATE_CHANGE cb = conn->client->cfg->connection_state_change;

	if (cb != NULL) {
		cb(conn->client, conn->state, newstate);
	}

	conn->state = newstate;
}

int connection_init(struct dsio_connection *conn, struct dsio_client *client)
{
	memset(conn, 0, sizeof *conn);
	conn->client = client;
	conn->endpoint.client = client;
	conn->endpoint.on_open = on_open;
	conn->endpoint.on_close = on_close;
	conn->endpoint.on_message = on_message;
	conn->endpoint.on_error = on_error;
	return client->cfg->websocket_connect(client->cfg, &conn->endpoint);
}

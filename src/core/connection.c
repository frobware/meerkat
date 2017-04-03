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
	dsio_log_notice("on_open\n");
	return connection_state_exec(&ws->client->connection, "WS_OPEN", 7);
}

static int on_close(struct dsio_websocket *ws)
{
	dsio_log_notice("on_close\n");
	return connection_state_exec(&ws->client->connection, "WS_CLOSE", 8);
}

static int on_error(struct dsio_websocket *ws, const char *msg)
{
	dsio_log_notice("on_error\n");
	return connection_state_exec(&ws->client->connection, "WS_ERROR", 8);
}

static int on_message(struct dsio_websocket *ws, void *data, size_t len)
{
	int rc;
	struct dsio_msg msg;
	char msgid[128];
	size_t n;

	rc = dsio_msg_parse(ws->client->cfg->allocator, data, &msg);

	if (rc != DSIO_OK) {
		dsio_log_err("unknown message: %.*s\n", (int)len, (char *)data);
		return -1;
	}

	/* Turn data into something we want to visualize in Ragel. */

	n = snprintf(msgid, sizeof(msgid)-1, "%s%c%s",
		     msg.topic->ident,
		     '_',
		     msg.action->ident);

	dsio_log(DSIO_LL_CONNECTION,
		 "message: %s_%s\n",
		 msg.topic->descr,
		 msg.action->descr);

	connection_state_exec(&ws->client->connection, msgid, n);

	return 0;
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
	/* char *auth = "{}"; */

	/* char *username; */
	/* char *password; */

	/* if (c->client->cfg->username != NULL) { */
	/* 	username = dsio_mprintf("\"username\":%s", */
	/* 				c-client->cfg->username); */
	/* } */
	    
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

void connection_state_change(struct dsio_connection *conn,
				    enum dsio_connection_state next)
{
	DSIO_CONNECTION_STATE_CHANGE cb = conn->client->cfg->connection_state_change;
	
	if (cb != NULL) {
		cb(conn->client, conn->state, next);
	}

	conn->state = next;
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
	connection_state_init(&client->connection, client);
	return client->cfg->websocket_connect(client->cfg, &conn->endpoint);
}

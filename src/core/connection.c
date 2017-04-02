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
	return connection_state_exec(&ws->client->connection, "OPEN", 4);
}

static int on_close(struct dsio_websocket *ws)
{
	dsio_log_notice("on_close\n");
	return connection_state_exec(&ws->client->connection, "CLOSE", 5);
}

static int on_error(struct dsio_websocket *ws, const char *msg)
{
	dsio_log_notice("on_error\n");
	return connection_state_exec(&ws->client->connection, "ERROR", 5);
}

static int on_message(struct dsio_websocket *ws, void *buf, size_t len)
{
	int rc;
	struct dsio_msg msg;
	char msgid[128];
	size_t n;

	dsio_log_notice("on_message\n");

	rc = dsio_msg_parse(ws->client->cfg->allocator, buf, &msg);

	if (rc != DSIO_OK) {
		dsio_log_err("unknown message: %.*s\n", (int)len, (char *)buf);
		return -1;
	}

	/* Turn the message into something Ragel can eaily consume. */

	n = snprintf(msgid, sizeof(msgid)-1, "%s%c%s",
		     msg.topic->ident,
		     '_',
		     msg.action->ident);

	dsio_log_notice("message: %.*s\n", (int)len, (char *)buf);
	connection_state_exec(&ws->client->connection, msgid, n);

	return 0;
}

int connection_send_challenge_response(struct dsio_connection *c)
{
#if 0
	char *m = dsio_mprintf(c->client->cfg->allocator,
			       "C%cCHR%c%s%c",
			       DSIO_MSG_PART_SEPARATOR,
			       DSIO_MSG_PART_SEPARATOR,
			       c->client->cfg->uri,
			       DSIO_MSG_SEPARATOR);
#endif
	char *m = dsio_msg_create(c->client->cfg->allocator,
				  DSIO_TOPIC_CONNECTION,
				  DSIO_ACTION_CHALLENGE_RESPONSE,
				  c->client->cfg->uri);
	printf("<<<<<<<<<<<<<<<<%s>>>>>>>>>>>>>>>>>>>>\n", m);
	return c->endpoint.send(&c->endpoint, m, strlen(m));
}

int connection_send_auth_response(struct dsio_connection *c)
{
	char *m = dsio_mprintf(c->client->cfg->allocator,
			       "A%cCHR%c%s%c",
			       DSIO_MSG_PART_SEPARATOR,
			       DSIO_MSG_PART_SEPARATOR,
			       c->client->cfg->uri,
			       DSIO_MSG_SEPARATOR);

	return c->endpoint.send(&c->endpoint, m, strlen(m));
}

void connection_state_update_client(struct dsio_connection *conn,
				    enum dsio_connection_state newstate)
{
	conn->state = newstate;

	if (conn->client->cfg->connection_state_change != NULL) {
		conn->client->cfg->connection_state_change(conn->client, newstate);
	}
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

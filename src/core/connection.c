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
#include "client.h"
#include "connection.h"
#include "connection-state.h"

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
	return connection_fsm_exec(&ws->client->connection.state, "OPEN", 4);
}

static int on_close(struct dsio_websocket *ws)
{
	dsio_log_notice("on_close\n");
	return connection_fsm_exec(&ws->client->connection.state, "CLOSE", 5);
}

static int on_error(struct dsio_websocket *ws, const char *msg)
{
	dsio_log_notice("on_error\n");
	return connection_fsm_exec(&ws->client->connection.state, "ERROR", 5);
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

	/* turn the message into a more human consumable message. */
	n = snprintf(msgid, sizeof(msgid)-1, "%s%c%s",
		     msg.topic->ident,
		     '_', 
		     msg.action->ident);

	dsio_log_notice("message: %.*s\n", (int)len, (char *)buf);
	connection_fsm_exec(&ws->client->connection.state, msgid, n);

	return 0;
}

int dsio_conn_init(struct dsio_connection *connection, struct dsio_client *client)
{
	memset(connection, 0, sizeof *connection);
	connection->client = client;
	connection->endpoint.client = client;
	connection->endpoint.on_open = on_open;
	connection->endpoint.on_close = on_close;
	connection->endpoint.on_message = on_message;
	connection->endpoint.on_error = on_error;
	connection_fsm_init(&client->connection.state, client);
	return client->cfg->websocket_connect(client->cfg, &connection->endpoint);
}

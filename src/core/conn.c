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

#include <string.h>

#include <dsio/conn.h>
#include <dsio/client.h>

int dsio_conn_init(struct dsio_conn *conn, struct dsio_client *client)
{
	memset(conn, 0, sizeof *conn);
	conn->client = client;
	return (*client->cfg->websocket_connect)(conn);
}

#if 0
static int on_open(struct dsio_client *client)
{
	dsio_log_notice("CLIENT_ESTABLISHED\n");
	client->state = DSIO_CLIENT_OPEN;
	return 0;
}

static int on_close(struct dsio_client *client)
{
	client->state = DSIO_CLIENT_CLOSED;
	dsio_log_notice("CLOSED\n");
	return 0;
}

static int on_error(struct dsio_client *client, const char *msg)
{
	dsio_log_notice("ERROR %s\n", msg);
	return 0;
}

static int on_message(struct dsio_client *client, void *buf, size_t len)
{
	int rc;
	struct dsio_msg msg;

	dsio_log_notice("MESSAGE %zd, '%s'\n", len, (char *)buf);
	rc = dsio_msg_parse(client->cfg->allocator, buf, &msg);

	if (rc != DSIO_OK) {
		char errmsg[256];
		snprintf(errmsg, 255, "unknown message: %s", (char *)buf);
		return client->on_error(client, errmsg);
	}
	
	return 0;
}
#endif

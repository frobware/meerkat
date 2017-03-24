/* Copyright (C) 2017 Andrew Mcdermott
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
#include <dsio/client.h>
#include "mprintf.h"

static void on_open(struct dsio_client *client)
{
	client->state = DSIO_CLIENT_OPEN;
	printf("%s:%d\n", __FILE__, __LINE__);
}

static void on_close(struct dsio_client *client)
{
	client->state = DSIO_CLIENT_OPEN;
	printf("%s:%d\n", __FILE__, __LINE__);
}

static void on_error(struct dsio_client *client)
{
	printf("%s:%d\n", __FILE__, __LINE__);
}

static void on_message(struct dsio_client *client)
{
	printf("%s:%d\n", __FILE__, __LINE__);
}

int dsio_login(struct dsio_client *client, const struct dsio_client_cfg *cfg)
{
	int rc;
	memset(client, 0, sizeof *client);
	client->buf = dsio_mprintf(cfg->allocator, "client: %p", client);
 	client->cfg = cfg;
	client->on_open = on_open;
	client->on_close = on_close;
	client->on_error = on_error;
	client->on_message = on_message;
	client->state = DSIO_CLIENT_CLOSED;
	rc = (*client->cfg->websocket_connect)(client);
	printf("websocket_connect rc=%d\n", rc);
	return rc;
}

void dsio_logout(struct dsio_client *client)
{
	(*client->cfg->websocket_disconnect)(client);
}


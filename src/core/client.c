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
#include <dsio/client.h>
#include <dsio/log.h>
#include <dsio/conn.h>
#include "mprintf.h"

int dsio_login(struct dsio_client *client, const struct dsio_client_cfg *cfg)
{
	memset(client, 0, sizeof *client);
	client->cfg = cfg;
	client->state = DSIO_CLIENT_CLOSED;
	client->conn.client = client;
	return dsio_conn_init(&client->conn, client);
}

void dsio_logout(struct dsio_client *client)
{
	(*client->cfg->websocket_disconnect)(&client->conn);
}

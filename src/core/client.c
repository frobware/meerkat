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

#include "mprintf.h"
#include "client.h"
#include "connection.h"

int dsio_client_create(struct dsio_client **clientp, struct dsio_client_cfg *cfg)
{
	int rc;
	struct dsio_client *client;

	*clientp = NULL;

	if ((client = DSIO_MALLOC(cfg->allocator, sizeof *client)) == NULL) {
		return DSIO_NOMEM;
	}

	memset(client, 0, sizeof *client);
	client->cfg = cfg;
	client->connection.client = client;
	rc = dsio_conn_init(&client->connection, client);

	if (rc != DSIO_OK) {
		DSIO_FREE(cfg->allocator, client);
		return rc;
	}

	*clientp = client;
	return DSIO_OK;
}

int dsio_client_service(struct dsio_client *client)
{
	return client->cfg->websocket_service(&client->connection.endpoint);
}

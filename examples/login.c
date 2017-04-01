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
#include <dsio/dsio.h>
#include "../src/libwebsockets/dsio-libwebsockets-impl.h"

#include <libwebsockets.h>

int main(int argc, char *argv[])
{
	int rc;
	struct dsio_client *client = NULL;
	struct dsio_client_cfg client_cfg;

	if (argc != 2) {
		fprintf(stderr, "usage: <URI>\n");
		return EXIT_FAILURE;
	}

	memset(&client_cfg, 0, sizeof client_cfg);
	client_cfg.allocator = dsio_stdlib_allocator;
	client_cfg.uri = argv[1];
	client_cfg.username = NULL;
	client_cfg.password = NULL;
	client_cfg.websocket_connect = dsio_libwebsockets_connect;
	client_cfg.websocket_disconnect = dsio_libwebsockets_disconnect;
	client_cfg.websocket_service = dsio_libwebsockets_service;

	if ((rc = dsio_client_create(&client, &client_cfg)) != DSIO_OK) {
		fprintf(stderr, "client create failed: %s\n", dsio_strerror(rc));
		return EXIT_FAILURE;
	}

	return dsio_client_service(client);
}

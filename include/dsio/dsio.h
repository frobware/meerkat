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

#include <dsio/allocator.h>
#include <dsio/websocket.h>
#include <dsio/log.h>

/* Status codes. */

#define DSIO_OK		0
#define DSIO_ERROR	1
#define DSIO_NOMEM	2

#define DSIO_NELEMENTS(A) ((sizeof((A)) / sizeof((A))[0]))

struct dsio_client_cfg {
	const struct dsio_allocator *allocator;
	const char *uri;
	const char *username;
	const char *password;
	const char *ssl_cert_filepath;
	const char *ssl_private_key_filepath;
	int allow_self_signed_certs;
	DSIO_WEBSOCKET_CONNECT websocket_connect;
	DSIO_WEBSOCKET_DISCONNECT websocket_disconnect;
	DSIO_WEBSOCKET_SERVICE websocket_service;
	void *userdata;
};

extern int dsio_client_create(struct dsio_client **result, struct dsio_client_cfg *cfg);
extern void dsio_client_delete(struct dsio_client *client);
extern int dsio_client_service(struct dsio_client *client);

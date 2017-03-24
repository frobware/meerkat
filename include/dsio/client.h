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

#include <dsio/message.h>
#include <dsio/websocket.h>

#define DSIO_CLIENT_OK 0
#define DSIO_CLIENT_CONNECT_ERROR 1

enum dsio_client_state {
	DSIO_CLIENT_OPEN,
	DSIO_CLIENT_CLOSED
};

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
};

struct dsio_client {
	char *buf;
	enum dsio_client_state state;
	const struct dsio_client_cfg *cfg;
	void (*on_open)(struct dsio_client *client);
	void (*on_close)(struct dsio_client *client);
	void (*on_error)(struct dsio_client *client, const char *msg);
	void (*on_message)(struct dsio_client *client, const struct dsio_msg *msg);
	void *userdata;
};

extern int dsio_login(struct dsio_client *c, const struct dsio_client_cfg *cfg);
extern void dsio_logout(struct dsio_client *c);

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

#include <dsio/connection.h>
#include <dsio/message.h>
#include <dsio/websocket.h>

struct dsio_client {
	int (*on_error)(struct dsio_client *client, const char *errmsg);
	int (*on_message)(struct dsio_client *client, struct dsio_msg *msg);
	struct dsio_connection connection;
	void *userdata;
};

extern int dsio_login(struct dsio_client *c, struct dsio_connection_cfg *cfg);
extern void dsio_logout(struct dsio_client *c);

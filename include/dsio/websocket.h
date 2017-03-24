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

#include <stdlib.h>
#include <dsio/allocator.h>

struct dsio_client;

struct dsio_websocket {
	int (*close)();
	int (*send)(void *buffer, size_t length);
	struct dsio_client *client;
};

typedef int (*DSIO_WEBSOCKET_CONNECT)(struct dsio_client *client);
typedef void (*DSIO_WEBSOCKET_DISCONNECT)(struct dsio_client *client);
typedef int (*DSIO_WEBSOCKET_MSGPUMP)(struct dsio_websocket *ws);

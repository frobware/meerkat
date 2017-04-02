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

enum dsio_action_tag {
	DSIO_ACTION_ACK = 0,
	DSIO_ACTION_CREATE,
	DSIO_ACTION_CHALLENGE,
	DSIO_ACTION_CHALLENGE_RESPONSE,
	DSIO_ACTION_CREATEORREAD,
	DSIO_ACTION_DELETE,
	DSIO_ACTION_ERROR,
	DSIO_ACTION_EVENT,
	DSIO_ACTION_HAS,
	DSIO_ACTION_LISTEN,
	DSIO_ACTION_LISTEN_ACCEPT,
	DSIO_ACTION_LISTEN_REJECT,
	DSIO_ACTION_PATCH,
	DSIO_ACTION_PING,
	DSIO_ACTION_PONG,
	DSIO_ACTION_PROVIDER_UPDATE,
	DSIO_ACTION_QUERY,
	DSIO_ACTION_READ,
	DSIO_ACTION_REDIRECT,
	DSIO_ACTION_REJECTION,
	DSIO_ACTION_REQUEST,
	DSIO_ACTION_RESPONSE,
	DSIO_ACTION_SUBSCRIBE,
	DSIO_ACTION_SUBSCRIPTION_HAS_PROVIDER,
	DSIO_ACTION_SNAPSHOT,
	DSIO_ACTION_UPDATE,
	DSIO_ACTION_UNLISTEN,
	DSIO_ACTION_UNSUBSCRIBE,
	DSIO_ACTION_NR_ACTIONS
};

struct dsio_action_type {
	const char *ident;
	size_t ident_len;
	const char *descr;
	enum dsio_action_tag type;
};

extern struct dsio_action_type dsio_actions[];
extern struct dsio_action_type *dsio_action_lookup(const char *p, size_t len);

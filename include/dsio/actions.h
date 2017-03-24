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

typedef enum {
	ACTION_ACK = 0,
	ACTION_CHALLENGE,
	ACTION_CHALLENGE_RESPONSE,
	ACTION_CREATE,
	ACTION_CREATEORREAD,
	ACTION_DELETE,
	ACTION_ERROR,
	ACTION_EVENT,
	ACTION_HAS,
	ACTION_LISTEN,
	ACTION_LISTEN_ACCEPT,
	ACTION_LISTEN_REJECT,
	ACTION_PATCH,
	ACTION_PROVIDER_UPDATE,
	ACTION_QUERY,
	ACTION_READ,
	ACTION_REDIRECT,
	ACTION_REJECTION,
	ACTION_REQUEST,
	ACTION_RESPONSE,
	ACTION_SNAPSHOT,
	ACTION_SUBSCRIBE,
	ACTION_SUBSCRIPTION_HAS_PROVIDER,
	ACTION_UNLISTEN,
	ACTION_UNSUBSCRIBE,
	ACTION_UPDATE,

	ACTION_NR_ACTIONS,
} dsio_action_tag;

struct dsio_action_type {
	const char *const ident;
	const char *const descr;
	const dsio_action_tag type;
};

extern struct dsio_action_type dsio_actions[];
extern struct dsio_action_type *dsio_action_lookup(const char *ident);

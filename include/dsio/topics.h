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
	TOPIC_AUTH = 0,
	TOPIC_CONNECTION,
	TOPIC_EVENT,
	TOPIC_RPC,
	TOPIC_RECORD,
	TOPIC_ERROR,
	TOPIC_NR_TOPICS,
} dsio_topic_tag;

struct dsio_topic_type {
	const char *const ident;
	const char *const descr;
	const dsio_topic_tag type;
};

extern struct dsio_topic_type dsio_topics[];
extern struct dsio_topic_type *dsio_topic_lookup(const char *ident);

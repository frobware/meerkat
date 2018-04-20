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

#include <stdarg.h>

#include "topics.h"
#include "actions.h"

extern char *dsio_msg_build(const struct dsio_allocator *allocator,
			    enum dsio_topic_tag topic,
			    enum dsio_action_tag action,
			    ...);

extern char *dsio_msg_vbuild(const struct dsio_allocator *allocator,
			     enum dsio_topic_tag topic,
			     enum dsio_action_tag action,
			     va_list ap);
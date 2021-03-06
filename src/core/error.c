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

#include <dsio/error.h>

static const char *const errors[] = {
	"success",
	"logic error",
	"out of memory",
	"invalid message",
	"unknown topic",
	"unknown action",
};

const char *dsio_strerror(int n)
{
	if (n < 0 || n >= DSIO_NR_ERRORS) {
		return "unknown error";
	}

	return errors[n];
}

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
#include <stdarg.h>

#include <dsio/dsio.h>
#include <dsio/allocator.h>

#include "mprintf.h"

extern char *dsio_mprintf(const struct dsio_allocator *a,
			  const char *format, ...)
	__attribute__ ((format(printf, 2, 3)));

/*
 * Print into memory obtained from malloc() - this is the example code
 * from the vsnprintf manual page. This function sprintf's fmt/args
 * into a buffer that grows to accomodate the result. Returns NULL if
 * there was insufficient memory, or a pointer to the formatted
 * message on success.
 */
char *dsio_mprintf(const struct dsio_allocator *allocator, const char *fmt, ...)
{
	size_t size = 0;
	char *p = NULL;
	va_list ap;

	/* Determine required size. */
	va_start(ap, fmt);
	size = vsnprintf(p, size, fmt, ap);
	va_end(ap);

	size++;			/* For '\0' */

	if ((p = DSIO_MALLOC(allocator, size)) == NULL) {
		return NULL;
	}

	va_start(ap, fmt);
	size = vsnprintf(p, size, fmt, ap);
	va_end(ap);

	return p;
}

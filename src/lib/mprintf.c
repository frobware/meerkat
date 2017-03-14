/*
 * Copyright 2017 Andrew McDermott
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdarg.h>

#include <dsio/dsio.h>
#include <dsio/allocator.h>

#include "mprintf.h"

char *dsio_mprintf(const struct dsio_allocator *a, const char *format, ...) __attribute__ ((format(printf, 2, 3)));

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

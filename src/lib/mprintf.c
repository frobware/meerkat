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

	if ((p = DSIO_ALLOC(allocator, size)) == NULL) {
		return NULL;
	}

	va_start(ap, fmt);
	size = vsnprintf(p, size, fmt, ap);
	va_end(ap);

	return p;
}

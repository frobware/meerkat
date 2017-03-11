#include <stdio.h>
#include <stdarg.h>

#include <dsio/dsio.h>
#include <dsio/allocator.h>

/*
 * Print into memory obtained from malloc().  sprintf fmt/args into a
 * buffer that grows to accomodate the result. Returns NULL if there
 * was insufficient memory, or a pointer to the formatted message on
 * success.
 */
char *dsio_mprintf(const struct dsio_allocator *allocator, const char *fmt, ...)
{
	int n, size = 17;
	char *p, *np;
	va_list ap;

	if ((p = DSIO_ALLOC(allocator, size)) == NULL)
		return NULL;

	while (1) {
		va_start(ap, fmt);
		n = vsnprintf(p, size, fmt, ap);
		va_end(ap);

		if (n > -1 && n < size)
			return p;

		/* Not enough space; grow some! */

		if (n > -1) {
			size = n+1;
		} else {
			size *= 2;
		}

		if ((np = DSIO_REALLOC(allocator, p, size)) == NULL) {
			DSIO_FREE(allocator, p);
			return NULL;
		} else {
			p = np;
		}
	}
}

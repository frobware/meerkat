/* Dynamic String Buffer */

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include "strbuf.h"

char strbuf_ephemeral[1] = { '\0' };

void strbuf_init(struct strbuf *strbuf,
		 const struct dsio_allocator *allocator)
{
	assert(allocator != NULL);
	dsio_bbuf_init(strbuf, allocator, 0);
	strbuf->data = strbuf_ephemeral;
}

void strbuf_release(struct strbuf *strbuf)
{
	if (strbuf->data != strbuf_ephemeral)
		DSIO_FREE(strbuf->allocator, strbuf->data);
}

char *strbuf_steal(struct strbuf *strbuf)
{
	char *result;

	assert(strbuf->data[strbuf->capacity - 1] == '\0');

	if (strbuf->len == 0)
		return NULL;

	result = strbuf->data;
	strbuf_init(strbuf, strbuf->allocator);

	return result;
}

void strbuf_reset(struct strbuf *strbuf)
{
	strbuf->len = 0;

	if (strbuf->data != strbuf_ephemeral) {
		assert(strbuf->capacity > 0);
		strbuf->data[0] = '\0';
	} else {
		assert(strbuf_ephemeral[0] == '\0');
	}
}

int strbuf_fmt(struct strbuf *strbuf, const char *fmt, ...)
{
	int rc;
	va_list ap;

	va_start(ap, fmt);
	rc = strbuf_vfmt(strbuf, fmt, ap);
	va_end(ap);

	return rc;
}

int strbuf_vfmt(struct strbuf *strbuf, const char *fmt, va_list ap)
{
	va_list ap_local;
	size_t size;
	int rc;
	int n;
	char *write_head;

	va_copy(ap_local, ap);
	size = vsnprintf(NULL, 0, fmt, ap_local);
	va_end(ap_local);

	if (size == 0)
		return DSIO_OK;
	if (strbuf->data == strbuf_ephemeral)
		strbuf->data = NULL;

	rc = dsio_bbuf_reserve(strbuf, size + 1, &write_head);
	if (rc != DSIO_OK)
		return rc;
	strbuf->data[strbuf->capacity - 1] = '\0';

	n = vsnprintf(write_head, size + 1, fmt, ap);
	if (n > -1 && n < size)
		return DSIO_ERROR;
	strbuf->len--;

	return DSIO_OK;
}

int strbuf_addc(struct strbuf *strbuf, int c)
{
	int rc;

	if (strbuf->data == strbuf_ephemeral)
		strbuf->data = NULL;

	rc = dsio_bbuf_write(strbuf, c);
	if (rc != DSIO_OK)
		return rc;

	strbuf->data[strbuf->len] = '\0';

	return DSIO_OK;
}

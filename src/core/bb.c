#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <dsio/error.h>
#include <dsio/allocator.h>

#include "bb.h"

/*
 * A byte buffer; similar to what you find in glib but its initial
 * property is a NUL-terminated string. Its growth factor is pow(2)
 * which may be too aggressive in some circumstances.
 */

static char dsio_bb_ephemeral[] = { '\0' };

static void dsio_bb_init(struct dsio_bb *bb, const struct dsio_allocator *allocator)
{
	bb->allocator = allocator;
	bb->len = 0;
	bb->capacity = 0;
	bb->data = dsio_bb_ephemeral;
	assert(dsio_bb_ephemeral[0] == '\0');
}

static int dsio_bb_capacity_set(struct dsio_bb *bb, size_t new_capacity)
{
	char *new_buf;
	size_t curr_capacity;

	if (new_capacity < bb->capacity)
		return DSIO_OK;

	curr_capacity = bb->capacity ? bb->capacity : 1;

	while (curr_capacity <= new_capacity)
		curr_capacity *= 2;

	new_buf = DSIO_REALLOC(bb->allocator, bb->data, curr_capacity);

	if (new_buf == NULL)
		return DSIO_NOMEM;

	bb->capacity = curr_capacity;
	bb->data = new_buf;

	return DSIO_OK;
}

static int dsio_bb_write(struct dsio_bb *bb, unsigned char byte)
{
	int rc = dsio_bb_capacity_set(bb, bb->len + 1);

	if (rc != DSIO_OK)
		return rc;

	bb->data[bb->len++] = byte;

	return DSIO_OK;
}

static int dsio_bb_reserve(struct dsio_bb *bb, size_t new_data_len, char **new_data)
{
	size_t newsz = bb->len + new_data_len;
	int rc = dsio_bb_capacity_set(bb, newsz);

	if (rc != DSIO_OK)
		return rc;

	*new_data = bb->data + bb->len;
	bb->len = newsz;

	return DSIO_OK;
}

void dsio_strbuf_init(struct dsio_bb *bb, const struct dsio_allocator *allocator)
{
	dsio_bb_init(bb, allocator);
}

char *dsio_strbuf_purloin(struct dsio_bb *bb)
{
	char *data;

	if (bb->len == 0)
		return NULL;

	data = bb->data;
	dsio_bb_init(bb, bb->allocator);

	return data;
}

void dsio_strbuf_reset(struct dsio_bb *bb)
{
	bb->len = 0;

	if (bb->data != dsio_bb_ephemeral) {
		bb->data[0] = '\0';
	}
}

int dsio_strbuf_fmt(struct dsio_bb *bb, const char *fmt, ...)
{
	int rc;
	va_list ap;

	va_start(ap, fmt);
	rc = dsio_strbuf_vfmt(bb, fmt, ap);
	va_end(ap);

	return rc;
}

int dsio_strbuf_vfmt(struct dsio_bb *bb, const char *fmt, va_list ap)
{
	va_list ap_local;
	size_t size, n;
	int rc;
	char *next;

	va_copy(ap_local, ap);
	size = vsnprintf(NULL, 0, fmt, ap_local);
	va_end(ap_local);

	if (size == 0)
		return DSIO_OK;

	if (bb->data == dsio_bb_ephemeral)
		bb->data = NULL;

	rc = dsio_bb_reserve(bb, size + 1, &next);

	if (rc != DSIO_OK)
		return rc;

	bb->data[bb->capacity - 1] = '\0';

	n = vsnprintf(next, size + 1, fmt, ap);

	if (n < size)
		return DSIO_ERROR;

	bb->len--;		/* why? */

	return DSIO_OK;
}

int dsio_strbuf_addc(struct dsio_bb *bb, int c)
{
	int rc;

	if (bb->data == dsio_bb_ephemeral)
		bb->data = NULL;

	rc = dsio_bb_write(bb, c);

	if (rc != DSIO_OK)
		return rc;

	bb->data[bb->len] = '\0';

	return DSIO_OK;
}

int dsio_bb_append(struct dsio_bb *bb, const char *new_data, size_t new_data_len)
{
	size_t newsz = bb->len + new_data_len;
	int rc = dsio_bb_capacity_set(bb, newsz);

	if (rc != DSIO_OK)
		return rc;

	memcpy(&bb->data[bb->len], new_data, new_data_len);
	bb->len = newsz;

	return DSIO_OK;
}

void dsio_bb_terminate(struct dsio_bb *bb)
{
	if (bb->data != dsio_bb_ephemeral)
		DSIO_FREE(bb->allocator, bb->data);
}

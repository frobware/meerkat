#pragma once

#include <stdarg.h>

struct dsio_allocator;

struct dsio_bb {
	char *data;
	size_t len;
	size_t capacity;
	const struct dsio_allocator *allocator;
};

extern void dsio_strbuf_init(struct dsio_bb *bb, const struct dsio_allocator *allocator);
extern char *dsio_strbuf_purloin(struct dsio_bb *bb);
extern void dsio_strbuf_reset(struct dsio_bb *bb);
extern int dsio_strbuf_fmt(struct dsio_bb *bb, const char *fmt, ...);
extern int dsio_strbuf_vfmt(struct dsio_bb *bb, const char *fmt, va_list ap);
extern int dsio_strbuf_addc(struct dsio_bb *bb, int c);
extern void dsio_strbuf_terminate(struct dsio_bb *bb);

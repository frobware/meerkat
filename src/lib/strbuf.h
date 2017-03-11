#pragma once

#include <stdlib.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

extern char strbuf_ephemeral[1];

struct strbuf {
	char *data;
	size_t len;
	size_t capacity;
	const struct dsio_allocator *allocator;
};

#define STRBUF_INIT(A) { strbuf_ephemeral, 0, 0, (A) }

#ifdef __GNUC__
int strbuf_fmt(struct strbuf *strbuf, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
#endif

extern void strbuf_init(struct strbuf *strbuf, const struct dsio_allocator *allocator);
extern void strbuf_release(struct strbuf *strbuf);
extern char *strbuf_steal(struct strbuf *strbuf);
extern void strbuf_reset(struct strbuf *strbuf);
extern int strbuf_fmt(struct strbuf *strbuf, const char *fmt, ...);
extern int strbuf_vfmt(struct strbuf *strbuf, const char *fmt, va_list ap);
extern int strbuf_addc(struct strbuf *strbuf, int c);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

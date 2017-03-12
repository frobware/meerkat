#pragma once

#include <dsio/allocator.h>

char *dsio_mprintf(const struct dsio_allocator *a, const char *format, ...) __attribute__ ((format(printf, 2, 3)));

extern char *dsio_mprintf(const struct dsio_allocator *allocator,
			  const char *fmt, ...);

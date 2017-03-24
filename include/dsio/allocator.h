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

#pragma once

#include <stdlib.h>

#define DSIO_MALLOC(A, SIZE) (((A)->malloc)((A), (SIZE)))
#define DSIO_FREE(A, PTR) (((A)->free)((A), (PTR)))
#define DSIO_REALLOC(A, PTR, SIZE) (((A)->realloc)((A), (PTR), (SIZE)))

struct dsio_allocator;

typedef void *(*DSIO_MALLOC_FUNC) (const struct dsio_allocator *, size_t size);
typedef void *(*DSIO_REALLOC_FUNC) (const struct dsio_allocator *, void *p, size_t size);
typedef void (*DSIO_FREE_FUNC) (const struct dsio_allocator *, void *p);

struct dsio_allocator {
	DSIO_MALLOC_FUNC malloc;
	DSIO_REALLOC_FUNC realloc;
	DSIO_FREE_FUNC free;
};

extern struct dsio_allocator *dsio_stdlib_allocator;

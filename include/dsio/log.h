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

#include <stdarg.h>

enum dsio_log_levels {
	DSIO_LL_ERR	   = 1 << 0,
	DSIO_LL_WARN	   = 1 << 1,
	DSIO_LL_NOTICE	   = 1 << 2,
	DSIO_LL_INFO	   = 1 << 3,
	DSIO_LL_DEBUG	   = 1 << 4,
	DSIO_LL_CONNECTION = 1 << 5,
	DSIO_LL_COUNT	   = 6
};

extern void dsio_log(int level, const char *format, ...)
	__attribute__((format(printf, 2, 3)));

extern void dsio_logv(int level, const char *format, va_list ap)
	__attribute__((format(printf, 2, 0)));

extern int dsio_log_level_get(void);
extern void dsio_log_level_set(int level, void (*func)(int level, const char *line));

#define dsio_log_err(...) dsio_log(DSIO_LL_ERR, __VA_ARGS__)
#define dsio_log_warn(...) dsio_log(DSIO_LL_WARN, __VA_ARGS__)
#define dsio_log_notice(...) dsio_log(DSIO_LL_NOTICE, __VA_ARGS__)
#define dsio_log_info(...) dsio_log(DSIO_LL_INFO, __VA_ARGS__)
#define dsio_log_debug(...) dsio_log(DSIO_LL_DEBUG, __VA_ARGS__)

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

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <dsio/log.h>

typedef void (*LOG_EMITTER)(int level, const char *line, void *userdata);

static void *log_userdata;
static int log_level = DSIO_LL_ERR | DSIO_LL_WARN | DSIO_LL_NOTICE;

static const char *const log_level_names[] = {
	"ERROR",
	"WARNING",
	"NOTICE",
	"INFO",
	"DEBUG",
	"CONNECTION",
};

static unsigned long long time_in_microseconds(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((unsigned long long)tv.tv_sec * 1000000LL) + tv.tv_usec;
}

static unsigned int msbit(unsigned int value)
{
	unsigned int r = 0;
	/*
	 * The log base 2 of an integer is the same as the position of
	 * the highest bit set (or most significant bit set, MSB).
	 */
	while (value >>= 1)
		r++;

	return r;
}

static void log_emit_stderr(int level, const char *line, void *userdata)
{
	char buf[128];
	unsigned long long now = time_in_microseconds() / 100;

	sprintf(buf, "[%llu:%04llu] %s: ",
		now / 10000LL,
		now % 10000LL,
		log_level_names[msbit(level)]);

	fprintf(stderr, "%s%s", buf, line);
}

LOG_EMITTER log_emit = log_emit_stderr;

void dsio_log_logv(int filter, const char *format, va_list ap)
{
	char buf[256];

	if (!(log_level & filter))
		return;

	vsnprintf(buf, sizeof(buf), format, ap);
	buf[sizeof(buf) - 1] = '\0';
	log_emit(filter, buf, log_userdata);
}

void dsio_log(int filter, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	dsio_log_logv(filter, format, ap);
	va_end(ap);
}

int dsio_log_level_get(void)
{
	return log_level;
}

void dsio_log_level_set(int level, void (*func)(int level, const char *line, void *userdata), void *userdata)
{
	log_level = level;
	log_userdata = userdata;
	
	if (func != NULL)
		log_emit = func;
}

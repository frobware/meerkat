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
#include <assert.h>
#include <dsio/log.h>
#include "client.h"
#include "connection.h"

/* Debugging Trace Transitions. */

#define TraceT(ACTION, CURR_STATE, NEXT_STATE) \
	dsio_log(DSIO_LL_CONNECTION, "%d -> %d -> action(%s)\n", \
		 CURR_STATE, NEXT_STATE, "" # ACTION)

%%{

machine connection;
access c->;
include connection "state-actions.rl";

### events

CLOSE	= "CLOSE";
ERROR	= "ERROR";
OPEN	= "OPEN";

C_CH  = "C_CH";
C_PI  = "C_PI";

### state chart

main := (
  start: (
	  OPEN @open -> AwaitingConnection
  ),

  AwaitingConnection: (
	  CLOSE @close -> final |
	  C_CH @challenge_response -> Challenging
  ),

  Challenging: (
	  CLOSE @close -> final |
	  C_PI @pong -> Challenging
  )

) >begin $!error;

}%%

%% write data;

int connection_state_init(struct dsio_connection *c, struct dsio_client *client)
{
	c->client = client;
	%% write init;
	return 1;		/* good */
}

/* Returns:
 *   1 if we're already in the final state
 *  -1 if we had a failure
 *   0 to continue accepting events
 */
inline int connection_state_assert(struct dsio_connection *c, const char *event)
{
	if (c->cs == connection_error) {
		if (event != NULL) {
			dsio_log(DSIO_LL_ERR, "event='%s'\n", event);
		}
		dsio_log(DSIO_LL_CONNECTION, "c->cs = connection_state_error\n");
		return -1;
	}

	if (c->cs >= connection_first_final) {
		dsio_log(DSIO_LL_CONNECTION, "c->cs = connection_state_first_final\n");
		return 1;
	}

	return 0;
}

/*
 * Inject an event into the machine.
 *
 * Return 0 to accept more events, 1 for finished, -1 for failure.
 */
int connection_state_exec(struct dsio_connection *c, const char *event, size_t len)
{
	const char *p = event;
	const char *pe = p + len;
	const char *eof = NULL;

	dsio_log(DSIO_LL_CONNECTION, "%s event\n", event);

	if (connection_state_done(c, event))
		return -1;

	%% write exec;

	return connection_state_assert(c, event);
}

int connection_state_finish(struct dsio_connection *c)
{
	dsio_log(DSIO_LL_CONNECTION, "connection_state_finish()\n");
	return connection_state_assert(c, NULL);
}

int connection_state_done(struct dsio_connection *c, const char *event)
{
	return c->cs == connection_error || c->cs == connection_first_final;
}

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
access conn->;
include connection "state-actions.rl";

### events

WS_CLOSE    = "WS_CLOSE";
WS_ERROR    = "WS_ERROR";
WS_OPEN	    = "WS_OPEN";

A_ACK	    = "A_A";
A_ERR	    = "A_E";

C_A   = "C_A";
C_CH  = "C_CH";
C_PI  = "C_PI";
C_REJ = "C_REJ";

### state chart

main := (
  start: (
	  WS_OPEN @reconnecting -> AwaitingConnection
  ),
  AwaitingConnection: (
	  WS_CLOSE @close -> final |
	  C_CH @challenge_response -> ChallengingWait
  ),
  ChallengingWait: (
	  WS_CLOSE @close -> final |
	  C_REJ @close -> final |
	  C_PI @pong -> ChallengingWait |
	  C_A @authenticate -> AwaitingAuthentication
  ),
  AwaitingAuthentication: (
	  WS_CLOSE @close -> final |
	  C_PI @pong -> AwaitingAuthentication |
	  A_ERR @error -> final |
	  A_ACK @open -> Open
  ),
  Open: (
	  WS_CLOSE @close -> final |
	  C_PI @pong -> Open
  ) -> Open
) >begin $!error;

}%%

%% write data;

int connection_state_init(struct dsio_connection *conn, struct dsio_client *client)
{
	conn->client = client;
	%% write init;
	return 1;		/* good */
}

/* Returns:
 *   1 if we're already in the final state
 *  -1 if we had a failure
 *   0 if OK to continue accepting events
 */
inline int connection_state_assert(struct dsio_connection *conn, const char *event)
{
	if (conn->cs == connection_error) {
		if (event != NULL) {
			dsio_log(DSIO_LL_ERR, "event='%s'\n", event);
		}
		dsio_log(DSIO_LL_CONNECTION, "conn->cs = connection_state_error\n");
		return -1;
	}

	if (conn->cs >= connection_first_final) {
		dsio_log(DSIO_LL_CONNECTION, "conn->cs = connection_state_first_final\n");
		return 1;
	}

	return 0;
}

/*
 * Inject an event into the machine.
 *
 * Return 0 to accept more events, 1 for finished, -1 for failure.
 */
int connection_state_exec(struct dsio_connection *conn, const char *event, size_t len)
{
	const char *p = event;
	const char *pe = p + len;
	const char *eof = NULL;

	dsio_log(DSIO_LL_CONNECTION, "inject '%s'\n", event);

	if (connection_state_done(conn, event)) {
		dsio_log(DSIO_LL_ERR, "something bad happened; %s event\n", event);
		return -1;
	}

	%% write exec;

	return connection_state_assert(conn, event);
}

int connection_state_finish(struct dsio_connection *conn)
{
	dsio_log(DSIO_LL_CONNECTION, "connection_state_finish()\n");
	return connection_state_assert(conn, NULL);
}

int connection_state_done(struct dsio_connection *conn, const char *event)
{
	return conn->cs == connection_error || conn->cs == connection_first_final;
}

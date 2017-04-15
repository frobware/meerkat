/* Copyright (C) 2017 Andrew McDermott -*-c-*-
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

/* Debug: Trace Transitions. */

#define TraceT(ACTION, CURR_STATE, NEXT_STATE) \
	dsio_log(DSIO_LL_NOTICE, "%d -> %d -> action(%s)\n", \
		 CURR_STATE, NEXT_STATE, "" # ACTION)

%%{

machine connection;
access conn->;

action begin {
	TraceT(begin, fcurs, ftargs);
	sm_state_set(conn, DSIO_CONNECTION_CLOSED);
}

action start {
	TraceT(start, fcurs, ftargs);
	sm_state_set(conn, DSIO_CONNECTION_AWAITING_CONNECTION);
}

action challenge {
	TraceT(challenge, fcurs, ftargs);
	sm_state_set(conn, DSIO_CONNECTION_CHALLENGING);
	connection_send_challenge_response(conn);
}

action authenticate {
	TraceT(challenge_response, fcurs, ftargs);
	sm_state_set(conn, DSIO_CONNECTION_AWAITING_AUTHENTICATION);
	connection_send_auth_response(conn);
}

action open {
	TraceT(open, fcurs, ftargs);
	sm_state_set(conn, DSIO_CONNECTION_OPEN);
}

action close {
	TraceT(close, fcurs, ftargs);
	sm_state_set(conn, DSIO_CONNECTION_CLOSED);
}

action error {
	TraceT(error, fcurs, ftargs);
	sm_state_set(conn, DSIO_CONNECTION_ERROR);
}

action pong {
	TraceT(pong, fcurs, ftargs);
	connection_send_pong_response(conn);
}

### websocket events

WS_CLOSE    = "WS_CLOSE";
WS_ERROR    = "WS_ERROR";
WS_OPEN	    = "WS_OPEN";

### auth events

A_ACK	    = "A_A";
A_ERR	    = "A_E";

### connection events

C_ACTION_ACK	   = "C_A";
C_ACTION_CHALLENGE = "C_CH";
C_ACTION_ERROR	   = "C_E";
C_ACTION_PING	   = "C_PI";
C_ACTION_REDIRECT  = "C_RED";
C_ACTION_REJECT    = "C_REJ";

### state chart

main := (
  start: (
	  WS_OPEN @start -> AwaitingConnection
  ),
  AwaitingConnection: (
	  WS_CLOSE @close -> final |
	  C_ACTION_CHALLENGE @challenge -> ChallengingWait
  ),
  ChallengingWait: (
	  WS_CLOSE @close -> final |
	  C_ACTION_REJECT @close -> final |
	  C_ACTION_REDIRECT @close -> final |
	  C_ACTION_PING @pong -> ChallengingWait |
	  C_ACTION_ACK @authenticate -> AwaitingAuthentication
  ),
  AwaitingAuthentication: (
	  WS_CLOSE @close -> final |
	  C_ACTION_PING @pong -> AwaitingAuthentication |
	  A_ERR @error -> final |
	  A_ACK @open -> Open
  ),
  Open: (
	  WS_CLOSE @close -> final |
	  C_ACTION_PING @pong -> Open
  )
) $!error;

}%%

%% write data;

int sm_init(struct dsio_connection *conn, struct dsio_client *client)
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
int sm_assert(struct dsio_connection *conn, const char *event)
{
	if (conn->cs == connection_error) {
		if (event != NULL) {
			dsio_log(DSIO_LL_ERR, "event='%s'\n", event);
		}
		dsio_log(DSIO_LL_CONNECTION, "conn->cs = connection_error\n");
		return -1;
	}

	if (conn->cs >= connection_first_final) {
		dsio_log(DSIO_LL_CONNECTION, "conn->cs = connection_first_final\n");
		return 1;
	}

	return 0;
}

/*
 * Execuate an event against the state machine.
 *
 * Returns 0 to accept more events, 1 for finished, -1 for failure.
 */
int sm_exec(struct dsio_connection *conn, const char *event, size_t len)
{
	const char *p = event;
	const char *pe = p + len;
	const char *eof = NULL;

	dsio_log(DSIO_LL_NOTICE, "RECV '%s'\n", event);

	if (sm_done(conn, event)) {
		dsio_log(DSIO_LL_ERR, "something bad happened; %s event\n", event);
		return -1;
	}

	%% write exec;

	return sm_assert(conn, event);
}

int sm_finish(struct dsio_connection *conn)
{
	dsio_log(DSIO_LL_CONNECTION, "sm_finish()\n");
	return sm_assert(conn, NULL);
}

int sm_done(struct dsio_connection *conn, const char *event)
{
	return conn->cs == connection_error || conn->cs == connection_first_final;
}

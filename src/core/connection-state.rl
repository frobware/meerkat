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
#include "connection-state.h"
#include "connection.h"

/* Debugging Trace Transitions. */

#define TraceT(ACTION, CURR_STATE, NEXT_STATE) \
	dsio_log(DSIO_LL_CONNECTION, "%d -> %d -> action(%s)\n", \
		 CURR_STATE, NEXT_STATE, "" # ACTION)

%%{

machine connection_fsm;
access state->;
include connection_fsm "connection-state-actions.rl";

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

int connection_fsm_init(struct connection_fsm *state, struct dsio_client *client)
{
	state->client = client;
	%% write init;
	return 1;		/* good */
}

/* Returns:
 *   1 if we're already in the final state
 *  -1 if we had a failure
 *   0 to continue accepting events
 */
inline int connection_fsm_assert(struct connection_fsm *state, const char *event)
{
	if (state->cs == connection_fsm_error) {
		if (event != NULL) {
			dsio_log(DSIO_LL_ERR, "event='%s'\n", event);
		}
		dsio_log(DSIO_LL_CONNECTION, "state->cs = connection_fsm_error\n");
		return -1;
	}

	if (state->cs >= connection_fsm_first_final) {
		dsio_log(DSIO_LL_CONNECTION, "state->cs = connection_fsm_first_final\n");
		return 1;
	}

	return 0;
}

/*
 * Inject an event into the machine.
 *
 * Return 0 to accept more events, 1 for finished, -1 for failure.
 */
int connection_fsm_exec(struct connection_fsm *state, const char *event, size_t len)
{
	const char *p = event;
	const char *pe = p + len;
	const char *eof = NULL;

	dsio_log(DSIO_LL_CONNECTION, "%s event\n", event);

	if (connection_fsm_done(state, event))
		return -1;

	%% write exec;

	return connection_fsm_assert(state, event);
}

int connection_fsm_finish(struct connection_fsm *state)
{
	dsio_log(DSIO_LL_CONNECTION, "connection_fsm_finish()\n");
	return connection_fsm_assert(state, NULL);
}

int connection_fsm_done(struct connection_fsm *state, const char *event)
{
	return state->cs == connection_fsm_error || state->cs == connection_fsm_first_final;
}

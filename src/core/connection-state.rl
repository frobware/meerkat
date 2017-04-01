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
#include "connection-state.h"
#include "connection.h"

/* Debugging Trace Transitions. */

#if 1
#define TraceT(ACTION, CURR_STATE, NEXT_STATE) \
	dsio_log(DSIO_LL_CONNECTION, "%d -> %d -> action(%s)\n", \
		 CURR_STATE, NEXT_STATE, "" # ACTION)
#else
#define TraceT(A,S,N)
#endif

%%{

machine connection_fsm;
access state->;
include connection_fsm "connection-state-actions.rl";

### events

CLOSE	= "CLOSE";
ERROR	= "ERROR";
OPEN	= "OPEN";

C_CH   = "C_CH";
C_CHR  = "C_CHR";
C_PING = "C_PI";

### state chart

Connection = (
  start: (
    OPEN -> AwaitingConnection
  ),

  AwaitingConnection: (
    C_CH @challenge -> ChallengingWait
  ),

  Ping: (
    C_PING @ping -> AwaitingConnection
  ),
  
  ChallengingWait: (
    C_CHR -> ChallengingWait
  ),

  Closing: (CLOSE @close -> final)

) >begin $!error;

main := Connection;

}%%

%% write data;

int connection_fsm_init(struct connection_fsm *state)
{
	%% write init;

	return 1;		/* good */
}

inline int connection_fsm_assert(struct connection_fsm *state)
{
	if (state->cs == connection_fsm_error) {
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
 * Inject the event against into the machine.
 *
 * Return 0 to accept more events, 1 for finished, -1 for failure.
 */
int connection_fsm_exec(struct connection_fsm *state, const char *event, size_t len)
{
	const char *p = event;
	const char *pe = p + len;
	const char *eof = NULL;

	dsio_log(DSIO_LL_CONNECTION, "event='%s'\n", event);

	%% write exec;

	return connection_fsm_assert(state);
}

int connection_fsm_finish(struct connection_fsm *state)
{
	dsio_log(DSIO_LL_CONNECTION, "connection_fsm_finish\n");
	return connection_fsm_assert(state);
}

int connection_fsm_done(struct connection_fsm *state)
{
	dsio_log(DSIO_LL_CONNECTION, "connection_fsm_done\n");
	return state->cs == connection_fsm_error || state->cs == connection_fsm_first_final;
}

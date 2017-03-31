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
#include "connection-state.h"
#include "connection.h"

/* Debugging Trace Transitions. */

#if 1
#define TT(A,S,N) printf("%d -> %d : %s, '%c'",S, N, "" # A, *p)
#else
#define TT(A,S,N)
#endif

%%{

machine connection_fsm;
access state->;
include connection_fsm "connection-state-actions.rl";

### events

open  = 'O';
close = 'C';

### state chart

Connection = (
  start:     (open  -> Accepting),
  Accepting: (close -> Closing),
  Closing:   (close -> final)
) >begin $!error;

main := Connection;

}%%

%% write data;

int connection_fsm_init(struct connection_fsm *state)
{
	assert(state->next == NULL && "attempt to init an active state");
	state->cs = DSIO_CONNECTION_CLOSED;

	%% write init;

	return 1;
}

int connection_fsm_invariant(struct connection_fsm *state, enum connection_event event)
{
	if (state->cs == connection_fsm_error) {
		return -1;
	}

	if (state->cs >= connection_fsm_first_final) {
		return 1;
	}

	return 0;
}

int connection_fsm_exec(struct connection_fsm *state, enum connection_event event)
{
	const char *p = (const char *)&event;
	const char *pe = p + 1;
	const char *eof = NULL;

	%% write exec;

	return connection_fsm_invariant(state, event);
}

int connection_fsm_finish(struct connection_fsm *state)
{
	return connection_fsm_invariant(state, 0);
}

int connection_fsm_done(struct connection_fsm *state)
{
	return state->cs == connection_fsm_error || state->cs == connection_fsm_first_final;
}

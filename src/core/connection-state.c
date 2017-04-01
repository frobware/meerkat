
#line 1 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.rl"
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


#line 75 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.rl"



#line 42 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.c"
static const char _connection_fsm_actions[] = {
	0, 1, 0, 1, 1, 1, 2
};

static const char _connection_fsm_key_offsets[] = {
	0, 0, 1, 2, 3, 4, 5, 6, 
	7, 8, 9, 10, 11, 12
};

static const char _connection_fsm_trans_keys[] = {
	79, 80, 69, 78, 67, 95, 67, 72, 
	67, 95, 67, 72, 82, 0
};

static const char _connection_fsm_single_lengths[] = {
	0, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1
};

static const char _connection_fsm_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0
};

static const char _connection_fsm_index_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	14, 16, 18, 20, 22, 24
};

static const char _connection_fsm_trans_targs[] = {
	2, 0, 3, 0, 4, 0, 5, 0, 
	6, 0, 7, 0, 8, 0, 9, 0, 
	10, 0, 11, 0, 12, 0, 13, 0, 
	9, 0, 0
};

static const char _connection_fsm_trans_actions[] = {
	1, 3, 0, 3, 0, 3, 0, 3, 
	0, 3, 0, 3, 0, 3, 5, 3, 
	0, 3, 0, 3, 0, 3, 0, 3, 
	0, 3, 0
};

static const char _connection_fsm_eof_actions[] = {
	0, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3
};

static const int connection_fsm_start = 1;
static const int connection_fsm_first_final = 14;
static const int connection_fsm_error = 0;

static const int connection_fsm_en_main = 1;


#line 78 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.rl"

int connection_fsm_init(struct connection_fsm *state)
{
	
#line 103 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.c"
	{
	 state->cs = connection_fsm_start;
	}

#line 82 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.rl"

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

	
#line 142 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.c"
	{
	int _klen, _ps;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if (  state->cs == 0 )
		goto _out;
_resume:
	_keys = _connection_fsm_trans_keys + _connection_fsm_key_offsets[ state->cs];
	_trans = _connection_fsm_index_offsets[ state->cs];

	_klen = _connection_fsm_single_lengths[ state->cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _connection_fsm_range_lengths[ state->cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_ps =  state->cs;
	 state->cs = _connection_fsm_trans_targs[_trans];

	if ( _connection_fsm_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _connection_fsm_actions + _connection_fsm_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 5 "/home/aim/frobware/curly-garbanzo/src/core/connection-state-actions.rl"
	{
	TraceT(begin, (_ps), ( state->cs));
}
	break;
	case 1:
#line 13 "/home/aim/frobware/curly-garbanzo/src/core/connection-state-actions.rl"
	{
	TraceT(error, (_ps), ( state->cs));
}
	break;
	case 2:
#line 21 "/home/aim/frobware/curly-garbanzo/src/core/connection-state-actions.rl"
	{
	TraceT(challenge, (_ps), ( state->cs));
}
	break;
#line 234 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.c"
		}
	}

_again:
	if (  state->cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _connection_fsm_actions + _connection_fsm_eof_actions[ state->cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 1:
#line 13 "/home/aim/frobware/curly-garbanzo/src/core/connection-state-actions.rl"
	{
	TraceT(error, (_ps), ( state->cs));
}
	break;
#line 256 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.c"
		}
	}
	}

	_out: {}
	}

#line 115 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.rl"

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

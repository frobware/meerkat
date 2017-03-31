
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
#include "connection-state.h"
#include "connection.h"

/* Debugging Trace Transitions. */

#if 1
#define TT(A,S,N) printf("%d -> %d : %s, '%c'",S, N, "" # A, *p)
#else
#define TT(A,S,N)
#endif


#line 52 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.rl"



#line 39 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.c"
static const char _connection_fsm_actions[] = {
	0, 1, 0, 1, 1
};

static const char _connection_fsm_key_offsets[] = {
	0, 0, 1, 2, 3
};

static const char _connection_fsm_trans_keys[] = {
	79, 67, 67, 0
};

static const char _connection_fsm_single_lengths[] = {
	0, 1, 1, 1, 0
};

static const char _connection_fsm_range_lengths[] = {
	0, 0, 0, 0, 0
};

static const char _connection_fsm_index_offsets[] = {
	0, 0, 2, 4, 6
};

static const char _connection_fsm_trans_targs[] = {
	2, 0, 3, 0, 4, 0, 0, 0
};

static const char _connection_fsm_trans_actions[] = {
	1, 3, 0, 3, 0, 3, 3, 0
};

static const char _connection_fsm_eof_actions[] = {
	0, 3, 3, 3, 0
};

static const int connection_fsm_start = 1;
static const int connection_fsm_first_final = 4;
static const int connection_fsm_error = 0;

static const int connection_fsm_en_main = 1;


#line 55 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.rl"

int connection_fsm_init(struct connection_fsm *state)
{
	assert(state->next == NULL && "attempt to init an active state");
	state->cs = DSIO_CONNECTION_CLOSED;

	
#line 91 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.c"
	{
	 state->cs = connection_fsm_start;
	}

#line 62 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.rl"

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

	
#line 121 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.c"
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
	TT(begin, (_ps), ( state->cs));
	state->next = NULL;
}
	break;
	case 1:
#line 14 "/home/aim/frobware/curly-garbanzo/src/core/connection-state-actions.rl"
	{
	TT(error, (_ps), ( state->cs)); 
}
	break;
#line 208 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.c"
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
#line 14 "/home/aim/frobware/curly-garbanzo/src/core/connection-state-actions.rl"
	{
	TT(error, (_ps), ( state->cs)); 
}
	break;
#line 230 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.c"
		}
	}
	}

	_out: {}
	}

#line 86 "/home/aim/frobware/curly-garbanzo/src/core/connection-state.rl"

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

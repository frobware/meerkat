#pragma once

/* Synthetic events for the Ragel state machine. The settings here
 * must match the events defined in connection-state.rl or it will all
 * break. */
enum connection_event { 
	EVENT_OPEN   = 'O',
	EVENT_CLOSED = 'C',
	EVENT_MESSAGE = 'M',
};

enum connection_state {
	DSIO_CONNECTION_CLOSED = 0,
	DSIO_CONNECTION_AWAITING_CONNECTION,
	DSIO_CONNECTION_CHALLENGING,
	DSIO_CONNECTION_AWAITING_AUTHENTICATION,
	DSIO_CONNECTION_AUTHENTICATING,
	DSIO_CONNECTION_OPEN,
	DSIO_CONNECTION_ERROR,
	DSIO_CONNECTION_RECONNECTING,
	DSIO_CONNECTION_NR_STATES
};

struct connection_fsm {
	int cs;
	struct connection_fsm_machine *next;
};

extern int connection_fsm_init(struct connection_fsm *state);
extern int connection_fsm_invariant(struct connection_fsm *state, enum connection_event event);
extern int connection_fsm_exec(struct connection_fsm *state, enum connection_event event);
extern int connection_fsm_finish(struct connection_fsm *state);
extern int connection_fsm_done(struct connection_fsm *state);

extern const char *const dsio_connection_fsm_names[DSIO_CONNECTION_NR_STATES];

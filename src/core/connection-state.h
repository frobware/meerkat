#pragma once

enum dsio_connection_state {
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
	struct dsio_client *client;
	enum dsio_connection_state state;
	int cs;
};

extern int connection_fsm_init(struct connection_fsm *state, struct dsio_client *client);
extern int connection_fsm_assert(struct connection_fsm *state, const char *event);
extern int connection_fsm_exec(struct connection_fsm *state, const char *event, size_t len);
extern int connection_fsm_finish(struct connection_fsm *state);
extern int connection_fsm_done(struct connection_fsm *state, const char *event);

extern const char *const dsio_connection_fsm_names[DSIO_CONNECTION_NR_STATES];

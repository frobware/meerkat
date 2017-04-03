%%{

machine connection;

action begin {
	TraceT(begin, fcurs, ftargs);
	connection_state_change(conn, DSIO_CONNECTION_AWAITING_CONNECTION);
}

action reconnecting {
	TraceT(reconnecting, fcurs, ftargs);
	connection_state_change(conn, DSIO_CONNECTION_CHALLENGING);
}

action challenge_response {
	TraceT(challenge_response, fcurs, ftargs);
	connection_state_change(conn, DSIO_CONNECTION_AWAITING_AUTHENTICATION);
	connection_send_challenge_response(conn);
}

action authenticate {
	TraceT(authenticate, fcurs, ftargs);
	connection_state_change(conn, DSIO_CONNECTION_AUTHENTICATING);
	connection_send_auth_response(conn);
}

action open {
	TraceT(open, fcurs, ftargs);
	connection_state_change(conn, DSIO_CONNECTION_OPEN);
}

action close {
	TraceT(close, fcurs, ftargs);
	connection_state_change(conn, DSIO_CONNECTION_CLOSED);
}

action error {
	TraceT(error, fcurs, ftargs);
	connection_state_change(conn, DSIO_CONNECTION_ERROR);
}

action pong {
	TraceT(pong, fcurs, ftargs);
	connection_send_pong_response(conn);
}

}%%

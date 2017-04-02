%%{

machine connection;

action begin {
	TraceT(begin, fcurs, ftargs);
	connection_state_update_client(conn, DSIO_CONNECTION_CLOSED);
}

action open {
	TraceT(open, fcurs, ftargs);
	connection_state_update_client(conn, DSIO_CONNECTION_AWAITING_CONNECTION);
}

action challenge_response {
	TraceT(challenge_response, fcurs, ftargs);
	connection_send_challenge_response(conn);
}

action authenticate {
	TraceT(authenticate, fcurs, ftargs);
	connection_send_auth_response(conn);
}

action error {
	TraceT(error, fcurs, ftargs);
	connection_state_update_client(conn, DSIO_CONNECTION_ERROR);
}

action pong {
	TraceT(pong, fcurs, ftargs);
}

action close {
	TraceT(close, fcurs, ftargs);
	connection_state_update_client(conn, DSIO_CONNECTION_CLOSED);
}

}%%

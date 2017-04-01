%%{

machine connection;

action begin {
	TraceT(begin, fcurs, ftargs);
	connection_state_update_client(conn, DSIO_CONNECTION_AWAITING_CONNECTION);
}

action open {
	TraceT(open, fcurs, ftargs);
	connection_state_update_client(conn, DSIO_CONNECTION_CHALLENGING);
}

action error {
	TraceT(error, fcurs, ftargs);
	connection_state_update_client(conn, DSIO_CONNECTION_ERROR);
}

action pong {
	TraceT(pong, fcurs, ftargs);
}

action challenge_response {
	TraceT(challenge_response, fcurs, ftargs);
	connection_send_challenge_response(conn);
}

action close {
	TraceT(close, fcurs, ftargs);
}

}%%

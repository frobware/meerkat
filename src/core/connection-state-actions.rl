%%{

machine connection_fsm;

action begin {
	state->client->connection.state.state = DSIO_CONNECTION_AWAITING_CONNECTION;
}

action open {
	TraceT(open, fcurs, ftargs);
}

action error {
	TraceT(error, fcurs, ftargs);
}

action pong {
	TraceT(pong, fcurs, ftargs);
}

action challenge_response {
	TraceT(challenge_response, fcurs, ftargs);
}

action close {
	TraceT(close, fcurs, ftargs);
}

}%%

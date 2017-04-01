%%{

machine connection;

action begin {
	TraceT(begin, fcurs, ftargs);
	c->state = DSIO_CONNECTION_AWAITING_CONNECTION;
	if (c->client->cfg->connection_state_change != NULL)
		c->client->cfg->connection_state_change(c->client, c->state);

}

action open {
	TraceT(open, fcurs, ftargs);
	c->state = DSIO_CONNECTION_CHALLENGING;
}

action error {
	TraceT(error, fcurs, ftargs);
	c->state = DSIO_CONNECTION_ERROR;
}

action pong {
	TraceT(pong, fcurs, ftargs);
}

action challenge_response {
	TraceT(challenge_response, fcurs, ftargs);
	connection_send_challenge_response(c);
}

action close {
	TraceT(close, fcurs, ftargs);
}

}%%

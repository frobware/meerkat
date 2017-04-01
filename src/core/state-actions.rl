%%{

machine connection;

action begin {
	TraceT(begin, fcurs, ftargs);
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
	connection_send_challenge_response(c);
}

action close {
	TraceT(close, fcurs, ftargs);
}

}%%

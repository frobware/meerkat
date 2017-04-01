%%{

machine connection_fsm;

action begin {
	TraceT(begin, fcurs, ftargs);
}

action open {
	TraceT(open, fcurs, ftargs);
}

action error {
	TraceT(error, fcurs, ftargs);
}

action ping {
	TraceT(ping, fcurs, ftargs);
}

action challenge {
	TraceT(challenge, fcurs, ftargs);
}

action close {
	TraceT(close, fcurs, ftargs);
	connection_fsm_finish(state);
}

}%%

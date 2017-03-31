%%{

machine connection_fsm;

action begin { 
	TraceT(begin, fcurs, ftargs);
	state->next = NULL;
}

action open {
	TraceT(open, fcurs, ftargs);
}

action error {
	TraceT(error, fcurs, ftargs); 
}

}%%


%%{

machine connection_fsm;

action begin { 
	TT(begin, fcurs, ftargs);
	state->next = NULL;
}

action open {
	TT(open, fcurs, ftargs);
}

action error {
	TT(error, fcurs, ftargs); 
}

}%%


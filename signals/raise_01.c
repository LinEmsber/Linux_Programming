/* An example of raise() */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>


// signal, ANSI C signal handling
//
// sighandler_t signal(int signum, sighandler_t handler);
//
// signal() sets the disposition of the signal signum to handler, which is
// either SIG_IGN, SIG_DFL, or the address of a programmer-defined function
// (a "signal handler").


// int raise(int sig);
//
// The raise() function sends a signal to the calling process or thread.
//
// raise() returns 0 on success, and nonzero for failure.


void signal_handler(int signal)
{
	printf("!! signal caught !!\n");
}

int main()
{

	int ret_signal, ret_raise;

	// Install signal() with SIGINT.
	ret_signal = signal(SIGINT, signal_handler);
	if( ret_signal == SIG_ERR){
		perror("signal");
	}

	printf("Going to raise a signal\n");

	ret_raise = raise(SIGINT);
	if( ret_raise != 0 ){
		perror("raise");
	}

	return 0;
}

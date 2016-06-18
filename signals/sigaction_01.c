/* An example of sigaction() to establish a handler for a SIGINT signal. */

#include <stdio.h>
#include <errno.h>

#include <signal.h>


// int sigaction(int signum, const struct sigaction *act,
// 		struct sigaction *oldact);

// struct sigaction {
//     void     (*sa_handler)(int);
//     void     (*sa_sigaction)(int, siginfo_t *, void *);
//     sigset_t   sa_mask;
//     int        sa_flags;
//     void     (*sa_restorer)(void);
// };

// sa_handler specifies the action to be associated with signum and may be
// SIG_DFL for the default action, SIG_IGN to ignore this signal, or a pointer
// to a signal handling function.  This function receives the signal number as
// its only argument.


void handler(int signum)
{
	// take appropriate actions for signal delivery
	if (signum == SIGINT){

		printf("signum = SIGINT\n");


		// If the signal is again generated while the handler is executing, then it is
		// marked as pending and later delivered when the handler returns. However, it
		// will later be delivered only ONCE. We also already noted that signals are
		// not queued.
		sleep(3);
		printf("sleep(3)\n");
	}


}


int main()
{

	struct sigaction sa;
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;

	if (sigaction(SIGINT, &sa, NULL) == -1){
		perror("Handle error\n");
	}


	for (;;)
		pause();

	return 0;
}

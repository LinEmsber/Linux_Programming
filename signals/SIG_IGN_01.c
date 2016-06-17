/* An example of SIG_IGN */


// compare with following code:
//
//
//	// using the default signal handler
//	raise(SIGTERM);
//
//	// never reached
//	printf("Exit main()\n");



#include <stdio.h>
#include <signal.h>

int main()
{

	// ignoring the signal, SIG_IGN.
	signal(SIGTERM, SIG_IGN);

	// give the process itself a SIGTERM.
	raise(SIGTERM);

	// reache here.
	printf("Exit main()\n");

	return 0;
}

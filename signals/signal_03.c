/* Running a signal handler while modifying a variable */

#include <stdio.h>
#include <signal.h>


// alarm, set an alarm clock for delivery of a signal
//
// unsigned int alarm(unsigned int seconds);
//
// alarm() arranges for a SIGALRM signal to be delivered to the calling
// process in seconds seconds.


struct two_int {
	int a, b;
} data;

void signal_handler(int signum)
{
	printf ("%d, %d\n", data.a, data.b);
}

int main (void){

	struct two_int zeros, ones;

	zeros.a = 0;
	zeros.b = 0;
	ones.a = 1;
	ones.b = 1;


	signal (SIGINT, signal_handler);

	while (1){

		data = zeros;
		data = ones;
	}

	return 0;
}

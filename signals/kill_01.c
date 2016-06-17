/* An example of kill(). */

// kill, send signal to a process
//
// int kill(pid_t pid, int sig);
//
// The kill() system call can be used to send any signal to any process
// group or process.


// EPERM           Operation not permitted
// ESRCH           No such process

#include <stdio.h>
#include <errno.h>
#include <signal.h>

int main()
{

	pid_t pid;
	int errno;

	pid = getpid();

	// kill the caller process itself.
	kill(pid, SIGINT);


	if (errno == EPERM){

		printf("Process exists, but we don't have permission to send it a signal\n");

	}else if (errno == ESRCH){

		printf("Process does not exist\n");

	}else{

		printf("kill");

	}

	return 0;
}

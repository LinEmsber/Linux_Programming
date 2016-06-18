/* An example of sigaction() with signal, SIGCHLD. */

/*
 * When a child process stops or terminates, SIGCHLD is sent to the parent process.
 * The default response to the signal is to ignore it. The signal can be caught and
 * the exit status from the child process can be obtained by immediately calling
 * wait(2). This allows zombie process entries to be removed as quickly as possible.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <signal.h>


// _exit, _Exit - terminate the calling process
//
// void _exit(int status);
//
// The function _exit() terminates the calling process "immediately". Any open file
// descriptors belonging to the process are closed; any children of the process are
// inherited by process 1, init, and the process's parent is sent a SIGCHLD signal.
//
// The value status is returned to the parent process as the process's exit status,
// and can be collected using one of the wait(2) family of calls.



// SIGCHLD handler
static void sigchld_hdl (int sig)
{
	printf("SIGCHLD handler\n");
}

int main (int argc, char *argv[])
{
	int status;
	pid_t pid;
	struct sigaction act;

	memset (&act, 0, sizeof(act));
	act.sa_flags = 0;
	act.sa_handler = sigchld_hdl;

	if (sigaction(SIGCHLD, &act, 0)) {
		perror ("sigaction");
		return 1;
	}

	pid = fork();

	if (pid < 0){

		perror ("fork");

	}else if (pid > 0){

		printf("parent process\n");

		// wait for child.
		waitpid(-1, NULL, 0);

		printf("parent process exit\n");

	}else if (pid == 0){

		printf("child process \n");

		// child process exit, then SIGCHLD is sent to parent process.
		_exit(status);
	}


	return 0;
}

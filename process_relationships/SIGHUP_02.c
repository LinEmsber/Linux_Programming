/* An example of signal SIGHUP */

// Close the working terminal to send the signal, SIGHUP, to all members of process group.
// Usage example: ./a.out d d s s d s d s d > log


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <string.h>
#include <signal.h>


// Handler for SIGHUP
static void handler(int sig)
{
	printf("PID %ld: caught signal %2d (%s)\n", (long) getpid(), sig, strsignal(sig));
}

int main(int argc, char *argv[])
{
	int i;
	pid_t parent_pid, child_pid;
	struct sigaction sa;

	if (argc < 2 || strcmp(argv[1], "--help") == 0){
		printf("%s {d|s}... [ > sig.log 2>&1 ]\n", argv[0]);
	}

	// make stdout unbuffered
	setbuf(stdout, NULL);

	parent_pid = getpid();
	printf("PID of parent process is:       %ld\n", (long) parent_pid);
	printf("Foreground process group ID is: %ld\n", (long) tcgetpgrp(STDIN_FILENO));

	// create child processes
	for (i = 1; i < argc; i++) {

		child_pid = fork();
		if (child_pid == -1){
			perror("fork");
		}

		// child process
		if (child_pid == 0) {


			// letter d, the child process places itself in its own (different) process group.
			if (*argv[i] == 'd'){

				if (setpgid(0, 0) == -1){
					perror("setpgid");
				}

			// letter s, the child remains in the same process group as its parent.
			}else if (*argv[i] == 's'){

			}

			sigemptyset(&sa.sa_mask);
			sa.sa_flags = 0;
			sa.sa_handler = handler;

			if (sigaction(SIGHUP, &sa, NULL) == -1){
				perror("sigaction");
			}

			break;
		}
	}

	// Ensure each process eventually terminates
	alarm(60);

	// print the stauts, PID, PGID.
	printf("PID=%ld PGID=%ld\n", (long) getpid(), (long) getpgrp());

	// wait for termination.
	for (;;){
		pause();
	}

	return 0;
}

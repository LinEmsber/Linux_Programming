/*
 * When the shell receives SIGHUP, it in turn sends SIGHUP to the jobs it has created.
 *
 * The main task of this program is to create a child process, and then have both the
 * parent and the child pause to catch SIGHUP and display a message if it is received.
 * If the program is given an optional command-line argument (which may be any string),
 * the child places itself in a different process group (within the same session).
 *
 * This is useful to show that the shell doesn’t send SIGHUP to a process group that
 * it did not create, even if it is in the same session as the shell.
 */

// Close the working terminal to send the signal, SIGHUP, to all members of process group.
//
// compare the both following results:
//
// case 1:
// $ ./a.out > samegroup
//
// case 2:
// $ ./a.out x > diffgroup

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig)
{
	// printf("%ld: caught SIGHUP\n", (long) getpid());
}

int main(int argc, char *argv[])
{
	int ret_sigaction;
	pid_t pid;
	struct sigaction sa;

	// Make stdout unbuffered
	setbuf(stdout, NULL);

	// set signal
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	ret_sigaction = sigaction(SIGHUP, &sa, NULL);
	if (ret_sigaction == -1)
		perror("sigaction");


	pid = fork();

	if (pid == -1)
		perror("fork");

	// The child process
	if (pid == 0 && argc > 1){

		// move to the new process group.
		if (setpgid(0, 0) == -1){
			perror("setpgid");
		}
	}

	printf("PID=%ld; PPID=%ld; PGID=%ld; SID=%ld\n",
							(long) getpid(),
							(long) getppid(),
							(long) getpgrp(),
							(long) getsid(0));

	// An unhandled SIGALRM ensures this process will die if nothing else terminates it
	// you have 30 secs to close the terminal to get the SIGHUP.
	alarm(30);


	for (;;){

		pause();
		printf("%ld: caught SIGHUP\n", (long) getpid());
	}

	return 0;
}

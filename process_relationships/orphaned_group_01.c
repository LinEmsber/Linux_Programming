/* SIGHUP and orphaned process groups */

// When a controlling process terminates, its terminal becomes free and a new session
// can be established on it. (In fact, another user could log in on the terminal.)
// This could cause a problem if any processes from the old session are still trying
// to use that terminal.

// Execute the following two cases. Check and use command "ps aux" to compare their
// results.
//
// case 1:
// $ ./a.out s
//
// case 2:
// $ ./a.out p

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <signal.h>
#include <fcntl.h>


// int sigemptyset(sigset_t *set);
//
// sigemptyset() initializes the signal set given by set to empty, with all signals
// excluded from the set.
//
// sigemptyset() return 0 on success and -1 on error.


// signal handler
static void handler(int sig)
{
	printf("PID: %ld, caught signal %d (%s)\n", (long) getpid(), sig, strsignal(sig));
}

int main(int argc, char *argv[])
{

	int i;
	struct sigaction sa;

	if (argc < 2 || strcmp(argv[1], "--help") == 0){
		printf("%s {s|p} ...\n", argv[0]);
	}

	// make stdout unbuffered.
	setbuf(stdout, NULL);

	// initialize the signal set, and set struct sigaction.
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;

	// regoster signal_handler as our signal handler for SIGHUP and SIGCONT.
	if (sigaction(SIGHUP, &sa, NULL) == -1){
		perror("sigaction");
	}
	if (sigaction(SIGCONT, &sa, NULL) == -1){
		perror("sigaction");
	}

	printf("parent, PID: %ld, PPID: %ld, PGID: %ld, SID: %ld\n",
							(long) getpid(),
							(long) getppid(),
							(long) getpgrp(),
							(long) getsid(0));

	// create one child for each command-line argumen.
	for (i = 1; i < argc; i++) {
		switch (fork()) {

		// error
		case -1:
			perror("fork");

		// child
		case 0:
			printf("child,  PID: %ld, PPID: %ld, PGID: %ld, SID: %ld\n",
									(long) getpid(),
									(long) getppid(),
									(long) getpgrp(),
									(long) getsid(0));


			// stop via signal
			if (*argv[i] == 's') {

				printf("child,  raise(SIGSTOP)\n");
				printf("PID: %ld, stopping\n", (long) getpid());
				raise(SIGSTOP);

				// The both of child and parent terminate after sleep()(3 secs).

			// wait for signal
			} else {

				// we die if no SIGHUP.
				alarm(60);
				printf("PID: %ld, pausing\n", (long) getpid());
				pause();

				// The parent terminates after 3 secs, but child not.
			}

			_exit(EXIT_SUCCESS);

		// parent carries on round loop
		default:
			break;
		}
	}

	// parent falls through to here after creating all children.

	// give children a chance to start.
	sleep(3);
	printf("parent exiting\n");

	// and orphan children and their group.
	exit(EXIT_SUCCESS);
}

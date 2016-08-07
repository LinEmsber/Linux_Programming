/* An example of tcgetpgrp() */


// tcgetpgrp, tcsetpgrp - get and set terminal foreground process group
//
// pid_t tcgetpgrp(int fd);
//
// int tcsetpgrp(int fd, pid_t pgrp);
//
// The function tcgetpgrp() returns the process group ID of the foreground process group
// on the terminal associated to fd, which must be the controlling terminal of the calling
// process.
//
// The function tcsetpgrp() makes the process group with process group ID pgrp the foreground
// process group on the terminal associated to fd, which must be the controlling terminal
// of the calling process, and still be associated with its session.  Moreover, pgrp must be a
// (nonempty) process group belonging to the same session as the calling process.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>

int main()
{
	pid_t pgid;
	pid_t stdin_PGID;

	pgid = getpgrp();

	// Get the PGIDs for stdin.
	stdin_PGID = tcgetpgrp(STDIN_FILENO);
	printf("The PGID is: %d\n", pgid);

	if (stdin_PGID == -1) {
		printf("Could not get PGID for stdin.n");
		return(EXIT_FAILURE);
	}
	else if (tcsetpgrp(STDIN_FILENO, stdin_PGID) == -1) {
		printf("Could not set PGID\n");
		return(EXIT_FAILURE);
	}

	printf("The PGID has been changed to %d\n", stdin_PGID);
	return(EXIT_SUCCESS);
}

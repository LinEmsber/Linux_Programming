/* An example of tcgetpgrp() */


#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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

/* An example of tcgetpgrp() */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// tcgetpgrp, get terminal foreground process group
//
// pid_t tcgetpgrp(int fd);
//
// The function tcgetpgrp() returns the process group ID of the foreground process
// group on the terminal associated to fd, which must be the controlling terminal
// of the calling process.
//
// When fd refers to the controlling terminal of the calling process, the function
// tcgetpgrp() will return the foreground process group ID of that terminal.

// tcsetpgrp, set terminal foreground process group
//
// int tcsetpgrp(int fd, pid_t pgrp);
//
// The function tcsetpgrp() makes the process group with process group ID, pgrp ,
// the foreground process group on the terminal associated to, fd, which must be
// the controlling terminal of the calling process, and still be associated with
// its session.  Moreover, pgrp must be a process group belonging to the same
// session as the calling process.
//
// When successful, tcsetpgrp() returns 0.

int main(void)
{
	int fd;
	pid_t pgid, pid;
	int ret_tcgetpgrp;

	// get pid, and ppid.
	pid = getpid();
	pgid = getpgrp();

	printf("Before tcgetpgrp(),\npid = %d, pgid = %d\n", pid, pgid);

	// open
	fd = open("/dev/tty", O_RDWR);
	if (fd < 0) {
		perror("open");
	}

	// get the now process group id
	pgid = tcgetpgrp(fd);
	if ( pgid < 0) {
		perror("tcgetpgrpn");
	}

	printf("foreground process group ID = %d\n", pgid);

	// change the process group of the calling terminal.
	ret_tcgetpgrp = tcsetpgrp(fd, pgid);
	if (ret_tcgetpgrp < 0) {
		perror("tcsetpgrp");
	}

	// check the results.
	pgid = tcgetpgrp(fd);
	if (pgid < 0) {
		perror("tcgetpgrp");
	}

	printf("After tcsetpgrp(),\nforeground process group ID = %d\n", pgid);

	return 0;
}

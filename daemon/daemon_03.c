/* An example of creating a daemon */

// $ ./a.out [n seconds]
// $ ps -C a.out -o "pid ppid pgid sid tty command"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// Bit-mask values for 'flags' argument of becomeDaemon()

// Don't chdir("/")
#define BD_NO_CHDIR 		01

// Don't close all open files
#define BD_NO_CLOSE_FILES 	02

// Don't reopen stdin, stdout, and stderr to /dev/null
#define BD_NO_REOPEN_STD_FDS 	04

// Don't do a umask(0)
#define BD_NO_UMASK0 		010

// Maximum file descriptors to close if sysconf(_SC_OPEN_MAX) is indeterminate
#define BD_MAX_CLOSE 		8192

// Returns 0 on success, -1 on error
int becomeDaemon(int flags)
{
	int maxfd, fd;

	switch (fork()) {

		case -1:
			return -1;
		case 0:
			break;

		// while parent terminates
		default:
			_exit(EXIT_SUCCESS);
	}

	// the child process becomes leader of new session.
	if (setsid() == -1)
		return -1;


	// Ensure this child process is not session leader.
	switch (fork()) {

		case -1:
			return -1;
		case 0:
			break;
		default:
			_exit(EXIT_SUCCESS);
	}

	// Clear file mode creation mask
	if (!(flags & BD_NO_UMASK0))
		umask(0);

	// Change to root directory
	if (!(flags & BD_NO_CHDIR))
		chdir("/");

	// Close all open files
	if (!(flags & BD_NO_CLOSE_FILES)) {

		// get configuration information at run time.
		maxfd = sysconf(_SC_OPEN_MAX);

		// If the limit is indeterminate, we take a guess.
		if (maxfd == -1)
			maxfd = BD_MAX_CLOSE;

		for (fd = 0; fd < maxfd; fd++)
			close(fd);
	}

	if (!(flags & BD_NO_REOPEN_STD_FDS)) {
		close(STDIN_FILENO);            /* Reopen standard fd's to /dev/null */

		fd = open("/dev/null", O_RDWR);

		if (fd != STDIN_FILENO)         /* 'fd' should be 0 */
			return -1;
		if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
			return -1;
		if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
			return -1;
	}

	return 0;
}


int main(int argc, char *argv[])
{

	becomeDaemon(0);

	/* Normally a daemon would live forever; we just sleep for a while */
	sleep( (argc > 1) ? atoi(argv[1]) : 20 );

	exit(EXIT_SUCCESS);
}

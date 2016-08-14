/* An example of poll() */

#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#define TIMEOUT 3

int main()
{
	int ret;
	struct pollfd fds[2];

	// monitor stdin for input
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;

	// monitor stdout for ability to write
	fds[1].fd = STDOUT_FILENO;
	fds[1].events = POLLOUT;

	// use poll() to simultaneously check whether a read from stdin and
	// a write to stdout will block.
	ret = poll(fds, 2, TIMEOUT);
	if (ret == -1){
		perror("poll");
		return 1;
	}

	if (!ret){
		printf("%d seconds elapsed.\n", TIMEOUT);
		return 0;
	}

	// there is data to read.
	if (fds[0].revents & POLLIN){
		printf("stdin is readable\n");
	}

	// writing will not block.
	if (fds[1].revents & POLLOUT){
		printf("stdout is writable\n");
	}

	return 0;
}

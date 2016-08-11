/* The sample demonstration of the use of poll() */


// int poll(struct pollfd *fds, nfds_t nfds, int timeout);
//
// poll() performs a similar task to select(2): it waits for one of a set of file
// descriptors to become ready to perform I/O.
//
// The set of file descriptors to be monitored is specified in the fds
// argument, which is an array of structures of the following form:
//
// struct pollfd {
//     int   fd;         /* file descriptor */
//     short events;     /* requested events */
//     short revents;    /* returned events */
// };


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <time.h>
#include <poll.h>

int main(int argc, char *argv[])
{

	int i;
	int num_pipes, ready, rand_pipe, num_writes;

	// struct pollfd array represents how many event to monitor.
	struct pollfd *poll_fd;

	// file descriptors for pipes.
	int (*pipe_fd)[2];

	// argv[1]: the number of pipes
	// argv[2]: the number of pipes select randomly to be written.
	// argc error handling
	if (argc < 2 || strcmp(argv[1], "--help") == 0){
		printf("%s [number of pipe] [random number of pipe to be written]\n", argv[0]);
	}

	// pipe numbers
	num_pipes = atoi(argv[1]);

	// allocate memory for pipe fds
	pipe_fd = calloc(num_pipes, sizeof(int [2]));
	if (pipe_fd == NULL){
		perror("calloc");
	}

	// allocate memory for poll fds
	poll_fd = calloc(num_pipes, sizeof(struct pollfd));
	if (poll_fd == NULL)
		printf("calloc");

	// Create the number of pipes specified on command line
	for (i = 0; i < num_pipes; i++){

		// create pipe.
		if (pipe(pipe_fd[i]) == -1){
			perror("pipe");
		}
	}


	// perform specified number of writes to random pipes
	num_writes = (argc > 2) ? atoi(argv[2]) : 1;

	srandom((int) time(NULL));
	for (i = 0; i < num_writes; i++) {

		rand_pipe = random() % num_pipes;
		printf("Writing to fd: %3d (read fd: %3d)\n", pipe_fd[rand_pipe][1], pipe_fd[rand_pipe][0]);

		if (write(pipe_fd[rand_pipe][1], "a", 1) == -1){
			perror("write");
		}
	}


	// Build the file descriptor list to be supplied to poll().
	// This list is set to contain the file descriptors for the read ends of all of the pipes.
	for (i = 0; i < num_pipes; i++) {
		poll_fd[i].fd = pipe_fd[i][0];
		poll_fd[i].events = POLLIN;
	}

	ready = poll(poll_fd, num_pipes, 0);
	if (ready == -1){
		perror("poll");
	}


	printf("poll() returned: %d\n", ready);

	/* Check which pipes have data available for reading */

	for (i = 0; i < num_pipes; i++){
		if (poll_fd[i].revents & POLLIN){
			printf("Readable: %3d\n", poll_fd[i].fd);
		}
	}


	exit(EXIT_SUCCESS);
}

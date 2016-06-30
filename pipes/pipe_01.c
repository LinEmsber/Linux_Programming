/* An example of pipe() */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


// int pipe(int pipefd[2]);
//
// pipe() creates a pipe, a unidirectional data channel that can be used for IPC.
// The array pipefd is used to return two file descriptors referring to the ends of
// the pipe.  pipefd[0] refers to the read end of the pipe. pipefd[1] refers to the
// write end of the pip
//
// On success, zero is returned.


// int dup2(int oldfd, int newfd);
//
// The dup2() system call performs the same task as dup(), but instead of using the
// lowest-numbered unused file descriptor, it uses the file descriptor number specified
// in newfd.  If the file descriptor newfd was previously open, it is silently
// closed before being reused.
//
// On success, these system calls return the new file descriptor.


int main()
{
	int fd[2];
	pid_t childpid;

	pipe(fd);

	if((childpid = fork()) == -1)	{

		perror("fork");
		exit(1);

	}else if(childpid == 0){


		// Child process closes up input side of pipe
		close(fd[0]);
		dup2(0, fd[0]);

	}else{

		// Parent process closes up output side of pipe
		close(fd[1]);

	}

}

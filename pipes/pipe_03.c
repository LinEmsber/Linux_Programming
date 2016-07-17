/* An example of pipe() */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define MAX_LINE 256;

int main()
{
	int nr;
	int fd[2];
	pid_t pid;
	char line[MAX_LINE];


	// create a pipe
	if (pipe(fd) < 0){
		perror("pipe");
	}

	// fork
	if ( ( pid = fork) < 0 ){

		perror("fork");

	}else if (pid > 0){

		close(fd[0]);
		write(fd[1], "hello world\n", 12);

	}else{

		close(fd[1]);
		nr = read(fd[0], line, MAX_LINE);
		write(STDOUT_FILENO, line, nr);
	}

	exit(0);
}

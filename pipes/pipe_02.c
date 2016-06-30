/* An example of pipe() */

// parent process write message to child process, and child process
// read the message.


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Read characters from the pipe and echo them to STDOUT.
void read_from_parent_pipe (int file)
{
	int c;
	FILE *stream;
	stream = fdopen (file, "r");

	while ((c = fgetc (stream)) != EOF){
		putchar (c);
	}

	fclose (stream);
}


// Write some random text to the pipe.
void write_to_child_pipe (int file)
{
	FILE *stream;
	stream = fdopen (file, "w");

	fprintf (stream, "Hi~\n");
	fprintf (stream, "The message from parent process.\n");

	fclose (stream);
}

int main (void)
{
	pid_t pid;
	int fd[2];

	// create pipe.
	// If successful, pipe returns a value of 0.
	if (pipe (fd)){
		perror("Pipe");
		return EXIT_FAILURE;
	}

	pid = fork ();

	// error
	if (pid < (pid_t) 0){

		perror("fork");
		return EXIT_FAILURE;

	// child
	}else if (pid ==  0){

		// This is the child process.Close other end first.
		close (fd[1]);

		read_from_parent_pipe (fd[0]);

		return EXIT_SUCCESS;

	// parent
	}else{
		// This is the parent process. Close other end first.
		close (fd[0]);

		write_to_child_pipe (fd[1]);

		return EXIT_SUCCESS;
	}

	return 0;
}

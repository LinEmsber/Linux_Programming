/* The practice of execv() with fork() */


// Usage: ./a.out argument_1 argument_2


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// execv - execute a file
//
// int execv(const char *path, char *const argv[]);
//
// The execv(), execvp(), and execvpe() functions provide an array of pointers
// to null-terminated strings that represent the argument list available to the
// new program. The array of pointers must be terminated by a null pointer.


// _exit, _Exit - terminate the calling process
//
// The function _exit() terminates the calling process "immediately".
// Any open file descriptors belonging to the process are closed; any
// children of the process are inherited by process 1, init, and the
// process's parent is sent a SIGCHLD signal.
//
// The value status is returned to the parent process as the process's
// exit status, and can be collected using one of the wait(2) family of
// calls.
//
// The function _Exit() is equivalent to _exit().


int main(int argc, char **argv)
{

	int w;
	pid_t cpid;

	int wait_status;
	int exit_status;


	cpid = fork();
	char *const parament_list_1[] = {"./print_out_argument", argv[1], NULL};
	char *const parament_list_2[] = {"./print_out_argument", argv[2], NULL};


	// parent process
	if (cpid > 0){

		// wait for child process to terminate first, and then execute a file.
		w = waitpid(cpid, &wait_status, WUNTRACED | WCONTINUED);

		execv("./print_out_argument", parament_list_1);
		exit(EXIT_SUCCESS);

	// child process
	}else if (cpid == 0){

		execv("./print_out_argument", parament_list_2);
		_exit(exit_status);

	// error
	}else{

		perror("fork failed");
		exit(EXIT_FAILURE);

	}

	return 0;
}

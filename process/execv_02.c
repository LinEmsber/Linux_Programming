/* An example of execv() part 2 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// On success, the system call fork_execv() returns 0 and 1 for parent and child processes.
//
// On error, -1 is returned.
int fork_execv(char *path, char *const argv[])
{

	int wait_status;
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1){
		return -1;
	}


	// parent process
	if (child_pid > 0) {

		// waiting for the child process
		wait(&wait_status);
		return 1;

	// child process
	// The child process would be replaced by the another process image.
	}else if (child_pid == 0){

		execv(path, argv);
		return 0;
	}

}

int main(int argc, char **argv)
{
	int return_fork_1;
	int return_fork_2;

	char *const parament_list_1[] = {"./print_out_argument", argv[1], NULL};
	char *const parament_list_2[] = {"./print_out_argument", argv[2], NULL};

	return_fork_1 = fork_execv("print_out_argument", parament_list_1);
	if (return_fork_1 < 0){
		perror("fork_execv");
	}


	return_fork_2 = fork_execv("print_out_argument", parament_list_2);
	if (return_fork_2 < 0){
		perror("fork_execv");
	}

	return 0;
}

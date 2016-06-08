/* An example of fork() */

/*
 * The parent process do not wait child process, so child process
 * becomes the zombie, the child's PPID becomes init or init-user
 */

#include <stdio.h>

int main(){

	int pid;

	// Duplicate. Child and parent continue from here.
	printf("===============fork()===============\n");
	pid = fork();

	// pid is non zero, so it must is the parent
	if (pid > 0){

		printf("===============parent process===============\n");
		printf("The parent process with PID: %d, PPID: %d.\n",	getpid(), getppid());
		printf("New child's PID is %d.\n", pid);
		printf("===============parent process===============\n");


	// pid is zero, so it must is the child.
	}else if ( pid == 0){

		printf("===============child process===============\n");
		// Make sure that the parent terminates first.
		sleep(3);
		printf("The child process with PID: %d, PPID: %d.\n", getpid(), getppid());
		system("ps aux | grep init");
		printf("===============child process===============\n");

	// error
	}else{
		printf("error");

	}

	return 0;
}

/* An example of wait() */

#include <stdio.h>
#include <stdlib.h>

main()
{
	int pid;
	int w;
	int status;

	// Duplicate. Child and parent continue from here.
	pid = fork();
	printf("RUN: pid=fork(), pid: %d\n", pid);

	// pid is non zero, so it must is the parent
	if (pid > 0){

		printf("===============parent process===============\n");
		printf("The parent process with PID: %d, PPID: %d.\n",	getpid(), getppid());
		printf("New child's PID is %d.\n", pid);
		w = wait(&status);
		printf("Waiting for child\n");
		printf("===============parent process===============\n");


	// pid is zero, so it must is the child.
	}else if ( pid == 0 ){

		printf("===============child process===============\n");
		printf("The child process with PID: %d, PPID: %d.\n", getpid(), getppid());
		printf("===============child process===============\n");

	// error happen, no child process is created.
	} else{

		printf("error");
	}

	printf("\nPID: %d terminates.\n", pid);

	return 0;
}

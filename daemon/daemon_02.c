/* An implementation of simple daemon */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {

	int i;
	FILE *fp = NULL;
	pid_t pid = 0;
	pid_t sid = 0;


	// Create child process
	pid = fork();

	// error
	if (pid < 0) {

		perror("fork");
		exit(1);

	// child process
	}else if (pid == 0){

		printf("in the child, pid: %d\n", pid);

	// parent process
	}else if (pid > 0){

		printf("parent process exit");
		exit(EXIT_SUCCESS);
	}

	//unmask the file mode
	umask(0);


	// The child process inherited parent's process group, and process group
	// leader will still is parent process.
	//
	// We want to create child's own process group and let it become process
	// group leader.

	//set new session
	sid = setsid();
	if(sid < 0){
		exit(1);
	}

	// Change the current working directory to root.
	chdir("/home/et548548/Linux_Programming/daemon/");



	// We shold cholse STDIN, STDOUT, and STDERR. Because daemon doesnt interact
	// with the user directly. This operation can save system resources, and
	// prevent any related security problems.

	// Close stdin. stdout and stderr
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	// Open a log file in write mode.
	fp = fopen ("Log_another_daemon.txt", "w+");
	if (fp == NULL){
		perror("Error");
	}

	for ( i = 0; i < 10; i++){
		
		// write the time to the file.
		char string[32];
		time_t tt = time(NULL);
		struct tm *tm = localtime(&tt);

		strftime(string, sizeof(string), "%c", tm);
		fprintf(fp, "%s\n", string);

		sleep(5);
		fflush(fp);
	}

	fclose(fp);
	return 0;
}

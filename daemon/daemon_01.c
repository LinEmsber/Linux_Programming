/* An sample of daemon */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#define DAEMON_NAME "new_daemon"

void process(){
	syslog (LOG_NOTICE, "Writing to Syslog");
}

int main(int argc, char *argv[]) {

	// To set our logging mask and open the log.
	//
	// int setlogmask(int mask);
	// mask:
	// LOG_NOTICE, normal, but significant, condition
	setlogmask( LOG_UPTO(LOG_NOTICE) );

	// send messages to the system logger.
	//
	// void openlog(const char *ident, int option, int facility);
	// The option argument specifies flags which control the operation of openlog().
	// The facility argument establishes a default to be used if none is specified
	// in subsequent calls to syslog().
	openlog(DAEMON_NAME,  LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
	syslog(LOG_INFO, "Entering Daemon");


	pid_t pid, sid;

	// To fork a parent rocess.
	// On success, the PID of the child process is returned in the parent,
	// and 0 is returned in the child.
	pid = fork();

	// error
	if (pid < 0) {

		printf("error happen\n");
		exit(EXIT_FAILURE);

	// child process
	}else if (pid == 0){

		printf("in the child, pid: %d\n", pid);

	// parent process
	}else if (pid > 0) {

		printf("in the parent, pid: %d\n", pid);
		exit(EXIT_SUCCESS);

	}

	// To change file mask.
	umask(0);

	// To create a new session for our child.
	sid = setsid();
	if (sid < 0) {
		 exit(EXIT_FAILURE);
	}

	// To change directory.
	ret_chdir = chdir("~/");
	if (ret_chdir < 0) {
		 exit(EXIT_FAILURE);
	}

	// To close standard file descriptors.
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	// The daemon is starting.
	while(1){

		process();    // run our process
		sleep(3);

	}

	// To close the log.
	closelog ();
}

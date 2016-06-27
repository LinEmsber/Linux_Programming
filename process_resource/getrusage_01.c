/* An example of getrusage() */

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

int main() {

	int i, j, k;
	int max_fd;
	struct rusage usage;
	struct timeval user_time, system_time;


	// do something to waste time
	k = 0;
	for (i = 0; i < 10000; i++) {

		for (j = 0; j < 10000; j++) {
			k += 20;
		}
	}

	max_fd = sysconf(_SC_OPEN_MAX);


	// user mode time
	getrusage(RUSAGE_SELF, &usage);
	user_time = usage.ru_utime;

	// kernel mode time
	getrusage(RUSAGE_SELF, &usage);
	system_time = usage.ru_stime;


	printf("user_time: %ld.%lds\n", user_time.tv_sec, user_time.tv_usec);
	printf("system_time: %ld.%lds\n", system_time.tv_sec, system_time.tv_usec);

	return 0;
}

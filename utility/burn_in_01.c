/* simple CPU burn-in */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void* compute(void* arg)
{
	do {} while(1);
	return NULL;
}

int main(int argc, char* argv[])
{
	int t = 0;
	int prime, num_threads;

	if(argc != 2) {
		printf("Error: wrong number of command-line arguments\n");
		printf("Usage: %s <integer>\n", argv[0]);
		exit(1);
	}

	num_threads = atoi(argv[1]);

	// thread ID
	pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
	printf("Number of threads: %d\n\n", num_threads);

	// create threads
	for(t = 0; t < num_threads; ++t)
		pthread_create(&threads[t], NULL, &compute, NULL);

	// waits for the thread specified by thread ID to terminate.
	for(t = 0; t < num_threads; ++t)
		pthread_join(threads[t], (void*) NULL);

	return 0;
}

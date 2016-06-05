/* A simple implementation of pthread_mutex  */

/*
 * Add and subtraction of the global variable avail in multi-threads,
 * and using mutex to make sure that the race condition would not
 * happen.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int avail = 0;

void *fun(void *arg)
{
	int i;
	int counter = atoi( arg);

	for ( i=0; i< counter; i++){
		// sleep(1);

		pthread_mutex_lock(&mutex);
		printf("%d\n", i);
		avail++;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main( int argc, char *argv[])
{

	int i;
	pthread_t tid;
	int total_requested;
	int unit;
	int token;

	// adding of natural number
	total_requested = 0;
	for (i=0; i<argc; i++){
		total_requested = total_requested + atoi(argv[i]);
		pthread_create(&tid, NULL, fun, argv[i]);
	}

	unit = 0;
	token = 0;
	for (;;){
		pthread_mutex_lock(&mutex);

		while( avail>0 ){
			unit++;
			avail--;
			printf("unit=%d\n", unit);
			token = (unit >= total_requested)? 1:0;
		}

		pthread_mutex_unlock(&mutex);

		if(token)
			break;
	}
	return 0;
}

/*
 * Shows how to use thread cleanup handlers.
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <pthread.h>

void cleanup(void *arg)
{
	printf("clearnup: %s\n", (char *)arg);
}

void *thread_fun_1(void *arg)
{
	printf("thread 1 start\n");

	pthraed_cleanup_push(cleanup, "thread 1 first handler");
	pthraed_cleanup_push(cleanup, "thread 1 second handler");

	printf("thread 1 push complete\n");

	if (arg)
		return ((void *) 1);


	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

	return ((void *) 1);
}

void *thread_fun_2(void *arg)
{
	printf("thread 2 start\n");

	pthraed_cleanup_push(cleanup, "thread 2 first handler");
	pthraed_cleanup_push(cleanup, "thread 2 second handler");

	printf("thread 2 push complete\n");

	if (arg)
		pthread_exit( (void *) 2 );
	

	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

	return ((void *) 2);
}

int main()
{
	int err;
	pthread_t tid_1, tid_2;
	void *ret;

	// thread 1
	err = pthread_create(&tid_1, NULL, thread_fun_1, (void *) 1);
	if (err != 0){
		perror("pthread_create");
	}

	// thread 2
	err = pthread_create(&tid_1, NULL, thread_fun_1, (void *) 1);
	if (err != 0){
		perror("pthread_create");
	}

	err = pthread_join(tid_1, &ret);
	if (err != 0){
		perror("pthread_join");
	}

	err = pthread_join(tid_1, &ret);
	if (err != 0){
		perror("pthread_join");
	}


	printf("thread 2 exit code %ld\n", (long)ret);

	return 0;
}

/* Using pointers to obtain the global variables which are returned from the threads */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

pthread_t tid[2];
int ret_1, ret_2;

void * thread_function(void *arg)
{
	unsigned long i = 0;
	pthread_t id = pthread_self();

	/* wait some time */
	for(i = 0; i < 0xFFFFFFFF; i++);

	/* select thread */
	if( pthread_equal(id, tid[0]) ) {
		printf("First thread processing done\n");
		ret_1  = 100;
		pthread_exit(&ret_1);

	} else {
		printf("Second thread processing done\n");
		ret_2  = 200;
		pthread_exit(&ret_2);

	}

	return NULL;
}

int main(void)
{
	int i = 0;
	int err;
	int * ptr_ret[2];

	for(i = 0; i < 2; i++) {

		err = pthread_create( &(tid[i]), NULL, &thread_function, NULL );
		if (err != 0)
			printf("can't create thread :[%s]", strerror(err));
		else
			printf("Thread created successfully\n");

	}

	pthread_join(tid[0], (void**) &(ptr_ret[0]) );
	pthread_join(tid[1], (void**) &(ptr_ret[1]) );

	printf("return value from first thread: %d\n", *ptr_ret[0]);
	printf("return value from second thread: %d\n", *ptr_ret[1]);

	return 0;
}

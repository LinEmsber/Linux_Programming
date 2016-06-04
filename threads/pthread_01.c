/* An example of pthread */


#include <stdio.h>
#include <errno.h>
#include <pthread.h>

// this function is run by the second thread
void *increase_num(void *num_ptr)
{
	int *x_ptr = (int *)num_ptr;

	while( ++(*x_ptr) < 100 );
	printf("x increament finished\n");
}

int main()
{
	int x = 0;
	int y = 0;
	pthread_t new_thread;
	int ret_pthread_create, ret_pthread_join;

	printf("origin:\nx: %d, y: %d\n", x, y);


	// int pthread_create(pthread_t *thread,
	// 			const pthread_attr_t *attr,
	// 			void *(*start_routine) (void *),
	// 			void *arg);
	//
	// On success, pthread_create() returns 0;

	ret_pthread_create = pthread_create(&new_thread, NULL, increase_num, &x);
	if ( ret_pthread_create != 0){
		perror("pthread_create");
	}


	while(++y < 100);
	printf("y increament  finished\n");


	// pthread_join - join with a terminated thread
	// int pthread_join(pthread_t thread, void **retval);
	//
	// On success, pthread_join() returns 0
	ret_pthread_join = pthread_join(new_thread, NULL);
	if( ret_pthread_join != 0){
		perror("pthread_join");
	}

	printf("final:\nx: %d, y: %d\n", x, y);

	return 0;
}

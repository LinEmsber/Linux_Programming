/* An example of pthread */
/* 
   pthread_create - create a new thread
   int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

   pthread_join - join with a terminated thread
   int pthread_join(pthread_t thread, void **retval);
 */

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

// this function is run by the second thread
void * increase_num(void *num_ptr)
{
	int * x_ptr = (int *) num_ptr;

	while( *x_ptr < 100 ){
		(*x_ptr)++;
	}

	printf("x increament finished\n");

	return 0;
}

int main()
{
	int ret;
	pthread_t tid;
	int x = 0;
	int y = 0;

	printf("x: %d, y: %d\n", x, y);

	ret = pthread_create(&tid, NULL, increase_num, &x);
	if ( ret != 0){
		perror("pthread_create");
	}

	while(y < 100){
		y++;
	}
	printf("y increament finished\n");

	ret = pthread_join(tid, NULL);
	if( ret != 0){
		perror("pthread_join");
	}

	printf("final x: %d, y: %d\n", x, y);

	return 0;
}

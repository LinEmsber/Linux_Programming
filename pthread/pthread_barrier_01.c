/* The example of the pthread_barrier_t */
/* pthread_barrier_wait — synchronize at a barrier
   int pthread_barrier_wait(pthread_barrier_t *barrier);

   pthread_barrier_destroy, pthread_barrier_init — destroy and initialize a barrier object
   int pthread_barrier_destroy(pthread_barrier_t *barrier);
   int pthread_barrier_init(pthread_barrier_t *restrict barrier, const pthread_barrierattr_t *restrict attr, unsigned count);
 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define THREAD_NUMBER 4

pthread_barrier_t barrier;

void * thread(void * arg)
{
	time_t now;
	char buf [27];
	int i = *( (int *) arg);

	time(&now);
	printf("Time: %.19s, [thread] ready to work!\n", ctime_r(&now, buf));

	// The different thread with different sleep time.
	usleep( 1000000 * i);

	// When a thread calls pthread_barrier_wait(), it will block until the number of 
	// threads specified initially in the pthread_barrier_init() .
	pthread_barrier_wait(&barrier);

	time(&now);
	printf("Time: %.19s, [thread]: worke completed.\n", ctime_r(&now, buf));

	return NULL;
}

int main(int argc, char **argv)
{
	int i;
	int arg;
	time_t now;
	char buf [27];

	pthread_t pid[THREAD_NUMBER];

	time(&now);
	pthread_barrier_init( &barrier, NULL, THREAD_NUMBER );
	printf("Time: %.19s, [main]: pthread_barrier_init done.\n", ctime_r(&now, buf));

	for (i = 0; i < THREAD_NUMBER; i++){
		arg = i;
		pthread_create( &pid[i], NULL, &thread, (void *) &arg );
	}

	for (i = 0; i < THREAD_NUMBER; i++){
		pthread_join(pid[i], NULL);
	}

	time(&now);
	printf("Time: %.19s, [main]: pthread well done.\n", ctime_r(&now, buf));
	pthread_barrier_destroy(&barrier);

	return 0;
}
